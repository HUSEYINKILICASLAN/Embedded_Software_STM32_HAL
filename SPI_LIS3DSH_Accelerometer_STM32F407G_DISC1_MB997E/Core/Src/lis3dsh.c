/*
 * lis3dsh.c
 *
 *  Created on: 2 Eki 2026
 *      Author: hkaslan84_2
 */
/*
 * lis3dsh.c
 *
 *  LIS3DSH ivmeolcer surucusu - STM32F407G-DISC1 (MB997C ve sonrasi, U5=LIS3DSH)
 */

#include "lis3dsh.h"

/* su an aktif olan full scale e gore hassasiyet (g/LSB) degerini tutar,
 * LIS3DSH_Init ve LIS3DSH_Set_Full_Scale fonksiyonlari tarafindan guncellenir,
 * LIS3DSH_Read_G_Data bu degeri kullanir */
static float Current_Sensitivity = LIS3DSH_SENSITIVITY_2G;

uint8_t LIS3DSH_Read_Whoami(SPI_HandleTypeDef *hspi) {
	return LIS3DSH_Read_Register(hspi, LIS3DSH_REG_WHO_AM_I);	//sabit deger 0x3F donmesi beklenir
}

LIS3DSH_Init_Status LIS3DSH_Init(SPI_HandleTypeDef *hspi) {
	CTRL_REG4_Register_t CTRL_REG4_Register = { 0 };
	CTRL_REG5_Register_t CTRL_REG5_Register = { 0 };
	uint8_t Temp_Reg = 0;

	/* CTRL_REG4: 100Hz veri hizi, BDU acik (yirtilmis veri okumayi engeller), 3 eksen de aktif */
	CTRL_REG4_Register.ODR = LIS3DSH_ODR_100HZ;
	CTRL_REG4_Register.BDU = 0x01;
	CTRL_REG4_Register.Zen = 0x01;
	CTRL_REG4_Register.Yen = 0x01;
	CTRL_REG4_Register.Xen = 0x01;

	Temp_Reg = *((uint8_t*) &CTRL_REG4_Register);
	LIS3DSH_Write_Register(hspi, LIS3DSH_REG_CTRL_REG4, Temp_Reg);

	/* CTRL_REG5: +-2g full scale, 4-wire SPI (SIM=0), self test kapali */
	CTRL_REG5_Register.FSCALE = LIS3DSH_FS_2G;
	CTRL_REG5_Register.SIM = 0x00;
	CTRL_REG5_Register.ST = 0x00;
	CTRL_REG5_Register.BW = 0x00;

	Temp_Reg = *((uint8_t*) &CTRL_REG5_Register);
	LIS3DSH_Write_Register(hspi, LIS3DSH_REG_CTRL_REG5, Temp_Reg);

	Current_Sensitivity = LIS3DSH_SENSITIVITY_2G;	//init te secilen full scale ile hassasiyeti eslestir

	return LIS3DSH_INIT_SUCCESS;
}

void LIS3DSH_Read_Accelerometer(SPI_HandleTypeDef *hspi, int16_t *Accelerometer_Data) {
	uint8_t Low_Byte = 0, High_Byte = 0;

	/* X ekseni - once dusuk byte sonra yuksek byte okunur, 16 bit e birlestirilir */
	Low_Byte = LIS3DSH_Read_Register(hspi, LIS3DSH_REG_OUT_X_L);
	High_Byte = LIS3DSH_Read_Register(hspi, LIS3DSH_REG_OUT_X_H);
	Accelerometer_Data[0] = (int16_t) ((High_Byte << 8) | Low_Byte);

	/* Y ekseni */
	Low_Byte = LIS3DSH_Read_Register(hspi, LIS3DSH_REG_OUT_Y_L);
	High_Byte = LIS3DSH_Read_Register(hspi, LIS3DSH_REG_OUT_Y_H);
	Accelerometer_Data[1] = (int16_t) ((High_Byte << 8) | Low_Byte);

	/* Z ekseni */
	Low_Byte = LIS3DSH_Read_Register(hspi, LIS3DSH_REG_OUT_Z_L);
	High_Byte = LIS3DSH_Read_Register(hspi, LIS3DSH_REG_OUT_Z_H);
	Accelerometer_Data[2] = (int16_t) ((High_Byte << 8) | Low_Byte);
}

