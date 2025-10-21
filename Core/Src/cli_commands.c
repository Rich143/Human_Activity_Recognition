/*
 * cli_commands.c
 *
 *  Created on: Aug 5, 2025
 *      Author: richardmatthews
 */

#include "cli_commands.h"
#include "FreeRTOS_CLI.h"
#include "FreeRTOS_Types.h"
#include "High_Level/motion_sensor_interface.h"
#include "b-u585i-iot02a-bsp/b_u585i_iot02a_errno.h"
#include "ble_at_client.h"
#include "flash_log.h"
#include "config.h"
#include "cli.h"
#include "High_Level/nor_flash.h"
#include "terminal_print.h"

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

bool get_address_size(const char *pcCommandString, uint32_t *address, uint32_t *size) {
    uint32_t paramterStringLength;
    const char * address_string = FreeRTOS_CLIGetParameter(pcCommandString,
                                                    1,
                                                    &paramterStringLength);

    if (address_string == NULL || paramterStringLength == 0) {
        return false;
    }

    const char * size_string = FreeRTOS_CLIGetParameter(pcCommandString,
                                                    2,
                                                    &paramterStringLength);

    if (size_string == NULL || paramterStringLength == 0) {
        return false;
    }

    *address = (uint32_t)strtoul(address_string, NULL, 0);
    *size = (uint32_t)strtoul(size_string, NULL, 0);

    return true;
}

BaseType_t flashDumpCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    const uint32_t buf_elements = 256 / sizeof(uint32_t);

    uint32_t buf[buf_elements];
    uint32_t readAddress;

    uint32_t address;
    uint32_t size;

    if (!get_address_size(pcCommandString, &address, &size)) {
        snprintf(pcWriteBuffer, xWriteBufferLen, "Invalid address or size\n");
        return pdFALSE;
    }

    readAddress = address;
    size = size * sizeof(buf);

    while (readAddress < (address + size)) {
        printf("Address: 0x%08lX\n\n", readAddress);
        int32_t status = nor_flash_read(readAddress, (uint8_t *)&buf, sizeof(buf));
        if (status != BSP_ERROR_NONE) {
            printf("Failed to read: %ld\n", status);
            return pdFALSE;
        }

        for (int i = 0; i < buf_elements; i++) {
            cli_printf("0x%08lX ", buf[i]);

            if ((i + 1) % 16 == 0) {
                cli_printf("\n");
            }
        }
        cli_printf("\n");

        readAddress += sizeof(buf);
    }

    return pdFALSE;
}

BaseType_t flashClearCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    const uint32_t buf_elements = 256 / sizeof(uint32_t);

    uint32_t clearAddress;

    uint32_t address;
    uint32_t num_sectors;

    if (!get_address_size(pcCommandString, &address, &num_sectors)) {
        snprintf(pcWriteBuffer, xWriteBufferLen, "Invalid address or num_sectors\n");
        return pdFALSE;
    }

    clearAddress = address * NOR_FLASH_SECTOR_SIZE;

    for (int i = 0; i < num_sectors; i ++) {
        printf("Erasing address 0x%08lX\n", clearAddress);
        int32_t status = nor_flash_erase_sector(clearAddress);
        if (status != BSP_ERROR_NONE) {
            printf("Failed to erase sector: %ld\n", status);
            return pdFALSE;
        }
        clearAddress += NOR_FLASH_SECTOR_SIZE;
    }
    return pdFALSE;
}

BaseType_t flashClearChipCommand(char *pcWriteBuffer,
                           size_t xWriteBufferLen,
                           const char *pcCommandString)
{
    printf("Erasing chip\n");
    int32_t status = nor_flash_erase_chip();
    if (status != BSP_ERROR_NONE) {
        printf("Failed to erase chip: %ld\n", status);
    } else {
        printf("Done\n");
    }

    return pdFALSE;
}


BaseType_t flashClearedCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    int32_t status = nor_flash_check_cleared();
    if (status != BSP_ERROR_NONE) {
        snprintf(pcWriteBuffer, xWriteBufferLen, "Flash not cleared\n");
        return pdFALSE;
    }
    snprintf(pcWriteBuffer, xWriteBufferLen, "Flash cleared\n");
    return pdFALSE;
}

BaseType_t getIMUScaleCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    int32_t full_scale;

    int32_t status = motion_sensor_get_full_scale(MOTION_SENSOR_ACCEL,
                                                  &full_scale);
    if (status != BSP_ERROR_NONE) {
        snprintf(pcWriteBuffer, xWriteBufferLen, "Error getting IMU scale\n");
        return pdFALSE;
    }

    snprintf(pcWriteBuffer, xWriteBufferLen, "IMU Full Scale: ±%ld g\n", full_scale);

    return pdFALSE;
}

BaseType_t enablePredictionsCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const
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

    snprintf(pcWriteBuffer, xWriteBufferLen, "Predictions Enabled %u\n", enabled);
    config_set_predictions_enabled(enabled);

    return pdFALSE;
}
