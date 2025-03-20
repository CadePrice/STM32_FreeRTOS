/*
 * main.c
 *
 *  Created on: Mar 13, 2025
 *      Author: CadeP
 */

#include "main.h"

int main(void)
{
	// 1. Initialize the HAL and configure the system clock
	HAL_Init();

	// 2. Configure GPIOs
	GPIO_Config();

	// 3. Create FreeRTOS tasks
	xTaskCreate(ButtonTask,      // Task function
			"ButtonTask",    // Task name (for debugging)
	        128,             // Stack size
	        NULL,            // Parameters
	        1,               // Priority
	        NULL);           // Task handle

	// 4. Start the FreeRTOS scheduler
	vTaskStartScheduler();

	// Should never reach here
	while(1);
}


void ButtonTask(void *params)
{
    // Local variable to track button state
    int flag = 1;

    for(;;) // FreeRTOS task loop
    {
        // If button is pressed (Pin = 0) and we haven't toggled yet
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == 0 && flag == 1)
		{
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_10);
			flag = 0;
		}
		else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == 1 && flag == 0)
		{
			flag = 1;
		}

        // Non-blocking delay: 50ms
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void GPIO_Config(void)
{
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIOC_Init = {0};

    // LED Output (PC10)
    GPIOC_Init.Pin = GPIO_PIN_10 | GPIO_PIN_11;
    GPIOC_Init.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOC_Init.Pull = GPIO_NOPULL;
    GPIOC_Init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC,&GPIOC_Init);

    // Button Input (PC8)
    GPIOC_Init.Pin = GPIO_PIN_8;
    GPIOC_Init.Mode = GPIO_MODE_INPUT;
    GPIOC_Init.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC,&GPIOC_Init);
}
