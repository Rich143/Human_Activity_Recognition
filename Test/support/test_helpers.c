#include "unity.h"
#include "test_helpers.h"

void check_signal(const accel_data_t *actual, const accel_data_t *expected) {
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(expected->num_samples, actual->num_samples,
                                     "Sample count mismatch");

    for (uint32_t i = 0; i < expected->num_samples; ++i) {
        char message[128];

        sprintf(message, "X Accel value mismatch at index %u",
                i, expected->x[i], actual->x[i]);
        TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.001f, expected->x[i], actual->x[i], message);

        sprintf(message, "Y Accel value mismatch at index %u",
                i, expected->y[i], actual->y[i]);
        TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.001f, expected->y[i], actual->y[i], message);

        sprintf(message, "Z Accel value mismatch at index %u",
                i, expected->z[i], actual->z[i]);
        TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.001f, expected->z[i], actual->z[i], message);
    }
}

void check_signal_close(const accel_data_t *actual, const accel_data_t *expected,
                        float32_t tolerance)
{
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(expected->num_samples, actual->num_samples,
                                     "Sample count mismatch");

    for (uint32_t i = 0; i < expected->num_samples; ++i) {
        char message[128];

        sprintf(message, "X Accel value mismatch at index %u",
                i, expected->x[i], actual->x[i]);
        TEST_ASSERT_FLOAT_WITHIN_MESSAGE(tolerance, expected->x[i], actual->x[i], message);

        sprintf(message, "Y Accel value mismatch at index %u",
                i, expected->y[i], actual->y[i]);
        TEST_ASSERT_FLOAT_WITHIN_MESSAGE(tolerance, expected->y[i], actual->y[i], message);

        sprintf(message, "Z Accel value mismatch at index %u",
                i, expected->z[i], actual->z[i]);
        TEST_ASSERT_FLOAT_WITHIN_MESSAGE(tolerance, expected->z[i], actual->z[i], message);
    }
}

