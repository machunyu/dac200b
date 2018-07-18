#ifndef  __HS_QUEUE_H__
#define  __HS_QUEUE_H__


#include "Nano100Series.h"


extern void hs_RxLoopQueue_create(void);

extern void hs_RxLoopQueue_clear(void);

extern uint8_t hs_RxLoopQueue_is_empty(void);

extern uint8_t hs_RxLoopQueue_is_full(void);

extern void hs_RxLoopQueue_enter(uint8_t data);

extern uint8_t hs_RxLoopQueue_delete(void);

extern uint16_t hs_RxLoopQueue_get_size(void);

extern uint16_t hs_RxLoopQueue_get_data(uint8_t **data);

extern uint16_t hs_RxLoopQueue_read_all(uint8_t *data);

extern uint16_t hs_RxLoopQueue_read_data(uint8_t *data, uint16_t size);

extern void hs_RxLoopQueue_test(void);


#endif