#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <intrinsics.h>
#include "hs_type.h"
#include "hs_timer.h"
#include "hs_door.h"
#include "hs_led.h"
#include "hs_beep.h"
#include "hs_power.h"
#include "key_led.h"
#include "hs_a33io.h"
#include "hs_delay.h"
#include "hs_sys_mgr.h"
#include "hs_timer_list.h"



static Timer_Node_t *TimerListHead = NULL;


static void hs_timer_list_traverse(void);


static Timer_Node_t *hs_timer_list_creat(void)
{
     Timer_Node_t *head = NULL;

     head = (Timer_Node_t*) malloc(sizeof(Timer_Node_t));
     if( NULL == head ) {
         hs_printf(ERROR, "error : fail to malloc memmory for timer list head ...\r\n");
         return NULL;
     }

     memset(head, 0, sizeof(Timer_Node_t));

     head->next = NULL;

     return head;
}

static uint8_t hs_timer_list_insert_node(Timer_Node_t *pTimer)
{
    Timer_Node_t *p = TimerListHead;

    __disable_interrupt();
    
    while( p->next != NULL ) {
        p = p->next;
    }
    
    p->next = pTimer;
    pTimer->next = NULL;
    
    __enable_interrupt();
    
    return 0;
}

static uint8_t hs_timer_list_find_node(uint8_t ucTimerID, Timer_Node_t **pNode)
{
    uint8_t ret = 0;
    Timer_Node_t *p = TimerListHead->next;

    __disable_interrupt();

    while( p != NULL ) {
        if( p->data.ucID == ucTimerID) break;
        p = p->next;
    }

    if( p != NULL ) {
        *pNode = p;
        ret = 1;
    }

    __enable_interrupt();
    
    return ret;
}

static void hs_timer_list_traverse(void)
{
    AppTimer_t *pTimer = NULL;
    Timer_Node_t *p = TimerListHead;

    __disable_interrupt();

    while( p->next != NULL) {
        pTimer = &p->next->data;
        if(pTimer->ucRunFlag) {
            pTimer->ulRunTime--;
            if(!pTimer->ulRunTime) {
                if(pTimer->fpHandler1) {
                    pTimer->fpHandler1();
                }

                if(pTimer->fpHandler2) {
                    pTimer->fpHandler2(pTimer->ulHandler2Para);
                }

                if(TIMER_MODE_LIMIT == pTimer->ucRunMode) {
                    pTimer->ulRunCount--;
                    if(pTimer->ulRunCount) {
                        pTimer->ulRunTime = pTimer->ulReloadTime;
                    } else {
                        pTimer->ucRunFlag = 0;
                    }
                } else {
                    pTimer->ulRunTime = pTimer->ulReloadTime;
                }
            }
        }

        p = p->next;
    }
    
    __enable_interrupt();

    return;
}

uint8_t hs_timer_list_init(void)
{
    Timer_Node_t *pNode = NULL;

    hs_timer_init();

    TimerListHead = hs_timer_list_creat();
    if( TimerListHead == NULL ) {
        return 1;
    }

    pNode = (Timer_Node_t*)malloc(sizeof(Timer_Node_t) * 5);
    if( pNode == NULL ) {
        hs_printf(ERROR, "error : fail to malloc memory for new node ... \r\n");
        return 1;
    }

    memset(pNode, 0, sizeof(Timer_Node_t) * 5);
    pNode[0].data.ucID = KEYBOARD_TIMER_ID;
    pNode[0].data.ucRunMode = TIMER_MODE_LIMIT;
    pNode[0].data.ulReloadTime = 2;
    pNode[0].data.fpHandler1 = NULL;
    pNode[0].data.fpHandler2 = hs_key_led_on;
    hs_timer_list_insert_node(&pNode[0]);

    
    pNode[1].data.ucID = CLSOE_TIMER_ID;
    pNode[1].data.ucRunMode = TIMER_MODE_LIMIT;
    pNode[1].data.ulReloadTime = g_SysInfo.ucOpenHoldPeriod * (1000 / TIMER_PERIOD_MS);
    pNode[1].data.fpHandler1 = NULL;
    pNode[1].data.fpHandler2 = hs_door_close;
    hs_timer_list_insert_node(&pNode[1]);

    pNode[2].data.ucID = BEEP_TIMER_ID;
    pNode[2].data.ucRunMode = TIMER_MODE_LIMIT;
    pNode[2].data.ulReloadTime = 3;
    pNode[2].data.fpHandler1 = hs_beep_off;
    pNode[2].data.fpHandler2 = NULL;
    hs_timer_list_insert_node(&pNode[2]);
    
    pNode[3].data.ucID = ERROR_TIMER_ID;
    pNode[3].data.ucRunMode = TIMER_MODE_UNLIMIT;
    pNode[3].data.ulReloadTime = 4;
    pNode[3].data.fpHandler1 = NULL;
    pNode[3].data.fpHandler2 = hs_led_toggle;
    hs_timer_list_insert_node(&pNode[3]);
    
    pNode[4].data.ucID = NOTIFY_TIMER_ID;
    pNode[4].data.ucRunMode = TIMER_MODE_LIMIT;
    pNode[4].data.ulReloadTime = 10;
    pNode[4].data.fpHandler1 = hs_a33_io_set_high;
    pNode[4].data.fpHandler2 = NULL;
    hs_timer_list_insert_node(&pNode[4]);

    hs_timer_reg_callback(hs_timer_list_traverse);

    hs_timer_start();

    return 0;
}

uint8_t hs_timer_node_run(uint8_t ucTimerID, uint32_t ulRunCount, uint8_t ucHandler2Para)
{
    uint8_t ret = 0;
    AppTimer_t *pTimer = NULL;
    Timer_Node_t *pNode = NULL;

    if( hs_timer_list_find_node(ucTimerID, &pNode) ) {
        pTimer = &pNode->data;
        pTimer->ulRunTime = pTimer->ulReloadTime;
        pTimer->ulRunCount = ulRunCount;
        pTimer->ulHandler2Para = ucHandler2Para;
        
        pTimer->ucRunFlag = 1;
        ret = 1;
    }

    return ret;
}

uint8_t hs_timer_node_stop(uint8_t ucTimerID)
{
    uint8_t ret = 0;
    AppTimer_t *pTimer = NULL;
    Timer_Node_t *pNode = NULL;

    if( hs_timer_list_find_node(ucTimerID, &pNode) ) {
        pTimer = &pNode->data;
        pTimer->ucRunFlag = 0;
        ret = 1;
    }
  
    (void) pTimer;
    
    return ret;
}

uint8_t hs_timer_node_update(uint8_t ucTimerID, AppTimer_t *data)
{
    uint8_t ret = 0;
    AppTimer_t *pTimer = NULL;
    Timer_Node_t *pNode = NULL;
    
    if( hs_timer_list_find_node(ucTimerID, &pNode) ) {
        pTimer = &pNode->data;
        if(data->ucRunMode) {
            pTimer->ucRunMode = data->ucRunMode;
        }
        
        if(data->ulReloadTime) {
            pTimer->ulReloadTime = data->ulReloadTime;
        }
        
        if(data->fpHandler1) {
            pTimer->fpHandler1 = data->fpHandler1;
        }
        
        if(data->fpHandler2) {
            pTimer->fpHandler2 = data->fpHandler2;
        }

        ret = 1;
    }

    return ret;
}

