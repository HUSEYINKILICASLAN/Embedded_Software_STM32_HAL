/*
 * max7219.h
 *
 *  Created on: 21 Eyl 2026
 *      Author: hkaslan84_2
 */

#ifndef INC_MAX7219_H_
#define INC_MAX7219_H_

#include "main.h" //main dosyasindaki tanimlamalari kullanabilmek icin yazildi


/* @DEFINE_GROUP_REGISTER_MAP */
#define MAX7219_NoOp				0x00	//register map te bulunan butun register lar eklenecek
#define MAX7219_Digit0				0x01
#define MAX7219_Digit1				0x02
#define MAX7219_Digit2				0x03
#define MAX7219_Digit3				0x04
#define MAX7219_Digit4				0x05
#define MAX7219_Digit5				0x06
#define MAX7219_Digit6				0x07
#define MAX7219_Digit7				0x08
#define MAX7219_Decode_Mode			0x09
#define MAX7219_Intensity			0x0A
#define MAX7219_Scan_Limit			0x0B
#define MAX7219_Shutdown			0x0C
#define MAX7219_Display_Test		0x0F

extern const uint8_t numbers[10][8];//numaralarin bulundugu matris tanimlandi, max7219.c dosyasinda fonksiyon govdesi oldugu icin extern ile yazildi, ram yerine flash hafizaya yazilmasi icin const yazildi

extern const uint8_t letters[26][8];//harflerin bulundugu matris tanimlandi, max7219.c dosyasinda fonksiyon govdesi oldugu icin extern ile yazildi, ram yerine flash hafizaya yazilmasi icin const yazildi

void MAX7219_Init(SPI_HandleTypeDef *hspi);//baslangic fonksiyonu prototipi tanimlandi

void MAX7219_Clear(SPI_HandleTypeDef *hspi);//ledlerin tamamini sondurme fonksiyonu tanimlandi

void MAX7219_Send_Data(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t data);//veri gonderme fonksiyonu tanimlandi (gerekli register ayarlari, boyut vb duzenlemeler var)

void MAX7219_Display_Matrix(SPI_HandleTypeDef *hspi, const uint8_t *matrix);//goruntulenecek deseni gonderme fonksiyonu tanimlandi

#endif /* INC_MAX7219_H_ */
