/*
 * defines.h
 *
 * Created: 14-6-2016 10:11:00
 *  Author: Kevin Slagmolen
 */ 


#ifndef DEFINES_H_
#define DEFINES_H_

#include <avr/io.h>

// CPU ClockSpeed
#define F_CPU				16000000UL

// START USART
#define USART_BAUD			250000
#define UBBR_BAUDRATE		(((F_CPU / (USART_BAUD * 16UL))) - 1)
// END USART

// START I2C
#define F_SCL				100000UL
#define PRESCALER			1
#define TWBR_VAL			((((F_CPU / F_SCL) / PRESCALER) - 16) / 2)
// END I2C

#define DCO_PROG_1			DDB0	// Enable pin for DCO1
#define DCO_PROG_2			DDB1	// Enable pin for DCO2

#define FREQ_1				45000	// Frequency for DCO1
#define FREQ_2				55000	// Frequency for DCO2

#define STATUS_LED			DDB5	// Status LED

#define OUTPUT_BAUDRATE		500		// Baudrate for the data
#define OWN_ADDRESS			0x01	// The address of the device (CAN BE ANYTHING BETWEEN 0X01 AND 0XFF)
#define OUTPUT_PIN			DDB2	// Pin that will select the frequency

#define DELAY_US			1000000 / OUTPUT_BAUDRATE

#define POLY				0x167	// Error checking poly

#endif /* DEFINES_H_ */