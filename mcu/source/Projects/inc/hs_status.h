#ifndef  __HS_SYS_MGR_H__
#define  __HS_SYS_MGR_H__


#include "Nano100Series.h"
#include "hs_config.h"


typedef enum {
    SYS_STATUS_FACT  = 0,
    SYS_STATUS_USED
} SysStatus;

typedef enum {
    SYS_STATE_SLEEP  = 'S',
    SYS_STATE_WAKEUP = 'W'
} SysState;


void hs_set_sys_status(SysStatus status);

SysStatus hs_get_sys_status(void);

void hs_set_sys_state(SysState state);

SysState hs_get_sys_state(void);


#endif