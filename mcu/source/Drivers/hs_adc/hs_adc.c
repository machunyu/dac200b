/*
* (C) Wuhan Hongshi Technologies. Copyright (c) 2016

* @Author: machunyu  <chunyu.ma@hongshi-tech.com>
* @data: 2/17/2017
* @file: hs_adc.c
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

#include <stdio.h>
#include "Nano100Series.h"
#include "hs_adc.h"
#include "key_led.h"
#include "hs_delay.h"


#define HS_DEF_MAX(a, b)    ((a) > (b) ? (a) : (b))
#define HS_DEF_MIN(a, b)    ((a) < (b) ? (a) : (b))


static volatile HS_Power status = HS_Normal_Power;


void hs_adc_init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();
    
    /* Enable ADC clock */
    CLK_EnableModuleClock(ADC_MODULE);
    
    CLK_SetModuleClock(ADC_MODULE, CLK_CLKSEL1_ADC_S_HIRC, CLK_ADC_CLK_DIVIDER(1));
    
    /* Set PA multi-function pins for ADC */
    SYS->PA_L_MFP &= ~(SYS_PA_L_MFP_PA4_MFP_Msk);
    SYS->PA_L_MFP |= SYS_PA_L_MFP_PA4_MFP_ADC_CH4;
    
    /* Disable PA.4 digital input path */
    PA->OFFD |= ((1<<4) << GP_OFFD_OFFD_Pos);
    
    /* Lock protected registers */
    SYS_LockReg();
    
    // Enable channel 4
    ADC_Open(ADC, 0, ADC_OPERATION_MODE_SINGLE, ADC_CH_4_MASK);
    
    // Set reference voltage to AVDD
    ADC_SET_REF_VOLTAGE(ADC, ADC_REFSEL_POWER);

    // Power on ADC
    ADC_POWER_ON(ADC);
}

void hs_adc_release(void)
{
    ADC_POWER_DOWN(ADC);
    
    ADC_Close(ADC);
    
    /* Unlock protected registers */
    SYS_UnlockReg();
    
    /* Enable ADC clock */
    CLK_DisableModuleClock(ADC_MODULE);
    
    /* Set PA.4 pin for gpio */
    SYS->PA_L_MFP  &= ~SYS_PA_L_MFP_PA4_MFP_Msk;

    /* Enable PA.4 digital input path */
    PA->OFFD &= ~((1 << 4) << GP_OFFD_OFFD_Pos);
    
    /* Lock protected registers */
    SYS_LockReg();
    
    GPIO_SetMode(PA, BIT4, GPIO_PMD_INPUT);
}

uint16_t hs_get_voltage(void)
{
    uint8_t  i;
    uint32_t ADCData, ulVoltage = 0;
    uint32_t Max = 0, Min = 0xffff;

    for(i=0; i<10; i++) {
        /* Start ADC1 Conversion using Software trigger*/
        ADC_START_CONV(ADC);

        /* Wait until ADC Channel 15 end of conversion */
        while(!ADC_GET_INT_FLAG(ADC, ADC_ADF_INT));

        ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);
        
        ADCData = 0;
	/* read ADC convertion result */
        ADCData = ADC_GET_CONVERSION_DATA(ADC, 4);
        
        hs_delay_ms(10);

        ulVoltage += ADCData;
        Max = HS_DEF_MAX(ADCData, Max);
        Min = HS_DEF_MIN(ADCData, Min);
    }

    ulVoltage = (ulVoltage - Max - Min) >> 3;

    return (uint16_t)ulVoltage;
}

static uint16_t hs_get_cur_voltage(uint8_t isInit)
{
    uint16_t usVoltage;
    uint32_t ulVoltage;
    
    if(isInit) {
        hs_adc_init();
    }
    hs_delay_ms(100);
    usVoltage = hs_get_voltage();
    hs_adc_release();
    
    ulVoltage = usVoltage * 3;
    ulVoltage = (ulVoltage * 1000) >> 11;

    return (uint16_t)ulVoltage;
}

/**
 * @brief	Probe the power module
 * @retval	0 if normal, other if exception
 */
HS_Power hs_power_probe(uint8_t isInit)
{
    uint16_t usVoltage;
    uint32_t ulVoltage;
    
    if(isInit) {
        hs_adc_init();
    }
    hs_delay_ms(100);
    usVoltage = hs_get_voltage();
    hs_adc_release();
    
    ulVoltage = usVoltage * 3;
    ulVoltage = (ulVoltage * 1000) >> 11;

    if(ulVoltage < 3400) {  //3.4V停止工作点
        status = HS_System_Stop;
    } else if(ulVoltage < 3500) { //3.6V虹膜停止工作点
        status = HS_Iris_Shutdown;
    } else if(ulVoltage < 3650) { //3.74V低压报警点
        status = HS_Low_Power;
    } else {
        status = HS_Normal_Power;
    }

    return status;
}

uint8_t hs_power_get_status(void)
{
    return status;
}

void hs_adc_test(void) 
{
    uint16_t usVoltage = 0;
    
    hs_key_led_init();
    
    while(1) {
        usVoltage = hs_get_cur_voltage(1);
        printf("-------> Cur Voltage : %ld \r\n", usVoltage);
        hs_key_led_toggle('8');
        hs_delay_ms(1000);
    }
}
