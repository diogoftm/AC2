.equ BASE, 0xBF88
.equ TRISB, 0x6040
.equ PORTB, 0x6050
.equ LATE, 0x6120
.equ TRISE, 0x6100
.data
.text
.globl main

main:
    lui $s0, BASE
    lw $s1, TRISE($s0)
    andi $s1, $s1, 0xFFF0
    sw $s1, TRISE($s0)
    lw $s1, TRISB($s0)
    ori $s1, $s1, 0x000F
    sw $s1, TRISB($s1)
loop:
    lw $s1, PORTB($s0)
    andi $s1, $s1, 0x000F
    lw $s2, LATE($s0)
    or $s2, $s2, $s1
    sw $s2, LATE($s0)

    j loop

end_main:
    jr $ra
    