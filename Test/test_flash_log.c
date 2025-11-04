#include "High_Level/flash_manager.h"
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

#include "config.h"
#include "log_utils.h"
#include "b-u585i-iot02a-bsp/b_u585i_iot02a_errno.h"
#include "mock_flash_manager.h"
#include "mock_uart.h"

// Helper macros for test status checking
#define TEST_FLASH_LOG_OK(expr) \
    do { \
        flash_log_status_t status = (expr); \
        TEST_ASSERT_EQUAL_INT_MESSAGE(FLASH_LOG_OK, status, \
                                      "Expected FLASH_LOG_OK"); \
    } while (0)

#define TEST_FLASH_LOG_STATUS(expr, expected) \
    do { \
        flash_log_status_t status = (expr); \
        TEST_ASSERT_EQUAL_INT_MESSAGE((expected), status, \
                                      "Unexpected flash_log_status value"); \
    } while (0)

typedef struct {
    accel_data_t unproc;
    accel_data_t filtered;
    ai_input_data_t ai_input;
    float model_output[AI_OUTPUT_CHANNEL];
    uint32_t output_class;
} test_log_input_data_t;

test_log_input_data_t testLogInputData;
flash_log_row_t *gTestExpectedRows;

flash_region_t data_log_region = {
    .region_idx = FLASH_REGION_DATA_LOGS,
    .start_sector = CONFIG_FLASH_DATA_LOG_SECTOR_START,
    .num_sectors = CONFIG_FLASH_DATA_LOG_SIZE_SECTORS
};

flash_region_t *flash_manager_get_region_cb(flash_region_idxs_t region_idx, int numCalls) {
    if (region_idx != FLASH_REGION_DATA_LOGS) {
        TEST_FAIL_MESSAGE("flash_manager_get_region called with wrong region");
    }

    return &data_log_region;
}

void setUp(void) {
    flash_manager_get_region_Stub(flash_manager_get_region_cb);
}

void init_flash_log(void)
{
    flash_manager_init_ExpectAndReturn(FLASH_MANAGER_OK);

    TEST_FLASH_LOG_OK(flash_log_init());
}

flash_manager_status_t flash_manager_read_cb(flash_region_idxs_t region_idx,
                                             uint32_t readAddress,
                                             uint8_t* pBuffer, uint32_t size,
                                             int cmock_num_calls)
{
    uint8_t *flash = (uint8_t *)gTestExpectedRows;
    memcpy(pBuffer, &flash[readAddress], size);

    return BSP_ERROR_NONE;
}

void initTestFlash(uint32_t numRows) {
    gTestExpectedRows = malloc(numRows * sizeof(flash_log_row_t));
    memset(gTestExpectedRows, 0xFF, numRows * sizeof(flash_log_row_t));

    flash_manager_read_Stub(flash_manager_read_cb);

    init_flash_log();
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

    if (gTestExpectedRows != NULL) {
        free(gTestExpectedRows);
        gTestExpectedRows = NULL;
    }
}

