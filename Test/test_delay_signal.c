#include "Unity/src/unity.h"
#include "accel_data_type.h"
#include "unity.h"
#include "delay_signal.h"
#include "test_helpers.h"

#include "support/imu_test_data.h"

#include "support/test_signal_helpers.h"

// Helper macros for test status checking
#define TEST_DELAY_OK(expr) \
    do { \
        delay_status_t status = (expr); \
        TEST_ASSERT_EQUAL_INT_MESSAGE(DELAY_STATUS_OK, status, "Expected DELAY_STATUS_OK"); \
    } while (0)

#define TEST_DELAY_STATUS(expr, expected) \
    do { \
        delay_status_t status = (expr); \
        TEST_ASSERT_EQUAL_INT_MESSAGE((expected), status, "Unexpected delay_status_t value"); \
    } while (0)

delay_signal_t delay_signal;

void setUp(void) {
    TEST_DELAY_OK(delay_signal_init(&delay_signal));
}
void tearDown(void) {}

void test_delay_init_fail(void) {
    TEST_DELAY_STATUS(delay_signal_init(NULL), DELAY_STATUS_ERROR_NULL);
}

void check_signal_single_value(accel_data_t *signal, uint32_t index, float x, float y, float z) {
    TEST_ASSERT_FLOAT_WITHIN(0.001, x, signal->x[index]);
    TEST_ASSERT_FLOAT_WITHIN(0.001, y, signal->y[index]);
    TEST_ASSERT_FLOAT_WITHIN(0.001, z, signal->z[index]);
}

void test_push_signal_null(void) {
    TEST_DELAY_STATUS(delay_signal_push_signal(NULL,
                                               NULL),
                      DELAY_STATUS_ERROR_NULL);

    TEST_DELAY_STATUS(delay_signal_push_signal(&delay_signal,
                                               NULL),
                      DELAY_STATUS_ERROR_NULL);

    TEST_DELAY_STATUS(delay_signal_get_delay_range(NULL,
                                                   NULL,
                                                   0, 0),
                      DELAY_STATUS_ERROR_NULL);

    TEST_DELAY_STATUS(delay_signal_get_delay_range(&delay_signal,
                                                   NULL,
                                                   0, 0),
                      DELAY_STATUS_ERROR_NULL);
}

void test_delay_single_value(void) {
    float x = 1.1f;
    float y = 2.1f;
    float z = 3.1f;

    float x_out = 0;
    float y_out = 0;
    float z_out = 0;

    accel_data_t input = {
        .num_samples = 1,
        .x = &x,
        .y = &y,
        .z = &z
    };

    accel_data_t delayed_signal = {
        .num_samples = 1,
        .x = &x_out,
        .y = &y_out,
        .z = &z_out
    };

    TEST_DELAY_OK(delay_signal_push_signal(&delay_signal,
                                           &input));


    TEST_DELAY_OK(delay_signal_get_delay_range(&delay_signal,
                                               &delayed_signal,
                                               0, 1));

    check_signal_single_value(&delayed_signal, 0, x, y, z);
}

void test_delay_single_old_value(void) {
    const uint32_t BUF_SIZE = 2;

    float x[BUF_SIZE] = {1.0f, 1.1f};
    float y[BUF_SIZE] = {2.0f, 2.1f};
    float z[BUF_SIZE] = {3.0f, 3.1f};

    float x_out = 0;
    float y_out = 0;
    float z_out = 0;

    accel_data_t input = {
        .num_samples = BUF_SIZE,
        .x = x,
        .y = y,
        .z = z
    };

    accel_data_t delayed_signal = {
        .num_samples = 1,
        .x = &x_out,
        .y = &y_out,
        .z = &z_out
    };

    TEST_DELAY_OK(delay_signal_push_signal(&delay_signal,
                                           &input));


    TEST_DELAY_OK(delay_signal_get_delay_range(&delay_signal,
                                               &delayed_signal,
                                               1, 1));

    check_signal_single_value(&delayed_signal, 0, x[0], y[0], z[0]);
}

