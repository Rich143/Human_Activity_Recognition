#include "flash_error_log.h"
#include "High_Level/flash_manager.h"
#include "flash_manager.h"
#include "flash_error_log_internal.h"
#include "stm32u5xx_hal.h"
#include "log_utils.h"
#include "High_Level/uart.h"

#include <assert.h>
#include <stdlib.h>

#define ERROR_LOG_BUFFER_SIZE_ROWS  (NOR_FLASH_PAGE_SIZE / sizeof(error_log_row_t))

uint32_t num_entries = 0;

error_log_row_t error_log_buffer[ERROR_LOG_BUFFER_SIZE_ROWS];

error_log_status_t error_log_init()
{
    flash_manager_status_t flash_status = flash_manager_init();
    if (flash_status != FLASH_MANAGER_OK) {
        return ERROR_LOG_FLASH_INIT_ERROR;
    }

    num_entries = 0;

    error_log_status_t status = error_log_recover_log_pointer();
    if (status != ERROR_LOG_OK) {
        return status;
    }

    return ERROR_LOG_OK;
}

uint32_t get_error_log_write_address() {
    return num_entries * sizeof(error_log_row_t);
}

error_log_status_t check_data_type(error_code_t error_code,
                                   error_data_types_t data_type) {
    // TODO: implement
    return ERROR_LOG_OK;
}

error_log_status_t error_log(error_code_t error_code,
                             error_data_types_t data_type,
                             error_code_data_t data)
{
    error_log_row_t row;

    error_log_status_t status = check_data_type(error_code, data_type);
    if (status != ERROR_LOG_OK) {
        return status;
    }

    row.header.row_start_marker = ERROR_LOG_ROW_START_MARKER;
    row.timestamp_ms = HAL_GetTick();
    row.error_code = error_code;
    row.data = data;
    row.padding = 0;

    uint32_t write_address = get_error_log_write_address();

    flash_manager_status_t flash_status =
        flash_manager_write(FLASH_REGION_ERROR_LOGS,
                            write_address, (uint8_t *)&row,
                            sizeof(row));
    if (flash_status == FLASH_MANAGER_ERROR_OUT_OF_BOUNDS) {
        return ERROR_LOG_FULL;
    } else if (flash_status != FLASH_MANAGER_OK) {
        return ERROR_LOG_FLASH_WRITE_ERROR;
    }

    num_entries++;

    return ERROR_LOG_OK;
}

uint32_t ceil_div(uint32_t a, uint32_t b) {
    return (a + b - 1) / b;
}


error_log_status_t error_log_clear_logs(void)
{
    if (num_entries == 0) {
        return ERROR_LOG_OK;
    }

    uint32_t numSectors = ceil_div(num_entries * ERROR_LOG_ROW_SIZE_BYTES,
                                   NOR_FLASH_SECTOR_SIZE);

    for (uint32_t i = 0; i < numSectors; i++) {
        int32_t status =
            flash_manager_erase_sector(FLASH_REGION_ERROR_LOGS,
                                       i);
        if (status != 0) {
            return ERROR_LOG_FLASH_ERASE_ERROR;
        }
    }

    num_entries = 0;

    return ERROR_LOG_OK;
}

error_log_status_t log_utils_status_to_error_log_status(log_utils_status_t status)
{
    switch (status) {
        case LOG_UTILS_OK:
            return ERROR_LOG_OK;
            break;
        case LOG_UTILS_FLASH_READ_ERROR:
            return ERROR_LOG_FLASH_READ_ERROR;
            break;
        case LOG_UTILS_CORRUPT_LOG:
            return ERROR_LOG_CORRUPT_LOG_ERROR;
            break;
        case LOG_UTILS_LOG_FULL:
            return ERROR_LOG_FULL;
            break;
        default:
            return ERROR_LOG_ERROR;
    }
}

error_log_status_t error_log_recover_log_pointer()
{
    flash_region_t *region;

    region = flash_manager_get_region(FLASH_REGION_ERROR_LOGS);
    if (region == NULL) {
        return ERROR_LOG_CONFIG_ERROR;
    }

    log_utils_status_t status =
        log_utils_recover_log_pointer(region,
                                  ERROR_LOG_ROW_SIZE_BYTES,
                                  &num_entries,
                                  (uint8_t *)error_log_buffer,
                                  ERROR_LOG_BUFFER_SIZE_ROWS);

    return log_utils_status_to_error_log_status(status);
}

error_log_status_t error_log_get_num_log_entries()
{
    return num_entries;
}

error_log_status_t send_row_over_uart(error_log_row_t *row) {
    bool success = uart_cli_send_data((uint8_t *)row,
                                      sizeof(error_log_row_t));

    if (success) {
        return ERROR_LOG_OK;
    } else {
        return ERROR_LOG_UART_SEND_ERROR;
    }
}

handle_row_return_t error_log_handle_row(uint8_t *row_data, uint32_t
                                            row_size_bytes, void *user_data)
{
    error_log_status_t status
        = send_row_over_uart((error_log_row_t *)row_data);

    if (status != ERROR_LOG_OK) {
        *((error_log_status_t *)user_data) = status;

        return HANDLE_ROW_STOP;
    }

    return HANDLE_ROW_CONTINUE;
}

error_log_status_t error_log_send_over_uart() {
    error_log_status_t send_status = ERROR_LOG_OK;

    log_utils_status_t status = log_utils_iterate_logs(
        flash_manager_get_region(FLASH_REGION_ERROR_LOGS),
        ERROR_LOG_ROW_SIZE_BYTES,
        (uint8_t *)error_log_buffer,
        ERROR_LOG_BUFFER_SIZE_ROWS,
        num_entries,
        error_log_handle_row,
        &send_status
    );

    if (status != LOG_UTILS_OK) {
        return log_utils_status_to_error_log_status(status);
    }

    return send_status;
}
