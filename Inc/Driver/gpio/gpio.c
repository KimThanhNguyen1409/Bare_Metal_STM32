#include "gpio.h"

GPIO_Status_t GPIO_EnableClock(GPIO_TypeDef *GPIOx)
{
    if (GPIOx == NULL)
    {
        return GPIO_ERR_NULL_PTR;
    }
    if (GPIOx == GPIOA)
        RCC->APB2ENR |= (1U << 2);
    else if (GPIOx == GPIOB)
        RCC->APB2ENR |= (1U << 3);
    else if (GPIOx == GPIOC)
        RCC->APB2ENR |= (1U << 4);
    else if (GPIOx == GPIOD)
        RCC->APB2ENR |= (1U << 5);
    else if (GPIOx == GPIOE)
        RCC->APB2ENR |= (1U << 6);
    else
    {
        return GPIO_ERR_PARAM;
    }
    return GPIO_OK;
}

GPIO_Status_t GPIO_SetPin(GPIO_TypeDef *GPIOx, uint8_t pin, GPIO_Mode_t mode)
{
    if (GPIOx == NULL)
    {
        return GPIO_ERR_NULL_PTR;
    }
    if (pin > 15)
    {
        return GPIO_ERR_PARAM;
    }
    if (mode == GPIO_MODE_INPUT_PULLUP)
    {
        GPIOx->BSRR = (1U << pin);
    }
    else if (mode == GPIO_MODE_INPUT_PULLDOWN)
    {
        GPIOx->BRR = (1U << pin);
    }

    uint32_t config_value = (uint32_t)(mode & 0x0FU);

    if (pin < 8)
    {
        uint32_t shift = pin * 4U;
        GPIOx->CRL &= ~(0x0FU << shift);
        GPIOx->CRL |= (config_value << shift);
    }
    else
    {
        uint32_t shift = (pin - 8U) * 4U;
        GPIOx->CRH &= ~(0x0FU << shift);
        GPIOx->CRH |= (config_value << shift);
    }
    return GPIO_OK;
}

GPIO_Status_t GPIO_WritePin(GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t state)
{
    if (GPIOx == NULL)
        return GPIO_ERR_NULL_PTR;
    if (pin > 15)
        return GPIO_ERR_PARAM;

    if (state)
    {
        GPIOx->BSRR = (1 << pin);
    }
    else
    {
        GPIOx->BRR = (1 << pin);
    }

    return GPIO_OK;
}
uint8_t GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint8_t pin)
{
    if (GPIOx == NULL || pin > 15)
    {
        return 0;
    }
    return (uint8_t)((GPIOx->IDR >> pin) & 0x01U);
}
void GPIO_Toggle(GPIO_TypeDef *GPIOx, uint8_t pin)
{
    if (GPIOx == NULL)
        return GPIO_ERR_NULL_PTR;
    if (pin > 15)
        return GPIO_ERR_PARAM;

    if (GPIOx->ODR & (1U << pin))
    {
        GPIOx->BRR = (1U << pin);
    }
    else
    {
        GPIOx->BSRR = (1U << pin);
    }
    return GPIO_OK;
}

void GPIO_WritePort(GPIO_TypeDef *GPIOx, uint16_t value)
{
    if (GPIOx != NULL)
    {
        GPIOx->ODR = value;
    }
}

uint16_t GPIO_ReadPort(GPIO_TypeDef *GPIOx)
{
    if (GPIOx == NULL)
        return 0;
    return (uint16_t)(GPIOx->IDR & 0xFFFFU);
}