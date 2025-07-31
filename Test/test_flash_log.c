#include "accel_data_type.h"
#include "ai_input_data_type.h"
#include "support/imu_test_data.h"
#include "unity.h"

#include <stdlib.h>
#include <string.h>

#include "accel_data_type.h"
#include "ai_input_data_type.h"
#include "ai_output_data_type.h"

#include "flash_log.h"
#include "flash_log_internal.h"

#include "b-u585i-iot02a-bsp/b_u585i_iot02a_errno.h"
#include "mock_nor_flash.h"

// Helper macros for test status checking
#define TEST_FLASH_LOG_OK(expr) \
    do { \
        flash_log_status_t status = (expr); \
        TEST_ASSERT_EQUAL_INT_MESSAGE(FLASH_LOG_OK, status, \
                                      "Expected FLASH_LOG_OK"); \
    } while (0)

const uint32_t testFlashNumRows = 64;

typedef struct {
    accel_data_t unproc;
    accel_data_t filtered;
    ai_input_data_t ai_input;
    float model_output[AI_OUTPUT_CHANNEL];
    uint32_t output_class;
} test_log_input_data_t;

test_log_input_data_t testLogInputData;
flash_log_row_t *testExpectedRows;

void setUp(void)
{
    nor_flash_init_ExpectAndReturn(BSP_ERROR_NONE);

    TEST_FLASH_LOG_OK(flash_log_init());
}

void tearDown(void)
{
    if (testLogInputData.unproc.x != NULL) {
        free(testLogInputData.unproc.x);
        testLogInputData.unproc.x = NULL;
    }
    if (testLogInputData.unproc.y != NULL) {
        free(testLogInputData.unproc.y);
        testLogInputData.unproc.y = NULL;
    }
    if (testLogInputData.unproc.z != NULL) {
        free(testLogInputData.unproc.z);
        testLogInputData.unproc.z = NULL;
    }

    if (testLogInputData.filtered.x != NULL) {
        free(testLogInputData.filtered.x);
        testLogInputData.filtered.x = NULL;
    }
    if (testLogInputData.filtered.y != NULL) {
        free(testLogInputData.filtered.y);
        testLogInputData.filtered.y = NULL;
    }
    if (testLogInputData.filtered.z != NULL) {
        free(testLogInputData.filtered.z);
        testLogInputData.filtered.z = NULL;
    }

    if (testLogInputData.ai_input.data_array != NULL) {
        free(testLogInputData.ai_input.data_array);
        testLogInputData.ai_input.data_array = NULL;
    }

    if (testExpectedRows != NULL) {
        free(testExpectedRows);
        testExpectedRows = NULL;
    }
}

void test_single_write() {
    float unproc_x = 1.1f;
    float unproc_y = 2.2f;
    float unproc_z = 3.3f;

    accel_data_t unproc = {
        .num_samples = 1,
        .x = &unproc_x,
        .y = &unproc_y,
        .z = &unproc_z,
    };

    float filtered_x = 4.4f;
    float filtered_y = 5.5f;
    float filtered_z = 6.6f;

    accel_data_t filtered = {
        .num_samples = 1,
        .x = &filtered_x,
        .y = &filtered_y,
        .z = &filtered_z,
    };

    float data_array[1][AI_INPUT_WIDTH][AI_INPUT_CHANNEL];
    AI_INPUT_GET_X(data_array, 0) = 7.7;
    AI_INPUT_GET_Y(data_array, 0) = 8.8;
    AI_INPUT_GET_Z(data_array, 0) = 9.9;

    ai_input_data_t proc = {
        .data_array = data_array,
    };

    float model_output[4] = {10.10f, 11.11f, 12.12f, 13.13f};
    uint32_t output_class = 3;

    flash_log_row_t expected_row = {
        .row_start_marker = FLASH_LOG_ROW_START_MARKER,

        .unproc_x = unproc_x,
        .unproc_y = unproc_y,
        .unproc_z = unproc_z,

        .lowpass_filtered_x = filtered_x,
        .lowpass_filtered_y = filtered_y,
        .lowpass_filtered_z = filtered_z,

        .proc_x = AI_INPUT_GET_X(data_array, 0),
        .proc_y = AI_INPUT_GET_Y(data_array, 0),
        .proc_z = AI_INPUT_GET_Z(data_array, 0),

        .model_output = {model_output[0], model_output[1], model_output[2], model_output[3]},
        .output_class = output_class,
        .contains_output = 1,
    };

    nor_flash_write_ExpectWithArrayAndReturn(0, &expected_row, sizeof(flash_log_row_t), sizeof(flash_log_row_t), BSP_ERROR_NONE);

    flash_log_write_window(&unproc, &filtered, &proc, model_output, output_class, 1);
}

