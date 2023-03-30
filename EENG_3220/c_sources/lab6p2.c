/*
 * lab6p2.c
 *
 * Created: 3/29/2023 3:20:23 PM
 * Author : keajacm
 */ 

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>


void uart_init (void) {
	UCSR0B = (1<<TXEN0)|(1<<RXEN0)|(1<<RXCIE0);		//TX, RX enable, RX interrupt enable
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);				//8-bit
	UBRR0L = 103;									//9600 baud
}

void putCH (unsigned char ch) {
	while (! (UCSR0A & (1<<UDRE0))); //wait until UDR0 is empty
	UDR0 = ch;	//transmit ch
}

unsigned char getCH (void) {
	while (! (UCSR0A & (1<<RXC0))); //wait until new data
    unsigned char ch = UDR0;
	return ch;
}

int main(void) {
	
	uart_init();
	
	while (1) {
		unsigned char variable = getCH();
		_delay_ms(1000);
		putCH(variable);
	}
	
	return 0;
}

