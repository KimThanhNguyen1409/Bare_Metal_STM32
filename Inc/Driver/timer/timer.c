#include "timer.h"

void TIM_Init(TIMER_TypeDef* TIMx, uint16_t psc, uint16_t arr)
{
    switch ((uint32_t)(TIMx))
    {
        case TIMER1_PERIPH_BASE:
            RCC->APB2ENR |= (1 << 11);
            break;
        case TIMER2_PERIPH_BASE:
            RCC->APB1ENR |= (1 << 0);
            break;
        case TIMER3_PERIPH_BASE:
            RCC->APB1ENR |= (1 << 1);
            break;
        default:
            break;
    }
    TIMx->ARR = arr;
    TIMx->PSC = psc;
    TIMx->EGR |= (1 << 0); // Start and generates an update of the register (Read more in RM)
}
void TIM_Start(TIMER_TypeDef* TIMx)
{
    TIMx->CR1 |= (1 << 0);
}
void TIM_DelayUs(TIMER_TypeDef* TIMx, uint16_t us)
{
    TIMx->CNT = 0;
    while (TIMx->CNT < us)
    {
    }
}
void TIM_DelayMs(TIMER_TypeDef* TIMx, uint16_t ms)
{
    for (uint16_t i = 0; i < ms; i++)
    {
        TIM_DelayUs(TIMx, 1000);
    }
}
void TIM_Stop(TIMER_TypeDef* TIMx)
{
    TIMx->CR1 &= ~(1 << 0);
}
uint16_t TIM_GetCounter(TIMER_TypeDef* TIMx)
{
    return (uint16_t)(TIMx->CNT);
}
void TIM_SetCounter(TIMER_TypeDef* TIMx, uint16_t value)
{
    TIMx->CNT = value;
}
void TIM_PWM_Init(TIMER_TypeDef* TIMx, uint8_t channel)
{
    switch (channel)
    {
        case 1:
            TIMx->CCMR1 &= ~(0xFF << 0);
            TIMx->CCMR1 |= (0x06 << 4) | (1 << 3);
            TIMx->CCER |= (1 << 0);
            break;
        case 2:
            TIMx->CCMR1 &= ~(0xFF << 0);
            TIMx->CCMR1 |= (0x06 << 12) | (1 << 11);
            TIMx->CCER |= (1 << 4);
            break;
        case 3:
            TIMx->CCMR2 &= ~(0xFF << 0);
            TIMx->CCMR2 |= (0x06 << 4) | (1 << 3);
            TIMx->CCER |= (1 << 8);
            break;
        case 4:
            TIMx->CCMR2 &= ~(0xFF << 0);
            TIMx->CCMR2 |= (0x06 << 12) | (1 << 11);
            TIMx->CCER |= (1 << 12);
            break;
        default:
            break;
    }
}
void TIM_PWM_SetDuty(TIMER_TypeDef* TIMx, uint8_t channel, uint16_t value)
{
    switch (channel)
    {
        case 1:
            TIMx->CCR1 = value;
            break;
        case 2:
            TIMx->CCR2 = value;
            break;
        case 3:
            TIMx->CCR3 = value;
            break;
        case 4:
            TIMx->CCR4 = value;
            break;
        default:
            break;
    }
}