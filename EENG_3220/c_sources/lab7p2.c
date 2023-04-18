/*
 * lab7p2.c
 *
 * Created: 4/5/2023 2:55:42 PM
 * Author : keajacm
 * 
 * print to 16x2 LCD display from keypad input
 * D0-D7 = PORTD.0-7
 * Enable = PORTB.0
 * Reg select = PORTB.1
 * LED Backlight = PORTB.2
 * keyboard = PORTC.0:5, PORTB.3:4
 
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define enable PORTB0
#define RS PORTB1
#define backlight PORTB2
#define data PORTD
#define cmd PORTB

char keypad[4][4] = {'A','3','2','1',
					 'B','6','5','4',
					 'C','9','8','7',
					 'D',0x00,'0','*' };	//columns reversed from actual for scanning

char anchor[] = {0x04, 0x0A, 0x04, 0x1F, 0x04, 0x15, 0x0E, 0x00};

void lcd_Cmd(char command) {
	cmd &= ~(1<<RS);
	cmd &= ~(1<<enable);
	
	data = command;
	cmd |= (1<<enable);
	_delay_us(1);
	cmd &= ~ (1<<enable);
	_delay_us(100);
}

void printChar(char ch) {
	data = ch;
	cmd |= (1<<RS);
	
	cmd |= (1<<enable);
	_delay_us(1);
	cmd &= ~ (1<<enable);
	_delay_us(100);
}

void saveCG(char ch[]) {
	int i;
	for (i = 0; i<8; i++) {
		lcd_Cmd((0x40 + i));
		printChar(ch[i]);
	}
}

void lcd_init(void) {
	DDRD = 0xFF;	//data ddrd
	DDRB = 0x07;	//enable, RS, and backlight ddrb
	cmd = (1<<backlight);
	_delay_ms(100);
	lcd_Cmd(0x38);	//5x7, 8-bit
	lcd_Cmd(0x0F);	//display on, cursor blinking.
	saveCG(anchor);
	lcd_Cmd(0x01);	//clear
	_delay_ms(100);
	lcd_Cmd(0x02);	//cursor to home position
	_delay_us(2000);
}

void keyboard_init(void) {
	DDRC = 0x30;
	DDRB |= (1<<3);
	DDRB |= (1<<4);
	PORTC = 0xFF;
}

void printString(char st[]) {
	int i;
	for (i = 0; st[i]; i++) {
		printChar(st[i]);
	}
}

unsigned char pos(unsigned char col) {
	//takes raw column input and outputs 0-3 for keypad index
	//XXXX1110 -> 0
	//XXXX0111 -> 3
	col &= 0x0F;
	switch (col) {
		case 0x0E:
			return 0;
			break;
		case 0x0D:
			return 1;
			break;
		case 0x0B:
			return 2;
			break;
		case 0x07:
			return 3;
			break;
		default:
			return 5;
			break;
	}
}

char keyCheck(void) {
	unsigned char col;
	col = (PINC & 0x0F);
	while (col != 0x0F) {
		col = (PINC & 0x0F);
		_delay_ms(20);
	}
	PORTC &= ~(3<<4);
	PORTB &= ~(3<<3);
	_delay_ms(20);
	
	while (col == 0x0F) {
		col = (PINC & 0x0F);
		_delay_ms(20);
	}
	PORTC |= (3<<4);
	PORTB |= (1<<4); //turn on rows 1,2,3 leave off row 0.
	_delay_us(50);
	col = (PINC & 0x0F);
	if (col != 0x0F) {
		return keypad[0][pos(col)];
	}
	PORTB ^= (3<<3); //turn on row 0, turn off row 1.
	_delay_us(50);
	col = (PINC & 0x0F);
	if (col != 0x0F) {
		return keypad[1][pos(col)];
	}
	PORTC &= ~(1<<4);
	PORTB |= (3<<3); //turn on row 1, turn off row 2.
	_delay_us(50);
	col = (PINC & 0x0F);
	if (col != 0x0F) {
		return keypad[2][pos(col)];
	}
	PORTC ^= (3<<4); //turn on row 2, turn off row 3.
	_delay_us(50);
	col = (PINC & 0x0F);
	if (col != 0x0F) {
		return keypad[3][pos(col)];
	}
	else {
		PORTC &= ~(3<<4);
		PORTB &= ~(3<<3);
		return 0;
	}
	PORTC &= ~(3<<4);
	PORTB &= ~(3<<3);
}


int main(void) {
	lcd_init();
	keyboard_init();

    while (1) {
		char key = keyCheck();
		printChar(key);
    }
	return 0;
}