test_log_input_data_t *get_log_input_data_double_write() {
    float *unproc_x = malloc(2 * sizeof(float));
    float *unproc_y = malloc(2 * sizeof(float));
    float *unproc_z = malloc(2 * sizeof(float));

    unproc_x[0] = 1.1f; unproc_x[1] = 1.2f;
    unproc_y[0] = 2.2f; unproc_y[1] = 2.3f;
    unproc_z[0] = 3.3f; unproc_z[1] = 3.4f;

    float *filtered_x = malloc(2 * sizeof(float));
    float *filtered_y = malloc(2 * sizeof(float));
    float *filtered_z = malloc(2 * sizeof(float));

    filtered_x[0] = 4.4f; filtered_x[1] = 4.5f;
    filtered_y[0] = 5.5f; filtered_y[1] = 5.6f;
    filtered_z[0] = 6.6f; filtered_z[1] = 6.7f;

    testLogInputData.unproc.x = unproc_x;
    testLogInputData.unproc.y = unproc_y;
    testLogInputData.unproc.z = unproc_z;

    testLogInputData.filtered.x = filtered_x;
    testLogInputData.filtered.y = filtered_y;
    testLogInputData.filtered.z = filtered_z;

    float (*data_array)[AI_INPUT_WIDTH][AI_INPUT_CHANNEL] =
        malloc(2 * AI_INPUT_WIDTH * AI_INPUT_CHANNEL * sizeof(float));

    AI_INPUT_GET_X(data_array, 0) = 7.7f; AI_INPUT_GET_X(data_array, 1) = 7.8f;
    AI_INPUT_GET_Y(data_array, 0) = 8.8f; AI_INPUT_GET_Y(data_array, 1) = 8.9f;
    AI_INPUT_GET_Z(data_array, 0) = 9.9f; AI_INPUT_GET_Z(data_array, 1) = 9.0f;

    testLogInputData.ai_input.data_array = data_array;

    testLogInputData.model_output[0] = 10.10f; testLogInputData.model_output[1] = 11.11f;
    testLogInputData.model_output[2] = 12.12f; testLogInputData.model_output[3] = 13.13f;

    testLogInputData.output_class = 3;

    return &testLogInputData;
}

