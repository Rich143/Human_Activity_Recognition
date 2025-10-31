#include "log_utils.h"
#include "High_Level/flash_manager.h"

#include <stdio.h>

#define ALL_ONES(type) ((type)~(type)0)
#define CLEARED_FLASH(type) ALL_ONES(type)

uint32_t get_region_size(flash_region_t *region)
{
    return region->num_sectors * NOR_FLASH_SECTOR_SIZE;
}

flash_manager_status_t read_in_flash(flash_region_t *region,
                   uint32_t address,
                   uint8_t *buffer, uint32_t read_buffer_size_rows,
                   uint32_t row_size_bytes)
{
    uint32_t size = row_size_bytes * read_buffer_size_rows;

    return flash_manager_read(region->region_idx, address, buffer, size);
}

uint32_t *get_row_start_marker(uint32_t row_idx, uint8_t *buf,
                                   uint32_t row_size_bytes)
{
    uint32_t offset = row_idx * row_size_bytes;
    uint32_t *row_start_ptr = (uint32_t *)&buf[offset];

    return row_start_ptr;
}

bool row_contains_row_start_marker(uint32_t row_idx, uint8_t *buf,
                                   uint32_t row_size_bytes)
{
    // TODO: check if flash is cleared if not row start marker
    uint32_t *row_start_ptr = get_row_start_marker(row_idx, buf,
                                                   row_size_bytes);

    return *row_start_ptr == LOG_ROW_START_MARKER;
}

bool row_start_marker_cleared(uint32_t row_idx, uint8_t *buf, uint32_t
                              row_size_bytes)
{
    uint32_t *row_start_ptr = get_row_start_marker(row_idx, buf,
                                                   row_size_bytes);

    return *row_start_ptr == CLEARED_FLASH(uint32_t);
}

log_utils_status_t log_utils_recover_log_pointer(flash_region_t *region,
                                                 uint32_t row_size_bytes,
                                                 uint32_t *num_entries,
                                                 uint8_t *read_buffer,
                                                 uint32_t read_buffer_size_rows)
{
    uint32_t readAddress = 0;
    bool found_log_pointer = false;

    *num_entries = 0;

    uint32_t region_size = get_region_size(region);


    while (!found_log_pointer && readAddress < region_size) {
        int32_t status = read_in_flash(region, readAddress,
                                       read_buffer,
                                       read_buffer_size_rows, row_size_bytes);
        if (status != 0) {
            return LOG_UTILS_FLASH_READ_ERROR;
        }

        for (int i = 0; i < read_buffer_size_rows; i++) {
            if (!row_contains_row_start_marker(i, read_buffer,
                                               row_size_bytes))
            {
                if (!row_start_marker_cleared(i, read_buffer, row_size_bytes)) {
                    printf("Corrupted log, marker is 0x%lx\n",
                           *get_row_start_marker(i, read_buffer,
                                                 row_size_bytes));
                    return LOG_UTILS_CORRUPT_LOG;
                }

                *num_entries += i;
                found_log_pointer = true;
                break;
            }
        }

        if (!found_log_pointer) {
            readAddress += row_size_bytes * read_buffer_size_rows;
            *num_entries += read_buffer_size_rows;
        }
    }

    if (found_log_pointer) {
        return LOG_UTILS_OK;
    } else {
        return LOG_UTILS_LOG_FULL;
    }
}
