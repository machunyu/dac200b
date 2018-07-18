#include <intrinsics.h>
#include "hs_systick.h"


static volatile uint32_t ulTicks = 0;
static volatile SystcikCBack_t SysTickCB = NULL;


// period : 20 ms
void hs_systick_init(void)
{
    SysTick_Config(SystemCoreClock / SYS_TICK_FREQ_HZ);
}

void hs_systick_inc_ticks(void)
{
    ulTicks++;
}

uint32_t hs_systick_get_tick(void)
{
    return ulTicks;
}

void hs_systick_suspend(void)
{
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

void hs_systick_resume(void)
{
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

void hs_systick_callback(void)
{
    if( SysTickCB ) {
        uint32_t cnt = ulTicks % 10;
        
        if( !cnt ) {
            SysTickCB();
        }
    }
}

void hs_systick_reg_callback(SystcikCBack_t callback)
{
    __disable_interrupt();
    
    SysTickCB = callback;
    
    __enable_interrupt();
}

void hs_systick_unreg_callback(void)
{
    __disable_interrupt();
    
    SysTickCB = NULL;
    
    __enable_interrupt();
}
