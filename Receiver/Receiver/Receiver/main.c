/*
 * Receiver.c
 *
 * Created: 14-6-2016 12:55:42
 * Author : Kevin Slagmolen
 */ 

#include "Util/defines.h"
#include "Util/USART.h"
#include "data_conversion.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

FILE usart0_str = FDEV_SETUP_STREAM(USART0SendByte, NULL, _FDEV_SETUP_WRITE);

uint8_t receivedData[800];

volatile uint8_t receiving;



int main(void)
{
    for (int i = 0; i < 800; i++)
	{
		receivedData[i] = 0;
	}

	receiving = 0;

	USART0Init(); // Initlialize USART communication
	stdout =& usart0_str;
	
	printf("Start baudrate: %i\n", OUTPUT_BAUDRATE);

	DDRB &= ~(1 << INPUT_PIN); // Set INPUT_PIN as input

	PCICR |= (1 << PCIE0);
	PCMSK0 |= (1 << PCINT0);

	sei(); // Turn on interrupts

	int counter = 0;
	
	uint8_t zeroStreak = 0;

    while (1) 
    {
		while (receiving == 1)
		{
			_delay_us(DELAY_US / 4); // Wait a quarter of the baudrate to sample somewhere in the middle
			receivedData[counter] = PINB & (1 << INPUT_PIN);
			
			if (receivedData[counter] == 0) zeroStreak++; // Add one to the zeroStreak if the received bit is a 0
			else zeroStreak = 0; // Reset the zeroStreak if the received bit is a 1
			counter++; // Add one to the counter
			_delay_us((DELAY_US / 4) * 3); // Wait the other three quarters of the baudrate

			if (((counter % 8) == 0) && zeroStreak >= 8)  // If the counter is dividable by 8 and the zeroStreak is bigger or equal to 8
			{
				printf("\n\nStop Byte detected, stopped receiving.\n\n");

				for (int i = 0; i < counter; i++) // Loop through all the data
				{
					printf("%i", receivedData[i]);

					if ((i + 1) % 8 == 0) printf(" ");
				}

				char* received = convertToData(receivedData, counter); // Convert base2 to data

				if (received != NULL)
				{
					if (received[0] != 0xFF) // If the first byte is not equal to 255 (0xFF)
					{
						printf("Error detected...\n");
					}
					printf("Received data: %s\n\n", received);
				}
				else
					printf("Not for me\n");
				// Reset some variables
				counter = 0;
				receiving = 0;

				sei(); // Turn on interrupts again
			}
		}
    }
}

ISR(PCINT0_vect)
{
	receiving = 1;
	cli(); // Disable interrupts
}