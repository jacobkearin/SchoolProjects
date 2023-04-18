/*
 * lab7p1.c
 *
 * Created: 4/5/2023 2:55:42 PM
 * Author : keajacm
 * 
 * print to 16x2 LCD display
 * D0-D7 = PORTD.0-7
 * Enable = PORTB.0
 * Reg select = PORTB.1
 * LED Backlight = PORTB.2
 
 */


#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define enable PORTB0
#define RS PORTB1
#define backlight PORTB2
#define data PORTD
#define cmd PORTB


void lcd_Cmd(char command) {
	cmd &= ~(1<<RS);
	cmd &= ~(1<<enable);
	data = command;
	cmd |= (1<<enable);
	_delay_us(1);
	cmd &= ~ (1<<enable);
	_delay_us(100);
}

void lcd_init(void) {
	DDRD = 0xFF;	//data ddrd
	DDRB = 0x07;	//enable, RS, and backlight ddrb
	cmd = (1<<backlight);
	_delay_ms(100);
	
	lcd_Cmd(0x38);	//5x7, 8-bit
	lcd_Cmd(0x0F);	//display on, cursor blinking
	lcd_Cmd(0x01);	//clear
	_delay_ms(100);
	lcd_Cmd(0x02);	//cursor to home position
	_delay_us(2000);
}

void printChar(char ch) {
	data = ch;
	cmd |= (1<<RS);
	
	cmd |= (1<<enable);
	_delay_us(1);
	cmd &= ~ (1<<enable);
	_delay_us(100);	
}

void printString(char st[]) {
	int i;
	for (i = 0; st[i]; i++) {
		printChar(st[i]);
    }
}

int main(void) {
	lcd_init();
	printString("Hello World!");
	
    while (1) {}
}

