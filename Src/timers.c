#include "timers.h"

void TIM6_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim6);
}
