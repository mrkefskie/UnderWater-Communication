/*
 * Transmitter.c
 *
 * Created: 14-6-2016 09:55:51
 * Author : Kevin Slagmolen
 */ 

#include "Util/defines.h"
#include "Util/USART.h"
#include "Util/I2C.h"
#include "data_conversion.h"

#include <avr/io.h>
#include <util/delay.h>

FILE usart0_str = FDEV_SETUP_STREAM(USART0SendByte, NULL, _FDEV_SETUP_WRITE);

unsigned char i2cAddress;

void setFreq(uint32_t freq);
void sendData(uint8_t dataToSend[800]);
void sendPing();

int main(void)
{
	// Initialize IO
	DDRB |= (1 << OUTPUT_PIN); // Set the output pin
	DDRB |= (1 << DCO_PROG_1) | (1 << DCO_PROG_2) | (1 << STATUS_LED);

	PORTB |= (1 << STATUS_LED); // Turn on status LED
	_delay_ms(500); // Wait for 500ms
	PORTB &= ~(1 << STATUS_LED); // Turn off status LED

	USART0Init(); // Initialize USART communication
	stdout =& usart0_str;

	i2c_init();
	
	PORTB |= (1 << DCO_PROG_1);
	PORTB &= ~(1 << DCO_PROG_2);
	i2cAddress = 0x17;
	setFreq(FREQ_1);
	
	PORTB |= (1 << DCO_PROG_2);
	PORTB &= ~(1 << DCO_PROG_1);
	i2cAddress = 0x17;
	setFreq(FREQ_2);

	PORTB |= (1 << STATUS_LED); // Turn on status LED
	_delay_ms(500); // Wait for 500ms
	PORTB &= ~(1 << STATUS_LED); // Turn off status LED
	_delay_ms(500); // Wait for 500ms
	PORTB |= (1 << STATUS_LED); // Turn on status LED
	_delay_ms(500); // Wait for 500ms
	PORTB &= ~(1 << STATUS_LED); // Turn off status LED

	char input[100] = "Test";
	printf("\n\n\n\nSTART CODE WITH THIS INPUT: %s\nBAUDRATE: %i\n\n", input, OUTPUT_BAUDRATE); // Print the input string and baudrate

	uint8_t* dataToSend = fillDataArray(input, 0x02); // Convert data to base2
	
	unsigned int time = (1000 / OUTPUT_BAUDRATE) * dataLength;

	printf("Baudtime: %i\n", 1000 / OUTPUT_BAUDRATE);
	printf("Time needed to send data: %ims\n", time);

	printf("datalength: %i\n", dataLength);

	PORTB &= ~(1 << OUTPUT_PIN);

	_delay_ms(1000);
	sendData(dataToSend);

    while (1) 
    {
		sendPing();
		_delay_ms(2000); // Wait for 2 seconds
    }
}

void sendPing()
{
	PORTB ^= (1 << OUTPUT_PIN);
	_delay_us(DELAY_US);
	PORTB ^= (1 << OUTPUT_PIN);
	_delay_us(DELAY_US);
}

void sendData(uint8_t dataToSend[800])
{
	for (int i = 0; i < dataLength; i++)
	{
		if ((dataToSend[i] && 0b01) == 0)
			PORTB &= ~(1 << OUTPUT_PIN);
		else
			PORTB |= (1 << OUTPUT_PIN);

		_delay_us(DELAY_US);
	}
}

void setFreq(uint32_t freq)
{
	/*
	Calculate frequency
	See datasheet for more information: http://cds.linear.com/docs/en/datasheet/69034fe.pdf (page 8)
	*/
	#ifdef DEBUG
	printf("Frequency: %" PRIu32 "\n", freq);
	#endif

	uint32_t OCT = (uint32_t)floor(3.322 * log10(freq / 1039));
	uint32_t DAC = round(2048 - (2078 * pow(2, 10 + OCT) / freq));

	#ifdef DEBUG
	printf("OCT %" PRIu32 "\nDAC: %" PRIu32 "\n", OCT, DAC);
	#endif

	uint16_t setBits = OCT << 12 | DAC << 2 | 1 << 1; // Write all the bits in one variable

	// Split the bits in a high and a low part
	char meleon = (setBits >> 8) & 0xff; // High part
	char mander = setBits & 0xff; // Low part

	#ifdef DEBUG // Calculate the exact frequency of the DCO
	uint32_t power = (uint32_t)round(pow(2, OCT));
	double div = (double)DAC / 1024.0;
	double second = 2078 / (2 - div);

	uint32_t exactFreq = power * second;

	printf("Exact frequency: %" PRIu32 "\n", exactFreq);
	#endif

	uint8_t bits[2] = {meleon, mander}; // Set the high and low part in an array

	i2c_transmit(i2cAddress << 1, bits, 2);
}