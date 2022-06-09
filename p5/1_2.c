#include <detpic32.h>

void display_n_byte_decimal(unsigned char n);
void delay(unsigned int delay);
unsigned char toBcd(unsigned char value);

int main(void){
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISB = (TRISB & 0x80FF); //configuraçao dos segmentos do display [RD8 : RD14]
    TRISE = TRISE & 0x1100;
    int counter = 0; int i;

    while(1){

        i = 0;

        do{
            display_n_byte_decimal(counter); 
            delay(10);
        } while(++i < 20);

        counter++;
        
        if(counter>59) counter=0;

        LATE = (LATE & 0x1100) | toBcd(counter);
    }

    return 0;
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

void delay(unsigned int delay){
    resetCoreTimer();
    while(readCoreTimer()<delay*20000){}
}

unsigned char toBcd(unsigned char value){
    return ((value/10)<<4) + (value%10);
}