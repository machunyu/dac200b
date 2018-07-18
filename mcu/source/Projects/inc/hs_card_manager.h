#ifndef  __HS_CARD_MANAGER_H__
#define  __HS_CARD_MANAGER_H__


#include "Nano100Series.h"
#include "hs_config.h"


typedef struct {
    uint8_t ucAttr;
    uint8_t ucCardID[HS_CFG_CARD_A_ID_LEN];
} Card_A_TypeDef;   // sizeof(Card_A_TypeDef) = 5

typedef struct {
    uint8_t ucAttr;
    uint8_t ucCardID[HS_CFG_CARD_B_ID_LEN];
} Card_B_TypeDef;  // sizeof(Card_B_TypeDef) = 9

typedef struct {
    uint8_t ucCardType;
    uint8_t ucAttr;
    uint8_t ucIsOpen;
    uint8_t ucCardIDLen;
    uint8_t ucCardID[HS_CFG_CARD_ID_LEN];
} CardInfo_t;


typedef struct {
    uint8_t IDABuf[HS_CFG_CARD_A_DEFAULT_CAP >> 3];
    uint8_t IDBBuf[HS_CFG_CARD_B_DEFAULT_CAP >> 3];
} Manager_Info_t;


uint8_t hs_card_manager_init(void);

uint32_t hs_card_manager_assign_pos(uint8_t ucCardType);

uint8_t hs_card_manager_update_pos(uint8_t ucCardType, uint32_t ulPos);

uint8_t hs_card_manager_clear_pos(uint8_t ucCardType, uint32_t ulPos);

uint32_t hs_card_manager_get_card_volume(uint8_t ucCardType);

uint8_t hs_card_manager_erase_pos(void);

uint8_t hs_card_manager_insert(CardInfo_t info, uint32_t ulPos);

uint8_t hs_card_manager_modify(CardInfo_t info, uint32_t ulPos);

uint8_t hs_card_manager_delete(CardInfo_t info, uint32_t ulPos);

uint32_t hs_card_manager_match(CardInfo_t *pInInfo, CardInfo_t *pOutInfo);

uint8_t hs_card_manager_erase(void);

void hs_printf_card_info(CardInfo_t *pInfo);

uint8_t hs_card_manager_batch(uint8_t ucCardType, uint32_t ulOffset, uint8_t *info, uint32_t count);


#endif
