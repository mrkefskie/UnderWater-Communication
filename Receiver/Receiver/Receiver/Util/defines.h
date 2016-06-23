/*
 * defines.h
 *
 * Created: 14-6-2016 13:09:11
 *  Author: Kevin Slagmolen
 */ 


#ifndef DEFINES_H_
#define DEFINES_H_

#include <avr/io.h>

//#define DEBUG

#define F_CPU			16000000UL

/* START USART DEFINES */
#define USART_BAUD		921600
#define UBRR_BAUDRATE	(((F_CPU / (USART_BAUD * 16UL))) - 1)
/* END USART DEFINES */

/* START I2C DEFINES*/
#define F_SCL			100000UL
#define Prescaler		1
#define TWBR_val		((((F_CPU / F_SCL) / Prescaler) - 16) / 2)
/* END I2C DEFINES */

#define OUTPUT_BAUDRATE	500		// Baudrate for the data
#define OWN_ADDRESS		0x02	// The address of the device (CAN BE ANYTHING BETWEEN 0X01 AND 0XFF)

#define DELAY_MS		1000 / OUTPUT_BAUDRATE
#define DELAY_US		1000000 / OUTPUT_BAUDRATE
#define OUTPUT_PIN		DDB2	// Pin that will select the profile
#define INPUT_PIN		DDB0	// Pin that receives the bits

#define POLY			0x167	// Error checking poly


#endif /* DEFINES_H_ */