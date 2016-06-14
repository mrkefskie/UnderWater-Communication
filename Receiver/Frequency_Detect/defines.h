/*
 * defines.h
 *
 *  Created on: 31 mei 2016
 *      Author: Kevin Slagmolen
 */

#ifndef DEFINES_H_
#define DEFINES_H_

/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define DEBUG		// COMMENT THIS LINE TO DISABLE DEBUG INFO

#define CPU_TICK	48000000

#define BAUDRATE	100
#define WAIT_TICKS	CPU_TICK / BAUDRATE
#define INPUT_PORT	GPIO_PORT_P4
#define INPUT_PIN	GPIO_PIN1

#define POLY		0x167

#define FREQ1		45000
#define FREQ2		55000

#define TICKS_FREQ1 CPU_TICK / FREQ1
#define TICKS_FREQ2	CPU_TICK / FREQ2

#endif /* DEFINES_C_ */
