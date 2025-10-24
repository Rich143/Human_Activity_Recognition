#include "High_Level/flash_manager.h"
#include "High_Level/nor_flash.h"
#include "unity.h"
#include "mock_nor_flash.h"

#include "flash_manager.h"
#include "b-u585i-iot02a-bsp/b_u585i_iot02a_errno.h"

#include <stdint.h>

// Helper macros for test status checking
#define TEST_FLASH_MANAGER_OK(expr) \
    do { \
        flash_manager_status_t status = (expr); \
        TEST_ASSERT_EQUAL_INT_MESSAGE(FLASH_MANAGER_OK, status, \
                                      "Expected FLASH_MANAGER_OK"); \
    } while (0)

#define TEST_FLASH_MANAGER_STATUS(expr, expected) \
    do { \
        flash_manager_status_t status = (expr); \
        TEST_ASSERT_EQUAL_INT_MESSAGE((expected), status, \
                                      "Unexpected flash_manager_status value"); \
    } while (0)

void setUp(void) { }

void tearDown(void) { }

void test_double_init() {
    nor_flash_init_ExpectAndReturn(BSP_ERROR_NONE);

    TEST_FLASH_MANAGER_OK(flash_manager_init());
    TEST_FLASH_MANAGER_OK(flash_manager_init());
}

void test_write_data_logs() {
    uint8_t buffer[32];

    nor_flash_write_ExpectAndReturn(0, buffer, sizeof(buffer), BSP_ERROR_NONE);

    TEST_FLASH_MANAGER_OK(flash_manager_write(FLASH_REGION_DATA_LOGS,
                                              0, buffer, sizeof(buffer)));
}

void test_write_data_logs_another_sector() {
    uint8_t buffer[32];

    uint32_t write_address = 3 * NOR_FLASH_SECTOR_SIZE + 5;

    nor_flash_write_ExpectAndReturn(write_address, buffer, sizeof(buffer), BSP_ERROR_NONE);

    TEST_FLASH_MANAGER_OK(flash_manager_write(FLASH_REGION_DATA_LOGS,
                                              write_address, buffer, sizeof(buffer)));
}

void test_write_data_logs_out_of_bounds() {
    uint8_t buffer[32];

    uint32_t write_address = FLASH_MANAGER_DATA_LOG_SIZE_SECTORS * NOR_FLASH_SECTOR_SIZE;

    TEST_FLASH_MANAGER_STATUS(FLASH_MANAGER_ERROR_OUT_OF_BOUNDS,
                              flash_manager_write(FLASH_REGION_DATA_LOGS,
                                              write_address, buffer, sizeof(buffer)));
}

void test_read_data_logs() {
    uint8_t buffer[32];

    nor_flash_read_ExpectAndReturn(0, buffer, sizeof(buffer), BSP_ERROR_NONE);
    nor_flash_read_IgnoreArg_pBuffer();

    TEST_FLASH_MANAGER_OK(flash_manager_read(FLASH_REGION_DATA_LOGS,
                                              0, buffer, sizeof(buffer)));
}

void test_write_error_logs() {
    uint32_t write_address =
        FLASH_MANAGER_ERROR_LOG_SECTOR_START * NOR_FLASH_SECTOR_SIZE;

    uint8_t buffer[32];

    nor_flash_write_ExpectAndReturn(write_address, buffer, sizeof(buffer),
                                    BSP_ERROR_NONE);

    TEST_FLASH_MANAGER_OK(flash_manager_write(FLASH_REGION_ERROR_LOGS,
                                              0, buffer, sizeof(buffer)));
}

void test_write_error_logs_another_sector() {
    uint32_t write_address_base =
        FLASH_MANAGER_ERROR_LOG_SECTOR_START * NOR_FLASH_SECTOR_SIZE;

    uint32_t write_address_region = NOR_FLASH_SECTOR_SIZE + 13;

    uint32_t write_address = write_address_base + write_address_region;

    uint8_t buffer[32];

    nor_flash_write_ExpectAndReturn(write_address, buffer, sizeof(buffer),
                                    BSP_ERROR_NONE);

    TEST_FLASH_MANAGER_OK(flash_manager_write(FLASH_REGION_ERROR_LOGS,
                                              write_address_region, buffer, sizeof(buffer)));
}

void test_write_error_logs_out_of_bounds() {
    uint32_t write_address_region =
        FLASH_MANAGER_ERROR_LOG_SIZE_SECTORS * NOR_FLASH_SECTOR_SIZE + 5;

    uint8_t buffer[32];

    TEST_FLASH_MANAGER_STATUS(FLASH_MANAGER_ERROR_OUT_OF_BOUNDS,
                              flash_manager_write(FLASH_REGION_ERROR_LOGS,
                                                  write_address_region,
                                                  buffer,
                                                  sizeof(buffer)));
}

