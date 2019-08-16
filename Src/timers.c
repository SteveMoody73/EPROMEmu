#include "timers.h"

uint32_t g_time = 0;

void TIM6_IRQHandler(void)
{
	g_time++;

	HAL_TIM_IRQHandler(&htim6);
}
