#include "stm32f0xx_system.h"

ADC_HandleTypeDef hadc;
TIM_HandleTypeDef htim6;
UART_HandleTypeDef huart1;


void NMI_Handler(void)
{
}


void HardFault_Handler(void)
{
	while (1)
	{}
}

void SVC_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void HAL_MspInit(void)
{
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
}


void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	// Initializes the CPU, AHB and APB busses clocks
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.HSI14CalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
	RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;

	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	// Initializes the CPU, AHB and APB busses clocks
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
		Error_Handler();
	}

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;

	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
}

void Error_Handler(void)
{
}

void MX_ADC_Init(void)
{
	ADC_ChannelConfTypeDef sConfig = {0};

	// Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	hadc.Instance = ADC1;
	hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	hadc.Init.Resolution = ADC_RESOLUTION_12B;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
	hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc.Init.LowPowerAutoWait = DISABLE;
	hadc.Init.LowPowerAutoPowerOff = DISABLE;
	hadc.Init.ContinuousConvMode = DISABLE;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc.Init.DMAContinuousRequests = DISABLE;
	hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;

	if (HAL_ADC_Init(&hadc) != HAL_OK)
	{
		Error_Handler();
	}

	// Configure for the selected ADC regular channel to be converted.
	sConfig.Channel = ADC_CHANNEL_8;
	sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;

	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if (adcHandle->Instance==ADC1)
	{
		__HAL_RCC_ADC1_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();

		// ADC GPIO Configuration
		// PB0     ------> ADC_IN8
		GPIO_InitStruct.Pin = TARGET_VOLTAGE_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(TARGET_VOLTAGE_GPIO_Port, &GPIO_InitStruct);
	}
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{
	if (adcHandle->Instance==ADC1)
	{
		__HAL_RCC_ADC1_CLK_DISABLE();

		HAL_GPIO_DeInit(TARGET_VOLTAGE_GPIO_Port, TARGET_VOLTAGE_Pin);
	}
}

void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// GPIO Ports Clock Enable
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	// Configure GPIO pin Output Level
	HAL_GPIO_WritePin(GPIOC, BOARD_RESET_Pin | BOARD_RUN_Pin | SR_OE_Pin | SR_RST_Pin, GPIO_PIN_SET);

	// Configure GPIO pin Output Level
	HAL_GPIO_WritePin(GPIOC, ROM_ACTIVE_Pin | SR_STRB_Pin | SR_DATA_Pin, GPIO_PIN_RESET);

	// Configure GPIO pin Output Level
	HAL_GPIO_WritePin(GPIOA, D0_Pin | D1_Pin | D2_Pin | D3_Pin | D4_Pin | D5_Pin | D6_Pin | D7_Pin | SR_CLK_Pin, GPIO_PIN_RESET);

	// Configure GPIO pin Output Level
	HAL_GPIO_WritePin(GPIOB, ROM_WE_Pin | ROM_OE_Pin | ROM_CE_Pin | DATA_ROM_CE_Pin | DATA_DIR_Pin, GPIO_PIN_RESET);

	// Configure GPIO pins : PCPin PCPin PCPin
	GPIO_InitStruct.Pin = BOARD_RESET_Pin | BOARD_RUN_Pin | ROM_ACTIVE_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	// Configure GPIO pin : PtPin
	GPIO_InitStruct.Pin = BOARD_POWER_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(BOARD_POWER_GPIO_Port, &GPIO_InitStruct);

	// Configure GPIO pins : PAPin PAPin PAPin PAPin PAPin PAPin PAPin PAPin PAPin
	GPIO_InitStruct.Pin = D0_Pin | D1_Pin | D2_Pin | D3_Pin | D4_Pin | D5_Pin | D6_Pin | D7_Pin | SR_CLK_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// Configure GPIO pin : PtPin
	GPIO_InitStruct.Pin = SR_DATAIN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(SR_DATAIN_GPIO_Port, &GPIO_InitStruct);

	// Configure GPIO pins : PBPin PBPin PBPin PBPin PBPin
	GPIO_InitStruct.Pin = ROM_WE_Pin | ROM_OE_Pin | ROM_CE_Pin | DATA_ROM_CE_Pin | DATA_DIR_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// Configure GPIO pins : PCPin PCPin PCPin PCPin
	GPIO_InitStruct.Pin = SR_OE_Pin | SR_RST_Pin | SR_STRB_Pin | SR_DATA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}


void MX_TIM6_Init(void)
{
	htim6.Instance = TIM6;
	htim6.Init.Prescaler = 47999;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period = 100;
	htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

	if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
	{
		Error_Handler();
	}
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

	if (tim_baseHandle->Instance==TIM6)
	{
		__HAL_RCC_TIM6_CLK_ENABLE();
	    // TIM6 interrupt Init
	    HAL_NVIC_SetPriority(TIM6_IRQn, 2, 0);
	    HAL_NVIC_EnableIRQ(TIM6_IRQn);
	}
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

	if (tim_baseHandle->Instance==TIM6)
	{
		__HAL_RCC_TIM6_CLK_DISABLE();
	    // TIM6 interrupt Deinit
	    HAL_NVIC_DisableIRQ(TIM6_IRQn);
	}
}

void MX_USART1_UART_Init(void)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 57600; //115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		Error_Handler();
	}
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if (uartHandle->Instance==USART1)
	{
		__HAL_RCC_USART1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();

		// USART1 GPIO Configuration
		// PA9     ------> USART1_TX
		// PA10     ------> USART1_RX
		GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	    // USART1 interrupt Init
	    HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
	    HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{
	if (uartHandle->Instance==USART1)
	{
		__HAL_RCC_USART1_CLK_DISABLE();

		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);
		HAL_NVIC_DisableIRQ(USART1_IRQn);
	}
}