flash_log_row_t * get_expected_rows_double_write(test_log_input_data_t * testLogInputData) {
    flash_log_row_t expected_row_0 = {
        .row_start_marker = FLASH_LOG_ROW_START_MARKER,

        .unproc_x = testLogInputData->unproc.x[0],
        .unproc_y = testLogInputData->unproc.y[0],
        .unproc_z = testLogInputData->unproc.z[0],

        .lowpass_filtered_x = testLogInputData->filtered.x[0],
        .lowpass_filtered_y = testLogInputData->filtered.y[0],
        .lowpass_filtered_z = testLogInputData->filtered.z[0],

        .proc_x = AI_INPUT_GET_X(testLogInputData->ai_input.data_array, 0),
        .proc_y = AI_INPUT_GET_Y(testLogInputData->ai_input.data_array, 0),
        .proc_z = AI_INPUT_GET_Z(testLogInputData->ai_input.data_array, 0),

        .model_output = {0, 0, 0, 0},
        .output_class = 0,
        .contains_output = 0,
    };

    flash_log_row_t expected_row_1 = {
        .row_start_marker = FLASH_LOG_ROW_START_MARKER,

        .unproc_x = testLogInputData->unproc.x[1],
        .unproc_y = testLogInputData->unproc.y[1],
        .unproc_z = testLogInputData->unproc.z[1],

        .lowpass_filtered_x = testLogInputData->filtered.x[1],
        .lowpass_filtered_y = testLogInputData->filtered.y[1],
        .lowpass_filtered_z = testLogInputData->filtered.z[1],

        .proc_x = AI_INPUT_GET_X(testLogInputData->ai_input.data_array, 1),
        .proc_y = AI_INPUT_GET_Y(testLogInputData->ai_input.data_array, 1),
        .proc_z = AI_INPUT_GET_Z(testLogInputData->ai_input.data_array, 1),

        .model_output = {testLogInputData->model_output[0], testLogInputData->model_output[1], testLogInputData->model_output[2], testLogInputData->model_output[3]},
        .output_class = testLogInputData->output_class,
        .contains_output = 1,
    };

    flash_log_row_t * testExpectedRows = malloc(2 * sizeof(flash_log_row_t));

    memcpy(&testExpectedRows[0], &expected_row_0, sizeof(flash_log_row_t));
    memcpy(&testExpectedRows[1], &expected_row_1, sizeof(flash_log_row_t));

    /*testExpectedRows = malloc(2 * sizeof(flash_log_row_t));*/

    /*testExpectedRows[0].row_start_marker = FLASH_LOG_ROW_START_MARKER;*/

    /*testExpectedRows[0].unproc_x = testLogInputData->unproc.x[0];*/
    /*testExpectedRows[0].unproc_y = testLogInputData->unproc.y[0];*/
    /*testExpectedRows[0].unproc_z = testLogInputData->unproc.z[0];*/

    /*testExpectedRows[0].lowpass_filtered_x = testLogInputData->filtered.x[0];*/
    /*testExpectedRows[0].lowpass_filtered_y = testLogInputData->filtered.y[0];*/
    /*testExpectedRows[0].lowpass_filtered_z = testLogInputData->filtered.z[0];*/

    /*testExpectedRows[0].proc_x = AI_INPUT_GET_X(testLogInputData->ai_input.data_array, 0);*/
    /*testExpectedRows[0].proc_y = AI_INPUT_GET_Y(testLogInputData->ai_input.data_array, 0);*/
    /*testExpectedRows[0].proc_z = AI_INPUT_GET_Z(testLogInputData->ai_input.data_array, 0);*/

    /*testExpectedRows[0].model_output[0] = 0;*/
    /*testExpectedRows[0].model_output[1] = 0;*/
    /*testExpectedRows[0].model_output[2] = 0;*/
    /*testExpectedRows[0].model_output[3] = 0;*/

    /*testExpectedRows[0].output_class = 0;*/
    /*testExpectedRows[0].contains_output = 0;*/

    /*testExpectedRows[1].row_start_marker = FLASH_LOG_ROW_START_MARKER;*/

    /*testExpectedRows[1].unproc_x = testLogInputData->unproc.x[1];*/
    /*testExpectedRows[1].unproc_y = testLogInputData->unproc.y[1];*/
    /*testExpectedRows[1].unproc_z = testLogInputData->unproc.z[1];*/

    /*testExpectedRows[1].lowpass_filtered_x = testLogInputData->filtered.x[1];*/
    /*testExpectedRows[1].lowpass_filtered_y = testLogInputData->filtered.y[1];*/
    /*testExpectedRows[1].lowpass_filtered_z = testLogInputData->filtered.z[1];*/

    /*testExpectedRows[1].proc_x = AI_INPUT_GET_X(testLogInputData->ai_input.data_array, 1);*/
    /*testExpectedRows[1].proc_y = AI_INPUT_GET_Y(testLogInputData->ai_input.data_array, 1);*/
    /*testExpectedRows[1].proc_z = AI_INPUT_GET_Z(testLogInputData->ai_input.data_array, 1);*/

    /*testExpectedRows[0].model_output[0] = testLogInputData->model_output[0];*/
    /*testExpectedRows[0].model_output[1] = testLogInputData->model_output[1];*/
    /*testExpectedRows[0].model_output[2] = testLogInputData->model_output[2];*/
    /*testExpectedRows[0].model_output[3] = testLogInputData->model_output[3];*/

    /*testExpectedRows[1].output_class = testLogInputData->output_class;*/

    /*testExpectedRows[1].contains_output = 1;*/

    return testExpectedRows;
}

