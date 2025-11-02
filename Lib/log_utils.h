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

typedef enum {
    HANDLE_ROW_CONTINUE = 0,
    HANDLE_ROW_STOP,
} handle_row_return_t;

typedef handle_row_return_t (*handle_row_t)(uint8_t *row_data, uint32_t
                                            row_size_bytes, void *user_data);

log_utils_status_t log_utils_iterate_logs(flash_region_t *region,
                                          uint32_t row_size_bytes,
                                          uint8_t *read_buffer,
                                          uint32_t read_buffer_size_rows,
                                          uint32_t num_rows,
                                          handle_row_t handle_row,
                                          void *user_data);

log_utils_status_t log_utils_recover_log_pointer(flash_region_t *region,
                                                 uint32_t row_size_bytes,
                                                 uint32_t *num_entries,
                                                 uint8_t *read_buffer,
                                                 uint32_t read_buffer_size_rows);
