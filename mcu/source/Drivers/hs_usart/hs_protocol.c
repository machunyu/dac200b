#include <string.h>
#include "hs_usart.h"
#include "hs_protocol.h"
#include "hs_delay.h"


//8bit CRC (X(8) + X(2) + X(1) + 1)
uint8_t hs_protocol_calCRC8(uint8_t *vptr, uint32_t len)
{
    uint16_t crc = 0;
    uint32_t i, j;
    const uint8_t *dat = vptr;
    
    for (j = len; j; j--, dat++) {
        crc ^= (*dat << 8);
        for (i = 8; i; i--) {
            if (crc & 0x8000)
                crc ^= (uint16_t)(0x1070 << 3);
            crc <<= 1;
        }
    }
  
    return (uint8_t)(crc >> 8);
}

void hs_protocol_send_frame(SysMessage msg, uint8_t msgType, uint8_t *data, uint16_t dataLen)
{
    uint8_t Frame[128] = {0};
    uint16_t FrameLen, usMsg = (uint16_t)msg;
    
    FrameLen = 10 + dataLen;
    Frame[0] = 0xAA;
    Frame[1] = 0x55;
    Frame[2] = FrameLen >> 8;
    Frame[3] = FrameLen & 0xFF;
    Frame[4] = usMsg >> 8;
    Frame[5] = usMsg & 0xFF;
    Frame[6] = msgType;
    Frame[7] = dataLen >> 8;
    Frame[8] = dataLen & 0xFF;
    
    if(dataLen) {
        memcpy(Frame+9, data, dataLen);
    }
    
    Frame[FrameLen-1] = hs_protocol_calCRC8(Frame, FrameLen-1);
    
    hs_usart_send_data(Frame, FrameLen);
    
    //hs_print_message(Frame, FrameLen);
}

uint8_t hs_protocol_wait_ack(uint16_t timeout)
{
    uint8_t ret = 0;
    uint16_t i, cnt = timeout / 10;
    
    for(i=0; i<cnt; i++) {
        hs_delay_ms(10);
        if(RX_STATE_F_FINISH == hs_RxFrame_get_state()) {
            ret = 1;
            break;
        }
    }

    return ret;
}

uint16_t hs_protocol_get_frame_data(uint8_t *frame)
{
    return hs_RxFrame_read_data(frame);  
}

uint8_t hs_protocol_check_frame(uint8_t *frame, uint16_t frameLen)
{
    uint8_t ret = 0;
    uint8_t ucCrc8Val = 0;
    
    ucCrc8Val = hs_protocol_calCRC8(frame, frameLen-1);
    if( ucCrc8Val == frame[frameLen-1] ) {
        ret = 1;
    }
    
    return ret;
}
    

uint8_t hs_protocol_check_cmdType(uint16_t cmd)
{
    return ((cmd == MSG_READ_EVENT)  || \
            (cmd == MSG_SYS_STANDBY) || \
            (cmd == MSG_SYS_SLEEP)   || \
            (cmd == MSG_SYS_RST)     || \
            (cmd == MSG_SYSTEM_LOCK)     || \
            (cmd == MSG_SYSTEM_UNLOCK)      || \
            (cmd == MSG_CFG_SET)     || \
            (cmd == MSG_CFG_GET)     || \
            (cmd == MSG_IPWR_UP)     || \
            (cmd == MSG_IPWR_DN)     || \
            (cmd == MSG_BATCH_CARD_START)    || \
            (cmd == MSG_BATCH_CARD_STOP)    || \
            (cmd == MSG_BATCH_CARD_INSERT)    || \
            (cmd == MSG_GET_INSERT_STATE)    || \
            (cmd == MSG_CARD_REG)    || \
            (cmd == MSG_CARD_MOD)    || \
            (cmd == MSG_CARD_DEL)    || \
            (cmd == MSG_CARD_READ)   || \
            (cmd == MSG_CARD_WRITE)  || \
            (cmd == MSG_SET_RECOVERY)|| \
            (cmd == MSG_GET_UPDATE_INFO)|| \
            (cmd == MSG_BEAT_START)  || \
            (cmd == MSG_BEATING)     || \
            (cmd == MSG_BEAT_STOP)   || \
            (cmd == MSG_OPEN_DOOR)   || \
            (cmd == MSG_CLOSE_DOOR)  || \
            (cmd == MSG_READ_DOOR_STATE) || \
            (cmd == MSG_READ_BUTTON)     || \
            (cmd == MSG_READ_CARD_ID)    || \
            (cmd == MSG_READ_KEYBOARD)   || \
            (cmd == MSG_READ_IRIS)       || \
            (cmd == MSG_READ_RST_KEY));
}

uint16_t hs_frame_get_info(uint8_t *frame, Frame_Info info, uint8_t *data)
{
    uint16_t dataLen = 0;
    
    if(FRAME_MSG == info) {
        uint16_t msg = frame[4];
        
        msg = (msg << 8) + frame[5];
        *((uint16_t*)data) = msg;
        dataLen = 2;
    } else if(FRAME_TYPE == info) {
        dataLen = 1;
        *data = frame[6];
    } else {
        dataLen = frame[7];
        dataLen = (dataLen << 8) + frame[8];
        if(dataLen) {
            memcpy(data, frame+9, dataLen);
        }
    }

    return dataLen;
}

void hs_protocol_send_crcErr_Frame(void)
{
    uint8_t frame[] = CRC_ERR_FRAME;

    hs_usart_send_data(frame, sizeof(frame));
}

