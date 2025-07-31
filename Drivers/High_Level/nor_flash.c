#include "High_Level/nor_flash.h"

#include "b-u585i-iot02a-bsp/b_u585i_iot02a_ospi.h"
#include "b-u585i-iot02a-bsp/b_u585i_iot02a_errno.h"

int32_t nor_flash_init() {
    BSP_OSPI_NOR_Init_t Flash;

    Flash.InterfaceMode = BSP_OSPI_NOR_OPI_MODE;
    Flash.TransferRate  = BSP_OSPI_NOR_STR_TRANSFER;

    return BSP_OSPI_NOR_Init(0, &Flash);
}

int32_t nor_flash_get_info(uint32_t Instance, NOR_FLash_Info_t *pInfo) {
    BSP_OSPI_NOR_Info_t info;

    int32_t status = BSP_OSPI_NOR_GetInfo(0, &info);
    if (status != BSP_ERROR_NONE) {
        return status;
    }

    pInfo->FlashSize = info.FlashSize;
    pInfo->EraseSectorSize = info.EraseSectorSize;
    pInfo->EraseSectorsNumber = info.EraseSectorsNumber;
    pInfo->EraseSubSectorSize = info.EraseSubSectorSize;
    pInfo->EraseSubSectorNumber = info.EraseSubSectorNumber;
    pInfo->EraseSubSector1Size = info.EraseSubSector1Size;
    pInfo->EraseSubSector1Number = info.EraseSubSector1Number;
    pInfo->ProgPageSize = info.ProgPageSize;
    pInfo->ProgPagesNumber = info.ProgPagesNumber;

    return status;
}

int32_t nor_flash_get_status() {
    return BSP_OSPI_NOR_GetStatus(0);
}

int32_t nor_flash_erase_sector(uint32_t blockAddress) {
    return BSP_OSPI_NOR_Erase_Block(0, blockAddress,
                                    MX25LM51245G_ERASE_64K);
}

int32_t nor_flash_write(uint32_t writeAddress, uint8_t *pBuffer, uint32_t size)
{
    return BSP_OSPI_NOR_Write(0, pBuffer,
                              writeAddress, size);
}

int32_t nor_flash_read(uint32_t readAddress, uint8_t *pBuffer, uint32_t size) {
    return BSP_OSPI_NOR_Read(0, pBuffer,
                             readAddress, size);
}
