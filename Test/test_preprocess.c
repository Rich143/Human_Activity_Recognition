#include "support/test_helpers.h"
#include "unity.h"

#include "support/walking_gravity_down_input.h"
#include "support/walking_gravity_down_output.h"

#include "support/walking_gravity_up_input.h"
#include "support/walking_gravity_up_output.h"

#include "support/walking_slowly_rotating_input.h"
#include "support/walking_slowly_rotating_output.h"

#include "support/walking_capture_input.h"
#include "support/walking_capture_output.h"

#include "support/test_signal_helpers.h"

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

preprocess_t preprocess;

void setUp(void) {
    test_signal_setUp();
    TEST_PREPROCESS_OK(preprocess_init(&preprocess));
}

void tearDown(void) {
    test_signal_tearDown();
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
                                                             &batch_signal->batch_out);

        copy_output_batch(batch_signal);

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
#if 0
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
                                                             &batch_signal->batch_out);

        copy_output_batch(batch_signal);

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
#if 0
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
#endif

    check_signal_close(valid_outputs->valid_output,
                       valid_outputs->valid_expected_output,
                       1e-2f);
}

void test_slowly_rotating_gravity(void) {
    const uint32_t N = slowly_rotating_input_len;
    const uint32_t batch_size = 10;

    test_signal_t *test_signal = get_test_signals(slowly_rotating_input_x,
                                                  slowly_rotating_input_y,
                                                  slowly_rotating_input_z,
                                                  slowly_rotating_output_x,
                                                  slowly_rotating_output_y,
                                                  slowly_rotating_output_z,
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
                                                             &batch_signal->batch_out);

        copy_output_batch(batch_signal);

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

    uint32_t valid_start = SLOWLY_ROTATING_OUTPUT_VALID_START_IDX;

    valid_output_t *valid_outputs = get_valid_outputs(test_signal->output_signal,
                                                      test_signal->expected_output_signal,
                                                      valid_start,
                                                      N);

#if DEBUG_DUMP_PREPROCESS_OUTPUT
#if 0
    FILE *f = fopen("preprocess_output_slowly_rotating.py", "w");
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
        printf("Failed to open preprocess_output_slowly_rotating.py for writing\n");
    }
#endif
#endif

    check_signal_close(valid_outputs->valid_output,
                       valid_outputs->valid_expected_output,
                       1e-2f);
}

void test_real_signal(void) {
    const uint32_t N = walking_capture_input_len;
    const uint32_t batch_size = 10;

    test_signal_t *test_signal = get_test_signals(walking_capture_input_x,
                                                  walking_capture_input_y,
                                                  walking_capture_input_z,
                                                  walking_capture_output_x,
                                                  walking_capture_output_y,
                                                  walking_capture_output_z,
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
                                                             &batch_signal->batch_out);

        copy_output_batch(batch_signal);

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

    uint32_t valid_start = SLOWLY_ROTATING_OUTPUT_VALID_START_IDX;

    valid_output_t *valid_outputs = get_valid_outputs(test_signal->output_signal,
                                                      test_signal->expected_output_signal,
                                                      valid_start,
                                                      N);

#if DEBUG_DUMP_PREPROCESS_OUTPUT
    FILE *f = fopen("preprocess_output_walking_capture.py", "w");
    uint32_t print_num_samples = walking_capture_input_len;
    float32_t *print_input_x = walking_capture_input_x;
    float32_t *print_input_y = walking_capture_input_y;
    float32_t *print_input_z = walking_capture_input_z;

    float32_t *print_x = valid_outputs->valid_output->x;
    float32_t *print_y = valid_outputs->valid_output->y;
    float32_t *print_z = valid_outputs->valid_output->z;

    if (f) {
        fprintf(f, "input_x = [\n");
        for (uint32_t i = 0; i < print_num_samples; ++i) {
            fprintf(f, "    %.7g%s", print_input_x[i], (i < print_num_samples - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == print_num_samples - 1) fprintf(f, "\n");
        }

        fprintf(f, "]\n");
        fprintf(f, "input_y = [\n");
        for (uint32_t i = 0; i < print_num_samples; ++i) {
            fprintf(f, "    %.7g%s", print_input_y[i], (i < print_num_samples - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == print_num_samples - 1) fprintf(f, "\n");
        }

        fprintf(f, "]\n");
        fprintf(f, "input_z = [\n");
        for (uint32_t i = 0; i < print_num_samples; ++i) {
            fprintf(f, "    %.7g%s", print_input_z[i], (i < print_num_samples - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == print_num_samples - 1) fprintf(f, "\n");
        }

        fprintf(f, "]\n");

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
        printf("Failed to open preprocess_output_walking_capture.py for writing\n");
    }
#endif

    check_signal_close(valid_outputs->valid_output,
                       valid_outputs->valid_expected_output,
                       1e-2f);
}
