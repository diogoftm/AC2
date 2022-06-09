#include <detpic32.h>

volatile char c = 0;
volatile int voltage = 0;

typedef struct { 
    char mem[16]; // Storage area 
    int nchar;   // Number of characters to be transmitted 
    int posrd;   // Position of the next character to be transmitted 
} t_buf;

volatile t_buf txbuf;

void putcBuf(char c){
    txbuf.mem[txbuf.nchar]=c;
    txbuf.nchar++;
    txbuf.posrd=0;
}

void set_OC1RS(int p){
    OC1RS = (39061+1)*p/100;
}

void _int_(27) isr_ADC(void){
    int* d = (int *)&ADC1BUF0;
    int i = 0;
    voltage = 0;
    for(;i<4 ;i++){
        voltage += d[i*4];
    }
    voltage /= 4;
    voltage = (voltage*33+511)/1023; 
    char l = (voltage%10)+'0';
    char h = (voltage/10)+48;
    putcBuf(h);
    putcBuf(l);
    set_OC1RS(voltage*100/33);
    IEC1bits.U2TXIE = 1;
    IFS1bits.AD1IF = 0;
}

void _int_(8) isr_T2(void){
    if(c==0) c++;
    else{
        AD1CON1bits.ASAM = 1;
        c=0;
    }
    IFS0bits.T2IF = 0;
}

void _int_(32) isr_UART(void){
    if(IFS1bits.U2RXIF==1){
        IFS1bits.U2RXIF=0;
    }
    else if(IFS1bits.U2TXIF==1){
        if(txbuf.nchar>0){ 
            U2TXREG = txbuf.mem[txbuf.posrd];
            txbuf.posrd++;
            txbuf.nchar--;   
        } else{
            txbuf.posrd=0;
            IEC1bits.U2TXIE = 0;
        }
        IFS1bits.U2TXIF=0;
    }
}

void ADCconf(void){
    TRISBbits.TRISB4 = 1; // RBx digital output disconnected 
    AD1PCFGbits.PCFG4= 0; // RBx configured as analog input (AN4) 
    AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this 
            //  mode an internal counter ends sampling and 
            //  starts conversion 
    AD1CON1bits.CLRASAM = 1;  // Stop conversions when the 1st A/D converter 
            //  interrupt is generated. At the same time, 
            //  hardware clears the ASAM bit 
    AD1CON3bits.SAMC = 16;    // Sample time is 16 TAD (TAD = 100 ns) 
    AD1CON2bits.SMPI = 4-1;  // Interrupt is generated after XX samples 
            //  (replace XX by the desired number of 
            //  consecutive samples) 
    AD1CHSbits.CH0SA = 4;   // replace x by the desired input  
            //  analog channel (0 to 15) 
    AD1CON1bits.ON = 1;     // Enable A/D converter 
            //  This must the last command of the A/D 
            //  configuration sequence 
    IFS1bits.AD1IF = 0;
    IPC6bits.AD1IP = 2;
    IEC1bits.AD1IE = 1;

}

void UARTconf(void){
    //configuração da UART 2
    U2BRG = 10;
    U2MODEbits.BRGH = 0; //divisão por 16

    U2MODEbits.PDSEL = 0; //00 = 8-bit data, no parity
    U2MODEbits.STSEL = 0; //0 = 1 Stop bit

    U2STAbits.UTXEN = 1; //enable transmiter
    U2STAbits.URXEN = 1; //enable receiver

    U2MODEbits.ON = 1; //enable UART

    //configuração das interrupções da UART
    IPC8bits.U2IP = 2; //set priority
    IFS1bits.U2RXIF = 0; //reset RX interrupt flag
    IFS1bits.U2TXIF = 0; //reset TX interrupt flag
    IEC1bits.U2RXIE = 1; //disable interrupts for RX
    IEC1bits.U2TXIE = 0; //disable interrupts for TX

    U2STAbits.URXISEL = 0; //UART receiver interrupt mode
    U2STAbits.UTXISEL = 0; //UART transmiter interrupt mode
}

void timerConf(void){
    T2CONbits.TCKPS = 7;
    PR2 = 39061;
    TMR2 = 0;
    IPC2bits.T2IP = 2;
    IEC0bits.T2IE = 1;
    IFS0bits.T2IF = 0;
    T2CONbits.ON = 1;

    OC1CONbits.OCM = 6;  // PWM mode on OCx; fault pin disabled 
    OC1CONbits.OCTSEL =0;// Use timer T2 as the time base for PWM generation 
    OC1RS = 12500;   // Ton constant 
    OC1CONbits.ON = 1; // Enable OC1 module
}

int main(void){
    timerConf();
    ADCconf();
    UARTconf();
    TRISEbits.TRISE0 = 0;
    EnableInterrupts();
    while(1){
        LATEbits.LATE0 = PORTDbits.RD0;
    }
    return 0;
}