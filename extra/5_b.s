.equ printInt10, 7
.equ putChar, 3
.equ readCoreTimer, 11
.equ resetCoreTimer, 12
.equ inkey, 1
.data
.text
.globl main

main:
    li $s0, 0
    li $s1, 100
loop:
    addi $s0, $s0, 1

    move $a0, $s1
    jal delay

    li $v0, putChar
    li $a0, '\r'
    syscall

    li $v0, printInt10
    move $a0, $s0
    syscall

    li $v0, inkey
    syscall

    bgt $v0, '4', no_freq_change
    blt $v0, '0', no_freq_change

    addiu $v0, $v0, -47
    mul $v0, $v0, 2
    li $t0, 1000
    div $s1, $t0, $v0

no_freq_change:
    beq $s0, 99, reset
    j loop
reset:
    li $s0, 0
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
end_delay:
    jr $ra
