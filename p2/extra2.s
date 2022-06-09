.equ READ_CORE_TIMER, 11
.equ RESET_CORE_TIMER, 12
.equ PRINT_INT_10, 7
.equ PUT_CHAR, 3
.equ IN_KEY, 1
.data
.text
.globl main

main:
    addiu $sp, $sp, -4
	sw $ra, 0($sp)
	li $s0, 0  #10Hz
	li $s1, 0  #5Hz
	li $s2, 0  #1Hz
while:
    li $a0, 1000
    jal timeDone
    beq $v0, 0, key
    li $v0, PUT_CHAR
    li $a0, '\n'
    syscall
    li $v0, RESET_CORE_TIMER
    syscall
key:
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
	add $s2, $t2, 1
	add $s0, $t0, 1
	add $s1, $t1, 1
	li $s3, 0
	j print
if_200:
	add $s0, $t0, 1
	add $s1, $t1, 1
	j print
if_100:
	add $t0, $t0, 1
print:
	li $v0, PUT_CHAR
	li $a0, '\r'
	syscall

	li $v0, PRINT_INT_10
	move $a0, $s0
	syscall

	li $v0, PUT_CHAR
    li $a0, ' '
	syscall

	li $v0, PRINT_INT_10
	move $a0, $s1
	syscall

	li $v0, PUT_CHAR
    li $a0, ' '
    syscall

	li $v0, PRINT_INT_10
	move $a0, $s2
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


timeDone:
    li $t1, 0
    ble $a1, 0, else_timeDone
if_timeDone:
    li $v0, RESET_CORE_TIMER
    syscall
    j end_timeDone
else_timeDone:
    li $v0, READ_CORE_TIMER
    syscall
    move $t2, $v0
    li $t0, 20000
    mul $t5, $t0, $a0
    ble $t2, $t5, end_timeDone
if_2_timeDone:
    div $t1, $t2, $t0
end_timeDone:
    move $v0, $t1
    jr $ra
