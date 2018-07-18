#include <stdio.h>
#include <string.h>
#include "hs_rc523.h"
#include "hs_delay.h"
#include "hs_rc523_spi.h"


#define RC523_RESET             PB6
#define RC523_RESET_Port        PB
#define RC523_RESET_Pin	        BIT6

#define HIGH_BITs_OF_BYTE(byte)  ((byte) >> 4)
#define LOW_BITs_OF_BYTE(byte)   ((byte) & 0x0F)


/************************ SPI 数据传输 **************************/

static void hs_rc523_delay_ms(uint16_t nMs)
{
    hs_delay_ms(nMs);
}

static void rc523_write_reset_pin(uint8_t pinVal)
{
    RC523_RESET = pinVal ? 1 : 0;
}

static uint32_t rc523_rw_data(uint8_t *data, uint32_t len)
{
    return hs_spi_data_rw(data, len);
}

uint8_t rc523_ReadRawRC(uint8_t Address)
{
    uint8_t rx_data = 0;
    uint8_t tx_data = (Address<<1) & 0x7E | 0x80;
	
    rc523_rw_data(&tx_data, 1); 
    rc523_rw_data(&rx_data, 1);

    return rx_data;  
}

uint32_t rc523_WriteRawRC(uint8_t Address, uint8_t value)
{  
    uint8_t tx_data[2];
	
    tx_data[0] = (Address<<1) & 0x7E;
    tx_data[1] = value;
           
    return rc523_rw_data(tx_data, 2);
}

void rc523_Hex2String(uint8_t *in, uint16_t len, uint8_t *out)
{
    uint16_t i = 0;
    int8_t  hbit, lbit;
    int8_t  num[16] = {
    	'0', '1', '2', '3', '4', '5', '6', '7',
    	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };

    for(i=0; i<len; i++) {
    	hbit = HIGH_BITs_OF_BYTE(in[i]);
    	lbit = LOW_BITs_OF_BYTE(in[i]);
    	
    	out[2*i]   = num[hbit];
    	out[2*i+1] = num[lbit];
    }

    out[2*i]   = '\0';

    return;
}

void rc523_String2Hex(uint8_t *in, uint16_t len, uint8_t *out)
{
    uint16_t i = 0;
    uint8_t  hbit, lbit;

    for(i=0; i<len/2; i++) {
    	hbit = in[i*2];
    	lbit = in[i*2+1];
    	
        if(hbit >= '0' && hbit <= '9') {
            hbit = hbit - '0';
        } else if(hbit >= 'a' && hbit <= 'f') {
            hbit = hbit - 'a' + 10;
        } else if(hbit >= 'A' && hbit <= 'F') {
            hbit = hbit - 'A' + 10;
        }
        
        if(lbit >= '0' && lbit <= '9') {
            lbit = lbit - '0';
        } else if(lbit >= 'a' && lbit <= 'f') {
            lbit = lbit - 'a' + 10;
        } else if(lbit >= 'A' && lbit <= 'F') {
            lbit = lbit - 'A' + 10;
        }

    	out[i] = (hbit << 4) + lbit;
    }

    return;
}

void rc523_SetBitMask(uint8_t reg, uint8_t mask)  
{
    uint8_t tmp = 0x0;
    
    tmp = rc523_ReadRawRC(reg) | mask;
    rc523_WriteRawRC(reg, tmp | mask);  // set bit mask
}

void rc523_ClearBitMask(uint8_t reg, uint8_t mask)  
{
    int8_t  tmp = 0x0;
	
    tmp = rc523_ReadRawRC(reg) & (~mask);
    
    rc523_WriteRawRC(reg, tmp);  // clear bit mask
} 

void rc523_PcdAntennaOn(void)
{
    uint8_t i;
	
    i = rc523_ReadRawRC(TxControlReg);
    if (!(i & 0x03))
    {
        rc523_SetBitMask(TxControlReg, 0x03);
    }
}

