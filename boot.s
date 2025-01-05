#initialize _start function
	.global _start
#tell linker script that the .text section starts here
        .section .text
	#this SYS_EXIT code will be loaded into a0 which when
	#doing an ecall will exit boot... not what we need for our
	#current implementation of the program, but we did have it
	#implemented so we decided to keep it.
        .equ SYS_EXIT, 93

#this is the function the linker script starts at
_start:
	li sp, 0x87000000   #load sp at 0x87000000
	csrw sscratch, sp
        call main           #this will callback to main (c function)

#variable called counter that serves to keep track of how many times
#a process has been run, or more importantly, the number of switches
#between proga and progb that occur
counter:
	.word 0

#initialize switch_a function
        .global switch_a

#this function is used to be called from progb to load the pc of proga
#(which is 0x84000000) to increment counter, compare counter to 5, then
#run progb
switch_a:
        li t0, 0x84000000       #this will load proga pc to t0
	la t1, counter		#load address of the counter
	lw t2, 0(t1)		#load current value of counter
	addi t2, t2, 1		#increment counter by 1
	sw t2, 0(t1)		#store updated value
	li t3, 5		#load t3 with imm 5
	beq t2, t3, end_main_a	#if counter==5, sbi_shutdown()
        jalr zero, t0           #will jump and link and return pc at t0
	j switch_b		#switch to b

#initialize switch_b function
        .global switch_b

#this function is called from proga to load the pc of progb (0x84001000)
#then increment the counter, compare counter to 5, then run proga
switch_b:
        la t0, 0x84001000	#load progb pc to t0
	la t1, counter		#load address of the counter
	lw t2, 0(t1)		#load current value of counter
	addi t2, t2, 1		#increment counter by 1
	sw t2, 0(t1)		#store updated value
	li t3, 5		#load t3 with imm 5
	beq t2, t3, end_main_b	#if counter==5, sbi_shutdown()
        jalr zero, t0		#will jump and link and return pc at t0
	j switch_a		#switch to a

#initialize sbi_ecall function
        .global sbi_ecall

#sbi_ecall function, is used implement the sbi functions in sbi.c
sbi_ecall:
        ecall
        ret

#intialize sys_exit function
        .global sys_exit

#sys_exit code currently unused in our cooperative system
sys_exit:
        li a0, SYS_EXIT
        ecall

