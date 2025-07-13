#pragma once
#include <math.h>
#include "arm_math.h"

#define SLOWLY_ROTATING_OUTPUT_DURATION_SEC 100
#define SLOWLY_ROTATING_OUTPUT_SAMPLE_RATE_HZ 100
#define SLOWLY_ROTATING_OUTPUT_LEN 10000
#define SLOWLY_ROTATING_OUTPUT_VALID_START_IDX 270

extern const uint32_t slowly_rotating_output_len;
extern const float32_t slowly_rotating_output_x[];
extern const float32_t slowly_rotating_output_y[];
extern const float32_t slowly_rotating_output_z[];
