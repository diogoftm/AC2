#include <detpic32.h>

int main (void){

    TRISDbits.TRISD11 = 0;

    TRISBbits.TRISB4 = 1;      // RB4 digital output disconnected 
    AD1PCFGbits.PCFG4 = 0;     // RB4 configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;      // Conversion trigger selection bits: in this 
                               //  mode an internal counter ends sampling and 
                               //  starts conversion 

    AD1CON1bits.CLRASAM = 1;   // Stop conversions when the 1st A/D converter 
                               //  interrupt is generated. At the same time, 
                               //  hardware clears the ASAM bit

    AD1CON3bits.SAMC = 16;    // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 0;      // Interrupt is generated after 1 sample 

    AD1CHSbits.CH0SA = 4;      // replace x(4) by the desired input  
                               //  analog channel (0 to 15)

    AD1CON1bits.ON = 1;        // Enable A/D converter 
                               //  This must the last command of the A/D 
                               //  configuration sequence 

    volatile int aux;

    while(1){
        AD1CON1bits.ASAM=1;
        LATDbits.LATD11 = 1;
        while(IFS1bits.AD1IF == 0);
        LATDbits.LATD11 = 0;
        aux = ADC1BUF0;
        IFS1bits.AD1IF = 0;
    }

    return 0;
}