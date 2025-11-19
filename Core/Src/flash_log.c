#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "config.h"
#include "flash_log.h"
#include "flash_log_internal.h"

#include "ai_output_data_type.h"

#include "High_Level/nor_flash.h"
#include "High_Level/flash_manager.h"
#include "High_Level/uart.h"
#include "log_utils.h"
#include "utils.h"

// 15 4 byte values, plus 4 byte marker = 16 * 4 = 64 bytes
// This is a multiple of 256, to make it an even multiple of a sector
#define FLASH_LOG_ROW_SIZE 16 * 4

static_assert(sizeof(flash_log_row_t) == FLASH_LOG_ROW_SIZE,
              "sizeof(flash_log_row_t) != FLASH_LOG_ROW_SIZE");

// This makes buffer equal 4096 bytes, which equals a subsector of flash
#define FLASH_LOG_BUFFER_NUM_ROWS 64

#define FLASH_LOG_SIZE (flash_log_max_saved_rows * FLASH_LOG_ROW_SIZE)

// Max value allowed for number of saved rows
#define CONFIG_MAX_SAVED_ROWS (NOR_FLASH_SIZE / FLASH_LOG_ROW_SIZE)

#define ALL_ONES(type) ((type)~(type)0)
#define CLEARED_FLASH(type) ALL_ONES(type)

flash_log_row_t flash_log_buffer[FLASH_LOG_BUFFER_NUM_ROWS];
uint32_t flash_log_num_buffered_rows;
uint32_t flash_log_num_rows;

flash_log_status_t flash_log_init() {
    int rc = flash_manager_init();
    if (rc != 0) {
        return FLASH_LOG_ERROR;
    }

    flash_log_status_t status = flash_log_recover_log_pointer();
    if (status != FLASH_LOG_OK) {
        return status;
    }

    flash_log_num_buffered_rows = 0;

    return FLASH_LOG_OK;
}

flash_log_row_t *get_row_buffer() {
    if (flash_log_num_buffered_rows >= FLASH_LOG_BUFFER_NUM_ROWS) {
        return NULL;
    }

    flash_log_row_t *row = &flash_log_buffer[flash_log_num_buffered_rows];

    flash_log_num_buffered_rows++;

    return row;
}

uint32_t get_flash_log_write_address() {
    return flash_log_num_rows * sizeof(flash_log_row_t);
}

flash_log_status_t flash_manager_status_to_flash_log_status(flash_manager_status_t status)
{
    switch (status) {
        case FLASH_MANAGER_OK:
            return FLASH_LOG_OK;
        case FLASH_MANAGER_ERROR_OUT_OF_BOUNDS:
            return FLASH_LOG_FULL;
        case FLASH_MANAGER_ERROR_FLASH_WRITE:
            return FLASH_LOG_FLASH_WRITE_ERROR;
        case FLASH_MANAGER_ERROR_FLASH_READ:
            return FLASH_LOG_FLASH_READ_ERROR;
        case FLASH_MANAGER_ERROR_FLASH_ERASE:
            return FLASH_LOG_FLASH_ERASE_ERROR;
        case FLASH_MANAGER_ERROR_INVALID_REGION:
        case FLASH_MANAGER_ERROR:
        case FLASH_MANAGER_ERROR_FLASH_INIT:
            return FLASH_LOG_ERROR;
        default:
            return FLASH_LOG_ERROR;
    }
}

flash_log_status_t flush_row_buffer() {
    if (flash_log_num_buffered_rows == 0) {
        return FLASH_LOG_OK;
    }

    uint32_t writeAddress = get_flash_log_write_address();

    flash_manager_status_t status = flash_manager_write(FLASH_REGION_DATA_LOGS,
                                     writeAddress,
                                     (uint8_t *)&flash_log_buffer,
                                     FLASH_LOG_ROW_SIZE * flash_log_num_buffered_rows);

    if (status != FLASH_MANAGER_OK) {
        return flash_manager_status_to_flash_log_status(status);
    }

    flash_log_num_rows += flash_log_num_buffered_rows;
    flash_log_num_buffered_rows = 0;

    return FLASH_LOG_OK;
}

