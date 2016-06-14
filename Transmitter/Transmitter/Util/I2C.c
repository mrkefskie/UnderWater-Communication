/*
 * I2C.c
 *
 * Created: 14-6-2016 10:11:32
 *  Author: Kevin Slagmolen
 */ 

#include "defines.h"
#include "I2C.h"

#include <avr/io.h>
#include <util/twi.h>

void i2c_init()
{
	TWBR = (uint8_t)TWBR_VAL;
}

uint8_t i2c_start(uint8_t address)
{
	// Reset TWI control register
	TWCR = 0;
	// Transmit START condition
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	// Wait for end of transmission
	while (!(TWCR & (1 << TWINT)));

	// Check if the start condition was successfully transmitted
	if ((TWSR & 0xF8) != TW_START) return 1;

	// Load slave address into data register
	TWDR = address;
	// Start transmission of address
	TWCR = (1 << TWINT) | (1 << TWEN);
	// Wait for end of transmission
	while (!(TWCR & (1 << TWINT)));

	// Check if the device has acknowledged the READ / WRITE mode
	uint8_t twst = TW_STATUS & 0xF8;
	if ((twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK)) return 1;

	return 0;
}

uint8_t i2c_write(uint8_t data)
{
	// Load data into data register
	TWDR = data;
	// Start transmission of data
	TWCR = (1 << TWINT) | (1 << TWEN);
	// Wait for end of transmission
	while (!(TWCR & (1 << TWINT)));

	if ((TWSR & 0xF8) != TW_MT_DATA_ACK) return 1;

	return 0;
}

uint8_t i2c_read_ack()
{
	// Start TWI module and acknowledge data after reception
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	// Wait for end of transmission
	while (!(TWCR & (1 << TWINT)));
	// Return received data from TWDR
	return TWDR;
}

uint8_t i2c_read_nack()
{
	// Start receiving without acknowledging reception
	TWCR = (1 << TWINT) | (1 << TWEN);
	// Wait for end of transmission
	while (!(TWCR & (1 << TWINT)));
	// Return received data from TWDR
	return TWDR;
}

uint8_t i2c_transmit(uint8_t address, uint8_t* data, uint16_t length)
{
	if (i2c_start(address | I2C_WRITE)) return 1;

	for (uint16_t i = 0; i < length; i++)
	{
		if (i2c_write(data[i])) return 1;
	}

	i2c_stop();
	
	return 0;
}

uint8_t i2c_receive(uint8_t address, uint8_t* data, uint16_t length)
{
	if (i2c_start(address | I2C_READ)) return 1;

	for (uint16_t i = 0; i < length; i++)
	{
		data[i] = i2c_read_ack();
	}
	data[length - 1] = i2c_read_nack();

	i2c_stop();

	return 0;
}

uint8_t i2c_writeReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length)
{
	if (i2c_start(devaddr | 0x00)) return 1;

	i2c_write(regaddr);

	for (uint16_t i = 0; i < length; i++)
	{
		if (i2c_write(data[i])) return 1;
	}

	i2c_stop();

	return 0;
}

uint8_t i2c_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length)
{
	if (i2c_start(devaddr)) return 1;

	i2c_write(regaddr);

	if (i2c_start(devaddr | 0x01)) return 1;

	for (uint16_t i = 0; i < length; i++)
	{
		data[i] = i2c_read_ack();
	}
	data[length - 1] = i2c_read_nack();

	i2c_stop();

	return 0;
}

void i2c_stop()
{
	// Transmit STOP condition
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}