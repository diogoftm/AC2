.equ BASE, 0xBF88

.equ TRISE, 0x6100   
.equ PORTE, 0x6110   
.equ LATE,  0x6120 

.equ TRISB, 0x6040
.equ PORTB, 0x6050
.equ LATB,0x6060

.data
.text
.globl main

main:
    addiu $sp, $sp, -4
    sw $ra, 0($sp)

    li $s2, 0x000F #contador

    lui $s0, BASE

    lw $s1, TRISE($s0)
    andi $s1, $s1, 0xFFF0
    sw $s1, TRISE($s0)

    lw $s1, TRISB($s0)
    ori $s1, $s1, 0x000F
    sw $s1, TRISB($s0)
init:
    lw $s1, PORTB($s0)
    andi $s1, $s1, 0x0008
    beq $s1, 0x0000, set_down
set_up:
    li $s2, 0
    j counter_up_4bits
set_down:
    li $s2, 0x000F
    j counter_down_4bits

check:
    lw $s1, PORTB($s0)
    andi $s1, $s1, 0x0008
    beq $s1, 0x0000, down
up:
    j counter_up_4bits
down:
    j counter_down_4bits

counter_down_4bits:
    lw $s1, LATE($s0)
    andi $s1, $s1, 0xFFF0
display_down:
    or $s1, $s1, $s2
    sw $s1, LATE($s0)
wait_up:
    li $a0, 500
    jal delay
    beq $s2, 0, reset_down
    sub $s2, $s2, 1
    j check
reset_down:
    li $s2, 0x000F
    j check


counter_up_4bits:
    lw $s1, LATE($s0)
    andi $s1, $s1, 0xFFF0
    ble $s2, 0x000F, display_up
reset_up:
    li $s2, 0
display_up:
    or $s1, $s1, $s2
    sw $s1, LATE($s0)
wait_down:
    li $a0, 500
    jal delay
    addi $s2, $s2, 1

    j check


end:
    lw $ra, 0($sp)
    addiu $sp, $sp, 4
    jr $ra

delay:  #delay(unsigned int ms)
	li $v0, 12  #resetCoreTimer()
	syscall
while_delay:
	li $v0, 11  #readCoreTimer()
	syscall
	mul $t0, $a0, 20000
	blt $v0, $t0, while_delay
	jr $ra