#ifndef _SHIFTREGISTER_H_
#define _SHIFTREGISTER_H_

#include "stm32f0xx_system.h"

//Global Variables
extern uint32_t g_output_shift_data;
extern uint32_t g_input_shift_data;


void init_shift_registers(void);
uint32_t update_shift_registers();

void enableShiftRegisters(void);
void disableShiftRegisters(void);


#endif
