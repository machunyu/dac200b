#include <string.h>
#include "hs_flash.h"
#include "hs_config.h"
#include "hs_delay.h"


static uint8_t hs_flash_set_base(uint32_t ulBaseAddr)
{
    uint8_t ret = 0;
    uint32_t ulConfig[2];

    
    SYS_UnlockReg();
    FMC_Open();
    
    if (FMC_ReadConfig(ulConfig, 2) < 0) {
        ret = 1;
        goto exit;
    }

    if ((!(ulConfig[0] & 0x1)) && (ulConfig[1] == ulBaseAddr)) {
        ret = 0;
        goto exit;
    }

    FMC_ENABLE_CFG_UPDATE();

    ulConfig[0] &= ~0x1;
    ulConfig[1] = ulBaseAddr;

    if (FMC_WriteConfig(ulConfig, 2) < 0) {
        ret = 1;
        goto exit;
    }

    SYS->IPRST_CTL1 = SYS_IPRST_CTL1_CHIP_RST_Msk;
    
    FMC_Close();
    SYS_LockReg();
    
    return 0;

exit:
    /* Disable FMC ISP function */
    FMC_Close();

    /* Lock protected registers */
    SYS_LockReg();
    
    return ret;    
}

#if 0
uint8_t hs_set_bod_mode(uint32_t mode)
{
    uint8_t ucCurBODMode = 0;
    uint32_t ulConfig[2];

    SYS_UnlockReg();
    FMC_Open();
    
    if (FMC_ReadConfig(ulConfig, 2) < 0) {
        FMC_Close();
        SYS_LockReg();
        return 1;
    }
    
    ucCurBODMode = (ulConfig[0] >> 19) & 0x3;
    if(ucCurBODMode != mode) {
        FMC_ENABLE_CFG_UPDATE();
    
        ulConfig[0] &= ~(0x3 << 19);
        ulConfig[0] |=   mode << 19;
        if (FMC_WriteConfig(ulConfig, 2) < 0) {
            FMC_Close();
            SYS_LockReg();
            return 1;
        }
        
        SYS->IPRST_CTL1 = SYS_IPRST_CTL1_CHIP_RST_Msk;
    }
    
    if(BOD_MODE_EN_BOD25 == mode) {
        SYS_ENABLE_BOD25_RST();
        SYS_ENABLE_BOD25();
    } else if(BOD_MODE_EN_BOD20 == mode) {
        SYS_ENABLE_BOD20_RST();
        SYS_ENABLE_BOD20();
    } else if(BOD_MODE_EN_BOD17 == mode) {
        SYS_ENABLE_BOD17_RST();
        SYS_ENABLE_BOD17();
    }
    
    FMC_Close();
    SYS_LockReg();
    
    return 0;
}
#endif

static uint8_t hs_flash_page_read(uint32_t pageAddr, uint32_t *data)
{
    uint16_t i;
    uint32_t ulStartAddr = pageAddr;
    
    for(i=0; i<HS_CFG_FLASH_PAGE_SIZE/4; i++) {
        *data++ = FMC_Read(ulStartAddr);
        ulStartAddr += 4;
    }
    
    return 0;
}

static uint8_t hs_flash_page_write(uint32_t pageAddr, uint32_t *data)
{
    uint16_t i;
    uint32_t ulStartAddr = pageAddr;
    
    for(i=0; i<HS_CFG_FLASH_PAGE_SIZE/4; i++) {
        FMC_Write(ulStartAddr, *data++);
        ulStartAddr += 4;
    }
    
    return 0;
}

uint8_t hs_flash_init(void)
{
    return hs_flash_set_base(HS_CFG_DATA_FLASH_BASE);
}

