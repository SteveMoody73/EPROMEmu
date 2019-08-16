#include "stm32f0xx_system.h"
#include "timers.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"
#include "shiftregister.h"
#include "commands.h"

#include <stdlib.h>

const char *firmware_version = "v0.1";

char line_buf[MAX_BUFFER_LENGTH];

int main(void)
{
	// Reset of all peripherals, Initialises the Flash interface and the Systick.
	HAL_Init();

	// Configure the system clock
	SystemClock_Config();

	// Initialise all configured peripherals
	initialise_gpio();
	init_uart();
	init_shift_registers();

	while (1)
	{
		if (get_line(line_buf, sizeof(line_buf)))
		{
			decode_command(line_buf);
		}
	}
}