void rc523_PcdAntennaOff(void)
{
    rc523_ClearBitMask(TxControlReg, 0x03);
}
      
void rc523_CalulateCRC(uint8_t *pIndata,uint8_t len,uint8_t *pOutData)
{
    uint8_t i,n;
	
    rc523_ClearBitMask(DivIrqReg,0x04);
    rc523_WriteRawRC(CommandReg,PCD_IDLE);
    rc523_SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
       rc523_WriteRawRC(FIFODataReg, *(pIndata+i));   

    rc523_WriteRawRC(CommandReg, PCD_CALCCRC);

    i = 0xFF;
	
    do {
        n = rc523_ReadRawRC(DivIrqReg);
        i--;
    } while ((i!=0) && !(n&0x04));
	
    pOutData[0] = rc523_ReadRawRC(CRCResultRegL);
    pOutData[1] = rc523_ReadRawRC(CRCResultRegM);
}
	  
int8_t rc523_PcdReset(void)
{
    //RF_POWER_ON;
    rc523_write_reset_pin(1);
    //hs_rc523_delay_ms(5);
    rc523_write_reset_pin(0);  
    hs_rc523_delay_ms(2);
    rc523_write_reset_pin(1);
    hs_rc523_delay_ms(5);
    rc523_WriteRawRC(CommandReg, PCD_RESETPHASE);
    hs_rc523_delay_ms(5);
    rc523_WriteRawRC(ModeReg, 0x3D);
    rc523_WriteRawRC(TReloadRegL, 30);
    rc523_WriteRawRC(TReloadRegH, 0);
    rc523_WriteRawRC(TModeReg, 0x8D);
    rc523_WriteRawRC(TPrescalerReg, 0x3E);  

    rc523_WriteRawRC(TxAutoReg, 0x40);

    return MI_OK;                  
}

