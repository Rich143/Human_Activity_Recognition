#include "support/test_helpers.h"
#include "unity.h"
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

// Helper macros for test status checking
#define TEST_PREPROCESS_OK(expr) \
    do { \
        preprocess_status_t status = (expr); \
        TEST_ASSERT_EQUAL_INT_MESSAGE(PREPROCESS_STATUS_OK, status, "Expected PREPROCESS_STATUS_OK"); \
    } while (0)

#define TEST_PREPROCESS_STATUS(expr, expected) \
    do { \
        preprocess_status_t status = (expr); \
        TEST_ASSERT_EQUAL_INT_MESSAGE((expected), status, "Unexpected preprocess_status_t value"); \
    } while (0)

preprocess_t preprocess;

accel_data_t scratch_buffer;

accel_data_t *get_scratch_buffer(uint32_t num_samples) {
    scratch_buffer.x = (float32_t *)malloc(sizeof(float32_t) * num_samples);
    scratch_buffer.y = (float32_t *)malloc(sizeof(float32_t) * num_samples);
    scratch_buffer.z = (float32_t *)malloc(sizeof(float32_t) * num_samples);

    scratch_buffer.num_samples = num_samples;

    return &scratch_buffer;
}

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
    const uint32_t batch_size = 32;
    const uint32_t N = batch_size * 20;

    float32_t x[N] = {0};
    float32_t y[N] = {0};
    float32_t z[N] = {0};

    for (uint32_t i = 0; i < N; i++) {
        z[i] = -1000; // gravity down = -1000 mg (or -1 g)
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

        preprocess_status_t expected_status = PREPROCESS_STATUS_OK;

        const uint32_t num_samples_saved = start + batch_size;
        if (num_samples_saved < FILTER_MEAN_GROUP_DELAY + 1) {
            expected_status = PREPROCESS_STATUS_ERROR_BUFFERING;
        }

        TEST_PREPROCESS_STATUS(gravity_suppress_rotate(&preprocess,
                                                       &input_batch, scratch,
                                                       &output_batch),
                               expected_status);
    }

    uint32_t valid_start = FILTER_MEAN_GROUP_DELAY + 1;
    accel_data_t out_valid = {
        .num_samples = N - valid_start,
        .x = &x_out[valid_start],
        .y = &y_out[valid_start],
        .z = &z_out[valid_start]
    };

    accel_data_t signal_valid = {
        .num_samples = N - valid_start,
        .x = &x[valid_start],
        .y = &y[valid_start],
        .z = &z[valid_start]
    };

    check_signal_close(&out_valid, &signal_valid, 1e-4f);
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
