																					;schedular as an asm function
T0IR EQU 0xE0004000
VICIntEnable EQU 0xFFFFF010
VICVectAddr EQU 0xFFFFF034
Mode_USR    EQU 0x10
VICVectCntl0 EQU 0xFFFFF200
U0THR  EQU 0xE000C000
U0LSR  EQU 0xE000C014

	 AREA context_switch,CODE,READONLY
	 EXPORT context_switch
     
	 LDMDA R8,{R0-R4}	            
     LDR R8,=0x40001F10
	 LDRB R8,[R8]

	 LDR R9,=3

	 TEQ R8,R9
	 BEQ PROC3

	 SUB R9,R9,1
	 TEQ R8,R9
	 BEQ PROC2

	 SUB R9,R9,1
	 TEQ R8,R9
	 BEQ PROC1

	 SUB R9,R9,1
	 TEQ R8,R9
	 BEQ PROC0

	 
PROC1
	  
	 ;LDR R10,=0x00000002				 ;else write back flag as 2
	 ;STR R10,[R8]

	 ;now we have to load process of 8000

	 STMDA R13,{R13}^               ;Get user mode current sp
	 LDMIA R13,{R8}                 ;Store user mode SP in R8
     SUB R8,R8,#4                   ;Reduce user sp by 4
	 LDR R9,=0X40001FDC
	 STR R8,[R9]
	 ;STMDA R13,{R8}                 ;Put it back to stack
	 ;LDMIA R13,{R13}^               ;User modes SP increased by 4
	 
	 LDR R9,=0x40001038				;Find the process return address
	 LDR R9,[R9]     				;Get the address
	 SUB R9,R9,#4                   ;Reduce return address by 4
	 STR R9,[R8]                    ;Store the return address on user mode's process stack
     ;LDR R8,=0X40001FE8
     ;LDMIA R8,{R0-R4}                ;POP WORKING REG
	 LDR R8,=0x4000607C
	 STMDA R8,{R0-R12,R14}^		        ;all resgisters saved
	 MRS R8, SPSR 
	 LDR R9,=0x40001F04
	 STR R8,[R9]

	 ;UART0 debugging
;	 LDR R8,=0x31
;	 LDR R9,=U0THR
;	 STR R8,[R9]


	 ;now to save the 1k reg of ram at 0x40003000
     LDR R14,=0x40003000
	 LDR R12,=0x40000000
	 LDR R11,=0x0000005B

CONTINUE1

	 LDMIA R12,{R0-R10}
	 STMIA R14,{R0-R10}
	 ADD R12,#0x0000002C
	 ADD R14,#0x0000002C
	 SUBS R11,#1
	 BCS CONTINUE1
	 B SCHEDULAR

;load the proc 4000 reg
L_2
	 LDR R14,=0x40000000
	 LDR R12,=0x40004000
	 LDR R11,=0x0000005B
	 ;LDR R11,=0x00000004

CONTINUE2

	 LDMIA R12,{R0-R10}
	 STMIA R14,{R0-R10}
	 ADD R12,#0x0000002C
	 ADD R14,#0x0000002C
	 SUBS R11,#1
	 BCS CONTINUE2
	 
	 LDR R8,=0X40001FE0
	 LDMIA R8,{R13}^

	 LDR R8,=0x40006088
	 LDMIA R8,{R0-R12,R14}^             ;Load the user mode register

	 LDR R13,=0x4000103C

	 LDR R8,=0x40001F08
	 LDR R8,[R8]
	 ;ORR R8,R8,#Mode_USR

     MSR     CPSR_c, #Mode_USR                   ;Change the mode to user mode
	 
	 STMDB R13!,{R0-R1}
	 LDR R0,=0x40001F08
	 LDR R0,[R0]
	 MSR CPSR_f,R0
	 LDMIA R13!,{R0-R1}

	 LDMIA R13!,{R15}
	

PROC0

     ;LDR R10,=0x00000001
	 ;LDR R8,=0X40001FFC				 ;else write back flag as 1
	 ;STR R10,[R8]

	 ;now we have to load process of 6000
	 
     STMDA R13,{R13}^               ;Get user mode current sp
	 LDMIA R13,{R8}                 ;Store user mode SP in R8
     SUB R8,R8,#4                   ;Reduce user sp by 4
	 LDR R9,=0X40001FD8
	 STR R8,[R9]
	 ;STMDA R13,{R8}                 ;Put it back to stack
	 ;LDMIA R13,{R13}^               ;User modes SP increased by 4
	 
	 LDR R9,=0x40001038				;Find the process return address
	 LDR R9,[R9]     				;Get the address
	 SUB R9,R9,#4                   ;Reduce return address by 4
	 STR R9,[R8]                    ;Store the return address on user mode's process stack
	 ;LDR R8,=0X40001FE8
     ;LDMIA R8,{R0-R4}                ;POP WORKING REG
	 LDR R8,=0x4000603C
	 STMDA R8,{R0-R12,R14}^;all resgisters saved
	 MRS R8, SPSR 
	 LDR R9,=0x40001F00
	 STR R8,[R9]

	 ;UART0 debugging
