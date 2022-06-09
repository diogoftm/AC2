.equ BASE, 0xBF88
.equ TRISB, 0x6040
.equ PORTB, 0x6050
.equ TRISE, 0x6100
.equ LATE, 0x6120
.data
.text
.globl main

main:
    lui $s0, BASE

    lw $s1, TRISB($s0)
    ori $s1, $s1, 0x000F
    sw $s1, TRISB($s0)

    lw $s1, TRISE($s0)
    andi $s1, $s1, 0xFFF0
    sw $s1, TRISE($s0)

    lw $s1, PORTB($s0)
    andi $s1, $s1, 0x000F
    sra $s3, $s1, 3
    sra $s4, $s1, 1
    andi $s4, $s4, 0x0002
    or $s3, $s3, $s4
    sll $s4, $s1, 1
    andi $s4, $s4, 0x0004
    or $s3, $s3, $s4
    sll $s4, $s1, 3
    andi $s4, $s4, 0x0008
    or $s3, $s3, $s4

    lw $s2, LATE($s0)
    andi $s2, $s2, 0xFFF0
    or $s2, $s2, $s3
    
    sw $s2, LATE($s0) 

    j main

    jr $ra