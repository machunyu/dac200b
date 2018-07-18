#ifndef _HS_USART_H_
#define _HS_USART_H_


#include "Nano100Series.h"


#define FRAME_BUF_LEN      128


typedef enum {
    RX_STATE_F_HEAD = 1,
    RX_STATE_F_LEN,
    RX_STATE_F_FINISH
} RxState_t;

#pragma pack(1)
typedef struct {
    uint16_t header;
    uint16_t frame_len;
    uint16_t command;
    uint8_t  cmd_type;
    uint16_t dat_len;
    uint8_t  dat[FRAME_BUF_LEN-9];
}stFrame;
#pragma pack()

typedef struct {
    union {
        stFrame m_Frame;
        uint8_t  Frame[FRAME_BUF_LEN];
    };
    volatile uint16_t FrameLen;
    volatile uint16_t FrameRecvLen;
    volatile RxState_t FrameState;
} RxFrame_t;


extern void hs_usart_init(void);

extern void hs_usart_send_char(uint8_t data);

extern void hs_usart_send_string(uint8_t *string);

extern void hs_usart_send_data(uint8_t *data, uint16_t len);

extern void hs_RxFrame_clear(void);

extern void hs_RxFrame_insert_data(uint8_t data);

extern void hs_RxFrame_update_rx_state(void);

extern RxState_t hs_RxFrame_get_state(void);

extern uint16_t hs_RxFrame_read_data(uint8_t *data);

extern void hs_print_message(uint8_t *msg, uint16_t len);

extern RxFrame_t  RxFrame;

#endif