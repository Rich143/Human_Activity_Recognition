/*
 * flash_error_log_internal.h
 *
 *  Created on: October 22, 2025
 *      Author: richardmatthews
 */

#ifndef FLASH_ERROR_LOG_INTERNAL_H
#define FLASH_ERROR_LOG_INTERNAL_H

#include <stdint.h>

#include "flash_error_log.h"

#define ERROR_LOG_ROW_START_MARKER 0xFACE

typedef struct {
    uint16_t row_start_marker;

    uint32_t timestamp_ms; // ms since boot

    error_code_t error_code;
    error_code_data_t data;
} __attribute__((packed)) error_log_row_t;

#endif /* FLASH_ERROR_LOG_INTERNAL_H */
