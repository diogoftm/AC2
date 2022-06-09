#include <detpic32.h>

void delay(int t){
    resetCoreTimer();
    while(readCoreTimer()<20000*t){}
}

void display_dec(unsigned char val, char enable){
    static char flag = 0;
    static char d[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
    if(enable == 1){
        if(flag == 0){
            LATDbits.LATD5 = 0;
            LATDbits.LATD6 = 1;
            LATB = (LATB & 0x80FF) | (d[val/10] << 8);
        } else{
            LATDbits.LATD5 = 1;
            LATDbits.LATD6 = 0;
            LATB = (LATB & 0x80FF) | (d[val%10] << 8);
        }
        flag = flag ^ 1;
    } else{
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 0;
    }
}


int main(void){

    TRISB = TRISB | 0x000F;
    TRISDbits.TRISD8 = 1;
    TRISB = TRISB & 0x80FF;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISEbits.TRISE0 = 0;

    int time = 0;
    int t = 0;
    int blink_control = 0;
    int disp_enable = 1;
    char mode = 0;
    char c = 0;

    while(1){
        c = inkey();
        if(c=='t') mode=mode ^ 1;

        if(mode==0) time = PORTB & 0x000F;
        else if(c=='+') time++;
        else if(c=='-') time--;

        display_dec(time, 1);
        
        if(PORTDbits.RD8==0){
            while(t<time*1000){
                if(blink_control==500) disp_enable = 0;
                if(blink_control==1000){disp_enable=1; blink_control=0;}
                display_dec(time, disp_enable);
                delay(10);
                t=t+10;
                blink_control = blink_control + 10;
                LATEbits.LATE0 = disp_enable;
            }
            LATEbits.LATE0 = 0;
            blink_control = 0;
            t = 0;
        }

        delay(10);
    }

    return 0;
}