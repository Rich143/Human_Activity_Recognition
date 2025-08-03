#pragma once
#include <math.h>
#include "arm_math.h"

#define WALKING_CAPTURE_INPUT_DURATION_SEC 81
#define WALKING_CAPTURE_INPUT_SAMPLE_RATE_HZ 100
#define WALKING_CAPTURE_INPUT_LEN 8140

extern const uint32_t walking_capture_input_len;
extern const float32_t walking_capture_input_x[];
extern const float32_t walking_capture_input_y[];
extern const float32_t walking_capture_input_z[];
