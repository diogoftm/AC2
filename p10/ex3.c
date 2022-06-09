#include <detpic32.h>

void putc(char byte){
    while(U2STAbits.UTXBF==1);
    U2TXREG = byte;
}

void putstr(char *str){
    while(*str!='\n'){
        putc(*str);
        str += 1;
    }
}

int main(void){

    U2BRG = 10;
    U2MODEbits.BRGH = 0; //divisão por 16

    U2MODEbits.PDSEL = 0; //00 = 8-bit data, no parity
    U2MODEbits.STSEL = 0; //0 = 1 Stop bit

    U2STAbits.UTXEN = 1; //enable transmiter
    U2STAbits.URXEN = 1; //enable receiver

    U2MODEbits.ON = 1; //enable UART

    while(1){
        putstr("String de teste\n");
        resetCoreTimer();
        while(readCoreTimer()<20000*1000);
    }

    return 0;
}