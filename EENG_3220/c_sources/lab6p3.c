/*
 *	lab6p3.c
 *
 *	Created: 3/29/2023 3:55:17 PM
 *	Author : keajacm
 *	Target: ATmega328P
 *	Description: Serial interface to ATmega329P EEPROM
 *	todo: reformat and program to use interrupts instead of waiting for inputs
 */ 

#include <avr/io.h>

unsigned int eeadr = 0;

void uart_init (void) {		//initialize UART
	UCSR0B = (1<<TXEN0)|(1<<RXEN0)|(1<<RXCIE0);		//TX, RX enable, RX interrupt enable
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);				//8-bit
	UBRR0L = 103;									//9600 baud
}

void putCH (unsigned char ch) {		//send a character out serial port
	while (! (UCSR0A & (1<<UDRE0)));	//wait until UDR0 is empty
	UDR0 = ch;	//transmit ch
}

unsigned char getCH (void) {	//receive a character from serial port
	while (! (UCSR0A & (1<<RXC0)));		//wait until new data
	unsigned char ch = UDR0;
	return ch;
}

unsigned char hexcheck(unsigned char ch) {		//return 0 if input is a valid hex digit
	if (((ch>='0')&&(ch<='9'))||((ch>='A')&&(ch<='F'))||((ch>='a')&&(ch<='f'))) {
		return 0;
	}
	else {
		return 1;
	}
}

unsigned char hexToValue(unsigned char ch) {	//return 0-15 value from ascii hex character
	if ((ch>='0')&&(ch<='9')) {
		return (ch-'0');
	}
	else if ((ch>='A')&&(ch<='F')) {
		return (ch - 'A' + 10);
	}
	else if ((ch>='a')&&(ch<='f')) {
		return (ch - 'a' + 10);
	}
}

void putST (char st[]) {	//send a string of characters out serial port
	int i;
	for (i = 0; st[i]; i++) {
		while (! (UCSR0A & (1<<UDRE0))); //wait until UDR0 is empty
		UDR0 = st[i];
	}
}

void putHEX(unsigned char ch) {		//send a hex character out serial from 0-15 input
	if (ch<10) {
		putCH(ch+'0');
	}
	else {
		putCH('A' + ch - 10);
	}
}

void prog_init (void) {
	uart_init();
	putST("\n\rWelcome\n\r");
}

void help (void) {
	putST("\n\r----- Help -----\n\r");
	putST("D: dump EEPROM in hex\n\r");
	putST("p: print EEPROM as string\n\r");
	putST("E: erase EEPROM\n\r");
	putST("S: save new string to EEPROM\n\r");
	putST("H: save new hex data to EEPROM\n\r");
	putST("?: help\n\r");
}

int main(void) {
	
	prog_init();
	
	unsigned char Op;	//command input variable
	unsigned char ch;	//data variable
	int temp = 1;
	
	while (1) {
		putST("->");
		Op = getCH();
		
		switch(Op) {
			case 'D':	//dump eeprom as hex
				putST("Dump EEPROM as Hex:");
				for (unsigned int i = 0; i<32; i++) {
					putST("\n\r");
					for (unsigned int j = 0; j<32; j++) {
						while(EECR&(1<<EEPE));
						EEAR = (i*32)+j;
						EECR |= (1<<EERE);
						unsigned char ch0 = EEDR&15;
						unsigned char ch1 = EEDR>>4;
						putHEX(ch1);
						putHEX(ch0);
					}
				}
				putST("\n\r");
				break;
				
			case 'p':	//print eeprom as string
				putST("Print EEPROM: ");
				for (unsigned int i = 0; i<eeadr; i++) {
					while(EECR&(1<<EEPE));
					EEAR = i;
					EECR |= (1<<EERE);
					putCH(EEDR);
				}
				putST("\n\r");
				break;
				
			case 'E':	//erase eeprom
				putST("\n\rErasing EEPROM.");
				for (unsigned int i = 0; i<16; i++) {
					putST(".");
					for (unsigned int j = 0; j<64; j++) {
						while(EECR&(1<<EEPE));
						EEAR = (i*64)+j;
						EEDR = 0;
						EECR |= (1<<EEMPE);
						EECR |= (1<<EEPE);
					}
				}
				eeadr = 0;
				putST("\n\rEEPROM erased.\n\r");
				break;
				
			case 'S':	//save new string to eeprom at address 0
				putST("Press enter when complete. Type string to save: ");
				temp = 1;
				while(temp){
					ch = getCH();
					switch(ch) {
						case '\r':		//exit on enter
							temp = 0;
							break;
						case '\b':
							eeadr--;
							EEAR = eeadr;
						default:		//write characters to eeprom as they come in
							while(EECR&(1<<EEPE));
							EEAR = eeadr++;	
							EEDR = ch;
							EECR |= (1<<EEMPE);
							EECR |= (1<<EEPE);
							putCH(ch);
							break;
					}
				}
				putST("\n\r");
				break;
				
			case 'H':	//save new hex to EEPROM at address 0
				putST("Press enter when complete. Type hex data to save: ");
				temp = 1;
				while(temp){
					ch = getCH();
					switch(ch) {
						case '\r':
							temp = 0;
							break;
						default:	//receive two hex characters, concat, save as a byte in EEPROM
							putCH(ch);
							unsigned char a = hexcheck(ch);
							if (a) {
								temp = 0;
								break;
							}
							ch = (hexToValue(ch)<<4);
							
							unsigned char ch0 = getCH();
							putCH(ch0);
							a = hexcheck(ch0);
							if (a) {
								temp = 0;
								break;
							}
							ch0 = hexToValue(ch0);
							ch += ch0;
							
							while(EECR&(1<<EEPE));	//write byte
							EEAR = eeadr++;	
							EEDR = ch;
							EECR |= (1<<EEMPE);
							EECR |= (1<<EEPE);
							break;
					}
				}
				putST("\n\r");
				break;
				
			case '?':	//help
				help();
				break;
			
			case '\r':	//enter
				putST("\n\r");
				break;
				
			default:	//error
				putST("Error! Invalid command. Type ? for help. \n\r");
				break;
		}
	}
	return 0;
}
