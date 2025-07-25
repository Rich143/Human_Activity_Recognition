/*
 * ai_input_data_type.h
 *
 *  Created on: July 23, 2025
 *      Author: richardmatthews
 */

#ifndef AI_INPUT_DATA_TYPE_H
#define AI_INPUT_DATA_TYPE_H

#include "arm_math_types.h"

#define AI_INPUT_HEIGHT 24
#define AI_INPUT_WIDTH 3
#define AI_INPUT_CHANNEL 1

#define AI_INPUT_X_IDX 0
#define AI_INPUT_Y_IDX 1
#define AI_INPUT_Z_IDX 2

#define AI_INPUT_CHANNEL_IDX 0

// Accessors for accelerometer axes at a given sample index
#define AI_INPUT_GET_X(data_array, sample_idx) \
((data_array)[(sample_idx)][AI_INPUT_X_IDX][AI_INPUT_CHANNEL_IDX])

#define AI_INPUT_GET_Y(data_array, sample_idx) \
    ((data_array)[(sample_idx)][AI_INPUT_Y_IDX][AI_INPUT_CHANNEL_IDX])

#define AI_INPUT_GET_Z(data_array, sample_idx) \
    ((data_array)[(sample_idx)][AI_INPUT_Z_IDX][AI_INPUT_CHANNEL_IDX])


/**
 * AI Input data type
 *
 * Contains accel data (x, y, z) in m/s^2 to be passed to the x cube ai
 * inference engine
 *
 * Height is the sample idx
 * Width is the x,y,z accel axis idx
 * There is only one channel
 */
typedef struct {
    float (*data_array)[AI_INPUT_WIDTH][AI_INPUT_CHANNEL];
} ai_input_data_t;

#endif
