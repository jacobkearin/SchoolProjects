;
; lab2.asm
;
; Created: 2/1/2023 2:58:32 PM
; Author : keajacm
;
;	Target: ATmega328P

; Replace with your application code
start:

	LDI	R20, 0x01
	OUT	SPH, R20
	LDI	R20, 0x98
	OUT	SPL, R20

	LDI R20, 0x20;
	LDI R21, 0x30;
	LDI R22, 0x40;
	LDI R23, 0x50;
	LDI R24, 0x60;
	LDI R25, 0x70;

	STS 0x019D, R20;
	STS 0x019C, R21;
	STS 0x019B, R22;
	STS 0x019A, R23;
	STS 0x0199, R24;
	STS 0x0198, R25;

	POP R20;
	POP R21;
	POP R22;
	POP R23;
	POP R24;
	POP R25;

L1:	RJMP	L1
