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

        if (nor_flash_init() != 0) {
            return FLASH_MANAGER_ERROR_FLASH_INIT;
        }
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

bool sector_in_region(flash_region_t region, uint32_t sector) {
    uint32_t region_size_sectors = 0;

    switch (region) {
        case FLASH_REGION_DATA_LOGS:
            region_size_sectors = FLASH_MANAGER_DATA_LOG_SIZE_SECTORS;
            break;
        case FLASH_REGION_ERROR_LOGS:
            region_size_sectors = FLASH_MANAGER_ERROR_LOG_SIZE_SECTORS;
            break;
        default:
            return false;
    }

    if (sector >= 0 && sector < region_size_sectors) {
        return true;
    } else {
        return false;
    }
}

flash_manager_status_t region_address_to_flash_address(flash_region_t region,
                                                       uint32_t address,
                                                       uint32_t *flash_address)
{
    if (!address_in_region(region, address)) {
        return FLASH_MANAGER_ERROR_OUT_OF_BOUNDS;
    }

    (*flash_address) = address;

    switch (region) {
        case FLASH_REGION_DATA_LOGS:
            (*flash_address) += flash_sector_address(FLASH_MANAGER_DATA_LOG_SECTOR_START);
            break;
        case FLASH_REGION_ERROR_LOGS:
            (*flash_address) += flash_sector_address(FLASH_MANAGER_ERROR_LOG_SECTOR_START);
            break;
        default:
            return FLASH_MANAGER_ERROR_INVALID_REGION;
    }

    return FLASH_MANAGER_OK;
}

flash_manager_status_t region_sector_to_flash_address(flash_region_t region,
                                                      uint32_t sector_number,
                                                      uint32_t *flash_address)
{
    uint32_t sector_number_absolute;

    if (!sector_in_region(region, sector_number)) {
        return FLASH_MANAGER_ERROR_OUT_OF_BOUNDS;
    }

    switch (region) {
        case FLASH_REGION_DATA_LOGS:
            sector_number_absolute = sector_number + FLASH_MANAGER_DATA_LOG_SECTOR_START;
            break;
        case FLASH_REGION_ERROR_LOGS:
            sector_number_absolute = sector_number + FLASH_MANAGER_ERROR_LOG_SECTOR_START;
            break;
        default:
            return FLASH_MANAGER_ERROR_INVALID_REGION;
    }

    uint32_t address = flash_sector_address(sector_number_absolute);

    (*flash_address) = address;
    return FLASH_MANAGER_OK;
}


flash_manager_status_t flash_manager_write(flash_region_t region,
                                           uint32_t address,
                                           uint8_t *data,
                                           uint32_t size)
{
    uint32_t write_address = 0;
    flash_manager_status_t status = region_address_to_flash_address(region,
                                                                    address,
                                                                    &write_address);
    if (status != FLASH_MANAGER_OK) {
        return status;
    }

    if (nor_flash_write(write_address, data, size) != 0) {
        return FLASH_MANAGER_ERROR_FLASH_WRITE;
    }
}

flash_manager_status_t flash_manager_read(flash_region_t region,
                                          uint32_t address,
                                          uint8_t *data,
                                          uint32_t size)
{
    uint32_t read_address = 0;
    flash_manager_status_t status = region_address_to_flash_address(region,
                                                                    address,
                                                                    &read_address);
    if (status != FLASH_MANAGER_OK) {
        return status;
    }

    if (nor_flash_read(read_address, data, size) != 0) {
        return FLASH_MANAGER_ERROR_FLASH_READ;
    }
}

flash_manager_status_t flash_manager_erase_sector(flash_region_t region,
                                                  uint32_t sector_number)
{
    uint32_t erase_address = 0;
    flash_manager_status_t status = region_sector_to_flash_address(region,
                                                                    sector_number,
                                                                    &erase_address);
    if (status != FLASH_MANAGER_OK) {
        return status;
    }

    if (nor_flash_erase_sector(erase_address) != 0) {
        return FLASH_MANAGER_ERROR_FLASH_ERASE;
    }
}
