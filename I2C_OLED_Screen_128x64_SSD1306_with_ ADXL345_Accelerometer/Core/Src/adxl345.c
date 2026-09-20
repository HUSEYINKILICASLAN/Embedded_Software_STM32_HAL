/*
 * adxl345.c
 *
 *  Created on: 16 Eyl 2026
 *      Author: hkaslan84_2
 */

#include "adxl345.h"	//header dosyasi ilave edildi

//extern I2C_HandleTypeDef hi2c1;	//HAL_I2C_IsDeviceReady fonksiyonu handletypedef aldigi icin bunu yazdim. handletypedef ler c dosyasina yapistirilir. extern keyword u ise ayni dosyanin 2 yerde (main.c dosyasinda da var) kullanilamaz olmasi nedeniyle yazdim. extern turetilmis demektir. //daha sonra bunu kaldirdim cunku bu kod sadece i2c1 icin calisiyor, bunun yerine herhangi bir i2c ile kullanilabilmesi icin gereken kodu yazdim.

int ADXL345_Scan_Device_ID(I2C_HandleTypeDef *hi2cx) {//device bagli mi, adresini alabiliyor muyuz bulmak icin yazildi
	for (uint8_t address = 0; address < 255; ++address) {
		if (HAL_I2C_IsDeviceReady(hi2cx, address, 1, TIMEOUT) == HAL_OK) {
			return address;	//i2c modulun adresini bulduysa address degiskenine atayacak
		}
	}
	return -1;	//modul bagli degil veya adresi bulamazsa -1 dönecek
}

static ADXL345_READ_STATUS ADXL345_Read_Register_Data(I2C_HandleTypeDef *hi2cx, uint16_t Register_Address, uint16_t Size_of_Data, uint8_t *Data_Buffer) {//register datasini okumak icin bu fonksiyon yazildi, okuma basarili ise READ_SUCCESS donecek, basarisiz olursa READ_FAIL donecek, static yaptım cunku bu fonksiyonlar baska dosyada cagirilmayacak
	if (HAL_I2C_Mem_Read(hi2cx, ADXL345_DEVICE_ADDRESS, Register_Address, 1, Data_Buffer, Size_of_Data, TIMEOUT) == HAL_OK) {
		return READ_SUCCESS;
	}
	return READ_FAIL;
}

static ADXL345_WRITE_STATUS ADXL345_Write_Register_Data(I2C_HandleTypeDef *hi2cx, uint16_t Register_Address,	uint16_t Value) {//register a veri yazmak icin bu fonksiyon yazildi, okuma basarili ise WRITE_SUCCESS donecek, basarisiz olursa WRITE_FAIL donecek, static yaptim cunku bu fonksiyon baska dosyada cagirilmayacak
	uint8_t data[2] = { 0 };	//fonksiyondaki Value degiskeni 16 bit oldugu icin verinin 2 ye bolunmesi gereklidir. bunun icin 8 bitlik data isimli 2 boyutlu dizi olusturulup fonksiyona verilmistir.
	data[0] = Register_Address;
	data[1] = Value;

	if (HAL_I2C_Master_Transmit(hi2cx, ADXL345_DEVICE_ADDRESS, data, sizeof(data), TIMEOUT) == HAL_OK) {
		return WRITE_SUCCESS;
	}
	return WRITE_FAIL;
}

