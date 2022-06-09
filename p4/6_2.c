#include <detpic32.h>

void display_n(int n);

int main(void){
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD8 = 1; //configuração para o INT1

    TRISB = (TRISB & 0x80FF) | 0x000F; //configuraçao dos segmentos do display e dos switch's

    while(1){
        int val = PORTB & 0x000F;
        int disp = PORTDbits.RD8;
        if(disp == 1){ LATDbits.LATD5 = 0; LATDbits.LATD6 = 1;}
        else{LATDbits.LATD5 = 1; LATDbits.LATD6 = 0;}
        display_n(val); 
    }

    return 0;
}

void display_n(int n){
    int display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    LATB = (LATB & 0x80FF) | (display7Scodes[n] << 8);
}