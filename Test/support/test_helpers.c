#include "unity.h"
#include "test_helpers.h"

void check_signal(const AccelData *actual, const AccelData *expected) {
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(expected->num_samples, actual->num_samples,
                                     "Sample count mismatch");

    for (uint32_t i = 0; i < expected->num_samples; ++i) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, expected->x[i], actual->x[i]);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, expected->y[i], actual->y[i]);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, expected->z[i], actual->z[i]);
    }
}
