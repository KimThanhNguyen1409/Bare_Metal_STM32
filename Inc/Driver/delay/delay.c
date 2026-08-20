#include "delay.h"
#define CPU_FREQ_MHZ 8

void delay_us(uint32_t us)
{
    if (us == 0)
        return;
    SYSTICK->LOAD = (us * CPU_FREQ_MHZ) - 1;
    SYSTICK->VAL  = 0;
    SYSTICK->CTRL = 5;
    while (!(SYSTICK->CTRL & (1 << 16)))
    {
    }
    SYSTICK->CTRL = 0;
}

void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++)
    {
        delay_us(1000);
    }
}
