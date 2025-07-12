#pragma once
#include <math.h>
#include "arm_math.h"

#define GRAVITY_UP_INPUT_DURATION_SEC 100
#define GRAVITY_UP_INPUT_SAMPLE_RATE_HZ 100
#define GRAVITY_UP_INPUT_LEN 10000

extern const uint32_t gravity_up_input_len;
extern const float32_t gravity_up_input_x[];
extern const float32_t gravity_up_input_y[];
extern const float32_t gravity_up_input_z[];
