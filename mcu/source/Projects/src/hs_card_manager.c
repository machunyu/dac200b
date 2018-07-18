#include <stdio.h>
#include <string.h>
#include "hs_type.h"
#include "hs_iwdg.h"
#include "hs_flash.h"
#include "hs_rc523_api.h"
#include "hs_card_manager.h"
#include "hs_sys_mgr.h"

#define HS_SET_BIT(byte, bit)               ( (byte)  |=  (bit) )
#define HS_CLR_BIT(byte, bit)               ( (byte)  &= ~(bit) )
#define HS_IS_BIT_SET(byte, bit)            ( ((byte) & (bit)) == 0 ?  0 : 1)
#define HS_IS_BIT_CLR(byte, bit)            ( ((byte) & (bit)) == 0 ?  1 : 0)


Manager_Info_t g_CardMgrInfo;
uint8_t CardInfoBuf[5000];


uint8_t hs_card_manager_init(void)
{
    memset(&g_CardMgrInfo, 0xFF, sizeof(g_CardMgrInfo));
    return hs_flash_read(HS_CFG_CARD_MGR_ADDR, (uint8_t*)&g_CardMgrInfo, sizeof(g_CardMgrInfo));
}

uint32_t hs_card_manager_assign_pos(uint8_t ucCardType)
{
    uint32_t ulIndex = 0; 
    uint8_t *p = NULL;
    uint32_t ulCapacity = 0;
    uint8_t IDBytePos, IDBitPos;
    
    if(RC523_CARD_TYPE_A == ucCardType) {
        p = g_CardMgrInfo.IDABuf;
        ulCapacity = g_SysInfo.ulCardACapacity;
    } else {
        p = g_CardMgrInfo.IDBBuf;
        ulCapacity = g_SysInfo.ulCardBCapacity;
    }
    
    for(ulIndex=0; ulIndex<ulCapacity; ulIndex++) {
        IDBytePos = ulIndex / 8;
        IDBitPos  =  1 << (ulIndex % 8);
        if( HS_IS_BIT_SET(p[IDBytePos], IDBitPos) ) {
            return ulIndex;
        }
    }
    
    return 0xFFFF;
}

uint8_t hs_card_manager_update_pos(uint8_t ucCardType, uint32_t ulPos)
{
    uint8_t *p = NULL;
    uint8_t IDBytePos, IDBitPos;
    
    if(RC523_CARD_TYPE_A == ucCardType) {
        p = g_CardMgrInfo.IDABuf;
    } else {
        p = g_CardMgrInfo.IDBBuf;
    }

    IDBytePos = ulPos / 8;
    IDBitPos  =  1 << (ulPos % 8);
    
    HS_CLR_BIT(p[IDBytePos], IDBitPos);
    
    return hs_flash_write(HS_CFG_CARD_MGR_ADDR, (uint8_t*)&g_CardMgrInfo, sizeof(g_CardMgrInfo));
}

uint8_t hs_card_manager_clear_pos(uint8_t ucCardType, uint32_t ulPos)
{
    uint8_t *p = NULL;
    uint8_t IDBytePos, IDBitPos;
    
    if(RC523_CARD_TYPE_A == ucCardType) {
        p = g_CardMgrInfo.IDABuf;
    } else {
        p = g_CardMgrInfo.IDBBuf;
    }

    IDBytePos = ulPos / 8;
    IDBitPos  =  1 << (ulPos % 8);
    
    HS_SET_BIT(p[IDBytePos], IDBitPos);
    
    return hs_flash_write(HS_CFG_CARD_MGR_ADDR, (uint8_t*)&g_CardMgrInfo, sizeof(g_CardMgrInfo));
}

