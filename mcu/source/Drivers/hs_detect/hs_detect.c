
#include "hs_detect.h"
#include "hs_delay.h"


#define HS_DEF_MAX(a, b)    ((a) > (b) ? (a) : (b))
#define HS_DEF_MIN(a, b)    ((a) < (b) ? (a) : (b))


void hs_detect_init(void)
{
    ///* Unlock protected registers */
    //SYS_UnlockReg();
    //
    ///* Enable ADC clock */
    //CLK_EnableModuleClock(ADC_MODULE);
    //
    //CLK_SetModuleClock(ADC_MODULE, CLK_CLKSEL1_ADC_S_HXT, CLK_ADC_CLK_DIVIDER(1));
    //
    ///* Set PA multi-function pins for ADC */
    //SYS->PA_L_MFP &= ~(SYS_PA_L_MFP_PA6_MFP_Msk);
    //SYS->PA_L_MFP |= SYS_PA_L_MFP_PA6_MFP_ADC_CH6;
    //
    ///* Disable PA.4 digital input path */
    //PA->OFFD |= ((1<<6) << GP_OFFD_OFFD_Pos);
    //
    ///* Lock protected registers */
    //SYS_LockReg();

    // Enable channel 4
    ADC_Open(ADC, 0, ADC_OPERATION_MODE_SINGLE, ADC_CH_6_MASK);

    // Set reference voltage to AVDD
    ADC_SET_REF_VOLTAGE(ADC, ADC_REFSEL_POWER);

    // Power on ADC
    ADC_POWER_ON(ADC);
}

void hs_detect_adc_release(void)
{
    ADC_POWER_DOWN(ADC);
    
    ADC_Close(ADC);
    
    /* Unlock protected registers */
    SYS_UnlockReg();
    
    /* Enable ADC clock */
    CLK_DisableModuleClock(ADC_MODULE);
    
    /* Set PA.4 pin for gpio */
    SYS->PA_L_MFP  &= ~SYS_PA_L_MFP_PA6_MFP_Msk;

    /* Enable PA.4 digital input path */
    PA->OFFD &= ~((1 << 6) << GP_OFFD_OFFD_Pos);
    
    /* Lock protected registers */
    SYS_LockReg();
    
    GPIO_SetMode(PA, BIT6, GPIO_PMD_INPUT);
}

uint16_t hs_detect_get_sampling_val(void)
{
    uint8_t  i;
    uint32_t ADCData, ulVoltage = 0;
    uint32_t Max = 0, Min = 0xffff;

    for(i=0; i<4; i++) {
        /* Start ADC1 Conversion using Software trigger*/
        ADC_START_CONV(ADC);

        /* Wait until ADC Channel 15 end of conversion */
        while(!ADC_GET_INT_FLAG(ADC, ADC_ADF_INT));

        ADC_CLR_INT_FLAG(ADC, ADC_ADF_INT);

        ADCData = 0;
	/* read ADC convertion result */
        ADCData = ADC_GET_CONVERSION_DATA(ADC, 6);

        ulVoltage += ADCData;
        Max = HS_DEF_MAX(ADCData, Max);
        Min = HS_DEF_MIN(ADCData, Min);
    }

    ulVoltage = (ulVoltage - Max - Min) / 2;

    return (uint16_t)ulVoltage;
}

uint16_t hs_detect_get_voltage(void)
{
    uint16_t usVoltage;
    uint32_t ulVoltage;

    usVoltage = hs_detect_get_sampling_val();

    ulVoltage = usVoltage * 3;
    ulVoltage = (ulVoltage * 1000) >> 12;

    return (uint16_t)ulVoltage;
}

uint8_t hs_detect_get_wakeup_state(void)
{
    uint16_t usVoltage;
    uint32_t ulVoltage;
    uint8_t ret = 0;

    usVoltage = hs_detect_get_sampling_val();
    
    ulVoltage = usVoltage * 3;
    ulVoltage = (ulVoltage * 1000) >> 12;

    if(ulVoltage >= THRESHOLD_VAL) {
        ret = 1;
    }

    return ret;
}


