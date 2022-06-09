#include <detpic32.h>

volatile int voltage = 0;

void display(unsigned char n){
    static int display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
    static char displayFlag = 0;

    if(displayFlag==0){
        LATDbits.LATD5 = 0; LATDbits.LATD6 = 1;
        LATB = (LATB & 0x80FF) | (display7Scodes[n/10] << 8);
    }
    else{
        LATDbits.LATD5 = 1; LATDbits.LATD6 = 0;
        LATB = (LATB & 0x80FF) | (display7Scodes[n % 10] << 8);
    }

    displayFlag = displayFlag ^ 1;
}

void _int_(4) isr_T1(void){
    AD1CON1bits.ASAM=1;
    IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void){
    display(voltage);
    IFS0bits.T3IF = 0;
}

void _int_(27) isr_ADC(void){
    int *p = (int *)(&ADC1BUF0);
    int i = 0;
    for(;i<8;i++){
        voltage += p[i*4];
    }
    voltage /= 8;
    voltage = (voltage * 33 + 511)/1023;

    IFS1bits.AD1IF = 0;
}

void configureADC(void){
    TRISBbits.TRISB4 = 1;      // RB4 digital output disconnected 
    AD1PCFGbits.PCFG4 = 0;     // RB4 configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;      // Conversion trigger selection bits: in this 
                               //  mode an internal counter ends sampling and 
                               //  starts conversion 

    AD1CON1bits.CLRASAM = 1;   // Stop conversions when the 1st A/D converter 
                               //  interrupt is generated. At the same time, 
                               //  hardware clears the ASAM bit

    AD1CON3bits.SAMC = 16;    // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 8-1;      // Interrupt is generated after 4 sample 

    AD1CHSbits.CH0SA = 4;      // replace x(4) by the desired input  
                               //  analog channel (0 to 15)

    AD1CON1bits.ON = 1;        // Enable A/D converter 
                               //  This must the last command of the A/D 
                               //  configuration sequence
    IPC6bits.AD1IP = 2;
    IEC1bits.AD1IE = 1;
    IFS1bits.AD1IF = 0;
}

void configureTimers(void){
    T3CONbits.TCKPS = 2; 
    PR3 = 49999;         
    TMR3 = 0;            
    IPC3bits.T3IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1; // Enable timer T2 interrupts
    T3CONbits.TON = 1;  
    IFS0bits.T3IF = 0; // Reset timer T2 interrupt flag

    T1CONbits.TCKPS = 2; 
    PR1 = 62499;         
    TMR1 = 0;            
    IPC1bits.T1IP = 2; // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1; // Enable timer T2 interrupts
    T1CONbits.TON = 1; 
    IFS0bits.T1IF = 0; // Reset timer T2 interrupt flag
}

int main(void){
    TRISB = TRISB & 0x80FF;
    TRISD = TRISD & 0XFF9F;
    configureADC();
    configureTimers();
    EnableInterrupts();
    while(1);
    return 0;
}