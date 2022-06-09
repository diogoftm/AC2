#include <detpic32.h>

#define CALIBRATION 20000

void display_l(char c);
void delay(unsigned int time);

int main(void){
    char character[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    TRISB = TRISB & 0x80FF;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    LATD = (LATD & 0xFF9F) | 0x0020; // =  LATDbits.LATD5 = 1; LATDbits.LATD6 = 0;
    int i = 0;
    LATB = (LATB & 0x80FF); //reset ao display
    while(1){
        delay(250);
        display_l(character[i]);
        LATD = LATD ^ 0x0060;
        if(i<5) i++; else i=0;
    }
    return 0;
}

void display_l(char c){
    if(c=='a') LATB = (LATB & 0x80FF) | 0x7700;
    else if(c=='b') LATB = (LATB & 0x80FF) | 0x7C00;
    else if(c=='c') LATB = (LATB & 0x80FF) | 0x3900;
    else if(c=='d') LATB = (LATB & 0x80FF) | 0x5E00;
    else if(c=='e') LATB = (LATB & 0x80FF) | 0x7900;
    else if(c=='f') LATB = (LATB & 0x80FF) | 0x7100;
    else LATB = (LATB & 0x80FF) | 0x4000;
}

void delay(unsigned int time){
	resetCoreTimer();
    while(readCoreTimer()<time*CALIBRATION){

    }
}