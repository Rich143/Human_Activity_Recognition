/*
 * preprocess.c
 *
 *  Created on: June 22, 2025
 *      Author: richardmatthews
 */

#include "accel_data_type.h"
#include "arm_math.h"
#include "delay_signal.h"
#include "filter.h"
#include <math.h>
#include <stdio.h>

#include "preprocess.h"

/*void delay_signal(const float *input, float *output, int num_samples, int delay_samples, int num_axes) {*/
    /*for (int axis = 0; axis < num_axes; ++axis) {*/
        /*for (int i = 0; i < delay_samples; ++i) {*/
            /*output[i * num_axes + axis] = NAN;*/
        /*}*/
        /*for (int i = delay_samples; i < num_samples; ++i) {*/
            /*output[i * num_axes + axis] = input[(i - delay_samples) * num_axes + axis];*/
        /*}*/
    /*}*/
/*}*/

/*#define NUM_SECTIONS 3   // Match your Python sos shape*/
/*#define NUM_AXES     3   // X, Y, Z*/
/*#define BLOCK_SIZE   512 // Number of samples (adjust as needed)*/

/*// Filter mean group delay*/
/*#define MEAN_GROUP_DELAY 270*/

/*// Coefficients: 5 floats per section: {b0, b1, b2, a1, a2} for each biquad*/
/*float biquad_coeffs[NUM_SECTIONS * 5] = {*/
    /*// Paste values here from your Python-generated SOS output*/
    /*// For example:*/
    /*// 0.0176, 0.0351, 0.0176, -1.6070, 0.6597,*/
    /*// ...*/
/*};*/

/*// State buffers for each axis*/
/*float biquad_state_x[2 * NUM_SECTIONS] = {0};*/
/*float biquad_state_y[2 * NUM_SECTIONS] = {0};*/
/*float biquad_state_z[2 * NUM_SECTIONS] = {0};*/

/*// Filter instances for each axis*/
/*arm_biquad_cascade_df2T_instance_f32 filter_x;*/
/*arm_biquad_cascade_df2T_instance_f32 filter_y;*/
/*arm_biquad_cascade_df2T_instance_f32 filter_z;*/

/*// Initialize filters (call once)*/
/*void init_filters() {*/
    /*arm_biquad_cascade_df2T_init_f32(&filter_x, NUM_SECTIONS, biquad_coeffs, biquad_state_x);*/
    /*arm_biquad_cascade_df2T_init_f32(&filter_y, NUM_SECTIONS, biquad_coeffs, biquad_state_y);*/
    /*arm_biquad_cascade_df2T_init_f32(&filter_z, NUM_SECTIONS, biquad_coeffs, biquad_state_z);*/
/*}*/

/*// Filter 3-axis signal in-place*/
/*void lowpass_filter_3axis(const AccelData *input, AccelData *output) {*/
    /*if (input->num_samples != output->num_samples) {*/
        /*printf("Error: Input and output signal lengths must match.\n");*/
        /*return;*/
    /*}*/

    /*arm_biquad_cascade_df2T_f32(&filter_x, input->x, output->x, input->num_samples);*/
    /*arm_biquad_cascade_df2T_f32(&filter_y, input->y, output->y, input->num_samples);*/
    /*arm_biquad_cascade_df2T_f32(&filter_z, input->z, output->z, input->num_samples);*/
/*}*/

/*void colwise_dot(const float *a, const float *b, float *out_dot, int num_samples, int num_axes) {*/
    /*for (int i = 0; i < num_samples; ++i) {*/
        /*float dot = 0.0f;*/
        /*for (int j = 0; j < num_axes; ++j) {*/
            /*dot += a[i * num_axes + j] * b[i * num_axes + j];*/
        /*}*/
        /*out_dot[i] = dot;*/
    /*}*/
/*}*/

/*float data_g_x[BLOCK_SIZE];*/
/*float data_g_y[BLOCK_SIZE];*/
/*float data_g_z[BLOCK_SIZE];*/

