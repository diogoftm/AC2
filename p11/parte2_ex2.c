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

void _int_(32) isr_uart2(void){
    if(IFS1bits.U2TXIF==1){
        if(txbuf.nchar>0){
            U2TXREG = txbuf.mem[txbuf.posrd];
            txbuf.posrd++;
            txbuf.nchar--;   
        } else{
            IEC1bits.U2TXIE = 1;
        }
        IFS1bits.U2TXIF==0;
    }
}