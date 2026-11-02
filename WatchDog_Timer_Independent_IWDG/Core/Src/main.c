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
IWDG_HandleTypeDef hiwdg;

/* USER CODE BEGIN PV */
uint32_t flash_address = 0x0800FC00; // STM32F030R8 son sayfa adresi
uint16_t flash_data = 0x0045;        // Yazmak istediğiniz 16-bitlik veri
uint32_t read_data = 0;              // Sonucu doğrulamak için 32-bit okuma değişkeni
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_IWDG_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
  * @brief  HAL typedef yapılarını kullanarak kalan alanları sıfırlar ve 16-bit veriyi yazar.
  * @param  address: Yazılacak Flash adresi
  * @param  data: Yazılacak 16-bitlik veri
  * @retval HAL_StatusTypeDef: İşlem durumu sonucu
  */
HAL_StatusTypeDef Write_Flash_16Bit_Typedef(uint32_t address, uint16_t data) {
    HAL_StatusTypeDef status;
    FLASH_EraseInitTypeDef eraseInitStruct;
    uint32_t pageError = 0;

    // 1. Lock and Unlock: FLASH arayüzünün kilidini aç
    HAL_FLASH_Unlock();

    // 2. Monitor/Clear Flags: Eski işlem ve hata bayraklarını temizle
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR);

    // Silme öncesi köpeği besle
    HAL_IWDG_Refresh(&hiwdg);//IWDG devredeyken Flash silme ve yazma işlemleri sırasında mikrodenetleyicinin reset yememesi için Write_Flash_16Bit_Raw fonksiyonunun içine bekçi köpeğini besleme satırını (HAL_IWDG_Refresh) da eklemek kritik bir önem taşır.

    // 3. Erase Function: Typedef struct yapısını doldur ve sayfayı sil
    eraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    eraseInitStruct.PageAddress = address;
    eraseInitStruct.NbPages     = 1; // Sadece ilgili 1 sayfayı sil

    status = HAL_FLASHEx_Erase(&eraseInitStruct, &pageError);
    if (status != HAL_OK) {
        HAL_FLASH_Lock(); // Silme başarısızsa kilitle ve çık
        return status;
    }

    // 4. Program Functions:
    // İlk adım: Sizin gönderdiğiniz 16-bitlik veriyi yaz (0x0045)
    HAL_IWDG_Refresh(&hiwdg);//IWDG devredeyken Flash silme ve yazma işlemleri sırasında mikrodenetleyicinin reset yememesi için Write_Flash_16Bit_Raw fonksiyonunun içine bekçi köpeğini besleme satırını (HAL_IWDG_Refresh) da eklemek kritik bir önem taşır.
    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address, data);
    if (status != HAL_OK) {
        HAL_FLASH_Lock();
        return status;
    }

    // İkinci adım: Üst byte'ların 0xFFFF kalmasını önlemek için ardışık adrese 0x0000 yaz
    HAL_IWDG_Refresh(&hiwdg);//IWDG devredeyken Flash silme ve yazma işlemleri sırasında mikrodenetleyicinin reset yememesi için Write_Flash_16Bit_Raw fonksiyonunun içine bekçi köpeğini besleme satırını (HAL_IWDG_Refresh) da eklemek kritik bir önem taşır.
    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address + 2, 0x0000);
    if (status != HAL_OK) {
        HAL_FLASH_Lock();
        return status;
    }

    // 5. Lock and Unlock: İşlem bitince FLASH arayüzünü tekrar kilitle
    HAL_FLASH_Lock();

    return HAL_OK;
}

/**
  * @brief  Flash adresinden tam 32-bit (4 byte) okuma yapar.
  * @param  address: Okunacak Flash adresi
  * @retval 32-bitlik tam veri
  */
uint32_t Read_Flash_32Bit_Typedef(uint32_t address) {
    return *(volatile uint32_t*)address;
}
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
	MX_IWDG_Init();
	/* USER CODE BEGIN 2 */
	HAL_IWDG_Init(&hiwdg);

		// Typedef tabanlı fonksiyonu çağırarak yazma ve okuma işlemlerini doğrula
		if (Write_Flash_16Bit_Typedef(flash_address, flash_data) == HAL_OK) {
			read_data = Read_Flash_32Bit_Typedef(flash_address); // Sonuç: 0x00000045
		}
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		HAL_IWDG_Refresh(&hiwdg);//IWDG devredeyken Flash silme ve yazma işlemleri sırasında mikrodenetleyicinin reset yememesi için Write_Flash_16Bit_Raw fonksiyonunun içine bekçi köpeğini besleme satırını (HAL_IWDG_Refresh) da eklemek kritik bir önem taşır.

		HAL_GPIO_WritePin(User_LED_Green_GPIO_Port, User_LED_Green_Pin,	GPIO_PIN_SET);
		HAL_Delay(1000);
		HAL_GPIO_WritePin(User_LED_Green_GPIO_Port, User_LED_Green_Pin,	GPIO_PIN_RESET);//led sondukten 1 saniye sonra iwdg sistemi yeniden baslattigi icin led 1 saniye yanip 1 saniye soner
		HAL_Delay(3000);
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

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI
			| RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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
}

/**
 * @brief IWDG Initialization Function
 * @param None
 * @retval None
 */
static void MX_IWDG_Init(void) {

	/* USER CODE BEGIN IWDG_Init 0 */

	/* USER CODE END IWDG_Init 0 */

	/* USER CODE BEGIN IWDG_Init 1 */

	/* USER CODE END IWDG_Init 1 */
	hiwdg.Instance = IWDG;
	hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
	hiwdg.Init.Window = 4095;
	hiwdg.Init.Reload = 1249;
	if (HAL_IWDG_Init(&hiwdg) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN IWDG_Init 2 */

	/* USER CODE END IWDG_Init 2 */

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

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(User_LED_Green_GPIO_Port, User_LED_Green_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin : User_LED_Green_Pin */
	GPIO_InitStruct.Pin = User_LED_Green_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(User_LED_Green_GPIO_Port, &GPIO_InitStruct);

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