void LIS3DSH_Read_G_Data(SPI_HandleTypeDef *hspi, int16_t *Accelerometer_Data, float *G_Data) {
	for (int i = 0; i < 3; i++) {
		G_Data[i] = Accelerometer_Data[i] * Current_Sensitivity;	//su anki full scale a gore hassasiyetle carpilir
	}
}

/* --- EK OZELLIKLER --- */

uint8_t LIS3DSH_Read_Register(SPI_HandleTypeDef *hspi, uint8_t reg) {
	uint8_t Data = 0;
	uint8_t Reg_Address = reg | LIS3DSH_READ;

	LIS3DSH_CS_LOW;
	HAL_SPI_Transmit(hspi, &Reg_Address, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(hspi, &Data, 1, HAL_MAX_DELAY);
	LIS3DSH_CS_HIGH;

	return Data;
}

void LIS3DSH_Write_Register(SPI_HandleTypeDef *hspi, uint8_t reg, uint8_t data) {
	uint8_t Reg_Address = reg | LIS3DSH_WRITE;

	LIS3DSH_CS_LOW;
	HAL_SPI_Transmit(hspi, &Reg_Address, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(hspi, &data, 1, HAL_MAX_DELAY);
	LIS3DSH_CS_HIGH;
}

uint8_t LIS3DSH_Is_Data_Ready(SPI_HandleTypeDef *hspi) {
	uint8_t Status_Reg = LIS3DSH_Read_Register(hspi, LIS3DSH_REG_STATUS);
	return (Status_Reg & 0x08) ? 1 : 0;	//ZYXDA biti (bit3): yeni X,Y,Z verisi hazirsa 1
}

void LIS3DSH_Set_Full_Scale(SPI_HandleTypeDef *hspi, uint8_t fs_value) {
	uint8_t Current_CTRL_REG5 = LIS3DSH_Read_Register(hspi,
			LIS3DSH_REG_CTRL_REG5);

	Current_CTRL_REG5 &= ~(0x07 << 3);	//FSCALE bitlerini (bit3-5) temizle
	Current_CTRL_REG5 |= ((fs_value & 0x07) << 3);	//yeni full scale degerini yerlestir

	LIS3DSH_Write_Register(hspi, LIS3DSH_REG_CTRL_REG5, Current_CTRL_REG5);

	/* Current_Sensitivity i de yeni full scale ile eslestir, aksi halde
	 * LIS3DSH_Read_G_Data yanlis hassasiyetle hesaplama yapmaya devam eder */
	switch (fs_value) {
	case LIS3DSH_FS_2G:
		Current_Sensitivity = LIS3DSH_SENSITIVITY_2G;
		break;
	case LIS3DSH_FS_4G:
		Current_Sensitivity = LIS3DSH_SENSITIVITY_4G;
		break;
	case LIS3DSH_FS_6G:
		Current_Sensitivity = LIS3DSH_SENSITIVITY_6G;
		break;
	case LIS3DSH_FS_8G:
		Current_Sensitivity = LIS3DSH_SENSITIVITY_8G;
		break;
	case LIS3DSH_FS_16G:
		Current_Sensitivity = LIS3DSH_SENSITIVITY_16G;
		break;
	default:
		Current_Sensitivity = LIS3DSH_SENSITIVITY_2G;
		break;
	}
}

void LIS3DSH_Read_Accelerometer_Burst(SPI_HandleTypeDef *hspi,
		int16_t *Accelerometer_Data) {
	uint8_t Reg_Address = LIS3DSH_REG_OUT_X_L | LIS3DSH_READ
			| LIS3DSH_AUTO_INCREMENT;	//auto-increment biti sayesinde chip, adresi kendi kendine artirir
	uint8_t Raw_Data[6] = { 0 };	//X_L,X_H,Y_L,Y_H,Z_L,Z_H sirasiyla gelir

	LIS3DSH_CS_LOW;
	HAL_SPI_Transmit(hspi, &Reg_Address, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(hspi, Raw_Data, 6, HAL_MAX_DELAY);	//tek CS dongusunde 6 byte okunur, 6 ayri transaction yerine
	LIS3DSH_CS_HIGH;

	Accelerometer_Data[0] = (int16_t) ((Raw_Data[1] << 8) | Raw_Data[0]);	//X
	Accelerometer_Data[1] = (int16_t) ((Raw_Data[3] << 8) | Raw_Data[2]);	//Y
	Accelerometer_Data[2] = (int16_t) ((Raw_Data[5] << 8) | Raw_Data[4]);	//Z
}

