#include <detpic32.h>

typedef struct { 
    char mem[16]; // Storage area 
    int nchar;   // Number of characters to be transmitted 
    int posrd;   // Position of the next character to be transmitted 
} t_buf;

volatile t_buf txbuf;
volatile int counter = 0;

void putcInt(char s){
    if(txbuf.nchar==16){
        txbuf.nchar=12;
    }
    
    txbuf.mem[txbuf.nchar] = s;
    txbuf.nchar++;
    
    txbuf.posrd=0;
}

void _int_(32) isr_uart2(void){
    if(IFS1bits.U2RXIF==1){
        char r = U2RXREG;
        if(r=='D'){
            IEC1bits.U2TXIE = 1;
        }
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

void configureUART(void){
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

void _int_(4) isr_TIMER1(void){
    if(counter==2){
        putcInt(PORTBbits.RB3 + 48);
        putcInt(PORTBbits.RB2 + 48);
        putcInt(PORTBbits.RB1 + 48);
        putcInt(PORTBbits.RB0 + 48);
        counter = 0;
    } else{
        counter++;
    }
        IFS0bits.T1IF = 0;
}

void configTIMERS(void){
    T1CONbits.TCKPS = 3;
    PR1 = 39061;
    TMR1 = 0;
     
    IFS0bits.T1IF = 0;
    IPC1bits.T1IP = 2;
    IEC0bits.T1IE = 1;
    T1CONbits.ON = 1;
}

int main(void){
    configTIMERS();
    configureUART();
    EnableInterrupts();
    TRISB = TRISB | 0x000F;
    while(1);
    return 0;
}