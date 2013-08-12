#define F_CPU 8000000

//Define the BIT DELAY. The RS232 protocol doesn't use a clock pin
//It needs a timed delay, the baud per second
//To calculate the delay per bit for a given BPS
//Divide one second by how many bps you need, for example
//delay per bit = 1sec/38400bps = 26 useconds per bit

#define BITDELAY _delay_us(26)

//simple macros to set port to on or off
#define TXPIN_LOW (PORTB &= ~(1<<TXPIN))
#define TXPIN_HIGH (PORTB |= (1<<TXPIN))
//define the TX pin
#define TXPIN PB3

#include <avr/io.h>
#include <util/delay.h>

void TXInit(void)
{
	DDRB |= (1<<TXPIN); //TXPIN as output
	TXPIN_LOW; //TXPIN to low, meaning rs232 to HIGH = idle state.
}

void SendChar(char data)
{
	TXPIN_HIGH; //TXPIN to high. meaning rs232 to low = start bit.
	BITDELAY; //delay after start bit
	
	//apply NOT to reverse the bits, because rs232 reverses the bits
	data = ~data;
	int i;
	
	//loop to send the bits
	for (i=0;i<8;i++)
	{
		//lsb so check the 0th bit
		if (data & 0x01)
		{
			TXPIN_HIGH;
		}
		else
		{
			TXPIN_LOW;
		}
		//then shift right
		data >>= 1;
		//then bit delay
		BITDELAY;
	}
	
	//sent all 8 bits
	//sending stop bit
	TXPIN_LOW; //portb to low, meaning rs232 high. stop bit & back to idle
	BITDELAY;
	//back to idle
}


//sends a string formatted like: char String1[]="This is string 1"
void SendString(char* StringPtr)
{
	//sends a string
	while(*StringPtr != 0x00)
	{
		SendChar(*StringPtr);
		StringPtr++;
	}
}

// sends a carriage return, to look nice on the display when using strings
void SendCR(void)
{
	SendChar(0x0d);
}

int main(void)
{
	TXInit();
	
	while(1)
    {
		char String1[]="Going left!";
		SendString(String1);
		int a;
		a = 0b00000001;
		int i;
		for (i=0;i<8;i++)
		{
			SendChar(a);
			a <<= 1;
			_delay_ms(50);
		}
		
		String1[]="Going right!";
		SendString(String1);
		
		a = 0b10000000;
		for (i=0;i<8;i++)
		{
			SendChar(a);
			a >>= 1;
			_delay_ms(50);
		}
		
		_delay_ms(2000);
    }
}
