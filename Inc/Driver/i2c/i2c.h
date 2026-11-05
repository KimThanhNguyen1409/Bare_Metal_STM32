#ifndef I2C_H
#define I2C_H

#include "stm32f103.h"

void I2C_Init(I2C_TypeDef *I2Cx);
void I2C_WriteByte(I2C_TypeDef *I2Cx, uint8_t slave_addr, uint8_t reg_addr, uint8_t data);

#endif //I2C_H