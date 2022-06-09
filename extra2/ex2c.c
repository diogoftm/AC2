#include <detpic32.h>

volatile int counter = 0;
volatile int voltage = 0;

void display(unsigned char s){
    static char values[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char flag = 0;

    if(flag==0){
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x80FF)|((values[s%10])<<8);
    }else{
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x80FF)|((values[s/10])<<8);
    }

    flag = flag ^ 1;
}

void _int_(8) isr_T2(void){
    display(counter);
    IFS0bits.T2IF = 0;
}

void _int_(4) isr_T1(void){
    if(counter==99){
        counter = 0;
    } else{
        counter++;
    }
    IFS0bits.T1IF = 0;
}

void _int_(27) isr_ADC1(void){
    int *d = (int *)(&ADC1BUF0);
    int i = 0;
    for(;i<4 ;i++){
        voltage += d[i*4];
    }
    voltage /= 4;
    PR1 = (20000000/256)/(2*(1+voltage/127))-1;
    IFS1bits.AD1IF = 0;
}

void configTimers(void){
    T1CONbits.TCKPS = 3;
    PR1 = 31249;
    TMR1 = 0;

    IFS0bits.T1IF = 0;
    IPC1bits.T1IP = 2;
    IEC0bits.T1IE = 1;
    T1CONbits.ON = 1; 

    T2CONbits.TCKPS = 3;
    PR2 = 49999;
    TMR2 = 0;
    
    IFS0bits.T2IF = 0;
    IPC2bits.T2IP = 2;
    IEC0bits.T2IE = 1;
    T2CONbits.ON = 1;
}

void configADC(void){
    TRISBbits.TRISB4 = 1;      // RB4 digital output disconnected 
    AD1PCFGbits.PCFG4 = 0;     // RB4 configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;      // Conversion trigger selection bits: in this 
                               //  mode an internal counter ends sampling and 
                               //  starts conversion 

    AD1CON1bits.CLRASAM = 1;   // Stop conversions when the 1st A/D converter 
                               //  interrupt is generated. At the same time, 
                               //  hardware clears the ASAM bit

    AD1CON3bits.SAMC = 16;    // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 3;      // Interrupt is generated after 4 sample 

    AD1CHSbits.CH0SA = 4;      // replace x(4) by the desired input  
                               //  analog channel (0 to 15)

    AD1CON1bits.ON = 1;        // Enable A/D converter 
                               //  This must the last command of the A/D 
                               //  configuration sequence
    IFS1bits.AD1IF = 0;
    IPC6bits.AD1IP = 2;
    IEC1bits.AD1IE = 1;
}

int main(void){
    configTimers();
    configADC();
    EnableInterrupts();
    TRISB = TRISB & 0x80FF;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    while(1){
        resetCoreTimer();
        while(readCoreTimer()<20000*250);
        AD1CON1bits.ASAM=1;
    }
    return 0;
}