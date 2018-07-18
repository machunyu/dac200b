#include "main.h"


uint8_t hs_check_frame(RxFrame_t *pFrame)
{
    uint8_t ret = 0;
    if( !hs_protocol_check_frame(pFrame->Frame, pFrame->FrameLen)) {
        hs_protocol_send_crcErr_Frame();
        hs_printf(ERROR, "mcu recv crc error ...\r\n");
        
    } else if( !hs_protocol_check_cmdType(Conv_16_BIT(pFrame->m_Frame.command)) ) {
        hs_protocol_send_crcErr_Frame();
        hs_printf(ERROR, "unsupported Command ...\r\n");

    } else {
        ret = 1;
    }
    
    return ret;
}

extern uint32_t g_ulWakeupEventCnt;

void hs_msg_int_process(uint16_t usCommand, uint8_t *buf, uint16_t len)
{
    (void)len;
    //uint32_t offset;

    switch(usCommand) {
        case MSG_READ_EVENT :
        {
            hs_notify_handled(); 
            hs_protocol_send_frame(MSG_READ_EVENT, MSG_TYPE_ACK|ACK_SUCCEED, (uint8_t*)&g_sCurEvent, sizeof(g_sCurEvent));
            
            if(g_sCurEvent.eType == ETYPE_WAKEUP_KEY || g_sCurEvent.eType == ETYPE_WAKEUP_IR) {
                g_stFlag.ucWakeFlag = 1;
                g_ulWakeupEventCnt = 0;
                hs_led_on(LED_1);
                hs_module_wakeup();
            }
            hs_printf(WARN, "\r\n mcu ack read event signal to a33 && g_sCurEvent : %x\r\n", g_sCurEvent.eType);
            break;
        }

        case MSG_SYS_STANDBY :
        {
                g_stFlag.ucStandby = 1;
                hs_timer1_stop();
                hs_key_led_all_off();
                hs_heart_beator_stop();
                hs_set_sys_state(SYS_STATE_SLEEP);
                hs_protocol_send_frame(MSG_SYS_STANDBY, MSG_TYPE_ACK|ACK_SUCCEED, NULL, 0);
                hs_printf(WARN, "\r\n mcu ack standby to a33, System is ready now\r\n");
                break;
        }

        case MSG_SYS_SLEEP :
        {
                hs_protocol_send_frame(MSG_SYS_SLEEP, MSG_TYPE_ACK|ACK_SUCCEED, NULL, 0);
                hs_module_sleep();
                hs_led_off(LED_1);
                hs_printf(WARN, "\r\n mcu ack sleep signal to a33\r\n");
                break;
        }

        case MSG_SYS_RST :
        {
                hs_protocol_send_frame(MSG_SYS_RST, MSG_TYPE_ACK|ACK_SUCCEED, NULL, 0);
                hs_printf(WARN, "\r\n mcu ack reset signal to a33\r\n");
                hs_iris_off();
                hs_delay_ms(200);
                hs_a33_power_off();
                hs_timer1_stop();
                hs_key_led_all_off();
                hs_delay_s(3);
                hs_iwdg_reset_system();
                break;
        }

        case MSG_SYSTEM_LOCK :
        {
                g_stFlag.ucSysLock = 1;
                hs_timer1_start();
                hs_protocol_send_frame(MSG_SYSTEM_LOCK, MSG_TYPE_ACK|ACK_SUCCEED, NULL, 0);
                hs_printf(WARN, "\r\n mcu ack download start signal to a33\r\n");
                break;
        }

        case MSG_SYSTEM_UNLOCK :
        {
                g_stFlag.ucSysLock = 0;
                hs_protocol_send_frame(MSG_SYSTEM_UNLOCK, MSG_TYPE_ACK|ACK_SUCCEED, NULL, 0);
                hs_timer1_stop();
                hs_module_sleep();
                hs_printf(WARN, "\r\n mcu ack download stop signal to a33\r\n");
                break;
        }

        case MSG_CFG_SET : 
        {
                SysCfgTypeDef tmpSysConfig;
                hs_protocol_send_frame(MSG_CFG_SET, MSG_TYPE_ACK|ACK_SUCCEED, NULL, 0);
                memcpy((uint8_t*)&tmpSysConfig, (uint8_t*)buf, sizeof(tmpSysConfig));
                hs_sys_config_update(&tmpSysConfig);
                g_stFlag.ucUpdateSysConfig = 1;
                hs_printf(WARN, "\r\n mcu ack set config signal to a33\r\n");
                break;
        }

        case MSG_CFG_GET :
        {
                hs_protocol_send_frame(MSG_CFG_SET, MSG_TYPE_ACK|ACK_SUCCEED, (uint8_t*)&g_SysInfo, sizeof(g_SysInfo));
                hs_printf(WARN, "\r\n mcu ack get config signal to a33\r\n");
                break;
        }

        case MSG_IPWR_UP :
        {
                hs_iris_on();
                hs_protocol_send_frame(MSG_IPWR_UP, MSG_TYPE_ACK|ACK_SUCCEED, NULL, 0);
                hs_printf(WARN, "\r\n mcu ack iris power up signal to a33\r\n");
                break;
        }

        case MSG_IPWR_DN :
        {
                hs_iris_off();
                hs_protocol_send_frame(MSG_IPWR_UP, MSG_TYPE_ACK|ACK_SUCCEED, NULL, 0);
                hs_printf(WARN, "\r\n mcu ack iris power down signal to a33\r\n");
                break;
        }

        case MSG_GET_UPDATE_INFO :
        {
                hs_protocol_send_frame(MSG_GET_UPDATE_INFO, MSG_TYPE_ACK|ACK_SUCCEED, &g_stFlag.ucUpdateFlag, 1);
                hs_printf(WARN, "\r\n mcu ack get update info signal to a33 && ulUpdateFlag = %x\r\n", g_stFlag.ucUpdateFlag);
                break;
        }

        case MSG_BEAT_START :
        {
                uint32_t ulBeatPeriod;
                hs_protocol_send_frame(MSG_BEAT_START, MSG_TYPE_ACK|ACK_SUCCEED, NULL, 0);
                memcpy((uint8_t*)&ulBeatPeriod, buf, 4);
                hs_heart_beator_start(ulBeatPeriod);
                hs_led_on(LED_0);
                hs_printf(WARN, "\r\n mcu ack start beating signal to a33 && beat period : %ld\r\n", ulBeatPeriod);
                break;
        }

        case MSG_BEATING :
        {
                uint8_t state = (uint8_t)hs_get_sys_state();
                hs_protocol_send_frame(MSG_BEATING, MSG_TYPE_ACK|ACK_SUCCEED, &state, 1);
                hs_heart_beator_beating();
                hs_printf(WARN, "\r\n mcu ack beating signal to a33 \r\n");
                break;
        }

        case MSG_BEAT_STOP :
        {
                hs_protocol_send_frame(MSG_BEAT_STOP, MSG_TYPE_ACK|ACK_SUCCEED, NULL, 0);
                hs_heart_beator_stop();
                hs_led_off(LED_0);
                hs_printf(WARN, "\r\n mcu ack stop beating signal to a33 \r\n");
                break;
        }

        case MSG_OPEN_DOOR :
        {
                hs_protocol_send_frame(MSG_OPEN_DOOR, MSG_TYPE_ACK|ACK_SUCCEED, NULL, 0);
                hs_door_control_open();
                hs_printf(WARN, "\r\n mcu ack open door signal to a33 \r\n");
                break;
        }

        case MSG_CLOSE_DOOR :
        {
                hs_protocol_send_frame(MSG_OPEN_DOOR, MSG_TYPE_ACK|ACK_SUCCEED, NULL, 0);
                hs_door_close(g_SysInfo.ucOpenMethod);
                hs_printf(WARN, "\r\n mcu ack close door signal to a33 \r\n");
                break;
        }

        case MSG_READ_DOOR_STATE :
        {
                uint8_t state = hs_door_read_state();
                hs_protocol_send_frame(MSG_READ_DOOR_STATE, MSG_TYPE_ACK|ACK_SUCCEED, &state, 1);
                hs_printf(INFO, "\r\n mcu ack read door state signal to a33 && current door state : %d\r\n", state);
                break;
        }

        case MSG_READ_BUTTON :
        {
                uint8_t state = hs_door_read_button();
                hs_protocol_send_frame(MSG_READ_DOOR_STATE, MSG_TYPE_ACK|ACK_SUCCEED, &state, 1);
                hs_printf(INFO, "\r\n mcu ack read button signal to a33 && current button state : %d\r\n", state);
                break;
        }

        case MSG_READ_KEYBOARD :
        {
                hs_protocol_send_frame(MSG_READ_DOOR_STATE, MSG_TYPE_ACK|ACK_SUCCEED, &g_ucKeyVal, 1);
                hs_printf(INFO, "\r\n mcu ack read keyboard signal to a33 && current keyval : %d\r\n", g_ucKeyVal);
                break;
        }
            
        case MSG_READ_IRIS :
        {
                uint8_t state = hs_detect_get_wakeup_state();
                hs_protocol_send_frame(MSG_READ_DOOR_STATE, MSG_TYPE_ACK|ACK_SUCCEED, &state, 1);
                hs_printf(INFO, "\r\n mcu ack read iris signal to a33 && current iris state : %d\r\n", state);
                break;
        }

        case MSG_READ_RST_KEY :
        {
                uint8_t state = hs_key_read_pin();
                hs_protocol_send_frame(MSG_READ_DOOR_STATE, MSG_TYPE_ACK|ACK_SUCCEED, &state, 1);
                hs_printf(INFO, "\r\n mcu ack read reset key signal to a33 && current key state : %d\r\n", state);
                break;
        }

        case MSG_BATCH_CARD_START :
        {
                g_stFlag.ulBatchInsert = 0;
                g_stFlag.ucBatchCardFlag = 0;
                g_stFlag.ulBetchWriteFlag = 0;
                hs_protocol_send_frame(MSG_BATCH_CARD_START, MSG_TYPE_ACK|ACK_SUCCEED, NULL, 0);
                memcpy((uint8_t*)&g_stFlag.ulBetchOffset, buf, 4);
                hs_printf(INFO, "\r\n mcu ack batch download card start, offset=%d\r\n", g_stFlag.ulBetchOffset);
                break;
        }

        case MSG_BATCH_CARD_INSERT :
        {
                CardInfo_t *pInfo = NULL;
                pInfo = (CardInfo_t*)buf;
                
                hs_printf_card_info(pInfo);
                
                hs_protocol_send_frame(MSG_BATCH_CARD_INSERT, MSG_TYPE_ACK|ACK_SUCCEED, NULL, 0);
                
                if(pInfo->ucCardType == RC523_CARD_TYPE_A) {
                    Card_A_TypeDef *p = (Card_A_TypeDef *) ucBatchCardBuf;
                    
                    //offset = g_stFlag.ulBatchInsert*sizeof(Card_A_TypeDef);
                    //ucBatchCardBuf[offset] = pInfo->ucAttr;
                    g_stFlag.ulBetchCardType = RC523_CARD_TYPE_A;
                    p[g_stFlag.ulBatchInsert].ucAttr = pInfo->ucAttr;
                    memcpy(p[g_stFlag.ulBatchInsert].ucCardID, pInfo->ucCardID, HS_CFG_CARD_A_ID_LEN);
                } else if(pInfo->ucCardType == RC523_CARD_TYPE_B) {
                    Card_B_TypeDef *p = (Card_B_TypeDef *) ucBatchCardBuf;
                  
                    //offset = g_stFlag.ulBatchInsert*sizeof(Card_B_TypeDef);
                    //ucBatchCardBuf[offset] = pInfo->ucAttr;
                    g_stFlag.ulBetchCardType = RC523_CARD_TYPE_B;
                    p[g_stFlag.ulBatchInsert].ucAttr = pInfo->ucAttr;
                    memcpy(p[g_stFlag.ulBatchInsert].ucCardID, pInfo->ucCardID, HS_CFG_CARD_B_ID_LEN);
                }

                g_stFlag.ulBatchInsert++;

                hs_printf(INFO, " mcu ack batch card: %d\r\n", g_stFlag.ulBatchInsert);
                break;
        }
        
        case MSG_BATCH_CARD_STOP :
        {
                g_stFlag.ucBatchCardFlag = 1;
                hs_protocol_send_frame(MSG_BATCH_CARD_STOP, MSG_TYPE_ACK|ACK_SUCCEED, NULL, 0);
                hs_printf(INFO, "\r\n mcu ack batch download card stop\r\n");
                break;
        }
        
        case MSG_GET_INSERT_STATE :
        {
                hs_protocol_send_frame(MSG_BATCH_CARD_STOP, MSG_TYPE_ACK|ACK_SUCCEED, (uint8_t*)&g_stFlag.ulBetchWriteFlag, 1);
                hs_printf(INFO, "\r\n mcu ack batch write state\r\n");
                break;
        }
        
        default: break; 
    }
    //hs_led_toggle(LED_0);
}

