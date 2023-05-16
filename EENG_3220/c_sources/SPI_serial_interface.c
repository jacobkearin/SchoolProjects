/*
 * 
 *
 * Created: 4/19/2023 2:37:50 PM
 * Author : keajacm
 * 
 * use serial uart interface for manual interaction with SPI device
 * 
 * target device: ATmega328P
 * 
 */ 



#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

//tx = PD1, rx = PD0)
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

void putST (uint8_t st[]) {	//send a string of characters out serial port
	int i;
	for (i = 0; st[i]; i++) {
		while (! (UCSR0A & (1<<UDRE0))); //wait until UDR0 is empty
		UDR0 = st[i];
	}
}

uint8_t getCH (void) {
	while (! (UCSR0A & (1<<RXC0))); //wait until new data
	uint8_t ch = UDR0;
	return ch;
}

uint8_t getST[] (void) {
    uint_8t st[];
    for (int i = 0; i<0; i++) {
        while (! (UCSR0A & (1<<RXC0))); //wait until new data
	    uint8_t ch = UDR0;
        if (ch=='\r') {
            i = -1
        }
        else {
            st[i] = ch;
        }
    }
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

void sram_read_range(uint16_t addrL, uint16_t addrH) {
	PORTB &= ~(1<<SS);
	SPDR = readinst;
    while (!(SPSR & (1<<SPIF)));
    SPDR = addrL >> 8;
    while (!(SPSR & (1<<SPIF)));
    SPDR = addrL & 0xFF;
    while (!(SPSR & (1<<SPIF)));
    
	int i = addrH-addrL;
    for (i>0; i--) {
        SPI_write(0xFF);	
        uint8_t data = SPDR;
        putCH(data);
    }

    putST("\n\r")
	PORTB |= (1<<SS);
}

uint8_t hexcheck(uint8_t ch) {		//return 0 if input is a valid hex digit
	if (((ch>='0')&&(ch<='9'))||((ch>='A')&&(ch<='F'))||((ch>='a')&&(ch<='f'))) {
		return 0;
	}
	else {
		return 1;
	}
}

uint8_t hexToValue(uint8_t ch) {	//return 0-15 value from ascii hex character
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

void prog_init (void) {
	uart_init();
	SPI_init();
	putST("\n\rWelcome\n\r");
}

void help (void) {
	putST("\n\r----- Help -----\n\r");
	putST("W: write a hex byte to device and display received byte\n\r");
    putST("S: send a string of ASCII values\n\r");
    putST("H: send a string of raw hex values\n\r");
	putST("R: send 0xFF and display data reg\n\r");
    putST("P: dump all as ASCII from/to address(SRAM, etc)\n\r");
    putST("D: dump all as hex from/to address(SRAM, etc)\n\r");
	putST("?: help\n\r");
}

int main(void) {
	
	prog_init();
	
	uint8_t Op;	//command input variable
	uint8_t ch;	//data character
	int temp = 1;
	
	while (1) {
		putST("->");
		Op = getCH(); 

        switch(Op) {
			case 'W':
                uint8_t ch = getCH();

                

                break;

            case 'S':
                
            
                break;

            case 'H':   //user types hex byte, sends byte over SPI, prints out received values on enter
                putST("Press enter when complete. Type hex data to send: ");
				temp = 1;
                uint8_t rx_values[];
                uint16_t rxcounter = 0;
				while(temp){
					ch = getCH();
					switch(ch) {
						case '\r':
							temp = 0;
							break;
						default:	//receive two hex characters, concat, send as byte
							putCH(ch);
							uint8_t a = hexcheck(ch);
							if (a) {
								temp = 0;
                                putST("\n\rinvalid character\n\r")
								break;
							}
							ch = (hexToValue(ch)<<4);
							uint8_t ch0 = getCH();
							putCH(ch0);
							a = hexcheck(ch0);
							if (a) {
								temp = 0;
								break;
							}
							ch0 = hexToValue(ch0);
							ch += ch0;
							SPI_write(ch);
                            rx_values[rxcounter] = SPDR;
                            rxcounter++;
							break;
                    }
                }
                putST(rx_values);
                putST("\n\r");
				break;

            case 'R':
            
                putST("\n\r");
                break;

            case 'P':
                
                putST("\n\r");
                break;

            case 'D':
            
                putST("\n\r");
                break;

            case '?':
            
                putST("\n\r");
                break;

            default:	//error
				putST("Error! Invalid command. Type ? for help. \n\r");
				break;
        }
    }

    return 0;
}
          

		
    