uint8_t hs_flash_write(uint32_t addr, uint8_t *data, uint32_t len)
{
    uint8_t PageData[512] = { 0 };
    uint32_t ulAddr, ulPageAddr, ulOffset, ulWriteSize;
    
    SYS_UnlockReg();
    FMC_Open();
    
    ulAddr = HS_CFG_DATA_FLASH_BASE + addr;
    ulOffset = (ulAddr % HS_CFG_FLASH_PAGE_SIZE);
    ulPageAddr = (ulAddr / HS_CFG_FLASH_PAGE_SIZE) * HS_CFG_FLASH_PAGE_SIZE;
    
    if((ulOffset+len) <= 512) {
        ulWriteSize = len;
    } else {
        ulWriteSize = (512 - ulOffset);
    }
    
    hs_flash_page_read(ulPageAddr, (uint32_t*)PageData);
    memcpy(PageData+ulOffset, data, ulWriteSize);
    FMC_Erase(ulPageAddr);
    hs_flash_page_write(ulPageAddr, (uint32_t*)PageData);
    
    data += ulWriteSize;
    len  -= ulWriteSize;
    if(len > 0) {
        ulPageAddr += HS_CFG_FLASH_PAGE_SIZE;
    }

    while(len > HS_CFG_FLASH_PAGE_SIZE) {
        FMC_Erase(ulPageAddr);
        hs_flash_page_write(ulPageAddr, (uint32_t*)data);
        data += HS_CFG_FLASH_PAGE_SIZE ;
        len  -= HS_CFG_FLASH_PAGE_SIZE;
        ulPageAddr += HS_CFG_FLASH_PAGE_SIZE;
    }
    
    if(len > 0) {
        hs_flash_page_read(ulPageAddr, (uint32_t*)PageData);
        memcpy(PageData, data, len);
        FMC_Erase(ulPageAddr);
        hs_flash_page_write(ulPageAddr, (uint32_t*)PageData);
    }

    FMC_Close();
    SYS_LockReg();
    
    return 0;
}

uint8_t hs_flash_read(uint32_t addr, uint8_t *data, uint32_t len)
{
    uint8_t PageData[512] = { 0 };
    uint32_t ulAddr, ulPageAddr, ulOffset, ulReadSize;
    
    SYS_UnlockReg();
    FMC_Open();
     
    ulAddr = HS_CFG_DATA_FLASH_BASE + addr;
    ulOffset = ulAddr % HS_CFG_FLASH_PAGE_SIZE;
    ulPageAddr = (ulAddr / HS_CFG_FLASH_PAGE_SIZE) * HS_CFG_FLASH_PAGE_SIZE;
    
    if((ulOffset+len) <= 512) {
        ulReadSize = len;
    } else {
        ulReadSize = (512 - ulOffset);
    }

    hs_flash_page_read(ulPageAddr, (uint32_t*)PageData);
    memcpy(data, PageData+ulOffset, ulReadSize);
    
    data += ulReadSize;
    len  -= ulReadSize;
    if(len > 0) {
        ulPageAddr += HS_CFG_FLASH_PAGE_SIZE;
    }
    
    while(len > HS_CFG_FLASH_PAGE_SIZE) {
        hs_flash_page_read(ulPageAddr, (uint32_t*)data);
        data += HS_CFG_FLASH_PAGE_SIZE;
        len  -= HS_CFG_FLASH_PAGE_SIZE;
        ulPageAddr += HS_CFG_FLASH_PAGE_SIZE;
    }
    
    if(len > 0) {
        hs_flash_page_read(ulPageAddr, (uint32_t*)PageData);
        memcpy(data, PageData, len);
    }

    FMC_Close();
    SYS_LockReg();
    
    return 0;
}

uint32_t hs_flash_erase(uint32_t pageIndex, uint32_t pages)
{
    int32_t ulPageAddr, i, n = 0, ret = 0;
    
    SYS_UnlockReg();
    FMC_Open();
     
    ulPageAddr = HS_CFG_DATA_FLASH_BASE + pageIndex * HS_CFG_FLASH_PAGE_SIZE;
    
    for(i=0; i<pages; i++) {
        ret = FMC_Erase(ulPageAddr);
        if(!ret)  {
            n++;
        }
        
        ulPageAddr += HS_CFG_FLASH_PAGE_SIZE;
    }
    
    FMC_Close();
    SYS_LockReg();
    
    return (pages - n);
}


