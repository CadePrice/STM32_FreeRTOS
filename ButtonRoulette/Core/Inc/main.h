/*
 * main.h
 *
 *  Created on: Mar 11, 2025
 *      Author: CadeP
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// MISC
void delay(uint32_t delayVal);
void LCD_Send(uint8_t data, uint8_t cd);
void LCD_Print(const char *str);

// PHASE 0
void GPIO_Config(void);
void LCD_Init(void);

// PHASE 2
typedef struct
{
	int *array;
	int length;
	int safeCount;
	int dangerCount;
} RandomSeed;
RandomSeed getSeed(void);

#endif /* INC_MAIN_H_ */
