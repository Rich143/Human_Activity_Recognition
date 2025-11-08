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

BaseType_t printCSVDataCommand(char *pcWriteBuffer,
                           size_t xWriteBufferLen,
                           const char *pcCommandString );

static const CLI_Command_Definition_t printCSVDataCommandDefinition = {
        .pcCommand = "printCSV",
        .pcHelpString = "printCSV <enabled = 0|1>: Enable printing of CSV data\r\n",
        .pxCommandInterpreter = printCSVDataCommand,
        .cExpectedNumberOfParameters = 1,
};

BaseType_t logEnableCommand(char *pcWriteBuffer,
                           size_t xWriteBufferLen,
                           const char *pcCommandString );

static const CLI_Command_Definition_t logEnableCommandDefinition = {
        .pcCommand = "logEnable",
        .pcHelpString = "logEnable <enabled = 0|1>: Enable/Disable logging\r\n",
        .pxCommandInterpreter = logEnableCommand,
        .cExpectedNumberOfParameters = 1,
};

BaseType_t exitCLICommand(char *pcWriteBuffer,
                           size_t xWriteBufferLen,
                           const char *pcCommandString );

static const CLI_Command_Definition_t exitCLICommandDefinition = {
        .pcCommand = "exit",
        .pcHelpString = "exit: Exit the CLI\r\n",
        .pxCommandInterpreter = exitCLICommand,
        .cExpectedNumberOfParameters = 0,
};

BaseType_t flashDumpCommand(char *pcWriteBuffer,
                           size_t xWriteBufferLen,
                           const char *pcCommandString );

static const CLI_Command_Definition_t flashDumpCommandDefinition = {
        .pcCommand = "flashDump",
        .pcHelpString =
                "flashDump <address> <size>: Dump the (external nor) flash at the given address, of the given size (in 256 bytes blocks)\r\n",
        .pxCommandInterpreter = flashDumpCommand,
        .cExpectedNumberOfParameters = 2,
};

BaseType_t flashClearCommand(char *pcWriteBuffer,
                           size_t xWriteBufferLen,
                           const char *pcCommandString );

static const CLI_Command_Definition_t flashClearCommandDefinition = {
        .pcCommand = "flashClearSector",
        .pcHelpString =
                "flashClearSector <sector> <num sectors>: Clear the (external nor) flash at the given sector, of the given number of sectors\r\n",
        .pxCommandInterpreter = flashClearCommand,
        .cExpectedNumberOfParameters = 2,
};

BaseType_t flashClearChipCommand(char *pcWriteBuffer,
                           size_t xWriteBufferLen,
                           const char *pcCommandString );

static const CLI_Command_Definition_t flashClearChipCommandDefinition = {
        .pcCommand = "flashClearEntireChip",
        .pcHelpString =
                "flashClearEntireChip: Clear the (external nor) flash (entire chip).\r\n",
        .pxCommandInterpreter = flashClearChipCommand,
        .cExpectedNumberOfParameters = 0,
};

BaseType_t flashClearedCommand(char *pcWriteBuffer,
                           size_t xWriteBufferLen,
                           const char *pcCommandString );

static const CLI_Command_Definition_t flashClearedCommandDefinition = {
        .pcCommand = "isFlashCleared",
        .pcHelpString =
                "isFlashCleared: Check if the (external nor) flash has been cleared\r\n",
        .pxCommandInterpreter = flashClearedCommand,
        .cExpectedNumberOfParameters = 0,
};

BaseType_t getIMUScaleCommand(char *pcWriteBuffer,
                           size_t xWriteBufferLen,
                           const char *pcCommandString );

static const CLI_Command_Definition_t getIMUScaleCommandDefinition = {
        .pcCommand = "imuScale",
        .pcHelpString =
                "imuScale: Get the current IMU scale (in g)\r\n",
        .pxCommandInterpreter = getIMUScaleCommand,
        .cExpectedNumberOfParameters = 0,
};

BaseType_t enablePredictionsCommand(char *pcWriteBuffer,
                           size_t xWriteBufferLen,
                           const char *pcCommandString );

static const CLI_Command_Definition_t enablePredictionsCommandDefinition = {
        .pcCommand = "predictionsEnable",
        .pcHelpString =
                "predictionsEnable <0|1>: Enable (1)/Disable (0) predictions from the model. With this on, data collection rate is decreased (since the model running takes some time)\r\n",
        .pxCommandInterpreter = enablePredictionsCommand,
        .cExpectedNumberOfParameters = 1,
};

BaseType_t dumpErrorLogsCommand(char *pcWriteBuffer,
                           size_t xWriteBufferLen,
                           const char *pcCommandString );

static const CLI_Command_Definition_t dumpErrorLogsCommandDefinition = {
        .pcCommand = "dumpErrorLogs",
        .pcHelpString =
                "dumpErrorLogs: Dump error logs (binary format)\r\n",
        .pxCommandInterpreter = dumpErrorLogsCommand,
        .cExpectedNumberOfParameters = 0,
};

BaseType_t printErrorLogsCommand(char *pcWriteBuffer,
                           size_t xWriteBufferLen,
                           const char *pcCommandString );

static const CLI_Command_Definition_t printErrorLogsCommandDefinition = {
        .pcCommand = "printErrorLogs",
        .pcHelpString =
                "printErrorLogs: Print error logs (text)\r\n",
        .pxCommandInterpreter = printErrorLogsCommand,
        .cExpectedNumberOfParameters = 0,
};

BaseType_t writeErrorLogCommand(char *pcWriteBuffer,
                           size_t xWriteBufferLen,
                           const char *pcCommandString );

static const CLI_Command_Definition_t writeErrorLogCommandDefinition = {
        .pcCommand = "writeErrorLog",
        .pcHelpString =
                "writeErrorLog <type>: Write fake error log entry\r\n",
        .pxCommandInterpreter = writeErrorLogCommand,
        .cExpectedNumberOfParameters = 1,
};

BaseType_t errorLogSizeCommand(char *pcWriteBuffer,
                         size_t xWriteBufferLen,
                         const char *pcCommandString );

static const CLI_Command_Definition_t errorLogSizeCommandDefinition = {
        .pcCommand = "errorLogSize",
        .pcHelpString = "errorLogSize: Print the number of error log entries\r\n",
        .pxCommandInterpreter = errorLogSizeCommand,
        .cExpectedNumberOfParameters = 0,
};

BaseType_t errorLogClearCommand(char *pcWriteBuffer,
                         size_t xWriteBufferLen,
                         const char *pcCommandString );

static const CLI_Command_Definition_t errorLogClearCommandDefinition = {
        .pcCommand = "errorLogClear",
        .pcHelpString = "errorLogClear: Clear the error log\r\n",
        .pxCommandInterpreter = errorLogClearCommand,
        .cExpectedNumberOfParameters = 0,
};

#endif /* INC_CLI_COMMANDS_H_ */
