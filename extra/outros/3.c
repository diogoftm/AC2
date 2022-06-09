//ler o adc com 4 amostras e fazer o display da tensão 
#include <detpic32.h>

void delay(int val){
    resetCoreTimer();
    while(readCoreTimer()<val*20000){}
}

void display(unsigned char val){
    static char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
    static char flag = 0;

    if(flag==0){
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x80FF) | (display7Scodes[val&0x0F]<<8);
        //LATB = (LATB & 0x80FF) | (display7Scodes[val%10]<<8)
    }
    else{
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x80FF) | (display7Scodes[val>>4]<<8);
        //LATB = (LATB & 0x80FF) | (display7Scodes[val/10]<<8)
    }

    flag = flag ^ 1;
}

int main(void){

    TRISBbits.TRISB4 = 1; // RBx digital output disconnected
    AD1PCFGbits.PCFG4= 0; // RBx configured as analog input
    AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this
    // mode an internal counter ends sampling and
    // starts conversion
    AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
    // interrupt is generated. At the same time,
    // hardware clears the ASAM bit
    AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 4-1; // Interrupt is generated after XX samples
    // (replace XX by the desired number of
    // consecutive samples)
    AD1CHSbits.CH0SA = 4; // replace x by the desired input
    // analog channel (0 to 15)
    AD1CON1bits.ON = 1; // Enable A/D converter
    // This must the last command of the A/D
    // configuration sequence

    TRISB = TRISB & 0x80FF;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    int counter = 0;
    int media = 0;
    while(1){
        if(counter%200==0){
            AD1CON1bits.ASAM = 1;
            while( IFS1bits.AD1IF == 0 );
            IFS1bits.AD1IF = 0;

            int *d = (int *)(&ADC1BUF0);
            int i = 0;
            for(; i<4; i++){
                media += d[i*4];
            }
            media /= 4;
        }
        delay(10);
        int voltage = (media*33+511)/1023;
        display(voltage);
        counter += 10;
    }

    return 0;
}