.equ READ_CORE_TIMER, 11
.equ RESET_CORE_TIMER, 12
.equ PRINT_INT_10, 7
.equ PUT_CHAR, 3
.data
.text
.globl main

main:
    addiu $sp, $sp, -4
    sw $ra, 0($sp)

    li $a0, 10
    jal timeDone
    move $a0, $v0
    li $v0, PRINT_INT_10
    syscall
    j end

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

end:
    lw $ra, 0($sp)
    addiu $sp, $sp, 4
    jr $ra