flash_log_status_t flash_log_write_window(const accel_data_t *unproc,
                                          const accel_data_t *lowpass_filtered,
                                          const ai_input_data_t *proc,
                                          const float *model_output,
                                          uint32_t output_class,
                                          bool model_enabled,
                                          int window_size)
{
    if (window_size > FLASH_LOG_BUFFER_NUM_ROWS) {
        return FLASH_LOG_PARAM_ERROR;
    }

    for (int i = 0; i < window_size; i++) {
        flash_log_row_t *row = get_row_buffer();
        if (row == NULL) {
            return FLASH_LOG_ERROR;
        }

        row->row_start_marker = FLASH_LOG_ROW_START_MARKER;

        row->unproc_x = unproc->x[i];
        row->unproc_y = unproc->y[i];
        row->unproc_z = unproc->z[i];

        row->lowpass_filtered_x = lowpass_filtered->x[i];
        row->lowpass_filtered_y = lowpass_filtered->y[i];
        row->lowpass_filtered_z = lowpass_filtered->z[i];

        row->proc_x = AI_INPUT_GET_X(proc->data_array, i);
        row->proc_y = AI_INPUT_GET_Y(proc->data_array, i);
        row->proc_z = AI_INPUT_GET_Z(proc->data_array, i);

        if (model_enabled && i == (window_size - 1)) {
            row->contains_output = 1;

            for (int j = 0; j < AI_OUTPUT_CHANNEL; j++) {
                row->model_output[j] = model_output[j];
            }

            row->output_class = output_class;
        } else {
            for (int j = 0; j < AI_OUTPUT_CHANNEL; j++) {
                row->model_output[j] = 0;
            }

            row->output_class = 0;

            row->contains_output = 0;
        }

    }

    return flush_row_buffer();
}

flash_log_status_t log_utils_status_to_flash_log_status(log_utils_status_t status)
{
    switch (status) {
        case LOG_UTILS_OK:
            return FLASH_LOG_OK;
            break;
        case LOG_UTILS_FLASH_READ_ERROR:
            return FLASH_LOG_FLASH_READ_ERROR;
            break;
        case LOG_UTILS_CORRUPT_LOG:
            return FLASH_LOG_CORRUPT_LOG;
            break;
        case LOG_UTILS_LOG_FULL:
            return FLASH_LOG_FULL;
            break;
        default:
            return FLASH_LOG_ERROR;
    }
}

flash_log_status_t flash_log_recover_log_pointer() {
    flash_region_t *region;

    region = flash_manager_get_region(FLASH_REGION_DATA_LOGS);
    if (region == NULL) {
        return FLASH_LOG_ERROR;
    }

    log_utils_status_t status =
        log_utils_recover_log_pointer(region,
                                      FLASH_LOG_ROW_SIZE,
                                      &flash_log_num_rows,
                                      (uint8_t *)flash_log_buffer,
                                      FLASH_LOG_BUFFER_NUM_ROWS);

    return log_utils_status_to_flash_log_status(status);
}

uint32_t flash_log_get_num_log_entries() {
    return flash_log_num_rows;
}

