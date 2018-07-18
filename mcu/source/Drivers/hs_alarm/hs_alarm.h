#ifndef  __HS_ALARM_H__
#define  __HS_ALARM_H__


#include "Nano100Series.h"


typedef enum {
     Alarm_Mode_OFF = 0,
     Alarm_Mode_ON  = !Alarm_Mode_OFF
} Alarm_Mode;


void hs_alarm_init(void);

uint8_t hs_alarm_poll_pin_state(void);

void hs_set_alarm_mode(Alarm_Mode eNewMode);

void hs_alarm_test(void);

#endif