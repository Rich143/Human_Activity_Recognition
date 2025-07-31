/*
 * flash_log_internal.h
 *
 *  Created on: July 29, 2025
 *      Author: richardmatthews
 */

#ifndef FLASH_LOG_INTERNAL_H
#define FLASH_LOG_INTERNAL_H

#include <stdint.h>

#include "flash_log.h"

#define FLASH_LOG_ROW_START_MARKER 0xBEEDFACE

typedef struct {
    uint32_t row_start_marker;

    float unproc_x;
    float unproc_y;
    float unproc_z;

    float lowpass_filtered_x;
    float lowpass_filtered_y;
    float lowpass_filtered_z;

    float proc_x;
    float proc_y;
    float proc_z;

    float model_output[4];

    uint32_t output_class;

    uint32_t contains_output;
} flash_log_row_t;

#endif /* FLASH_LOG_INTERNAL_H */
