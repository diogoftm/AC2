#include <detpic32.h>

volatile char value = 0x00;

void display(unsigned char s){
    static char values[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char flag = 0;

    if(flag==0){
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x80FF)|((values[s & 0x0F])<<8);
    }else{
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x80FF)|((values[s >> 4])<<8);
    }

    flag = flag ^ 1;
}

void _int_(8) isr_T2(void){
    display(value);
    IFS0bits.T2IF = 0;
}

void timerConfig(void){
    T2CONbits.TCKPS = 2;
    PR2 = 49999;
    TMR2 = 0;

    IPC2bits.T2IP = 2; //priority
    IFS0bits.T2IF = 0; //reset flag
    IEC0bits.T2IE = 1; //enable

    T2CONbits.ON = 1;
}

int main(void){
    timerConfig();
    EnableInterrupts();
    TRISE = TRISE & 0xFFF0;
    TRISB = TRISB & 0x80FF;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    while(1){
        char c = inkey();
        if(c==0)continue;
        if(c=='0'){
            value = 0;
            LATE = (LATE & 0xFFF0) | 0x0001;
        } else if(c=='1'){
            value = 1;
            LATE = (LATE & 0xFFF0) | 0x0002;
        } else if(c=='2'){
            value = 2;
            LATE = (LATE & 0xFFF0) | 0x0004;
        } else if(c=='3'){
            value = 3;
            LATE = (LATE & 0xFFF0) | 0x0008;
        } else{
            value = 0xFF ;
            LATE = LATE | 0x000F;
            resetCoreTimer();
            while(readCoreTimer()<20000000);
            value = 0;
            LATE = LATE & 0xFFF0;
        }
    }
    return 0;
}
