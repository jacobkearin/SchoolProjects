/*
 * lab5p2.c
 *
 * Created: 3/15/2023 2:33:22 PM
 * Author : keajacm
 *	Target: ATmega328P
 *	Description:	Toggle LED at 1kHz using CTC timer and interrupts
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile int x = 1<<5;

ISR(TIMER1_COMPA_vect){
	uint8_t oldSREG = SREG;
	cli();
	x ^= 1<<5;
	TIFR1 |= 1<<OCF1A;
	SREG = oldSREG;
	sei();
}

int main(void)
{
	DDRB |= 1<<5;
	OCR1A = 249;
	TCCR1B = 0b00001011;
	TIMSK1 = 1<<OCIE1A;
	sei();
	
	while (1){
		PORTB = x;
	}
}
