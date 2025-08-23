/*
 * cli_commands.c
 *
 *  Created on: Aug 5, 2025
 *      Author: richardmatthews
 */

#include "cli_commands.h"
#include "FreeRTOS_CLI.h"
#include "FreeRTOS_Types.h"
#include "ble_at_client.h"
#include "flash_log.h"
#include "config.h"
#include "cli.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

BaseType_t logClearCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    flash_log_status_t status = flash_log_clear_logs();
    if (status == FLASH_LOG_OK) {
        snprintf(pcWriteBuffer, xWriteBufferLen, "Logs cleared\n");
        return pdFALSE;
    } else {
        snprintf(pcWriteBuffer, xWriteBufferLen, "Error clearing logs\n");
        return pdFALSE;
    }

    return pdFALSE;
}

BaseType_t logDumpCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    flash_log_status_t status = flash_log_send_over_uart();
    if (status == FLASH_LOG_OK) {
        snprintf(pcWriteBuffer, xWriteBufferLen, "Done\n");
        return pdFALSE;
    } else {
        snprintf(pcWriteBuffer, xWriteBufferLen, "Error printing logs\n");
        return pdFALSE;
    }

    return pdFALSE;
}

BaseType_t logSizeCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    uint32_t num_rows = flash_log_get_num_log_entries();

    snprintf(pcWriteBuffer, xWriteBufferLen, "Num Rows: %ld\n", num_rows);
    return pdFALSE;
}

BaseType_t pingCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    snprintf(pcWriteBuffer, xWriteBufferLen, "pong\n");
    return pdFALSE;
}


BaseType_t printCSVDataCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const
                           char *pcCommandString)
{
    uint32_t paramterStringLength;
    const char * enable = FreeRTOS_CLIGetParameter(pcCommandString,
                                                    1,
                                                    &paramterStringLength);

    uint8_t enabled = (uint8_t)atoi(enable);
    if (enabled != 1 && enabled != 0) {
        snprintf(pcWriteBuffer, xWriteBufferLen, "Invalid value for enable param\n");
        return pdFALSE;
    }

    snprintf(pcWriteBuffer, xWriteBufferLen, "Enabled %u\n", enabled);
    config_set_print_csv_data(enabled);

    return pdFALSE;
}

BaseType_t logEnableCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const
                             char *pcCommandString)
{
    uint32_t paramterStringLength;
    const char * enable = FreeRTOS_CLIGetParameter(pcCommandString,
                                                    1,
                                                    &paramterStringLength);

    uint8_t enabled = (uint8_t)atoi(enable);
    if (enabled != 1 && enabled != 0) {
        snprintf(pcWriteBuffer, xWriteBufferLen, "Invalid value for enable param\n");
        return pdFALSE;
    }

    snprintf(pcWriteBuffer, xWriteBufferLen, "Enabled %u\n", enabled);
    config_set_logging_enabled(enabled);

    return pdFALSE;
}

BaseType_t exitCLICommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    printf("Exiting CLI\n");
    config_set_cli_enabled(false);

    cli_setup_waiting_for_input();

    return pdFALSE;
}
