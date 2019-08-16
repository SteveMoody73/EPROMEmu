#include "usart.h"

void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart1);
}
