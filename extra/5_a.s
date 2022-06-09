.equ printInt10, 7
.equ putChar, 3
.equ readCoreTimer, 11
.equ resetCoreTimer, 12
.data
.text
.globl main

main:
    li $s0, 0
loop:
    addi $s0, $s0, 1

    li $a0, 100
    jal delay

    li $v0, putChar
    li $a0, '\r'
    syscall

    li $v0, printInt10
    move $a0, $s0
    syscall

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