int8_t rc523_PcdConfigISOType(uint8_t type)
{
   if (type == 'A')  //ISO14443_A
   { 
       rc523_ClearBitMask(Status2Reg, 0x08);
       rc523_WriteRawRC(ModeReg, 0x3D);//3F
       rc523_WriteRawRC(RxSelReg, 0x86);//84
       rc523_WriteRawRC(RFCfgReg,0x7F); //4F
       rc523_WriteRawRC(TReloadRegL, 30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
       rc523_WriteRawRC(TReloadRegH, 0);
       rc523_WriteRawRC(TModeReg, 0x8D);
       rc523_WriteRawRC(TPrescalerReg, 0x3E);
       hs_rc523_delay_ms(10);
       rc523_PcdAntennaOn();
   } else if( type == 'B') {  
       rc523_ClearBitMask(Status2Reg, 0x08);
       rc523_WriteRawRC(ModeReg, 0x2F);                                       // For 0xFFFF crc
       rc523_WriteRawRC(TReloadRegL, 30);
       rc523_WriteRawRC(TReloadRegH, 0);
       rc523_WriteRawRC(TModeReg, 0x8D);
       rc523_WriteRawRC(TPrescalerReg, 0x3E); 
       rc523_WriteRawRC(TxAutoReg, 0);  
    
       // ------------------------- 发送部分设置 ------------------------------
       rc523_WriteRawRC(GsNReg, 0xFF);                                        // TX输出电导设置
       rc523_WriteRawRC(CWGsCfgReg, 0x3F);
       rc523_WriteRawRC(ModGsCfgReg, 0x07);                                   // 调制指数设置RegModGsp,, TYPEB ModConductance 0x1A
       rc523_WriteRawRC(TxModeReg, 0x03);                                     // 编码器设置,106kbps,14443B
       rc523_WriteRawRC(BitFramingReg, 0x00);                                 // 调制脉宽,0x13->2.95us RegTypeBFraming ,,TYPEB
       rc523_WriteRawRC(AutoTestReg, 0x00);

       // ------------------------- 接收部分设置 ------------------------------
       rc523_WriteRawRC(RFCfgReg, 0x73);                                      // 0x59 RegRxControl1//73,
       rc523_WriteRawRC(RxModeReg, 0x0B);                                     // 解码器设置,,106kbps,14443B
       rc523_WriteRawRC(RxThresholdReg, 0x4D);                                // 高四位->最小信号强度，低三位->冲突最小信号强度,最大0xF7
 
       // ------------------------- TYPEB特征参数设定 -------------------------
       rc523_ClearBitMask(RxSelReg, 0x3F);                                     // TR0
       rc523_SetBitMask(RxSelReg, 0x08);
       rc523_ClearBitMask(TxModeReg, 0x80);                                   // 无CRC,无奇偶校验
       rc523_ClearBitMask(RxModeReg, 0x80);
       rc523_ClearBitMask(Status2Reg, 0x08);                                  // MFCrypto1On =0
               
       // ------------------------- 天线开关设置 ------------------------------
       rc523_PcdAntennaOff();
       hs_rc523_delay_ms(10);
       rc523_PcdAntennaOn();
   } else { 
       return -1; 
   }
   
   return MI_OK;
}

int8_t  rc523_PcdComMF522(uint8_t Command, 
                          uint8_t *pInData, 
                          uint8_t InLenByte,
                          uint8_t *pOutData, 
                          uint32_t *pOutLenBit)
{
    int8_t  status = MI_ERR;
    uint8_t irqEn   = 0x00;
    uint8_t waitFor = 0x00;
    uint8_t lastBits;
    uint8_t n;
    uint32_t i;
	
    switch (Command)
    {
        case PCD_AUTHENT:
            irqEn   = 0x12;
            waitFor = 0x10;
            break;
        case PCD_TRANSCEIVE:
            irqEn   = 0x77;
            waitFor = 0x30;
            break;
        default:
            break;
    }
   
    rc523_WriteRawRC(ComIEnReg, irqEn|0x80);
    rc523_ClearBitMask(ComIrqReg, 0x80);
    rc523_WriteRawRC(CommandReg, PCD_IDLE);
  
    rc523_SetBitMask(FIFOLevelReg, 0x80);
    
    for (i=0; i<InLenByte; i++)
       rc523_WriteRawRC(FIFODataReg, pInData[i]);    
   
    rc523_WriteRawRC(CommandReg, Command);
    
    if (Command == PCD_TRANSCEIVE)
        rc523_SetBitMask(BitFramingReg, 0x80); 
    
    i = 4000;
    do {
        n = rc523_ReadRawRC(ComIrqReg);
        i--;
    } while ((i!=0) && !(n&0x01) && !(n&waitFor));
	
    rc523_ClearBitMask(BitFramingReg, 0x80);

    if (i!=0) {    
        if(!(rc523_ReadRawRC(ErrorReg)&0x1B)) {
            status = MI_OK;
            if (n & irqEn & 0x01)
               status = MI_NOTAGERR;   
            if (Command == PCD_TRANSCEIVE) {
              	n = rc523_ReadRawRC(FIFOLevelReg);
             	lastBits = rc523_ReadRawRC(ControlReg) & 0x07;
                if (lastBits) {
                    *pOutLenBit = (n-1) * 8 + lastBits;   
	        } else {  
                    *pOutLenBit = n * 8;
	        }

               if (n == 0)  n = 1;   

               if (n > MAXRLEN)  n = MAXRLEN;   
               for (i=0; i<n; i++)
                  pOutData[i] = rc523_ReadRawRC(FIFODataReg); 
            }
        } else {   
           status = MI_ERR;  
        }
    }
   
   rc523_SetBitMask(ControlReg, 0x80);  // stop timer now
   rc523_WriteRawRC(CommandReg, PCD_IDLE);
   
   return status;
}

int8_t  rc523_PcdRequest(uint8_t req_code, uint8_t *pTagType)
{
    int8_t  status;  
    uint32_t  unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 
    
    rc523_ClearBitMask(Status2Reg, 0x08);
    rc523_WriteRawRC(BitFramingReg, 0x07);
    
    rc523_SetBitMask(TxControlReg, 0x03);
    
    ucComMF522Buf[0] = req_code;

    status = rc523_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 1, ucComMF522Buf, &unLen);
    
    if ((status == MI_OK) && (unLen == 0x10)) {    
        *pTagType     = ucComMF522Buf[0];
        *(pTagType+1) = ucComMF522Buf[1];
    } else { 
        status = MI_ERR;  
    }
    
    return status;
}

