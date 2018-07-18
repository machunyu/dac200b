
#include "hs_key_timer.h"


static volatile uint32_t g_nTicks = 0;


static void hs_key_timer_init(uint32_t ulPeriodInMs)
{
    uint32_t ulFrequency = 1000 / ulPeriodInMs;

    SYS_UnlockReg();
    
    CLK_EnableModuleClock(TMR1_MODULE);
    
    CLK_SetModuleClock(TMR1_MODULE, CLK_CLKSEL1_TMR1_S_HIRC, 0);
    
    SYS_LockReg();
    
    // Set timer frequency to ulFrequency Hz
    TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, ulFrequency);

    // Enable timer interrupt
    TIMER_EnableInt(TIMER1);
    
    NVIC_EnableIRQ(TMR1_IRQn);

    // Start Timer 0
    //TIMER_Start(TIMER1);
}

void hs_key_timer_start(void)
{
    g_nTicks = 0;
    
    hs_key_timer_init(20);
    
    TIMER_Start(TIMER1);
}

void hs_key_timer_stop(void)
{
    TIMER_Stop(TIMER1);
    
    SYS_UnlockReg();
    CLK_EnableModuleClock(TMR1_MODULE);
    SYS_LockReg();
    
    // Enable timer interrupt
    TIMER_DisableInt(TIMER1);
    
    NVIC_DisableIRQ(TMR1_IRQn);
}

void hs_key_timer_increase_ticks(void)
{
    g_nTicks++;
}

uint32_t hs_key_timer_get_ticks(void)
{
    return g_nTicks;
}