void test_read_error_logs() {
    uint32_t read_address =
        FLASH_MANAGER_ERROR_LOG_SECTOR_START * NOR_FLASH_SECTOR_SIZE;

    uint8_t buffer[32];

    nor_flash_read_ExpectAndReturn(read_address, buffer, sizeof(buffer), BSP_ERROR_NONE);
    nor_flash_read_IgnoreArg_pBuffer();
    nor_flash_read_IgnoreArg_size();

    TEST_FLASH_MANAGER_OK(flash_manager_read(FLASH_REGION_ERROR_LOGS,
                                              0, buffer, sizeof(buffer)));
}


void test_data_logs_erase() {
    nor_flash_erase_sector_ExpectAndReturn(0, BSP_ERROR_NONE);

    TEST_FLASH_MANAGER_OK(flash_manager_erase_sector(FLASH_REGION_DATA_LOGS, 0));
}

void test_error_logs_erase() {
    uint32_t erase_address = FLASH_MANAGER_ERROR_LOG_SECTOR_START * NOR_FLASH_SECTOR_SIZE;

    nor_flash_erase_sector_ExpectAndReturn(erase_address, BSP_ERROR_NONE);

    TEST_FLASH_MANAGER_OK(flash_manager_erase_sector(FLASH_REGION_ERROR_LOGS, 0));
}

void test_data_logs_erase_another_sector() {
    uint32_t sector = 5;
    uint32_t erase_address = sector * NOR_FLASH_SECTOR_SIZE;

    nor_flash_erase_sector_ExpectAndReturn(erase_address, BSP_ERROR_NONE);

    TEST_FLASH_MANAGER_OK(flash_manager_erase_sector(FLASH_REGION_DATA_LOGS, sector));
}

void test_error_logs_erase_another_sector() {
    uint32_t sector = 1;
    uint32_t erase_address = FLASH_MANAGER_ERROR_LOG_SECTOR_START * NOR_FLASH_SECTOR_SIZE;
    erase_address += sector * NOR_FLASH_SECTOR_SIZE;

    nor_flash_erase_sector_ExpectAndReturn(erase_address, BSP_ERROR_NONE);

    TEST_FLASH_MANAGER_OK(flash_manager_erase_sector(FLASH_REGION_ERROR_LOGS, sector));
}

void test_data_logs_erase_out_of_bounds() {
    uint32_t sector = FLASH_MANAGER_DATA_LOG_SIZE_SECTORS;
    uint32_t erase_address = sector * NOR_FLASH_SECTOR_SIZE;

    TEST_FLASH_MANAGER_STATUS(FLASH_MANAGER_ERROR_OUT_OF_BOUNDS,
                              flash_manager_erase_sector(FLASH_REGION_DATA_LOGS, sector));
}

void test_error_logs_erase_out_of_bounds() {
    uint32_t sector = FLASH_MANAGER_ERROR_LOG_SIZE_SECTORS;
    uint32_t erase_address = FLASH_MANAGER_ERROR_LOG_SECTOR_START * NOR_FLASH_SECTOR_SIZE;
    erase_address += sector * NOR_FLASH_SECTOR_SIZE;

    TEST_FLASH_MANAGER_STATUS(FLASH_MANAGER_ERROR_OUT_OF_BOUNDS,
                              flash_manager_erase_sector(FLASH_REGION_ERROR_LOGS, sector));
}

void test_read_error() {
    uint8_t buffer[32];

    nor_flash_read_ExpectAndReturn(0, buffer, sizeof(buffer), BSP_ERROR_PERIPH_FAILURE);
    nor_flash_read_IgnoreArg_pBuffer();
    nor_flash_read_IgnoreArg_size();
    nor_flash_read_IgnoreArg_readAddress();

    TEST_FLASH_MANAGER_STATUS(FLASH_MANAGER_ERROR_FLASH_READ,
                              flash_manager_read(FLASH_REGION_ERROR_LOGS, 0, buffer, sizeof(buffer)));
}

void test_write_error() {
    uint8_t buffer[32];

    nor_flash_write_ExpectAndReturn(0, buffer, sizeof(buffer), BSP_ERROR_PERIPH_FAILURE);
    nor_flash_write_IgnoreArg_pBuffer();
    nor_flash_write_IgnoreArg_size();
    nor_flash_write_IgnoreArg_writeAddress();

    TEST_FLASH_MANAGER_STATUS(FLASH_MANAGER_ERROR_FLASH_WRITE,
                              flash_manager_write(FLASH_REGION_ERROR_LOGS, 0, buffer, sizeof(buffer)));
}

void test_erase_error() {
    nor_flash_erase_sector_ExpectAndReturn(0, BSP_ERROR_PERIPH_FAILURE);
    nor_flash_erase_sector_IgnoreArg_blockAddress();

    TEST_FLASH_MANAGER_STATUS(FLASH_MANAGER_ERROR_FLASH_ERASE,
                              flash_manager_erase_sector(FLASH_REGION_ERROR_LOGS, 0));
}
