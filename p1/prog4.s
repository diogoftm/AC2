.data
str1: .asciiz "\nIntroduza um valor inteiro: "
str2: .asciiz "\nValor: "
str3: .asciiz "\nValor em base 10 (unsigned): "
.text
.globl main

main:
	
while:
	li $v0, 8
	la $a0, str1
	syscall
	li $v0, 5
	syscall
	move $a0, $v0
	li $v0, 7
	syscall

	j while
	jr $ra
