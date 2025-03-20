/*
 * main.h
 *
 *  Created on: Mar 11, 2025
 *      Author: CadeP
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "stm32f4xx_hal.h"

void GPIO_Config(void);
void LCD_Init(void);
void LCD_Send(uint8_t data, uint8_t cd);
void LCD_Command(uint8_t command);
void LCD_Data(uint8_t data);
void LCD_Print(const char *str);
void delay(uint32_t delayVal);

#endif /* INC_MAIN_H_ */
