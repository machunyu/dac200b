#include <string.h>
#include "hs_type.h"
#include "hs_timer.h"
#include "hs_event.h"
#include "hs_a33io.h"
#include "hs_delay.h"
#include "hs_timer_list.h"


#define NOTIFY_METHOD_PULSE    1
#define NOTIFY_METHOD_LEVEL    2
#define NOTIFY_METHOD          NOTIFY_METHOD_LEVEL


void hs_notify_event(void)
{
#if ( NOTIFY_METHOD == NOTIFY_METHOD_PULSE )
  
    hs_a33_io_set_low();
    
    hs_delay_ms(30);
    
    hs_a33_io_set_high();
    
#else

    hs_a33_io_set_low();

    hs_timer_node_run(NOTIFY_TIMER_ID, 1, 0);
    
#endif
}

void hs_notify_handled(void)
{
#if ( NOTIFY_METHOD == NOTIFY_METHOD_PULSE )
  
#else
    hs_a33_io_set_high();
#endif
}

void hs_print_event_info(EventTypeDef *pEvent, uint8_t mode)
{
    uint16_t i;
    uint8_t QMode[2][10] = { "creat", "handle" };
    
    hs_printf(INFO, " \r\n ================= %s event ==================== \r\n", QMode[mode]);
    hs_printf(INFO, "event type  : %x \r\n", pEvent->eType);
    hs_printf(INFO, "info length : %ld \r\n", pEvent->eInfoLen);
    hs_printf(INFO, "event info  : ");
    for(i=0; i<pEvent->eInfoLen; i++) {
        hs_printf(INFO, "%x ", pEvent->eInfo[i]);
    }

    hs_printf(INFO, "\r\n =============================================== \r\n");
    
    return;
}
