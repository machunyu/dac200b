#include "hs_rc523_spi.h"


uint8_t hs_spi_init(uint32_t mode, uint32_t speed)
{
    //SYS_UnlockReg();
    //
    //CLK_EnableModuleClock(SPI0_MODULE);
    //
    ///* Set multi function pin for SPI1 */
    //SYS->PC_L_MFP = SYS_PC_L_MFP_PC0_MFP_SPI0_SS0   | SYS_PC_L_MFP_PC1_MFP_SPI0_SCLK | \
    //                SYS_PC_L_MFP_PC2_MFP_SPI0_MISO0 | SYS_PC_L_MFP_PC3_MFP_SPI0_MOSI0;
    //
    //SYS_LockReg();

    /* Configure SPI0 as a master, 8-bit transaction*/
    SPI_Open(SPI0, SPI_MASTER, mode, 8, speed);

    SPI_DisableAutoSS(SPI0);

    SPI_SET_MSB_FIRST(SPI0);
    
    return 0;
}

uint8_t hs_spi_single_rw(uint8_t data)
{
    uint8_t ret = 0;
    uint32_t rxData, txDada = data;
    
    SPI_WRITE_TX0(SPI0, txDada);
    
    SPI_TRIGGER(SPI0);
    
    while(SPI_IS_BUSY(SPI0));

    rxData = SPI_READ_RX0(SPI0);
    
    ret = rxData & 0xff;
    
    return ret;
}

uint32_t hs_spi_data_rw(uint8_t *txData, uint32_t len)
{
     uint32_t i, cnt = 0;

     SPI_SET_SS0_LOW(SPI0);  // CS = 0

     for(i=0; i<len; i++, txData++) {
         *txData = hs_spi_single_rw(*txData);
         cnt++;
     }

     SPI_SET_SS0_HIGH(SPI0); // CS = 1

     return cnt;
}

