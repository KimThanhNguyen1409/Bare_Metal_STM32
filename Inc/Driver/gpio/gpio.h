#ifndef GPIO_H
#define GPIO_H

#include "stm32f103.h"

typedef enum
{
    GPIO_MODE_INPUT_ANALOG        = 0x00,
    GPIO_MODE_INPUT_FLOATING      = 0x04,
    GPIO_MODE_INPUT_PULLUD        = 0x08,
    GPIO_MODE_OUTPUT_PULL_10MHZ   = 0x01,
    GPIO_MODE_OUTPUT_PULL_2MHZ    = 0x03,
    GPIO_MODE_OUTPUT_PULL_50MHZ   = 0x07,
    GPIO_MODE_AF_PUSH_PULL_10MHZ  = 0x09,
    GPIO_MODE_AF_PUSH_PULL_2MHZ   = 0x0A,
    GPIO_MODE_AF_PUSH_PULL_50MHZ  = 0x0B,
    GPIO_MODE_AF_OPEN_DRAIN_50MHZ = 0x0F
} GPIO_MODE_Typedef;

void GPIO_EnableClock(GPIO_TypeDef* GPIOx);
void GPIO_SetPin(GPIO_TypeDef* GPIOx, uint8_t pin, GPIO_MODE_Typedef mode);
void GPIO_WritePin(GPIO_TypeDef* GPIOx, uint8_t pin, uint8_t state);
uint8_t GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint8_t pin);
void GPIO_Toggle(GPIO_TypeDef* GPIOx, uint8_t pin);

#endif // GPIO_H