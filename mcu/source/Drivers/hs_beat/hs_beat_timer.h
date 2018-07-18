#ifndef  __HS_BEAT_TIMER_H__
#define  __HS_BEAT_TIMER_H__


#include "Nano100Series.h"


extern void hs_beat_timer_init(uint32_t ulPeriodInMs);

extern void hs_beat_timer_restart(void);

extern void hs_beat_timer_start(void);
              
extern void hs_beat_timer_stop(void);
           
extern void hs_beat_timer_inc_ticks(void);

extern uint32_t hs_beat_timer_get_ticks(void);

extern void hs_beat_timer_clr_ticks(void);

#endif
