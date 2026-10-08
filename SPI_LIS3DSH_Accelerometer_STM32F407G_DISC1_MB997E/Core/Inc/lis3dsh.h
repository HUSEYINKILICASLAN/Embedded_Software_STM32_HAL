/*
 * lis3dsh.h
 *
 *  LIS3DSH ivmeolcer surucusu - STM32F407G-DISC1 (MB997C ve sonrasi, U5=LIS3DSH)
 *  lis302dl.c/.h ile ayni fonksiyon seti + ek ozellikler
 */

#ifndef INC_LIS3DSH_H_
#define INC_LIS3DSH_H_

#include "main.h"	//main dosyasindaki tanimlamalari kullanabilmek icin yazildi

/* @DEFINE_GROUP_REGISTER_MAP */
#define LIS3DSH_REG_INFO1					0x0D
#define LIS3DSH_REG_INFO2					0x0E
#define LIS3DSH_REG_WHO_AM_I				0x0F	//sabit deger: 0x3F

#define LIS3DSH_REG_OFF_X					0x10
#define LIS3DSH_REG_OFF_Y					0x11
#define LIS3DSH_REG_OFF_Z					0x12
#define LIS3DSH_REG_CS_X					0x13
#define LIS3DSH_REG_CS_Y					0x14
#define LIS3DSH_REG_CS_Z					0x15
#define LIS3DSH_REG_LC_L					0x16
#define LIS3DSH_REG_LC_H					0x17
#define LIS3DSH_REG_STAT					0x18
#define LIS3DSH_REG_PEAK1					0x19
#define LIS3DSH_REG_PEAK2					0x1A
#define LIS3DSH_REG_VFC_1					0x1B
#define LIS3DSH_REG_VFC_2					0x1C
#define LIS3DSH_REG_VFC_3					0x1D
#define LIS3DSH_REG_VFC_4					0x1E
#define LIS3DSH_REG_THRS3					0x1F

#define LIS3DSH_REG_CTRL_REG4				0x20	//ODR + eksen (X/Y/Z) aktivasyonu + BDU burada (LIS302DL deki CTRL_REG1 e karsilik gelir)
#define LIS3DSH_REG_CTRL_REG1				0x21	//state machine 1 konfigurasyonu
#define LIS3DSH_REG_CTRL_REG2				0x22	//state machine 2 konfigurasyonu
#define LIS3DSH_REG_CTRL_REG3				0x23	//interrupt konfigurasyonu
#define LIS3DSH_REG_CTRL_REG5				0x24	//full scale + bandwidth + self test + SPI modu burada
#define LIS3DSH_REG_CTRL_REG6				0x25	//interrupt2 + FIFO + boot + address auto-increment

#define LIS3DSH_REG_STATUS					0x27	//ZYXDA (veri hazir) biti burada
#define LIS3DSH_REG_OUT_X_L					0x28
#define LIS3DSH_REG_OUT_X_H					0x29
#define LIS3DSH_REG_OUT_Y_L					0x2A
#define LIS3DSH_REG_OUT_Y_H					0x2B
#define LIS3DSH_REG_OUT_Z_L					0x2C
#define LIS3DSH_REG_OUT_Z_H					0x2D

#define LIS3DSH_REG_FIFO_CTRL_REG			0x2E
#define LIS3DSH_REG_FIFO_SRC_REG			0x2F

#define LIS3DSH_READ						0x80	//okuma biti (bit7=1)
#define LIS3DSH_WRITE						0x00	//yazma biti (bit7=0)
#define LIS3DSH_AUTO_INCREMENT				0x40	//coklu byte okuma/yazmada adresi otomatik artirir (bit6=1)

/* Hassasiyet degerleri (g/LSB), 16 bit ham veri icin - full scale e gore degisir */
#define LIS3DSH_SENSITIVITY_2G				0.00006f
#define LIS3DSH_SENSITIVITY_4G				0.00012f
#define LIS3DSH_SENSITIVITY_6G				0.00018f
#define LIS3DSH_SENSITIVITY_8G				0.00024f
#define LIS3DSH_SENSITIVITY_16G				0.00073f

