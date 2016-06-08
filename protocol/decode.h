/*
 * decode.h
 *
 *  Created on: 31 mei 2016
 *      Author: Kevin Slagmolen
 */

#ifndef DECODE_H_
#define DECODE_H_

#include "../defines.h"

/**
 * convertToData
 *
 * With this function the incomming bits will be decoded to readable data for the user.
 *
 * @param bits All the bits in an array
 * @param length The length of the array
 *
 * @return The readable data
 */
char* convertToData(uint8_t bits[800], int length);

/**
 * CRC
 *
 * Do a cyclic Redundancy Check (CRC) on the data (to send or receive).
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

#endif /* DECODE_H_ */
