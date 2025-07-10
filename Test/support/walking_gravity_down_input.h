#pragma once
#include <math.h>
#include "arm_math.h"

#define GRAVITY_DOWN_INPUT_DURATION_SEC 100
#define GRAVITY_DOWN_INPUT_SAMPLE_RATE_HZ 100
#define GRAVITY_DOWN_INPUT_LEN 10000

extern const uint32_t gravity_down_input_len;
extern const float32_t gravity_down_input_x[];
extern const float32_t gravity_down_input_y[];
extern const float32_t gravity_down_input_z[];
