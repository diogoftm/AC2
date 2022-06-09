.equ READ_CORE_TIMER, 11
.equ RESET_CORE_TIMER, 12
.equ PUT_CHAR, 3
.equ PRINT_INT, 6
.data
.text
.globl main

main:
	li $t0, 0
while:
	li $v0, RESET_CORE_TIMER
	syscall
while_2:
	li $v0, READ_CORE_TIMER
	syscall
	ble $v0, 200000, while_2
end_while_2:
	move $a0, $t0
	li $v0, PRINT_INT
	li $a1, 0x0004000A   #especificação do numero de digitos e da base
	syscall

	addi $t0, $t0, 1
	li $a0, '\r'
	li $v0, PUT_CHAR
	syscall

	j while

	jr $ra 
