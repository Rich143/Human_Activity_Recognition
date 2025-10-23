/*
 * flash_manager.h
 *
 * Manages the external NOR flash
 *
 *  Created on: Oct 23, 2025
 *      Author: richardmatthews
 */

#ifndef NOR_FLASH_MANAGER_H
#define NOR_FLASH_MANAGER_H

#include <stdint.h>
#include "nor_flash.h"

#define FLASH_MANAGER_DATA_LOG_SECTOR_START 0
#define FLASH_MANAGER_DATA_LOG_SIZE_SECTORS 1020

#define FLASH_MANAGER_ERROR_LOG_SECTOR_START \
    (FLASH_MANAGER_DATA_LOG_SECTOR_START + FLASH_MANAGER_DATA_LOG_SIZE_SECTORS)

#define FLASH_MANAGER_ERROR_LOG_SIZE_SECTORS \
    (NOR_FLASH_NUM_SECTORS - FLASH_MANAGER_DATA_LOG_SIZE_SECTORS)

typedef enum {
    FLASH_MANAGER_OK,
    FLASH_MANAGER_ERROR_INVALID_REGION,
    FLASH_MANAGER_ERROR_OUT_OF_BOUNDS,
    FLASH_MANAGER_ERROR
} flash_manager_status_t;

typedef enum {
    FLASH_REGION_DATA_LOGS =0,
    FLASH_REGION_ERROR_LOGS,
} flash_region_t;

flash_manager_status_t flash_manager_init();

flash_manager_status_t flash_manager_read(flash_region_t region,
                                          uint32_t address,
                                          uint8_t *data,
                                          uint32_t size);

flash_manager_status_t flash_manager_write(flash_region_t region,
                                           uint32_t address,
                                           uint8_t *data,
                                           uint32_t size);

flash_manager_status_t flash_manager_erase_sector(flash_region_t region,
                                                  uint32_t sector_number);

#endif /* NOR_FLASH_MANAGER_H */
