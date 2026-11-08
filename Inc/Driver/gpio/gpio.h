#ifndef GPIO_H
#define GPIO_H

#include "stm32f103.h"

typedef enum
{
    GPIO_OK = 0,
    GPIO_ERR_PARAM,
    GPIO_ERR_NULL_PTR
} GPIO_Status_t;

#define GPIO_PIN_0   (1U << 0)
#define GPIO_PIN_1   (1U << 1)
#define GPIO_PIN_2   (1U << 2)
#define GPIO_PIN_3   (1U << 3)
#define GPIO_PIN_4   (1U << 4)
#define GPIO_PIN_5   (1U << 5)
#define GPIO_PIN_6   (1U << 6)
#define GPIO_PIN_7   (1U << 7)
#define GPIO_PIN_8   (1U << 8)
#define GPIO_PIN_9   (1U << 9)
#define GPIO_PIN_10  (1U << 10)
#define GPIO_PIN_11  (1U << 11)
#define GPIO_PIN_12  (1U << 12)
#define GPIO_PIN_13  (1U << 13)
#define GPIO_PIN_14  (1U << 14)
#define GPIO_PIN_15  (1U << 15)
#define GPIO_PIN_ALL (0xFFFFU)

typedef enum
{
    GPIO_MODE_INPUT_ANALOG      = 0x00,
    GPIO_MODE_INPUT_FLOATING    = 0x04,
    GPIO_MODE_INPUT_PULLDOWN    = 0x08,
    GPIO_MODE_INPUT_PULLUP      = 0x18,

    GPIO_MODE_OUTPUT_PULL_10MHZ = 0x01,
    GPIO_MODE_OUTPUT_PULL_2MHZ  = 0x02,
    GPIO_MODE_OUTPUT_PULL_50MHZ = 0x03,

    GPIO_MODE_OUTPUT_OD_10MHZ   = 0x05,
    GPIO_MODE_OUTPUT_OD_2MHZ    = 0x06,
    GPIO_MODE_OUTPUT_OD_50MHZ   = 0x07,

    GPIO_MODE_AF_PP_10MHZ       = 0x09,
    GPIO_MODE_AF_PP_2MHZ        = 0x0A,
    GPIO_MODE_AF_PP_50MHZ       = 0x0B,

    GPIO_MODE_AF_OD_10MHZ       = 0x0D,
    GPIO_MODE_AF_OD_2MHZ        = 0x0E,
    GPIO_MODE_AF_OD_50MHZ       = 0x0F
} GPIO_Mode_t;

GPIO_Status_t GPIO_EnableClock(GPIO_TypeDef *GPIOx);
GPIO_Status_t GPIO_SetPin(GPIO_TypeDef *GPIOx, uint8_t pin, GPIO_Mode_t mode);
GPIO_Status_t GPIO_WritePin(GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t state);
uint8_t       GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint8_t pin);
GPIO_Status_t GPIO_Toggle(GPIO_TypeDef *GPIOx, uint8_t pin);

void          GPIO_WritePort(GPIO_TypeDef *GPIOx, uint16_t value);
uint16_t      GPIO_ReadPort(GPIO_TypeDef *GPIOx);

#endif // GPIO_H
