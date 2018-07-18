#ifndef  __HS_EVENT_H__
#define  __HS_EVENT_H__

#include "Nano100Series.h"


#define EVENT_INFO_LEN       32
#define EVENT_QUEUE_SIZE     5

#define EVENT_CREATE         0
#define EVENT_HANDLE         1

#define OPEN_METHOD_BUTTON   1
#define OPEN_METHOD_RFID     2

          
#define ETYPE_WAKEUP_KEY     0x00000001
#define ETYPE_WAKEUP_IR      0x00000002
#define ETYPE_KEYBOARD       0x00000004
#define ETYPE_ARLARM         0x00000008
#define ETYPE_RFID           0x00000010
#define ETYPE_OPEN_BUTTON    0x00000020
#define ETYPE_DOOR_CLOSE     0x00000040


typedef struct {
     uint32_t eType;
     uint32_t eInfoLen;
     uint8_t  eInfo[EVENT_INFO_LEN];
} EventTypeDef;


void hs_notify_event(void);

void hs_notify_handled(void);

void hs_print_event_info(EventTypeDef *pEvent, uint8_t mode);


#endif
