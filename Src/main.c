#include "delay.h"
#include "gpio.h"
#include "stm32f103.h"

int main(void)
{
    GPIO_EnableClock(GPIOC);
    GPIO_SetPin(GPIOC, 13, GPIO_MODE_OUTPUT_PULL_2MHZ);
    while (1)
    {
        GPIO_Toggle(GPIOC, 13);
        delay_ms(1000);
    }
}