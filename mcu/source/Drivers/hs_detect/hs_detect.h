#ifndef  __HS_DETECT_H__
#define  __HS_DETECT_H__

#ifdef __cplusplus
 extern "C" {
#endif
   
#include "Nano100Series.h"
   
   
#define IDEL_VAL               50
#define THRESHOLD_VAL          (IDEL_VAL + 350)
   
   
void hs_detect_init(void);

uint16_t hs_detect_get_voltage(void);

uint8_t hs_detect_get_wakeup_state(void);


#ifdef __cplusplus
}
#endif
#endif