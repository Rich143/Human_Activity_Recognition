#include "support/test_helpers.h"
#include "unity.h"

#include "support/walking_gravity_down_input.h"
#include "support/walking_gravity_down_output.h"

#include "accel_data_type.h"
#include "preprocess.h"
#include "filter.h"
#include "delay_signal.h"

#include <float.h>
#include <stdlib.h>

// To get these directives to work:
// - quote the file name in " "
// - Add the directory containing the file to :paths: in project.yml
TEST_SOURCE_FILE("../Drivers/CMSIS-DSP/Source/FilteringFunctions/arm_biquad_cascade_df2T_f32.c")
TEST_SOURCE_FILE("../Drivers/CMSIS-DSP/Source/FilteringFunctions/arm_biquad_cascade_df2T_init_f32.c")

#define DEBUG_DUMP_GRAVITY_OUTPUT 1
#if DEBUG_DUMP_GRAVITY_OUTPUT
#include <stdio.h>
#endif

// Helper macros for test status checking
#define TEST_PREPROCESS_OK(expr) \
    do { \
        preprocess_status_t _status = (expr); \
        TEST_ASSERT_EQUAL_INT_MESSAGE(PREPROCESS_STATUS_OK, _status, "Expected PREPROCESS_STATUS_OK"); \
    } while (0)

#define TEST_PREPROCESS_STATUS(expr, expected) \
    do { \
        preprocess_status_t _status = (expr); \
        TEST_ASSERT_EQUAL_INT_MESSAGE((expected), _status, "Unexpected preprocess_status_t value"); \
    } while (0)

typedef struct {
    accel_data_t *expected_signal_in;
    accel_data_t *expected_signal_out;
    accel_data_t *output_signal;
} test_signal_t;

preprocess_t preprocess;

accel_data_t scratch_buffer;

test_signal_t test_signals;

accel_data_t *get_scratch_buffer(uint32_t num_samples) {
    scratch_buffer.x = (float32_t *)malloc(sizeof(float32_t) * num_samples);
    scratch_buffer.y = (float32_t *)malloc(sizeof(float32_t) * num_samples);
    scratch_buffer.z = (float32_t *)malloc(sizeof(float32_t) * num_samples);

    scratch_buffer.num_samples = num_samples;

    return &scratch_buffer;
}


/*
 *test_signal_t *get_test_signals(float32_t *x_in, float32_t *y_in, float32_t *z_in,
 *                                float32_t *x_out, float32_t *y_out, float32_t *z_out,
 *                                uint32_t num_samples)
 *{
 *    const uint32_t N = num_samples;
 *
 *    float32_t x[N];
 *    float32_t y[N];
 *    float32_t z[N];
 *
 *    for (uint32_t i = 0; i < N; i++) {
 *        x[i] = gravity_down_input_x[i];
 *        y[i] = gravity_down_input_y[i];
 *        z[i] = gravity_down_input_z[i];
 *    }
 *
 *    accel_data_t signal = {
 *        .num_samples = N,
 *        .x = x,
 *        .y = y,
 *        .z = z
 *    };
 *
 *    float x_expected[N];
 *    float y_expected[N];
 *    float z_expected[N];
 *
 *    for (uint32_t i = 0; i < N; ++i) {
 *        x_expected[i] = gravity_down_output_x[i];
 *        y_expected[i] = gravity_down_output_y[i];
 *        z_expected[i] = gravity_down_output_z[i];
 *    }
 *
 *    accel_data_t expected_output = {
 *        .num_samples = N,
 *        .x = x_expected,
 *        .y = y_expected,
 *        .z = z_expected
 *    };
 *
 *    float32_t x_out[N];
 *    float32_t y_out[N];
 *    float32_t z_out[N];
 *
 *    for (uint32_t i = 0; i < N; ++i) {
 *        x_out[i] = FLT_MAX;
 *        y_out[i] = FLT_MAX;
 *        z_out[i] = FLT_MAX;
 *    }
 *
 *    accel_data_t out = {
 *        .num_samples = N,
 *        .x = x_out,
 *        .y = y_out,
 *        .z = z_out
 *    };
 *
 *    test_signals.expected_signal_in = &signal;
 *    test_signals.expected_signal_out = &expected_output;
 *    test_signals.output_signal = &out;
 *
 *    return &test_signals;
 *}
 *
 */
void setUp(void) {
    TEST_PREPROCESS_OK(preprocess_init(&preprocess));
}

void tearDown(void) {
    if (scratch_buffer.x != NULL) {
        free(scratch_buffer.x);
        free(scratch_buffer.y);
        free(scratch_buffer.z);
    }
}

