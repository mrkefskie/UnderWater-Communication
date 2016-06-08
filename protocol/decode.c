/*
 * decode.c
 *
 *  Created on: 31 mei 2016
 *      Author: Kevin Slagmolen
 */

#include "decode.h"

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
char* convertToData(uint8_t bits[800], int length)
{
	// Reset some stuff and initialize some variables
	uint16_t i;
	int total = 0;
	char ret[100] = ""; // The return value
	char tmp = 0x00; // Create a temp char with no data
	uint8_t ascii[100]; // The int representation of the data

#ifdef DEBUG
	printf("Length: %i\n", length);
#endif

	// Loop through all the bits
	for (i = 0; i < length; i++)
	{
		tmp = tmp << 1 | (bits[i] & 0b01); // Add the bit to the right of the previous ones

#ifdef DEBUG
		printf("\ni:%i bits%i tmp%i,", i, (bits[i] & 0b01), (uint8_t)tmp);
#endif

		// If this is dividable by 8
		if ((i + 1) % 8 == 0)
		{
#ifdef DEBUG
			printf("\nchar %c\nTotal %i\n", (char)tmp, total);
#endif

			ret[total] = tmp; // Store the temp value in the return value
			total++; // Add one to the total
			tmp = 0x00; // Reset the temp value
		}
	}

	// Loop through all the chars
	for (i = 0; i < strlen(ret); i++)
	{
		ascii[i] = (uint8_t)ret[i]; // Convert them to ints
	}

	int checksum = CRC(ascii, strlen(ret), POLY, true); // Get the checksum

	ret[total - 1] = 0x00; // Set the last 2 byts to 0 (checksum and stopbyte)
	ret[total - 2] = 0x00;

#ifdef DEBUG
	printf("\n\nChecksum: %i\n", checksum);
#endif

	if (checksum == 0)
	{
#ifdef DEBUG
		printf("No errors.\n");
#endif
	}
	else
	{
		ret[0] = 0x00;
#ifdef DEBUG
		printf("Error detected.\n");
#endif
	}

	return ret;
}

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
int CRC(uint8_t data[100], int length, uint16_t poly, bool received)
{
#ifdef DEBUG
	printf("Length: %i\n", length);
#endif

	// Reset some stuff and initialize some variables
	int i;
	uint16_t allData = 0;

	for (i = 0; i < length; i++)
	{
#ifdef DEBUG
		printf("Data: %i, ", (uint8_t)data[i]);
#endif

		allData = allData << 8 | (uint8_t)data[i]; // Add the last byte to the right of the previous ones
	}

#ifdef DEBUG
	printf("\nallData: %u\n", allData);
#endif

	if (!received) // Check if the data is received or not
	{
		allData = allData << 8; // Add one byte of empty data at the right of the data
	}

	uint64_t calcPoly = poly; // Store the polynomial in a variable so we can modify it

	// Loop endlessly
	while (1)
	{
		bool shiftBack = false; // To see if we need to shift back later

		if (allData < 0xFF) break; // If the data is smaller then 255 (0xFF)
		while (calcPoly < allData)
		{
			calcPoly = calcPoly << 1; // Shift the polynomial left by one
			shiftBack = true;
		}
		if (shiftBack) calcPoly = calcPoly >> 1; // Shift the polynomial back one

#ifdef DEBUG
		printf("%i^%i=", allData, calcPoly);
#endif

		allData = allData ^ calcPoly; // Do the actual XOR

#ifdef DEBUG
		printf("%i\n", allData);
#endif

		calcPoly = poly; // Reset the polynomial to its original value
	}

	return allData;
}
