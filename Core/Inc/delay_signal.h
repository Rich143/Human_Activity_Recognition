/*
 * delay_signal.h
 *
 *  Created on: June 22, 2025
 *      Author: richardmatthews
 */

#ifndef DELAY_SIGNAL_H
#define DELAY_SIGNAL_H

#include "preprocess.h"

#define CIRCULAR_BUFFER_TYPE float
#define CIRCULAR_BUFFER_PREFIX f32_cb
#include "circular_buffer.h"

#define DELAY_MAX_LEN 270

typedef enum {
    DELAY_STATUS_OK = 0,
    DELAY_STATUS_ERROR_DELAY_TOO_LARGE,
    DELAY_STATUS_ERROR_NULL,
    DELAY_STATUS_ERROR_BUFFER_FULL,
    DELAY_STATUS_ERROR_OTHER,
} delay_status_t;

typedef struct {
    float x_buf[DELAY_MAX_LEN];
    float y_buf[DELAY_MAX_LEN];
    float z_buf[DELAY_MAX_LEN];

    f32_cb_t x_cb;
    f32_cb_t y_cb;
    f32_cb_t z_cb;
} delay_signal_t;

delay_status_t delay_signal_init(delay_signal_t *delay_signal);

delay_status_t delay_signal_push_signal(delay_signal_t *delay_signal, AccelData *signal);

delay_status_t delay_signal_get_delay_range(delay_signal_t *delay_signal,
                                            AccelData *signal,
                                            uint32_t delay_amount,
                                            uint32_t len);

#endif
