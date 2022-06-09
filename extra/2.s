.equ BASE, 0xBF88
.equ TRISE, 0x6100
.equ LATE, 0x6120
.equ readCoreTimer, 11
.equ resetCoreTimer, 12
.equ getChar, 2
.data
.text
.globl main

main:
    lui $s0, BASE

    lw $s1, TRISE($s0)
    andi $s1, $s1, 0xFFF0
    sw $s1, TRISE($s0)

    lw $s1, LATE($s0)
    andi $s1, $s1, 0xFFF0
    sw $s1, LATE($s0)
    
while:
    li $v0, getChar
    syscall
    beq $v0, '0', zero
    beq $v0, '1', one
    beq $v0, '2', two
    beq $v0, '3', three
    j other
zero:
    lw $s1, LATE($s0)
    andi $s1, $s1, 0xFFF0
    ori $s1, $s1, 0x0001
    sw $s1, LATE($s0)
    j while
one:
    lw $s1, LATE($s0)
    andi $s1, $s1, 0xFFF0
    ori $s1, $s1, 0x0002
    sw $s1, LATE($s0)
    j while
two:
    lw $s1, LATE($s0)
    andi $s1, $s1, 0xFFF0
    ori $s1, $s1, 0x0004
    sw $s1, LATE($s0)
    j while
three:
    lw $s1, LATE($s0)
    andi $s1, $s1, 0xFFF0
    ori $s1, $s1, 0x0008
    sw $s1, LATE($s0)
    j while
other:
    lw $s1, LATE($s0)
    ori $s1, $s1, 0x000F
    sw $s1, LATE($s0)
    li $a0, 1000
    jal delay
    lw $s1, LATE($s0)
    andi $s1, $s1, 0xFFF0
    sw $s1, LATE($s0)
    j while
nothing:
    j while

    jr $ra


delay:
    li $v0, resetCoreTimer
    syscall
    mul $t1, $a0, 20000
while_delay:
    li $v0, readCoreTimer
    syscall
    blt $v0, $t1, while_delay
end_delay:
    jr $ra
