In this example, I used the STM32 NUCLEO-F030R8 (MB1136 C-04) development board with a 128x64 pixel OLED screen featuring the SSD1306 module. After initially creating the project with I2C1 enabled and configuring pins PB8 and PB9 as SCL and SDA, I copied the files `ssd1306.h`, `ssd1306_conf.h`, `ssd1306_fonts.h`, and `ssd1306_tests.h` and pasted them into the `Inc` folder within the project directory. Next, I copied `ssd1306.c`, `ssd1306_fonts.c`, and `ssd1306_tests.c` and pasted them into the `Src` folder. I then right-clicked on the project in STM32CubeIDE and selected "Refresh," confirming that the pasted files appeared under the `Inc` and `Src` folders.

I opened the `ssd1306.h` file located in the `Inc` folder and then opened `ssd1306_conf.h` (referenced on line 16). In that file, I disabled the STM32F4 setting (line 12) and enabled the STM32F0 setting (line 10). Returning to `ssd1306.h`, I noted the function definitions starting at line 142. Subsequently, I included the necessary libraries in the `main` file; since `ssd1306.h` already includes both `ssd1306_conf.h` and `ssd1306_fonts.h`, I only needed to include `ssd1306.h` and `ssd1306_tests.h` (to display test patterns). To display my name, I created a character array named `name` and assigned the string "Huseyin" to it. Finally, under the "User Code Begin 2" section, I added `ssd1306_Init();` and `ssd1306_Fill(Black);`. I wrote the code for these functions; they initialize the SSD1306 and set the screen to a dark color.

Next, inside the `while` block, I wrote `ssd1306_SetCursor(10, 6);` to position the cursor. I then wrote `ssd1306_WriteString(name, Font_7x10, White);` to display my name. This function writes my name to the screen using 7x10-sized characters in the specified color (white, blue, yellow, etc.). I repeated this code three more times, resulting in my name appearing four times, stacked vertically on the screen. Finally, I wrote `ssd1306_UpdateScreen();`, which refreshes the display. If desired, a dead pixel check can be performed using the `tests` function.

<img width="1463" height="752" alt="i2c1" src="https://github.com/user-attachments/assets/2945782f-4064-42b5-b0fb-b173ccb308b8" />

<img width="1483" height="708" alt="i2c2" src="https://github.com/user-attachments/assets/03dea59b-e99b-4f64-89d6-439aee5a0d21" />

<img width="1460" height="675" alt="i2c3" src="https://github.com/user-attachments/assets/3bc6447f-88ea-4ae7-8819-7be86a348712" />

<img width="1349" height="618" alt="i2c4" src="https://github.com/user-attachments/assets/8c27eeb4-e210-4012-817b-1aeef10819cc" />

<img width="1157" height="615" alt="i2c5" src="https://github.com/user-attachments/assets/97a7b589-fcf9-4707-b4a9-30aae747f135" />

<img width="1134" height="503" alt="i2c6" src="https://github.com/user-attachments/assets/96cbfefd-936d-448b-a540-80a23b60fed2" />

<img width="1253" height="401" alt="i2c7" src="https://github.com/user-attachments/assets/db6fe63e-9518-47dc-8132-923d1b634524" />

<img width="1646" height="776" alt="i2c8" src="https://github.com/user-attachments/assets/cc9b4a18-ee29-484e-906b-9c1eeadc7710" />

<img width="1560" height="686" alt="i2c9" src="https://github.com/user-attachments/assets/dd2b5202-8447-485a-bcc9-cf28ab4190f2" />

<img width="1564" height="677" alt="i2c10" src="https://github.com/user-attachments/assets/fde84088-e39d-415c-aeed-e3d0fd7ee082" />

<img width="1513" height="774" alt="i2c11" src="https://github.com/user-attachments/assets/4a4a23b3-53a8-4466-aa53-f075b2904c11" />

Citation: https://xbowtie.com/egitmen/arifmandal



https://github.com/user-attachments/assets/e19a5197-da80-4be4-8280-77a33ad20427

