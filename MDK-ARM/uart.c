#include "uart.h"

// Debug Mode; comment out on Release
#define _DEBUG			0


/*! \brief Configures baud rate (refer datasheet) */
void initUART(void)
{
	// Not necessary; initialize anyway
	DDRD |= _BV(PD1);
	DDRD &= ~_BV(PD0);

	// Set baud rate; lower byte and top nibble
	UBRR0H = ((_UBRR) & 0xF00);
	UBRR0L = (uint8_t) ((_UBRR) & 0xFF);

	TX_START();
	RX_START();

	// Set frame format = 8-N-1
	UCSR0C = (_DATA << UCSZ00);

}

/*! \brief Returns a byte from the serial buffer
 * 	Use this function if the RX interrupt is not enabled.
 * 	Returns 0 on empty buffer
 */
uint8_t getByte(void)
{
	// Check to see if something was received
	while (!(UCSR0A & _BV(RXC0)));
	return (uint8_t) UDR0;
}


/*! \brief Transmits a byte
 * 	Use this function if the TX interrupt is not enabled.
 * 	Blocks the serial port while TX completes
 */
void putByte(unsigned char data)
{
	// Stay here until data buffer is empty
	while (!(UCSR0A & _BV(UDRE0)));
	UDR0 = (unsigned char) data;

}

/*! \brief Writes an ASCII string to the TX buffer */
void writeString(char *str)
{
	while (*str != '\0')
	{
		putByte(*str);
		++str;
	}
}

const char* readString(void)
{
	static char rxstr[RX_BUFF];
	static char* temp;
	temp = rxstr;

	while((*temp = getByte()) != '\n')
	{
		++temp;
	}

	return rxstr;
}

#if _DEBUG

	int main(void)
	{
		initUART();
		while(1)
		{
			writeString(readString());
			putByte('\r');
			putByte('\n');
		}
		return 0;
	}

#endif