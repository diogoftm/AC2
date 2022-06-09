#include <detpic32.h>
#include <stdlib.h>

void delay(unsigned int t){
    resetCoreTimer();
    while(readCoreTimer()<t*20000){}
}

void display(unsigned char n, char enable){
    static char flag = 1;
    static char vals[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    if(enable==1){
        if(flag == 1){
            LATDbits.LATD5 = 1;
            LATDbits.LATD6 = 0;
            LATB = (LATB & 0x80FF) | (vals[n%10]<<8);
        } else{
            LATDbits.LATD5 = 0;
            LATDbits.LATD6 = 1;
            LATB = (LATB & 0x80FF) | (vals[n/10]<<8);
        }
    }
    flag = flag ^ 0x01;
}


int main(void){

    int counter = 0;
    int freq = 100;
    char str[2];

    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 0;

    TRISB = TRISB & 0x80FF;

    char val = 0;
    char enable = 0;
    int period = 0;
    while(1){
        if(period==freq){
            counter++;
            putChar('\r');
            printInt10(counter);
            if(counter==99) counter=0;
            period=0;
        }

        str[0] = inkey();
        str[1] = '\n';
        if(str[0]<'5'&& str[0]>='0'){
            freq = 1000/ (2 * (1+atoi(str)));
            period=0;
        }
        else if(str[0]=='\n') {val = counter; enable=1;}
        
        display(val, enable);

        delay(10);
        period = period + 10;
    }

    return 0;
}