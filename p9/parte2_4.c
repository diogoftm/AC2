#include <detpic32.h>

void setPWM(unsigned int dutyCycle){ 
  if(dutyCycle>100) return;
  OC1RS = (dutyCycle/2*1000); // Determine OC1RS as a function of "dutyCycle" 
}

void configureTimer(void){
    T3CONbits.TCKPS = 2; 
    PR3 = 50000;         
    TMR3 = 0;            
    T3CONbits.TON = 1; 

    OC1CONbits.OCM = 6;  // PWM mode on OCx; fault pin disabled 
    OC1CONbits.OCTSEL = 1;// Use timer T3 as the time base for PWM generation 
    OC1CONbits.ON = 1; // Enable OC1 module 
}

int main(void){
    TRISCbits.TRISC14 = 0;
    configureTimer();
    setPWM(50);
    
    while(1){
        LATCbits.LATC14 = PORTDbits.RD0;
    }
}