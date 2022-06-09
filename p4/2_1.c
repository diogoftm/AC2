#include <detpic32.h>
#define CALIBRATION 20000

void delay(unsigned int time);

int main(void){
    TRISE = TRISE & 0xFFF0;  //configurar RE3, RE2, RE1 e RE0 como saidas 
    int count = 0; int c = 0;
    while(1){
        delay(500);
        if(count==0xFFFF) count = 0;
        count++;
        c = count & 0x000F;
        LATE = (LATE & 0XFFF0) | c;
    }
    return 0;
}

void delay(unsigned int time){
	resetCoreTimer();
    while(readCoreTimer()<time*CALIBRATION){

    }
}