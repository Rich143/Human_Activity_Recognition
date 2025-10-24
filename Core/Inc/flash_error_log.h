/*
 * flash_error_log.h
 *
 *  Created on: October 22, 2025
 *      Author: richardmatthews
 */

#ifndef FLASH_ERROR_LOG_H
#define FLASH_ERROR_LOG_H

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

typedef uint16_t error_code_t;
typedef uint32_t error_code_data_t;

typedef enum {
    ERROR_AI_ERROR,
    ERROR_IMU_READ_ERROR,
    ERROR_PREPROCESS_ERROR,
    ERROR_LOG_WRITE_ERROR,
} error_code_values_t;

typedef enum {
    DATA_NONE,
    DATA_BSP_ERROR_CODE, // BSP error code (iot2a errno .h)
    DATA_PREPROCESS_STATUS, // preprocess_status_t
} error_data_types_t;

typedef struct {
    error_code_t code;
    error_code_data_t data;
    const char *error_name;
    const char *data_label;
} ErrorDef;

// TODO: Ensure one entry per error code value
static const ErrorDef error_registry[] = {
    { .code = ERROR_IMU_READ_ERROR, .data = DATA_BSP_ERROR_CODE,
        .error_name = "IMU_READ_ERROR", .data_label = "BSP Error Code" },

    { .code = ERROR_PREPROCESS_ERROR, .data = DATA_PREPROCESS_STATUS,
        .error_name = "PREPROCESS_ERROR", .data_label = "Preprocess Status" },
};

typedef enum {
    ERROR_LOG_OK = 0,
    ERROR_LOG_FULL,
    ERROR_LOG_CONFIG_ERROR,
    ERROR_LOG_PARAM_ERROR,
    ERROR_LOG_FLASH_INIT_ERROR,
    ERROR_LOG_FLASH_WRITE_ERROR,
    ERROR_LOG_FLASH_READ_ERROR,
    ERROR_LOG_FLASH_ERASE_ERROR,
    ERROR_LOG_ERROR,
} error_log_status_t;

error_log_status_t error_log_init();

error_log_status_t error_log(error_code_t error_code,
                             error_data_types_t data_type,
                             error_code_data_t data);

error_log_status_t error_log_recover_log_pointer(void);

uint32_t error_log_get_num_log_entries(void);

error_log_status_t error_log_send_over_uart();

error_log_status_t error_log_clear_logs(void);

#endif /* FLASH_ERROR_LOG_H */
