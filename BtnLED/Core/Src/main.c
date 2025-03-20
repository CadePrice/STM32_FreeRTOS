/*
 * main.c
 *
 *  Created on: Mar 9, 2025
 *      Author: CadeP
 */

#include "main.h"

// HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
// delay(500000);

// Main Function
int main(void)
{
	GPIO_Config();
	int flag = 1;

	while(1)
	{
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == 0 && flag == 1)
		{
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
			flag = 0;
			delay(50000);
		}
		else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == 1 && flag == 0)
		{
			flag = 1;
			delay(50000);
		}
	}
}

// GPIO Configuration
void GPIO_Config(void)
{
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeDef GPIOC_Init = {};

	// LED Output
	GPIOC_Init.Pin = GPIO_PIN_8;
	GPIOC_Init.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOC,&GPIOC_Init);

	// Button Input
	GPIOC_Init.Pin = GPIO_PIN_10;
	GPIOC_Init.Mode = GPIO_MODE_INPUT;
	GPIOC_Init.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC,&GPIOC_Init);
}

void delay(uint32_t delayVal)
{
	uint32_t i;
	for(i=0;i < delayVal; i++)
	{
	}
}
