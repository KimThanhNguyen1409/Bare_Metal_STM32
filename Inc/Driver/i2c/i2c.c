#include "i2c.h"
#include "gpio.h"
#include "delay.h"

static I2C_Status_t
I2C_WaitFlag(I2C_TypeDef *I2Cx, volatile uint32_t *reg, uint32_t flag, uint8_t state, uint32_t timeout)
{
    uint32_t tick = timeout;
    while (((*reg & flag) != 0) != state)
    {
        if (I2Cx->SR1 & I2C_SR1_AF)
        {
            I2Cx->SR1 &= ~I2C_SR1_AF;
            I2Cx->CR1 |= I2C_CR1_STOP;
            return I2C_ERR_NACK;
        }
        if (timeout > 0 && --tick == 0)
        {
            I2Cx->CR1 |= I2C_CR1_STOP;
            return I2C_ERR_TIMEOUT;
        }
    }
    return I2C_OK;
}

void I2C_Bus_ClearStuck(I2C_TypeDef *I2Cx)
{
    uint8_t scl_pin = (I2Cx == I2C1) ? 6 : 10;
    uint8_t sda_pin = (I2Cx == I2C1) ? 7 : 11;
    GPIO_EnableClock(GPIOB);
    GPIO_SetPin(GPIOB, scl_pin, GPIO_MODE_OUTPUT_OD_50MHZ);
    GPIO_SetPin(GPIOB, sda_pin, GPIO_MODE_OUTPUT_OD_50MHZ);
    GPIO_WritePin(GPIOB, sda_pin, 1);
    for (uint8_t i = 0; i < 9; i++)
    {
        GPIO_WritePin(GPIOB, scl_pin, 0);
        delay_us(5);
        GPIO_WritePin(GPIOB, scl_pin, 1);
        delay_us(5);
    }

    GPIO_WritePin(GPIOB, sda_pin, 0);
    delay_us(5);
    GPIO_WritePin(GPIOB, scl_pin, 1);
    delay_us(5);
    GPIO_WritePin(GPIOB, sda_pin, 1);
}

I2C_Status_t I2C_Init(I2C_TypeDef *I2Cx, uint32_t apb1_clock_hz, uint32_t speed_hz)
{
    if (I2Cx == NULL)
        return I2C_ERR_NULL_PTR;
    if (apb1_clock_hz < 2000000UL || speed_hz == 0)
        return I2C_ERR_PARAM;

    GPIO_EnableClock(GPIOB);

    if (I2Cx == I2C1)
    {
        RCC->APB1ENR |= (1U << 21);
        GPIO_SetPin(GPIOB, 6, GPIO_MODE_AF_OD_50MHZ); // SCL: PB6
        GPIO_SetPin(GPIOB, 7, GPIO_MODE_AF_OD_50MHZ); // SDA: PB7
    }
    else if (I2Cx == I2C2)
    {
        RCC->APB1ENR |= (1U << 22);
        GPIO_SetPin(GPIOB, 10, GPIO_MODE_AF_OD_50MHZ); // SCL: PB10
        GPIO_SetPin(GPIOB, 11, GPIO_MODE_AF_OD_50MHZ); // SDA: PB11
    }
    else
    {
        return I2C_ERR_PARAM;
    }

    I2Cx->CR1 |= I2C_CR1_SWRST;
    I2Cx->CR1 &= ~I2C_CR1_SWRST;

    uint32_t freq_mhz = apb1_clock_hz / 1000000UL;
    I2Cx->CR2         = freq_mhz & 0x3FUL;

    if (speed_hz <= 1000000UL)
    {
        uint32_t crr = apb1_clock_hz / (2U * speed_hz);
        if (crr < 4U)
            crr = 4U;
        I2Cx->CCR   = crr;
        I2Cx->TRISE = freq_mhz + 1U;
    }
    else
    {
        uint32_t ccr = apb1_clock_hz / (3U * speed_hz);
        if (ccr < 1U)
            ccr = 1U;
        I2Cx->CCR   = (1U << 15) | ccr; // Bit 15: Fast Mode
        I2Cx->TRISE = ((freq_mhz * 300U) / 1000U) + 1U;
    }

    I2Cx->CR1 |= I2C_CR1_PE;
    return I2C_OK;
}

I2C_Status_t I2C_IsDeviceReady(I2C_TypeDef *I2Cx, uint8_t dev_addr, uint32_t timeout)
{
    if (I2Cx == NULL)
        return I2C_ERR_NULL_PTR;
    if (I2Cx->SR2 & I2C_SR2_BUSY)
        return I2C_ERR_BUSY;
    I2Cx->CR1 |= I2C_CR1_START;
    I2C_Status_t st = I2C_WaitFlag(I2Cx, &I2Cx->SR1, I2C_SR1_SB, 1, timeout);
    if (st != I2C_OK)
        return st;
    I2Cx->DR = (uint32_t)(dev_addr << 1);
    st       = I2C_WaitFlag(I2Cx, &I2Cx->SR1, I2C_SR1_ADDR, 1, timeout);
    if (st != I2C_OK)
        return st;
    (void)I2Cx->SR1;
    (void)I2Cx->SR2;
    I2Cx->CR1 |= I2C_CR1_STOP;
    return I2C_OK;
}

