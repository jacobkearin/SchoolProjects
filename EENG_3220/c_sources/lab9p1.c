/*
 * lab9p1.c
 *
 * Created: 4/19/2023 2:37:50 PM
 * Author : keajacm
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define MOSI 3
#define SCK 5
#define SS 2

const uint8_t readinst = 3;
const uint8_t writeinst = 2;

void uart_init (void) {
	UCSR0B = (1<<TXEN0)|(1<<RXEN0)|(1<<RXCIE0);		//TX, RX enable, RX interrupt enable
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);				//8-bit
	UBRR0L = 103;									//9600 baud
}

void SPI_init(void) {
	DDRB = (1<<MOSI)|(1<<SCK)|(1<<SS);
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void putCH (uint8_t ch) {
	while (! (UCSR0A & (1<<UDRE0))); //wait until UDR0 is empty
	UDR0 = ch;	//transmit ch
}

void SPI_write(uint8_t value) {
	PORTB &= ~(1<<SS);
	SPDR = value;
	while (!(SPSR & (1<<SPIF)));
}

void sram_write_byte(uint16_t addr, uint8_t data) {
	PORTB &= ~(1<<SS);
	SPDR = writeinst;
	while (!(SPSR & (1<<SPIF)));
	SPDR = addr >> 8;
	while (!(SPSR & (1<<SPIF)));
	SPDR = addr & 0xFF;
	while (!(SPSR & (1<<SPIF)));
	SPDR = data;
	while (!(SPSR & (1<<SPIF)));
	PORTB |= (1<<SS);
}

uint8_t sram_read_byte(uint16_t addr) {
	PORTB &= ~(1<<SS);
	SPDR = readinst;
	while (!(SPSR & (1<<SPIF)));
	SPDR = addr >> 8;
	while (!(SPSR & (1<<SPIF)));
	SPDR = addr & 0xFF;
	while (!(SPSR & (1<<SPIF)));
	SPI_write(0xFF);	
	uint8_t data = SPDR;
	PORTB |= (1<<SS);
	return data;
}

int main(void) {
	
	uart_init();
	SPI_init();
	
	SPI_write(0x01);
	SPI_write(0x00);
	
    while (1) { 
		
		sram_write_byte(0x00FF, 'H');
		sram_write_byte(0x0100, 'e');
		sram_write_byte(0x0101, 'l');
		sram_write_byte(0x0102, 'l');
		sram_write_byte(0x0103, 'o');
		uint8_t receive = sram_read_byte(0x00FF);
		putCH(receive);
		receive = sram_read_byte(0x0100);
		putCH(receive);
		receive = sram_read_byte(0x0101);
		putCH(receive);
		receive = sram_read_byte(0x0102);
		putCH(receive);
		receive = sram_read_byte(0x0103);
		putCH(receive);
        
		
		PORTB |= (1<<SS);
		_delay_ms(1000);
		
    }
}

