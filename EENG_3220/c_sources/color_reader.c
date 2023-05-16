/*
 * color_reader.c
 *
 * Target: ATmega328P
 * 
 * Description:
 *      Uses TCS34725 RGB sensor to sense color and displays values out
 *      on 20x4 LCD character display. Sensor and display are both i2c.
 * 
 *      TCS breakout board LEDs on PB0, hold button on INT1 
 * 
 * Created: 5/16/2023
 * Author : keajacm
 */ 

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>


#define LCDaddr 0x27<<1		//bit0: 0 for write, 1 for read
#define TCSaddr 0x29<<1

#define LCDprintCMD 0x0d	//
#define LCDaddrCMD 0x0c		//

#define REDlow 0x16			//address for red data low byte
#define GREENlow 0x18		//address for green data low byte
#define BLUElow 0x1a		//address for blue data low byte

#define redLoc 0xcc
#define greenLoc 0xa0
#define blueLoc 0xe0

static volatile uint8_t globvar = 0x00;

void i2c_init(void) {
	TWSR = 0;
	TWBR = 152;		//SCL freq of 50k
	TWCR = 0x04;	//enable TWI module
}

void i2c_start(void) {
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
}

void i2c_stop() {
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void i2c_write(uint8_t data) {
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while((TWCR & (1<<TWINT)) == 0);
}

uint8_t i2c_read(uint8_t isLast) {
	if(isLast == 0)
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);//send ACK
	else
	TWCR = (1<<TWINT)|(1<<TWEN);//send NACK
	while((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}

void LCD_send(uint8_t data, uint8_t cmd) {	//send LCD byte over i2c
	i2c_start(); 
	i2c_write(LCDaddr); 
	i2c_write((cmd & 0x0f) | (data & 0xf0)); 
	i2c_stop();
	
	i2c_start(); 
	i2c_write(LCDaddr); 
	i2c_write((cmd & 0x0b) | (data & 0xf0)); 
	i2c_stop();
	
	i2c_start(); 
	i2c_write(LCDaddr); 
	i2c_write((cmd & 0x0f) | ((data<<4) & 0xf0));
	i2c_stop();
	
	i2c_start(); 
	i2c_write(LCDaddr); 
	i2c_write((cmd & 0x0b) | ((data<<4) & 0xf0));
	i2c_stop();	
}

void lcd_init(void) {
	LCD_send(0x33, 0x04);
	_delay_ms(10);
	LCD_send(0x32, 0x04);
	_delay_ms(10);
	LCD_send(0x28, 0x04);
	_delay_ms(10);
	LCD_send(0x00, 0x04);
	_delay_ms(10);
	LCD_send(0x01, 0x04);
	_delay_ms(10);
	LCD_send(0x06, 0x04);
	_delay_ms(10);
	LCD_send(0x02, 0x04);
	_delay_ms(1000);
	LCD_send(0x0c, 0x04);
	
	LCD_send(0x88, LCDaddrCMD);
	LCD_send('-', LCDprintCMD);
	LCD_send('-', LCDprintCMD);
	LCD_send('-', LCDprintCMD);
	LCD_send('-', LCDprintCMD);
	
	LCD_send(0xc3, LCDaddrCMD);
	LCD_send('R', LCDprintCMD);
	LCD_send('E', LCDprintCMD);
	LCD_send('D', LCDprintCMD);
	LCD_send(':', LCDprintCMD);
	
	LCD_send(0x97, LCDaddrCMD);
	LCD_send('G', LCDprintCMD);
	LCD_send('R', LCDprintCMD);
	LCD_send('E', LCDprintCMD);
	LCD_send('E', LCDprintCMD);
	LCD_send('N', LCDprintCMD);
	LCD_send(':', LCDprintCMD);
	
	LCD_send(0xd7, LCDaddrCMD);
	LCD_send('B', LCDprintCMD);
	LCD_send('L', LCDprintCMD);
	LCD_send('U', LCDprintCMD);
	LCD_send('E', LCDprintCMD);
	LCD_send(':', LCDprintCMD);
}

void TCS_cmd(uint8_t cmd, uint8_t data) {
	i2c_start();
	i2c_write(TCSaddr);
	i2c_write(cmd);
	i2c_write(data);
	i2c_stop();
}

uint16_t TCS_read_color(uint8_t addr) {
	i2c_start();
	i2c_write(TCSaddr);
	i2c_write(addr | 0x80);
	i2c_start();
	i2c_write(TCSaddr | 0x01);
	uint8_t low = i2c_read(0);
	uint8_t high = i2c_read(1);
	i2c_stop();
	return ((high<<8) | low);
}

void tcs_init(void) {
	DDRB = 0x01;
	PORTB = 0x01;		//TCS breakout board LEDs on PB0
	i2c_start();
	i2c_write(TCSaddr);
	i2c_stop();
	
	TCS_cmd(0x81, 0x00);
	TCS_cmd(0x8F, 0x01);
	TCS_cmd(0x80, 0x01);
	_delay_ms(5);
	TCS_cmd(0x80, 0x03);
	_delay_ms(25);
}

void int_init(void) {
	
	EICRA = 3 << 2;		//HOLD button on INT1
	EIMSK = 1 << INT1;
}

ISR(INT1_vect) {		//tracking button press
	globvar = ~globvar;
	_delay_ms(10);      //debounce
}

uint8_t bit_to_ascii(uint8_t val) {
	if (val < 10) {
		return (val + '0');
	}
	else {
		return (val + 'A' - 10);
	}
}

void update_color(uint8_t lcd_loc, uint16_t value) {
	uint8_t char3, char2, char1, char0;
	char3 = bit_to_ascii((value>>12) & 0x0f);
	char2 = bit_to_ascii((value>>8) & 0x0f);
	char1 = bit_to_ascii((value>>4) & 0x0f);
	char0 = bit_to_ascii((value) & 0x0f);
	LCD_send(lcd_loc, LCDaddrCMD);
	LCD_send(char3, LCDprintCMD);
	LCD_send(char2, LCDprintCMD);
	LCD_send(char1, LCDprintCMD);
	LCD_send(char0, LCDprintCMD);
}

int main(void) {
	
	uint16_t REDval = 0;
	uint16_t GREENval = 0;
	uint16_t BLUEval = 0;
	
	i2c_init();
	tcs_init();
	lcd_init();
	int_init();
	
	sei();
	
	uint8_t toggle = 0;
	
	while(1) {
			
		if (globvar) {
			LCD_send(0x88, LCDaddrCMD);
			LCD_send('H', LCDprintCMD);
			LCD_send('O', LCDprintCMD);
			LCD_send('L', LCDprintCMD);
			LCD_send('D', LCDprintCMD);
			_delay_ms(50);
		}
		
		else {
			REDval = TCS_read_color(REDlow);
			GREENval = TCS_read_color(GREENlow);
			BLUEval = TCS_read_color(BLUElow);
			
			LCD_send(0x88, LCDaddrCMD);
			LCD_send('-', LCDprintCMD);
			LCD_send('-', LCDprintCMD);
			LCD_send('-', LCDprintCMD);
			LCD_send('-', LCDprintCMD);
			
			update_color(redLoc, REDval);
			update_color(greenLoc, GREENval);
			update_color(blueLoc, BLUEval);
			
			_delay_ms(700);		//minimum 614ms delay for highest resolution ADC values
		}
	}
}
