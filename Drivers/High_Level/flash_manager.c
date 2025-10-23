#include "flash_manager.h"

#include <stdbool.h>
#include <stdio.h>

#include "nor_flash.h"

static bool nor_flash_initialised = false;

flash_manager_status_t flash_manager_init() {
    if (nor_flash_initialised) {
        return FLASH_MANAGER_OK;
    } else {
        nor_flash_initialised = true;

        return nor_flash_init();
    }

}

uint32_t flash_sector_address(uint32_t sector) {
    return sector * NOR_FLASH_SECTOR_SIZE;
}

bool address_in_region(flash_region_t region, uint32_t address) {
    uint32_t region_size = 0;

    switch (region) {
        case FLASH_REGION_DATA_LOGS:
            region_size = FLASH_MANAGER_DATA_LOG_SIZE_SECTORS * NOR_FLASH_SECTOR_SIZE;
            break;
        case FLASH_REGION_ERROR_LOGS:
            region_size = FLASH_MANAGER_ERROR_LOG_SIZE_SECTORS * NOR_FLASH_SECTOR_SIZE;
            break;
        default:
            return false;
    }

    if (address >= 0 && address < region_size) {
        return true;
    } else {
        return false;
    }
}

flash_manager_status_t flash_manager_write(flash_region_t region,
                                           uint32_t address,
                                           uint8_t *data,
                                           uint32_t size)
{
    uint32_t write_address = address;

    switch (region) {
        case FLASH_REGION_DATA_LOGS:
            if (!address_in_region(FLASH_REGION_DATA_LOGS, address)) {
                return FLASH_MANAGER_ERROR_OUT_OF_BOUNDS;
            }

            write_address += flash_sector_address(FLASH_MANAGER_DATA_LOG_SECTOR_START);
            break;
        case FLASH_REGION_ERROR_LOGS:
            if (!address_in_region(FLASH_REGION_ERROR_LOGS, address)) {
                return FLASH_MANAGER_ERROR_OUT_OF_BOUNDS;
            }

            write_address += flash_sector_address(FLASH_MANAGER_ERROR_LOG_SECTOR_START);
            break;
        default:
            return FLASH_MANAGER_ERROR_INVALID_REGION;
    }

    return nor_flash_write(write_address, data, size);
}
