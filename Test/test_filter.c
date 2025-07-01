#include "support/test_helpers.h"
#include "unity.h"
#include "filter.h"
#include <float.h>

/*TEST_SOURCE_FILE("../weird_stuff.c")*/
/*TEST_SOURCE_FILE("../Drivers/weird_stuff_2.c")*/
TEST_SOURCE_FILE("../Drivers/CMSIS-DSP/Source/FilteringFunctions/arm_biquad_cascade_df2T_f32.c")
TEST_SOURCE_FILE("../Drivers/CMSIS-DSP/Source/FilteringFunctions/arm_biquad_cascade_df2T_init_f32.c")

// Helper macros for test status checking
#define TEST_FILTER_OK(expr) \
    do { \
        filter_status_t status = (expr); \
        TEST_ASSERT_EQUAL_INT_MESSAGE(FILTER_STATUS_OK, status, "Expected FILTER_STATUS_OK"); \
    } while (0)

#define TEST_FILTER_STATUS(expr, expected) \
    do { \
        filter_status_t status = (expr); \
        TEST_ASSERT_EQUAL_INT_MESSAGE((expected), status, "Unexpected filter_status_t value"); \
    } while (0)

void setUp(void) {}
void tearDown(void) {}

void test_dc_signal(void) {
    const uint32_t N = 100;

    float x[N] = {0};
    float y[N] = {0};
    float z[N] = {0};

    AccelData signal = {
        .num_samples = N,
        .x = x,
        .y = y,
        .z = z
    };

    float x_out[N];
    float y_out[N];
    float z_out[N];

    for (uint32_t i = 0; i < N; ++i) {
        x_out[i] = FLT_MAX;
        y_out[i] = FLT_MAX;
        z_out[i] = FLT_MAX;
    }

    AccelData out = {
        .num_samples = N,
        .x = x_out,
        .y = y_out,
        .z = z_out
    };

    TEST_FILTER_OK(filter_signal(&signal, &out));

    check_signal(&out, &signal);
}
