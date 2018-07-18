/*
* (C) Wuhan Hongshi Technologies. Copyright (c) 2017

* @Author: machunyu  <chunyu.ma@hongshi-tech.com>
* @data: 2/17/2017
* @file: hs_delay.h
* @dir: Doorlock/hs_delay/hs_delay.h
*
* NOTICE: The intellectual and technical concepts and all information and source codes contained herein
* are, and remains the property of the copyright holders and may be covered by Patents,
* patents in process, and are protected by trade secret or copyright law.
*
* This code is proprietary code. Redistribution and use of source and binary forms, with or without modifications
* and any form of dissemination of this information or reproduction of this material, even partially, is strictly
* forbidden unless prior written permission obtained from the author and copyright holders.
*/


#ifndef _HS_DELAY_H_
#define _HS_DELAY_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "Nano100Series.h"

extern void hs_delay_init(void);

extern void hs_delay_us(uint16_t nUS);

extern void hs_delay_ms(uint32_t nMS);

extern void hs_iwdg_delay_ms(uint32_t nMS);

extern void hs_delay_s(uint32_t nS);

#ifdef __cplusplus
}
#endif
#endif