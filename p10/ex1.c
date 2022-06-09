#include <detpic32.h>

int main(void){

    U3BRG = 10;
    U3MODEbits.BRGH = 0; //divisão por 16

    U3MODEbits.PDSEL = 0; //00 = 8-bit data, no parity
    U3MODEbits.STSEL = 0; //0 = 1 Stop bit

    U3STAbits.UTXEN = 1; //enable transmiter
    U3STAbits.URXEN = 1; //enable receiver

    U3MODEbits.ON = 1; //enable UART

    return 0;
}