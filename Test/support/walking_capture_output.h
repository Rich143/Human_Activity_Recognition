#pragma once
#include <math.h>
#include "arm_math.h"

#define WALKING_CAPTURE_OUTPUT_DURATION_SEC 81
#define WALKING_CAPTURE_OUTPUT_SAMPLE_RATE_HZ 100
#define WALKING_CAPTURE_OUTPUT_LEN 8140
#define WALKING_CAPTURE_OUTPUT_VALID_START_IDX 270

extern const uint32_t walking_capture_output_len;
extern const float32_t walking_capture_output_x[];
extern const float32_t walking_capture_output_y[];
extern const float32_t walking_capture_output_z[];
