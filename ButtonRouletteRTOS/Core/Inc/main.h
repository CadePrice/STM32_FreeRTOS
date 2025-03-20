/*
 * main.h
 *
 *  Created on: Mar 13, 2025
 *      Author: CadeP
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"

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
void SystemClock_Config(void);

// PHASE 1
void MainMenuTask(void *params);

// PHASE 2
typedef struct
{
	int *array;
	int length;
	int safeCount;
	int dangerCount;
} RandomSeed;
RandomSeed getSeed(void);
void RoundTask(void *params);
void GameTask (void *params);

// PHASE 3
void EndTask(void *params);

#endif /* INC_MAIN_H_ */
