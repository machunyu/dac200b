

#include "hs_key.h"


#define RESET_KEY               PB4
#define RESET_KEY_Port          PB
#define RESET_KEY_PIN           BIT4


void hs_key_init(void)
{
    GPIO_SetMode(RESET_KEY_Port, RESET_KEY_PIN, GPIO_PMD_INPUT);
    GPIO_ENABLE_PULL_UP(RESET_KEY_Port, RESET_KEY_PIN);
}

uint8_t hs_key_read_pin(void)
{
    return RESET_KEY;
}
