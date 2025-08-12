/*
 * cli_commands.c
 *
 *  Created on: Aug 5, 2025
 *      Author: richardmatthews
 */

#include "cli_commands.h"
#include "FreeRTOS_Types.h"
#include "flash_log.h"

#include <stddef.h>
#include <string.h>

BaseType_t logClearCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    flash_log_status_t status = flash_log_clear_logs();
    if (status == FLASH_LOG_OK) {
        vsnprintf(pcWriteBuffer, xWriteBufferLen, "Logs cleared\n");
        return pdFalse;
    } else {
        vsnprintf(pcWriteBuffer, xWriteBufferLen, "Error clearing logs\n");
        return pdFalse;
    }
}

BaseType_t logDumpCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    flash_log_status_t status = flash_log_send_over_uart();
    if (status == FLASH_LOG_OK) {
        vsnprintf(pcWriteBuffer, xWriteBufferLen, "Done\n");
        return pdFalse;
    } else {
        vsnprintf(pcWriteBuffer, xWriteBufferLen, "Error printing logs\n");
        return pdFalse;
    }
}

BaseType_t pingCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    vsnprintf(pcWriteBuffer, xWriteBufferLen, "pong\n");
    return pdFalse;
}