void test_delay_multiple_values(void) {
    const uint32_t BUF_SIZE = 2;

    float x[BUF_SIZE] = {1.0f, 1.1f};
    float y[BUF_SIZE] = {2.0f, 2.1f};
    float z[BUF_SIZE] = {3.0f, 3.1f};

    float x_out[BUF_SIZE] = {0};
    float y_out[BUF_SIZE] = {0};
    float z_out[BUF_SIZE] = {0};

    accel_data_t input = {
        .num_samples = BUF_SIZE,
        .x = x,
        .y = y,
        .z = z
    };

    accel_data_t delayed_signal = {
        .num_samples = 2,
        .x = x_out,
        .y = y_out,
        .z = z_out
    };

    TEST_DELAY_OK(delay_signal_push_signal(&delay_signal,
                                           &input));


    TEST_DELAY_OK(delay_signal_get_delay_range(&delay_signal,
                                               &delayed_signal,
                                               1, 2));

    check_signal(&delayed_signal, &input);
}

void test_delay_too_large(void) {
    const uint32_t num_samples = DELAY_BUFFER_SIZE;

    float x[DELAY_BUFFER_SIZE] = {0};
    float y[DELAY_BUFFER_SIZE] = {0};
    float z[DELAY_BUFFER_SIZE] = {0};

    // Fill input with dummy values
    for (uint32_t i = 0; i < DELAY_BUFFER_SIZE; ++i) {
        x[i] = 1.0f + i;
        y[i] = 2.0f + i;
        z[i] = 3.0f + i;
    }

    float x_out[DELAY_BUFFER_SIZE] = {0};
    float y_out[DELAY_BUFFER_SIZE] = {0};
    float z_out[DELAY_BUFFER_SIZE] = {0};

    accel_data_t input = {
        .num_samples = num_samples,
        .x = x,
        .y = y,
        .z = z
    };

    accel_data_t delayed_signal = {
        .num_samples = 1,
        .x = x_out,
        .y = y_out,
        .z = z_out
    };

    // Push all input samples into the delay buffer
    TEST_DELAY_OK(delay_signal_push_signal(&delay_signal, &input));

    // Request a delay equal to DELAY_MAX_ALLOWED_DELAY + 1 (out of bounds)
    TEST_DELAY_STATUS(delay_signal_get_delay_range(&delay_signal,
                                                   &delayed_signal,
                                                   DELAY_MAX_ALLOWED_DELAY + 1,
                                                   1),
                      DELAY_STATUS_ERROR_DELAY_TOO_LARGE);
}

void test_delay_insufficient_history(void) {
    const uint32_t num_samples_pushed = DELAY_BUFFER_SIZE - 1;
    const uint32_t delay_requested = DELAY_MAX_ALLOWED_DELAY;

    float x[DELAY_BUFFER_SIZE] = {0};
    float y[DELAY_BUFFER_SIZE] = {0};
    float z[DELAY_BUFFER_SIZE] = {0};

    for (uint32_t i = 0; i < num_samples_pushed; ++i) {
        x[i] = 1.0f + i;
        y[i] = 2.0f + i;
        z[i] = 3.0f + i;
    }

    float x_out = 0.0f;
    float y_out = 0.0f;
    float z_out = 0.0f;

    accel_data_t input = {
        .num_samples = num_samples_pushed,
        .x = x,
        .y = y,
        .z = z
    };

    accel_data_t delayed_signal = {
        .num_samples = 1,
        .x = &x_out,
        .y = &y_out,
        .z = &z_out
    };

    TEST_DELAY_OK(delay_signal_push_signal(&delay_signal, &input));

    // Requesting a delay that is within DELAY_MAX_ALLOWED_DELAY but larger than actual samples pushed
    TEST_DELAY_STATUS(delay_signal_get_delay_range(&delay_signal,
                                                   &delayed_signal,
                                                   delay_requested,
                                                   1),
                      DELAY_STATUS_ERROR_DELAY_TOO_LARGE);
}

void test_delay_full_length_max_delay(void) {
    const uint32_t MAX = DELAY_BUFFER_SIZE;

    float x[MAX], y[MAX], z[MAX];
    float x_out[MAX], y_out[MAX], z_out[MAX];

    for (uint32_t i = 0; i < MAX; ++i) {
        x[i] = 1.0f + i;
        y[i] = 2.0f + i;
        z[i] = 3.0f + i;
    }

    accel_data_t input = {
        .num_samples = MAX,
        .x = x,
        .y = y,
        .z = z
    };

    accel_data_t full_out = {
        .num_samples = MAX,
        .x = x_out,
        .y = y_out,
        .z = z_out
    };

    TEST_DELAY_OK(delay_signal_push_signal(&delay_signal, &input));
    TEST_DELAY_OK(delay_signal_get_delay_range(&delay_signal, &full_out, DELAY_MAX_ALLOWED_DELAY, MAX));

    check_signal(&full_out, &input);
}

