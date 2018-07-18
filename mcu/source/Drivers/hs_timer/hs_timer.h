#ifndef  __HS_TIMER_H__
#define  __HS_TIMER_H__


#include "Nano100Series.h"


#define TIMER_PERIOD_MS         50


#define TRIGG_MODE_UNLIMIT      1
#define TRIGG_MODE_LIMIT        2

#define TIMER_TRIGGER_0         0x01
#define TIMER_TRIGGER_1         0x02
#define TIMER_TRIGGER_ALL       0x03


typedef void (*TimerCallBack_t)(void);


extern volatile uint32_t g_nTicks;


void hs_timer_init(void);

void hs_timer_start(void);

void hs_timer_stop(void);

void hs_timer_release(void);

void hs_timer_callback(void);

void hs_timer_unreg_callback(void);

void hs_timer_reg_callback(TimerCallBack_t CB);

uint32_t hs_timer_get_ticks(void);

void hs_timer1_init(void);

void hs_timer1_start(void);

void hs_timer1_stop(void);

void hs_timer1_release(void);



#endif