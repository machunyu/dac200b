#include "hs_beat_timer.h"


static volatile uint32_t g_ulTicks = 0;


void hs_beat_timer_init(uint32_t ulPeriodInMs)
{
    uint32_t ulFrequency = 1000 / ulPeriodInMs;

    SYS_UnlockReg();
    
    CLK_EnableModuleClock(TMR1_MODULE);
    
    CLK_SetModuleClock(TMR1_MODULE, CLK_CLKSEL1_TMR1_S_HXT, 0);
    
    SYS_LockReg();
    
    // Set timer frequency to ulFrequency Hz
    TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, ulFrequency);

    // Enable timer interrupt
    TIMER_EnableInt(TIMER1);
    
    NVIC_EnableIRQ(TMR1_IRQn);

    g_ulTicks = 0;

    return;
}

void hs_beat_timer_start(void)
{
    TIMER_Start(TIMER1);
}

void hs_beat_timer_restart(void)
{
    TIMER_Stop(TIMER1);
    g_ulTicks = 0;
    TIMER_Start(TIMER1);
}

void hs_beat_timer_stop(void)
{
    TIMER_Stop(TIMER1);
}

void hs_beat_timer_inc_ticks(void)
{
    g_ulTicks++;
}

uint32_t hs_beat_timer_get_ticks(void)
{
    return g_ulTicks;
}

void hs_beat_timer_clr_ticks(void)
{
    g_ulTicks = 0;
}

