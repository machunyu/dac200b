
#include "hs_key.h"
#include "hs_delay.h"
#include "hs_poll.h"
#include "hs_systick.h"
#include "hs_config.h"
#include "hs_door.h"



uint8_t hs_poll_key(void)
{
    uint8_t ret = 0;
    uint32_t ulCurTicks = 0;
    uint32_t ulStartTicks = 0;
    uint32_t ulPressTime = 0;

    if( !hs_key_read_pin() ) {
        ulStartTicks = hs_systick_get_tick();
        
        while( !hs_key_read_pin() ) {
            ulCurTicks = hs_systick_get_tick();
            ulPressTime += (ulCurTicks - ulStartTicks) * SYS_TICK_PERIOD_MS;
            ulStartTicks = ulCurTicks;

            if( ulPressTime >= HS_CFG_RESET_TIME ) {
                ret = 1;
                break;
            }
        }
    }

    return ret;
}

uint8_t hs_poll_door_button(void)
{
    uint8_t ret = 0;
    
    if( !hs_door_read_button() ) {
        hs_delay_ms(30);
        if( !hs_door_read_button() ) {
            ret = 1;
        }
    }
    
    return ret;
}

uint8_t hs_poll_door_state(void)
{
    uint8_t ret = 0;
    
    if( !hs_door_read_state() ) {
        hs_delay_ms(30);
        if( !hs_door_read_state() ) {
            ret = 1;
        }
    }

    return ret;
}
