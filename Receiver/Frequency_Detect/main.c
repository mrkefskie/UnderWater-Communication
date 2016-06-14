/*
 * -------------------------------------------
 *    MSP432 DriverLib - v3_10_00_09 
 * -------------------------------------------
 *
 * --COPYRIGHT--,BSD,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/******************************************************************************
 * MSP432 Empty Project
 *
 * Description: An empty project that uses DriverLib
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 * Author: 
*******************************************************************************/
/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

#include "defines.h"

#define TARGET_FREQ		55000

static volatile bool on;
static volatile uint32_t time;
static volatile uint32_t prevtime;
static volatile int interval;
static volatile int freqToTicks;
static volatile int under;
static volatile int above;
static volatile uint8_t detected;

void TIMERInit(uint32_t count)
{
	/* Initializing Timer32 in module in 32-bit free-run mode (with max value
	 * of count
	 */
	MAP_Timer32_initModule(TIMER32_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
			TIMER32_PERIODIC_MODE);

	MAP_Timer32_setCount(TIMER32_BASE, count);
}

void RESTInit(void)
{
	/* Halting WDT and disabling master interrupts */
	MAP_WDT_A_holdTimer();
	MAP_Interrupt_disableMaster();

	MAP_FPU_enableModule();


	/* Configuring GPIO (4.7 A6) */
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN7, GPIO_TERTIARY_MODULE_FUNCTION);

	/* Selecting P1.2 and P1.3 in UART mode */
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
		GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

	/* Selecting P1.0 to output */
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

	/* Setting DCO to 48MHz  */
	MAP_PCM_setPowerState(PCM_AM_LDO_VCORE1);
	MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
}


int main(void)
{
	RESTInit();
	TIMERInit(0xFFFFFFFF);

	on = false;

	prevtime = 0xFFFFFFFF;

	freqToTicks = 48000000 / TARGET_FREQ * 2;
	under = freqToTicks - (freqToTicks * 0.02);
	above = freqToTicks + (freqToTicks * 0.02);

	/* Configuring GPIO */
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
	MAP_GPIO_setAsInputPinWithPullUpResistor(INPUT_PORT, INPUT_PIN);
	MAP_GPIO_clearInterruptFlag(INPUT_PORT, INPUT_PIN);

	/* Enabling interrupts */
	MAP_GPIO_enableInterrupt(INPUT_PORT, INPUT_PIN);

	MAP_Interrupt_enableInterrupt(INT_PORT4);

	printf("Start\n");

	MAP_Interrupt_enableMaster();

    while(1)
    {
    }
}

/* PORT4 ISR */
void PORT4_IRQHandler(void)
{
	uint32_t status;

	status = MAP_GPIO_getEnabledInterruptStatus(INPUT_PORT);

	MAP_GPIO_clearInterruptFlag(INPUT_PORT, status);


	if (on == true)
	{
		on = false;
		MAP_Timer32_startTimer(TIMER32_BASE, false);
	}
	else
	{
		on = true;

		time = MAP_Timer32_getValue(TIMER32_BASE);

		interval = prevtime - time;

		if ((interval < above) && (interval > under))
		{
			MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
			MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN5);
			detected = 1;
		}
		else
		{
			MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
			MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN5);
			detected = 0;
		}

		prevtime = time;
	}
}
