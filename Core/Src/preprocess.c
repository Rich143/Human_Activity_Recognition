/*
 * preprocess.c
 *
 *  Created on: June 22, 2025
 *      Author: richardmatthews
 */

#include "accel_data_type.h"
#include "ai_input_data_type.h"
#include "arm_math.h"
#include "delay_signal.h"
#include "filter.h"
#include <math.h>
#include <stdio.h>

#include "preprocess.h"

preprocess_status_t check_filter_status(filter_status_t status) {
    switch (status) {
        case FILTER_STATUS_OK:
            return PREPROCESS_STATUS_OK;
        case FILTER_STATUS_ERROR_NULL:
            return PREPROCESS_STATUS_ERROR_NULL;
        default:
            return PREPROCESS_STATUS_ERROR_OTHER;
    }
}

preprocess_status_t check_delay_status(delay_status_t status) {
    switch (status) {
        case DELAY_STATUS_OK:
            return PREPROCESS_STATUS_OK;
        case DELAY_STATUS_ERROR_NULL:
            return PREPROCESS_STATUS_ERROR_NULL;
        default:
            return PREPROCESS_STATUS_ERROR_OTHER;
    }
}

preprocess_status_t preprocess_init(preprocess_t *preprocess) {
    preprocess_status_t status;

    filter_status_t rc = filter_init(&preprocess->lp_filter);
    if ((status = check_filter_status(rc)) != PREPROCESS_STATUS_OK) {
        return status;
    }

    delay_status_t rc_d =
        delay_signal_init(&preprocess->delay_signal);
    if ((status = check_delay_status(rc_d)) != PREPROCESS_STATUS_OK) {
        return status;
    }

    return PREPROCESS_STATUS_OK;
}

preprocess_status_t get_delayed_signal(preprocess_t *preprocess, float *x, float *y, float *z) {
    accel_data_t data_delayed = {.num_samples = 1, .x = x, .y = y, .z = z};

    delay_status_t rc = delay_signal_get_delay_range(&preprocess->delay_signal,
                                 &data_delayed,
                                 FILTER_MEAN_GROUP_DELAY,
                                 1);
    if (rc == DELAY_STATUS_ERROR_DELAY_TOO_LARGE) {
        return PREPROCESS_STATUS_ERROR_BUFFERING;
    } else {
        return check_delay_status(rc);
    }
}

preprocess_status_t push_signal(preprocess_t *preprocess, float *x, float *y, float *z) {
    accel_data_t push_signal = {.num_samples = 1, .x = x, .y = y, .z = z};

    delay_status_t rc = delay_signal_push_signal(&preprocess->delay_signal, &push_signal);

    return check_delay_status(rc);
}

/**
 * @brief Rotates the input signal to align gravity down and removes the gravity component.
 *
 * @param input Input signal
 * @param scratch_buffer Scratch buffer for intermediate results. Must be of
 * the same len as input.
 * @param output Output signal (same len as input)
 *
 * @return preprocess_status_t status
 *
 */
preprocess_status_t gravity_suppress_rotate(preprocess_t *preprocess,
                                            const accel_data_t *input,
                                            accel_data_t *scratch_buffer,
                                            ai_input_data_t *output)
{
    preprocess_status_t status;
    bool buffering = false;

    /// TODO: verify scratch buffer length

    filter_status_t rc = filter_signal(&preprocess->lp_filter,
                                       input,
                                       scratch_buffer);
    if ((status = check_filter_status(rc)) != PREPROCESS_STATUS_OK) {
        return status;
    }

    for (int i = 0; i < input->num_samples; ++i) {
        // Gravity is lp filter output, stored in scratch buffer
        float32_t grav_x = scratch_buffer->x[i];
        float32_t grav_y = scratch_buffer->y[i];
        float32_t grav_z = scratch_buffer->z[i];

        // push signal, get delayed signal (by mean group delay), subtract
        // gravity

        status = push_signal(preprocess, &input->x[i], &input->y[i], &input->z[i]);
        if (status != PREPROCESS_STATUS_OK) {
            return status;
        }

        float32_t delayed_x;
        float32_t delayed_y;
        float32_t delayed_z;
        status = get_delayed_signal(preprocess, &delayed_x, &delayed_y, &delayed_z);
        if (status == PREPROCESS_STATUS_ERROR_BUFFERING) {
            buffering = true;

            // Keep pushing the rest of the signal, but don't process it
            continue;
        } else if (status != PREPROCESS_STATUS_OK) {
            return status;
        }

        float32_t dyn_x = delayed_x - grav_x;
        float32_t dyn_y = delayed_y - grav_y;
        float32_t dyn_z = delayed_z - grav_z;

        float32_t grav_m = grav_x * grav_x + grav_y * grav_y + grav_z * grav_z;
        float32_t sqrt_grav_m = sqrtf(grav_m);
        grav_m = 1.0f / sqrt_grav_m;
        grav_x *= grav_m, grav_y *= grav_m, grav_z *= grav_m;

        // This is the norm of || z x g ||
        // = || g_y^2 + g_x^2 ||, since sqrt(gx^2 + g_y^2 + g_z^2) = 1
        // = sqrt(1-g_z^2)
        float32_t sqrt_for_sin = sqrtf(1.0f - grav_z*grav_z);
        float32_t sin_theta = sqrt_for_sin, cos_theta = -grav_z;

        /* rotation axis: v = [-grav_y, grav_x, 0] / sin */

        float32_t v_x, v_y;
        float32_t v_factor;

        if (sqrt_for_sin != 0) {
            v_x = -grav_y / sin_theta;
            v_y = grav_x / sin_theta;
        } else {
            v_x = 0;
            v_y = 0;
        }

        v_factor = (v_x * dyn_x + v_y * dyn_y) * (1 - cos_theta);

        /*
         * Rodrigues' formula for rotations (a is the dynamic acceleration dyn)
         * a' = a * cos + (v x a) * sin + v * (v . a) * (1 - cos)
         */

        AI_INPUT_GET_X(output->data_array, i)
            = dyn_x * cos_theta + v_y * dyn_z * sin_theta + v_x * v_factor;

        AI_INPUT_GET_Y(output->data_array, i)
            = dyn_y * cos_theta - v_x * dyn_z * sin_theta + v_y * v_factor;

        AI_INPUT_GET_Z(output->data_array, i)
            = dyn_z * cos_theta + (v_x * dyn_y - v_y * dyn_x) * sin_theta;
    }

    if (buffering) {
        return PREPROCESS_STATUS_ERROR_BUFFERING;
    } else {
        return PREPROCESS_STATUS_OK;
    }
}
