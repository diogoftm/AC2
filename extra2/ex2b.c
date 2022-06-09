#include <detpic32.h>

volatile int counter = 0;

void display(unsigned char s){
    static char values[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char flag = 0;

    if(flag==0){
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x80FF)|((values[s%10])<<8);
    }else{
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x80FF)|((values[s/10])<<8);
    }

    flag = flag ^ 1;
}

void _int_(8) isr_T2(void){
    display(counter);
    IFS0bits.T2IF = 0;
}

void _int_(4) isr_T1(void){
    if(counter==99){
        counter = 0;
    } else{
        counter++;
    }
    IFS0bits.T1IF = 0;
}

void configTimers(void){
    T1CONbits.TCKPS = 3;
    PR1 = 31249;
    TMR1 = 0;

    IFS0bits.T1IF = 0;
    IPC1bits.T1IP = 2;
    IEC0bits.T1IE = 1;
    T1CONbits.ON = 1; 

    T2CONbits.TCKPS = 3;
    PR2 = 49999;
    TMR2 = 0;
    
    IFS0bits.T2IF = 0;
    IPC2bits.T2IP = 2;
    IEC0bits.T2IE = 1;
    T2CONbits.ON = 1;
}

int main(void){
    configTimers();
    EnableInterrupts();
    TRISB = TRISB & 0x80FF;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    while(1){
        char c = inkey();
        if(c>='0'&&c<'6'){
            PR1 = (20000000/256)/(2*(1+c-48))-1;
        }
    }
    return 0;
}