/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"	//ssd1306 kutuphanesinin gereken baslik dosyasi eklendi
#include "ssd1306_tests.h"	//ssd1306 kutuphanesinin test programinin baslik dosyasi eklendi
#include "adxl345.h"	//adxl345 kutuphanenin header dosyasi eklendi
#include "stdio.h"		//sprintf fonksiyonunu kullanabilmek icin
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

/* USER CODE BEGIN PV */
//int Device_Address = 0;	//bulunan adresi atamak icin degisken yazildi //init yazildiktan sonra ihtiyac kalmadigi icin yoruma cekilir.
ADXL345_INIT_STATUS Result;	//sonuclari kayit etmek icin olusturuldu
int16_t X_Value=0, Y_Value=0, Z_Value=0 ;	//x,y,z eksenlerindeki ivmelenme degerlerini kayit etmek icin olusturuldu
float G_X_Value=0, G_Y_Value=0, G_Z_Value=0 ;	//x,y,z eksenlerindeki ivmelenme degerlerini G cinsinden kayit etmek icin olusturuldu
char buffer[16];	//verileri yazdirmak icin karakter dizisi tanimlandi
char header[]="3 eksende ivmeler";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_I2C2_Init();
	/* USER CODE BEGIN 2 */
	ssd1306_Init();		//ssd1306 kutuphanesi baslatildi
	ssd1306_Fill(Black);	//oled ekran siyah yapildi
	Result = ADXL345_Init(&hi2c2);	//sensorde bir hata varsa INIT_FAIL doner ve kod calismayi durdurur. INIT_SUCCESS donerse kod calisiyordur.
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		/*
		ssd1306_SetCursor(10, 6);//cursor u (10,6) konumuna goturmek icin yazildi
		ssd1306_WriteString(name, Font_7x10, White);//name dizisindeki karakterler 7x10 font buyuklugunda yazildi
		ssd1306_SetCursor(10, 16);//cursor u (10,16) konumuna goturmek icin yazildi
		ssd1306_WriteString(name, Font_7x10, White);//name dizisindeki karakterler 7x10 font buyuklugunda yazildi
		ssd1306_SetCursor(10, 35);//cursor u (10,35) konumuna goturmek icin yazildi
		ssd1306_WriteString(name, Font_7x10, White);//name dizisindeki karakterler 7x10 font buyuklugunda yazildi
		ssd1306_SetCursor(10, 51);//cursor u (10,51) konumuna goturmek icin yazildi
		ssd1306_WriteString(name, Font_7x10, White);//name dizisindeki karakterler 7x10 font buyuklugunda yazildi
		ssd1306_UpdateScreen();		//ekran tazelendi
		*/
		X_Value=ADXL345_Get_Axis_Value(&hi2c2, X);	//x eksenindeki verileri al
		Y_Value=ADXL345_Get_Axis_Value(&hi2c2, Y);	//y eksenindeki verileri al
		Z_Value=ADXL345_Get_Axis_Value(&hi2c2, Z);	//z eksenindeki verileri al

		G_X_Value=ADXL345_Get_G_Value(&hi2c2, X, SCALE_FACTOR_4G);	//x eksenindeki verileri alip G cinsine cevir
		G_Y_Value=ADXL345_Get_G_Value(&hi2c2, Y, SCALE_FACTOR_4G);	//y eksenindeki verileri alip G cinsine cevir
		G_Z_Value=ADXL345_Get_G_Value(&hi2c2, Z, SCALE_FACTOR_4G);	//z eksenindeki verileri alip G cinsine cevir



		HAL_GPIO_WritePin(User_LED_GPIO_Port, User_LED_Pin, GPIO_PIN_SET);	//son olarak kullanici LED ini yak

		//ssd1306_SetCursor(3, 3);
		ssd1306_SetCursor(3, 3);//cursor u (3,3) konumuna goturmek icin yazildi
		ssd1306_WriteString(header, Font_7x10, White);//header dizisindeki karakterler 7x10 font buyuklugunda yazildi

		sprintf(buffer, "X:%d",X_Value);	//buffer a X_Value degerleri yazildi
		ssd1306_SetCursor(6, 19);	//cursor u (6,19) konumuna goturmek icin yazildi
		ssd1306_WriteString(buffer, Font_7x10, White);	//buffer daki X_Value degeri ekrana yazildi

		sprintf(buffer, "X:%f",G_X_Value);//buffer a G_X_Value degerleri yazildi
		ssd1306_SetCursor(56, 19);//cursor u (56,19) konumuna goturmek icin yazildi
		ssd1306_WriteString(buffer, Font_7x10, White);//buffer daki G_X_Value degeri ekrana yazildi

		sprintf(buffer, "Y:%d",Y_Value);//buffer a Y_Value degerleri yazildi
		ssd1306_SetCursor(6, 35);//cursor u (6,35) konumuna goturmek icin yazildi
		ssd1306_WriteString(buffer, Font_7x10, White);//buffer daki Y_Value degeri ekrana yazildi

		sprintf(buffer, "Y:%f",G_Y_Value);//buffer a G_Y_Value degerleri yazildi
		ssd1306_SetCursor(56, 35);//cursor u (56,35) konumuna goturmek icin yazildi
		ssd1306_WriteString(buffer, Font_7x10, White);//buffer daki G_Y_Value degeri ekrana yazildi

		sprintf(buffer, "Z:%d",Z_Value);//buffer a Z_Value degerleri yazildi
		ssd1306_SetCursor(6, 51);//cursor u (6,51) konumuna goturmek icin yazildi
		ssd1306_WriteString(buffer, Font_7x10, White);//buffer daki Z_Value degeri ekrana yazildi

		sprintf(buffer, "Z:%f",G_Z_Value);//buffer a G_Z_Value degerleri yazildi
		ssd1306_SetCursor(56, 51);//cursor u (56,51) konumuna goturmek icin yazildi
		ssd1306_WriteString(buffer, Font_7x10, White);//buffer daki G_Z_Value degeri ekrana yazildi

		ssd1306_UpdateScreen();		//!!!!!!! ekran tazelendi, bunu yazmazsan ekranda degerler gorunmez !!!!!
		HAL_Delay(500);	//ekranda daha yavas verilerin gorunmesi icin yazildi
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
	RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
	PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x00201D2B;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE)
			!= HAL_OK) {
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C2_Init(void) {

	/* USER CODE BEGIN I2C2_Init 0 */

	/* USER CODE END I2C2_Init 0 */

	/* USER CODE BEGIN I2C2_Init 1 */

	/* USER CODE END I2C2_Init 1 */
	hi2c2.Instance = I2C2;
	hi2c2.Init.Timing = 0x10805D88;
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c2) != HAL_OK) {
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE)
			!= HAL_OK) {
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C2_Init 2 */

	/* USER CODE END I2C2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */

	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(User_LED_GPIO_Port, User_LED_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : User_LED_Pin */
	GPIO_InitStruct.Pin = User_LED_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(User_LED_GPIO_Port, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */

	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
