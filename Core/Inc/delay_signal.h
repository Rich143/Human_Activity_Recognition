/*
 * delay_signal.h
 *
 *  Created on: June 22, 2025
 *      Author: richardmatthews
 */

#ifndef DELAY_SIGNAL_H
#define DELAY_SIGNAL_H

#include "accel_data_type.h"
#include "filter.h"

#define CIRCULAR_BUFFER_TYPE float
#define CIRCULAR_BUFFER_PREFIX f32_cb
#include "circular_buffer.h"

// Need to be able to delay by group delay for preprocessing
#define DELAY_BUFFER_SIZE FILTER_MEAN_GROUP_DELAY + 1
#define DELAY_MAX_ALLOWED_DELAY (DELAY_BUFFER_SIZE - 1)

typedef enum {
    DELAY_STATUS_OK = 0,
    DELAY_STATUS_ERROR_DELAY_TOO_LARGE,
    DELAY_STATUS_ERROR_NULL,
    DELAY_STATUS_ERROR_BUFFER_FULL,
    DELAY_STATUS_ERROR_OTHER,
} delay_status_t;

typedef struct {
    float x_buf[DELAY_BUFFER_SIZE];
    float y_buf[DELAY_BUFFER_SIZE];
    float z_buf[DELAY_BUFFER_SIZE];

    f32_cb_t x_cb;
    f32_cb_t y_cb;
    f32_cb_t z_cb;
} delay_signal_t;

delay_status_t delay_signal_init(delay_signal_t *delay_signal);

delay_status_t delay_signal_push_signal(delay_signal_t *delay_signal, const accel_data_t *signal);

delay_status_t delay_signal_get_delay_range(delay_signal_t *delay_signal,
                                            accel_data_t *signal,
                                            uint32_t delay_amount,
                                            uint32_t len);

#endif
