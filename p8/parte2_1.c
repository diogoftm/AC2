#include <detpic32.h>

void delay(int value){
    resetCoreTimer();
    while(readCoreTimer()<20000*value);
}

int main(void){

    TRISEbits.TRISE0 = 0;
    TRISDbits.TRISD8 = 1;

    while(1){
        LATEbits.LATE0 = 0;
        if(PORTDbits.RD9 == 0){
            LATEbits.LATE0 = 1;
            delay(3000);
        }
    }

    return 0;
}