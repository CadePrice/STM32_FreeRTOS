/*
 * main.c
 *
 *  Created on: Mar 11, 2025
 *      Author: CadeP
 */

#include "main.h"

// Main Function
int main(void)
{
	// PHASE 0: GPIO Configurations, LCD Configurations, Variable Initialization
	GPIO_Config();
	LCD_Init();
	int round, turn, life1, life2;
	srand(time(NULL));
	char LCDOutput[33];

	// GAMEPLAY LOOP
	while(1)
	{
		// PHASE 1A: Main Menu
		sprintf(LCDOutput, "-BUTTONROULETTE-PRESS BU TO STRT");
		LCD_Print(LCDOutput);
		while((HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == 1) && (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == 1))
		{
		}

		// PHASE 2A: Initialize Game Variables
		round = 0;
		turn  = 0;
		life1 = 1;
		life2 = 1;
		while(1)
		{
			// PHASE 2B: Round Screen
			round++;
			RandomSeed roundSeed = getSeed();
			sprintf(LCDOutput, "    ROUND  %d    SAFE:%d |DANGER:%d", round, roundSeed.safeCount, roundSeed.dangerCount);
			LCD_Print(LCDOutput);
			delay(5000000);

			// PHASE 2C: Game Screen
			for(int i=0; i < roundSeed.length; i++)
			{
				// Update Game Screen
				if(turn % 2 == 0)
				{
					sprintf(LCDOutput, "PLR1<-      PLR2<3:%d        <3:%d", life1, life2);
					LCD_Print(LCDOutput);
				}
				else
				{
					sprintf(LCDOutput, "PLR1      ->PLR2<3:%d        <3:%d", life1, life2);
					LCD_Print(LCDOutput);
				}
				while(1)
				{
					// Offense (White Button)
					if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == 0)
					{
						delay(2000000);
						if(roundSeed.array[i] == 0)
						{
							HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_10);
							delay(2000000);
							HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_10);
							delay(2000000);
							break;
						}
						else
						{
							HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_11);
							delay(2000000);
							HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_11);
							delay(2000000);
							if(turn % 2 == 0)
							{
								life2--;
							}
							else
							{
								life1--;
							}
							break;
						}
					}

					// Defense (Red Button)
					if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == 0)
					{
						delay(2000000);
						if(roundSeed.array[i] == 0)
						{
							HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_10);
							delay(2000000);
							HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_10);
							delay(2000000);
							turn--;
							break;
						}
						else
						{
							HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_11);
							delay(2000000);
							HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_11);
							delay(2000000);
							if(turn % 2 == 0)
							{
								life1--;
							}
							else
							{
								life2--;
							}
							break;
						}
					}
				}

				// Check for game end
				if(life1 == 0)
				{
					sprintf(LCDOutput, "-PLAYER 2 WINS!-PRESS BU TO STRT");
					LCD_Print(LCDOutput);
					break;
				}
				else if(life2 == 0)
				{
					sprintf(LCDOutput, "-PLAYER 1 WINS!-PRESS BU TO STRT");
					LCD_Print(LCDOutput);
					break;
				}
				turn++;

			}
			free(roundSeed.array);

			// Check if games still happening
			if((life1 == 0) || (life2 == 0))
			{
				LCD_Print(LCDOutput);
				break;
			}
		}

		// Phase 2D: Conclusion
		delay(250000);
		while((HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == 1) && (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == 1))
		{
		}
		delay(250000);
	}
}

// Configure all GPIO Ports
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

	// Output PC (LCD, LED)
	GPIOC_Init.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10 | GPIO_PIN_11;
	GPIOC_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOC_Init.Pull = GPIO_NOPULL;
	GPIOC_Init.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC,&GPIOC_Init);

	// Input PC (Button)
	GPIOC_Init.Pin = GPIO_PIN_8 | GPIO_PIN_9;
	GPIOC_Init.Mode = GPIO_MODE_INPUT;
	GPIOC_Init.Pull = GPIO_PULLUP;
	GPIOC_Init.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC,&GPIOC_Init);
}

// Initialize the LCD to Display Information
void LCD_Init(void)
{
	delay(150000);
	LCD_Send(0x30, 0);     							// Function set
	delay(50000);
	LCD_Send(0x30, 0);     							// Function set again
	delay(50000);
	LCD_Send(0x30, 0);     							// Function set again
	delay(50000);
	LCD_Send(0x38, 0);     							// Set 8-bit mode, 2-line display, 5x8 font
	LCD_Send(0x0C, 0);     							// Display ON, cursor OFF
	LCD_Send(0x01, 0);     							// Clear display
	delay(100000);
	LCD_Send(0x06, 0);     							// Increment cursor after each character
}

// LCD Send Information
// cd = 0 -> command | cd = 1 -> data
void LCD_Send(uint8_t data, uint8_t cd)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, cd ? GPIO_PIN_SET : GPIO_PIN_RESET);

	GPIOC->ODR = (GPIOC->ODR & 0xFFFFFF00) | data;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
	delay(10000);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
	delay(10000);
}

// Print Screen to LCD
void LCD_Print(const char *str)
{
	int i = 0;
	LCD_Send(0x80, 0);
	while(*str)
	{
		if(i==16)
		{
			LCD_Send(0xC0, 0);
		}
		LCD_Send(*str++, 1);
		i++;
	}
}

// Create a Random Round Seed
RandomSeed getSeed(void)
{
	while(1)
	{
		int safe = 0;
		int danger = 0;

		// Create array length
		int length = (rand()%9) + 2;				// Create array length

		int *array = malloc(length * sizeof(int));	// Allocate memory and point to first array element

		for (int i = 0; i < length; i++)			// Fill the array with 1s and 0s
		{
			array[i] = rand() % 2;
			if(array[i] == 0) {safe++;}
			else {danger++;}
		}
		if((safe == 0) || (danger == 0)) {continue;}

		RandomSeed result = {array, length, safe, danger};
		return result;
	}
}

// For Loop Creates Soft Delay in Program
void delay(uint32_t delayVal)
{
	uint32_t i;
	for(i=0;i < delayVal; i++)
	{
	}
}
