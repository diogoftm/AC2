#include <detpic32.h>

void display_n_byte_decimal(unsigned char n, char enable);
void delay(unsigned int delay);
unsigned char toBcd(unsigned char value);

int main(void){
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISB = (TRISB & 0x80FF); //configuraçao dos segmentos do display [RD8 : RD14]
    TRISE = TRISE & 0x1100;
    TRISBbits.TRISB0 = 1;
    int counter = 0; int i; int t_freq; char up_down; int c;

    while(1){

        if(PORTBbits.RB0 == 0){ up_down=0; t_freq=20;}
        else{ up_down=1; t_freq=50;}

        i = 0;

        do{
            display_n_byte_decimal(counter, 1); 
            delay(10);
        } while(++i < t_freq);

        if(up_down == 0) counter++;
        else counter--;

        if(counter>59){
            counter--;
            c = 0;
            i = 0;
            do{
                if(c<20){
                    display_n_byte_decimal(counter, 1);
                }
                else display_n_byte_decimal(counter, 0);
                delay(10);
                c++;
                if(c>40) c = 0;
            }while(++i<500);
                
            counter=0;
        }

        if(counter<0){
            counter = 0;
            i = 0;
            c = 0;
            do{
                if(c<50){
                    display_n_byte_decimal(counter, 1);
                }
                else display_n_byte_decimal(counter, 0);
                delay(10);
                c++;
                if(c>100) c = 0;
            }while(++i<500);
                
            counter=59;
        }


        

        LATE = (LATE & 0x1100) | toBcd(counter);
    }

    return 0;
}

void display_n_byte_decimal(unsigned char n, char enable){
    static int display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
    static char displayFlag = 0;
    if(enable==1){
        if(displayFlag==0){
            LATDbits.LATD5 = 0; LATDbits.LATD6 = 1;
            LATB = (LATB & 0x80FF) | (display7Scodes[n/10] << 8);
        }
        else{
            LATDbits.LATD5 = 1; LATDbits.LATD6 = 0;
            LATB = (LATB & 0x80FF) | (display7Scodes[n % 10] << 8);
        }
    }
    else{LATDbits.LATD5 = 0; LATDbits.LATD6 = 0;}
    displayFlag = displayFlag ^ 1;
}

void delay(unsigned int delay){
    resetCoreTimer();
    while(readCoreTimer()<delay*20000){}
}

unsigned char toBcd(unsigned char value){
    return ((value/10)<<4) + (value%10);
}