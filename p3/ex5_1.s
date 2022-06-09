.equ BASE, 0xBF88

.equ TRISE, 0x6100   
.equ PORTE, 0x6110   
.equ LATE,  0x6120

.equ TRISD, 0x60C0
.equ PORTD, 0x60D0
.equ LATD,0x60E0

.data
.text
.globl main

main:
    addi $sp, $sp, -4
    sw $ra, 0($sp)

    lui $s0, BASE
    li $s2, 0

    lw $s1, TRISE($s0)
    andi $s1, $s1, 0xFFFE
    sw $s1, TRISE($s0)

    lw $s1, TRISD($s0)
    andi $s1, $s1, 0xFFFE
    sw $s1, TRISD($s0)
while:
    lw $s1, LATE($s0)
    lw $s3, LATD($s0)

    andi $s1, $s1, 0xFFFE
    andi $s3, $s3, 0xFFFE

    or $s1, $s2, $s1
    or $s3, $s2, $s3
    sw $s1, LATE($s0)
    sw $s3, LATD($s0)

    li $a0, 500
    jal delay

    xori $s2, $s2, 0x0001

    j while
    
end:
    lw $ra, 0($sp)
    addi $sp, $sp, 4
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