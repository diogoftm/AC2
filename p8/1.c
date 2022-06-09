#include <detpic32.h>

int main(void){
    T3CONbits.TCKPS = 14;
    PR3 = 39062;         
    TMR3 = 0;            // Reset timer T3 count register
    T3CONbits.TON = 1;   // Enable timer T3 (must be the last command of the
                         // timer configuration sequence)
    while(1){
        while(IFS0bits.T3IF==0){}
        IFS0bits.T3IF=0;
        putChar('.');
    }

    return 0;
}