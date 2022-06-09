#include <detpic32.h>

void delay(unsigned int t){
    resetCoreTimer();
    while(readCoreTimer()<(t*20000)){}
}

void display(unsigned char c){
    static char selector = 0;
    static int val[] = {0xBF, 0x86, 0xDB, 0xCF, 0xF1};
    if(selector == 0){
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        if(c<4){
            LATB = (LATB & 0x80FF) | (val[c & 0x0F] << 8);
        }
        else LATB = (LATB & 0x80FF) | (val[4] << 8);
    }
    else{
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        if(c<4){
            LATB = (LATB & 0x80FF) | (val[c >> 4] << 8);
        }
        else LATB = (LATB & 0x80FF) | (val[4] << 8);
    }

    selector = selector ^ 0x01;
}


int main(void){

    TRISE = TRISE & 0xFFF0;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISB = TRISB & 0x80FF;
    LATE = LATE & 0xFFF0;

    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 0;
    LATB = (LATB & 0x80FF) & 0x80FF;
    char l = 0;
    while(1){
        char in = inkey();
        if(in!=0) l = in;
        if(l=='0'){ LATE = (LATE & 0xFFF0) | 0x0001; display(0);}
        else if(l=='1') {LATE = (LATE & 0xFFF0) | 0x0002; display(1);}
        else if(l=='2') {LATE = (LATE & 0xFFF0) | 0x0004; display(2);}
        else if(l=='3') {LATE = (LATE & 0xFFF0) | 0x0008; display(3);}
        else if(in==0) continue;
        else{
            LATE = LATE | 0x000F;
            int t = 0;
            while(t<1000){
                display(in);
                delay(10);
                t = t + 10;
            }
            LATE = LATE & 0xFFF0;
            LATDbits.LATD5 = 0;
            LATDbits.LATD6 = 0;
        }
    }

    return 0;
}