#include <detpic32.h>

int main(void){

    TRISB = TRISB | 0x000F;
    TRISE = TRISE & 0xFF00;
    while(1){
        int val = PORTB & 0x000F;
        int reversed = val >> 3;
        reversed = reversed | ((val >> 1) & 0x0002);
        reversed = reversed | ((val << 1) & 0x0004);
        reversed = reversed | ((val << 3) & 0x0008);
        int disp = (reversed & 0x000F) | ((val^0x000F)<<4);
        LATE = (LATE & 0xFF00) | disp;
    }

    return 0;
}