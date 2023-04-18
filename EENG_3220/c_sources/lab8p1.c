/*
 * lab8p1.c
 *
 * Created: 4/12/2023 2:35:53 PM
 * Author : keajacm
 * use AVR built-in ADC to measure two analog values, as well as
 * internal temp sensor and send values over UART
 *
 */

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

const unsigned char adc0mux = 0x40; //adcmux values for adc0, adc1, and internal temp sensor
const unsigned char adc1mux = 0x41;
const unsigned char itempmux = 0xC8;


void uart_init (void) {		//initialize UART
	UCSR0B = (1<<TXEN0)|(1<<RXEN0)|(1<<RXCIE0);		//TX, RX enable, RX interrupt enable
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);				//8-bit
	UBRR0L = 103;									//9600 baud
}

void uartTX_CH (unsigned char ch) {		//send a character out serial port
	while (! (UCSR0A & (1<<UDRE0)));	//wait until UDR0 is empty
	UDR0 = ch;	//transmit ch
}

void uartTX_ST (char st[]) {	//send a string of characters out serial port
	int i;
	for (i = 0; st[i]; i++) {
		while (! (UCSR0A & (1<<UDRE0))); //wait until UDR0 is empty
		UDR0 = st[i];
	}
}

void uart_send_dec (uint16_t num) {
	char buffer[4];
	int i = 0;
	while (num) {
		buffer[i] = (num % 10) + '0';
		num /= 10;
		i++;
	}
	while (i) {
		i--;
		uartTX_CH(buffer[i]);
	}
}

void adc_init (void) {
	DDRC = 0;
	ADCSRA = 0x87;
}

volatile uint16_t get_adc(unsigned char mux) {
	ADMUX = mux;
	ADCSRA |= (1<<ADSC);					//start adc
	while (ADCSRA & (1<<ADSC));				//wait to finish
	return ADC;
}

int main(void) {
	uart_init();
	adc_init();
    while (1) {
		uart_send_dec(get_adc(adc0mux));
		uartTX_ST(", ");
		uart_send_dec(get_adc(adc1mux));
		uartTX_ST(", ");
		uart_send_dec(get_adc(itempmux));
		uartTX_ST(" \n\r");
		_delay_ms(1000);
    }
}

