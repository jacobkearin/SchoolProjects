; part 2 solution
;
; Lab3.asm
;
; Created: 2/8/2023 12:05:39 PM
; Author : keajacm
;	Target: ATmega328P
;
;	Description: Adjustable-Speed, Bi-Directional Counter
;
;	Building upon your work in Part 1, use the ATmega328P Xplained Mini to build an adjustable-speed, bi-
;	directional counter.
;	Use the same hardware as was used in Part 1.
;	Utilize the code from Part 1, but enhance it with these features:
;	The code will still use the 7-segment display, but now it will operate as a counter.
;	The four switch inputs are now used to select the mode of operation:
;	Bit 0 – counter enable/disable: 0 = don’t count (hold value), 1 = count
;	Bit 1 – count up/down: 0 = count up, 1 = count down
;	Bits 3:2 – speed control: 00 = 1 Hz, 01 = 2 Hz, 10 = 3 Hz, 11 = 4 Hz
;	The counter will wrap in both directions:
;	When counting up, roll-over from 9 to 0
;	When counting down, roll-under from 0 to 9
;	You’ll want to create a delay function that checks input bits 3:2 to determine the delay.


start:
	LDI R20, 0xFF
	OUT DDRD, R20	;PORTD set to output
	LDI R20, 0B11110000
	OUT PORTC, R20
	LDI R28, 12
	LDI R29, 8
	LDI R30, 4
	LDI R31, 0

LOOP:
SEG0:
	LDI R16, 0b00111111	;load 0
	OUT PORTD, R16
	CALL DELAY
	CALL CHECK
	CPSE R17, R18
	JMP SEG9
	JMP SEG1

SEG1:
	LDI R16, 0b00000110	;load 1
	OUT PORTD, R16
	CALL DELAY
	CALL CHECK
	CPSE R17, R18
	JMP SEG0
	JMP SEG2

SEG2:
	LDI R16, 0b01011011	;load 2
	OUT PORTD, R16
	CALL DELAY
	CALL CHECK
	CPSE R17, R18
	JMP SEG1
	JMP SEG3

SEG3:
	LDI R16, 0b01001111	;load 3
	OUT PORTD, R16
	CALL DELAY
	CALL CHECK
	CPSE R17, R18
	JMP SEG2
	JMP SEG4

SEG4:
	LDI R16, 0b01100110	;load 4
	OUT PORTD, R16
	CALL DELAY
	CALL CHECK
	CPSE R17, R18
	JMP SEG3
	JMP SEG5

SEG5:
	LDI R16, 0b01101101	;load 5
	OUT PORTD, R16
	CALL DELAY
	CALL CHECK
	CPSE R17, R18
	JMP SEG4
	JMP SEG6

SEG6:
	LDI R16, 0b01111101	;load 6
	OUT PORTD, R16
	CALL DELAY
	CALL CHECK
	CPSE R17, R18
	JMP SEG5
	JMP SEG7

SEG7:
	LDI R16, 0b00000111	;load 7
	OUT PORTD, R16
	CALL DELAY
	CALL CHECK
	CPSE R17, R18
	JMP SEG6
	JMP SEG8

SEG8:
	LDI R16, 0b01111111	;load 8
	OUT PORTD, R16
	CALL DELAY
	CALL CHECK
	CPSE R17, R18
	JMP SEG7
	JMP SEG9

SEG9:
	LDI R16, 0b01101111	;load 9
	OUT PORTD, R16
	CALL DELAY
	CALL CHECK
	CPSE R17, R18
	JMP SEG8
	JMP SEG0

DELAY:
	IN R20, PINC	;load input to R20
	ANDI R20, 12
H4:
	CPSE R20, R28
	JMP H3
	CALL HZ12	; 1/12 hz signal called 3 times
	CALL HZ12
	CALL HZ12
	RET
H3:
	CPSE R20, R29	; 1/12 hz signal called 4 times
	JMP H2
	CALL HZ12
	CALL HZ12
	CALL HZ12
	CALL HZ12
	RET
H2:
	CPSE R20, R30	; 1/12 hz signal called 6 times
	JMP H1
	CALL HZ12
	CALL HZ12
	CALL HZ12
	CALL HZ12
	CALL HZ12
	CALL HZ12
	RET
H1:
	CPSE R20, R31	; 1/12 hz signal delay called 12 times
	JMP DELAY
	CALL HZ12
	CALL HZ12
	CALL HZ12
	CALL HZ12
	CALL HZ12
	CALL HZ12
	CALL HZ12
	CALL HZ12
	CALL HZ12
	RET

HZ12:
	LDI R26, 0xFF
L0:
	LDI R25, 0xFF
L1:
	DEC R25
	NOP
	NOP
	NOP
	NOP
	BRNE L1
	DEC R26
	NOP
	NOP
	NOP
	NOP
	BRNE L0
	RET

CHECK:
	IN R20, PINC	;load input to R20
	ANDI R20, 1
	LDI R22, 1
	CPSE R20, R22
	JMP DISABLE
	IN R17, PINC	;load input to R20
	ANDI R17, 2
	LDI R18, 0
	RET

DISABLE:
	IN R20, PINC	;load input to R20
	ANDI R20, 1
	LDI R22, 0
	CPSE R20, R22
	RET
	RJMP DISABLE