void hs_msg_poll_process(unsigned short usCommand, uint8_t *ReqData, uint16_t ReqLen)
{
    uint8_t  ret, AckData[100];

    switch(usCommand) {
        case MSG_CARD_REG :
        {
                CardInfo_t Info;
                uint32_t ulPos = 0;
                //uint8_t regFlag = 0;
                
                memcpy(&Info, ReqData, sizeof(Info));
                //hs_printf_card_info(&Info);
                ulPos = hs_card_manager_assign_pos(Info.ucCardType);
                if(ulPos != 0xFFFF) {
                    ret = hs_card_manager_insert(Info, ulPos);
                    if( !ret ) {
                        hs_protocol_send_frame(MSG_CARD_REG, MSG_TYPE_ACK|ACK_SUCCEED, NULL, 0);
                        hs_printf(INFO, " mcu insert new card succeed && Pos : %ld \r\n", ulPos);
                    } else {
                        hs_protocol_send_frame(MSG_CARD_REG, MSG_TYPE_ACK|ACK_FAILED, NULL, 0);
                        hs_printf(ERROR, "error : mcu insert new card failed ....\r\n");
                    }
                } else {
                    hs_protocol_send_frame(MSG_CARD_REG, MSG_TYPE_ACK|ACK_FAILED, NULL, 0);
                    hs_printf(ERROR, "error : mcu fail to assign pos for new card ....\r\n");
                }
                hs_printf(INFO, "mcu ack register card signal to a33\r\n");
                break;
        }

        case MSG_CARD_MOD :
        {
                CardInfo_t Info;
                uint32_t ulPos = 0;
                uint8_t ModFlag = 0;
                
                memcpy(&Info, ReqData, sizeof(Info));
                hs_printf_card_info(&Info);
                
                ulPos = hs_card_manager_match(&Info, NULL);
                if(ulPos != 0xFFFF) {
                    ret = hs_card_manager_modify(Info, ulPos);
                    if( !ret ) {
                        ModFlag = 1;
                        hs_printf(INFO, " mcu modify card info succeed && Pos : %ld \r\n", ulPos);
                    } else {
                        hs_printf(ERROR, "error : mcu modify card info failed ....\r\n");
                    }
                } else {
                    hs_printf(INFO, " card not exist ... !!!\r\n");
                }
                
                if(ModFlag) {
                    hs_protocol_send_frame(MSG_CARD_MOD, MSG_TYPE_ACK|ACK_SUCCEED, NULL, 0);
                } else {
                    hs_protocol_send_frame(MSG_CARD_MOD, MSG_TYPE_ACK|ACK_FAILED, NULL, 0);
                }
                
                hs_printf(INFO, "\r\n mcu ack modify card signal to a33\r\n");

                break;
        }

        case MSG_CARD_DEL :
        {
                CardInfo_t Info;
                uint32_t ulPos = 0;
                uint8_t DelFlag = 0;
                
                memcpy(&Info, ReqData, sizeof(Info));
                hs_printf_card_info(&Info);
                
                ulPos = hs_card_manager_match(&Info, NULL);
                if(ulPos != 0xFFFF) {
                    ret = hs_card_manager_delete(Info, ulPos);
                    if( !ret ) {
                        DelFlag = 1;
                        hs_printf(INFO, " mcu delete card succeed && Pos : %ld \r\n", ulPos);
                    } else {
                        hs_printf(ERROR, "error : mcu delete card failed ....\r\n");
                    }
                } else {
                    hs_printf(INFO, " card not exist ... !!!\r\n");
                }
                
                if(DelFlag) {
                    hs_protocol_send_frame(MSG_CARD_DEL, MSG_TYPE_ACK|ACK_SUCCEED, NULL, 0);
                } else {
                    hs_protocol_send_frame(MSG_CARD_DEL, MSG_TYPE_ACK|ACK_FAILED, NULL, 0);
                }
                
                hs_printf(INFO, "\r\n mcu ack delete card signal to a33\r\n");
                
                break;
        }
                
        case MSG_CARD_READ :
        {
                uint8_t i, len = 0;
                uint8_t blocks = (ReqData[1] < 5) ? ReqData[1] : 4;
                RC523_Mem_Block blockAddr = (RC523_Mem_Block)ReqData[0];
                
                ret = 0;
                for(i=0; i<blocks; i++, blockAddr++) {
                    ret |= hs_rc523_read_block(blockAddr, AckData+len, NULL);
                    len += 16;
                }
                
                if( !ret ) {
                    hs_protocol_send_frame(MSG_CARD_READ, MSG_TYPE_ACK|ACK_SUCCEED, AckData, len);
                    hs_printf(INFO, " mcu read card data succeed : \r\n");
                    hs_print_message(AckData, len);
                    hs_printf(INFO, "\r\n==============================\r\n");
                } else {
                    hs_protocol_send_frame(MSG_CARD_READ, MSG_TYPE_ACK|ACK_FAILED, NULL, 0);
                    hs_printf(INFO, " mcu read card failed ...\r\n");
                }
                
                hs_printf(INFO, "\r\n mcu ack read card signal to a33\r\n");
                
                break;
        }
                
        case MSG_CARD_WRITE :
        {
                uint8_t len = 0, *p = ReqData + 2;
                uint8_t i, blocks = ReqData[1] < 4 ? ReqData[1] : 3;
                RC523_Mem_Block blockAddr = (RC523_Mem_Block)ReqData[0];

                ret = 0;
                hs_printf(INFO, "\r\n============== data to write ================\r\n");
                hs_print_message(ReqData+2, ReqLen-2);
                hs_printf(INFO, "\r\n============== data to write ================\r\n");

                for(i=0; i<blocks; i++, blockAddr++) {
                    ret |= hs_rc523_write_block(blockAddr, p+len);
                    len += 16;
                    hs_delay_ms(10);
                }
               
                if( !ret ) {
                    hs_protocol_send_frame(MSG_CARD_WRITE, MSG_TYPE_ACK|ACK_SUCCEED, NULL, 0);
                    hs_printf(INFO, " mcu write card data succeed : \r\n");
                    hs_print_message(p, len);
                    hs_printf(INFO, "\r\n==============================\r\n");
                } else {
                    hs_protocol_send_frame(MSG_CARD_WRITE, MSG_TYPE_ACK|ACK_FAILED, NULL, 0);
                    hs_printf(INFO, " mcu write card data failed ... \r\n");
                }
                
                hs_printf(INFO, "\r\n mcu ack write card signal to a33\r\n");
                
                break;
        }

        case MSG_SET_RECOVERY :
        {
                ret = hs_module_reset();
                if( !ret ) {
                    hs_protocol_send_frame(MSG_SET_RECOVERY, MSG_TYPE_ACK|ACK_SUCCEED, NULL, 0);
                    hs_printf(INFO, "\r\n mcu set factory succeed !!!\r\n");
                } else {
                    hs_protocol_send_frame(MSG_SET_RECOVERY, MSG_TYPE_ACK|ACK_FAILED, NULL, 0);
                    hs_printf(INFO, "\r\n mcu set factory failed ...\r\n");
                }

                hs_printf(INFO, "\r\n mcu ack set recovery signal to a33\r\n");

                break;
        }
			
        case MSG_READ_CARD_ID :
        {
                uint8_t cnt = 3, ret = 0;
                uint8_t ID[10], IDLen;
                RC523_Card_Type type = (RC523_Card_Type)ReqData[0];
                
                memset(ID, 0, sizeof(ID));
                memset(AckData, 0, sizeof(AckData));
                if( RC523_CARD_TYPE_A == type ) {
                    IDLen = 4;
                } else {
                    IDLen = 8;
                }

                ret = hs_rc523_config(type);
                if( !ret ) {
                    do {
                        cnt--;
                        hs_delay_ms(30);
                        ret = hs_rc523_get_card_id(type, ID);
                        if( !ret ) {
                            hs_hex_2_string(ID, IDLen, AckData);
                        }
                    } while(cnt && ret);
                    
                    if( !ret ) {
                        hs_printf(INFO, "CurType : %c, card id : %s \r\n", ReqData[0], AckData);
                        hs_protocol_send_frame(MSG_READ_CARD_ID, MSG_TYPE_ACK|ACK_SUCCEED, AckData, IDLen * 2);
                    } else {
                        hs_protocol_send_frame(MSG_READ_CARD_ID, MSG_TYPE_ACK|ACK_FAILED, NULL, 0);
                    }
                } else {
                    hs_protocol_send_frame(MSG_READ_CARD_ID, MSG_TYPE_ACK|ACK_FAILED, NULL, 0);
                } 
                
                break;
        }
        
        default: break; 
    }
    //hs_led_toggle(LED_0);
}