/*AccelData data_g = {*/
    /*.num_samples = BLOCK_SIZE,*/
    /*.x = data_g_x,*/
    /*.y = data_g_y,*/
    /*.z = data_g_z*/
/*};*/

/*void gravity_rotation(const AccelData *data, AccelData *rotated_out) {*/
    /*float data_dyn[BLOCK_SIZE * 3];*/
    /*float data_g_delayed[BLOCK_SIZE * 3];*/

    /*// 1. Filter input to get gravity component*/
    /*lowpass_filter_3axis(data, &data_g);*/

    /*// 2. Delay gravity to align with dynamic component*/
    /*delay_signal(data_g, data_g_delayed, num_samples, mean_group_delay, num_axes);*/

    /*// 3. Subtract to get dynamic component*/
    /*for (int i = 0; i < num_samples * num_axes; ++i) {*/
        /*if (isnan(data_g_delayed[i])) {*/
            /*data_dyn[i] = NAN;*/
        /*} else {*/
            /*data_dyn[i] = data[i] - data_g_delayed[i];*/
        /*}*/
    /*}*/

    /*// 4. Find valid_start (first non-NaN sample)*/
    /*int valid_start = 0;*/
    /*while (valid_start < num_samples && isnan(data_dyn[valid_start * num_axes])) {*/
        /*++valid_start;*/
    /*}*/

    /*// 5. Normalize gravity vector*/
    /*float dot[BLOCK_SIZE];*/
    /*colwise_dot(&data_g[valid_start * num_axes], &data_g[valid_start * num_axes], dot, num_samples - valid_start, num_axes);*/
    /*for (int i = valid_start; i < num_samples; ++i) {*/
        /*float norm = sqrtf(dot[i - valid_start]);*/
        /*for (int j = 0; j < num_axes; ++j) {*/
            /*data_g[i * num_axes + j] /= norm;*/
        /*}*/
    /*}*/

    /*// 6. Rotate dynamic vector to align gravity with Z (cross/cos/sin etc.)*/
    /*// TO-DO: implement Rodrigues' rotation formula or quaternion rotation*/

    /*// For now: output the normalized dynamic component*/
    /*for (int i = valid_start * num_axes; i < num_samples * num_axes; ++i) {*/
        /*rotated_out[i - valid_start * num_axes] = data_dyn[i];*/
    /*}*/
/*}*/

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

/**
 * @brief Subtracts the gravity component from the input signal.
 *
 * @param data_g Gravity component
 * @param signal_delayed Signal delayed by mean group delay of lowpass filter
 * @param[out] data_dyn Dynamic component
 *
 * @return preprocess_status_t status
 */
/*preprocess_status_t get_dynamic_component(accel_data_t *data_g,*/
                                          /*accel_data_t *signal_delayed,*/
                                          /*accel_data_t *data_dyn)*/
/*{*/
    /*if (data_g == NULL || signal_delayed == NULL || data_dyn == NULL) {*/
        /*return PREPROCESS_STATUS_ERROR_NULL;*/
    /*}*/

    /*for (uint32_t i = 0; i < data_g->num_samples; ++i) {*/
        /*data_dyn->x[i] = signal_delayed->x[i] - data_g->x[i];*/
        /*data_dyn->y[i] = signal_delayed->y[i] - data_g->y[i];*/
        /*data_dyn->z[i] = signal_delayed->z[i] - data_g->z[i];*/
    /*}*/

    /*return PREPROCESS_STATUS_OK;*/
/*}*/

