#pragma once

#include "accel_data_type.h"

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


void test_signal_tearDown(void);

accel_data_t *get_scratch_buffer(uint32_t num_samples);

test_signal_t *get_test_signals(const float32_t *x_in,
                                const float32_t *y_in,
                                const float32_t *z_in,
                                const float32_t *x_expected,
                                const float32_t *y_expected,
                                const float32_t *z_expected,
                                uint32_t num_samples);

batch_signal_t *get_batch_signals(const accel_data_t *in,
                                  accel_data_t *out,
                                  uint32_t start,
                                  uint32_t batch_size);

valid_output_t *get_valid_outputs(const accel_data_t *output,
                                  const accel_data_t *expected_output,
                                  uint32_t valid_start,
                                  uint32_t N);
