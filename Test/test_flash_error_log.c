#include "High_Level/nor_flash.h"

#include "b-u585i-iot02a-bsp/b_u585i_iot02a_errno.h"
#include "flash_error_log.h"
#include "flash_error_log_internal.h"
#include "mock_nor_flash.h"

#include "unity.h"

#include <stdlib.h>
#include <string.h>

// Helper macros for test status checking
#define TEST_ERROR_LOG_OK(expr) \
    do { \
        error_log_status_t status = (expr); \
        TEST_ASSERT_EQUAL_INT_MESSAGE(ERROR_LOG_OK, status, \
                                      "Expected ERROR_LOG_OK"); \
    } while (0)

#define TEST_ERROR_LOG_STATUS(expr, expected) \
    do { \
        error_log_status_t status = (expr); \
        TEST_ASSERT_EQUAL_INT_MESSAGE((expected), status, \
                                      "Unexpected error_log_status value"); \
    } while (0)

void setUp(void) {
    // TODO: Handle both flash log and error log sharing nor flash
    // writes and reads are probably fine, but need to avoid double init
    /*nor_flash_init_ExpectAndReturn(BSP_ERROR_NONE);*/
}

void tearDown(void) { }

/*void fill_error_log_buffer(error_log_row_t *row_buffer,*/
                           /*uint32_t timestamp_ms,*/
                           /*error_code_t error_code,*/
                           /*error_code_data_t error_data)*/
/*{*/
    /*row_buffer->row_start_marker = ERROR_LOG_ROW_START_MARKER;*/
    /*row_buffer->timestamp_ms = timestamp_ms;*/
    /*row_buffer->error_code = error_code;*/
    /*row_buffer->data = error_data;*/
/*}*/

void test_log_single_error(void) {
    uint32_t timestamp_ms = 0;
    error_code_t error_code = ERROR_IMU_READ_ERROR;
    error_code_data_t error_data = BSP_ERROR_COMPONENT_FAILURE;

    error_log_row_t row_buffer = {
        .row_start_marker = ERROR_LOG_ROW_START_MARKER,
        .timestamp_ms = timestamp_ms,
        .error_code = error_code,
        .data = error_data
    };


    // TODO Use correct start address
    nor_flash_write_ExpectWithArrayAndReturn(0, &row_buffer,
                                             sizeof(row_buffer), sizeof(row_buffer),
                                             BSP_ERROR_NONE);

    TEST_ERROR_LOG_OK(error_log(ERROR_IMU_READ_ERROR,
                                DATA_BSP_ERROR_CODE,
                                BSP_ERROR_COMPONENT_FAILURE));
}
