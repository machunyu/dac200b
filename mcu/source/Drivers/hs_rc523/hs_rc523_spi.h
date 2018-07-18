#ifndef  __HS_RC523_SPI_H__
#define  __HS_RC523_SPI_H__


#ifdef  __cplusplus
    extern "C" {
#endif
  

#include "Nano100Series.h"
  

uint8_t hs_spi_init(uint32_t mode, uint32_t speed);

uint8_t hs_spi_single_rw(uint8_t data);

uint32_t hs_spi_data_rw(uint8_t *txData, uint32_t len);


#ifdef  __cplusplus
}
#endif

#endif