I2C_Status_t I2C_WriteBuffer(
    I2C_TypeDef *I2Cx, uint8_t dev_addr, uint8_t reg_addr, const uint8_t *pData, uint16_t len, uint32_t timeout)
{
    if (I2Cx == NULL || pData == NULL)
        return I2C_ERR_NULL_PTR;
    if (I2Cx->SR2 & I2C_SR2_BUSY)
        return I2C_ERR_BUSY;
    I2C_Status_t st;
    I2Cx->CR1 |= I2C_CR1_START;
    if ((st = I2C_WaitFlag(I2Cx, &I2Cx->SR1, I2C_SR1_SB, 1, timeout)) != I2C_OK)
        return st;
    I2Cx->DR = (uint32_t)(dev_addr << 1);
    if ((st = I2C_WaitFlag(I2Cx, &I2Cx->SR1, I2C_SR1_ADDR, 1, timeout)) != I2C_OK)
        return st;
    (void)I2Cx->SR1;
    (void)I2Cx->SR2;
    if ((st = I2C_WaitFlag(I2Cx, &I2Cx->SR1, I2C_SR1_TXE, 1, timeout)) != I2C_OK)
        return st;
    I2Cx->DR = reg_addr;
    for (uint16_t i = 0; i < len; i++)
    {
        if ((st = I2C_WaitFlag(I2Cx, &I2Cx->SR1, I2C_SR1_TXE, 1, timeout)) != I2C_OK)
            return st;
        I2Cx->DR = pData[i];
    }
    if ((st = I2C_WaitFlag(I2Cx, &I2Cx->SR1, I2C_SR1_BTF, 1, timeout)) != I2C_OK)
        return st;
    I2Cx->CR1 |= I2C_CR1_STOP;
    return I2C_OK;
}

I2C_Status_t I2C_WriteByte(I2C_TypeDef *I2Cx, uint8_t dev_addr, uint8_t reg_addr, uint8_t data, uint32_t timeout)
{
    return I2C_WriteBuffer(I2Cx, dev_addr, reg_addr, &data, 1, timeout);
}

I2C_Status_t
I2C_ReadBuffer(I2C_TypeDef *I2Cx, uint8_t dev_addr, uint8_t reg_addr, uint8_t *pData, uint16_t len, uint32_t timeout)
{
    if (I2Cx == NULL || pData == NULL)
        return I2C_ERR_NULL_PTR;
    if (len == 0)
        return I2C_ERR_PARAM;
    if (I2Cx->SR2 & I2C_SR2_BUSY)
        return I2C_ERR_BUSY;
    I2C_Status_t st;
    I2Cx->CR1 |= I2C_CR1_START;
    if ((st = I2C_WaitFlag(I2Cx, &I2Cx->SR1, I2C_SR1_SB, 1, timeout)) != I2C_OK)
        return st;
    I2Cx->DR = (uint32_t)(dev_addr << 1);
    if ((st = I2C_WaitFlag(I2Cx, &I2Cx->SR1, I2C_SR1_ADDR, 1, timeout)) != I2C_OK)
        return st;
    (void)I2Cx->SR1;
    (void)I2Cx->SR2;
    if ((st = I2C_WaitFlag(I2Cx, &I2Cx->SR1, I2C_SR1_TXE, 1, timeout)) != I2C_OK)
        return st;
    I2Cx->DR = reg_addr;
    if ((st = I2C_WaitFlag(I2Cx, &I2Cx->SR1, I2C_SR1_TXE, 1, timeout)) != I2C_OK)
        return st;

    I2Cx->CR1 |= I2C_CR1_ACK;
    I2Cx->CR1 |= I2C_CR1_START;
    if ((st = I2C_WaitFlag(I2Cx, &I2Cx->SR1, I2C_SR1_SB, 1, timeout)) != I2C_OK)
        return st;
    I2Cx->DR = (uint32_t)((dev_addr << 1) | 1U);
    if ((st = I2C_WaitFlag(I2Cx, &I2Cx->SR1, I2C_SR1_ADDR, 1, timeout)) != I2C_OK)
        return st;

    if (len == 1)
    {
        I2Cx->CR1 &= ~I2C_CR1_ACK;
        (void)I2Cx->SR1;
        (void)I2Cx->SR2;
        I2Cx->CR1 |= I2C_CR1_STOP;
        if ((st = I2C_WaitFlag(I2Cx, &I2Cx->SR1, I2C_SR1_RXNE, 1, timeout)) != I2C_OK)
            return st;
        pData[0] = (uint8_t)I2Cx->DR;
    }
    else
    {
        (void)I2Cx->SR1;
        (void)I2Cx->SR2;
        for (uint16_t i = 0; i < len; i++)
        {
            if (i == (len - 1U))
            {
                I2Cx->CR1 &= ~I2C_CR1_ACK;
                I2Cx->CR1 |= I2C_CR1_STOP;
            }
            if ((st = I2C_WaitFlag(I2Cx, &I2Cx->SR1, I2C_SR1_RXNE, 1, timeout)) != I2C_OK)
                return st;
            pData[i] = (uint8_t)I2Cx->DR;
        }
    }

    return I2C_OK;
}

I2C_Status_t I2C_ReadByte(I2C_TypeDef *I2Cx, uint8_t dev_addr, uint8_t reg_addr, uint8_t *pData, uint32_t timeout)
{
    return I2C_ReadBuffer(I2Cx, dev_addr, reg_addr, pData, 1, timeout);
}