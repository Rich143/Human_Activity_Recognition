#include "flash_error_log.h"
#include "High_Level/flash_manager.h"
#include "flash_manager.h"
#include "flash_error_log_internal.h"
#include "stm32u5xx_hal.h"

#include <assert.h>

uint32_t num_entries = 0;

error_log_status_t error_log_init()
{
    flash_manager_status_t status = flash_manager_init();
    if (status != FLASH_MANAGER_OK) {
        return ERROR_LOG_FLASH_INIT_ERROR;
    }

    num_entries = 0;

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

    row.row_start_marker = ERROR_LOG_ROW_START_MARKER;
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
