#ifndef  __MAIN_H__
#define  __MAIN_H__


#include <stdio.h>
#include <string.h>
#include "hs_type.h"


#include "key_led.h"
#include "hs_delay.h"
#include "hs_timer.h"
#include "hs_iwdg.h"
#include "hs_beep.h"
#include "hs_lcd.h"
#include "hs_led.h"
#include "hs_key.h"
#include "hs_door.h"
#include "hs_a33io.h"
#include "hs_power.h"
#include "hs_usart.h"
#include "hs_debug.h"
#include "hs_detect.h"
#include "hs_flash.h"
#include "hs_keyboard.h"
#include "Nano100Series.h"
#include "hs_protocol.h"
#include "hs_rc523_api.h"
#include "hs_systick.h"
#include "hs_test.h"
#include "hs_beat_timer.h"
#include "hs_sys_mgr.h"
#include "hs_status.h"
#include "hs_module.h"
#include "hs_event.h"
#include "hs_poll.h"
#include "hs_door_control.h"
#include "hs_heart_beat.h"
#include "hs_algorithm.h"
#include "hs_timer_list.h"
#include "hs_card_manager.h"
#include "hs_msg_process.h"

typedef struct s_flag {
    uint8_t ucStandby;
    uint8_t ucOpenFlag;
    uint8_t ucSysLock;
    uint8_t ucUpdateSysConfig;
    uint8_t ucCtrDoorFlag;
    uint8_t ucUpdateFlag;
    uint8_t ucWakeFlag;
    uint8_t ucBatchCardFlag;
    uint32_t ulBatchInsert;
    uint32_t ulBetchOffset;
    uint8_t ulBetchWriteFlag;
    uint8_t ulBetchCardType;
}stFLAGEnum;

extern stFLAGEnum g_stFlag;
extern EventTypeDef g_sCurEvent;
extern uint8_t g_ucKeyVal;
extern uint8_t ucBatchCardBuf[4000];

#endif