#include "support/test_helpers.h"
#include "unity.h"

#include "support/walking_gravity_down_input.h"
#include "support/walking_gravity_down_output.h"

#include "support/walking_gravity_up_input.h"
#include "support/walking_gravity_up_output.h"

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

#define DEBUG_DUMP_PREPROCESS_OUTPUT 1
#if DEBUG_DUMP_PREPROCESS_OUTPUT
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
    accel_data_t *input_signal;
    accel_data_t *expected_output_signal;
    accel_data_t *output_signal;
} test_signal_t;

typedef struct {
    accel_data_t *batch_in;
    accel_data_t *batch_out;
} batch_signal_t;

typedef struct {
    accel_data_t *valid_output;
    accel_data_t *valid_expected_output;
} valid_output_t;

preprocess_t preprocess;

accel_data_t scratch_buffer;

test_signal_t test_signals;

batch_signal_t batch_signals;

valid_output_t valid_outputs;

accel_data_t preprocess_output;

accel_data_t *get_scratch_buffer(uint32_t num_samples) {
    scratch_buffer.x = (float32_t *)malloc(sizeof(float32_t) * num_samples);
    scratch_buffer.y = (float32_t *)malloc(sizeof(float32_t) * num_samples);
    scratch_buffer.z = (float32_t *)malloc(sizeof(float32_t) * num_samples);

    scratch_buffer.num_samples = num_samples;

    return &scratch_buffer;
}


test_signal_t *get_test_signals(const float32_t *x_in, const float32_t *y_in, const float32_t *z_in,
                                const float32_t *x_expected, const float32_t *y_expected, const float32_t *z_expected,
                                uint32_t num_samples)
{
    const uint32_t N = num_samples;

    static accel_data_t signal;

    signal.num_samples = N;
    signal.x = x_in;
    signal.y = y_in;
    signal.z = z_in;

    static accel_data_t expected_output;

    expected_output.num_samples = N;
    expected_output.x = x_expected;
    expected_output.y = y_expected;
    expected_output.z = z_expected;

    preprocess_output.x = (float32_t *)malloc(sizeof(float32_t) * N);
    preprocess_output.y = (float32_t *)malloc(sizeof(float32_t) * N);
    preprocess_output.z = (float32_t *)malloc(sizeof(float32_t) * N);

    for (uint32_t i = 0; i < N; ++i) {
        preprocess_output.x[i] = FLT_MAX;
        preprocess_output.y[i] = FLT_MAX;
        preprocess_output.z[i] = FLT_MAX;
    }

    static accel_data_t out;

    out.num_samples = N;
    out.x = preprocess_output.x;
    out.y = preprocess_output.y;
    out.z = preprocess_output.z;

    test_signals.input_signal = &signal;
    test_signals.expected_output_signal = &expected_output;
    test_signals.output_signal = &out;

    return &test_signals;
}

batch_signal_t *get_batch_signals(const accel_data_t *in, accel_data_t *out, uint32_t start, uint32_t batch_size) {
    static accel_data_t input_batch;

    input_batch.num_samples = batch_size;
    input_batch.x = &in->x[start];
    input_batch.y = &in->y[start];
    input_batch.z = &in->z[start];

    static accel_data_t output_batch;

    output_batch.num_samples = batch_size;
    output_batch.x = &out->x[start];
    output_batch.y = &out->y[start];
    output_batch.z = &out->z[start];

    batch_signals.batch_in = &input_batch;
    batch_signals.batch_out = &output_batch;

    return &batch_signals;
}

valid_output_t *get_valid_outputs(const accel_data_t *output, const accel_data_t *expected_output,
                                  uint32_t valid_start, uint32_t N)
{
    static accel_data_t out_valid;

    out_valid.num_samples = N - valid_start;
    out_valid.x = &output->x[valid_start];
    out_valid.y = &output->y[valid_start];
    out_valid.z = &output->z[valid_start];

    static accel_data_t expected_valid;

    expected_valid.num_samples = N - valid_start;
    expected_valid.x = &expected_output->x[valid_start];
    expected_valid.y = &expected_output->y[valid_start];
    expected_valid.z = &expected_output->z[valid_start];

    valid_outputs.valid_output = &out_valid;
    valid_outputs.valid_expected_output = &expected_valid;

    return &valid_outputs;
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

    if (preprocess_output.x != NULL) {
        free(preprocess_output.x);
        free(preprocess_output.y);
        free(preprocess_output.z);
    }
}

