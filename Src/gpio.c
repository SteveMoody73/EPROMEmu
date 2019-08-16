#include "gpio.h"

void initialise_gpio(void)
{
	MX_GPIO_Init();
	MX_ADC_Init();
	MX_TIM6_Init();
}

void set_target_reset_state(uint8_t state)
{
	HAL_GPIO_WritePin(BOARD_RESET_GPIO_Port, BOARD_RESET_Pin, state);
}

uint8_t read_target_vcc(void)
{
	(uint8_t)HAL_GPIO_ReadPin(BOARD_POWER_GPIO_Port, BOARD_POWER_Pin);
}
