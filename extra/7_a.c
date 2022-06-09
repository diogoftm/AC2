#include <detpic32.h>
#include <stdlib.h>

void delay(unsigned int t){
    resetCoreTimer();
    while(readCoreTimer()<t*20000){}
}


int main(void){

    TRISB = TRISB | 0x000F;
    char str[3];
    str[1] = ' ';

    while(1){
        putChar('\r');
        printStr("DS4=");
        str[0] = PORTBbits.RB3 + 48;
        printStr(str);
        printStr("DS3=");
        str[0] = PORTBbits.RB2 + 48;
        printStr(str);
        printStr("DS2=");
        str[0] = PORTBbits.RB1 + 48;
        printStr(str);
        printStr("DS1=");
        str[0] = PORTBbits.RB0 + 48;
        printStr(str);
        delay(1000);
    }

    return 0;
}