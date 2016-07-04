/*
 * data_conversion.c
 *
 * Created: 14-6-2016 10:59:17
 *  Author: Kevin Slagmolen
 */ 

#include "data_conversion.h"
#include <inttypes.h>
#include <stdbool.h>

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
uint8_t* fillDataArray(char data[100], uint8_t receivce_address)
{
    // Reset some stuff and initialize some variables
    dataLength = 0;
    int i;
    uint8_t ret[800]; // The return variable
    uint8_t data_num[100]; // The int representation of the data

	// Add start byte
	for (i = 0; i < 8; i++)
	{
		ret[dataLength++] = 1;
	}

	// Add own address
	ret[dataLength++] = (OWN_ADDRESS & 0b00000001);
	ret[dataLength++] = (OWN_ADDRESS & 0b00000010) >> 1;
	ret[dataLength++] = (OWN_ADDRESS & 0b00000100) >> 2;
	ret[dataLength++] = (OWN_ADDRESS & 0b00001000) >> 3;
	ret[dataLength++] = (OWN_ADDRESS & 0b00010000) >> 4;
	ret[dataLength++] = (OWN_ADDRESS & 0b00100000) >> 5;
	ret[dataLength++] = (OWN_ADDRESS & 0b01000000) >> 6;
	ret[dataLength++] = (OWN_ADDRESS & 0b10000000) >> 7;

	// Add receiving address
	ret[dataLength++] = (receivce_address & 0b00000001);
	ret[dataLength++] = (receivce_address & 0b00000010) >> 1;
	ret[dataLength++] = (receivce_address & 0b00000100) >> 2;
	ret[dataLength++] = (receivce_address & 0b00001000) >> 3;
	ret[dataLength++] = (receivce_address & 0b00010000) >> 4;
	ret[dataLength++] = (receivce_address & 0b00100000) >> 5;
	ret[dataLength++] = (receivce_address & 0b01000000) >> 6;
	ret[dataLength++] = (receivce_address & 0b10000000) >> 7;

    // Convert the chars to int's
    for (i = 0; i < strlen(data); i++)
    {
        data_num[i] = (uint8_t)data[i];
    }

    int checksum = CRC(data_num, strlen(data), POLY, false); // Get a checksum for the data

    #ifdef DEBUG // Debugging
    printf("\nChecksum: %i\n", checksum);
    #endif // DEBUG

    // Loop through all the data
    for (i = 0; i < strlen(data); i++)
    {
        uint8_t j;
        uint8_t ascii = (uint8_t)data[i]; // Take one of the data bytes

        // Loop through the bits 1 at a time
        for (j = 0; j < 8; j++)
        {
            uint8_t tmp = (ascii & 0b10000000) >> 7; // Select the leftmost bit
            ascii = ascii << 1; // Shift the data left by 1

            ret[dataLength] = tmp; // Store the leftmost bit in an array

            dataLength++; // Add one to the total data length

            #ifdef DEBUG // Debugging
            printf("%i", tmp);
            #endif
        }
        #ifdef DEBUG // Debugging
        printf(" ");
        #endif
    }

    // Loop 8 more times for the CRC byte
    for (i = 0; i < 8; i++)
    {
        int tmp = (checksum & 0b10000000) >> 7; // Select the leftmost bit
        checksum = checksum << 1; // Shift the checksum left by 1

        ret[dataLength] = tmp; // Store the 2 leftmost bits in an array
        dataLength++; // Add one to the total data length
    }

	for (i = 0; i < 8; i++)
	{
		ret[dataLength] = 0;
		dataLength++;
	}

    #ifdef DEBUG // Debugging
    printf("\ndataLength: %i\n", dataLength);
    #endif // DEBUG

    return ret;
}

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
    char tmp = 0b00000000; // Create a temp char with no data
    uint8_t ascii[100]; // The int representation of the data

    #ifdef DEBUG // Debugging
    printf("length: %i\n", length);
    #endif // DEBUG

    // Loop through all the bits
    for (i = 0; i < length; i++)
    {
        #ifdef DEBUG // Debugging
        printf("\ni:%i", i);
        #endif

        tmp = tmp << 1 | (bits[i] & 0b1); // Add the bit to the right of the previous ones

        #ifdef DEBUG // Debugging
        printf(" bits%i tmp%i,", (bits[i] & 0b1), (uint8_t)tmp);
        #endif

        // If this is dividable by 8
        if ((i + 1) % 8 == 0)
        {
            #ifdef DEBUG // Debugging
            printf("\nchar %c\nTotal %i\n", tmp, total);
            #endif // DEBUG

			if (tmp != 0b11111111)
			{
				if (total == 1)
				{
					if (tmp != OWN_ADDRESS)
					{
						printf("Not for me\n\n");
						//break;
						//return NULL;
					}
				}
				ret[total] = tmp; // Store the temp value in the return value
				total++; // Add one to the total
				tmp = 0b00000000; // Reset the temp value
			}
		}
    }

    // Loop through all the chars
    for (i = 0; i < strlen(ret); i++)
    {
        ascii[i] = (uint8_t)ret[i]; // Convert them to ints
    }

    int checksum = CRC(ascii, strlen(ret), POLY, true); // Do the checksum

    ret[total - 1] = 0b00000000; // Set the last 2 bytes to 0 (this is the checksum and end byte)
	ret[total - 2] = 0b00000000;

    #ifdef DEBUG // Debugging
    printf("\n\nChecksum: %i\n\n", checksum);
    #endif // DEBUG

    if (checksum == 0)
    {
		ret[0] = 0xFF;
        #ifdef DEBUG // Debugging
        printf("No errors.\n");
        #endif // DEBUG
    }
    else
    {
        ret[0] = 0x00;
        #ifdef DEBUG // Debugging
        printf("Error detected.\n");
        #endif // DEBUG
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
    #ifdef DEBUG // Debugging
    printf("Length: %i\n", length);
    #endif // DEBUG

    // Reset some stuff and initialize some variables
    int i;
    uint16_t allData = 0;

    // Loop through all the bits
    for (i = 0; i < length; i++)
    {
        #ifdef DEBUG // Debugging
        printf("Data: %i, ", (uint8_t)data[i]);
        #endif // DEBUG

        allData = allData << 8 | (uint8_t)data[i]; // Add the last byte to the right of the previous ones
    }

    #ifdef DEBUG // Debugging
    printf("\nallData: %u\n", allData);
    #endif // DEBUG

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

        #ifdef DEBUG // Debugging
        printf("%i^%i=", allData, calcPoly);
        #endif // DEBUG

        allData = allData ^ calcPoly; // Do the actual XOR

        #ifdef DEBUG // Debugging
        printf("%i\n", allData);
        #endif // DEBUG

        calcPoly = poly; // Reset the polynomial to its original value
    }

    return allData;
}