/*
 * preprocess.h
 *
 *  Created on: June 22, 2025
 *      Author: richardmatthews
 */

#ifndef NOR_FLASH_H
#define NOR_FLASH_H

#include <stdint.h>

typedef struct
{
  uint32_t FlashSize;                        /*!< Size of the flash                             */
  uint32_t EraseSectorSize;                  /*!< Size of sectors for the erase operation       */
  uint32_t EraseSectorsNumber;               /*!< Number of sectors for the erase operation     */
  uint32_t EraseSubSectorSize;               /*!< Size of subsector for the erase operation     */
  uint32_t EraseSubSectorNumber;             /*!< Number of subsector for the erase operation   */
  uint32_t EraseSubSector1Size;              /*!< Size of subsector 1 for the erase operation   */
  uint32_t EraseSubSector1Number;            /*!< Number of subsector 1 for the erase operation */
  uint32_t ProgPageSize;                     /*!< Size of pages for the program operation       */
  uint32_t ProgPagesNumber;                  /*!< Number of pages for the program operation     */
} NOR_FLash_Info_t;


int32_t nor_flash_init();
int32_t nor_flash_get_info(uint32_t Instance, NOR_FLash_Info_t *pInfo);
int32_t nor_flash_get_status();
int32_t nor_flash_erase_sector(uint32_t blockAddress);
int32_t nor_flash_write(uint32_t writeAddress, uint8_t *pBuffer, uint32_t size);
int32_t nor_flash_read(uint32_t readAddress, uint8_t *pBuffer, uint32_t size);

#endif /* NOR_FLASH_H */
