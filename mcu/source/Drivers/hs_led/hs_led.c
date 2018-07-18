#include "hs_led.h"


#define LED0                     PA13
#define LED0_Port                PA
#define LED0_Pin	         BIT13

#define LED1                     PB9
#define LED1_Port                PB
#define LED1_Pin	         BIT9



void hs_led_init(void)
{
    GPIO_SetMode(LED0_Port, LED0_Pin, GPIO_PMD_OUTPUT);
    GPIO_SetMode(LED1_Port, LED1_Pin, GPIO_PMD_OUTPUT);
    
    LED0 = 1; LED1 = 1;
}

void hs_led_on(uint8_t LED)
{
    if(LED_0 == LED) {
        LED0 = 0;
    } else {
        LED1 = 0;
    }
}

void hs_led_off(uint8_t LED)
{
    if(LED_0 == LED) {
        LED0 = 1;
    } else {
        LED1 = 1;
    }
}

void hs_led_toggle(uint8_t LED)
{
    if(LED_0 == LED) {
        LED0_Port->DOUT ^= LED0_Pin;
    } else {
        LED1_Port->DOUT ^= LED1_Pin;
    }
}


