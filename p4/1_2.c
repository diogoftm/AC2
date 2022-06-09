#include <detpic32.h>

void display(char c);

int main(void){
    TRISB = TRISB & 0x80FF;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;
    LATB = (LATB & 0x80FF); //reset ao display
    while(1){
        char character = getChar();
        display(character);
    }
    return 0;
}

void display(char c){
    if(c=='a') LATB = (LATB & 0x80FF) | 0x7700;
    else if(c=='b') LATB = (LATB & 0x80FF) | 0x7C00;
    else if(c=='c') LATB = (LATB & 0x80FF) | 0x3900;
    else if(c=='d') LATB = (LATB & 0x80FF) | 0x5E00;
    else LATB = (LATB & 0x80FF) | 0x4000;
}