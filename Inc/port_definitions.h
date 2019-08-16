#ifndef PORT_DEFINITIONS_H
#define PORT_DEFINITIONS_H

#include "stm32f0xx_hal.h"

#define BOARD_RESET_Pin GPIO_PIN_13
#define BOARD_RESET_GPIO_Port GPIOC
#define BOARD_POWER_Pin GPIO_PIN_14
#define BOARD_POWER_GPIO_Port GPIOC
#define BOARD_RUN_Pin GPIO_PIN_15
#define BOARD_RUN_GPIO_Port GPIOC

#define D0_Pin GPIO_PIN_0
#define D0_GPIO_Port GPIOA
#define D1_Pin GPIO_PIN_1
#define D1_GPIO_Port GPIOA
#define D2_Pin GPIO_PIN_2
#define D2_GPIO_Port GPIOA
#define D3_Pin GPIO_PIN_3
#define D3_GPIO_Port GPIOA
#define D4_Pin GPIO_PIN_4
#define D4_GPIO_Port GPIOA
#define D5_Pin GPIO_PIN_5
#define D5_GPIO_Port GPIOA
#define D6_Pin GPIO_PIN_6
#define D6_GPIO_Port GPIOA
#define D7_Pin GPIO_PIN_7
#define D7_GPIO_Port GPIOA

#define TARGET_VOLTAGE_Pin GPIO_PIN_0
#define TARGET_VOLTAGE_GPIO_Port GPIOB

#define ROM_ACTIVE_Pin GPIO_PIN_0
#define ROM_ACTIVE_GPIO_Port GPIOC
#define ROM_WE_Pin GPIO_PIN_11
#define ROM_WE_GPIO_Port GPIOB
#define ROM_OE_Pin GPIO_PIN_12
#define ROM_OE_GPIO_Port GPIOB
#define ROM_CE_Pin GPIO_PIN_13
#define ROM_CE_GPIO_Port GPIOB

#define DATA_ROM_CE_Pin GPIO_PIN_14
#define DATA_ROM_CE_GPIO_Port GPIOB
#define DATA_DIR_Pin GPIO_PIN_15
#define DATA_DIR_GPIO_Port GPIOB

#define SR_DATAIN_Pin GPIO_PIN_4
#define SR_DATAIN_GPIO_Port GPIOC
#define SR_OE_Pin GPIO_PIN_6
#define SR_OE_GPIO_Port GPIOC
#define SR_RST_Pin GPIO_PIN_7
#define SR_RST_GPIO_Port GPIOC
#define SR_STRB_Pin GPIO_PIN_8
#define SR_STRB_GPIO_Port GPIOC
#define SR_DATA_Pin GPIO_PIN_9
#define SR_DATA_GPIO_Port GPIOC
#define SR_CLK_Pin GPIO_PIN_8
#define SR_CLK_GPIO_Port GPIOA

#endif
