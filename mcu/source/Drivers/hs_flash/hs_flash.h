#ifndef  __HS_FLASH_H__
#define  __HS_FLASH_H__


#include "Nano100Series.h"


//#define BOD_MODE_EN_BOD25          0UL
//#define BOD_MODE_EN_BOD20          1UL
//#define BOD_MODE_EN_BOD17          2UL
//#define BOD_MODE_DISABLE           3UL


uint8_t hs_flash_init(void);

uint8_t hs_flash_write(uint32_t addr, uint8_t *data, uint32_t len);

uint8_t hs_flash_read(uint32_t addr, uint8_t *data, uint32_t len);

uint32_t hs_flash_erase(uint32_t pageIndex, uint32_t pages);

//uint8_t hs_set_bod_mode(uint32_t mode);


#endif