// GD = 10, BS = 5
// B 1: sample idxs: 0..4, buffering
// B 2: sample idxs: 5..9, buffering (need GD + 1 samples)
// B 3: sample idxs: 10..14, delay idxs: 10-10 = 0 .. 14-10 = 4
//                           delay amts: delay + BS = 14 .. delay = 10
//
/*preprocess_status_t get_delayed_signal(preprocess_t *preprocess, uint32_t batch_idx, float *x, float *y, float *z) {*/
    /*accel_data_t data_delayed = {.num_samples = 1, .x = x, .y = y, .z = z};*/

    /*delay_status_t rc = delay_signal_get_delay_range(&preprocess->delay_signal,*/
                                 /*&data_delayed,*/
                                 /*FILTER_MEAN_GROUP_DELAY - batch_idx,*/
                                 /*1);*/
    /*if (rc == DELAY_STATUS_ERROR_DELAY_TOO_LARGE) {*/
        /*return PREPROCESS_STATUS_ERROR_BUFFERING;*/
    /*} else {*/
        /*return check_delay_status(rc);*/
    /*}*/
/*}*/

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
                                            accel_data_t *output)
{
    preprocess_status_t status;

    /// TODO: verify scratch buffer length

    const uint32_t batch_size = input->num_samples;

    float x[batch_size];
    float y[batch_size];
    float z[batch_size];
    accel_data_t data_dyn = {.num_samples = batch_size, .x = x, .y = y, .z = z}; // TODO Change

    filter_status_t rc = filter_signal(&preprocess->lp_filter,
                                       input,
                                       scratch_buffer);
    if ((status = check_filter_status(rc)) != PREPROCESS_STATUS_OK) {
        return status;
    }

    // TODO: maybe should push element by element, and pop like
    // that. That would save needing GROUP DELAY + batch size space in the delay
    // buffer
    /*delay_status_t rc_d = delay_signal_push_signal(&preprocess->delay_signal,*/
                                                   /*input);*/
    /*if ((status = check_delay_status(rc_d)) != PREPROCESS_STATUS_OK) {*/
        /*return status;*/
    /*}*/

    for (int i = 0; i < input->num_samples; ++i) {
        // Gravity is lp filter output, stored in scratch buffer
        float32_t grav_x = scratch_buffer->x[i];
        float32_t grav_y = scratch_buffer->y[i];
        float32_t grav_z = scratch_buffer->z[i];

        // push signal, get delayed signal (by mean group delay), subtract
        // gravity

        push_signal(preprocess, &input->x[i], &input->y[i], &input->z[i]);

        float delayed_x;
        float delayed_y;
        float delayed_z;
        status = get_delayed_signal(preprocess, &delayed_x, &delayed_y, &delayed_z);
        if (status != PREPROCESS_STATUS_OK) {
            return status;
        }

        float32_t dyn_x = delayed_x - grav_x;
        float32_t dyn_y = delayed_y - grav_y;
        float32_t dyn_z = delayed_z - grav_z;

        float32_t grav_m = grav_x * grav_x + grav_y * grav_y + grav_z * grav_z;
        float32_t sqrt_grav_m = sqrtf(grav_m);
        grav_m = 1.0f / sqrt_grav_m;
        grav_x *= grav_m, grav_y *= grav_m, grav_z *= grav_m;

        float32_t sqrt_for_sin = sqrtf(1.0f - grav_z*grav_z);
        float32_t sin_theta = sqrt_for_sin, cos_theta = -grav_z;

        /* rotation axis: v = [-grav_y, grav_x, 0] / sin */
        float32_t v_x = -grav_y / sin_theta, v_y = grav_x / sin_theta;
        float32_t v_factor = (v_x * dyn_x + v_y * dyn_y) * (1 - cos_theta);

        /*
         * Rodrigues' formula for rotations (a is the dynamic acceleration dyn)
         * a' = a * cos + (v x a) * sin + v * (v . a) * (1 - cos)
         */
        output->x[i] = dyn_x * cos_theta + v_y * dyn_z * sin_theta + v_x * v_factor;
        output->y[i] = dyn_y * cos_theta - v_x * dyn_z * sin_theta + v_y * v_factor;
        output->z[i] = dyn_z * cos_theta + (v_x * dyn_y - v_y * dyn_x) * sin_theta;
    }

    return PREPROCESS_STATUS_OK;
}
