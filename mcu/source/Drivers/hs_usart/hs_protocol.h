#ifndef  __HS_PROTOCOL_H__
#define  __HS_PROTOCOL_H__

#include "Nano100Series.h"


#define MSG_TYPE_REQ     0x10
#define MSG_TYPE_ACK     0x20

#define ACK_SUCCEED      0x01
#define ACK_FAILED       0x02


#define CRC_ERR_FRAME    { 0xAA, 0x55, 0x00, 0x09, 0xFF, 0xEE, 0xFF, 0xEE, 0x48 }


typedef enum {
    MSG_READ_EVENT      = 0x0101,
    MSG_SYS_STANDBY     = 0x0201,
    MSG_SYS_SLEEP       = 0x0202,
    MSG_SYS_RST         = 0x0203,
    MSG_SYSTEM_LOCK     = 0x0204,
    MSG_SYSTEM_UNLOCK   = 0x0205,
    MSG_CFG_SET         = 0x0301,
    MSG_CFG_GET         = 0x0302,
    MSG_IPWR_UP         = 0x0401,
    MSG_IPWR_DN         = 0x0402,
    MSG_GET_UPDATE_INFO = 0x0602,
    MSG_BEAT_START      = 0x0701,
    MSG_BEATING         = 0x0702,
    MSG_BEAT_STOP       = 0x0703,
    MSG_OPEN_DOOR       = 0x0801,
    MSG_CLOSE_DOOR      = 0x0802,
    MSG_READ_DOOR_STATE = 0x0803,
    MSG_READ_BUTTON     = 0x0804,
    MSG_READ_KEYBOARD   = 0x0806,
    MSG_READ_IRIS       = 0x0807,
    MSG_READ_RST_KEY    = 0x0808,
    MSG_BATCH_CARD_START = 0x0901,
    MSG_BATCH_CARD_STOP = 0x0902,
    MSG_BATCH_CARD_INSERT = 0x0903,
    MSG_GET_INSERT_STATE = 0x0904,
    
    MSG_CARD_REG        = 0x2001,
    MSG_CARD_MOD        = 0x2002,
    MSG_CARD_DEL        = 0x2003,
    MSG_CARD_READ       = 0x2004,
    MSG_CARD_WRITE      = 0x2005,
    MSG_READ_CARD_ID    = 0x2006,
    
    MSG_SET_RECOVERY    = 0x2101,
      
} SysMessage;

typedef enum {
    FRAME_MSG,
    FRAME_TYPE,
    FRAME_DATA
} Frame_Info;


uint8_t hs_protocol_calCRC8(uint8_t *vptr, uint32_t len);

uint16_t hs_protocol_get_frame_data(uint8_t *frame);

uint8_t hs_protocol_check_frame(uint8_t *frame, uint16_t frameLen);

uint8_t hs_protocol_check_cmdType(uint16_t cmd);

uint8_t hs_protocol_wait_ack(uint16_t timeout);

uint16_t hs_frame_get_info(uint8_t *frame, Frame_Info info, uint8_t *data);

void hs_protocol_send_frame(SysMessage msg, uint8_t msgType, uint8_t *data, uint16_t dataLen);

void hs_protocol_send_crcErr_Frame(void);


#endif
