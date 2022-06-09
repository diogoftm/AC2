.equ printInt10, 7
.equ putChar, 3
.equ readCoreTimer, 11
.equ resetCoreTimer, 12
.equ printstr, 8
.equ putchar, 3
.equ BASE, 0xBF88
.equ TRISB, 0x6040
.equ PORTB, 0x6050
.data
str1: .asciiz "DS4="
str2: .asciiz "DS3="
str3: .asciiz "DS2="
str4: .asciiz "DS1="
.text
.globl main

main:
    addiu $sp, $sp, -12
    sw $ra, 0($sp) 
    sw $s0, 4($sp)
    sw $s1, 8($sp)
    lui $s0, BASE
    lw $s1, TRISB($s0)
    ori $s1, $s1, 0x000F
    sw $s1, TRISB($s0)
loop:
    lw $s1, PORTB($s0)
    andi $s1, $s1, 0x000F
    
    li $v0, putChar
    li $a0, '\r'
    syscall

    li $v0, printstr
    la $a0, str1
    syscall

    li $v0, putChar
    andi $a0, $s1, 0x0008
    srl $a0, $a0, 3
    add $a0, $a0, 48
    syscall
    li $a0, ' '
    syscall

    li $v0, printstr
    la $a0, str2
    syscall

    li $v0, putChar
    andi $a0, $s1, 0x0004
    srl $a0, $a0, 2
    add $a0, $a0, 48
    syscall
    li $a0, ' '
    syscall

    li $v0, printstr
    la $a0, str3
    syscall

    li $v0, putChar
    andi $a0, $s1, 0x0002
    srl $a0, $a0, 1
    add $a0, $a0, 48
    syscall
    li $a0, ' '
    syscall

    li $v0, printstr
    la $a0, str4
    syscall

    li $v0, putChar
    andi $a0, $s1, 0x0001
    add $a0, $a0, 48
    syscall
    li $a0, ' '
    syscall

    li $a0, 1000
    jal delay
    j loop
end: 
    lw $ra, 0($sp) 
    lw $s0, 4($sp)
    lw $s1, 8($sp)
    addiu $sp, $sp, 12
    jr $ra

delay:
    li $v0, resetCoreTimer
    syscall
    mul $t0, $a0, 20000
while_delay:
    li $v0, readCoreTimer
    syscall
    blt $v0, $t0, while_delay
end_delay:
    jr $ra