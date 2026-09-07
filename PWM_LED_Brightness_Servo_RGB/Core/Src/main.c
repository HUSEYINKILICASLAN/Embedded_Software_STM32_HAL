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
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */
uint8_t rgb_state = 0; // Butona basılma durumunu tutar (0: Kapalı, 1: Kırmızı, 2: Yeşil, 3: Mavi)
uint8_t button_last_state = 1; // Butonun önceki durumu (Pull-up buton için varsayılan 1)
int16_t servo_angle = 0;       // Servo açısı (0 - 180 arası)
int8_t servo_direction = 1;    // Servo yönü (+1 ileri, -1 geri)
uint32_t led_last_tick = 0;      // PA8 LED parlaklık zamanlayıcısı için
uint8_t led_duty_index = 0;      // 0,25,50,75,100 sırasını takip eder
const uint8_t led_duty_steps[5] = { 0, 25, 50, 75, 100 };	//sabit duty değerlerini saklamak için oluşturuldu
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Set_PWM_Duty_Cycle(uint16_t duty) {	//bu fonksiyon, LED parlaklığını ayarlamak için duty cycle değiştirmek için yazıldı
	if (duty > 100) {
		duty = 100;   // 100'ü aşan değerleri sınırla
	}
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, duty * 10);	//Bu makro, STM32 mikrodenetleyicilerinde üretilen PWM sinyalinin Duty Cycle (Görev Döngüsü / Doluluk Oranı) değerini anlık olarak değiştirmek için kullanılan donanımsal bir makro komuttur. Bu komut, doğrudan ilgili Timer'ın CCR (Capture/Compare Register) adındaki donanım kaydına (hafıza hücresine) belirlediğiniz "Pulse" (adım) değerini yazar. Bu komut arka planda karmaşık C kodları çalıştırmaz. Doğrudan işlemcinin register adresine eriştiği için sadece 1 veya 2 saat çevriminde (birkaç nanosaniyede) tamamlanır. Bu sayede motor kontrolü veya hassas LED parlaklık ayarı yaparken işlemciyi hiç yormaz ve gecikmeye neden olmaz.
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
	MX_TIM1_Init();
	MX_TIM3_Init();
	/* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); //timer1 in kanal1 den PWM baslatmasi icin yazildi, LED parlaklığı ayarlanacak
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); //timer1 in kanal2 den PWM baslatmasi icin yazildi, RGB - Kırmızı
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); //timer1 in kanal3 den PWM baslatmasi icin yazildi, RGB - Yeşil
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4); //timer1 in kanal4 den PWM baslatmasi icin yazildi, RGB - Mavi

	// Timer 3 PWM Kanalını Başlat (Servo için)
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); //timer3 ün kanal1 den PWM baslatmasi icin yazildi, servo kontrol MG995

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* --- 1. BUTON VE RGB LED KONTROLÜ --- */
		// Butonun basılıp basılmadığını kontrol et (Mavi buton basılınca 0 verir)
		uint8_t button_current_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);

		if (button_current_state == 0 && button_last_state == 1) // Yükselen kenar/Düşen kenar kontrolü (Debounce koruması ile)
				{
			HAL_Delay(50); // Buton arkını (titreşimi) önlemek için kısa gecikme

			rgb_state++;
			if (rgb_state > 3)
				rgb_state = 1; // 1: Kırmızı, 2: Yeşil, 3: Mavi döngüsü

			// Önce tüm RGB renkleri kapat (ARR=999 için Pulse=0 tamamen kapatır)
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);

			// Aktif olan rengi %100 parlaklıkta yak (Pulse=999)
			if (rgb_state == 1)
				__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 999); // Kırmızı Açık
			else if (rgb_state == 2)
				__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 999); // Yeşil Açık
			else if (rgb_state == 3)
				__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 999); // Mavi Açık
		}
		button_last_state = button_current_state;

		/* --- 2. SERVO MOTOR 0-180 DERECE HAREKETİ --- */
		// MG995 için: 0 derece = 1ms pulse (Pulse değeri 1000)
		// 180 derece = 2ms pulse (Pulse değeri 2000)
		// Açı değerini pulse değerine doğrusal (map) olarak dönüştürüyoruz:
		uint16_t servo_pulse = 1000 + ((servo_angle * 1000) / 180);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, servo_pulse);

		// Açıyı sürekli değiştirerek ileri-geri hareket ettir
		servo_angle += servo_direction;
		if (servo_angle >= 180) {
			servo_angle = 180;
			servo_direction = -1; // Ters yöne dön
		} else if (servo_angle <= 0) {
			servo_angle = 0;
			servo_direction = 1;  // İleri yöne dön
		}

		/* --- 3. PA8 LED PARLAKLIK KONTROLÜ (1 SANİYEDE BİR ARTAN DUTY CYCLE) --- */
		if (HAL_GetTick() - led_last_tick >= 1000) {
			led_last_tick = HAL_GetTick();

			Set_PWM_Duty_Cycle(led_duty_steps[led_duty_index]);

			led_duty_index++;
			if (led_duty_index >= 5) {
				led_duty_index = 0;   // 0'a dön, döngü baştan başlasın
			}
		}

		HAL_Delay(15); // Servonun mekanik olarak yetişebilmesi ve hareketin yumuşak olması için küçük bir bekleme
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		/*
		 Set_PWM_Duty_Cycle(0);
		 HAL_Delay(1000);

		 Set_PWM_Duty_Cycle(25);
		 HAL_Delay(1000);

		 Set_PWM_Duty_Cycle(50);
		 HAL_Delay(1000);

		 Set_PWM_Duty_Cycle(75);
		 HAL_Delay(1000);

		 Set_PWM_Duty_Cycle(100);
		 HAL_Delay(1000);
		 */
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
}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void) {

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 48 - 1;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 1000 - 1;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4)
			!= HAL_OK) {
		Error_Handler();
	}
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */
	HAL_TIM_MspPostInit(&htim1);

}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void) {

	/* USER CODE BEGIN TIM3_Init 0 */

	/* USER CODE END TIM3_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };

	/* USER CODE BEGIN TIM3_Init 1 */

	/* USER CODE END TIM3_Init 1 */
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 48 - 1;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 20000 - 1;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim3) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM3_Init 2 */

	/* USER CODE END TIM3_Init 2 */
	HAL_TIM_MspPostInit(&htim3);

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
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin : User_Button_Pin */
	GPIO_InitStruct.Pin = User_Button_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(User_Button_GPIO_Port, &GPIO_InitStruct);

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
