#include <detpic32.h>

int main(void){

    TRISB = TRISB | 0x000F;
    TRISE = TRISE & 0xFFF0;

    while(1){
        int val = PORTB & 0x000F;
        LATE = (LATE & 0xFFF0) | val;
    }

    return 0;
}