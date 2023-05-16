/*
 * Lab9_I2C.c
 *
 * Created: 4/19/2023 3:12:13 PM
 * Author : oldnicj
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>


void i2c_init(void){
	TWSR = 0;
	TWBR = 152;//SCL freq of 50k
	TWCR = 0x04;//enable TWI module
}

void i2c_start(void){
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
}

void i2c_stop(){
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void i2c_write(unsigned char data) {
	TWDR = data;
	TWCR = (1<< TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
}

unsigned char i2c_read(unsigned char isLast) {
	if(isLast ==0)
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);//send ACK
	else
		TWCR = (1<<TWINT)|(1<<TWEN);//send NACK
	while((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}

void rtc_init(unsigned char h, unsigned char  m, unsigned char s) {
	i2c_start();
	i2c_write(0b11010000);//address of RTC
	i2c_write(0x07);//setting register to 7, the control reg
	i2c_write(0x00);
	i2c_stop();
	
	_delay_us(100);
	
	i2c_start();
	i2c_write(0xD0);//address of RTC
	i2c_write(0);
	i2c_write(s);
	i2c_write(m);
	i2c_write(h);
	i2c_stop();
	
	return;
}

void rtc_getTime(unsigned char *h, unsigned char *m, unsigned char *s){
	i2c_start();
	i2c_write(0xD0);
	i2c_write(0);
	i2c_stop();
	
	i2c_start();
	i2c_write(0xD1);
	*s = i2c_read(1);
	*m = i2c_read(1);
	*h = i2c_read(0);
	i2c_stop();
}

void usart_init(void){
	UCSR0B = (1<<TXEN0);
	UCSR0C = (1<< UCSZ01)|(1<<UCSZ00);
	UBRR0L = 103;
}

void usart_sendByte(unsigned char ch){
	while(!(UCSR0A&(1<<UDRE0)));
	UDR0 = ch;
}

void usart_sendPackedBCD(unsigned char data){
	usart_sendByte('0' + (data>>4));
	usart_sendByte('0'+(data &0x0F));
	return;
}


int main(void){
	
	unsigned char hour, min, sec;
	
	i2c_init();
	rtc_init(0x06,0x04,0x13);//hh:mm:ss
    usart_init();
	
	
    while (1) 
    {
		rtc_getTime(&hour, &min, &sec);
		usart_sendPackedBCD(hour);
		usart_sendByte(':');
		usart_sendPackedBCD(min);
		usart_sendByte(':');
		usart_sendPackedBCD(sec);
		usart_sendByte('\n');
		usart_sendByte('\r');
		
		_delay_ms(200);
		
    }
}

