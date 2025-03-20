/*
 * main.c
 *
 *  Created on: Mar 13, 2025
 *      Author: CadeP
 */

#include "main.h"

// Global Variables
int roundCount, turn, life1, life2;
char LCDOutput[33];

RandomSeed roundSeed;  // Global variable
TaskHandle_t mainMenuTaskHandle = NULL;
TaskHandle_t roundTaskHandle = NULL;
TaskHandle_t gameTaskHandle = NULL;
TaskHandle_t endTaskHandle = NULL;

int main(void)
{
	// PHASE 0: GPIO Configurations, LCD Configurations, Variable Initialization
	HAL_Init();
	SystemClock_Config();

	GPIO_Config();
	LCD_Init();
	srand(time(NULL));     															// Generates Round Seed

	xTaskCreate(MainMenuTask, "MainMenuTask", 128, NULL, 4, &mainMenuTaskHandle);	// Main Menu Where Program Begins
	xTaskCreate(RoundTask, "RoundTask", 128, NULL, 3, &roundTaskHandle);			// Round Screen. Generates Seed
	xTaskCreate(GameTask, "GameTask", 128, NULL, 2, &gameTaskHandle);				// Where Each Round is Played Out
	xTaskCreate(EndTask, "EndTask", 128, NULL, 1, &endTaskHandle);					// Display Winner

	vTaskSuspend(roundTaskHandle);
	vTaskSuspend(gameTaskHandle);
	vTaskSuspend(endTaskHandle);
	vTaskStartScheduler();
	while(1);
}

void MainMenuTask(void *params)
{
	for(;;)
	{
		sprintf(LCDOutput, "-BUTTONROULETTE-PRESS BU TO STRT");
		LCD_Print(LCDOutput);
		if((HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == 0) || (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == 0))
		{
			vTaskDelete(roundTaskHandle);
			roundTaskHandle = NULL;  // Clear the old handle
			xTaskCreate(RoundTask, "RoundTask", 128, NULL, 3, &roundTaskHandle);
	        vTaskDelay(pdMS_TO_TICKS(2));

			vTaskDelete(gameTaskHandle);
			gameTaskHandle = NULL;  // Clear the old handle
			xTaskCreate(GameTask, "GameTask", 128, NULL, 2, &gameTaskHandle);
			vTaskSuspend(gameTaskHandle);

			vTaskResume(roundTaskHandle);
			vTaskSuspend(NULL);
		}
        vTaskDelay(pdMS_TO_TICKS(2));
	}
}

void RoundTask (void *params)
{
	// PHASE 2A: Initialize Game Variables
	roundCount = 0;
	turn  = 0;
	life1 = 3;
	life2 = 3;
	for(;;)
	{
		// PHASE 2B: Round Screen
		roundCount++;
		roundSeed = getSeed();
		sprintf(LCDOutput, "    ROUND  %d    SAFE:%d |DANGER:%d", roundCount, roundSeed.safeCount, roundSeed.dangerCount);
		LCD_Print(LCDOutput);
        vTaskDelay(pdMS_TO_TICKS(4000));

        vTaskResume(gameTaskHandle);
        vTaskSuspend(NULL);
	}
}

void GameTask (void *params)
{
	for(;;)
	{
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
					vTaskDelay(pdMS_TO_TICKS(1000));
					if(roundSeed.array[i] == 0)
					{
						HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_10);
						vTaskDelay(pdMS_TO_TICKS(1000));
						HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_10);
						vTaskDelay(pdMS_TO_TICKS(1000));
						break;
					}
					else
					{
						HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_11);
						vTaskDelay(pdMS_TO_TICKS(1000));
						HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_11);
						vTaskDelay(pdMS_TO_TICKS(1000));
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
					vTaskDelay(pdMS_TO_TICKS(1000));
					if(roundSeed.array[i] == 0)
					{
						HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_10);
						vTaskDelay(pdMS_TO_TICKS(1000));
						HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_10);
						vTaskDelay(pdMS_TO_TICKS(1000));
						turn--;
						break;
					}
					else
					{
						HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_11);
						vTaskDelay(pdMS_TO_TICKS(1000));
						HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_11);
						vTaskDelay(pdMS_TO_TICKS(1000));
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
			vTaskResume(endTaskHandle);
			vTaskSuspend(NULL);
		}
		vTaskResume(roundTaskHandle);
		vTaskSuspend(NULL);
	}
}

void EndTask (void *params)
{
	for(;;)
	{
		if((HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == 0) || (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == 0))
		{
			vTaskResume(mainMenuTaskHandle);
			vTaskSuspend(NULL);
		}
        vTaskDelay(pdMS_TO_TICKS(2));
	}
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
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
