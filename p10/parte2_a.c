#include <detpic32.h>

void putc(char byte){
    while(U1STAbits.UTXBF==1);
    U1TXREG = byte;
}

char getc(void){
    while(U2STAbits.URXDA==0);
    return U2RXREG;
}

int main(void){

    //U1BRG = ((20000000 + 8 * 9600)/(16*9600))-1;
    U1BRG = 10;
    U1MODEbits.BRGH = 0; //divisão por 16

    U1MODEbits.PDSEL = 1; //00 = 8-bit data, no parity
    U1MODEbits.STSEL = 1; //0 = 1 Stop bit

    U1STAbits.UTXEN = 1; //enable transmiter
    U1STAbits.URXEN = 1; //enable receiver

    U1MODEbits.ON = 1; //enable UART

    while(1){
        putc(0x13);
        resetCoreTimer();
        while(readCoreTimer()<200000);
    }

    return 0;
}