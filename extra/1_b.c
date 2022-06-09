#include <detpic32.h>

int main(void){

    TRISB = TRISB | 0x000F;
    TRISE = TRISE & 0xFFF0;
    while(1){
        int val = PORTB & 0x000F;
        int reversed = val >> 3;
        reversed = reversed | ((val >> 1) & 0x0002);
        reversed = reversed | ((val << 1) & 0x0004);
        reversed = reversed | ((val << 3) & 0x0008);
        LATE = (LATE & 0xFFF0) | (reversed & 0x000F);
    }

    return 0;
}