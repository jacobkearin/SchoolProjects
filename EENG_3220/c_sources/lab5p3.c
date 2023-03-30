/*
 * GccApplication1.c
 *
 * Created: 3/15/2023 3:42:23 PM
 * Author : keajacm
 *	Target: ATmega328P
 *	Description:	implement millis() to toggle LED at 0.5Hz using CTC timer and interrupts
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile long mils = 0;

ISR(TIMER1_COMPA_vect){
	mils += 1;
	TIFR1 |= 1<<OCF1A;
}

unsigned long millis(void) {
	unsigned long ms;
	uint8_t oldSREG = SREG;
	cli();
	ms = mils;
	SREG = oldSREG;
	sei();
	return ms;
}

int main(void)
{
	DDRB |= 1<<5;
	OCR1A = 249;
	TCCR1B = 0b00001011;
	TIMSK1 = 1<<OCIE1A;
	sei();
	int x = 0;
	int y = 0;
	
	while (1){
		x = millis() - y;
		if (x >= 500) {
			PORTB ^= 1<<5;
			y += 500;
		}
	}
}