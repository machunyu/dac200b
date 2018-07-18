#include "hs_door_control.h"
#include "hs_timer_list.h"
#include "hs_sys_mgr.h"
#include "hs_door.h"


uint8_t hs_door_control_open(void)
{
    if(HS_CFG_LOCK_MODE_CLOSE == g_SysInfo.ucLockMode) {
        hs_door_open(g_SysInfo.ucOpenMethod);
        hs_timer_node_run(CLSOE_TIMER_ID, 1, g_SysInfo.ucOpenMethod);
    } else {
        hs_door_open(g_SysInfo.ucOpenMethod);
    }

    return 0;
}