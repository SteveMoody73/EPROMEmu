#include "shiftregister.h"

//Global Variables
uint32_t g_output_shift_data = 0;		//!< Data to be written to the shift register outputs
uint32_t g_input_shift_data = 0;		//!< Data read back from the shift register inputs


void set_sr_clock(uint8_t state)
{
	HAL_GPIO_WritePin(SR_CLK_GPIO_Port, SR_CLK_Pin, state);
}

void set_sr_latch(uint8_t state)
{
	HAL_GPIO_WritePin(SR_STRB_GPIO_Port, SR_STRB_Pin, state);
}

void set_sr_data_out(uint8_t state)
{
	HAL_GPIO_WritePin(SR_DATA_GPIO_Port, SR_DATA_Pin, state);
}

void set_sr_reset_state(uint8_t state)
{
	HAL_GPIO_WritePin(SR_RST_GPIO_Port, SR_RST_Pin, state);
}

void set_sr_output_state(uint8_t state)
{
	HAL_GPIO_WritePin(SR_OE_GPIO_Port, SR_OE_Pin, state);
}

uint32_t read_sr_data_in(void)
{
	// Read a single bit from port pin.
	return HAL_GPIO_ReadPin(SR_DATAIN_GPIO_Port, SR_DATAIN_Pin);
}

void init_shift_registers(void)
{
	set_sr_latch(1);
	set_sr_clock(1);
						 
	// Set the default value on the shift registers
	update_shift_registers();
}

// Updated the outputs on the Shift Register
uint32_t update_shift_registers(void)
{
	uint32_t data_out = 0;
	uint32_t data_in = 0;
	uint32_t bit_mask;
	uint8_t i;

	// Shift Data out
	data_out = g_output_shift_data;
	bit_mask = 0x0800;

	for (i = 0; i < 24; i++)
	{
		set_sr_clock(0);

		if (data_out & bit_mask)
			set_sr_data_out(1);
		else
			set_sr_data_out(0);

		bit_mask >>= 1;
		set_sr_clock(1);
	}

	// Once 16 bits have been loaded in, strobe to send the data to U7/U8 parallel outputs.
	set_sr_latch(0);
	set_sr_latch(1);
	

	// Read the data back in and verify that it's correct.
	// Data is clocked on negative edge.
	for (i = 0; i < 24; i++)
	{
		set_sr_clock(0);
		data_in <<= 1;  //Data is shifted in MSB to LSB.
		data_in |= read_sr_data_in();
		set_sr_clock(1);
	}

	set_sr_clock(0);

	// Check if the data read and sent the same
	if ((data_in & 0x00FFFFFF) != g_output_shift_data)
	{
		//setShiftRegError();
	}

	g_input_shift_data = data_in;
	return g_input_shift_data;
}

