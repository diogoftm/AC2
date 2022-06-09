#include <detpic32.h>
#include <stdlib.h>

void delay(unsigned int delay){
    resetCoreTimer();
    while(readCoreTimer()<delay*20000){}
}

void display_n_byte_decimal(unsigned char n){
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

int main (void){

    TRISBbits.TRISB4 = 1;      // RB4 digital output disconnected 
    AD1PCFGbits.PCFG4 = 0;     // RB4 configured as analog input (AN4)
    AD1CON1bits.SSRC = 7;      // Conversion trigger selection bits: in this 
                               //  mode an internal counter ends sampling and 
                               //  starts conversion 

    AD1CON1bits.CLRASAM = 1;   // Stop conversions when the 1st A/D converter 
                               //  interrupt is generated. At the same time, 
                               //  hardware clears the ASAM bit

    AD1CON3bits.SAMC = 16;    // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 3;      // Interrupt is generated after 4 sample 

    AD1CHSbits.CH0SA = 4;      // replace x(4) by the desired input  
                               //  analog channel (0 to 15)

    AD1CON1bits.ON = 1;        // Enable A/D converter 
                               //  This must the last command of the A/D 
                               //  configuration sequence 

    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISB = (TRISB & 0x80FF); //configuraçao dos segmentos do display
    
    int t = 0;
    int i;
    int amplitude = 0;

    while(1){
        if(t%200==0){
            t=0;
            
            AD1CON1bits.ASAM=1;
            while(IFS1bits.AD1IF == 0);
            int v = 0;
            
            int *p = (int *)(&ADC1BUF0);
            for(i = 0; p <= (int *)(&ADC1BUF3); p+=4){  //como p é o endereço de um int, ao sumar-lhe 1
                v = v + *p;                        //ele move-se 4 bytes, logo, neste caso tem-se de
                                                  //sumar 4 para se mover 16 bytes
                i++;
            }

            IFS1bits.AD1IF = 0;

            int media = v/4;
            printf("Media= %d", media);
            amplitude = (media*33+511)/1023;
            printf("Tensao=%d ",t);
    
        }
        
        display_n_byte_decimal(amplitude);
        delay(10);
        t = t + 10;
        
    }

    return 0;
}