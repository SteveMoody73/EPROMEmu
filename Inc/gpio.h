#ifndef GPIO_H
#define GPIO_H

#include "stm32f0xx_system.h"

void initialise_gpio(void);

void set_target_reset_state(uint8_t state);
uint8_t read_target_vcc(void);

#endif