void test_dc_signal_gravity_down(void) {
    const uint32_t batch_size = 10;
    const uint32_t N = gravity_down_input_len;

    test_signal_t *test_signal = get_test_signals(gravity_down_input_x,
                                                  gravity_down_input_y,
                                                  gravity_down_input_z,
                                                  gravity_down_output_x,
                                                  gravity_down_output_y,
                                                  gravity_down_output_z,
                                                  N);

    accel_data_t *scratch = get_scratch_buffer(batch_size);

    for (uint32_t start = 0; start < N; start += batch_size) {
        batch_signal_t *batch_signal = get_batch_signals(test_signal->input_signal,
                                                         test_signal->output_signal,
                                                         start,
                                                         batch_size);

        preprocess_status_t status = gravity_suppress_rotate(&preprocess,
                                                             batch_signal->batch_in,
                                                             scratch,
                                                             batch_signal->batch_out);

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

    valid_output_t *valid_outputs = get_valid_outputs(test_signal->output_signal,
                                                      test_signal->expected_output_signal,
                                                      valid_start,
                                                      N);

#if DEBUG_DUMP_PREPROCESS_OUTPUT
    FILE *f = fopen("preprocess_output.py", "w");
    uint32_t print_num_samples = valid_outputs->valid_output->num_samples;
    float32_t *print_x = valid_outputs->valid_output->x;
    float32_t *print_y = valid_outputs->valid_output->y;
    float32_t *print_z = valid_outputs->valid_output->z;

    if (f) {
        fprintf(f, "output_x = [\n");
        for (uint32_t i = 0; i < print_num_samples; ++i) {
            fprintf(f, "    %.7g%s", print_x[i], (i < print_num_samples - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == print_num_samples - 1) fprintf(f, "\n");
        }

        fprintf(f, "]\n");
        fprintf(f, "output_y = [\n");
        for (uint32_t i = 0; i < print_num_samples; ++i) {
            fprintf(f, "    %.7g%s", print_y[i], (i < print_num_samples - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == print_num_samples - 1) fprintf(f, "\n");
        }

        fprintf(f, "]\n");
        fprintf(f, "output_z = [\n");
        for (uint32_t i = 0; i < print_num_samples; ++i) {
            fprintf(f, "    %.7g%s", print_z[i], (i < print_num_samples - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == print_num_samples - 1) fprintf(f, "\n");
        }

        fprintf(f, "]\n");
        fclose(f);
    } else {
        printf("Failed to open gravity_output.py for writing\n");
    }
#endif

    check_signal_close(valid_outputs->valid_output,
                       valid_outputs->valid_expected_output,
                       1e-2f);
}

void test_dc_signal_gravity_up_rotates_down(void) {
    const uint32_t N = gravity_up_input_len;
    const uint32_t batch_size = 10;

    test_signal_t *test_signal = get_test_signals(gravity_up_input_x,
                                                  gravity_up_input_y,
                                                  gravity_up_input_z,
                                                  gravity_up_output_x,
                                                  gravity_up_output_y,
                                                  gravity_up_output_z,
                                                  N);

    accel_data_t *scratch = get_scratch_buffer(N);

    for (uint32_t start = 0; start < N; start += batch_size) {
        batch_signal_t *batch_signal = get_batch_signals(test_signal->input_signal,
                                                         test_signal->output_signal,
                                                         start,
                                                         batch_size);

        preprocess_status_t status = gravity_suppress_rotate(&preprocess,
                                                             batch_signal->batch_in,
                                                             scratch,
                                                             batch_signal->batch_out);

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

    uint32_t valid_start = GRAVITY_UP_OUTPUT_VALID_START_IDX;

    valid_output_t *valid_outputs = get_valid_outputs(test_signal->output_signal,
                                                      test_signal->expected_output_signal,
                                                      valid_start,
                                                      N);

#if DEBUG_DUMP_PREPROCESS_OUTPUT
    FILE *f = fopen("preprocess_output_gravity_up.py", "w");
    uint32_t print_num_samples = valid_outputs->valid_output->num_samples;
    float32_t *print_x = valid_outputs->valid_output->x;
    float32_t *print_y = valid_outputs->valid_output->y;
    float32_t *print_z = valid_outputs->valid_output->z;

    if (f) {
        fprintf(f, "output_x = [\n");
        for (uint32_t i = 0; i < print_num_samples; ++i) {
            fprintf(f, "    %.7g%s", print_x[i], (i < print_num_samples - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == print_num_samples - 1) fprintf(f, "\n");
        }

        fprintf(f, "]\n");
        fprintf(f, "output_y = [\n");
        for (uint32_t i = 0; i < print_num_samples; ++i) {
            fprintf(f, "    %.7g%s", print_y[i], (i < print_num_samples - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == print_num_samples - 1) fprintf(f, "\n");
        }

        fprintf(f, "]\n");
        fprintf(f, "output_z = [\n");
        for (uint32_t i = 0; i < print_num_samples; ++i) {
            fprintf(f, "    %.7g%s", print_z[i], (i < print_num_samples - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == print_num_samples - 1) fprintf(f, "\n");
        }

        fprintf(f, "]\n");

        uint32_t print_expected_num_samples = valid_outputs->valid_output->num_samples;
        float32_t *print_expected_x = valid_outputs->valid_expected_output->x;
        float32_t *print_expected_y = valid_outputs->valid_expected_output->y;
        float32_t *print_expected_z = valid_outputs->valid_expected_output->z;

        fprintf(f, "expected_x = [\n");
        for (uint32_t i = 0; i < print_expected_num_samples; ++i) {
            fprintf(f, "    %.7g%s", print_expected_x[i], (i < print_expected_num_samples - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == print_expected_num_samples - 1) fprintf(f, "\n");
        }

        fprintf(f, "]\n");
        fprintf(f, "expected_y = [\n");
        for (uint32_t i = 0; i < print_expected_num_samples; ++i) {
            fprintf(f, "    %.7g%s", print_expected_y[i], (i < print_expected_num_samples - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == print_expected_num_samples - 1) fprintf(f, "\n");
        }

        fprintf(f, "]\n");
        fprintf(f, "expected_z = [\n");
        for (uint32_t i = 0; i < print_expected_num_samples; ++i) {
            fprintf(f, "    %.7g%s", print_expected_z[i], (i < print_expected_num_samples - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == print_expected_num_samples - 1) fprintf(f, "\n");
        }

        fprintf(f, "]\n");

        fclose(f);
    } else {
        printf("Failed to open gravity_output.py for writing\n");
    }
#endif

    check_signal_close(valid_outputs->valid_output,
                       valid_outputs->valid_expected_output,
                       1e-2f);
}
