#ifndef  __HS_HEART_BEAT_H__
#define  __HS_HEART_BEAT_H__

#include "Nano100Series.h"
#include "hs_beat_timer.h"


#define HEART_BEAT_DEFAULT_PERIOD_MS      50000UL   


void hs_heart_beator_init(void);

void hs_heart_beator_start(uint32_t ulPeriod);

void hs_heart_beator_refresh(void);

void hs_heart_beator_stop(void);

void hs_heart_beator_beating(void);

uint8_t hs_heart_beator_is_enabled(void);

uint8_t hs_heart_beator_is_alive(void);

void hs_heart_beator_callback(void);


#endif
