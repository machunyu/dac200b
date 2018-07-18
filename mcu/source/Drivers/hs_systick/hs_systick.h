#ifndef  __HS_SYSTICK_H__
#define  __HS_SYSTICK_H__


#include "Nano100Series.h"


#define SYS_TICK_FREQ_HZ         20UL
#define SYS_TICK_PERIOD_MS       (1000 / SYS_TICK_FREQ_HZ)


typedef void (*SystcikCBack_t) (void);


void hs_systick_init(void);

void hs_systick_inc_ticks(void);

uint32_t hs_systick_get_tick(void);

void hs_systick_suspend(void);

void hs_systick_resume(void);

void hs_systick_callback(void);

void hs_systick_reg_callback(SystcikCBack_t callback);

void hs_systick_unreg_callback(void);


#endif
