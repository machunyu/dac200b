#ifndef _HS_KEY_TIMER_H_
#define _HS_KEY_TIMER_H_

#include "Nano100Series.h"

#define KEY_TIMER_PERIOD_MS     20           
#define KEY_PRESSED_TICKS       (50 * 3)     // 3s


extern void hs_key_timer_start(void);

extern void hs_key_timer_stop(void);

extern void hs_key_timer_increase_ticks(void);

extern uint32_t hs_key_timer_get_ticks(void);


#endif