// 防冲撞
int8_t  rc523_PcdAnticoll(uint8_t *pSnr)
{
    int8_t  status;
    uint8_t i,snr_check=0;
    uint32_t  unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 
    
    rc523_ClearBitMask(Status2Reg,0x08);
    rc523_WriteRawRC(BitFramingReg,0x00);
    rc523_ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = rc523_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	for (i=0; i<4; i++) {   
            *(pSnr+i)  = ucComMF522Buf[i];
            snr_check ^= ucComMF522Buf[i];
        }
        if (snr_check != ucComMF522Buf[i])
            status = MI_ERR;    
    }
    
    rc523_SetBitMask(CollReg,0x80);
	
    return status;
}

int8_t rc523_PcdSelect(uint8_t *pSnr)
{
    int8_t  status;
    uint8_t i;
    uint32_t  unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
	
    rc523_CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    rc523_ClearBitMask(Status2Reg,0x08);

    status = rc523_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18)) {  
        status = MI_OK;
    } else { 
        status = MI_ERR; 
    }

    return status;
}

int8_t  rc523_PcdAuthState(uint8_t auth_mode,
			   uint8_t addr,
			   uint8_t *pKey,
			   uint8_t *pSnr)
{
    int8_t  status;
    uint32_t  unLen;
    uint8_t i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
        ucComMF522Buf[i+2] = *(pKey+i);   
    for (i=0; i<6; i++)
        ucComMF522Buf[i+8] = *(pSnr+i);   
    
    status = rc523_PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(rc523_ReadRawRC(Status2Reg) & 0x08)))
       status = MI_ERR;   
    
    return status;
}

int8_t  rc523_PcdRead(uint8_t addr, uint8_t *pData)
{
    int8_t  status;
    uint32_t  unLen;
    uint8_t i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    rc523_CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = rc523_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);
    if ((status == MI_OK) && (unLen == 0x90)) {
        for (i=0; i<16; i++)
            *(pData+i) = ucComMF522Buf[i];   
    } else {  
    	status = MI_ERR; 
    }
    
    return status;
}

int8_t rc523_PcdWrite(uint8_t addr, uint8_t *pData)
{
    int8_t  status;
    uint32_t  unLen;
    uint8_t i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    rc523_CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = rc523_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        status = MI_ERR;   
        
    if (status == MI_OK)
    {
        for (i=0; i<16; i++)
            ucComMF522Buf[i] = *(pData+i);   
		
        rc523_CalulateCRC(ucComMF522Buf, 16, &ucComMF522Buf[16]);

        status = rc523_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
           status = MI_ERR;   
    }    
	
    return status;
}

int8_t rc523_PcdHalt(void)
{
    int8_t  status;
    uint32_t  unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    rc523_CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);

    status = rc523_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    return status;
}

int8_t  rc523_PcdValue(uint8_t dd_mode,uint8_t addr,uint8_t *pValue)
{
    int8_t  status,i;
    uint32_t  unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = dd_mode;
    ucComMF522Buf[1] = addr;
    rc523_CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = rc523_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
       status = MI_ERR;   
        
    if (status == MI_OK)
    {
        //memcpy(ucComMF522Buf, pValue, 4);
        for (i=0; i<16; i++)
            ucComMF522Buf[i] = *(pValue+i);   
		
        rc523_CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
        unLen = 0;
        status = rc523_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
        if (status != MI_ERR)
            status = MI_OK;    
    }
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = PICC_TRANSFER;
        ucComMF522Buf[1] = addr;
        rc523_CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]); 
   
        status = rc523_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
           status = MI_ERR;   
    }

    return status;
}

