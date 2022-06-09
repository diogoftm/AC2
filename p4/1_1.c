#include <detpic32.h>
#define CALIBRATION (FREQ / 2000)

void delay(unsigned int val);

int main(void){
    TRISCbits.TRISC14 = 0;
    
    while(1){
        delay(500);
        LATCbits.LATC14 = LATCbits.LATC14 ^ 1;
    }
    return 0;
}

void delay(unsigned int val){
	resetCoreTimer();
    while(readCoreTimer()<val*CALIBRATION){

    }
}