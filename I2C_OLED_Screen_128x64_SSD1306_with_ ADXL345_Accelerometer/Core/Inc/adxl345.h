/*
 * adxl345.h
 *
 *  Created on: 16 Eyl 2026
 *      Author: hkaslan84_2
 */

#ifndef INC_ADXL345_H_
#define INC_ADXL345_H_

#include "main.h"	//main dosyasindaki tanimlamalari kullanabilmek icin yazildi

/* @DEFINE_GROUP DEVICE_ADDRESS */
#define ADXL345_DEVICE_ADDRESS 0xA6	//i2c modulun adresi tanimlandi

/* @DEFINE_GROUP_REGISTER_MAP */
#define DEVID          		0x00		//register map te bulunan butun register lar eklenecek
#define THRESH_TAP			0x1D
#define OFSX				0x1E
#define OFSY				0x1F
#define OFSZ				0x20
#define DUR					0x21
#define Latent				0x22
#define Window				0x23
#define THRESH_ACT			0x24
#define THRESH_INACT		0x25
#define TIME_INACT			0x26
#define ACT_INACT_CTL		0x27
#define THRESH_FF			0x28
#define TIME_FF				0x29
#define TAP_AXES			0x2A
#define ACT_TAP_STATUS		0x2B
#define BW_RATE				0x2C
#define POWER_CTL			0x2D
#define INT_ENABLE			0x2E
#define INT_MAP				0x2F
#define INT_SOURCE			0x30
#define DATA_FORMAT			0x31
#define DATAX0				0x32
#define DATAX1				0x33
#define DATAY0				0x34
#define DATAY1				0x35
#define DATAZ0				0x36
#define DATAZ1				0x37
#define FIFO_CTL			0x38
#define FIFO_STATUS			0x39

#define TIMEOUT				1000

/* @DEFINE_GROUP WAKEUP_FREQUENCY */
#define WAKE_UP_8HZ			0x00	//power_ctl register inin wake up reading bitleri tanimlandi
#define WAKE_UP_4HZ			0x01
#define WAKE_UP_2HZ			0x02
#define WAKE_UP_1HZ			0x03

/* @DEFINE_GROUP G_RANGE */
#define RANGE_2G			0x00	//data_format register inin range bitleri tanimlandi
#define RANGE_4G			0x01
#define RANGE_8G			0x02
#define RANGE_16G			0x03

/* @DEFINE_GROUP BANDWIDTH_RATE */
#define BW_RATE_1600		0x0F	//BW_RATE register inin rate bitleri tanimlandi
#define BW_RATE_800			0x0E
#define BW_RATE_400			0x0D
#define BW_RATE_200			0x0C
#define BW_RATE_100			0x0B
#define BW_RATE_50			0x0A
#define BW_RATE_25			0x09
#define BW_RATE_12_5		0x08
#define BW_RATE_6_25		0x07
#define BW_RATE_3_13		0x06
#define BW_RATE_1_56		0x05
#define BW_RATE_0_78		0x04
#define BW_RATE_0_39		0x03
#define BW_RATE_0_20		0x02
#define BW_RATE_0_10		0x01
#define BW_RATE_0_05		0x00

/* @DEFINE_GROUP AXIS */
#define X					DATAX0	//DATAX0 register inin kisaltilmasi icin yazildi
#define Y					DATAY0	//DATAY0 register inin kisaltilmasi icin yazildi
#define Z					DATAZ0	//DATAZ0 register inin kisaltilmasi icin yazildi

/* @DEFINE_GROUP SCALE_FACTOR */
#define SCALE_FACTOR_2G		(float)1/256.0	//G degerlerini bulmak icin gereken katsayilar tanimlandi datasheet e gore
#define SCALE_FACTOR_4G		(float)1/128.0
#define SCALE_FACTOR_8G		(float)1/64.0
#define SCALE_FACTOR_16G	(float)1/32.0


typedef enum {	//fonksiyon okuma yaptiginda geri donecek degeri belirlemek icin yazildi
	READ_FAIL = 0,
	READ_SUCCESS = 1,
} ADXL345_READ_STATUS;


