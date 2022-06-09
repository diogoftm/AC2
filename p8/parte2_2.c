#include <detpic32.h>

volatile char tCount =0;

void _int_(8) isr_T2(void){
    tCount++;
    if(tCount == 6){
        LATEbits.LATE0 = 0;
        T2CONbits.TON = 0;
    }
    IFS0bits.T2IF = 0;
}

void _int_(7) isr_INT1(void){
    T2CONbits.TON = 1;   // Enable timer T2 (must be the last command of the
    IFS0bits.INT1IF = 0;
    LATEbits.LATE0 = 1;
    tCount = 0;
}

int main(void){

    TRISEbits.TRISE0 = 0;
    LATEbits.LATE0 = 0;
    TRISDbits.TRISD8 = 1;

    IPC1bits.INT1IP = 2;
    INTCONbits.INT1EP = 0;
    IEC0bits.INT1IE = 1;
    IFS0bits.INT1IF = 0;

    T2CONbits.TCKPS = 7;
    PR2 = 39062;         
    TMR2 = 0;            // Reset timer T2 count register
                         // timer configuration sequence)
    IPC2bits.T2IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T2IE = 1; // Enable timer T2 interrupts
    IFS0bits.T2IF = 0; // Reset timer T2 interrupt flag
    
    EnableInterrupts();

    while(1);

    return 0;
}