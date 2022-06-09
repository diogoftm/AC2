#include <detpic32.h>

void putc(char byte){
    while(U2STAbits.UTXBF==1);
    U2TXREG = byte;
}

void _int_(32) isr_uart2(void){
    if(IFS1bits.U2RXIF == 1){
        putc(U2RXREG);
        IFS1bits.U2RXIF = 0;
    }
}

int main(void){

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
    IFS1bits.U2RXIF = 0; //reset interrupt flag
    IEC1bits.U2RXIE = 1; //enable interrupts for RX
    IEC1bits.U2TXIE = 0; //desable interrupts for TX

    U2STAbits.URXISEL = 0;
    U2STAbits.UTXISEL = 0;

    EnableInterrupts();

    while(1);

    return 0;
}