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

void check_signal(AccelData *signal, uint32_t index, float x, float y, float z) {
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

    check_signal(&delayed_signal, 0, x, y, z);
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

    check_signal(&delayed_signal, 0, x[0], y[0], z[0]);
}
