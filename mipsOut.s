### MIPS EXECUTABLE BY TOUCAN COMPILER ###

	.data
stra: .asciiz "TEXE calculated result: "
	.text

#START_MAIN#
main:
	addi $sp, $sp, -4 			# push ra
	sw   $ra, 0($sp)

	li   $v0, 4 				# display string
	la   $a0, stra
	syscall

								# funcall start
	addi $t0, $zero, 24 			# intLiteral -> t0
	addi $sp, $sp, -4 			# push t0
	sw   $t0, 0($sp)

	jal fib 					# jump to fun
	lw   $t1, 0($sp) 			# pop in t1
	addi $sp, $sp, 4
	addi $sp, $sp, 4
	addi $sp, $sp, -4 			# push t1
	sw   $t1, 0($sp)
								# funcall end

	lw   $a0, 0($sp) 			# pop in a0
	addi $sp, $sp, 4
	li   $v0, 1 				# display a0
	syscall

	lw	  $ra, 0($sp) 			# pop in ra
	addi $sp, $sp, 4
	jr	  $ra 					# jump ra


#FUNCTIONS#

fib:
	move $t6, $t7 				# calleeSave t7 in t6
	move $t7, $sp 				# save varPointer in t7
	addi $sp, $sp, -4 			# push ra
	sw	  $ra, 0($sp)

	addi $sp, $sp, -4 			# push t6 (calleeSave t7)
	sw	  $t6, 0($sp)

	lw   $t0, 0($t7) 			# getVariable(t7) in t0
	addi $sp, $sp, -4 			# push t0
	sw   $t0, 0($sp)

	lw   $t0, 0($sp) 			# pop in t0
	addi $sp, $sp, 4
	bgtz  $t0, IFLABL100 		# jump if t0>0
	addi $t0, $zero, 0 			# intLiteral -> t0
	addi $sp, $sp, -4 			# push t0
	sw   $t0, 0($sp)

	j IFLABL101

IFLABL100: 						# else part
	lw   $t0, 0($t7) 			# getVariable(t7) in t0
	addi $sp, $sp, -4 			# push t0
	sw   $t0, 0($sp)

	addi $t0, $zero, 1 			# intLiteral -> t0
	addi $sp, $sp, -4 			# push t0
	sw   $t0, 0($sp)

	lw   $t1, 0($sp) 			# pop in t1
	addi $sp, $sp, 4
	lw   $t0, 0($sp) 			# pop in t0
	addi $sp, $sp, 4
	sub  $t2, $t0, $t1 			# [-] t0, t1 -> t2
	addi $sp, $sp, -4 			# push t2
	sw   $t2, 0($sp)

	lw   $t0, 0($sp) 			# pop in t0
	addi $sp, $sp, 4
	bgtz  $t0, IFLABL102 		# jump if t0>0
	addi $t0, $zero, 1 			# intLiteral -> t0
	addi $sp, $sp, -4 			# push t0
	sw   $t0, 0($sp)

	j IFLABL103

IFLABL102: 						# else part
								# funcall start
	lw   $t0, 0($t7) 			# getVariable(t7) in t0
	addi $sp, $sp, -4 			# push t0
	sw   $t0, 0($sp)

	addi $t0, $zero, 1 			# intLiteral -> t0
	addi $sp, $sp, -4 			# push t0
	sw   $t0, 0($sp)

	lw   $t1, 0($sp) 			# pop in t1
	addi $sp, $sp, 4
	lw   $t0, 0($sp) 			# pop in t0
	addi $sp, $sp, 4
	sub  $t2, $t0, $t1 			# [-] t0, t1 -> t2
	addi $sp, $sp, -4 			# push t2
	sw   $t2, 0($sp)

	jal fib 					# jump to fun
	lw   $t1, 0($sp) 			# pop in t1
	addi $sp, $sp, 4
	addi $sp, $sp, 4
	addi $sp, $sp, -4 			# push t1
	sw   $t1, 0($sp)
								# funcall end

								# funcall start
	lw   $t0, 0($t7) 			# getVariable(t7) in t0
	addi $sp, $sp, -4 			# push t0
	sw   $t0, 0($sp)

	addi $t0, $zero, 2 			# intLiteral -> t0
	addi $sp, $sp, -4 			# push t0
	sw   $t0, 0($sp)

	lw   $t1, 0($sp) 			# pop in t1
	addi $sp, $sp, 4
	lw   $t0, 0($sp) 			# pop in t0
	addi $sp, $sp, 4
	sub  $t2, $t0, $t1 			# [-] t0, t1 -> t2
	addi $sp, $sp, -4 			# push t2
	sw   $t2, 0($sp)

	jal fib 					# jump to fun
	lw   $t1, 0($sp) 			# pop in t1
	addi $sp, $sp, 4
	addi $sp, $sp, 4
	addi $sp, $sp, -4 			# push t1
	sw   $t1, 0($sp)
								# funcall end

	lw   $t1, 0($sp) 			# pop in t1
	addi $sp, $sp, 4
	lw   $t0, 0($sp) 			# pop in t0
	addi $sp, $sp, 4
	add  $t2, $t0, $t1 			# [+] t0, t1 -> t2
	addi $sp, $sp, -4 			# push t2
	sw   $t2, 0($sp)