#define LIS3DSH_CS_LOW						HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
#define LIS3DSH_CS_HIGH						HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);

/* ODR (Output Data Rate) secenekleri - CTRL_REG4 bit4-7 */
#define LIS3DSH_ODR_POWERDOWN				0x00
#define LIS3DSH_ODR_3_125HZ					0x01
#define LIS3DSH_ODR_6_25HZ					0x02
#define LIS3DSH_ODR_12_5HZ					0x03
#define LIS3DSH_ODR_25HZ					0x04
#define LIS3DSH_ODR_50HZ					0x05
#define LIS3DSH_ODR_100HZ					0x06
#define LIS3DSH_ODR_200HZ					0x07
#define LIS3DSH_ODR_400HZ					0x08
#define LIS3DSH_ODR_800HZ					0x09
#define LIS3DSH_ODR_1600HZ					0x0A

/* Full scale secenekleri - CTRL_REG5 bit3-5 */
#define LIS3DSH_FS_2G						0x00
#define LIS3DSH_FS_4G						0x01
#define LIS3DSH_FS_6G						0x02
#define LIS3DSH_FS_8G						0x03
#define LIS3DSH_FS_16G						0x04

typedef enum {
	LIS3DSH_INIT_FAIL = 0, LIS3DSH_INIT_SUCCESS = 1,
} LIS3DSH_Init_Status;

/* CTRL_REG4: ODR + BDU + eksen aktivasyonu (LSB->MSB sirasiyla) */
typedef struct {
	uint8_t Xen :1;	//X ekseni aktif/pasif
	uint8_t Yen :1;	//Y ekseni aktif/pasif
	uint8_t Zen :1;	//Z ekseni aktif/pasif
	uint8_t BDU :1;	//Block Data Update (okuma bitene kadar guncellemeyi bekletir)
	uint8_t ODR :4;	//Output Data Rate (veri hizi)
} CTRL_REG4_Register_t;

/* CTRL_REG5: SIM + Self Test + Full Scale + Bandwidth (LSB->MSB sirasiyla) */
typedef struct {
	uint8_t SIM :1;	//SPI arayuz modu (0: 4-wire, 1: 3-wire)
	uint8_t ST :2;		//self test modu
	uint8_t FSCALE :3;	//full scale secimi
	uint8_t BW :2;		//anti-alias filtre bandwidth
} CTRL_REG5_Register_t;

uint8_t LIS3DSH_Read_Whoami(SPI_HandleTypeDef *hspi);

LIS3DSH_Init_Status LIS3DSH_Init(SPI_HandleTypeDef *hspi);

void LIS3DSH_Read_Accelerometer(SPI_HandleTypeDef *hspi, int16_t *Accelerometer_Data);

void LIS3DSH_Read_G_Data(SPI_HandleTypeDef *hspi, int16_t *Accelerometer_Data, float *G_Data);

/* --- EK OZELLIKLER --- */

uint8_t LIS3DSH_Read_Register(SPI_HandleTypeDef *hspi, uint8_t reg);	//tek bir register i okumak icin genel amacli fonksiyon

void LIS3DSH_Write_Register(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t data);	//tek bir register e yazmak icin genel amacli fonksiyon

uint8_t LIS3DSH_Is_Data_Ready(SPI_HandleTypeDef *hspi);	//STATUS registerindeki ZYXDA bitini kontrol eder, yeni veri hazir mi diye

void LIS3DSH_Set_Full_Scale(SPI_HandleTypeDef *hspi, uint8_t fs_value);	//calisirken full scale (2g/4g/6g/8g/16g) degistirmek icin

void LIS3DSH_Read_Accelerometer_Burst(SPI_HandleTypeDef *hspi, int16_t *Accelerometer_Data);	//auto-increment ile tek CS islemiyle 6 byte okur, daha hizli

#endif /* INC_LIS3DSH_H_ */
