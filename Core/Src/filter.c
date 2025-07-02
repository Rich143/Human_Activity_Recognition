#include "filter.h"

#include "arm_math.h"

#define NUM_SECTIONS 3
static float32_t biquad_state_x[2 * NUM_SECTIONS] = {0};
static float32_t biquad_state_y[2 * NUM_SECTIONS] = {0};
static float32_t biquad_state_z[2 * NUM_SECTIONS] = {0};

float32_t biquad_coeffs[] = {
    2.74708283177703535e-04f,     5.49416566355407070e-04f,
    2.74708283177703535e-04f,     1.96891130024857519e+00f,
    -9.69169987583545445e-01f,     3.16227766016837953e-05f,
    6.32455532033675906e-05f,     3.16227766016837953e-05f,
    1.97707749719089332e+00f,     -9.77337257449627850e-01f,
    3.16227766016837953e-05f,     6.32455532033675906e-05f,
    3.16227766016837953e-05f,     1.99138319885919479e+00f,
    -9.91644838686520758e-01f
};

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