flash_log_status_t flash_log_print_csv() {
    uint32_t readAddress = 0;
    uint32_t num_rows = flash_log_get_num_log_entries();

    printf("unproc_x,unproc_y,unproc_z,lowpass_filtered_x,lowpass_filtered_y,lowpass_filtered_z,");
    printf("proc_x,proc_y,proc_z,contains_output,");
    printf("model_output_0,model_output_1,model_output_2,output_class\n");

    for (uint32_t i = 0; i < num_rows; i += FLASH_LOG_BUFFER_NUM_ROWS) {
        int32_t status = flash_manager_read(FLASH_REGION_DATA_LOGS,
                                            readAddress,
                                            (uint8_t *)&flash_log_buffer,
                                            sizeof(flash_log_buffer));
        if (status != 0) {
            return FLASH_LOG_ERROR;
        }

        for (uint32_t j = 0; j < FLASH_LOG_BUFFER_NUM_ROWS; ++j) {
            flash_log_row_t *row = &flash_log_buffer[j];

            if (row->row_start_marker != FLASH_LOG_ROW_START_MARKER) {
                // End of log
                if (i + j < num_rows) {
                    printf("End of log reached early, corrupted log?. Marker is 0x%lx\n", row->row_start_marker);
                }
                return FLASH_LOG_OK;
            }

            printf("%f,%f,%f,%f,%f,%f,%f,%f,%f",
                   row->unproc_x, row->unproc_y, row->unproc_z,
                   row->lowpass_filtered_x, row->lowpass_filtered_y, row->lowpass_filtered_z,
                   row->proc_x, row->proc_y, row->proc_z);

#if LOG_PRINT_CSV_INCLUDE_MODEL_OUTPUT
            printf(",%lu,", row->contains_output);

            for (uint32_t k = 0; k < AI_OUTPUT_CHANNEL; ++k) {
                printf("%f,", row->model_output[k]);
            }

            printf("%lu\n", row->output_class);
#else
            printf("\n");
#endif
        }

        readAddress += sizeof(flash_log_buffer);
    }

    return FLASH_LOG_OK;
}

flash_log_status_t send_row_over_uart(flash_log_row_t *row) {
    bool success = uart_cli_send_data((uint8_t *)row, sizeof(flash_log_row_t));

    if (success) {
        return FLASH_LOG_OK;
    } else {
        return FLASH_LOG_ERROR;
    }
}

#if CONFIG_LOG_USE_TEST_DATA == 0
handle_row_return_t flash_log_handle_row(uint8_t *row_data, uint32_t
                                            row_size_bytes, void *user_data)
{
    flash_log_status_t status
        = send_row_over_uart((flash_log_row_t *)row_data);

    if (status != FLASH_LOG_OK) {
        *((flash_log_status_t *)user_data) = status;

        return HANDLE_ROW_STOP;
    }

    return HANDLE_ROW_CONTINUE;
}

flash_log_status_t flash_log_send_over_uart() {
    flash_log_status_t send_status = FLASH_LOG_OK;

    log_utils_status_t status = log_utils_iterate_logs(
        flash_manager_get_region(FLASH_REGION_DATA_LOGS),
        FLASH_LOG_ROW_SIZE,
        (uint8_t *)flash_log_buffer,
        FLASH_LOG_BUFFER_NUM_ROWS,
        flash_log_num_rows,
        flash_log_handle_row,
        &send_status
    );

    if (status != LOG_UTILS_OK) {
        return log_utils_status_to_flash_log_status(status);
    }

    return send_status;
}

#else

#include "Recorded_Data/log_test_data.h"

flash_log_status_t flash_log_send_over_uart() {
    for (uint32_t i = 0; i < LOG_TEST_DATA_NUM_ROWS; ++i) {
        flash_log_status_t status = send_row_over_uart(&log_test_data[i]);
        if (status != FLASH_LOG_OK) {
            return status;
        }
    }

    return FLASH_LOG_OK;
}

#endif

flash_log_status_t flash_log_clear_logs() {
    if (flash_log_num_rows == 0) {
        return FLASH_LOG_OK;
    }

    uint32_t numSectors = ceil_div(flash_log_num_rows * FLASH_LOG_ROW_SIZE,
                                   NOR_FLASH_SECTOR_SIZE);

    for (uint32_t i = 0; i < numSectors; i++) {
        int32_t status = flash_manager_erase_sector(FLASH_REGION_DATA_LOGS, i);
        if (status != 0) {
            return FLASH_LOG_ERROR;
        }
    }

    flash_log_num_rows = 0;

    return FLASH_LOG_OK;
}