void test_delay_partial_window(void) {
    const uint32_t MAX = DELAY_BUFFER_SIZE;
    const uint32_t delay = MAX * 4 / 5;
    const uint32_t len = delay * 5 / 6;

    float x[MAX], y[MAX], z[MAX];
    float x_out[len] = {0}, y_out[len] = {0}, z_out[len] = {0};

    for (uint32_t i = 0; i < MAX; ++i) {
        x[i] = 1.0f + i;
        y[i] = 2.0f + i;
        z[i] = 3.0f + i;
    }

    accel_data_t input = {
        .num_samples = MAX,
        .x = x,
        .y = y,
        .z = z
    };

    accel_data_t partial_out = {
        .num_samples = len,
        .x = x_out,
        .y = y_out,
        .z = z_out
    };

    const uint32_t expected_start_index = MAX - delay - 1;
    accel_data_t expected_output = {
        .num_samples = len,
        .x = &x[expected_start_index],
        .y = &y[expected_start_index],
        .z = &z[expected_start_index]
    };

    TEST_DELAY_OK(delay_signal_push_signal(&delay_signal, &input));
    TEST_DELAY_OK(delay_signal_get_delay_range(&delay_signal, &partial_out, delay, len));


    check_signal(&partial_out, &expected_output);
}

void test_delay_max_len_plus_1(void) {
    float x[DELAY_BUFFER_SIZE], y[DELAY_BUFFER_SIZE], z[DELAY_BUFFER_SIZE];
    float x_out = 0, y_out = 0, z_out = 0;

    for (uint32_t i = 0; i < DELAY_BUFFER_SIZE; ++i) {
        x[i] = 1.0f + i;
        y[i] = 2.0f + i;
        z[i] = 3.0f + i;
    }

    accel_data_t input = {
        .num_samples = DELAY_BUFFER_SIZE,
        .x = x,
        .y = y,
        .z = z
    };

    accel_data_t delayed = {
        .num_samples = 1,
        .x = &x_out,
        .y = &y_out,
        .z = &z_out
    };

    TEST_DELAY_OK(delay_signal_push_signal(&delay_signal, &input));

    // This should fail because delay = DELAY_MAX_ALLOWED_DELAY + 1 (too large)
    TEST_DELAY_STATUS(delay_signal_get_delay_range(&delay_signal,
                                                   &delayed,
                                                   DELAY_MAX_ALLOWED_DELAY + 1,
                                                   1),
                      DELAY_STATUS_ERROR_DELAY_TOO_LARGE);
}

void test_real_signal(void) {
    const int batch_size = 24;
    const int delay_amount = 270;

    TEST_DELAY_OK(delay_signal_init(&delay_signal));

    test_signal_t *test_signal = get_test_signals(unproc_x,
                                                  unproc_y,
                                                  unproc_z,
                                                  unproc_x,
                                                  unproc_y,
                                                  unproc_z,
                                                  IMU_SIGNAL_LENGTH);

    for (uint32_t i = 0; i < IMU_SIGNAL_LENGTH; i += batch_size) {
        /*printf("Testing batch %d\n", i / batch_size);*/
        accel_data_t input = {
            .num_samples = batch_size,
            .x = &(test_signal->input_signal->x[i]),
            .y = &(test_signal->input_signal->y[i]),
            .z = &(test_signal->input_signal->z[i])
        };

        accel_data_t batch_output = {
            .num_samples = batch_size,
            .x = &(test_signal->output_signal->x[i]),
            .y = &(test_signal->output_signal->y[i]),
            .z = &(test_signal->output_signal->z[i])
        };

        int end_input_idx = i + batch_size;
        int output_idx = end_input_idx - delay_amount - 1;
        /*printf("Input end idx %d, Output idx %d\n", end_input_idx, output_idx);*/
        accel_data_t expected_output = {
            .num_samples = batch_size,
            .x = &(test_signal->input_signal->x[output_idx]),
            .y = &(test_signal->input_signal->y[output_idx]),
            .z = &(test_signal->input_signal->z[output_idx])
        };

        TEST_DELAY_OK(delay_signal_push_signal(&delay_signal, &input));

        delay_status_t status = delay_signal_get_delay_range(&delay_signal, &batch_output, delay_amount, batch_size);
        if (i <= delay_amount) {
            continue;
        } else {
            TEST_ASSERT_EQUAL_INT_MESSAGE(DELAY_STATUS_OK, status, "Unexpected delay_status_t value"); \
        }


        check_signal(&batch_output, &expected_output);
    }



}
