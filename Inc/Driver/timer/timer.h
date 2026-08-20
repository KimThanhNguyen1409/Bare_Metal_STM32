#ifndef TIMER_H
#define TIMER_H

#include "stm32f103.h"

void TIM_Init(TIMER_TypeDef* TIMx, uint16_t psc, uint16_t arr);
void TIM_Start(TIMER_TypeDef* TIMx);
void TIM_Stop(TIMER_TypeDef* TIMx);
uint16_t TIM_GetCounter(TIMER_TypeDef* TIMx);
void TIM_SetCounter(TIMER_TypeDef* TIMx, uint16_t value);

void TIM_DelayUs(TIMER_TypeDef* TIMx, uint16_t us);
void TIM_DelayMs(TIMER_TypeDef* TIMx, uint16_t ms);

void TIM_PWM_Init(TIMER_TypeDef* TIMx, uint8_t channel);
void TIM_PWM_SetDuty(TIMER_TypeDef* TIMx, uint8_t channel, uint16_t value);

#endif // TIMER_H