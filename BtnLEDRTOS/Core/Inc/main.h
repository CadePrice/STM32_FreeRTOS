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

void ButtonTask(void *params);
void SystemClock_Config(void);
void GPIO_Config(void);

#endif /* INC_MAIN_H_ */
