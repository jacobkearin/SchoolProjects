/*
 * lab6p1.c
 *
 * Created: 3/29/2023 2:31:55 PM
 * Author : keajacm
 */ 

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>


void uart_init (void) {
	UCSR0B = (1<<TXEN0);				//TX enable
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);	//8-bit
	UBRR0L = 103;						//9600 baud
}

void putCH (unsigned char ch) {
	while (! (UCSR0A & (1<<UDRE0))); //wait until UDR0 is empty
	UDR0 = ch;	//transmit ch
}



int main(void) {
	uart_init();
	
    while (1) {
		_delay_ms(100);
		putCH('J');
    }
}

