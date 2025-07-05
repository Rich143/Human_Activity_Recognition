/*
 * filter.h
 *
 *  Created on: June 30, 2025
 *      Author: richardmatthews
 */

#ifndef FILTER_SIGNALS_H
#define FILTER_SIGNALS_H

#include "accel_data_type.h"
#include "arm_math.h"

#define FILTER_NUM_SECTIONS 3

// Hardcoded group delay calculated in python
#define FILTER_MEAN_GROUP_DELAY 270

typedef enum {
    FILTER_STATUS_OK = 0,
    FILTER_STATUS_ERROR_NULL,
    FILTER_STATUS_ERROR_OTHER,
} filter_status_t;

typedef struct {
    arm_biquad_cascade_df2T_instance_f32 filter_x;
    arm_biquad_cascade_df2T_instance_f32 filter_y;
    arm_biquad_cascade_df2T_instance_f32 filter_z;

    float32_t biquad_state_x[2 * FILTER_NUM_SECTIONS];
    float32_t biquad_state_y[2 * FILTER_NUM_SECTIONS];
    float32_t biquad_state_z[2 * FILTER_NUM_SECTIONS];
} filter_t;

filter_status_t filter_init(filter_t *filter);
filter_status_t filter_signal(filter_t *filter, const accel_data_t *data, accel_data_t *output);

#endif
