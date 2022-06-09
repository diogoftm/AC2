.equ READ_CORE_TIMER, 11
.equ RESET_CORE_TIMER, 12
.equ PRINT_INT_10, 7
.equ PUT_CHAR, 3
.equ IN_KEY, 1
.data
.text
.globl main

main:
	li $t0, 0  #10Hz
	li $t1, 0  #5Hz
	li $t2, 0  #1Hz
	addiu $sp, $sp, -4
	sw $ra, 0($sp)
while:
	li $v0, IN_KEY
	syscall
	beq $v0, 0, cont
	beq $v0, 't', end
cont:
	li $a0, 100
	jal delay
	add $s3, $s3, 100

	beq $s3, 1000, if_1000
	rem $t4, $s3, 200
	beq $t4, 0, if_200
	rem $t4, $s3, 100
	beq $t4, 0, if_100
	j while
if_1000:
	add $t2, $t2, 1
	add $t0, $t0, 1
	add $t1, $t1, 1
	li $s3, 0
	j print
if_200:
	add $t0, $t0, 1
	add $t1, $t1, 1
	j print
if_100:
	add $t0, $t0, 1
print:
	li $v0, PUT_CHAR
	li $a0, '\r'
	syscall

	li $v0, PRINT_INT_10
	move $a0, $t0
	syscall

	li $v0, PUT_CHAR
    li $a0, ' '
	syscall

	li $v0, PRINT_INT_10
	move $a0, $t1
	syscall

	li $v0, PUT_CHAR
    li $a0, ' '
    syscall

	li $v0, PRINT_INT_10
	move $a0, $t2
	syscall

	j while

end:
	lw $ra, 0($sp)
	addiu $sp, $sp, 4
	jr $ra

delay:
	li $v0, RESET_CORE_TIMER  #resetCoreTimer()
    syscall
while_delay:
	li $v0, READ_CORE_TIMER  #readCoreTimer()
    syscall
    li $t4, 20000
    mul $t6, $a0, $t4
    blt $v0, $t6, while_delay
	jr $ra
