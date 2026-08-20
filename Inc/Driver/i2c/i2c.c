#include "i2c.h"
#include "gpio.h"

void I2C_Init(I2C_TypeDef* I2Cx)
{
    switch ((uint32_t)I2Cx)
    {
        case 1:
            RCC->APB1ENR &= ~(1 << 21);
            RCC->APB1ENR |= (1 << 21);
            GPIO_SetPin(GPIOB, 6, GPIO_MODE_AF_OPEN_DRAIN_50MHZ);
            GPIO_SetPin(GPIOB, 7, GPIO_MODE_AF_OPEN_DRAIN_50MHZ);
            break;
        case 2:
            RCC->APB1ENR &= ~(1 << 22);
            RCC->APB1ENR |= (1 << 22);
            GPIO_SetPin(GPIOB, 10, GPIO_MODE_AF_OPEN_DRAIN_50MHZ);
            GPIO_SetPin(GPIOB, 11, GPIO_MODE_AF_OPEN_DRAIN_50MHZ);
            break;
        default:
            break;
    }
    I2Cx->CR1 |= (1 << 15);
    I2Cx->CR1 &= ~(1 << 15);

    I2Cx->CR2   = 8;
    I2Cx->CCR   = 40; // CCR = f_APB1 / (2 * 100kHz) = 8MHz / (2 * 100kHz) = 40
    I2Cx->TRISE = 9;  // TRISE = (1000ns * f_APB1) + 1 = 8 + 1 = 9
    I2Cx->CR1 |= (1 << 0);
}

void I2C_WriteByte(I2C_TypeDef* I2Cx, uint8_t slave_addr, uint8_t reg_addr, uint8_t data)
{
    I2Cx->CR1 |= (1 << 8);
    while (!(I2Cx->SR1 & (1 << 0)))
        ;
    I2Cx->DR = slave_addr;
    while (!(I2Cx->SR1 & (1 << 1)))
        ;
    (void)I2Cx->SR2;
    while (!(I2Cx->SR1 & (1 << 7)))
        ;
    I2Cx->DR = reg_addr;
    while (!(I2Cx->SR1 & (1 << 7)))
        ;
    I2Cx->DR = data;
    while (!(I2Cx->SR1 & (1 << 2)))
        ;
    I2Cx->CR1 |= (1 << 9);
}