uint32_t hs_card_manager_get_card_volume(uint8_t ucCardType)
{
    uint32_t ulIndex = 0; 
    uint8_t *p = NULL;
    uint32_t ulVolume = 0;
    uint32_t ulCapacity;
    uint8_t IDBytePos, IDBitPos;
    
    if(RC523_CARD_TYPE_A == ucCardType) {
        p = g_CardMgrInfo.IDABuf;
        ulCapacity = g_SysInfo.ulCardACapacity;
    } else {
        p = g_CardMgrInfo.IDBBuf;
        ulCapacity = g_SysInfo.ulCardBCapacity;
    }
    
    for(ulIndex=0; ulIndex<ulCapacity; ulIndex++) {
        IDBytePos = ulIndex / 8;
        IDBitPos  =  1 << (ulIndex % 8);
        if( HS_IS_BIT_CLR(p[IDBytePos], IDBitPos) ) {
            ulVolume++;
        }
    }
    
    return ulVolume;
}

uint8_t hs_card_manager_erase_pos(void)
{
    memset(&g_CardMgrInfo, 0xFF, sizeof(g_CardMgrInfo));
    return hs_flash_write(HS_CFG_CARD_MGR_ADDR, (uint8_t*)&g_CardMgrInfo, sizeof(g_CardMgrInfo));
}

uint8_t hs_card_manager_insert(CardInfo_t info, uint32_t ulPos)
{
    uint8_t ret = 0;
    Card_A_TypeDef CardA;
    Card_B_TypeDef CardB;
    uint32_t ulStartPos;
    
    if(info.ucCardType == RC523_CARD_TYPE_A) {
        CardA.ucAttr = info.ucAttr;
        memcpy(CardA.ucCardID, info.ucCardID, HS_CFG_CARD_A_ID_LEN);
        ulStartPos = HS_CFG_CARD_A_ADDR + ulPos * sizeof(Card_A_TypeDef);
        ret |= hs_flash_write(ulStartPos, (uint8_t*)&CardA, sizeof(CardA));
        ret |= hs_card_manager_update_pos(RC523_CARD_TYPE_A, ulPos);
    } else if(info.ucCardType == RC523_CARD_TYPE_B) {
        CardB.ucAttr = info.ucAttr;
        memcpy(CardB.ucCardID, info.ucCardID, HS_CFG_CARD_B_ID_LEN);
        ulStartPos = HS_CFG_CARD_B_ADDR + ulPos * sizeof(Card_B_TypeDef);
        ret |= hs_flash_write(ulStartPos, (uint8_t*)&CardB, sizeof(CardB));
        ret |= hs_card_manager_update_pos(RC523_CARD_TYPE_B, ulPos);
    }
    
    return ret;
}

uint8_t hs_card_manager_batch(uint8_t ucCardType, uint32_t ulOffset, uint8_t *info, uint32_t count)
{
    uint8_t ret = 0, *p = NULL;
    uint32_t ulAddr, index;
    uint8_t IDBytePos, IDBitPos;
    
    if(ucCardType == RC523_CARD_TYPE_A) {
        p = g_CardMgrInfo.IDABuf;
        ulAddr = HS_CFG_CARD_A_ADDR + ulOffset * sizeof(Card_A_TypeDef);
        ret |= hs_flash_write(ulAddr, info, count * sizeof(Card_A_TypeDef));
        for(index=ulOffset; index<count; index++) {
            IDBytePos = index / 8;
            IDBitPos  = 1 << (index % 8);
            
            HS_CLR_BIT(p[IDBytePos], IDBitPos);
        }
        
        ret |= hs_flash_write(HS_CFG_CARD_MGR_ADDR, (uint8_t*)&g_CardMgrInfo, sizeof(g_CardMgrInfo));
        
    } else if(ucCardType == RC523_CARD_TYPE_B) {
        p = g_CardMgrInfo.IDBBuf;
        ulAddr = HS_CFG_CARD_B_ADDR + ulOffset * sizeof(Card_B_TypeDef);
        ret |= hs_flash_write(ulAddr, info, count * sizeof(Card_B_TypeDef));
        for(index=ulOffset; index<count; index++) {
            IDBytePos = index / 8;
            IDBitPos  = 1 << (index % 8);
            
            HS_CLR_BIT(p[IDBytePos], IDBitPos);
        }
        
        ret |= hs_flash_write(HS_CFG_CARD_MGR_ADDR, (uint8_t*)&g_CardMgrInfo, sizeof(g_CardMgrInfo));
    }
    
    return ret;
}