int8_t  rc523_PcdBakValue(uint8_t sourceaddr, uint8_t goaladdr)
{
    int8_t  status;
    uint32_t  unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_RESTORE;
    ucComMF522Buf[1] = sourceaddr;
    rc523_CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = rc523_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
       status = MI_ERR;   
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = 0;
        ucComMF522Buf[1] = 0;
        ucComMF522Buf[2] = 0;
        ucComMF522Buf[3] = 0;
        rc523_CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
 
        status = rc523_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
        if (status != MI_ERR)
            status = MI_OK;    
    }
    
    if (status != MI_OK)
        return MI_ERR;   
    
    ucComMF522Buf[0] = PICC_TRANSFER;
    ucComMF522Buf[1] = goaladdr;

    rc523_CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = rc523_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
       status = MI_ERR;   

    return status;
}

int8_t rc523_RCRequestTypeB(void)
{
    int8_t status;
    uint32_t len;
    uint8_t buf[MAXRLEN] = {0};
    
    buf[0] = 0x05;
    buf[1] = 0x00;
    buf[2] = 0x00;
    buf[3] = 0x71; // crc
    buf[4] = 0xFF;

    status = rc523_PcdComMF522(PCD_TRANSCEIVE, buf, 5, buf, &len);// 判断回应的数据buf是否为"50,00,...."           
    
    return status;
}

int8_t rc523_RCGetUIDTypeB(uint8_t *pUID)
{
    int8_t status;
    uint32_t len, i;
    uint8_t buf[MAXRLEN] = {0};
    
    buf[0] = 0x00;
    buf[1] = 0x36;
    buf[2] = 0x00;
    buf[3] = 0x00;
    buf[4] = 0x08;
    buf[5] = 0x57; // crc
    buf[6] = 0x44;

    status = rc523_PcdComMF522(PCD_TRANSCEIVE, buf, 7, buf, &len); // 判断回应的数据是否为UID "....9000"
    if (status == MI_OK)
    {
        //10字节,前8字节位身份证唯一的ID号(UID),后两字节是0×90 0×00,用来判断得到的是否是身份证ID
	for(i=0; i<10;i++) {
	    pUID[i] = buf[i];
	}
    }
    
    return status;
}

int8_t rc523_RCATTRIBTypeB(void)
{
    int8_t status;
    uint32_t len;
    uint8_t buf[MAXRLEN] = {0};
    
    buf[ 0] = 0x1D;// 1d 00 00 00 00 00 08 01 08
    buf[ 1] = 0x00;// PUPI
    buf[ 2] = 0x00;
    buf[ 3] = 0x00;
    buf[ 4] = 0x00;
    buf[ 5] = 0x00;
    buf[ 6] = 0x08;
    buf[ 7] = 0x01;
    buf[ 8] = 0x08;
    buf[ 9] = 0xF3;// crc
    buf[10] = 0x10;

    status = rc523_PcdComMF522(PCD_TRANSCEIVE, buf, 11, buf, &len);// 判断回应的数据buf是否为"08"

    return status;
}

uint8_t rc523_init(void)
{
    hs_spi_init(RC_523_SPI_MODE, RC_523_SPI_SPEED);
    
    GPIO_SetMode(RC523_RESET_Port, RC523_RESET_Pin, GPIO_PMD_OUTPUT);
    
    rc523_PcdReset();
    rc523_PcdAntennaOff();
    rc523_PcdAntennaOn();

    return 0;
}
 
uint8_t rc523_release(void)
{
    SPI_Close(SPI0);
    
    return 0;
}

