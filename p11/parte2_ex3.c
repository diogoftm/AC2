#include <detpic32.h>

typedef struct { 
    char mem[100]; // Storage area 
    int nchar;   // Number of characters to be transmitted 
    int posrd;   // Position of the next character to be transmitted 
} t_buf;

volatile t_buf txbuf;

void putstrInt(char *s){
    while(txbuf.nchar>0);
    while(*s!='\n'){
        txbuf.mem[txbuf.nchar] = *s;
        txbuf.nchar++;
        s++;
    }
    txbuf.posrd=0;
    IEC1bits.U2TXIE = 1;
}

void _int_(32) isr_uart2(void){
    if(IFS1bits.U2TXIF==1){
        if(txbuf.nchar>0){
            U2TXREG = txbuf.mem[txbuf.posrd];
            txbuf.posrd++;
            txbuf.nchar--;   
        } else{
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
    IEC1bits.U2RXIE = 0; //disable interrupts for RX
    IEC1bits.U2TXIE = 0; //disable interrupts for TX

    U2STAbits.URXISEL = 0; //UART interrupt mode
}

int main(void){
    configureUART();
    EnableInterrupts();
    txbuf.nchar = 0;

    while(1){
        putstrInt("Test string \n");
    }
    return 0;
}

