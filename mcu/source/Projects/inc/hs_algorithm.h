#ifndef  __HS_ALGORITHM_H__
#define  __HS_ALGORITHM_H__


#include "Nano100Series.h"
#include "hs_rc523_api.h"


typedef uint8_t (*Key_t)[12];


void hs_algorithm_open_door(void);

void hs_algorithm_close_door(void);

uint8_t hs_algorithm_is_card_open(RC523_Card_Type card, uint8_t *cid, Key_t key);


#endif