void test_single_write() {
    initTestFlash(1);

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

    flash_manager_write_ExpectWithArrayAndReturn(FLASH_REGION_DATA_LOGS, 0, (uint8_t *)&expected_row, sizeof(flash_log_row_t), sizeof(flash_log_row_t), BSP_ERROR_NONE);

    TEST_FLASH_LOG_OK(flash_log_write_window(&unproc, &filtered, &proc, model_output, output_class, true, 1));
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

flash_log_row_t * get_expected_rows_double_write(test_log_input_data_t *testLogInputData,
                                                 flash_log_row_t *testExpectedRows)
{
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

    memcpy(&testExpectedRows[0], &expected_row_0, sizeof(flash_log_row_t));
    memcpy(&testExpectedRows[1], &expected_row_1, sizeof(flash_log_row_t));

    return testExpectedRows;
}

int32_t print_rows_from_write(uint32_t writeAddress, uint8_t* pBuffer, uint32_t size, int cmock_num_calls) {
    flash_log_row_t *rows = (flash_log_row_t*)pBuffer;

    uint32_t num_rows = size / sizeof(flash_log_row_t);

    for (uint32_t i = 0; i < num_rows; i++) {
        printf("Row %d\n", i);
        printf("Header: 0x%X\n", rows[i].row_start_marker);
        printf("Unproc: %f, %f, %f\n", rows[i].unproc_x, rows[i].unproc_y, rows[i].unproc_z);
        printf("Filtered: %f, %f, %f\n", rows[i].lowpass_filtered_x, rows[i].lowpass_filtered_y, rows[i].lowpass_filtered_z);
        printf("AI Input: %f, %f, %f\n", rows[i].proc_x, rows[i].proc_y, rows[i].proc_z);
        printf("Model Output: %f, %f, %f, %f\n", rows[i].model_output[0], rows[i].model_output[1], rows[i].model_output[2], rows[i].model_output[3]);
        printf("Output Class: %d\n", rows[i].output_class);
        printf("Contains Output: %d\n", rows[i].contains_output);
        printf("\n\n\n");
    }

    return BSP_ERROR_NONE;
}
// Debug Code stubs
/*nor_flash_write_Stub(print_rows_from_write);*/
/*print_rows_from_write(0, (uint8_t *)expectedRows, 2 * sizeof(flash_log_row_t), 0);*/

void test_double_write() {
    test_log_input_data_t *testLogInputData = get_log_input_data_double_write();

    initTestFlash(2);

    flash_log_row_t *expectedRows = get_expected_rows_double_write(testLogInputData, gTestExpectedRows);

    flash_manager_write_ExpectWithArrayAndReturn(FLASH_REGION_DATA_LOGS, 0,
                                                 (uint8_t *)expectedRows,
                                                 2 * sizeof(flash_log_row_t),
                                                 2 * sizeof(flash_log_row_t),
                                                 BSP_ERROR_NONE);

    TEST_FLASH_LOG_OK(flash_log_write_window(&testLogInputData->unproc,
                           &testLogInputData->filtered,
                           &testLogInputData->ai_input,
                           testLogInputData->model_output,
                           testLogInputData->output_class,
                           true,
                           2));
}

void test_multiple_double_writes() {
    test_log_input_data_t *testLogInputData = get_log_input_data_double_write();

    initTestFlash(8);
    flash_log_row_t *expectedRows = get_expected_rows_double_write(testLogInputData, gTestExpectedRows);

    for (int i = 0; i < 8; i += 2) {
        uint32_t expectedAddress = i * sizeof(flash_log_row_t);

        flash_manager_write_ExpectWithArrayAndReturn(FLASH_REGION_DATA_LOGS,
                                                     expectedAddress,
                                                     (uint8_t *)expectedRows,
                                                     2 * sizeof(flash_log_row_t),
                                                     2 * sizeof(flash_log_row_t),
                                                     BSP_ERROR_NONE);

        TEST_FLASH_LOG_OK(flash_log_write_window(&testLogInputData->unproc,
                                                 &testLogInputData->filtered,
                                                 &testLogInputData->ai_input,
                                                 testLogInputData->model_output,
                                                 testLogInputData->output_class,
                                                 true,
                                                 2));
    }


}

flash_manager_status_t flash_manager_write_stub(uint32_t region,
                                                uint32_t address,
                                                uint8_t *data,
                                                uint32_t size,
                                                int numCalls)
{
    if (region != FLASH_REGION_DATA_LOGS) {
        TEST_FAIL_MESSAGE("flash_manager_write called with wrong region");
    }

    uint32_t max_address = CONFIG_FLASH_DATA_LOG_SIZE_SECTORS * NOR_FLASH_SECTOR_SIZE - 1;

    if (address > max_address) {
        return FLASH_MANAGER_ERROR_OUT_OF_BOUNDS;
    }

    return FLASH_MANAGER_OK;
}

void test_fill_log() {
    int num_logs = CONFIG_FLASH_DATA_LOG_SIZE_SECTORS * NOR_FLASH_SECTOR_SIZE /
        sizeof(flash_log_row_t);

    initTestFlash(num_logs);

    test_log_input_data_t *testLogInputData = get_log_input_data_double_write();

    flash_manager_write_Stub(flash_manager_write_stub);

    for (int i = 0; i < num_logs; i += 2) {
        TEST_FLASH_LOG_OK(flash_log_write_window(&testLogInputData->unproc,
                                                 &testLogInputData->filtered,
                                                 &testLogInputData->ai_input,
                                                 testLogInputData->model_output,
                                                 testLogInputData->output_class,
                                                 true,
                                                 2));
    }

    TEST_FLASH_LOG_STATUS(flash_log_write_window(&testLogInputData->unproc,
                                             &testLogInputData->filtered,
                                             &testLogInputData->ai_input,
                                             testLogInputData->model_output,
                                             testLogInputData->output_class,
                                             true,
                                             2),
                          FLASH_LOG_FULL);
}

void test_recover_log_pointer_less_than_buffer_size() {
    const uint32_t testFlashNumRows = 10;

    test_log_input_data_t * testLogInputData = get_log_input_data_double_write();

    initTestFlash(testFlashNumRows);
    flash_log_row_t * expectedRows =
        get_expected_rows_double_write(testLogInputData,
                                       gTestExpectedRows);

    TEST_FLASH_LOG_OK(flash_log_recover_log_pointer());

    uint32_t numLoggedRows = flash_log_get_num_log_entries();

    TEST_ASSERT_EQUAL_UINT32(2, numLoggedRows);
}

void test_recover_log_pointer_greater_than_buffer_size() {
    const uint32_t testFlashNumRows = 128;

    test_log_input_data_t * testLogInputData = get_log_input_data_double_write();

    initTestFlash(testFlashNumRows);

    for (uint32_t i = 0; i < testFlashNumRows - 10; i += 2) {
        get_expected_rows_double_write(testLogInputData,
                                       &gTestExpectedRows[i]);
    }

    TEST_FLASH_LOG_OK(flash_log_recover_log_pointer());

    uint32_t numLoggedRows = flash_log_get_num_log_entries();

    TEST_ASSERT_EQUAL_UINT32(testFlashNumRows - 10, numLoggedRows);
}

void test_clear_logs_single_sector() {
    const uint32_t testFlashNumRows = 128;

    const uint32_t testFlashRowsFilled = testFlashNumRows - 10;


    test_log_input_data_t * testLogInputData = get_log_input_data_double_write();

    initTestFlash(testFlashNumRows);

    for (uint32_t i = 0; i < testFlashRowsFilled; i += 2) {
        get_expected_rows_double_write(testLogInputData,
                                       &gTestExpectedRows[i]);
    }

    TEST_FLASH_LOG_OK(flash_log_recover_log_pointer());

    uint32_t numLoggedRows = flash_log_get_num_log_entries();

    TEST_ASSERT_EQUAL_UINT32(testFlashRowsFilled, numLoggedRows);

    flash_manager_erase_sector_ExpectAndReturn(FLASH_REGION_DATA_LOGS, 0, BSP_ERROR_NONE);

    // Clear the logs
    TEST_FLASH_LOG_OK(flash_log_clear_logs());

    // Verify that the logs are cleared
    numLoggedRows = flash_log_get_num_log_entries();
    TEST_ASSERT_EQUAL_UINT32(0, numLoggedRows);
}

void test_clear_logs_multiple_sectors() {
    const uint32_t testFlashNumSectorsFilled = 5;

    const uint32_t testFlashNumRows
        = NOR_FLASH_SECTOR_SIZE * testFlashNumSectorsFilled /
        sizeof(flash_log_row_t);

    const uint32_t testFlashRowsFilled = testFlashNumRows - 10;


    test_log_input_data_t * testLogInputData = get_log_input_data_double_write();

    initTestFlash(testFlashNumRows);

    for (uint32_t i = 0; i < testFlashRowsFilled; i += 2) {
        get_expected_rows_double_write(testLogInputData,
                                       &gTestExpectedRows[i]);
    }

    TEST_FLASH_LOG_OK(flash_log_recover_log_pointer());

    uint32_t numLoggedRows = flash_log_get_num_log_entries();

    TEST_ASSERT_EQUAL_UINT32(testFlashRowsFilled, numLoggedRows);

    for (uint32_t i = 0; i < testFlashNumSectorsFilled; i++) {
        flash_manager_erase_sector_ExpectAndReturn(FLASH_REGION_DATA_LOGS, i, BSP_ERROR_NONE);
    }

    // Clear the logs
    TEST_FLASH_LOG_OK(flash_log_clear_logs());

    // Verify that the logs are cleared
    numLoggedRows = flash_log_get_num_log_entries();
    TEST_ASSERT_EQUAL_UINT32(0, numLoggedRows);
}

#define RUN_PRINT_TESTS 0

void test_print_log_csv() {
#if RUN_PRINT_TESTS
    const uint32_t testFlashNumRows = 128;

    test_log_input_data_t * testLogInputData = get_log_input_data_double_write();

    initTestFlash(testFlashNumRows);

    for (uint32_t i = 0; i < testFlashNumRows - 10; i += 2) {
        get_expected_rows_double_write(testLogInputData,
                                       &gTestExpectedRows[i]);
    }

    TEST_FLASH_LOG_OK(flash_log_recover_log_pointer());

    flash_log_print_csv();
#endif
}

flash_log_row_t *
get_expected_rows_double_write_model_disabled(test_log_input_data_t *testLogInputData,
                                              flash_log_row_t *testExpectedRows)
{
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

        .model_output = {0, 0, 0, 0},
        .output_class = 0,
        .contains_output = 0,
    };

    memcpy(&testExpectedRows[0], &expected_row_0, sizeof(flash_log_row_t));
    memcpy(&testExpectedRows[1], &expected_row_1, sizeof(flash_log_row_t));

    return testExpectedRows;
}

void test_double_write_model_disabled() {
    test_log_input_data_t *testLogInputData = get_log_input_data_double_write();

    initTestFlash(2);

    flash_log_row_t *expectedRows = get_expected_rows_double_write_model_disabled(testLogInputData, gTestExpectedRows);

    flash_manager_write_ExpectWithArrayAndReturn(FLASH_REGION_DATA_LOGS, 0,
                                                 (uint8_t *)expectedRows,
                                                 2 * sizeof(flash_log_row_t),
                                                 2 * sizeof(flash_log_row_t),
                                                 BSP_ERROR_NONE);

    TEST_FLASH_LOG_OK(flash_log_write_window(&testLogInputData->unproc,
                           &testLogInputData->filtered,
                           &testLogInputData->ai_input,
                           testLogInputData->model_output,
                           testLogInputData->output_class,
                           false,
                           2));
}
