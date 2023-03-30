;
; lab4_asm.asm
;
; Created: 2/22/2023 2:42:31 PM
; Author : keajacm
;	Target: ATmega328P
;
;	Description:	Implement a 32-bit PRNG to produce a pseudo-random sequence of bytes, to be displayed on
:					eight LEDs. Use a debounced pushbutton to activate the calculation and display of the next value.


; Replace with your application code
start:
	.DEF X0 = R20
	.DEF X1 = R21
	.DEF X2 = R22
	.DEF X3 = R23

	.DEF Y0 = R24
	.DEF Y1 = R25
	.DEF Y2 = R26
	.DEF Y3 = R27

	.DEF OUT0 = R28
	.DEF OUT1 = R29
	.DEF OUT2 = R30
	.DEF OUT3 = R31

	.DEF AL = R10
	.DEF AH = R11
	.DEF BL = R12
	.DEF BH = R13

	LDI X0, 0xB5	;X = R23:R20 = $1F123BB5 = 521288629 
	LDI X1, 0x3B
	LDI X2, 0x12
	LDI X3, 0x1F

	LDI Y0, 0xE5	;Y = R27:R24 = $159A55E5 = 362436069
	LDI Y1, 0x55
	LDI Y2, 0x9A
	LDI Y3, 0x15

	LDI R28, 0x50	;A = R11:R10 = $4650 = 18000
	LDI R29, 0x46
	MOVW AL, R28

	LDI R28, 0xB7	;B = R13:R12 = $78B7 = 30903
	LDI R29, 0x78
	MOVW BL, R28

	LDI R16, 0xFF
	OUT DDRD, R16


waitpress:
	IN R19, PINB
	SBRS R19, 5
	RJMP waitpress
	CALL delay
	CALL PRNG
	CALL waitrelease
	RJMP waitpress
	

delay:
	LDI R17, 0xFF
L0:
	LDI R18, 0xFF
L1:
	DEC R18
	BRNE L1
	DEC R17
	BRNE L0

	RET


PRNG:
	;__________ XL = XL * A 
	LDI OUT0, 0
	LDI OUT1, 0
	LDI OUT2, 0
	LDI OUT3, 0

	MUL X0, AL
	MOVW OUT0, R0
	
	MUL X0, AH
	ADD OUT1, R0
	ADC OUT2, R1

	MUL X1, AL
	ADD OUT1, R0
	ADC OUT2, R1

	MUL X1, AH
	ADD OUT2, R0
	ADC OUT3, R1

	ADD OUT0, X2 ;XH>>16 + out
	ADC OUT1, X3

	MOVW X0, OUT0 ;put back into X
	MOVW X2, OUT2

	;___________YL = YL * B
	LDI OUT0, 0
	LDI OUT1, 0
	LDI OUT2, 0
	LDI OUT3, 0

	MUL Y0, BL
	MOVW OUT0, R0
	
	MUL Y0, BH
	ADD OUT1, R0
	ADC OUT2, R1

	MUL Y1, BL
	ADD OUT1, R0
	ADC OUT2, R1

	MUL Y1, BH
	ADD OUT2, R0
	ADC OUT3, R1

	ADD OUT0, Y2 ;YH>>16 + out
	ADC OUT1, Y3

	MOVW Y0, OUT0 ;put back into Y
	MOVW Y2, OUT2

	;____________ (XL + YL)/2
	ADD OUT0, X0
	ADC OUT1, X1
	LSR OUT1
	ROR OUT0
	
	OUT PORTD, OUT0
	RET


waitrelease:
	IN R19, PINB
	SBRC R19, 5
	RJMP waitrelease
	CALL delay
	JMP waitpress
	

	

