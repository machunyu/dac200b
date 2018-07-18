#include <stdio.h>
#include <string.h>
#include "hs_sys_mgr.h"
#include "hs_flash.h"
#include "hs_config.h"
#include "hs_type.h"
#include "hs_timer.h"
#include "hs_timer_list.h"


#define MAKE_VERSION(Main, Sub)   ( (Main <<  8) | (Sub) )



#define HW_VERSION             MAKE_VERSION(0x01, 0x03)
#define SW_VERSION             MAKE_VERSION(0x03, 0x05)


SysCfgTypeDef g_SysInfo;
static const char WeekDay[7][5] = { "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN" };


static void hs_print_date(Date_t *pDate)
{
    char ucDateString[50];

    memset(ucDateString, 0, 50);
    if(pDate->ucWeekday > 0x7) pDate->ucWeekday = 0;

    sprintf(ucDateString, "%ld-%d-%d %s %2d:%2d:%2d", pDate->usYear, pDate->ucMonth, pDate->ucDay,
                                                      WeekDay[pDate->ucWeekday],
                                                      pDate->ucHour, pDate->ucMinute, pDate->ucSecond);
    hs_printf(INFO, "%s", ucDateString);
}


uint8_t hs_sys_info_init(void)
{
    unsigned char ret;
    memset(&g_SysInfo, 0, sizeof(g_SysInfo));
    ret = hs_flash_read(HS_CFG_SYS_INFO_ADDR, (uint8_t*)&g_SysInfo, sizeof(SysCfgTypeDef));

    if(g_SysInfo.ucLockMode == 0xFF) g_SysInfo.ucLockMode = HS_CFG_DEFAULT_LOCK_MODE;
    if(g_SysInfo.ucOpenHoldPeriod == 0xFF) g_SysInfo.ucOpenHoldPeriod = HS_CFG_DEFAULT_OPEN_PERIOD;
    if(g_SysInfo.ucOpenMethod == 0xFF) g_SysInfo.ucOpenMethod = HS_CFG_DEFAULT_OPEN_METHOD;
    if(g_SysInfo.ucIrisPowCtl == 0xFF) g_SysInfo.ucIrisPowCtl = 0;
    if(g_SysInfo.ulCardACapacity == 0xFFFFFFFF) g_SysInfo.ulCardACapacity = 5000;
    if(g_SysInfo.ulCardBCapacity == 0xFFFFFFFF) g_SysInfo.ulCardBCapacity = 5000;
    
    if(g_SysInfo.usHwVersion == 0xFFFF) g_SysInfo.usHwVersion = HW_VERSION;
    if(g_SysInfo.usSwVersion == 0xFFFF) g_SysInfo.usSwVersion = SW_VERSION;
    
    return ret;
}

uint8_t hs_sys_config_update(SysCfgTypeDef *pSysConfig)
{
    Date_t EmptyDate;

    memset(&EmptyDate, 0, sizeof(EmptyDate));

    pSysConfig->ucOpenMethod ? g_SysInfo.ucOpenMethod = pSysConfig->ucOpenMethod : 0;
    pSysConfig->ucLockMode ? g_SysInfo.ucLockMode = pSysConfig->ucLockMode : 0;
    pSysConfig->ulMach_id ? g_SysInfo.ulMach_id = pSysConfig->ulMach_id : 0;

    if(pSysConfig->ulCardACapacity) {
        g_SysInfo.ulCardACapacity = pSysConfig->ulCardACapacity <= HS_CFG_CARD_A_DEFAULT_CAP ? pSysConfig->ulCardACapacity : HS_CFG_CARD_A_DEFAULT_CAP;
    }
    
    if(pSysConfig->ulCardBCapacity) {
        g_SysInfo.ulCardBCapacity = pSysConfig->ulCardBCapacity <= HS_CFG_CARD_B_DEFAULT_CAP ? pSysConfig->ulCardBCapacity : HS_CFG_CARD_B_DEFAULT_CAP;
    }
    
    if(pSysConfig->ucOpenHoldPeriod) {
        AppTimer_t data;
        
        memset(&data, 0, sizeof(data));
        g_SysInfo.ucOpenHoldPeriod = pSysConfig->ucOpenHoldPeriod;
        data.ulReloadTime = pSysConfig->ucOpenHoldPeriod * 1000 / TIMER_PERIOD_MS;
        hs_timer_node_update(CLSOE_TIMER_ID, &data);
    }
    
    if( memcmp(&pSysConfig->sDate, &EmptyDate, sizeof(Date_t)) ) {
        memcpy(&g_SysInfo.sDate, &pSysConfig->sDate, sizeof(Date_t));
    }
    
    return 0;
}

uint8_t hs_sys_config_saveall(void)
{
    return hs_flash_write(HS_CFG_SYS_INFO_ADDR, (uint8_t*)&g_SysInfo, sizeof(SysCfgTypeDef));
}

uint8_t hs_sys_get_conifg_info(SysCfgTypeDef **pSysConfig)
{
    *pSysConfig = &g_SysInfo;
    
    return 0;
}

uint16_t hs_sys_get_hw_version(void)
{
    return HW_VERSION;
}

uint16_t hs_sys_get_sw_version(void)
{
    return SW_VERSION;
}

void hs_printf_config_info(void)
{
    hs_printf(INFO, "\r\n================ config info ==================== \r\n");
    hs_printf(INFO, "OpenMethod     : %ld \r\n", g_SysInfo.ucOpenMethod);
    hs_printf(INFO, "LockMode       : %ld \r\n", g_SysInfo.ucLockMode);
    hs_printf(INFO, "OpenHoldPeriod : %ld \r\n", g_SysInfo.ucOpenHoldPeriod);
    hs_printf(INFO, "CardACapacity  : %ld \r\n", g_SysInfo.ulCardACapacity);
    hs_printf(INFO, "CardBCapacity  : %ld \r\n", g_SysInfo.ulCardBCapacity);
    hs_printf(INFO, "HwVersion      : %ld \r\n", g_SysInfo.usHwVersion);
    hs_printf(INFO, "SwVersion      : %ld \r\n", g_SysInfo.usSwVersion);
    hs_printf(INFO, "Mach_id        : %x \r\n",  g_SysInfo.ulMach_id);
    hs_printf(INFO, "Date           : ");
    hs_print_date(&g_SysInfo.sDate);
    hs_printf(INFO, "\r\n================================================= \r\n");
}
