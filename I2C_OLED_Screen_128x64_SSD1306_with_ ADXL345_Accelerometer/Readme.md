In this example, I used the STM32 NUCLEO-F030R8 (MB1136 C-04) development board with a ADXL345 triple axis accelerometer with I2C2 and configuring pins PB10 and PB11. Also 128x64 pixel OLED screen featuring the SSD1306 module with I2C1 and configuring pins PB8 and PB9. After initially creating the project with I2C1 and I2C2 enabled. And i copied the files; adxl345.h, ssd1306.h, ssd1306_conf.h, ssd1306_fonts.h, and ssd1306_tests.h and pasted them into the Inc folder within the project directory. Next, I copied adxl345.c, ssd1306.c, ssd1306_fonts.c, and ssd1306_tests.c and pasted them into the Src folder. I then right-clicked on the project in STM32CubeIDE and selected "Refresh," confirming that the pasted files appeared under the Inc and Src folders. I opened the ssd1306.h file located in the Inc folder and then opened ssd1306_conf.h (referenced on line 16). In that file, I disabled the STM32F4 setting (line 12) and enabled the STM32F0 setting (line 10). Next, i saw the axis datas while running with live expression of CubeIDE. Then i successfully got the OLED screen working.

Citation: https://xbowtie.com/egitmen/arifmandal



https://github.com/user-attachments/assets/eaabee50-60b0-4654-8ceb-9f4609f00648

