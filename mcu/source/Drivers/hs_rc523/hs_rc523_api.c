#include <stdio.h>
#include <string.h>
#include "hs_delay.h"
#include "hs_rc523.h"
#include "hs_rc523_api.h"


void hs_hex_2_string(uint8_t *in, uint16_t len, uint8_t *out)
{
    rc523_Hex2String(in, len, out);
}

void hs_string_2_hex(uint8_t *in, uint16_t len, uint8_t *out)
{
    rc523_String2Hex(in, len, out);
}

int8_t hs_rc523_config(RC523_Card_Type cardType)
{
    rc523_PcdReset();

    return rc523_PcdConfigISOType(cardType);
}

int8_t hs_rc523_get_card_id(RC523_Card_Type cardType, uint8_t *id)
{
     int8_t ret = MI_ERR;
     uint8_t CardID[10] = {0};

     if(cardType == RC523_CARD_TYPE_A) {
         uint8_t temp[4] = {0};

         if(MI_OK == rc523_PcdRequest(0x52, temp)) {
             if(MI_OK == rc523_PcdAnticoll(CardID)) {
                 //rc523_Hex2String(CardID, 4, id);
                 memcpy(id, CardID, 4);
                 ret = MI_OK;
             } 
         }
     } else {
         ret = rc523_RCRequestTypeB();
         if(ret == MI_OK) {
             ret |= rc523_RCATTRIBTypeB();
             ret |= rc523_RCGetUIDTypeB(CardID);
             if(ret == MI_OK) {
                 //rc523_Hex2String(CardID, 8, id);
               memcpy(id, CardID, 8);
             } 
         }
     }
     
     return ret;
}

int8_t hs_rc523_read_block(RC523_Mem_Block block, uint8_t *data, uint8_t *cardID)
{
    int8_t status;
    uint8_t tempBuffer[30];
    uint8_t MLastSelectedSnr[4]; 
    uint8_t tempCardID[10] = {0};
    uint8_t DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 

    
    status = rc523_PcdRequest(PICC_REQIDL, tempBuffer);  
    if(status != 0) {  
        status = rc523_PcdRequest(PICC_REQIDL, tempBuffer);  
        if(status!=0) {    
            printf("------> fail to rc523_PcdRequest \r\n");
            return status;  
        }  
    }

    status = rc523_PcdAnticoll(tempBuffer);  
    if(status != 0) {  
        printf("------> fail to rc523_PcdAnticoll \r\n");
        return status; 
    } 
    
    memcpy(MLastSelectedSnr, tempBuffer,4); 
    
    if(cardID != NULL) {
	memcpy(tempCardID, tempBuffer, 4);
	rc523_Hex2String(tempCardID, 4, cardID);
    }

    status = rc523_PcdSelect(MLastSelectedSnr);  
    if(status != MI_OK) {  
        printf("------> fail to rc523_PcdSelect \r\n");
        return status;  
    }  

    status = rc523_PcdAuthState(PICC_AUTHENT1A, (uint8_t)block, DefaultKey, MLastSelectedSnr); // 校验卡密码  
    if(status != MI_OK) {  
        printf("------> fail to rc523_PcdAuthState \r\n");
        return status;  
    }  

    status = rc523_PcdRead((uint8_t)block, data); 
    if(status != MI_OK) {  
        printf("------> fail to rc523_PcdAuthState \r\n");
    } 
    
    return status;
}

int8_t hs_rc523_write_block(RC523_Mem_Block block, uint8_t *data)
{
    int8_t status;
    uint8_t tempBuffer[30];
    uint8_t MLastSelectedSnr[4]; 
    uint8_t DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 
    
    status = rc523_PcdRequest(PICC_REQIDL, tempBuffer);  
    if(status != 0)  
    {  
        status = rc523_PcdRequest(PICC_REQIDL, tempBuffer);  
        if(status!=0)                 
        {    
            printf("------> fail to rc523_PcdRequest \r\n");
            return status;  
        }  
    }

    status = rc523_PcdAnticoll(tempBuffer);  
    if(status != 0)  
    {  
        printf("------> fail to rc523_PcdAnticoll \r\n");
        return status; 
    }  
	
    memcpy(MLastSelectedSnr,tempBuffer,4); 

    status = rc523_PcdSelect(MLastSelectedSnr);  
    if(status != MI_OK)  
    {  
        printf("------> fail to rc523_PcdSelect \r\n");
        return status;  
    }  

    status = rc523_PcdAuthState(PICC_AUTHENT1A, (uint8_t)block, DefaultKey, MLastSelectedSnr); // 校验卡密码  
    if(status != MI_OK)  
    {  
        printf("------> fail to rc523_PcdAuthState \r\n");
        return status;  
    }  
	
    status = rc523_PcdWrite((uint8_t)block, data);  
    if(status != MI_OK)  
    {  
        printf("------> fail to rc523_PcdWrite \r\n");
    } 

    return status;
}

uint8_t hs_rc523_init(void)
{
    return rc523_init();
}

uint8_t hs_rc523_release(void)
{
    return rc523_release();
}