uint8_t hs_card_manager_modify(CardInfo_t info, uint32_t ulPos)
{
    uint8_t ret = 0;
    Card_A_TypeDef CardA;
    Card_B_TypeDef CardB;
    uint32_t ulStartPos;

    if(info.ucCardType == RC523_CARD_TYPE_A) {
        CardA.ucAttr = info.ucAttr;
        memcpy(CardA.ucCardID, info.ucCardID, HS_CFG_CARD_A_ID_LEN);
        ulStartPos = HS_CFG_CARD_A_ADDR + ulPos * sizeof(Card_A_TypeDef);
        ret |= hs_flash_write(ulStartPos, (uint8_t*)&CardA, sizeof(CardA));
    } else if(info.ucCardType == RC523_CARD_TYPE_B) {
        CardB.ucAttr = info.ucAttr;
        memcpy(CardB.ucCardID, info.ucCardID, HS_CFG_CARD_B_ID_LEN);
        ulStartPos = HS_CFG_CARD_B_ADDR + ulPos * sizeof(Card_B_TypeDef);
        ret |= hs_flash_write(ulStartPos, (uint8_t*)&CardB, sizeof(CardB));
    }

    return ret;
}

uint8_t hs_card_manager_delete(CardInfo_t info, uint32_t ulPos)
{
    uint8_t ret = 0;
    Card_A_TypeDef CardA;
    Card_B_TypeDef CardB;
    uint32_t ulStartPos;

    if(info.ucCardType == RC523_CARD_TYPE_A) {
        memset(&CardA, 0, sizeof(CardA));
        ulStartPos = HS_CFG_CARD_A_ADDR + ulPos * sizeof(Card_A_TypeDef);
        ret |= hs_flash_write(ulStartPos, (uint8_t*)&CardA, sizeof(CardA));
        ret |= hs_card_manager_clear_pos(RC523_CARD_TYPE_A, ulPos);
    } else if(info.ucCardType == RC523_CARD_TYPE_B) {
        memset(&CardB, 0, sizeof(CardB));
        ulStartPos = HS_CFG_CARD_B_ADDR + ulPos * sizeof(Card_B_TypeDef);
        ret |= hs_flash_write(ulStartPos, (uint8_t*)&CardB, sizeof(CardB));
        ret |= hs_card_manager_clear_pos(RC523_CARD_TYPE_B, ulPos);
    }

    return ret;
}


static uint32_t hs_card_manager_fast_match(uint8_t    *buf, 
                                           uint32_t   ulMatchCnt,
                                           CardInfo_t *pInInfo, 
                                           CardInfo_t *pOutInfo)
{
    uint32_t i, ulPos = 0;
    
    if(pInInfo->ucCardType == RC523_CARD_TYPE_A) {
        Card_A_TypeDef CardA;
        Card_A_TypeDef *p = (Card_A_TypeDef*)buf;
        
        CardA.ucAttr = pInInfo->ucAttr;
        memcpy(CardA.ucCardID, pInInfo->ucCardID, HS_CFG_CARD_A_ID_LEN);
        
        for(i=0; i<ulMatchCnt; i++) {
            if( !memcmp(p[i].ucCardID, CardA.ucCardID, HS_CFG_CARD_A_ID_LEN) ) {
                ulPos = i;
                if(pOutInfo != NULL) {
                    pOutInfo->ucAttr = p[i].ucAttr;
                    pOutInfo->ucCardType = RC523_CARD_TYPE_A;
                    pOutInfo->ucCardIDLen = HS_CFG_CARD_A_ID_LEN;
                    memcpy(pOutInfo->ucCardID, p[i].ucCardID, HS_CFG_CARD_A_ID_LEN);
                }
                return ulPos;
            }
        }
    } else {
        Card_B_TypeDef CardB;
        Card_B_TypeDef *p = (Card_B_TypeDef*)buf;
        
        CardB.ucAttr = pInInfo->ucAttr;
        memcpy(CardB.ucCardID, pInInfo->ucCardID, HS_CFG_CARD_B_ID_LEN);
        
        for(i=0; i<ulMatchCnt; i++) {
            if( !memcmp(p[i].ucCardID, CardB.ucCardID, HS_CFG_CARD_B_ID_LEN) ) {
                ulPos = i;
                if(pOutInfo != NULL) {
                    pOutInfo->ucAttr = p[i].ucAttr;
                    pOutInfo->ucCardType = RC523_CARD_TYPE_B;
                    pOutInfo->ucCardIDLen = HS_CFG_CARD_B_ID_LEN;
                    memcpy(pOutInfo->ucCardID, p[i].ucCardID, HS_CFG_CARD_B_ID_LEN);
                }
                return ulPos;
            }
        }
    }
    
    return 0xFFFF;
}

