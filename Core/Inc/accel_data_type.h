/*
 * accel_data_type.h
 *
 *  Created on: July 2, 2025
 *      Author: richardmatthews
 */

#ifndef ACCEL_DATA_TYPE_H
#define ACCEL_DATA_TYPE_H

#include "arm_math_types.h"

typedef struct {
    float32_t *x;
    float32_t *y;
    float32_t *z;
    uint32_t num_samples;
} accel_data_t;

#endif
