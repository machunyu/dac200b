#include "hs_power.h"


#define A33_POWER_CTL                  PB7
#define A33_POWER_CTL_Port             PB
#define A33_POWER_CTL_Pin	       BIT7

#define IRIS_POWER_CTL                 PE5
#define IRIS_POWER_CTL_Port            PE
#define IRIS_POWER_CTL_Pin	       BIT5

#define A33_SLEEP_CTL                  PB5
#define A33_SLEEP_CTL_Port             PB
#define A33_SLEEP_CTL_Pin	       BIT5

void hs_power_init(void)
{
    GPIO_SetMode(A33_POWER_CTL_Port, A33_POWER_CTL_Pin, GPIO_PMD_OUTPUT);
    GPIO_SetMode(IRIS_POWER_CTL_Port, IRIS_POWER_CTL_Pin, GPIO_PMD_OUTPUT);
    GPIO_SetMode(A33_SLEEP_CTL_Port, A33_SLEEP_CTL_Pin, GPIO_PMD_INPUT);
    
    A33_POWER_CTL = 0; IRIS_POWER_CTL = 0;
}

void hs_a33_power_on(void)
{
    A33_POWER_CTL = 1;
}

void hs_a33_power_off(void)
{
    A33_POWER_CTL = 0;
}

void hs_iris_on(void)
{
    IRIS_POWER_CTL = 1;
}

void hs_iris_off(void)
{
    IRIS_POWER_CTL = 0;
}
