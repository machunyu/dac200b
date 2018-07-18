#ifndef _HS_MSG_PROCESS_H_
#define _HS_MSG_PROCESS_H_

uint8_t hs_check_frame(RxFrame_t *pFrame);

void hs_msg_int_process(uint16_t usCommand, uint8_t *buf, uint16_t len);

void hs_msg_poll_process(uint16_t usCommand,  uint8_t *ReqData, uint16_t ReqLen);

#endif
