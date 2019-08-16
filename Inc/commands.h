#ifndef DECODECOMMANDS_H
#define DECODECOMMANDS_H

#include "stm32f0xx_system.h"

enum CommsErrors
{
	CMD_NO_ERROR 		=  0,
	CMD_NOT_KNOWN 		= -1,
	CMD_BUSY			= -2,
	CMD_PARAM_ERROR		= -3,
	CMD_RANGE_ERROR 	= -4
};

#define MAX_BUFFER_LENGTH 255

typedef struct
{
	char *command;				// Two character command code
	int16_t (*function_ptr)();  // Pointer to command function
	char *desc;					// Pointer to description string for help
} command_entry_t;

void decode_command(char* commandStr);

#endif

