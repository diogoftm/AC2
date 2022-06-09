.equ printInt10, 7
.equ putChar, 3
.equ readCoreTimer, 11
.equ resetCoreTimer, 12
.equ printstr, 8
.equ BASE, 0xBF88
.equ TRISB, 0x6040
.equ PORTB, 0x6050
.equ TRISE, 0x6100
.equ LATE, 0x6120
.equ TRISD, 0x60C0
.equ PORTD, 0x60D0

.data
.text
.globl main

main:
    lui $s0, BASE

    lw $s1, TRISB($s0)
    ori $s1, $s1, 0x000F
    sw $s1, TRISB($s0)

    lw $s1, TRISE($s0)
    andi $s1, $s1, 0xFF0E
    sw $s1, TRISE($s0)

    lw $s1, TRISD($s0)
    ori $s1, $s1, 0x0100
    sw $s1, TRISD($s0)

    lw $s2, LATE($s0)
    andi $s2, $s2, 0xFFFE
    sw $s2, LATE($s0)

    li $s3, 0

loop:
    lw $s1, PORTB($s0)
    andi $s4, $s1, 0x000F
    sll $s1, $s4, 4
    
    lw $s2, LATE($s0)
    andi $s2, $s2, 0xFF0F
    or $s2, $s1, $s2
    sw $s2, LATE($s0)

    lw $s2, PORTD($s0)
    srl $s2, $s2, 8
    andi $s2, $s2, 0x0001
    beq $s2, 0, blink
    j loop
blink:
    lw $s2, LATE($s0)
    xor $s2, $s2, 0x0001
    sw $s2, LATE($s0)

    li $a0, 500
    jal delay
    addi $s3, $s3, 1
    mul $t0, $s4, 2
    bge $s3, $t0, end_blink
    j blink
end_blink:
    li $s3, 0
    j loop

end:
    jr $ra



delay:
    mul $t0, $a0, 20000
    li $v0, resetCoreTimer
    syscall
while_delay:
    li $v0, readCoreTimer
    syscall
    blt $v0, $t0, while_delay
    jr $ra