;	 LDR R0,=0x30
;	 LDR R1,=U0THR
;	 STR R0,[R1]


	 ;now to save the 1k reg of ram at 0x40006000
     LDR R14,=0x40002000
	 LDR R12,=0x40000000
	 LDR R11,=0x0000005B
	 ;LDR R11,=0x00000004

CONTINUE3

	 LDMIA R12,{R0-R10}
	 STMIA R14,{R0-R10}
	 ADD R12,#0x0000002C
	 ADD R14,#0x0000002C
	 SUBS R11,#1
	 BCS CONTINUE3
	 B SCHEDULAR

;load the proc 7000 reg
L_1
	 LDR R14,=0x40000000
	 LDR R12,=0x40003000
	 LDR R11,=0x0000005B
	 ;LDR R11,=0x00000004

CONTINUE4

	 LDMIA R12,{R0-R10}
	 STMIA R14,{R0-R10}
	 ADD R12,#0x0000002C
	 ADD R14,#0x0000002C
	 SUBS R11,#1
	 BCS CONTINUE4

	LDR R8,=0X40001FDC
	LDMIA R8,{R13}^

	 LDR R8,=0x40006048
	 LDMIA R8,{R0-R12,R14}^             ;Load the user mode register

	 LDR R13,=0x4000103C

	 LDR R8,=0x40001F04
	 LDR R8,[R8]
	 ORR R8,R8,#Mode_USR

     MSR     CPSR_c, #Mode_USR                   ;Change the mode to user mode 

	 STMDB R13!,{R0-R1}
	 LDR R0,=0x40001F04
	 LDR R0,[R0]
	 MSR CPSR_f,R0
	 LDMIA R13!,{R0-R1}
	  

	 LDMIA R13!,{R15}


PROC2
		 ;LDR R10,=0x00000003				 ;else write back flag as 3
	     ;STR R10,[R8]
    ;now we have to load process of 7000
	 
     STMDA R13,{R13}^               ;Get user mode current sp
	 LDMIA R13,{R8}                 ;Store user mode SP in R8
     SUB R8,R8,#4                   ;Reduce user sp by 4
	 LDR R9,=0X40001FE0
	 STR R8,[R9]
	 ;STMDA R13,{R8}                 ;Put it back to stack
	 ;LDMIA R13,{R13}^               ;User modes SP increased by 4
	 
	 LDR R9,=0x40001038				;Find the process return address
	 LDR R9,[R9]     				;Get the address
	 SUB R9,R9,#4                   ;Reduce return address by 4
	 STR R9,[R8]                    ;Store the return address on user mode's process stack
	 ;LDR R8,=0X40001FEC
     ;LDMIA R8,{R0-R4}                ;POP WORKING REG
	 LDR R8,=0x400060BC
	 STMDA R8,{R0-R12,R14}^		        ;all resgisters saved

	 MRS R8, SPSR 
	 LDR R9,=0x40001F08
	 STR R8,[R9]

	 ;UART0 debugging
;	 LDR R0,=0x32
;	 LDR R1,=U0THR
;	 STR R0,[R1]


	 ;now to save the 1k reg of ram at 0x40007C00
     LDR R14,=0x40004000
	 LDR R12,=0x40000000
	 LDR R11,=0x0000005B
	 ;LDR R11,=0x00000004

CONTINUE5

	 LDMIA R12,{R0-R10}
	 STMIA R14,{R0-R10}
	 ADD R12,#0x0000002C
	 ADD R14,#0x0000002C
	 SUBS R11,#1
	 BCS CONTINUE5
	 B SCHEDULAR

;load the proc 7000 reg
L_3
	 LDR R14,=0x40000000
	 LDR R12,=0x40005000
	 LDR R11,=0x0000005B
	 ;LDR R11,=0x00000004

CONTINUE6

	 LDMIA R12,{R0-R10}
	 STMIA R14,{R0-R10}
	 ADD R12,#0x0000002C
	 ADD R14,#0x0000002C
	 SUBS R11,#1
	 BCS CONTINUE6
	 LDR R8,=0X40001FE4
	 LDMIA R8,{R13}^


	 LDR R8,=0x400060C8
	 LDMIA R8,{R0-R12,R14}^             ;Load the user mode register

	 LDR R13,=0x4000103C

	 LDR R8,=0x40001F0C
	 LDR R8,[R8]
	 ORR R8,R8,#Mode_USR

     MSR     CPSR_c, #Mode_USR                   ;Change the mode to user mode 
	 STMDB R13!,{R0-R1}
	 LDR R0,=0x40001F0C
	 LDR R0,[R0]
	 MSR CPSR_f,R0
	 LDMIA R13!,{R0-R1}
	  

	 LDMIA R13!,{R15}








