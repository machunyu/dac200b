#ifndef _HS_LED_H_
#define _HS_LED_H_


#include "Nano100Series.h"


typedef enum {
    LED_0 = 0,
    LED_1,
} Led_TypeDef;


extern void hs_led_init(void);

extern void hs_led_on(uint8_t LED);

extern void hs_led_toggle(uint8_t LED);

extern void hs_led_off(uint8_t LED);


#endif
