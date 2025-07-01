#include "filter.h"

#include "arm_math.h"

#define NUM_SECTIONS 3
static float32_t biquad_state_x[2 * NUM_SECTIONS] = {0};
static float32_t biquad_state_y[2 * NUM_SECTIONS] = {0};
static float32_t biquad_state_z[2 * NUM_SECTIONS] = {0};

static float32_t biquad_coeffs[5 * NUM_SECTIONS] = {0};

filter_status_t filter_signal(AccelData *data, AccelData *output) {
    arm_biquad_cascade_df2T_instance_f32 filter_x;
    arm_biquad_cascade_df2T_init_f32(&filter_x, NUM_SECTIONS, biquad_coeffs, biquad_state_x);

    arm_biquad_cascade_df2T_instance_f32 filter_y;
    arm_biquad_cascade_df2T_init_f32(&filter_y, NUM_SECTIONS, biquad_coeffs, biquad_state_y);

    arm_biquad_cascade_df2T_instance_f32 filter_z;
    arm_biquad_cascade_df2T_init_f32(&filter_z, NUM_SECTIONS, biquad_coeffs, biquad_state_z);

    arm_biquad_cascade_df2T_f32(&filter_x, data->x, output->x, data->num_samples);
    arm_biquad_cascade_df2T_f32(&filter_y, data->y, output->y, data->num_samples);
    arm_biquad_cascade_df2T_f32(&filter_z, data->z, output->z, data->num_samples);

    return FILTER_STATUS_OK;
}
