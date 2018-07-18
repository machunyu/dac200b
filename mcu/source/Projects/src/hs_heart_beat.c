#include "hs_heart_beat.h"
#include "hs_iwdg.h"
#include "hs_systick.h"


typedef struct {
    volatile uint8_t  ucIsEnable;
    volatile uint32_t ulStartTicks;
    volatile uint32_t ulBeatTime;
    volatile uint32_t ulBeatPeriod;
} Heart_Beator_t;


Heart_Beator_t  Beator;


void hs_heart_beator_init(void)
{
    Beator.ucIsEnable = 0;
    Beator.ulStartTicks = 0;
    Beator.ulBeatTime = 0;
    Beator.ulBeatPeriod = HEART_BEAT_DEFAULT_PERIOD_MS;
}

void hs_heart_beator_start(uint32_t ulPeriod)
{
    Beator.ucIsEnable = 1;
    Beator.ulStartTicks = hs_systick_get_tick();
    Beator.ulBeatTime = 0;
    Beator.ulBeatPeriod = ulPeriod;
}

void hs_heart_beator_stop(void)
{
    Beator.ucIsEnable = 0;
    Beator.ulStartTicks = 0;
    Beator.ulBeatTime = 0;
    Beator.ulBeatPeriod = HEART_BEAT_DEFAULT_PERIOD_MS;
}

void hs_heart_beator_refresh(void)
{
    uint32_t ulStartTicks = Beator.ulStartTicks;
    uint32_t ulCurTicks = hs_systick_get_tick();
    
    Beator.ulBeatTime += (ulCurTicks - ulStartTicks) * SYS_TICK_PERIOD_MS;
    Beator.ulStartTicks = ulCurTicks;

    return;
}

void hs_heart_beator_beating(void)
{
    Beator.ulStartTicks = hs_systick_get_tick();
    Beator.ulBeatTime = 0;
}

uint8_t hs_heart_beator_is_enabled(void)
{
    return Beator.ucIsEnable;
}

uint8_t hs_heart_beator_is_alive(void)
{
    uint32_t ulBeatTime   = Beator.ulBeatTime;
    uint32_t ulBeatPeriod = Beator.ulBeatPeriod;
    
    return (ulBeatTime < ulBeatPeriod) ? 1 : 0;
}

void hs_heart_beator_callback(void)
{
    hs_iwdg_reset_system();
}
