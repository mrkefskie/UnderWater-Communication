/*
 * data_conversion.h
 *
 * Created: 14-6-2016 10:59:06
 *  Author: Kevin Slagmolen
 */ 


#ifndef DATA_CONVERSION_H_
#define DATA_CONVERSION_H_

#include "Util/defines.h"
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

// Variables
int dataLength;

// Functions
/**
 * fillDataArray
 *
 * This functions converts the data (in chars) to a array of integers.
 *
 * @param data The data to convert.
 *
 * @return The array of integers which can be send.
 *
*/
uint8_t* fillDataArray(char data[100]);

/**
 * convertToData
 *
 * With this function the incoming bits will be decoded to readable data for the user.
 *
 * @param bits All the bits in an array
 * @param length The length of the array
 *
 * @return The readable data
*/
char* convertToData(uint8_t bits[400], int length);

/**
 * CRC
 *
 * Do a Cyclic Redundancy Check (CRC) on the data (to send or receive).
 * For more information about CRC, visit https://en.wikipedia.org/wiki/Cyclic_redundancy_check
 *
 * @param data The data to do the CRC on
 * @param length The length of the data
 * @param poly The used divisor
 * @param received Is the data received or not
 *
 * @return The generated checksum
*/
int CRC(uint8_t data[100], int length, uint16_t poly, bool received);

#endif /* DATA_CONVERSION_H_ */