.equ BASE, 0xBF88

.equ TRISE, 0x6100   
.equ PORTE, 0x6110   
.equ LATE,  0x6120 

.equ TRISB, 0x6040
.equ PORTB, 0x6050
.equ LATB,0x6060

.equ TRISD, 0x60C0
.equ PORTD, 0x60D0
.equ LATD,0x60E0

.data
.text
.globl main

main:
    lui $t0, BASE
    lw $t1, TRISE($t0)
    andi $t1, $t1, 0xFFFE
    sw $t1, TRISE($t0)

    lw $t1, TRISD($t0)
    ori $t1, $t1, 0x0100
    sw $t1, TRISD($t0)

while:
    lw $t1, PORTD($t0)
    #not $t1, $t1
    andi $t1, $t1, 0x0100
    srl $t1, $t1, 8
    
    lw $t2, LATE($t0)
    andi $t2, $t2, 0xFFFE
    or $t2, $t1, $t2
    sw $t2, LATE($t0)
    j while
    
end:
    jr $ra