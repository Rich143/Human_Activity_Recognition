#include "support/test_helpers.h"
#include "unity.h"
#include "filter.h"
#include <float.h>
#include "support/test_signal_fs100Hz_f01Hz_f08Hz_len60s.h"
#include "support/filtered_output_fs100Hz_f01Hz_f08Hz_len60s.h"

#define DEBUG_DUMP_FILTERED_OUTPUT 0
#if DEBUG_DUMP_FILTERED_OUTPUT
#include <stdio.h>
#endif

// To get these directives to work:
// - quote the file name in " "
// - Add the directory containing the file to :paths: in project.yml
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

static filter_t filter;

void check_signal_close(const AccelData *actual, const AccelData *expected, float32_t tolerance) {
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(expected->num_samples, actual->num_samples,
                                     "Sample count mismatch");

    for (uint32_t i = 0; i < expected->num_samples; ++i) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, expected->x[i], actual->x[i]);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, expected->y[i], actual->y[i]);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, expected->z[i], actual->z[i]);
    }
}

void setUp(void) {
    TEST_FILTER_OK(filter_init(&filter));
}
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

    TEST_FILTER_OK(filter_signal(&filter, &signal, &out));

    check_signal(&out, &signal);
}

void test_filtered_signal_match(void) {
    const uint32_t N = test_signal_len;
    const float tol = 1e-4f;

    float x[N];
    float y[N];
    float z[N];

    float x_expected[N];
    float y_expected[N];
    float z_expected[N];

    for (uint32_t i = 0; i < N; ++i) {
        x[i] = test_signal[i];
        y[i] = test_signal[i];
        z[i] = test_signal[i];
    }

    for (uint32_t i = 0; i < N; ++i) {
        x_expected[i] = filtered_output[i];
        y_expected[i] = filtered_output[i];
        z_expected[i] = filtered_output[i];
    }

    float x_out[N];
    float y_out[N];
    float z_out[N];

    for (uint32_t i = 0; i < N; ++i) {
        x_out[i] = FLT_MAX;
        y_out[i] = FLT_MAX;
        z_out[i] = FLT_MAX;
    }

    AccelData input = {
        .num_samples = N,
        .x = x,
        .y = y,
        .z = z
    };

    AccelData output = {
        .num_samples = N,
        .x = x_out,
        .y = y_out,
        .z = z_out
    };

    AccelData expected_output = {
        .num_samples = N,
        .x = x_expected,
        .y = y_expected,
        .z = z_expected
    };

    TEST_FILTER_OK(filter_signal(&filter, &input, &output));

#if DEBUG_DUMP_FILTERED_OUTPUT
    FILE *f = fopen("filtered_output.py", "w");
    if (f) {
        fprintf(f, "filtered_x = [\n");
        for (uint32_t i = 0; i < N; ++i) {
            fprintf(f, "    %.7g%s", x_out[i], (i < N - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == N - 1) fprintf(f, "\n");
        }

        fprintf(f, "]\n\nfiltered_y = [\n");
        for (uint32_t i = 0; i < N; ++i) {
            fprintf(f, "    %.7g%s", y_out[i], (i < N - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == N - 1) fprintf(f, "\n");
        }

        fprintf(f, "]\n\nfiltered_z = [\n");
        for (uint32_t i = 0; i < N; ++i) {
            fprintf(f, "    %.7g%s", z_out[i], (i < N - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == N - 1) fprintf(f, "\n");
        }

        fprintf(f, "]\n\nexpected = [\n");
        for (uint32_t i = 0; i < N; ++i) {
            fprintf(f, "    %.7g%s", filtered_output[i], (i < N - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == N - 1) fprintf(f, "\n");
        }
        fprintf(f, "]\n");

        fclose(f);
    } else {
        printf("Failed to open output file for writing\n");
    }
#endif

    check_signal_close(&output, &expected_output, tol);
}

void test_filtered_signal_match_batched(void) {
    const uint32_t N = test_signal_len;
    const uint32_t BATCH_SIZE = 32;
    const float tol = 1e-4f;

    float x[N];
    float y[N];
    float z[N];

    float x_expected[N];
    float y_expected[N];
    float z_expected[N];

    for (uint32_t i = 0; i < N; ++i) {
        x[i] = test_signal[i];
        y[i] = test_signal[i];
        z[i] = test_signal[i];
        x_expected[i] = filtered_output[i];
        y_expected[i] = filtered_output[i];
        z_expected[i] = filtered_output[i];
    }

    float x_out[N];
    float y_out[N];
    float z_out[N];

    for (uint32_t i = 0; i < N; ++i) {
        x_out[i] = FLT_MAX;
        y_out[i] = FLT_MAX;
        z_out[i] = FLT_MAX;
    }

    // Process signal in batches
    for (uint32_t start = 0; start < N; start += BATCH_SIZE) {
        uint32_t batch_len = (start + BATCH_SIZE <= N) ? BATCH_SIZE : (N - start);

        AccelData input_batch = {
            .num_samples = batch_len,
            .x = &x[start],
            .y = &y[start],
            .z = &z[start]
        };

        AccelData output_batch = {
            .num_samples = batch_len,
            .x = &x_out[start],
            .y = &y_out[start],
            .z = &z_out[start]
        };

        TEST_FILTER_OK(filter_signal(&filter, &input_batch, &output_batch));
    }

    AccelData actual_output = {
        .num_samples = N,
        .x = x_out,
        .y = y_out,
        .z = z_out
    };

    AccelData expected = {
        .num_samples = N,
        .x = x_expected,
        .y = y_expected,
        .z = z_expected
    };

#if DEBUG_DUMP_FILTERED_OUTPUT
    FILE *f = fopen("filtered_output_batched.py", "w");
    if (f) {
        fprintf(f, "filtered_batched_x = [\n");
        for (uint32_t i = 0; i < N; ++i) {
            fprintf(f, "    %.7g%s", x_out[i], (i < N - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == N - 1) fprintf(f, "\n");
        }

        fprintf(f, "]\n\nfiltered_batched_y = [\n");
        for (uint32_t i = 0; i < N; ++i) {
            fprintf(f, "    %.7g%s", y_out[i], (i < N - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == N - 1) fprintf(f, "\n");
        }

        fprintf(f, "]\n\nfiltered_batched_z = [\n");
        for (uint32_t i = 0; i < N; ++i) {
            fprintf(f, "    %.7g%s", z_out[i], (i < N - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == N - 1) fprintf(f, "\n");
        }

        fprintf(f, "]\n\nexpected_batched = [\n");
        for (uint32_t i = 0; i < N; ++i) {
            fprintf(f, "    %.7g%s", filtered_output[i], (i < N - 1) ? "," : "");
            if ((i + 1) % 8 == 0 || i == N - 1) fprintf(f, "\n");
        }
        fprintf(f, "]\n");

        fclose(f);
    } else {
        printf("Failed to open output file for writing\n");
    }
#endif

    check_signal_close(&actual_output, &expected, tol);
}