PROC3
	   ;LDR R10,=0x00000000				 ;else write back flag as 0
	   ;STR R10,[R8]
     ;now we have to load process of 7000
	 
     STMDA R13,{R13}^               ;Get user mode current sp
	 LDMIA R13,{R8}                 ;Store user mode SP in R8
     SUB R8,R8,#4                   ;Reduce user sp by 4
	 LDR R9,=0X40001FE4
	 STR R8,[R9]
	 ;STMDA R13,{R8}                 ;Put it back to stack
	 ;LDMIA R13,{R13}^               ;User modes SP increased by 4
	 
	 LDR R9,=0x40001038				;Find the process return address
	 LDR R9,[R9]     				;Get the address
	 SUB R9,R9,#4                   ;Reduce return address by 4
	 STR R9,[R8]                    ;Store the return address on user mode's process stack

	 ;LDR R8,=0X40001FEC
	 ;LDMIA R8,{R0-R4}
	 LDR R8,=0x400060FC
	 STMDA R8,{R0-R12,R14}^		        ;all resgisters saved

	 MRS R8, SPSR 
	 LDR R9,=0x40001F0C
	 STR R8,[R9]

	 ;UART0 debugging
;	 LDR R0,=0x33
;	 LDR R1,=U0THR
;	 STR R0,[R1]


	 ;now to save the 1k reg of ram at 0x40007C00
     LDR R14,=0x40005000
	 LDR R12,=0x40000000
	 LDR R11,=0x0000005B
	 ;LDR R11,=0x00000004

CONTINUE7

	 LDMIA R12,{R0-R10}
	 STMIA R14,{R0-R10}
	 ADD R12,#0x0000002C
	 ADD R14,#0x0000002C
	 SUBS R11,#1
	 BCS CONTINUE7

	 B SCHEDULAR

;load the proc 7000 reg
L_0
	 LDR R14,=0x40000000
	 LDR R12,=0x40002000
	 LDR R11,=0x0000005B
	 ;LDR R11,=0x00000004

CONTINUE8

	 LDMIA R12,{R0-R10}
	 STMIA R14,{R0-R10}
	 ADD R12,#0x0000002C
	 ADD R14,#0x0000002C
	 SUBS R11,#1
	 BCS CONTINUE8
	 LDR R8,=0X40001FD8
	 LDMIA R8,{R13}^

	 LDR R8,=0x40006008
	 LDMIA R8,{R0-R12,R14}^             ;Load the user mode register

	 LDR R13,=0x4000103C

	 LDR R8,=0x40001F00
	 LDR R8,[R8]
	 ORR R8,R8,#Mode_USR

     MSR     CPSR_c, #Mode_USR                   ;Change the mode to user mode 
	 STMDB R13!,{R0-R1}
	 LDR R0,=0x40001F00
	 LDR R0,[R0]
	 MSR CPSR_f,R0
	 LDMIA R13!,{R0-R1}
	  

	 LDMIA R13!,{R15}	 
	 
SCHEDULAR
   
	 ;Moving the queu up
     LDR R8,=0x40001F10
	 LDRB R10,[R8]

	 LDR R8,=0x40001F11
	 LDRB R11,[R8]

	 LDR R9,=0x40001F10
	 STRB R11,[R9]

     LDR R8,=0x40001F12
	 LDRB R11,[R8]

	 LDR R9,=0x40001F11
	 STRB R11,[R9]	 

	 LDR R8,=0x40001F13
	 LDRB R11,[R8]

	 LDR R9,=0x40001F12
	 STRB R11,[R9]

	 STRB R10,[R8]
	 ;top corresponds to the next incomming proc
	 ;now check if the proc is ready else repete

	 LDR R8,=0x40001F10              ;top of queu
	 LDRB R8,[R8]                    ;get it in

	 LDR R9,=0

	 TEQ R8,R9
	 BEQ ZERO


	 ADD R9,R9,#1
	 TEQ R8,R9
	 BEQ ONE

	 ADD R9,R9,#1
	 TEQ R8,R9
	 BEQ TWO

	 LDR R8,=0x40001F18
	 LDRB R8,[R8]
	 TEQ R8,#0
	 BEQ L_3
	 B SCHEDULAR

ZERO
	 LDR R8,=0x40001F15
	 LDRB R8,[R8]
	 TEQ R8,#0
	 BEQ L_0
	 B SCHEDULAR

ONE
	 LDR R8,=0x40001F16
	 LDRB R8,[R8]
	 TEQ R8,#0
	 BEQ L_1
	 B SCHEDULAR

TWO
	 LDR R8,=0x40001F17
	 LDRB R8,[R8]
	 TEQ R8,#0
	 BEQ L_2
	 B SCHEDULAR


	 AREA push_working_reg,CODE,READONLY
	 EXPORT push_working_reg

	 LDR R8,=0x40001FF8
	 STMDA R8,{R0-R4}

	 BX R14
     
	 END
	      