#include "High_Level/flash_manager.h"
#include "High_Level/nor_flash.h"

#include "b-u585i-iot02a-bsp/b_u585i_iot02a_errno.h"

#include "flash_error_log.h"
#include "flash_error_log_internal.h"
#include "mock_flash_manager.h"
#include "mock_stm32u5xx_hal.h"

#include "unity.h"

#include <stdlib.h>
#include <string.h>


// Hack, since including preprocess.h will make ceedling try to compile preprocess.c
#define PREPROCESS_STATUS_ERROR_OTHER 1

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
    flash_manager_init_ExpectAndReturn(FLASH_MANAGER_OK);

    TEST_ERROR_LOG_OK(error_log_init());
}

void tearDown(void) { }

void fill_error_log_buffer(error_log_row_t *row_buffer,
                           uint32_t timestamp_ms,
                           error_code_t error_code,
                           error_code_data_t error_data)
{
    row_buffer->row_start_marker = ERROR_LOG_ROW_START_MARKER;
    row_buffer->timestamp_ms = timestamp_ms;
    row_buffer->error_code = error_code;
    row_buffer->data = error_data;
    row_buffer->padding = 0;
}

void test_log_single_error(void) {
    uint32_t timestamp_ms = 0;
    error_code_t error_code = ERROR_IMU_READ_ERROR;
    error_code_data_t error_data = BSP_ERROR_COMPONENT_FAILURE;

    error_log_row_t row_buffer;
    fill_error_log_buffer(&row_buffer, timestamp_ms, error_code, error_data);

    HAL_GetTick_ExpectAndReturn(0);
    flash_manager_write_ExpectWithArrayAndReturn(FLASH_REGION_ERROR_LOGS, 0,
                                                 (uint8_t *)&row_buffer,
                                                 sizeof(row_buffer),
                                                 sizeof(row_buffer),
                                                 FLASH_MANAGER_OK);

    TEST_ERROR_LOG_OK(error_log(ERROR_IMU_READ_ERROR,
                                DATA_BSP_ERROR_CODE,
                                BSP_ERROR_COMPONENT_FAILURE));
}

void test_log_multiple_errors(void) {
    uint32_t timestamp_ms = 0;
    error_code_t error_code_1 = ERROR_IMU_READ_ERROR;
    error_code_data_t error_data_1 = BSP_ERROR_COMPONENT_FAILURE;

    error_log_row_t row_buffer;
    fill_error_log_buffer(&row_buffer, timestamp_ms, error_code_1, error_data_1);

    timestamp_ms = 1100;
    error_code_t error_code_2 = ERROR_PREPROCESS_ERROR;
    error_code_data_t error_data_2 = PREPROCESS_STATUS_ERROR_OTHER;

    error_log_row_t row_buffer_2;
    fill_error_log_buffer(&row_buffer_2, timestamp_ms, error_code_2, error_data_2);

    HAL_GetTick_ExpectAndReturn(0);
    flash_manager_write_ExpectWithArrayAndReturn(FLASH_REGION_ERROR_LOGS, 0,
                                                 (uint8_t *)&row_buffer,
                                                 sizeof(row_buffer),
                                                 sizeof(row_buffer),
                                                 FLASH_MANAGER_OK);

    HAL_GetTick_ExpectAndReturn(timestamp_ms);

    uint32_t flash_manager_address_2 = sizeof(row_buffer);
    flash_manager_write_ExpectWithArrayAndReturn(FLASH_REGION_ERROR_LOGS,
                                                 flash_manager_address_2,
                                                 (uint8_t *)&row_buffer_2,
                                                 sizeof(row_buffer_2),
                                                 sizeof(row_buffer_2),
                                                 FLASH_MANAGER_OK);

    TEST_ERROR_LOG_OK(error_log(error_code_1,
                                DATA_BSP_ERROR_CODE,
                                error_data_1));

    TEST_ERROR_LOG_OK(error_log(error_code_2,
                                DATA_BSP_ERROR_CODE,
                                error_data_2));
}

// TODO: Test log full

