#pragma once

#include "High_Level/flash_manager.h"

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#define LOG_ROW_START_MARKER 0xBEEDFACE
#define LOG_ROW_START_MARKER_SIZE_BYTES 4

typedef enum {
    LOG_UTILS_OK = 0,
    LOG_UTILS_LOG_FULL,
    LOG_UTILS_CORRUPT_LOG,
    LOG_UTILS_FLASH_READ_ERROR,
} log_utils_status_t;

typedef struct {
    uint32_t row_start_marker;
} __attribute__((packed)) log_utils_row_header_t;

static_assert(LOG_ROW_START_MARKER_SIZE_BYTES == sizeof(log_utils_row_header_t),
              "LOG_ROW_START_MARKER_SIZE_BYTES != sizeof(log_utils_row_header_t)");

log_utils_status_t log_utils_recover_log_pointer(flash_region_t *region,
                                                 uint32_t row_size_bytes,
                                                 uint32_t *num_entries,
                                                 uint8_t *read_buffer,
                                                 uint32_t read_buffer_size_rows);

//log_utils_status_t log_utils_send_over_uart
