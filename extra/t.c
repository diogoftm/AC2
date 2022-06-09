#include <detpic32.h>

void delay(int t){
    resetCoreTimer();
    while(readCoreTimer()<t*20000);
}

void decimal_display(unsigned char value){
    static char flag = 0;
    static char vals[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    if(flag == 0){
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x80FF) | (vals[value%10] << 8); 
    }else{
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x80FF) | (vals[value/10] << 8); 
    }

    flag = flag ^ 1;
}

void hex_display(unsigned value){
    static char vals[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char flag = 0;

    if(flag == 0){
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x80FF) | (vals[value&0x0F]<<8);
    }else{
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x80FF) | (vals[value>>4]<<8);
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

    TRISB = (TRISB | 0x000F) & 0x80FF;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    int periodo = 0;
    int count = 0;
    int val = 0;
    int i = 0;
    int average = 0;
    int voltage = 0;

    while(1){
        AD1CON1bits.ASAM = 1;
        while( IFS1bits.AD1IF == 0 );
        IFS1bits.AD1IF = 0;

        int *d = (int *)(&ADC1BUF0);
        i = 0;
        average = 0;
        for(;i<4;i++){
            average += d[i*4];
        }
        average /= 4; 

        periodo = 1/((average&0x000F)*1023/4)+1;
        voltage = (average*33+511)/1023;
        printInt10(voltage);
        val = PORTB & 0x000F;
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

        while(count < periodo){
            decimal_display(val);
            delay(10);
            count += 10;
        }
    }

    return 0;
}