typedef enum {	//fonksiyon basladiginda geri donecek degeri belirlemek icin yazildi
	INIT_FAIL = 0,
	INIT_SUCCESS = 1,
}ADXL345_INIT_STATUS;


typedef enum {	//fonksiyon basladiginda geri donecek degeri belirlemek icin yazildi
	WRITE_FAIL = 0,
	WRITE_SUCCESS = 1,
}ADXL345_WRITE_STATUS;


typedef struct{	//POWER_CTL register inin bitleri icin tanimlandi
	uint8_t Wakeup:2;	//(LSB) 0x00 -> 8Hz, 0x01 -> 4Hz, 0x02 -> 2Hz, 0x03 -> 1Hz (@DEFINE_GROUP WAKEUP_FREQUENCY)
	uint8_t Sleep:1;	//2.bit
	uint8_t Measure:1;	//3.bit
	uint8_t AUTO_SLEEP:1;	//4.bit
	uint8_t Link:1;			//5.bit
	uint8_t Reserved:2;		//6.bit
}Power_Control_Register_t;


typedef struct{	//DATA_FORMAT register inin bitleri icin tanimlandi
	uint8_t Range:2;		//(LSB) 0x00 -> +-2G, 0x01 -> +-4G, 0x02 -> +-8G, 0x03 -> +-16G (@DEFINE_GROUP G_RANGE)
	uint8_t Justify:1;		//2.bit
	uint8_t FULL_RES:1;		//3.bit
	uint8_t Reserved:1;		//4.bit
	uint8_t INT_INVERT:1;	//5.bit
	uint8_t SPI:1;			//6.bit
	uint8_t SELF_TEST:1;	//7.bit
}Data_Format_Register_t;


typedef struct{	//BW_RATE register inin bitleri icin tanimlandi
	uint8_t Rate:4;		//(LSB) 0x00 -> 0.05Hz, 0x01 -> 0.1Hz, 0x02 -> 0.2Hz, 0x03 -> 0.39Hz, 0x04 -> 0.78Hz, 0x05 -> 1.56Hz, 0x06 -> 3.13Hz, 0x07 -> 6.25Hz, 0x08 -> 12.5Hz, 0x09 -> 25Hz, 0x0A -> 50Hz, 0x0B -> 100Hz, 0x0C -> 200Hz, 0x0D -> 400Hz, 0x0E -> 800Hz, 0x0F -> 1600Hz (@DEFINE_GROUP BANDWIDTH_RATE)
	uint8_t LOW_POWER:1;	//4.bit
	uint8_t Reserved:3;		//5-6-7.bitler
}BW_Rate_Register_t;




int ADXL345_Scan_Device_ID(I2C_HandleTypeDef *hi2cx);	//i2c modul adres arama fonksiyonu prototipi

ADXL345_INIT_STATUS ADXL345_Init(I2C_HandleTypeDef *hi2cx);	//kutuphaneyi baslatma fonksiyon prototipi

//ADXL345_READ_STATUS ADXL345_Read_Register_Data(I2C_HandleTypeDef *hi2cx, uint16_t Register_Address, uint16_t Size_of_Data, uint8_t *Data_Buffer);//register datasini okumak icin olusturulan fonksiyonun prototipi

//ADXL345_WRITE_STATUS ADXL345_Write_Register_Data(I2C_HandleTypeDef *hi2cx, uint16_t Register_Address, uint16_t Value);//register datasina veri yazmak icin olusturulan fonksiyonun prototipi

int16_t ADXL345_Get_Axis_Value(I2C_HandleTypeDef *hi2cx, uint8_t Axis);	//x,y,z eksenlerinden veri almak icin gereken fonksiyon prototipi yazildi (@DEFINE_GROUP AXIS)

float ADXL345_Get_G_Value(I2C_HandleTypeDef *hi2cx, uint8_t Axis, float Scale_Factor);	//x,y,z eksenlerinden alinan verileri G cinsine cevirmek icin gereken fonksiyon prototipi yazildi (@DEFINE_GROUP AXIS) (@DEFINE_GROUP SCALE_FACTOR)

#endif /* INC_ADXL345_H_ */
