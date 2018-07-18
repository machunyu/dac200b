/******************************************************************************
 * @file     main.c
 * @version  V1.00
 * $Revision: 3 $
 * $Date: 14/09/11 7:08p $
 * @brief    A project template for Nano100BN series MCUs
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/


#include "main.h"


stFLAGEnum g_stFlag;
EventTypeDef g_sCurEvent;
uint8_t g_ucKeyVal = 0;
uint32_t g_ulWakeupEventCnt = 0;
uint8_t ucBatchCardBuf[4000];
uint8_t g_ucInitFalg = 0;


char CopyRight[50];


void SYS_Init(void)
{
    SYS_UnlockReg();

    /* Enable external HXT */
    CLK_EnableXtalRC( CLK_PWRCTL_HXT_EN_Msk );

    /* Enable LIRC */
    CLK_EnableXtalRC( CLK_PWRCTL_LIRC_EN_Msk );

    /* Waiting for 12MHz clock ready */
    CLK_WaitClockReady( CLK_CLKSTATUS_HXT_STB_Msk );

    /* Waiting for LIRC clock ready */
    CLK_WaitClockReady( CLK_CLKSTATUS_LIRC_STB_Msk );

    /* Set HCLK frequency 32MHz */
    CLK_SetCoreClock(32000000);
    
    /* Enable IP clock */
    CLK_EnableModuleClock(UART1_MODULE);
    
    /* Enable IP clock */
    CLK_EnableModuleClock(UART0_MODULE);
    
    /* Enable ADC clock */
    CLK_EnableModuleClock(ADC_MODULE);
    
    /* Enable IP clock */
    //CLK_EnableModuleClock(WDT_MODULE);
    
    CLK_EnableModuleClock(TMR0_MODULE);
        
    CLK_EnableModuleClock(TMR1_MODULE);
    
    CLK_EnableModuleClock(SPI0_MODULE);

    /* Select IP clock source */
    CLK_SetModuleClock(UART1_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_UART_CLK_DIVIDER(1));
    
    /* Select IP clock source */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_UART_CLK_DIVIDER(1));
    
    CLK_SetModuleClock(ADC_MODULE, CLK_CLKSEL1_ADC_S_HXT, CLK_ADC_CLK_DIVIDER(1));
        
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HXT, 0);
    
    CLK_SetModuleClock(TMR1_MODULE, CLK_CLKSEL1_TMR1_S_HXT, 0);
    
    /* Select IP clock source */
    //CLK_SetModuleClock(WDT_MODULE, 0, 0);
    
    /* Set PA multi-function pins for UART1 RXD and TXD */
    SYS->PA_L_MFP &= ~( SYS_PA_L_MFP_PA2_MFP_Msk | SYS_PA_L_MFP_PA3_MFP_Msk);
    SYS->PA_L_MFP |= (SYS_PA_L_MFP_PA3_MFP_UART1_TX|SYS_PA_L_MFP_PA2_MFP_UART1_RX);
    
    /* Set PA multi-function pins for UART1 RXD and TXD */
    SYS->PA_H_MFP &= ~( SYS_PA_H_MFP_PA14_MFP_Msk | SYS_PA_H_MFP_PA15_MFP_Msk);
    SYS->PA_H_MFP |= (SYS_PA_H_MFP_PA15_MFP_UART0_TX | SYS_PA_H_MFP_PA14_MFP_UART0_RX);
    
    /* Set PA multi-function pins for ADC */
    SYS->PA_L_MFP &= ~(SYS_PA_L_MFP_PA6_MFP_Msk);
    SYS->PA_L_MFP |= SYS_PA_L_MFP_PA6_MFP_ADC_CH6;
    
    /* Disable PA.4 digital input path */
    PA->OFFD |= ((1<<6) << GP_OFFD_OFFD_Pos);
   
    /* Set multi function pin for SPI1 */
    SYS->PC_L_MFP = SYS_PC_L_MFP_PC0_MFP_SPI0_SS0   | SYS_PC_L_MFP_PC1_MFP_SPI0_SCLK | \
                    SYS_PC_L_MFP_PC2_MFP_SPI0_MISO0 | SYS_PC_L_MFP_PC3_MFP_SPI0_MOSI0;
    
    SYS_LockReg();
    
    GPIO_SetMode(PC, BIT14, GPIO_PMD_OUTPUT);   // set GP2Y0E03 standby pin high
    GPIO_SetMode(PD, BIT14, GPIO_PMD_OUTPUT);   // set GP2Y0E03 standby pin high
    GPIO_SetMode(PB, BIT8,  GPIO_PMD_OUTPUT);   // set GP2Y0E03 standby pin high
    GPIO_SetMode(PB, BIT5,  GPIO_PMD_OUTPUT);   // set GP2Y0E03 standby pin high

    PB5 = 1; PB8 = 1; PC14 = 1; PD14 = 1;
}

