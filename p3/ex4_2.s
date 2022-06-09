.equ BASE, 0xBF88

.equ TRISE, 0x6100   
.equ PORTE, 0x6110   
.equ LATE,  0x6120

.equ TRISB, 0x6040
.equ PORTB, 0x6050
.equ LATB,  0x6060

.data
.text
.globl main

main:
    lui $s0, BASE
    li $s3, 0x0001

    lw $s1, TRISE($s0)
    andi $s1, $s1, 0xFFF0
    sw $s1, TRISE($s0)

    lw $s1, TRISB($s0)
    ori $s1, $s1, 0x0002
    sw $s1, TRISB($s0)

shift:
    lw $s1, LATE($s0)
    andi $s1, $s1, 0xFFF0
    or $s1, $s1, $s3 
    sw $s1, LATE($s0)
    lw $s1, PORTB($s0)
    andi $s1, $s1, 0x0002
    beq $s1, 0x0000, left
    j right
left:
    li $a0, 300
    jal delay
    beq $s3, 0x0008, reset_left
    sll $s3, $s3, 1
    j shift
right:
    li $a0, 300
    jal delay
    beq $s3, 0x0001, reset_right
    srl $s3, $s3, 1
    j shift
reset_right:
    li $s3, 0x0008
    j shift
reset_left:
    li $s3, 0x0001
    j shift

delay:  #delay(unsigned int ms)
	li $v0, 12  #resetCoreTimer()
	syscall
while_delay:
	li $v0, 11  #readCoreTimer()
	syscall
	mul $t0, $a0, 20000
	blt $v0, $t0, while_delay
	jr $ra