IFLABL103: 						# endif
IFLABL101: 						# endif
	lw	  $t0, 0($sp) 			# pop in t0
	addi $sp, $sp, 4
	lw	  $t7, 0($sp) 			# pop in t7
	addi $sp, $sp, 4
	lw	  $ra, 0($sp) 			# pop in ra
	addi $sp, $sp, 4
	addi $sp, $sp, -4 			# push t0
	sw	  $t0, 0($sp)

	jr   $ra 					# jump ra


fac:
	move $t6, $t7 				# calleeSave t7 in t6
	move $t7, $sp 				# save varPointer in t7
	addi $sp, $sp, -4 			# push ra
	sw	  $ra, 0($sp)

	addi $sp, $sp, -4 			# push t6 (calleeSave t7)
	sw	  $t6, 0($sp)

	lw   $t0, 0($t7) 			# getVariable(t7) in t0
	addi $sp, $sp, -4 			# push t0
	sw   $t0, 0($sp)

	lw   $t0, 0($sp) 			# pop in t0
	addi $sp, $sp, 4
	bgtz  $t0, IFLABL104 		# jump if t0>0
	addi $t0, $zero, 1 			# intLiteral -> t0
	addi $sp, $sp, -4 			# push t0
	sw   $t0, 0($sp)

	j IFLABL105

IFLABL104: 						# else part
	lw   $t0, 0($t7) 			# getVariable(t7) in t0
	addi $sp, $sp, -4 			# push t0
	sw   $t0, 0($sp)

								# funcall start
	lw   $t0, 0($t7) 			# getVariable(t7) in t0
	addi $sp, $sp, -4 			# push t0
	sw   $t0, 0($sp)

	addi $t0, $zero, 1 			# intLiteral -> t0
	addi $sp, $sp, -4 			# push t0
	sw   $t0, 0($sp)

	lw   $t1, 0($sp) 			# pop in t1
	addi $sp, $sp, 4
	lw   $t0, 0($sp) 			# pop in t0
	addi $sp, $sp, 4
	sub  $t2, $t0, $t1 			# [-] t0, t1 -> t2
	addi $sp, $sp, -4 			# push t2
	sw   $t2, 0($sp)

	jal fac 					# jump to fun
	lw   $t1, 0($sp) 			# pop in t1
	addi $sp, $sp, 4
	addi $sp, $sp, 4
	addi $sp, $sp, -4 			# push t1
	sw   $t1, 0($sp)
								# funcall end

	lw   $t1, 0($sp) 			# pop in t1
	addi $sp, $sp, 4
	lw   $t0, 0($sp) 			# pop in t0
	addi $sp, $sp, 4
	mul  $t2, $t0, $t1 			# [*] t0, t1 -> t2
	addi $sp, $sp, -4 			# push t2
	sw   $t2, 0($sp)

IFLABL105: 						# endif
	lw	  $t0, 0($sp) 			# pop in t0
	addi $sp, $sp, 4
	lw	  $t7, 0($sp) 			# pop in t7
	addi $sp, $sp, 4
	lw	  $ra, 0($sp) 			# pop in ra
	addi $sp, $sp, 4
	addi $sp, $sp, -4 			# push t0
	sw	  $t0, 0($sp)

	jr   $ra 					# jump ra


