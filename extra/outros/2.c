//colocar o valor dos switches no 7seg display menos significativo
#include <detpic32.h>

void display(unsigned char val){
    static char v[] = {0x77,0x41,0x3b,0x6b,0x4d,0x6e,0x7e,0x43,0x7f,0x6f,0x5f,0x7c,0x36,0x79,0x3e,0x1e};

    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;

    LATB = (LATB & 0x80FF) | (v[val&0x0F] << 8);

}

int main(void){

    TRISB = (TRISB & 0x80FF) | 0x000F;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    while(1){
        display(PORTB & 0x000F);
    }

    return 0;
}