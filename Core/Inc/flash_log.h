/*
 * flash_log.h
 *
 *  Created on: July 29, 2025
 *      Author: richardmatthews
 */

#ifndef FLASH_LOG_H
#define FLASH_LOG_H

#include "accel_data_type.h"
#include "ai_input_data_type.h"
#include <stdint.h>


typedef enum {
    FLASH_LOG_OK = 0,
    FLASH_LOG_ERROR
} flash_log_status_t;

flash_log_status_t flash_log_init(void);

flash_log_status_t flash_log_write_window(const accel_data_t *unproc,
                                          const accel_data_t *lowpass_filtered,
                                          const ai_input_data_t *proc,
                                          const float *model_output,
                                          uint32_t output_class,
                                          int window_size);

#endif /* FLASH_LOG_H */
