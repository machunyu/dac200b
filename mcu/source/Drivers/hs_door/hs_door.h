#ifndef  __HS_DOOR_H__
#define  __HS_DOOR_H__


#include "Nano100Series.h"
#include "hs_config.h"



void hs_door_init(void);

void hs_door_open(uint8_t method);

void hs_door_close(uint8_t method);

uint8_t hs_door_read_state(void);

uint8_t hs_door_read_button(void);


#endif
