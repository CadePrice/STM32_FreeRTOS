Project 1 - BtnLED
Introduced me to Stm32CubeIDE and C using a NUCLEO-F401RE microcontroller. Configured GPIO ports to create a latching mechanic using a button and an LED.
  - GPIOC P8: LED Output
  - GPIOC P10: Button Input
  - 5V, resistors, and GND connected appropriately.

Project 2 - LCD
Configured a 16x2 LCD in 8-bit mode to output the value of a given input.
  - GND: R/W, VSS, Potentiometer
  - 5V: VDD, Potentiometer
  - V0: Potentiometer (middle)
  - GPIOA P5: RS
  - GPIOA P7: E
  - GPIOC P0: D0
  - GPIOC P1: D1
  - GPIOC P2: D2
  - GPIOC P3: D3
  - GPIOC P4: D4
  - GPIOC P5: D5
  - GPIOC P6: D6
  - GPIOC P7: D7

Project 3 - Button Roulette
Created a basic game using LED, button, LCD, random number generation, and logic.
  - GND: R/W, VSS, Potentiometer
  - 5V: VDD, Potentiometer
  - V0: Potentiometer (middle)
  - GPIOA P5: RS
  - GPIOA P7: E
  - GPIOC P0: D0
  - GPIOC P1: D1
  - GPIOC P2: D2
  - GPIOC P3: D3
  - GPIOC P4: D4
  - GPIOC P5: D5
  - GPIOC P6: D6
  - GPIOC P7: D7
  - GPIOC P8: Button 1
  - GPIOC P9: Button 2
  - GPIOC P10: Led 1
  - GPIOC P10: Led 2
  - 5V, resistors, and GND connected appropriately.

Project 4 - BtnLEDRTOS
Introduced me to FreeRTOS, adding tasks and the movement between tasks into the previous BtnLED project.
  - Same GPIO as Project 1

Project 5 - ButtonRouletteRTOS
Used FreeRTOS and tasks to recreate the flow of the previous Button Roulette project.
  - Same GPIO as Project 3
