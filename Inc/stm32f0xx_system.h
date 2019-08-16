#ifndef STM32F0xx_SYSTEM_H
#define STM32F0xx_SYSTEM_H

#include "stm32f0xx_hal.h"
#include "port_definitions.h"

extern ADC_HandleTypeDef hadc;
extern TIM_HandleTypeDef htim6;
extern UART_HandleTypeDef huart1;

void NMI_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void SystemClock_Config(void);
void Error_Handler();

void MX_ADC_Init(void);
void MX_GPIO_Init(void);
void MX_TIM6_Init(void);
void MX_USART1_UART_Init(void);

#endif
