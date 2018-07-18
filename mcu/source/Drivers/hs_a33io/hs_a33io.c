#include "hs_a33io.h"


#define A33_IO            PB8
#define A33_IO_Port       PB
#define A33_IO_Pin	  BIT8


void hs_a33_io_init(void)
{
    GPIO_SetMode(A33_IO_Port, A33_IO_Pin, GPIO_PMD_OUTPUT);
    A33_IO = 1;
}

void hs_a33_io_set_high(void)
{
    A33_IO = 1;
}

void hs_a33_io_set_low(void)
{
    A33_IO = 0;
}
