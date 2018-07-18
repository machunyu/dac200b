#ifndef  __HS_CONFIG_H__
#define  __HS_CONFIG_H__


#define HS_CFG_FLASH_PAGE_SIZE                512
#define HS_CFG_DATA_FLASH_BASE               (uint32_t)(32 * 1024)
#define HS_CFG_DATA_FLASH_END                (uint32_t)(63 * 1024)
#define HS_CFG_DATA_FLASH_SIZE               (uint32_t)(HS_CFG_DATA_FLASH_END - HS_CFG_DATA_FLASH_BASE)
#define HS_CFG_DATA_FLASH_PAGES              ((HS_CFG_DATA_FLASH_END - HS_CFG_DATA_FLASH_BASE) / HS_CFG_FLASH_PAGE_SIZE)


#define HS_CFG_RESET_CNT_ADDR                0
#define HS_CFG_UPDATE_INFO_ADDR              8
#define HS_CFG_SYS_INFO_ADDR                 512
#define HS_CFG_CARD_MGR_ADDR                 1024
#define HS_CFG_CARD_A_ADDR                  (3 * 1024)
#define HS_CFG_CARD_B_ADDR                  (29 * 1024)
#define HS_CFG_CARD_END_ADDR                (31 * 1024)


#define HS_CFG_N_W_N_B                      0
#define HS_CFG_WHITE_LIST                   1
#define HS_CFG_BLACK_LIST                   2

#define HS_CFG_LOCK_MODE_CLOSE              1
#define HS_CFG_LOCK_MODE_OPRN               2
#define HS_CFG_OPEN_METHOD_RELAY            1
#define HS_CFG_OPEN_METHOD_WEIGEN           2


#define HS_CFG_CARD_A_DEFAULT_CAP           5000
#define HS_CFG_CARD_B_DEFAULT_CAP           5000
#define HS_CFG_DEFAULT_OPEN_METHOD          HS_CFG_OPEN_METHOD_RELAY   // RELAY
#define HS_CFG_DEFAULT_OPEN_PERIOD          3   // 8S
#define HS_CFG_DEFAULT_LOCK_MODE            HS_CFG_LOCK_MODE_CLOSE   // ГЃБе

#define HS_CFG_UPDATE_INFO                (0xFFFFFFFF)
#define HS_CFG_UNUPDATE_INFO              (0xF8F8F8F8)


#define HS_CFG_RESET_TIME                  3000UL
#define HS_CFG_CARD_A_ID_LEN               4
#define HS_CFG_CARD_B_ID_LEN               8
#define HS_CFG_CARD_ID_LEN                 8


#endif
