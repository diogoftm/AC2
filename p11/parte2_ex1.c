#include <detpic32.h>

typedef struct { 
    char mem[100]; // Storage area 
    int nchar;   // Number of characters to be transmitted 
    int posrd;   // Position of the next character to be transmitted 
} t_buf;

volatile t_buf txbuf;

void putctrInt(char *s){
    while(txbuf.nchar>0);
    while(*s!='\n'){
        txbuf.mem[txbuf.nchar] = *s;
        txbuf.nchar++;
    }
    txbuf.posrd=0;
    IEC1bits.U2TXIE = 1;
}

