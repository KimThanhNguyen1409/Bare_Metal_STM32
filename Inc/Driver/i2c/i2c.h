#ifndef I2C_H
#define I2C_H

#include "stm32f103.h"

typedef enum
{
    I2C_OK = 0,
    I2C_ERR_NULL_PTR,
    I2C_ERR_PARAM,
    I2C_ERR_TIMEOUT,
    I2C_ERR_BUSY,
    I2C_ERR_NACK
} I2C_Status_t;

#define I2C_CR1_PE    (1U << 0)
#define I2C_CR1_START (1U << 8)
#define I2C_CR1_STOP  (1U << 9)
#define I2C_CR1_ACK   (1U << 10)
#define I2C_CR1_SWRST (1U << 15)
#define I2C_SR1_SB    (1U << 0)
#define I2C_SR1_ADDR  (1U << 1)
#define I2C_SR1_BTF   (1U << 2)
#define I2C_SR1_RXNE  (1U << 6)
#define I2C_SR1_TXE   (1U << 7)
#define I2C_SR1_AF    (1U << 10)
#define I2C_SR2_BUSY  (1U << 1)

I2C_Status_t I2C_Init(I2C_TypeDef *I2Cx, uint32_t apb1_clock_hz, uint32_t speed_hz);
void         I2C_Bus_ClearStuck(I2C_TypeDef *I2Cx);
I2C_Status_t I2C_IsDeviceReady(I2C_TypeDef *I2Cx, uint8_t dev_addr, uint32_t timeout);
I2C_Status_t I2C_WriteByte(I2C_TypeDef *I2Cx, uint8_t dev_addr, uint8_t reg_addr, uint8_t data, uint32_t timeout);
I2C_Status_t I2C_WriteBuffer(
    I2C_TypeDef *I2Cx, uint8_t dev_addr, uint8_t reg_addr, const uint8_t *pData, uint16_t len, uint32_t timeout);
I2C_Status_t I2C_ReadByte(I2C_TypeDef *I2Cx, uint8_t dev_addr, uint8_t reg_addr, uint8_t *pData, uint32_t timeout);
I2C_Status_t
I2C_ReadBuffer(I2C_TypeDef *I2Cx, uint8_t dev_addr, uint8_t reg_addr, uint8_t *pData, uint16_t len, uint32_t timeout);

#endif // I2C_H
