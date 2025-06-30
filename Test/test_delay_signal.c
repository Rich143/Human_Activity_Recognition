
#include "preprocess.h"
#include "unity.h"
#include "delay_signal.h"

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

void setUp(void) {}
void tearDown(void) {}

delay_signal_t delay_signal;

void test_delay_init(void) {
    TEST_DELAY_OK(delay_signal_init(&delay_signal));
}

void test_delay_init_fail(void) {
    TEST_DELAY_STATUS(delay_signal_init(NULL), DELAY_STATUS_ERROR_NULL);
}

void check_signal(const AccelData *actual, const AccelData *expected) {
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(expected->num_samples, actual->num_samples,
                                     "Sample count mismatch");

    for (uint32_t i = 0; i < expected->num_samples; ++i) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, expected->x[i], actual->x[i]);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, expected->y[i], actual->y[i]);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, expected->z[i], actual->z[i]);
    }
}

void check_signal_single_value(AccelData *signal, uint32_t index, float x, float y, float z) {
    TEST_ASSERT_FLOAT_WITHIN(0.001, x, signal->x[index]);
    TEST_ASSERT_FLOAT_WITHIN(0.001, y, signal->y[index]);
    TEST_ASSERT_FLOAT_WITHIN(0.001, z, signal->z[index]);
}

void test_push_signal_null(void) {
    TEST_DELAY_OK(delay_signal_init(&delay_signal));

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

    AccelData input = {
        .num_samples = 1,
        .x = &x,
        .y = &y,
        .z = &z
    };

    AccelData delayed_signal = {
        .num_samples = 1,
        .x = &x_out,
        .y = &y_out,
        .z = &z_out
    };

    TEST_DELAY_OK(delay_signal_init(&delay_signal));

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

    AccelData input = {
        .num_samples = BUF_SIZE,
        .x = x,
        .y = y,
        .z = z
    };

    AccelData delayed_signal = {
        .num_samples = 1,
        .x = &x_out,
        .y = &y_out,
        .z = &z_out
    };

    TEST_DELAY_OK(delay_signal_init(&delay_signal));

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

    AccelData input = {
        .num_samples = BUF_SIZE,
        .x = x,
        .y = y,
        .z = z
    };

    AccelData delayed_signal = {
        .num_samples = 2,
        .x = x_out,
        .y = y_out,
        .z = z_out
    };

    TEST_DELAY_OK(delay_signal_init(&delay_signal));

    TEST_DELAY_OK(delay_signal_push_signal(&delay_signal,
                                           &input));


    TEST_DELAY_OK(delay_signal_get_delay_range(&delay_signal,
                                               &delayed_signal,
                                               1, 2));

    check_signal(&delayed_signal, &input);
}

void test_delay_too_large(void) {
    const uint32_t num_samples = DELAY_MAX_LEN;

    float x[DELAY_MAX_LEN] = {0};
    float y[DELAY_MAX_LEN] = {0};
    float z[DELAY_MAX_LEN] = {0};

    // Fill input with dummy values
    for (uint32_t i = 0; i < DELAY_MAX_LEN; ++i) {
        x[i] = 1.0f + i;
        y[i] = 2.0f + i;
        z[i] = 3.0f + i;
    }

    float x_out[DELAY_MAX_LEN] = {0};
    float y_out[DELAY_MAX_LEN] = {0};
    float z_out[DELAY_MAX_LEN] = {0};

    AccelData input = {
        .num_samples = num_samples,
        .x = x,
        .y = y,
        .z = z
    };

    AccelData delayed_signal = {
        .num_samples = 1,
        .x = x_out,
        .y = y_out,
        .z = z_out
    };

    TEST_DELAY_OK(delay_signal_init(&delay_signal));

    // Push all input samples into the delay buffer
    TEST_DELAY_OK(delay_signal_push_signal(&delay_signal, &input));

    // Request a delay equal to DELAY_MAX_LEN (out of bounds)
    TEST_DELAY_STATUS(delay_signal_get_delay_range(&delay_signal,
                                                   &delayed_signal,
                                                   DELAY_MAX_LEN, 1),
                      DELAY_STATUS_ERROR_DELAY_TOO_LARGE);
}