void test_double_write() {
    /*float unproc_x[2] = {1.1f, 1.2f};*/
    /*float unproc_y[2] = {2.2f, 2.3f};*/
    /*float unproc_z[2] = {3.3f, 3.4f};*/

    /*accel_data_t unproc = {*/
        /*.num_samples = 2,*/
        /*.x = unproc_x,*/
        /*.y = unproc_y,*/
        /*.z = unproc_z,*/
    /*};*/

    /*float filtered_x[2] = {4.4f, 4.5f};*/
    /*float filtered_y[2] = {5.5f, 5.6f};*/
    /*float filtered_z[2] = {6.6f, 6.7f};*/

    /*accel_data_t filtered = {*/
        /*.num_samples = 2,*/
        /*.x = filtered_x,*/
        /*.y = filtered_y,*/
        /*.z = filtered_z,*/
    /*};*/

    /*float data_array[2][AI_INPUT_WIDTH][AI_INPUT_CHANNEL];*/
    /*AI_INPUT_GET_X(data_array, 0) = 7.7;*/
    /*AI_INPUT_GET_Y(data_array, 0) = 8.8;*/
    /*AI_INPUT_GET_Z(data_array, 0) = 9.9;*/

    /*AI_INPUT_GET_X(data_array, 1) = 7.8;*/
    /*AI_INPUT_GET_Y(data_array, 1) = 8.9;*/
    /*AI_INPUT_GET_Z(data_array, 1) = 9.10;*/

    /*ai_input_data_t proc = {*/
        /*.data_array = data_array,*/
    /*};*/

    /*float model_output[4] = {10.10f, 11.11f, 12.12f, 13.13f};*/
    /*uint32_t output_class = 3;*/

    test_log_input_data_t *testLogInputData = get_log_input_data_double_write();

    flash_log_row_t *expectedRows = get_expected_rows_double_write(testLogInputData);

    /*flash_log_row_t expected_row_0 = {*/
        /*.row_start_marker = FLASH_LOG_ROW_START_MARKER,*/

        /*.unproc_x = testLogInputData->unproc.x[0],*/
        /*.unproc_y = testLogInputData->unproc.y[0],*/
        /*.unproc_z = testLogInputData->unproc.z[0],*/

        /*.lowpass_filtered_x = testLogInputData->filtered.x[0],*/
        /*.lowpass_filtered_y = testLogInputData->filtered.y[0],*/
        /*.lowpass_filtered_z = testLogInputData->filtered.z[0],*/

        /*.proc_x = AI_INPUT_GET_X(testLogInputData->ai_input.data_array, 0),*/
        /*.proc_y = AI_INPUT_GET_Y(testLogInputData->ai_input.data_array, 0),*/
        /*.proc_z = AI_INPUT_GET_Z(testLogInputData->ai_input.data_array, 0),*/

        /*.model_output = {0, 0, 0, 0},*/
        /*.output_class = 0,*/
        /*.contains_output = 0,*/
    /*};*/

    /*flash_log_row_t expected_row_1 = {*/
        /*.row_start_marker = FLASH_LOG_ROW_START_MARKER,*/

        /*.unproc_x = testLogInputData->unproc.x[1],*/
        /*.unproc_y = testLogInputData->unproc.y[1],*/
        /*.unproc_z = testLogInputData->unproc.z[1],*/

        /*.lowpass_filtered_x = testLogInputData->filtered.x[1],*/
        /*.lowpass_filtered_y = testLogInputData->filtered.y[1],*/
        /*.lowpass_filtered_z = testLogInputData->filtered.z[1],*/

        /*.proc_x = AI_INPUT_GET_X(testLogInputData->ai_input.data_array, 1),*/
        /*.proc_y = AI_INPUT_GET_Y(testLogInputData->ai_input.data_array, 1),*/
        /*.proc_z = AI_INPUT_GET_Z(testLogInputData->ai_input.data_array, 1),*/

        /*.model_output = {testLogInputData->model_output[0], testLogInputData->model_output[1], testLogInputData->model_output[2], testLogInputData->model_output[3]},*/
        /*.output_class = testLogInputData->output_class,*/
        /*.contains_output = 1,*/
    /*};*/

    /*[>flash_log_row_t expected_row_1 = {<]*/
        /*[>.row_start_marker = FLASH_LOG_ROW_START_MARKER,<]*/

        /*[>.unproc_x = unproc_x[1],<]*/
        /*[>.unproc_y = unproc_y[1],<]*/
        /*[>.unproc_z = unproc_z[1],<]*/

        /*[>.lowpass_filtered_x = filtered_x[1],<]*/
        /*[>.lowpass_filtered_y = filtered_y[1],<]*/
        /*[>.lowpass_filtered_z = filtered_z[1],<]*/

        /*[>.proc_x = AI_INPUT_GET_X(data_array, 1),<]*/
        /*[>.proc_y = AI_INPUT_GET_Y(data_array, 1),<]*/
        /*[>.proc_z = AI_INPUT_GET_Z(data_array, 1),<]*/

        /*[>.model_output = {model_output[0], model_output[1], model_output[2], model_output[3]},<]*/
        /*[>.output_class = output_class,<]*/
        /*[>.contains_output = 1,<]*/
    /*[>};<]*/

    nor_flash_write_ExpectWithArrayAndReturn(0, &expectedRows[0], sizeof(flash_log_row_t), sizeof(flash_log_row_t), BSP_ERROR_NONE);
    nor_flash_write_ExpectWithArrayAndReturn(0, &expectedRows[1], sizeof(flash_log_row_t), sizeof(flash_log_row_t), BSP_ERROR_NONE);

    flash_log_write_window(&testLogInputData->unproc,
                           &testLogInputData->filtered,
                           &testLogInputData->ai_input,
                           testLogInputData->model_output,
                           testLogInputData->output_class,
                           2);
}

void test_recover_log_pointer() {

}
