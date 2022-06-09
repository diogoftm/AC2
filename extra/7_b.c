#include <detpic32.h>
#include <stdlib.h>

void delay(unsigned int t){
    resetCoreTimer();
    while(readCoreTimer()<t*20000){}
}


int main(void){
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

    TRISB = TRISB | 0x000F;
    int periodo = 500;

    while(1){
        AD1CON1bits.ASAM=1;
        while(IFS1bits.AD1IF == 0);
        IFS1bits.AD1IF = 0;
        periodo = 200 + ((ADC1BUF0 * 800) / 1023);

        putChar('\r');
        printStr("DS4=");
        str[0] = PORTBbits.RB3 + 48;
        printStr(str);
        printStr("DS3=");
        str[0] = PORTBbits.RB2 + 48;
        printStr(str);
        printStr("DS2=");
        str[0] = PORTBbits.RB1 + 48;
        printStr(str);
        printStr("DS1=");
        str[0] = PORTBbits.RB0 + 48;
        printStr(str);

        delay(periodo);
    }

    return 0;
}