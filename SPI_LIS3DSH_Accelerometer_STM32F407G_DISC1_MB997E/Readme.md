In this example, I used the STM32F407G-Disc1 MB997E Discovery Development Board because of onboard LIS3DSH module. I used SPI1 for communication at 100MHz and master. I choose clock polarity low and clock phase 1 edge, data size 8 bits, first bit MSB, prescaler 2, baud rate 6.25 MBit/s, CRC disable, NSS software I used pin PE3 as CS (GPIO output level High), pin PA5 as SCK and pin PA7 as MOSI. I wrote the lis3dsh.c and lis3dsh.h library files. I observed the changes axis of X-Y-Z accelerometer values on live expressions. 

citation: https://xbowtie.com/egitmen/arifmandal



https://github.com/user-attachments/assets/3e276cfc-089d-4eca-b342-f699a7f6d7a0

