.data
.text
.globl main

main:
do:
        li $v0, 1
        syscall
        move $a0, $v0
	beq $a0, 0, else
if:
	li $v0, 3
        syscall
	j while
else:
        li $v0, 3
        li $a0, '.'
	syscall
while:
        bne $a0, '\n', do
        jr $ra
