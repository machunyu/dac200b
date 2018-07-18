#ifndef _HS_KEYBOARD_LED_H_
#define _HS_KEYBOARD_LED_H_

#include "Nano100Series.h"

#define LED0                    PA8
#define LED0_Port		PA
#define LED0_Pin		BIT8

#define LED1                    PA0
#define LED1_Port		PA
#define LED1_Pin		BIT0

#define LED2                    PB10
#define LED2_Port		PB
#define LED2_Pin		BIT10

#define LED3                    PD7
#define LED3_Port		PD
#define LED3_Pin		BIT7

#define LED4                    PB13
#define LED4_Port		PB
#define LED4_Pin		BIT13

#define LED5                    PB1
#define LED5_Port		PB
#define LED5_Pin		BIT1

#define LED6                    PD6
#define LED6_Port		PD
#define LED6_Pin		BIT6

#define LED7                    PC7
#define LED7_Port		PC
#define LED7_Pin		BIT7

#define LED8                    PA9
#define LED8_Port		PA
#define LED8_Pin		BIT9

#define LED9                    PB3
#define LED9_Port		PB
#define LED9_Pin		BIT3

#define LED_STAR                PC6
#define LED_STAR_Port		PC
#define LED_STAR_Pin		BIT6

#define LED_HASH                PB2
#define LED_HASH_Port		PB
#define LED_HASH_Pin		BIT2


extern void hs_key_led_init(void);
extern void hs_key_led_all_on(void);
extern void hs_key_led_on(uint8_t ledNum);
extern void hs_key_leds_on(void *LedString);
extern void hs_key_led_toggle(uint8_t ledNum);
extern void hs_key_led_off(uint8_t ledNum);
extern void hs_key_led_all_off(void);
extern void hs_key_led_start_up(void);
extern void hs_key_led_open_door(void);
extern void hs_key_led_close_door(void);

#endif
