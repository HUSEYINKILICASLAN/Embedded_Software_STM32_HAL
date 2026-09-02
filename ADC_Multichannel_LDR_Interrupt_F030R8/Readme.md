In this example, i used STM32 NUCLEO-F030R8 MB1136 C-04 development board for reading potentiometer values and reading LDR values ​​using the multichannel of ADC in interrupt mode. I used PA1, PA4, PB0 and PC1 pins for on/off LED in bar-graph mode according to potentiometer position also PA0 and PC0 pins for ADC inputs. I used a function that can read potentiometer value and calculate the corresponding voltage between 0 to 3.3V. And i used constrain and map functions for arranging which LED will on or off. If turn the pot to right, this will increase the voltage and LEDs will on and reverse is also true. I used green LEDs and beadboard and 1K resistances. I used voltage divider circuit for reading LDR values with 10K resistance. I also sent the read values ​​from the serial port to the computer via USART2 using the `sprintf` function.



https://github.com/user-attachments/assets/b20b36f3-c17b-4619-98c8-392141567f0c