#if 0

int main(void)
{
    SYS_Init();

    hs_debug_init();
    hs_systick_init();
    hs_delay_init();
    
    hs_power_init(); 

#if 1
    hs_a33_power_on();
    hs_delay_s(1);
    hs_lcd_init();
    hs_delay_s(5);
    hs_iris_on();
#endif
    
    hs_key_led_test();
    //hs_keyboard_test();
    //hs_timer_test();
    //hs_beep_test();
    //hs_alarm_test();
    //hs_detect_test();
    //hs_key_test();
    //hs_debug_led_test();
    //hs_power_test();
    //hs_key_timer_test();
    //hs_usart_test();
    //hs_RxLoopQueue_test();
    //hs_report_test();
    //hs_rc523_test();
    //hs_iwdg_test();
    //hs_systick_test();
    //hs_flash_test();
    //hs_timer1_test();
    //hs_timer_list_test();


    while(1) {
        printf("Hello World ..\r\n");
        hs_delay_ms(1000);
    }
}                                                                                                                          

#else

int main(void)
{
    uint8_t ret = 0;
    uint8_t ucIRState = 0;
    RxState_t RecvState;
    uint32_t ulPos = 0;
    uint32_t ulCurTicks = 0;
    uint32_t ulReadCardTicks = 0;
    uint8_t IDString[20] = { 0 };
    uint16_t usTryTimes = 0;
    uint8_t ucNotifyFlag = 0;
    uint32_t ulWakeupPeriod = 0;
    uint32_t ulWakeupStartTicks = 0;
    uint32_t ulWakeupEventUpCnt = 0;
    RC523_Card_Type CurCardType;
    uint8_t ucLastID[HS_CFG_CARD_ID_LEN];
    uint8_t ID[HS_CFG_CARD_ID_LEN], IDLen;
    CardInfo_t CardInfo, TempCardInfo;


    SYS_Init();
    
    ret = hs_module_init();
    
    memset(&g_stFlag, 0, sizeof(g_stFlag));
    CurCardType = RC523_CARD_TYPE_B; IDLen = 8;
    memset(ID, 0, sizeof(HS_CFG_CARD_ID_LEN));
    memset(ucLastID,   0, sizeof(HS_CFG_CARD_ID_LEN));
    memset(&CardInfo,  0, sizeof(CardInfo));
    memset(&TempCardInfo, 0, sizeof(TempCardInfo));
    
    sprintf(CopyRight, "\r\n=                                   %s                      =", __DATE__);
    hs_printf(INFO, "\r\n======================================================================");
    hs_printf(INFO, "\r\n=       (C) COPYRIGHT 2010 WuHan Hongshi Technologies Co.,Ltd        =");
    hs_printf(INFO, "\r\n=                                                                    =");
    hs_printf(INFO, "\r\n=             DAC200B System Application  (Version 3.5.0)            =");
    hs_printf(INFO, "\r\n=                                                                    =");
    hs_printf(INFO, "\r\n=                                   By HongShi Embest Team           =");
    hs_printf(INFO, "\r\n=                                                                    =");
    hs_printf(INFO, "%s", CopyRight);
    hs_printf(INFO, "\r\n======================================================================");
    hs_printf(INFO, "\r\n\r\n");

    hs_printf(INFO, "System init is %s, now will start Linux System, Please waitting...\r\n", ret ? "NOK" : "OK");

    hs_rc523_config(CurCardType);

    ulReadCardTicks = hs_systick_get_tick();
    ulWakeupStartTicks = hs_systick_get_tick();

    while(1) {
        ulCurTicks = hs_systick_get_tick();

        RecvState = hs_RxFrame_get_state();
        if( RX_STATE_F_FINISH == RecvState ) {
            hs_msg_poll_process(Conv_16_BIT(RxFrame.m_Frame.command), RxFrame.m_Frame.dat, Conv_16_BIT(RxFrame.m_Frame.dat_len));
            hs_RxFrame_clear();
        }

        if(g_stFlag.ucSysLock == 0) {
            // poll wakeup event
            if( g_stFlag.ucStandby && (SYS_STATE_SLEEP == hs_get_sys_state()) ) {
                ucIRState = hs_detect_get_wakeup_state();
                if(ucIRState) {
                    ucNotifyFlag = 1;
                    g_sCurEvent.eType = ETYPE_WAKEUP_IR;
                    g_sCurEvent.eInfoLen = 0;
                    
                } else if( (g_ucKeyVal = hs_keyboard_get()) < 0xFF ) {
                    g_sCurEvent.eType = ETYPE_WAKEUP_KEY;
                    g_sCurEvent.eInfoLen = 0;
                    ucNotifyFlag = 1;

                } else {
                    ucNotifyFlag = 0;
                }

                if(ucNotifyFlag) {
                    ucNotifyFlag = 0;
                    g_ulWakeupEventCnt++;
                    
                    if(g_ulWakeupEventCnt == 1) {
                        ulWakeupPeriod = 0;
                        ulWakeupStartTicks = ulCurTicks;
                         hs_print_event_info(&g_sCurEvent, EVENT_CREATE);
                         hs_printf(INFO, "\r\n -------> mcu wakeup event to a33\r\n");
                         hs_notify_event();
                         ulWakeupEventUpCnt = 1;
                    } else {
                        ulWakeupPeriod = (ulCurTicks - ulWakeupStartTicks) * SYS_TICK_PERIOD_MS;
                        if(ulWakeupPeriod > 600) {
                            ulWakeupPeriod = 0;
                            ulWakeupStartTicks = ulCurTicks;
                            hs_print_event_info(&g_sCurEvent, EVENT_CREATE);
                            hs_notify_event();
                            if(ulWakeupEventUpCnt++ > 8) {
                                ulWakeupEventUpCnt = 0;
                                hs_iris_off();
                                hs_iwdg_delay_ms(100);
                                hs_a33_power_off();
                                hs_timer1_stop();
                                hs_key_led_all_off();
                                g_stFlag.ucStandby = 0;
                                hs_delay_s(3);
                                hs_printf(INFO, "wakeup timeout, mcu will reset a33\r\n");
                                hs_iwdg_reset_system();
                            } else {
                                hs_printf(INFO, "\r\n -------> %d: mcu wakeup event to a33\r\n", ulWakeupEventUpCnt);
                            }
                        }
                    }
                }
            }

            if(g_stFlag.ucWakeFlag) {
                g_stFlag.ucWakeFlag = 0;
                hs_module_wakeup();
                ret = hs_module_probe();
                if( ret ) {
                    hs_printf(INFO, "\r\n error : module probe failed ....\r\n");
                }
            }

            // poll keyboard && notify key value to a33
            if( SYS_STATE_WAKEUP == hs_get_sys_state() ) {
                g_ucKeyVal = hs_keyboard_get();
                if(g_ucKeyVal < 0xEE) {
                    hs_key_led_all_on();
                    hs_module_response_key(g_ucKeyVal);
                    g_sCurEvent.eType = ETYPE_KEYBOARD;
                    g_sCurEvent.eInfo[0] = g_ucKeyVal;
                    g_sCurEvent.eInfoLen = 1;
                    hs_print_event_info(&g_sCurEvent, EVENT_CREATE);
                    hs_notify_event();
                    hs_printf(INFO, "\r\n -------> keyboard press event to a33 \r\n");
                }
            }

            // rc523 read card && notify card id to a33 && open door if it meet open condition
            memset(ID, 0, sizeof(ID));
            ret = hs_rc523_get_card_id(CurCardType, ID);
            if( !ret ) { // successful
                uint32_t ulPeriod = 0;
                
                usTryTimes = 0;
                ulPeriod = (ulCurTicks - ulReadCardTicks) * SYS_TICK_PERIOD_MS;

                if( memcmp(ID, ucLastID, HS_CFG_CARD_ID_LEN) ) {
                    ucNotifyFlag = 1;
                    memcpy(ucLastID, ID, HS_CFG_CARD_ID_LEN);
                } else if(ulPeriod > 2000) {
                    ucNotifyFlag = 1;
                } else {
                    ucNotifyFlag = 0;
                }

                if(ucNotifyFlag) {
                    ulPeriod = 0;
                    ucNotifyFlag = 0;
                    hs_hex_2_string(ID, IDLen, IDString);
                    hs_printf(INFO, "rc523 find a card : %s \r\n", IDString);
                    memset(&CardInfo, 0, sizeof(CardInfo));
                    memset(&TempCardInfo, 0, sizeof(TempCardInfo));
                    CardInfo.ucCardIDLen = IDLen;
                    CardInfo.ucCardType = CurCardType;
                    memcpy(CardInfo.ucCardID, ID, IDLen); 
                    if( (g_ucInitFalg == 2) && (CurCardType == RC523_CARD_TYPE_A) ) {
                        ulPos = hs_card_manager_match(&CardInfo, &TempCardInfo);
                        if( ulPos != 0xFFFF && TempCardInfo.ucAttr != HS_CFG_BLACK_LIST) {
                            CardInfo.ucIsOpen = 1;
                            CardInfo.ucAttr = TempCardInfo.ucAttr;
                            hs_printf(INFO, " mcu match card succeed && card Pos : %ld \r\n", ulPos);
                            //hs_printf_card_info(&TempCardInfo);
                        } else {
                            hs_printf(INFO, " mcu match card failed ... \r\n");
                        }
                    }
                    
                    if(CardInfo.ucIsOpen) {
                        hs_door_control_open();
                    }

                    ulReadCardTicks = ulCurTicks;
                    g_sCurEvent.eType = ETYPE_RFID;
                    memcpy(g_sCurEvent.eInfo, &CardInfo, sizeof(CardInfo));
                    g_sCurEvent.eInfoLen = sizeof(CardInfo);
                    hs_print_event_info(&g_sCurEvent, EVENT_CREATE);
                    
                    if(g_stFlag.ucStandby) {
                        hs_printf(INFO, "\r\n -------> mcu notify RFID event to a33  \r\n");
                        hs_notify_event();
                    } else {
                        memset(&g_sCurEvent, 0, sizeof(g_sCurEvent));
                    }
                }
            
                memset(ID, 0, sizeof(HS_CFG_CARD_ID_LEN));
            } else {
                usTryTimes++;
                if(usTryTimes > 20) {
                    usTryTimes = 0;
                    if(CurCardType == RC523_CARD_TYPE_A) {
                        IDLen = 8;
                        CurCardType = RC523_CARD_TYPE_B;
                    } else {
                        IDLen = 4;
                        CurCardType = RC523_CARD_TYPE_A;
                    }

                    ret = hs_rc523_config(CurCardType);
                    if( ret ) {
                        hs_printf(ERROR, " error : fail to hs_rc523_config ... \r\n");
                    }
                }
            }
        }

        // poll door button and open door if pressed down + notyfy button open event to a33
        if( !hs_door_read_button() ) {
            hs_iwdg_delay_ms(30); 
            if( !hs_door_read_button() ) {
                hs_door_control_open();
                while( !hs_door_read_button() );
                if( !g_stFlag.ucSysLock && g_stFlag.ucStandby ) {
                    g_sCurEvent.eType = ETYPE_OPEN_BUTTON;
                    g_sCurEvent.eInfoLen = 0;
                    hs_print_event_info(&g_sCurEvent, EVENT_CREATE);
                    hs_notify_event();
                    hs_printf(INFO, "\r\n -------> mcu notify door opened event to a33  \r\n");
                }
            }
        }

        // poll reset key and reset a33 && iris if key press down for more than 3 s
        if( hs_poll_key() ) {
            hs_iris_off();
            hs_iwdg_delay_ms(200);
            hs_a33_power_off();
            hs_timer1_stop();
            hs_key_led_all_off();
            g_stFlag.ucStandby = 0;
            hs_delay_s(3);
            while( !hs_key_read_pin() );
            hs_printf(INFO, "mcu will reset a33 as reset key pressed \r\n");
            hs_iwdg_reset_system();
        }

        // refresh heart beator && check if system is alive and reset system if it fail to beat again
        if( hs_heart_beator_is_enabled() ) {
            hs_heart_beator_refresh();
            if( !hs_heart_beator_is_alive() ) {
                hs_printf(INFO, "mcu recv no beating for long time and will reset system ... \r\n");
                hs_iris_off();
                hs_iwdg_delay_ms(200);
                hs_a33_power_off();
                g_stFlag.ucStandby = 0;
                hs_timer1_stop();
                hs_key_led_all_off();
                hs_delay_s(3);
                hs_heart_beator_callback();
            }
        }

        // update system configuration
        if(g_stFlag.ucUpdateSysConfig) {
            g_stFlag.ucUpdateSysConfig = 0;
            hs_sys_config_saveall();
            hs_printf_config_info();
        }

        if(g_stFlag.ucCtrDoorFlag == 0x1) {  // led mode (open door)
            g_stFlag.ucCtrDoorFlag = 0;
            hs_key_led_open_door();
        }
        
        if(g_stFlag.ucCtrDoorFlag == 0x2) {  // led mode (close door)
            g_stFlag.ucCtrDoorFlag = 0;
            hs_key_led_close_door();
        }

        if(g_stFlag.ucBatchCardFlag) {
            g_stFlag.ucBatchCardFlag = 0;
            if(g_stFlag.ulBetchCardType == RC523_CARD_TYPE_A) {
                if( (g_stFlag.ulBetchOffset+g_stFlag.ulBatchInsert) < HS_CFG_CARD_A_DEFAULT_CAP) {
                    //hs_printf(INFO, "write card data : \r\n");
                    //hs_print_message(ucBatchCardBuf, g_stFlag.ulBatchInsert * sizeof(Card_A_TypeDef));
                    //hs_printf(INFO, "\r\n============================\r\n");
                    ret = hs_card_manager_batch(RC523_CARD_TYPE_A, g_stFlag.ulBetchOffset, ucBatchCardBuf, g_stFlag.ulBatchInsert);
                    //if(!ret) {
                        //memset(ucBatchCardBuf, 0, sizeof(ucBatchCardBuf));
                        //hs_printf(INFO, "read card data : \r\n");
                        //hs_flash_read(HS_CFG_CARD_A_ADDR+g_stFlag.ulBetchOffset*sizeof(Card_A_TypeDef), ucBatchCardBuf, g_stFlag.ulBatchInsert * sizeof(Card_A_TypeDef));
                        //hs_print_message(ucBatchCardBuf, g_stFlag.ulBatchInsert * sizeof(Card_A_TypeDef));
                        //hs_printf(INFO, "\r\n============================\r\n");
                    //}
                }
            } else if(g_stFlag.ulBetchCardType == RC523_CARD_TYPE_B) {
                if( (g_stFlag.ulBetchOffset+g_stFlag.ulBatchInsert) < HS_CFG_CARD_B_DEFAULT_CAP ) {
                    //rintf(INFO, "card data : \r\n");
                    //rint_message(ucBatchCardBuf, g_stFlag.ulBatchInsert * sizeof(Card_A_TypeDef));
                    //rintf(INFO, "============================\r\n");
                    ret = hs_card_manager_batch(RC523_CARD_TYPE_B, g_stFlag.ulBetchOffset, ucBatchCardBuf, g_stFlag.ulBatchInsert);
                }
            }
            
            hs_printf(INFO, "card type=%c, offset=%ld, insert count = %ld, insert : %s\r\n", g_stFlag.ulBetchCardType, g_stFlag.ulBetchOffset,
                                                                                          g_stFlag.ulBatchInsert, ret ? "Failed" : "succeed");
            g_stFlag.ulBetchWriteFlag = 1;
        }
        
        if(g_ucInitFalg == 1) {
            g_ucInitFalg = 2;
            ret = hs_module_later_init();
            hs_printf(INFO, "System later init is %s\r\n", ret ? "NOK" : "OK"); 
        }

        // feet watch dog
        hs_iwdg_feet();
    }
}

#endif

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/
