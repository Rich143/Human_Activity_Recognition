/*
 * nor_flash.h
 *
 *  Created on: June 22, 2025
 *      Author: richardmatthews
 */

#ifndef NOR_FLASH_H
#define NOR_FLASH_H

#include <stdint.h>

#define NOR_FLASH_SIZE           (uint32_t)(512*1024*1024/8) /* 512 Mbits => 64MBytes        */
#define NOR_FLASH_SECTOR_SIZE    (uint32_t)(64 * 1024)       /* 1024 sectors of 64KBytes     */
#define NOR_FLASH_SUBSECTOR_SIZE (uint32_t)(4  * 1024)       /* 16384 subsectors of 4KBytes  */
#define NOR_FLASH_NUM_SECTORS    (uint32_t)(NOR_FLASH_SIZE/NOR_FLASH_SECTOR_SIZE)

#define NOR_FLASH_PAGE_SIZE      (uint32_t)256                /* 262144 pages of 256 Bytes   */

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
int32_t nor_flash_erase_chip();
int32_t nor_flash_write(uint32_t writeAddress, uint8_t *pBuffer, uint32_t size);
int32_t nor_flash_read(uint32_t readAddress, uint8_t *pBuffer, uint32_t size);
int32_t nor_flash_check_cleared();

#endif /* NOR_FLASH_H */
