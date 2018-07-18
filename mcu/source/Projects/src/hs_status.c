#include "hs_status.h"


static volatile SysState State;
static volatile SysStatus Status;


void hs_set_sys_status(SysStatus status)
{
    switch(status)
    {
        case SYS_STATUS_FACT:
            Status = SYS_STATUS_FACT;
            break;
    
        case SYS_STATUS_USED:
            Status = SYS_STATUS_USED;
            break;
            
        default :   
            Status = SYS_STATUS_FACT;
            break;
    }
}

SysStatus hs_get_sys_status(void)
{
    return Status;
}

void hs_set_sys_state(SysState state)
{
    switch(state)
    {
        case SYS_STATE_SLEEP:
            State = SYS_STATE_SLEEP;
            break;

        case SYS_STATE_WAKEUP:
            State = SYS_STATE_WAKEUP;
            break;

        default :   
            State = SYS_STATE_SLEEP;
            break;
    }
}

SysState hs_get_sys_state(void)
{
    return State;
}
