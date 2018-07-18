#ifndef  __HS_MODULE_H__
#define  __HS_MODULE_H__


#include "Nano100Series.h"


typedef enum {
    STATUS_OK = 0,
    STATUS_ERR_RC523,
    STATUS_ERR_KEYBOARD
} Module_Status_t;


uint8_t hs_module_init(void);

uint8_t hs_module_later_init(void);

uint8_t hs_module_suspend(void);

uint8_t hs_module_resume(void);

uint8_t hs_module_reset(void);

uint8_t hs_module_probe(void);

uint8_t hs_module_sleep(void);

uint8_t hs_module_wakeup(void);

void hs_module_show_sync(void);

uint8_t hs_module_status_cback(Module_Status_t status);

uint8_t hs_module_response_key(uint8_t keyVal);


#endif