void test_dc_signal_gravity_down(void) {
    const uint32_t batch_size = 10;
    const uint32_t N = gravity_down_input_len;

    float32_t x[N];
    float32_t y[N];
    float32_t z[N];

    for (uint32_t i = 0; i < N; i++) {
        x[i] = gravity_down_input_x[i];
        y[i] = gravity_down_input_y[i];
        z[i] = gravity_down_input_z[i];
    }

    accel_data_t signal = {
        .num_samples = N,
        .x = x,
        .y = y,
        .z = z
    };

    float x_expected[N];
    float y_expected[N];
    float z_expected[N];

    for (uint32_t i = 0; i < N; ++i) {
        x_expected[i] = gravity_down_output_x[i];
        y_expected[i] = gravity_down_output_y[i];
        z_expected[i] = gravity_down_output_z[i];
    }

    accel_data_t expected_output = {
        .num_samples = N,
        .x = x_expected,
        .y = y_expected,
        .z = z_expected
    };

    float32_t x_out[N];
    float32_t y_out[N];
    float32_t z_out[N];

    for (uint32_t i = 0; i < N; ++i) {
        x_out[i] = FLT_MAX;
        y_out[i] = FLT_MAX;
        z_out[i] = FLT_MAX;
    }

    accel_data_t out = {
        .num_samples = N,
        .x = x_out,
        .y = y_out,
        .z = z_out
    };

    accel_data_t *scratch = get_scratch_buffer(batch_size);

    for (uint32_t start = 0; start < N; start += batch_size) {
        accel_data_t input_batch = {
            .num_samples = batch_size,
            .x = &x[start],
            .y = &y[start],
            .z = &z[start]
        };

        accel_data_t output_batch = {
            .num_samples = batch_size,
            .x = &x_out[start],
            .y = &y_out[start],
            .z = &z_out[start]
        };

        preprocess_status_t status = gravity_suppress_rotate(&preprocess,
                                                             &input_batch,
                                                             scratch,
                                                             &output_batch);

        if (start < FILTER_MEAN_GROUP_DELAY + 1) {
            if (!(status == PREPROCESS_STATUS_ERROR_BUFFERING || status == PREPROCESS_STATUS_OK)) {
                char message[128];

                sprintf(message, "Unexpected preprocess_status_t value: %d", status);
                TEST_FAIL_MESSAGE(message);
            }
        } else {
            TEST_PREPROCESS_OK(status);
        }
    }

    uint32_t valid_start = GRAVITY_DOWN_OUTPUT_VALID_START_IDX;
    accel_data_t out_valid = {
        .num_samples = N - valid_start,
        .x = &x_out[valid_start],
        .y = &y_out[valid_start],
        .z = &z_out[valid_start]
    };

    accel_data_t expected_valid = {
        .num_samples = N - valid_start,
        .x = &x_expected[valid_start],
        .y = &y_expected[valid_start],
        .z = &z_expected[valid_start]
    };

#if DEBUG_DUMP_GRAVITY_OUTPUT
    FILE *f = fopen("gravity_output.py", "w");
    if (f) {
        fprintf(f, "gravity_x = [\n");
        for (uint32_t i = 0; i < out_valid.num_samples; ++i) {
            fprintf(f, "    %.7g%s", out_valid.x[i], (i < out_valid.num_samples - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == out_valid.num_samples - 1) fprintf(f, "\n");
        }

        fprintf(f, "]\n\ngravity_y = [\n");
        for (uint32_t i = 0; i < out_valid.num_samples; ++i) {
            fprintf(f, "    %.7g%s", out_valid.y[i], (i < out_valid.num_samples - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == out_valid.num_samples - 1) fprintf(f, "\n");
        }

        fprintf(f, "]\n\ngravity_z = [\n");
        for (uint32_t i = 0; i < out_valid.num_samples; ++i) {
            fprintf(f, "    %.7g%s", out_valid.z[i], (i < out_valid.num_samples - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == out_valid.num_samples - 1) fprintf(f, "\n");
        }

        fprintf(f, "]\n");
        fclose(f);
    } else {
        printf("Failed to open gravity_output.py for writing\n");
    }
#endif

    check_signal_close(&out_valid, &expected_valid, 1e-2f);
}

void test_dc_signal_gravity_up_rotates_down(void) {
    const uint32_t N = 100;

    float32_t x[N] = {0};
    float32_t y[N] = {0};
    float32_t z[N] = {0};

    for (uint32_t i = 0; i < N; i++) {
        z[i] = 1000; // 1g up
    }

    accel_data_t signal = {
        .num_samples = N,
        .x = x,
        .y = y,
        .z = z
    };

    float32_t x_out[N];
    float32_t y_out[N];
    float32_t z_out[N];

    for (uint32_t i = 0; i < N; ++i) {
        x_out[i] = FLT_MAX;
        y_out[i] = FLT_MAX;
        z_out[i] = FLT_MAX;
    }

    accel_data_t out = {
        .num_samples = N,
        .x = x_out,
        .y = y_out,
        .z = z_out
    };

    float32_t x_out_expected[N];
    float32_t y_out_expected[N];
    float32_t z_out_expected[N];

    for (uint32_t i = 0; i < N; ++i) {
        x_out_expected[i] = 0;
        y_out_expected[i] = 0;
        z_out_expected[i] = -1000; // 1g down
    }

    accel_data_t expected = {
        .num_samples = N,
        .x = x_out_expected,
        .y = y_out_expected,
        .z = z_out_expected
    };

    accel_data_t *scratch = get_scratch_buffer(N);
    TEST_PREPROCESS_OK(gravity_suppress_rotate(&preprocess,
                                               &signal,
                                               scratch,
                                               &out));

    check_signal_close(&out, &expected, 1e-4f);
}
