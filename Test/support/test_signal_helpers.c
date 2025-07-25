#include "unity.h"
#include <stdlib.h>

#include "test_signal_helpers.h"

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


test_signal_t *get_test_signals(const float32_t *x_in,
                                const float32_t *y_in,
                                const float32_t *z_in,
                                const float32_t *x_expected,
                                const float32_t *y_expected,
                                const float32_t *z_expected,
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

void init_batch_signals() {
    float *data_array = malloc(sizeof(float) * AI_INPUT_HEIGHT * AI_INPUT_WIDTH * AI_INPUT_CHANNEL);
    batch_signals.batch_out.data_array =
        (float (*)[AI_INPUT_WIDTH][AI_INPUT_CHANNEL])data_array;
}

batch_signal_t *get_batch_signals(const accel_data_t *in,
                                  accel_data_t *out,
                                  uint32_t start,
                                  uint32_t batch_size)
{
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
    batch_signals.accel_data_t_batch_out = &output_batch;

    return &batch_signals;
}

void copy_output_batch(batch_signal_t *batch) {
    for (uint32_t i = 0; i < batch->batch_in->num_samples; ++i) {
        batch->accel_data_t_batch_out->x[i] =
            AI_INPUT_GET_X(batch->batch_out.data_array, i);

        batch->accel_data_t_batch_out->y[i] =
            AI_INPUT_GET_Y(batch->batch_out.data_array, i);

        batch->accel_data_t_batch_out->z[i] =
            AI_INPUT_GET_Z(batch->batch_out.data_array, i);
    }
}

valid_output_t *get_valid_outputs(const accel_data_t *output,
                                  const accel_data_t *expected_output,
                                  uint32_t valid_start,
                                  uint32_t N)
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

void test_signal_setUp() {
    init_batch_signals();
}

void test_signal_tearDown(void) {
    if (preprocess_output.x != NULL) {
        free(preprocess_output.x);
        free(preprocess_output.y);
        free(preprocess_output.z);
    }

    if (scratch_buffer.x != NULL) {
        free(scratch_buffer.x);
        free(scratch_buffer.y);
        free(scratch_buffer.z);
    }

    if (batch_signals.batch_out.data_array != NULL) {
        free(batch_signals.batch_out.data_array);
    }
}
