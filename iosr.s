IO0CLR EQU 0xE002800C
IO0SET EQU 0xE0028004
IO1CLR EQU 0xE002801C
IO1SET EQU 0xE0028014
Mode_USR        EQU     0x10
                             
	    					AREA IO_SR,CODE,READONLY
        					EXPORT IO_SR
        					MOV R8,R0
        					AND R8,R8,#3
	    					LDR R9,=0
							TEQ R8,R9
							BEQ IOR0
	    					ADD R9,R9,#1
        					TEQ R8,R9
							BEQ IOS0
	    					ADD R9,R9,#1
							TEQ R8,R9
	    					BEQ IOR1
							ADD R9,R9,#1
	    					TEQ R8,R9
							BEQ IOS1
	    					LDR R0,=0
        					LDMIA R13!,{R8-R12}
        					MOV R1,R14
	    					MSR CPSR_c,#Mode_USR
	    					BX R1





IOR0    				     MOV R8,R1
        				     LDR R9,=IO0CLR
                             LDR R10,[R9]
		                     LDR R11,=1
		                     ORR R10,R10,R11,LSL R8
		                     STR R10,[R9]
                             LDR R0,=1
                             LDMIA R13!,{R8-R12}
	                         MOV R1,R14
							 MSR CPSR_c,#Mode_USR
							 BX R1


IOS0                         MOV R8,R1
                             LDR R9,=IO0SET
							 LDR R10,[R9]
							 LDR R11,=1
							 ORR R10,R10,R11,LSL R8
							 STR R10,[R9]
                             LDR R0,=1
                             LDMIA R13!,{R8-R12}
							 MOV R1,R14
							 MSR CPSR_c,#Mode_USR
							 BX R1

IOR1                         MOV R8,R1
                             LDR R9,=IO1CLR
							 LDR R10,[R9]
							 LDR R11,=1
							 ORR R10,R10,R11,LSL R8
							 STR R10,[R9]
                             LDR R0,=1
                             LDMIA R13!,{R8-R12}
							 MOV R1,R14
							 MSR CPSR_c,#Mode_USR
							 BX R1

IOS1                         MOV R8,R1
                             LDR R9,=IO1SET
							 LDR R10,[R9]
							 LDR R11,=1
							 ORR R10,R10,R11,LSL R8
							 STR R10,[R9]
                             LDR R0,=1
                             LDMIA R13!,{R8-R12}
							 MOV R1,R14
							 MSR CPSR_c,#Mode_USR
							 BX R1
						     END
 

							