#include <detpic32.h>

volatile unsigned char voltage = 0;

void display(char value){
    static char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
    static char flag = 0;

    if(flag == 0){
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0x80FF) | (display7Scodes[value%10] << 8);
    }
    else{
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0x80FF) | (display7Scodes[value/10] << 8);
    }

    flag = flag ^ 1;
}

void delay(int value){
    resetCoreTimer();
    while(readCoreTimer()<20000*value){};
}

void adcConfig(void){
    TRISBbits.TRISB4 = 1; // RBx digital output disconnected
    AD1PCFGbits.PCFG4= 0; // RBx configured as analog input
    AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this
    // mode an internal counter ends sampling and
    // starts conversion
    AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
    // interrupt is generated. At the same time,
    // hardware clears the ASAM bit
    AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 8-1; // Interrupt is generated after XX samples
    // (replace XX by the desired number of
    // consecutive samples)
    AD1CHSbits.CH0SA = 4; // replace x by the desired input
    // analog channel (0 to 15)
    AD1CON1bits.ON = 1; // Enable A/D converter
    // This must the last command of the A/D
    // configuration sequence
    IPC6bits.AD1IP = 2; // configure priority of A/D interrupts
    IFS1bits.AD1IF = 0; // clear A/D interrupt flag
    IEC1bits.AD1IE = 1; // enable A/D interrupts
}

void displaysConfig(void){
    TRISB = TRISB & 0x80FF;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
}

void _int_(27) isr_adc(void){

    int *d = (int *) (&ADC1BUF0);
    int i = 0;
    int average = 0;
    for(;i<8;i++){
        average += d[i*4];
    }
    average /= 8;
    voltage = (average*33+511)/1023;
    
    IFS1bits.AD1IF = 0; //Reset adc flag
}

int main(void){
    adcConfig();
    displaysConfig();
    EnableInterrupts();
    int count = 0;
    while(1){

        if(count % 200 == 0){
            AD1CON1bits.ASAM = 1;
            count = 0;
        }
        display(voltage);
        delay(10);
        count += 10;
    };

    return 0;
}

