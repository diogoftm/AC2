#include <detpic32.h>

void delay(unsigned int t){
    resetCoreTimer();
    while(readCoreTimer()<(t*20000)){}
}


int main(void){

    TRISE = TRISE & 0xFFF0;
    LATE = LATE & 0xFFF0;

    while(1){
        char in = getChar();
        if(in=='0') LATE = (LATE & 0xFFF0) | 0x0001;
        else if(in=='1') LATE = (LATE & 0xFFF0) | 0x0002;
        else if(in=='2') LATE = (LATE & 0xFFF0) | 0x0004;
        else if(in=='3') LATE = (LATE & 0xFFF0) | 0x0008;
        else{
            LATE = LATE | 0x000F;
            delay(1000);
            LATE = LATE & 0xFFF0;
        }
    }

    return 0;
}
