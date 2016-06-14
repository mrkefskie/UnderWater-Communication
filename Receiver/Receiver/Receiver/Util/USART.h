/*
 * USART.h
 *
 * Created: 14-6-2016 13:11:43
 *  Author: Kevin Slagmolen
 */ 


#ifndef USART_H_
#define USART_H_

#include "defines.h"

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

void USART0Init(void);
int USART0SendByte(char u8Data, FILE *stream);

#endif /* USART_H_ */