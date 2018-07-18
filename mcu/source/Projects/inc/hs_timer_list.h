#ifndef  __HS_TIMER_LIST_H__
#define  __HS_TIMER_LIST_H__


#include "Nano100Series.h"




#define KEYBOARD_TIMER_ID          1
#define CLSOE_TIMER_ID             2
#define BEEP_TIMER_ID              3
#define ERROR_TIMER_ID             4
#define NOTIFY_TIMER_ID            5


#define TIMER_MODE_LIMIT           1
#define TIMER_MODE_UNLIMIT         2


typedef void (*TimerHandler1_t)(void);
typedef void (*TimerHandler2_t)(uint8_t);


typedef struct {
    uint8_t ucID;
    uint8_t ucRunFlag;
    uint8_t ucRunMode;
    uint32_t ulRunTime;
    uint32_t ulReloadTime;
    uint32_t ulRunCount;
    TimerHandler1_t fpHandler1;
    TimerHandler2_t fpHandler2;
    uint8_t ulHandler2Para;
} AppTimer_t;

typedef struct Timer_Node
{
    AppTimer_t data;
    struct Timer_Node *next;
} Timer_Node_t;


uint8_t hs_timer_list_init(void);

uint8_t hs_timer_node_run(uint8_t ucTimerID, uint32_t ulRunCount, uint8_t ulHandler2Para);

uint8_t hs_timer_node_stop(uint8_t ucTimerID);

uint8_t hs_timer_node_update(uint8_t ucTimerIDk, AppTimer_t *data);


#endif