uint32_t hs_card_manager_match(CardInfo_t *pInInfo, CardInfo_t *pOutInfo)
{
    uint8_t ret = 0;
    uint32_t ulRet = 0;
    uint32_t i, ulStartPos;
    uint32_t ulMatchCountPerTimes;
    uint32_t ulMatchTimes, ulFlashReadSize;
    uint32_t ulPos = 0;


    if(pInInfo->ucCardType == RC523_CARD_TYPE_A) {
        ulFlashReadSize = 5000;
        ulMatchCountPerTimes = 5000 / 5;
        ulMatchTimes = HS_CFG_CARD_A_DEFAULT_CAP / ulMatchCountPerTimes;
        ulStartPos = HS_CFG_CARD_A_ADDR;
    } else {
        ulFlashReadSize = 4500;
        ulMatchCountPerTimes = 4500 / 9;
        ulMatchTimes = HS_CFG_CARD_B_DEFAULT_CAP / ulMatchCountPerTimes;
        ulStartPos = HS_CFG_CARD_B_ADDR;
    }

    for(i=0; i<ulMatchTimes; i++) {
        memset(CardInfoBuf, 0, sizeof(CardInfoBuf));
        ret = hs_flash_read(ulStartPos, CardInfoBuf, ulFlashReadSize);
        if( !ret ) {
            ulRet = hs_card_manager_fast_match(CardInfoBuf, ulMatchCountPerTimes, pInInfo, pOutInfo);
            if(ulRet != 0xFFFF) {
                ulPos += ulRet;
                return ulPos;
            } else {
                ulPos += ulMatchCountPerTimes;
                ulStartPos += ulFlashReadSize;
            }
        } else {
            ulPos += ulMatchCountPerTimes;
            ulStartPos += ulFlashReadSize;
            hs_printf(ERROR, "error : fail to hs_flash_read ....\r\n");
        }
        
        hs_iwdg_feet();
    }

    return 0xFFFF;
}

uint8_t hs_card_manager_erase(void)
{
    uint8_t ret = 0;
    uint32_t ulPage;
    uint32_t ulStartPages;
    
    ulStartPages = HS_CFG_CARD_A_ADDR / HS_CFG_FLASH_PAGE_SIZE;
    ulPage = (HS_CFG_CARD_END_ADDR - HS_CFG_CARD_A_ADDR) / HS_CFG_FLASH_PAGE_SIZE;

    memset(&g_CardMgrInfo, 0xFF, sizeof(g_CardMgrInfo));
    ret |= hs_flash_write(HS_CFG_CARD_MGR_ADDR, (uint8_t*)&g_CardMgrInfo, sizeof(g_CardMgrInfo));
    ret |= hs_flash_erase(ulStartPages, ulPage);

    return ret;
}

void hs_printf_card_info(CardInfo_t *pInfo)
{
    uint8_t ID[20] = { 0 };
    
    if(pInfo->ucCardType == RC523_CARD_TYPE_A) {
        hs_hex_2_string(pInfo->ucCardID, 4, ID);
    } else {
        hs_hex_2_string(pInfo->ucCardID, 8, ID);
    }
    
    //hs_printf(INFO, "\r\n====================== card info =======================\r\n");
    hs_printf(INFO, "type : %c\r\n", pInfo->ucCardType);
    hs_printf(INFO, "attr : %d\r\n", pInfo->ucAttr);
    hs_printf(INFO, "id   : %s\r\n", ID);
    //hs_printf(INFO, "========================================================\r\n");
}
