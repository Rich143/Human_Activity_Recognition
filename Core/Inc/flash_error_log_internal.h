/*
 * flash_error_log_internal.h
 *
 *  Created on: October 22, 2025
 *      Author: richardmatthews
 */

#ifndef FLASH_ERROR_LOG_INTERNAL_H
#define FLASH_ERROR_LOG_INTERNAL_H

#include <stdint.h>
#include <assert.h>

#include "flash_error_log.h"

#define ERROR_LOG_ROW_START_MARKER 0xFACE

#define ERROR_LOG_ROW_SIZE_BYTES 16

typedef struct {
    uint16_t row_start_marker;

    uint32_t timestamp_ms; // ms since boot

    error_code_t error_code;
    error_code_data_t data;

    uint32_t padding;
} __attribute__((packed)) error_log_row_t;

static_assert(ERROR_LOG_ROW_SIZE_BYTES == sizeof(error_log_row_t),
              "ERROR_LOG_ROW_SIZE_BYTES != sizeof(error_log_row_t)");

#endif /* FLASH_ERROR_LOG_INTERNAL_H */
