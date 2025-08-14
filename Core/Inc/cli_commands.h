/*
 * cli_commands.h
 *
 *  Created on: Aug 5, 2025
 *      Author: richardmatthews
 */

#ifndef INC_CLI_COMMANDS_H_
#define INC_CLI_COMMANDS_H_

#include "FreeRTOS_CLI.h"

BaseType_t logClearCommand(char *pcWriteBuffer,
                         size_t xWriteBufferLen,
                         const char *pcCommandString );

static const CLI_Command_Definition_t logClearCommandDefinition = {
        .pcCommand = "logClear",
        .pcHelpString = "logClear: Clear Logs\r\n",
        .pxCommandInterpreter = logClearCommand,
        .cExpectedNumberOfParameters = 0,
};

BaseType_t logDumpCommand(char *pcWriteBuffer,
                         size_t xWriteBufferLen,
                         const char *pcCommandString );

/// TODO! Should this take a param to control including model output in log
/// dump
static const CLI_Command_Definition_t logDumpCommandDefinition = {
        .pcCommand = "logDump",
        .pcHelpString = "logDump: Dump Logs\r\n",
        .pxCommandInterpreter = logDumpCommand,
        .cExpectedNumberOfParameters = 0,
};

BaseType_t logSizeCommand(char *pcWriteBuffer,
                         size_t xWriteBufferLen,
                         const char *pcCommandString );

static const CLI_Command_Definition_t logSizeCommandDefinition = {
        .pcCommand = "logSize",
        .pcHelpString = "logSize: Print the number of log entries (number of rows, where one row is one IMU sample)\r\n",
        .pxCommandInterpreter = logSizeCommand,
        .cExpectedNumberOfParameters = 0,
};

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
