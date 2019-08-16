#include "gpio.h"

void initialise_gpio(void)
{
	MX_GPIO_Init();
	MX_ADC_Init();
	MX_USART1_UART_Init();
	MX_TIM6_Init();
}
