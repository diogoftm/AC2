.data
.text
.globl main

main:
do:
	li $v0, 2
	syscall 
	move $a0, $v0
	li $v0, 3
	syscall
	bne $a0, '\n', do
	jr $ra
