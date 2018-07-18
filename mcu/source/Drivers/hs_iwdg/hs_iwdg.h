#ifndef  __HS_IWDG_H__
#define  __HS_IWDG_H__


#include "Nano100Series.h"


typedef enum {
    WDT_PERIOD_104_MS   = WDT_TIMEOUT_2POW4,
    WDT_PERIOD_108_MS   = WDT_TIMEOUT_2POW6, 
    WDT_PERIOD_128_MS   = WDT_TIMEOUT_2POW8, 
    WDT_PERIOD_204_MS   = WDT_TIMEOUT_2POW10,
    WDT_PERIOD_512_MS   = WDT_TIMEOUT_2POW12,
    WDT_PERIOD_1741_MS  = WDT_TIMEOUT_2POW14,
    WDT_PERIOD_6656_MS  = WDT_TIMEOUT_2POW16,
    WDT_PERIOD_26316_MS = WDT_TIMEOUT_2POW18
} WDT_Period_Typedef;


void hs_iwdg_init(WDT_Period_Typedef ePeriodInMs);

void hs_iwdg_reset(WDT_Period_Typedef ePeriodInMs);

void hs_iwdg_feet(void);

void hs_iwdg_reset_system(void);


#endif
