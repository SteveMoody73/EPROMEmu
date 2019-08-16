#include "stm32f0xx_system.h"
#include "timers.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"

int main(void)
{
	// Reset of all peripherals, Initializes the Flash interface and the Systick.
	HAL_Init();

	// Configure the system clock
	SystemClock_Config();

	// Initialize all configured peripherals */
	initialise_gpio();

	while (1)
	{
	}
}
