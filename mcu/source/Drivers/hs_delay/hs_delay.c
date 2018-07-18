/*
* (C) Wuhan Hongshi Technologies. Copyright (c) 2017

* @Author: machunyu  <chunyu.ma@hongshi-tech.com>
* @data: 2/17/2017
* @file: hs_delay.c
* @dir: Doorlock/hs_delay/hs_delay.c
*
* NOTICE: The intellectual and technical concepts and all information and source codes contained herein
* are, and remains the property of the copyright holders and may be covered by Patents,
* patents in process, and are protected by trade secret or copyright law.
*
* This code is proprietary code. Redistribution and use of source and binary forms, with or without modifications
* and any form of dissemination of this information or reproduction of this material, even partially, is strictly
* forbidden unless prior written permission obtained from the author and copyright holders.
*/

#include "hs_delay.h"
#include "hs_iwdg.h"

volatile uint8_t fac_us = 0;


void hs_delay_init(void)
{
    fac_us = CLK_GetHCLKFreq()/4000000;
}

//*****************************************************************************
//
//! \brief Provides a small delay.
//!
//! \param ulCount is the number of delay loop iterations to perform.
//!
//! This function provides a means of generating a constant length delay.  It
//! is written in assembly to keep the delay consistent across tool chains,
//! avoiding the need to tune the delay based on the tool chain in use.
//!
//! The loop takes 3 cycles/loop.
//!
//! \return None.
//
//*****************************************************************************
#if defined(gcc) || defined(__GNUC__)
void __attribute__((naked))
SysCtlDelay(unsigned long ulCount)
{
    __asm("    sub     r0, #1\n"
          "    bne     SysCtlDelay\n"
          "    bx      lr");
}
#endif
#if defined(ewarm) || defined(__ICCARM__)
void
SysCtlDelay(unsigned long ulCount)
{
    __asm("    subs    r0, #1\n"
          "    bne.n   SysCtlDelay\n"
          "    bx      lr");
}
#endif
#if defined(rvmdk) || defined(__CC_ARM)
__asm void
SysCtlDelay(unsigned long ulCount)
{
    subs    r0, #1;
    bne     SysCtlDelay;
    bx      lr;
}
#endif

/**
 * @brief	us delay
 * @param	us delay time
 */
void hs_delay_us(uint16_t nUS)
{
    SysCtlDelay(nUS * fac_us);
}

/**
 * @brief	ms delay
 * @param	ms delay time
 */
void hs_delay_ms(uint32_t nMS)
{
    while(nMS--) {
        hs_delay_us(1000);
    }
}

void hs_iwdg_delay_ms(uint32_t nMS)
{
    while(nMS--) {
        hs_delay_us(1000);
        hs_iwdg_feet();
    }
}

/**
 * @brief	s delay
 * @param	s delay time
 */
void hs_delay_s(uint32_t nS)
{
    while(nS--) {
        hs_iwdg_delay_ms(1000);
    }
}
