
.data
.text
.globl main

main:
delay:  #delay(unsigned int ms)
	li $v0, 12  #resetCoreTimer()
	syscall
while_delay:
	li $v0, 11  #readCoreTimer()
	syscall
	li $s0, 20000
	multu $a0, $s0
	mflo $s0
	blt $v0, $s0, while_delay
	jr $ra
end: 
	jr $ra
