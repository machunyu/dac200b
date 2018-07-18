#include "hs_beep.h"


#define BEEP            PB11
#define BEEP_Port       PB
#define BEEP_Pin	BIT11


void hs_beep_init(void)
{
    GPIO_SetMode(BEEP_Port, BEEP_Pin, GPIO_PMD_OUTPUT);
    BEEP = 0;
}

void hs_beep_on(void)
{
    BEEP = 1;
}

void hs_beep_toggle(void)
{
    BEEP_Port->DOUT ^= BEEP_Pin;
}

void hs_beep_off(void)
{
    BEEP = 0;
}
