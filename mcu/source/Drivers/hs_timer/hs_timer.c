#include <stdio.h>
#include <string.h>
#include "hs_timer.h"
#include "hs_led.h"
#include "hs_beep.h"


static TimerCallBack_t TimerCallBack = NULL;
volatile uint32_t g_ulTicks = 0, g_nTicks = 0;


void hs_timer_init(void)
{
    uint32_t ulFrequency = 1000 / TIMER_PERIOD_MS;

    //SYS_UnlockReg();
    //
    //CLK_EnableModuleClock(TMR0_MODULE);
    //
    //CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HXT, 0);
    //
    //SYS_LockReg();
    
    // Set timer frequency to 1Hz
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, ulFrequency);

    // Enable timer interrupt
    TIMER_EnableInt(TIMER0);

    NVIC_EnableIRQ(TMR0_IRQn);
    
    g_ulTicks = 0;
}

void hs_timer_start(void)
{
    TIMER_Start(TIMER0);
}

void hs_timer_stop(void)
{
    TIMER_Stop(TIMER0);
}

void hs_timer_release(void)
{
    TIMER_Stop(TIMER0);
  
    TIMER_Close(TIMER0);
    
    SYS_UnlockReg();
    
    CLK_DisableModuleClock(TMR0_MODULE);
   
    SYS_LockReg();

    TIMER_DisableInt(TIMER0);
    
    NVIC_DisableIRQ(TMR0_IRQn);
} 

uint32_t hs_timer_get_ticks(void)
{
    return g_ulTicks;
}

void hs_timer_reg_callback(TimerCallBack_t CB)
{
    TimerCallBack = CB;
}

void hs_timer_unreg_callback(void)
{
    TimerCallBack = NULL;
}

void hs_timer_callback(void)
{
    g_ulTicks++;
    if( TimerCallBack ) {
        TimerCallBack();
    }
}

void hs_timer1_init(void)
{
    uint32_t ulFrequency = 1000 / 50;

    //SYS_UnlockReg();
    //
    //CLK_EnableModuleClock(TMR1_MODULE);
    //
    //CLK_SetModuleClock(TMR1_MODULE, CLK_CLKSEL1_TMR1_S_HXT, 0);
    //
    //SYS_LockReg();
    
    // Set timer frequency to 1Hz
    TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, ulFrequency);

    // Enable timer interrupt
    TIMER_EnableInt(TIMER1);

    NVIC_EnableIRQ(TMR1_IRQn);
    
    g_nTicks = 0;
}

void hs_timer1_start(void)
{
    g_nTicks = 0;
    TIMER_Start(TIMER1);
}

void hs_timer1_stop(void)
{
    TIMER_Stop(TIMER1);
}

void hs_timer1_release(void)
{
    TIMER_Stop(TIMER1);
  
    TIMER_Close(TIMER1);
    
    SYS_UnlockReg();
    
    CLK_DisableModuleClock(TMR1_MODULE);
   
    SYS_LockReg();

    TIMER_DisableInt(TIMER1);
    
    NVIC_DisableIRQ(TMR1_IRQn);
} 

