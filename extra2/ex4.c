#include <detpic32.h>

volatile int duty = 0;

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

void setPWM(unsigned int dutyCycle){ 
    if(dutyCycle>100) return;
    OC1RS = dutyCycle*200; // Determine OC1RS as a function of "dutyCycle" 
}

void _int_(27) isr_ADC(void){
    duty = (ADC1BUF0*100)/1023;
    setPWM(duty);
    display(duty);
    IFS1bits.AD1IF = 0;
}

void _int_(20) isr_TIMER5(void){
    display(duty);
    IFS0bits.T5IF = 0;
}

void config_ADC(void){
    TRISBbits.TRISB4 = 1;      // RB4 digital output disconnected 
    AD1PCFGbits.PCFG4 = 0;     // RB4 configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;      // Conversion trigger selection bits: in this 
                               //  mode an internal counter ends sampling and 
                               //  starts conversion 

    AD1CON1bits.CLRASAM = 1;   // Stop conversions when the 1st A/D converter 
                               //  interrupt is generated. At the same time, 
                               //  hardware clears the ASAM bit

    AD1CON3bits.SAMC = 16;    // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 0;      // Interrupt is generated after 4 sample 

    AD1CHSbits.CH0SA = 4;      // replace x(4) by the desired input  
                               //  analog channel (0 to 15)

    AD1CON1bits.ON = 1;        // Enable A/D converter 
                               //  This must the last command of the A/D 
                               //  configuration sequence
    IPC6bits.AD1IP = 2;
    IEC1bits.AD1IE = 1;
    IFS1bits.AD1IF = 0;
}

void config_timer(void){
    T2CONbits.TCKPS = 0;
    PR2 = 19999;
    TMR2 = 0;
    T2CONbits.ON = 1;

    OC1CONbits.OCM = 6;  // PWM mode on OCx; fault pin disabled 
    OC1CONbits.OCTSEL = 0;// Use timer T2 as the time base for PWM generation 
    OC1CONbits.ON = 1; // Enable OC1 module 

    T5CONbits.TCKPS = 2;
    PR5 = 49999;
    TMR5 = 0;
    T5CONbits.ON = 1;

    IFS0bits.T5IF = 0;
    IPC5bits.T5IP = 2;
    IEC0bits.T5IE = 1;
}

int main(void){
    config_ADC();
    config_timer();
    EnableInterrupst();
    while(1){
        AD1CON1bits.ASAM=1;
        
        while(readCoreTimer()<20000*250);
    }
    return 0;
}