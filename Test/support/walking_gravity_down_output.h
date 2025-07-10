#pragma once
#include <math.h>
#include "arm_math.h"

#define GRAVITY_DOWN_OUTPUT_DURATION_SEC 100
#define GRAVITY_DOWN_OUTPUT_SAMPLE_RATE_HZ 100
#define GRAVITY_DOWN_OUTPUT_LEN 10000
#define GRAVITY_DOWN_OUTPUT_VALID_START_IDX 270

extern const uint32_t gravity_down_output_len;
extern const float32_t gravity_down_output_x[];
extern const float32_t gravity_down_output_y[];
extern const float32_t gravity_down_output_z[];
