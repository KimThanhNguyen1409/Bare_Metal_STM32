#include "delay.h"

#define CPU_FREQ_MHZ        8
#define COREDEBUG_DEMRC_TRC (1 << 24)
#define DWT_CTRL_CYCCNTENA  (1 << 0)

void delay_init()
{
    COREDEBUG->DEMCR = COREDEBUG_DEMRC_TRC;

    DWT->CYCCNT      = 0;

    DWT->CTRL        = DWT_CTRL_CYCCNTENA;
}

void delay_us(uint32_t us)
{
    if (us == 0)
        return;

    if (!(DWT->CTRL & DWT_CTRL_CYCCNTENA))
    {
        delay_init();
    }

    uint32_t start_tick = DWT->CYCCNT;
    uint32_t wait_ticks = us * CPU_FREQ_MHZ;

    while ((DWT->CYCCNT - start_tick) < wait_ticks)
    {
    }
}

void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++)
    {
        delay_us(1000);
    }
}
