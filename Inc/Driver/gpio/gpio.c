#include "gpio.h"

void GPIO_EnableClock(GPIO_TypeDef* GPIOx)
{
    switch ((uint32_t)GPIOx)
    {
        case GPIOA_PERIPH_BASE:
            RCC->APB2ENR |= (1 << 2);
            break;
        case GPIOB_PERIPH_BASE:
            RCC->APB2ENR |= (1 << 3);
            break;
        case GPIOC_PERIPH_BASE:
            RCC->APB2ENR |= (1 << 4);
            break;
        case GPIOD_PERIPH_BASE:
            RCC->APB2ENR |= (1 << 5);
            break;
        case GPIOE_PERIPH_BASE:
            RCC->APB2ENR |= (1 << 6);
            break;
        default:
            break;
    }
}
void GPIO_SetPin(GPIO_TypeDef* GPIOx, uint8_t pin, GPIO_MODE_Typedef mode)
{
    if (pin < 8)
    {
        GPIOx->CRL &= ~(0xF << (pin * 4));
        GPIOx->CRL |= (mode << (pin * 4));
    }
    else
    {
        GPIOx->CRH &= ~(0xF << ((pin - 8) * 4));
        GPIOx->CRH |= (mode << ((pin - 8) * 4));
    }
}
void GPIO_WritePin(GPIO_TypeDef* GPIOx, uint8_t pin, uint8_t state)
{
    if (state)
    {
        GPIOx->BSRR = (1 << pin);
    }
    else
    {
        GPIOx->BRR = (1 << pin);
    }
}
uint8_t GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint8_t pin)
{
    return ((GPIOx->IDR >> pin) & 1);
}
void GPIO_Toggle(GPIO_TypeDef* GPIOx, uint8_t pin)
{
    GPIOx->ODR ^= (1 << pin);
}