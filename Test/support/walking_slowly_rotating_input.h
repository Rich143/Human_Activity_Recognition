#pragma once
#include <math.h>
#include "arm_math.h"

#define SLOWLY_ROTATING_INPUT_DURATION_SEC 100
#define SLOWLY_ROTATING_INPUT_SAMPLE_RATE_HZ 100
#define SLOWLY_ROTATING_INPUT_LEN 10000

extern const uint32_t slowly_rotating_input_len;
extern const float32_t slowly_rotating_input_x[];
extern const float32_t slowly_rotating_input_y[];
extern const float32_t slowly_rotating_input_z[];
