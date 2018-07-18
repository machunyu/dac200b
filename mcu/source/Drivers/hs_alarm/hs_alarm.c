#include "hs_alarm.h"
#include "hs_beep.h"
#include "hs_delay.h"


#define ALARM                     PB9
#define ALARM_PORT                PB
#define ALARM_PIN                 BIT9


void hs_alarm_init(void)
{
    GPIO_SetMode(ALARM_PORT, ALARM_PIN, GPIO_PMD_INPUT);
    GPIO_ENABLE_PULL_UP(ALARM_PORT, ALARM_PIN);
}

uint8_t hs_alarm_poll_pin_state(void)
{
    return (ALARM == 0 ? 0 : 1);
}

void hs_alarm_test(void)
{
    uint8_t ucAlarmFlag = 0;
        
    hs_beep_init();
    hs_alarm_init();

    while(1) {
        if(!hs_alarm_poll_pin_state()) {
            hs_delay_ms(50);
            if(!hs_alarm_poll_pin_state()) {
                ucAlarmFlag = 1;
                hs_beep_on();
            }
            while(!hs_alarm_poll_pin_state()) {
                while(!hs_alarm_poll_pin_state()) {
                    hs_delay_ms(50);
                }
                hs_delay_ms(50);
            }
        } else if(ucAlarmFlag) {
            ucAlarmFlag = 0;
            hs_beep_off();
        }
        
        hs_delay_ms(50);
    }
}
