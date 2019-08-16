#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "commands.h"
#include "gpio.h"

extern const char *firmware_version;

int16_t cmd_print_help(void);
int16_t cmd_print_version(void);
int16_t cmd_reset_state(char *params);

const command_entry_t g_command_tab[] =
{
	{"??", cmd_print_help, "Displays this help with a list of available commands"},
	{":S", cmd_reset_state, "Sets the reset state of the target board, params 0 or 1"},
	{":V", cmd_print_version, "Displays the version number"}
};

// Number of commands in the Command Table
const uint16_t g_number_commands = sizeof(g_command_tab) / sizeof(command_entry_t);

// This allows commands from RS232 interface to control the board, format given above.
void decode_command(char* commandStr)
{
	char tempCommand[3] = {0};
  	char *paramPtr;

  	// Convert command string to upper case
  	strupr(commandStr);

	// skip whitespace characters
	while (isspace(*commandStr) && (*commandStr != '\0'))
		commandStr++;

	if (*commandStr == '\0')
	{
		// no command found on
    	printf("%d\n", CMD_NOT_KNOWN);
		return;
	}

	// command found search for parameter string
	paramPtr = commandStr + 2;

	// skip whitespace characters
	while (isspace(*paramPtr) && (*paramPtr != '\0'))
		paramPtr++;

	// set parameter pointer to zero if no parameter found on line
	if (*paramPtr == '\0')
		paramPtr = 0;

	// copy 1st 2 characters of command string into tempCommand
	tempCommand[0] = commandStr[0];
	tempCommand[1] = commandStr[1];

	// search for valid command
	for (int cmd = 0; cmd < g_number_commands; cmd++)
	{
		if (strcmp(tempCommand, g_command_tab[cmd].command) == 0)
		{
			// jump to matching command and save error code from command handler
			int tmpErr = (*g_command_tab[cmd].function_ptr)(paramPtr);

			if (tmpErr != CMD_NO_ERROR)
				printf("%d\n", tmpErr);

			return;
		}
	}

	// here if no commands matched
	printf("%d\n", CMD_NOT_KNOWN);
}

int16_t cmd_print_help()
{
	printf("Commands available:\n");

	// Print the command code and the description for each command in the table
	for (uint16_t i = 0; i < g_number_commands; i++)
	{
		if (g_command_tab[i].command != NULL)
			printf("\t%s - %s\n", g_command_tab[i].command, g_command_tab[i].desc);
		else
			printf("%s\n", g_command_tab[i].desc);

	}
	return CMD_NO_ERROR;
}

int16_t cmd_print_version()
{
	printf("%d\nEPROM Emulator firmware version: %s\n", CMD_NO_ERROR, firmware_version);
	return CMD_NO_ERROR;
}

int16_t cmd_reset_state(char *params)
{
	int state;

	int numParams = sscanf(params, "%d", &state);

	if (numParams != 1)
	{
		printf("%d\n", CMD_PARAM_ERROR);
		return CMD_PARAM_ERROR;
	}

	set_target_reset_state((uint8_t)state);

	printf("%d\n", CMD_NO_ERROR);
	return CMD_NO_ERROR;
}
