#include <assert.h>

#include "flash_log.h"
#include "flash_log_internal.h"

#include "ai_output_data_type.h"

#include "High_Level/nor_flash.h"

// 15 4 byte values, plus 4 byte marker = 16 * 4 = 64 bytes
// This is a multiple of 256, to make it an even multiple of a sector
#define FLASH_LOG_ROW_SIZE 16 * 4

static_assert(sizeof(flash_log_row_t) == FLASH_LOG_ROW_SIZE,
              "sizeof(flash_log_row_t) != FLASH_LOG_ROW_SIZE");

flash_log_status_t flash_log_init(void) {
    return nor_flash_init();
}

flash_log_status_t flash_log_write_window(const accel_data_t *unproc,
                                          const accel_data_t *lowpass_filtered,
                                          const ai_input_data_t *proc,
                                          const float *model_output,
                                          uint32_t output_class,
                                          int window_size)
{
    flash_log_row_t row;

    for (int i = 0; i < window_size; i++) {
        row.row_start_marker = FLASH_LOG_ROW_START_MARKER;

        row.unproc_x = unproc->x[i];
        row.unproc_y = unproc->y[i];
        row.unproc_z = unproc->z[i];

        row.lowpass_filtered_x = lowpass_filtered->x[i];
        row.lowpass_filtered_y = lowpass_filtered->y[i];
        row.lowpass_filtered_z = lowpass_filtered->z[i];

        row.proc_x = AI_INPUT_GET_X(proc->data_array, i);
        row.proc_y = AI_INPUT_GET_Y(proc->data_array, i);
        row.proc_z = AI_INPUT_GET_Z(proc->data_array, i);

        if (i == (window_size - 1)) {
            row.contains_output = 1;

            for (int j = 0; j < AI_OUTPUT_CHANNEL; j++) {
                row.model_output[j] = model_output[j];
            }

            row.output_class = output_class;
        } else {
            for (int j = 0; j < AI_OUTPUT_CHANNEL; j++) {
                row.model_output[j] = 0;
            }

            row.output_class = 0;

            row.contains_output = 0;
        }

        int32_t rc = nor_flash_write(0, (uint8_t *)&row, FLASH_LOG_ROW_SIZE);
        if (rc != 0) {
            return FLASH_LOG_ERROR;
        }
    }

    return FLASH_LOG_OK;
}
