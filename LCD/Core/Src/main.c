/*
 * main.c
 *
 *  Created on: Mar 11, 2025
 *      Author: CadeP
 */

#include "main.h"
#include <stdio.h>

// Main Function
int main(void)
{
	GPIO_Config();
	LCD_Init();
	delay(100000);
	//LCD_Command(0x02);
	int var = 5;
	char str[32];
	sprintf(str, "The value is: %d", var);
	LCD_Print(str);
}

// GPIO Configuration
void GPIO_Config(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeDef GPIOA_Init = {0};
	GPIO_InitTypeDef GPIOC_Init = {0};

	// LCD PA
	GPIOA_Init.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIOA_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOA_Init.Pull = GPIO_NOPULL;
	GPIOA_Init.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA,&GPIOA_Init);

	// LCD PC
	GPIOC_Init.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIOC_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOC_Init.Pull = GPIO_NOPULL;
	GPIOC_Init.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC,&GPIOC_Init);
}

// LCD Configuration
void LCD_Init(void)
{

	delay(150000);
	LCD_Command(0x30);     // Function set
	delay(50000);
	LCD_Command(0x30);     // Function set again
	delay(50000);
	LCD_Command(0x30);     // Function set again
	delay(50000);
	LCD_Command(0x38);     // Set 8-bit mode, 2-line display, 5x8 font
	LCD_Command(0x0C);     // Display ON, cursor OFF
	LCD_Command(0x01);     // Clear display
	delay(100000);
	LCD_Command(0x06);     // Increment cursor after each character
}

// LCD Send Information
// Command: cd = 0 | Data: cd = 1
void LCD_Send(uint8_t data, uint8_t cd)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, cd ? GPIO_PIN_SET : GPIO_PIN_RESET);	// cd = 0 -> command | cd = 1 -> data

	GPIOC->ODR = (GPIOC->ODR & 0xFFFFFF00) | data;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
	delay(10000);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
	delay(10000);
}

void LCD_Command(uint8_t command)
{
	LCD_Send(command, 0);
}

void LCD_Data(uint8_t data)
{
	LCD_Send(data, 1);
}

void LCD_Print(const char *str)
{
	while(*str)
	{
		LCD_Data(*str++);
	}
}


void delay(uint32_t delayVal)
{
	uint32_t i;
	for(i=0;i < delayVal; i++)
	{
	}
}

