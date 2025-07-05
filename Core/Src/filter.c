#include "filter.h"

#include "arm_math.h"
#include <stdbool.h>

const float32_t biquad_coeffs[] = {
    2.74708283177703535e-04f,     5.49416566355407070e-04f,
    2.74708283177703535e-04f,     1.96891130024857519e+00f,
    -9.69169987583545445e-01f,     3.16227766016837953e-05f,
    6.32455532033675906e-05f,     3.16227766016837953e-05f,
    1.97707749719089332e+00f,     -9.77337257449627850e-01f,
    3.16227766016837953e-05f,     6.32455532033675906e-05f,
    3.16227766016837953e-05f,     1.99138319885919479e+00f,
    -9.91644838686520758e-01f
};

/**
 * Initialize the filter
 *
 * This resets the filter state
 *
 * @param filter Pointer to filter state struct
 *
 * @return status
 */
filter_status_t filter_init(filter_t *filter) {
    if (filter == NULL) {
        return FILTER_STATUS_ERROR_NULL;
    }

    arm_biquad_cascade_df2T_init_f32(&filter->filter_x,
                                     FILTER_NUM_SECTIONS,
                                     biquad_coeffs,
                                     filter->biquad_state_x);

    arm_biquad_cascade_df2T_init_f32(&filter->filter_y,
                                     FILTER_NUM_SECTIONS,
                                     biquad_coeffs,
                                     filter->biquad_state_y);

    arm_biquad_cascade_df2T_init_f32(&filter->filter_z,
                                     FILTER_NUM_SECTIONS,
                                     biquad_coeffs,
                                     filter->biquad_state_z);

    return FILTER_STATUS_OK;
}

filter_status_t filter_signal(filter_t *filter, const accel_data_t *data,
                              accel_data_t *output)
{
    if (filter == NULL || data == NULL || output == NULL) {
        return FILTER_STATUS_ERROR_NULL;
    }

    arm_biquad_cascade_df2T_f32(&filter->filter_x, data->x,
                                output->x, data->num_samples);

    arm_biquad_cascade_df2T_f32(&filter->filter_y, data->y,
                                output->y, data->num_samples);

    arm_biquad_cascade_df2T_f32(&filter->filter_z, data->z,
                                output->z, data->num_samples);

    return FILTER_STATUS_OK;
}
