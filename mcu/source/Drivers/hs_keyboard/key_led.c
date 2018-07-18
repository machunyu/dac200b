#include "hs_delay.h"
#include "key_led.h"
#include "hs_status.h"

void hs_key_led_init(void)
{
    GPIO_SetMode(PA, BIT0 | BIT8 | BIT9, GPIO_PMD_OUTPUT);
    GPIO_SetMode(PB, BIT1 | BIT2 | BIT3 | BIT10 | BIT13, GPIO_PMD_OUTPUT);
    GPIO_SetMode(PC, BIT6 | BIT7, GPIO_PMD_OUTPUT);
    GPIO_SetMode(PD, BIT6 | BIT7, GPIO_PMD_OUTPUT);
    
    LED0 = 0;
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;
    LED5 = 0;
    LED6 = 0;
    LED7 = 0;
    LED8 = 0;
    LED9 = 0;
    LED_STAR = 0;
    LED_HASH = 0;
}

void hs_key_led_all_on(void)
{
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 1;
    LED6 = 1;
    LED7 = 1;
    LED8 = 1;
    LED9 = 1;
    LED_STAR = 1;
    LED_HASH = 1;
}

void hs_key_led_all_off(void)
{  
    LED0 = 0;
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;
    LED5 = 0;
    LED6 = 0;
    LED7 = 0;
    LED8 = 0;
    LED9 = 0;
    LED_STAR = 0;
    LED_HASH = 0;
}

void hs_key_led_on(uint8_t ledNum)
{
    if(ledNum == '0') {     // '0'
        LED0 = 1;
    } else if(ledNum == '1') {   // '1'
        LED1 = 1;
    } else if(ledNum == '2'){   // '2'
        LED2 = 1;
    } else if(ledNum == '3'){   // '3'
        LED3 = 1;
    } else if(ledNum == '4'){   // '4'
       LED4 = 1;
    } else if(ledNum == '5'){   // '5'
        LED5 = 1;
    } else if(ledNum == '6'){   // '6'
        LED6 = 1;
    } else if(ledNum == '7'){   // '7'
       LED7 = 1;
    } else if(ledNum == '8'){   // '8'
       LED8 = 1;
    } else if(ledNum == '9'){   // '9'
       LED9 = 1;
    } else if(ledNum == '*'){    // '*'
        LED_STAR = 1;
    } else if(ledNum == '#'){    // '#'
      LED_HASH = 1;
    } else {
        return;
    }
}

void hs_key_led_toggle(uint8_t ledNum)
{
    if(ledNum == '0') {     // '0'
        LED0 ^= 1;
    } else if(ledNum == '1') {   // '1'
        LED1 ^= 1;
    } else if(ledNum == '2'){   // '2'
        LED2 ^= 1;
    } else if(ledNum == '3'){   // '3'
        LED3 ^= 1;
    } else if(ledNum == '4'){   // '4'
        LED4 ^= 1;
    } else if(ledNum == '5'){   // '5'
        LED5 ^= 1;
    } else if(ledNum == '6'){   // '6'
        LED6 ^= 1;
    } else if(ledNum == '7'){   // '7'
        LED7 ^= 1;
    } else if(ledNum == '8'){   // '8'
        LED8 ^= 1;
    } else if(ledNum == '9'){   // '9'
        LED9 ^= 1;
    } else if(ledNum == '*'){    // '*'
        LED_STAR ^= 1;
    } else if(ledNum == '#'){    // '#'
        LED_HASH ^= 1;
    } else {
        return;
    }
}

void hs_key_led_off(uint8_t ledNum)
{
    if(ledNum == '0') {    // '0'
        LED0 = 0;
    } else if(ledNum == '1') {     // '1'
        LED1 = 0;
    } else if(ledNum == '2'){     // '2'
        LED2 = 0;
    } else if(ledNum == '3'){     // '3'
        LED3 = 0;
    } else if(ledNum == '4'){   // '4'
        LED4 = 0;
    } else if(ledNum == '5'){    // '5'
        LED5 = 0;
    } else if(ledNum == '6'){    // '6'
        LED6 = 0;
    } else if(ledNum == '7'){    // '7'
        LED7 = 0;
    } else if(ledNum == '8'){    // '8'
        LED8 = 0;
    } else if(ledNum == '9'){    // '9'
        LED9 = 0;
    } else if(ledNum == '*'){     // '*'
        LED_STAR = 0;
    } else if(ledNum == '#'){   // '#'
        LED_HASH = 0;
    } else {
        return;
    }
}

void hs_key_leds_on(void *LedString)   // hs_key_leds_on("123456789*#")
{ 
    uint8_t ledNum = 0;
    uint8_t *ledString = (uint8_t *)LedString;

    hs_key_led_all_off();
    while(ledString[ledNum]) {
        hs_key_led_on(ledString[ledNum]);
        ledNum++;
    }
}

void hs_key_led_start_up(void)
{
    //hs_key_leds_on("123");
    //hs_delay_ms(200);
    //hs_key_leds_on("456");
    //hs_delay_ms(200);
    //hs_key_leds_on("789");
    //hs_delay_ms(200);
    //hs_key_leds_on("*0#");
    //hs_delay_ms(200);
    //hs_key_leds_on("789*0#");
    //hs_delay_ms(200);
    //hs_key_leds_on("456789*0#");
    //hs_delay_ms(200);
    hs_key_leds_on("123456789*0#");
}

void hs_key_led_open_door(void)
{
    hs_key_leds_on("13468");
    hs_delay_ms(400);
    hs_key_leds_on("46790");
    hs_delay_ms(400);
    if(SYS_STATE_SLEEP == hs_get_sys_state()) {
        hs_key_led_all_off();
    } else {
        hs_key_led_all_on();
    }
    
}

void hs_key_led_close_door(void)
{
    hs_key_leds_on("13579");
    hs_delay_ms(400);
    if(SYS_STATE_SLEEP == hs_get_sys_state()) {
        hs_key_led_all_off();
    } else {
        hs_key_led_all_on();
    }
}
