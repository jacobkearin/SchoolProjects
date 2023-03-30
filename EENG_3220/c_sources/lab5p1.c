/*
 * lab5p1.c
 *
 * Created: 3/8/2023 2:35:12 PM
 * Author : keajacm
 *	Target: ATmega328P
 *	Description:	Toggle LED at 1Hz using CTC timer
 */ 


#include <avr/io.h>


int main(void)
{
	DDRB = 1<<5;
	OCR1A = 62499;
	TCCR1B = 0b00001100;

    /* Replace with your application code */
    
	while (1){
		if ((TIFR1 & 1<<OCF1A) == 0){
		}
		else {
			if ((PINB & 1<<PINB7)==0){
				PORTB ^= 1<<5;
				TIFR1 |= 1<<OCF1A;
			}
		}

	}
}

