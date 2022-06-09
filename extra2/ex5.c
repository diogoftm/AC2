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

void _int_(4) isr_TIMER1(void){
    putstr( "RB30=\n");
    putc(PORTBbits.RB3 + 48);
    putc(PORTBbits.RB2 + 48);
    putc(PORTBbits.RB1 + 48);
    putc(PORTBbits.RB0 + 48);
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

void configUART(void){
    //configuração da UART 2
    U2BRG = 1041;
    U2MODEbits.BRGH = 0; //divisão por 16

    U2MODEbits.PDSEL = 0; //00 = 8-bit data, no parity
    U2MODEbits.STSEL = 0; //0 = 1 Stop bit

    U2STAbits.UTXEN = 1; //enable transmiter
    U2STAbits.URXEN = 1; //enable receiver

    U2MODEbits.ON = 1; //enable UART
    U2STAbits.UTXISEL = 0;
}

int main(void){
    configTIMERS();
    configUART();
    EnableInterrupts();
    TRISB = TRISB | 0x000F;
    while(1){

    }
    return 0;
}