ADXL345_INIT_STATUS ADXL345_Init(I2C_HandleTypeDef *hi2cx) {//register data okuma baslatma fonksiyonu cagirildi. bu fonksiyonun sonucu INIT_SUCCESS ise dogru calisiyordur, INIT_FAIL ise hata vardir ve kod calismaz
	uint8_t Data_Buffer = 0;
	ADXL345_Read_Register_Data(hi2cx, DEVID, 1, &Data_Buffer);

	if (Data_Buffer != 0xE5) {
		return INIT_FAIL;
	}

	uint8_t tempReg = 0;

	Power_Control_Register_t Power_Control = { 0 };	//Power_Control isimli struct degiskeni tanimlandi
	Power_Control.Wakeup = WAKE_UP_8HZ;	//struct degiskeninin elemanlarina (POWER_CTL register inin bitlerine deger yazilmasi icin) degerler ataniyor
	Power_Control.Sleep = 0x00;
	Power_Control.Measure = 0x01;
	Power_Control.AUTO_SLEEP = 0x00;
	Power_Control.Link = 0x00;
	Power_Control.Reserved = 0x00;

	tempReg = *((uint8_t*) &Power_Control);	//tempReg degiskenine tip donusumu yapiliyor cunku power_control_register_t tipindeki struct birden fazla uint8 tipinde veri iceriyor, tempReg degiskeni ise bir tane uint8 tipinde veri iceriyor.

	ADXL345_Write_Register_Data(hi2cx, POWER_CTL, tempReg);	//POWER_CTL register ina degerler gonderildi.

	Data_Format_Register_t Data_Format_Control={0};	//Data_Format_Control isimli struct degiskeni tanimlandi
	Data_Format_Control.Range= RANGE_4G;	//struct degiskeninin elemanlarina (DATA_FORMAT register inin bitlerine deger yazilmasi icin) degerler ataniyor
	Data_Format_Control.Justify=0x00;
	Data_Format_Control.FULL_RES=0x00;
	Data_Format_Control.Reserved=0x00;
	Data_Format_Control.INT_INVERT=0x00;
	Data_Format_Control.SPI=0x00;
	Data_Format_Control.SELF_TEST=0x00;

	tempReg = *((uint8_t*) &Data_Format_Control);	//tempReg degiskenine tip donusumu yapiliyor

	ADXL345_Write_Register_Data(hi2cx, DATA_FORMAT, tempReg);	//DATA_FORMAT register ina degerler gonderildi.


	BW_Rate_Register_t BW_Rate_Control={0};	//BW_Rate_Control isimli struct degiskeni tanimlandi
	BW_Rate_Control.Rate=BW_RATE_800;
	BW_Rate_Control.LOW_POWER=0x00;
	BW_Rate_Control.Reserved=0x00;

	tempReg = *((uint8_t*) &BW_Rate_Control);	//tempReg degiskenine tip donusumu yapiliyor

	ADXL345_Write_Register_Data(hi2cx, BW_RATE, tempReg);	//BW_RATE register ina degerler gonderildi.


	return INIT_SUCCESS;
}

int16_t ADXL345_Get_Axis_Value(I2C_HandleTypeDef *hi2cx, uint8_t Axis){	//eksene gore degerleri almak icin yazilan fonksiyon, int16_t tipinde deger dondurecegini biliyorum.
	uint8_t Data[2]={0};	//2 boyutlu dizi tanimlandi cunku ADXL345_Read_Register_Data fonksiyonu 8 bitlik degerler okur
	int16_t Output_Data=0;	//okunacak degerleri kayit edip cikis vermek icin olusturuldu

	ADXL345_Read_Register_Data(hi2cx, Axis, 2, Data);	//okuma islemi yapildi.

	Output_Data=((Data[1]<<8)|Data[0]);		//2 tane 8 bitlik veriyi birleştirmek icin, MSB olan bayt (data[1]) 8 bit sola kaydirilip LSB olan bayt (data[0]) ile OR lanır)

	return Output_Data;
}


float ADXL345_Get_G_Value(I2C_HandleTypeDef *hi2cx, uint8_t Axis, float Scale_Factor){
	int16_t Output_Data=0;	//okunacak eksen degerlerini kayit etmek icin olusturuldu
	float G_Data=0;			//G cinsine cevrilen verileri kayit etmek icin olusturuldu

	Output_Data=ADXL345_Get_Axis_Value(hi2cx, Axis);	//verilen eksenden okuma islemi yapildi

	G_Data=(float)(Output_Data * Scale_Factor);	//bulunan eksen degerini G cinsine cevirmek icin gereken islem yapildi

	return G_Data;

}
