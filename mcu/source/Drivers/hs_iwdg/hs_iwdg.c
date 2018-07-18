#include <intrinsics.h>
#include "hs_iwdg.h"


void hs_iwdg_init(WDT_Period_Typedef ePeriod)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    ///* Enable IP clock */
    CLK_EnableModuleClock(WDT_MODULE);
    //
    ///* Select IP clock source */
    CLK_SetModuleClock(WDT_MODULE, 0, 0);
    
    // WDT timeout every 2^14 WDT clock, disable system reset, disable wake up system
    WDT_Open((uint32_t)ePeriod, 0, TRUE, FALSE);
    
    /* Lock protected registers */
    SYS_LockReg();
}

void hs_iwdg_reset(WDT_Period_Typedef ePeriod)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    // stop WDT 
    WDT->CTL &= ~WDT_CTL_WTE_Msk;
    
    // WDT timeout every 2^14 WDT clock, disable system reset, disable wake up system
    WDT_Open((uint32_t)ePeriod, 0, TRUE, FALSE);
    
    /* Lock protected registers */
    SYS_LockReg();
}

void hs_iwdg_feet(void)
{
    __disable_interrupt();
    
    /* Unlock protected registers */
    SYS_UnlockReg();

    WDT_RESET_COUNTER();
    
    /* Lock protected registers */
    SYS_LockReg();
    
    __enable_interrupt();
}

void hs_iwdg_reset_system(void)
{
    while(1);
}


