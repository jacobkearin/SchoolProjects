/*
 * main.c
 *
 * Created: 2/22/2023 2:37:56 PM
 *  Author: keajacm
 *	Target: ATmega328P
 *	Description:	Implement a 32-bit PRNG to produce a pseudo-random sequence of bytes, to be displayed on
 *					eight LEDs. Use a debounced pushbutton to activate the calculation and display of the next value.
 */ 
#define F_CPU 16000000UL
#include <util/delay.h>
#include <xc.h>
#include <avr/io.h>


int main(void)
{	
	DDRD = 0xFF;
	unsigned long X = 521288629;
	unsigned long Y = 362436069;
	unsigned int A = 18000;
	unsigned int B = 30903;
	char input = 0;
	
	while(1){
		_delay_ms(50);
		input = PINB;
		if (input & 0b00100000) {
			_delay_ms(50);
			X = ((X & 0xFFFF) * A) + (X>>16);
			Y = ((Y & 0xFFFF) * B) + (Y>>16);
			PORTD = (((X & 0xFFFF) + (Y & 0xFFFF)) / 2) & 0xFF;
		}
	
		while (input & 0b00100000) {
			input = PINB;
		}
	}
}