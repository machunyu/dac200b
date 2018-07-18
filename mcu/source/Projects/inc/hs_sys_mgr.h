#ifndef  __HS_SYS_MGR1_H__
#define  __HS_SYS_MGR1_H__


#include "Nano100Series.h"


typedef struct {
    uint16_t usYear;
    uint8_t  ucMonth;
    uint8_t  ucDay;
    uint8_t  ucWeekday;
    uint8_t  ucHour;
    uint8_t  ucMinute;
    uint8_t  ucSecond;
} Date_t;

typedef struct {
    uint8_t  ucOpenMethod; // 开门方式 ： 1 -> 继电器，2 - > 维根
    uint8_t  ucLockMode;  // 常开常闭: 1 -> 常闭， 2 -> 常开
    uint8_t  ucOpenHoldPeriod; // 开门保持时间，单位为 S
    uint8_t  ucIrisPowCtl;   // 保留
    uint32_t ulCardACapacity;  // A 类卡的容量
    uint32_t ulCardBCapacity;  // B 类卡的容量
    uint16_t usHwVersion;  // 硬件版本
    uint16_t usSwVersion; // 软件版本
    uint32_t ulIrisSens;  //机器 ID
    uint32_t ulMach_id;  //机器 ID
    Date_t   sDate;   // 日期
} SysCfgTypeDef;


uint8_t hs_sys_info_init(void);

uint8_t hs_sys_config_update(SysCfgTypeDef *pSysConfig);

uint8_t hs_sys_config_saveall(void);

uint8_t hs_sys_get_conifg_info(SysCfgTypeDef **pSysConfig);

uint16_t hs_sys_get_hw_version(void);

uint16_t hs_sys_get_sw_version(void);

void hs_printf_config_info();

extern SysCfgTypeDef g_SysInfo;

#endif
