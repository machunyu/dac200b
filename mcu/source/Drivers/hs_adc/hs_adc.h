/*
* (C) Wuhan Hongshi Technologies. Copyright (c) 2016

* @Author: machunyu  <chunyu.ma@hongshi-tech.com>
* @data: 2/17/2017
* @file: hs_adc.h
* @dir: Irislock/src/driver/stm32/src/hs_iris.c
*
* NOTICE: The intellectual and technical concepts and all information and source codes contained herein
* are, and remains the property of the copyright holders and may be covered by Patents,
* patents in process, and are protected by trade secret or copyright law.
*
* This code is proprietary code. Redistribution and use of source and binary forms, with or without modifications
* and any form of dissemination of this information or reproduction of this material, even partially, is strictly
* forbidden unless prior written permission obtained from the author and copyright holders.
*/

#ifndef _HS_ADC_H_
#define _HS_ADC_H_

/**
 * @enum	HS_Power
 * Name a variety of power status
 */
typedef enum HS_Power
{
    HS_Normal_Power     = 0x00, /**< normal power status */
    HS_Low_Power        = 0x01, /**< Low power status */
    HS_Iris_Shutdown    = 0x02, /**< shutdown iris status */
    HS_System_Stop      = 0x04  /**< system stop running status */
} HS_Power;


extern void hs_adc_init(void);
extern void hs_adc_release(void);
extern HS_Power hs_power_probe(uint8_t isInit);
extern uint16_t hs_get_voltage(void);
extern uint8_t hs_power_get_status(void);
extern void hs_adc_test(void);


#endif