void test_delay_insufficient_history(void) {
    const uint32_t num_samples_pushed = DELAY_MAX_LEN - 1;
    const uint32_t delay_requested = DELAY_MAX_LEN - 1;

    float x[DELAY_MAX_LEN] = {0};
    float y[DELAY_MAX_LEN] = {0};
    float z[DELAY_MAX_LEN] = {0};

    for (uint32_t i = 0; i < num_samples_pushed; ++i) {
        x[i] = 1.0f + i;
        y[i] = 2.0f + i;
        z[i] = 3.0f + i;
    }

    float x_out = 0.0f;
    float y_out = 0.0f;
    float z_out = 0.0f;

    AccelData input = {
        .num_samples = num_samples_pushed,
        .x = x,
        .y = y,
        .z = z
    };

    AccelData delayed_signal = {
        .num_samples = 1,
        .x = &x_out,
        .y = &y_out,
        .z = &z_out
    };

    TEST_DELAY_OK(delay_signal_init(&delay_signal));

    TEST_DELAY_OK(delay_signal_push_signal(&delay_signal, &input));

    // Requesting a delay that is within DELAY_MAX_LEN but larger than actual samples pushed
    TEST_DELAY_STATUS(delay_signal_get_delay_range(&delay_signal,
                                                   &delayed_signal,
                                                   delay_requested,
                                                   1),
                      DELAY_STATUS_ERROR_DELAY_TOO_LARGE);
}

void test_delay_full_length_max_delay(void) {
    const uint32_t MAX = DELAY_MAX_LEN;

    float x[MAX], y[MAX], z[MAX];
    float x_out[MAX], y_out[MAX], z_out[MAX];

    for (uint32_t i = 0; i < MAX; ++i) {
        x[i] = 1.0f + i;
        y[i] = 2.0f + i;
        z[i] = 3.0f + i;
    }

    AccelData input = {
        .num_samples = MAX,
        .x = x,
        .y = y,
        .z = z
    };

    AccelData full_out = {
        .num_samples = MAX,
        .x = x_out,
        .y = y_out,
        .z = z_out
    };

    TEST_DELAY_OK(delay_signal_init(&delay_signal));
    TEST_DELAY_OK(delay_signal_push_signal(&delay_signal, &input));
    TEST_DELAY_OK(delay_signal_get_delay_range(&delay_signal, &full_out, MAX - 1, MAX));

    check_signal(&full_out, &input);
}

void test_delay_partial_window(void) {
    const uint32_t MAX = DELAY_MAX_LEN;
    const uint32_t delay = MAX * 4 / 5;
    const uint32_t len = delay * 5 / 6;

    float x[MAX], y[MAX], z[MAX];
    float x_out[len] = {0}, y_out[len] = {0}, z_out[len] = {0};

    for (uint32_t i = 0; i < MAX; ++i) {
        x[i] = 1.0f + i;
        y[i] = 2.0f + i;
        z[i] = 3.0f + i;
    }

    AccelData input = {
        .num_samples = MAX,
        .x = x,
        .y = y,
        .z = z
    };

    AccelData partial_out = {
        .num_samples = len,
        .x = x_out,
        .y = y_out,
        .z = z_out
    };

    const uint32_t expected_start_index = MAX - delay - 1;
    AccelData expected_output = {
        .num_samples = len,
        .x = &x[expected_start_index],
        .y = &y[expected_start_index],
        .z = &z[expected_start_index]
    };

    TEST_DELAY_OK(delay_signal_init(&delay_signal));
    TEST_DELAY_OK(delay_signal_push_signal(&delay_signal, &input));
    TEST_DELAY_OK(delay_signal_get_delay_range(&delay_signal, &partial_out, delay, len));


    check_signal(&partial_out, &expected_output);
}
