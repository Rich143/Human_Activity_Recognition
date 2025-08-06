/*
 * cli_commands.h
 *
 *  Created on: Aug 5, 2025
 *      Author: richardmatthews
 */

#ifndef INC_CLI_COMMANDS_H_
#define INC_CLI_COMMANDS_H_

#include "FreeRTOS_CLI.h"

BaseType_t pingCommand(char *pcWriteBuffer,
                         size_t xWriteBufferLen,
                         const char *pcCommandString );

static const CLI_Command_Definition_t pingCommandDefinition = {
        .pcCommand = "ping",
        .pcHelpString = "ping: Ping the device\r\n",
        .pxCommandInterpreter = pingCommand,
        .cExpectedNumberOfParameters = 0,
};

#endif /* INC_CLI_COMMANDS_H_ */
