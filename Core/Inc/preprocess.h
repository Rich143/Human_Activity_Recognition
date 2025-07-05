/*
 * preprocess.h
 *
 *  Created on: June 22, 2025
 *      Author: richardmatthews
 */

#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <float.h>
#include "arm_math.h"
#include "accel_data_type.h"
#include "delay_signal.h"
#include "filter.h"

typedef enum {
    PREPROCESS_STATUS_OK = 0,
    PREPROCESS_STATUS_ERROR_NULL,
    PREPROCESS_STATUS_ERROR_BUFFERING,
    PREPROCESS_STATUS_ERROR_OTHER,
} preprocess_status_t;

typedef struct {
  delay_signal_t delay_signal; // Store delayed version of original signal for isolation
                               // gravity
  filter_t lp_filter; // Lowpass filter to isolate gravity
} preprocess_t;

preprocess_status_t preprocess_init(preprocess_t *preprocess);

preprocess_status_t gravity_suppress_rotate(preprocess_t *preprocess,
                                            const accel_data_t *input,
                                            accel_data_t *scratch_buffer,
                                            accel_data_t *output);
#endif