void test_clear_error_log() {
    flash_manager_write_IgnoreAndReturn(FLASH_MANAGER_OK);
    HAL_GetTick_IgnoreAndReturn(0);
    int num_logs = 128;

    flash_manager_erase_sector_ExpectAndReturn(FLASH_REGION_ERROR_LOGS, 0,
                                               FLASH_MANAGER_OK);

    for (int i = 0; i < num_logs; i++) {
        TEST_ERROR_LOG_OK(error_log(ERROR_IMU_READ_ERROR,
                                    DATA_BSP_ERROR_CODE,
                                    BSP_ERROR_COMPONENT_FAILURE));
    }

    TEST_ERROR_LOG_OK(error_log_clear_logs());
}

void test_clear_error_log_multiple_sectors() {
    flash_manager_write_IgnoreAndReturn(FLASH_MANAGER_OK);
    HAL_GetTick_IgnoreAndReturn(0);

    // go over the sector boundary
    int num_logs = NOR_FLASH_SECTOR_SIZE / sizeof(error_log_row_t) + 1;

    flash_manager_erase_sector_ExpectAndReturn(FLASH_REGION_ERROR_LOGS, 0,
                                               FLASH_MANAGER_OK);
    flash_manager_erase_sector_ExpectAndReturn(FLASH_REGION_ERROR_LOGS, 1,
                                               FLASH_MANAGER_OK);

    for (int i = 0; i < num_logs; i++) {
        TEST_ERROR_LOG_OK(error_log(ERROR_IMU_READ_ERROR,
                                    DATA_BSP_ERROR_CODE,
                                    BSP_ERROR_COMPONENT_FAILURE));
    }

    TEST_ERROR_LOG_OK(error_log_clear_logs());
}

void test_clear_error_log_then_write() {
    flash_manager_write_IgnoreAndReturn(FLASH_MANAGER_OK);
    HAL_GetTick_IgnoreAndReturn(0);
    int num_logs = 128;

    flash_manager_erase_sector_ExpectAndReturn(FLASH_REGION_ERROR_LOGS, 0,
                                               FLASH_MANAGER_OK);

    for (int i = 0; i < num_logs; i++) {
        TEST_ERROR_LOG_OK(error_log(ERROR_IMU_READ_ERROR,
                                    DATA_BSP_ERROR_CODE,
                                    BSP_ERROR_COMPONENT_FAILURE));
    }

    TEST_ERROR_LOG_OK(error_log_clear_logs());

    test_log_multiple_errors();
}

flash_manager_status_t flash_manager_write_stub(uint32_t region,
                                                uint32_t address,
                                                uint8_t *data,
                                                uint32_t size,
                                                int numCalls)
{
    if (region != FLASH_REGION_ERROR_LOGS) {
        TEST_FAIL_MESSAGE("flash_manager_write called with wrong region");
    }

    uint32_t max_address = FLASH_MANAGER_ERROR_LOG_SIZE_SECTORS * NOR_FLASH_SECTOR_SIZE - 1;

    if (address > max_address) {
        return FLASH_MANAGER_ERROR_OUT_OF_BOUNDS;
    }

    return FLASH_MANAGER_OK;
}


void test_log_full() {
    flash_manager_write_Stub(flash_manager_write_stub);
    HAL_GetTick_IgnoreAndReturn(0);

    // fill error logs
    int num_logs = FLASH_MANAGER_ERROR_LOG_SIZE_SECTORS * NOR_FLASH_SECTOR_SIZE / sizeof(error_log_row_t);

    for (int i = 0; i < num_logs; i++) {
        TEST_ERROR_LOG_OK(error_log(ERROR_IMU_READ_ERROR,
                                    DATA_BSP_ERROR_CODE,
                                    BSP_ERROR_COMPONENT_FAILURE));
    }

    TEST_ERROR_LOG_STATUS(error_log(ERROR_IMU_READ_ERROR,
                                    DATA_BSP_ERROR_CODE,
                                    BSP_ERROR_COMPONENT_FAILURE), ERROR_LOG_FULL);
}
