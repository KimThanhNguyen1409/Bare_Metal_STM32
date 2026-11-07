#ifndef SPI_H
#define SPI_H
#include "stm32f103.h"

typedef enum
{
    SPI_OK = 0,
    SPI_ERR_TIMEOUT,
    SPI_ERR_BUSY,
    SPI_ERR_NULL_PTR,
    SPI_ERR_MODF,
    SPI_ERR_OVR
} SPI_Status_t;

typedef enum{
    SPI_MODE_SLAVE = 0x00,
    SPI_MODE_MASTER = (1 << 2)
}SPI_Mode_t;

typedef enum{
    SPI_CPOL_LOW = 0x00,
    SPI_CPOL_HIGH = (1 << 1)
} SPI_CPOL_t;

typedef enum{
    SPI_CPHA_1EDGE = 0x00,
    SPI_CPHA_2EDGE = (1 << 0)
} SPI_CPHA_t;

typedef enum {
    SPI_BAUDRATE_DIV2   = (0 << 3),
    SPI_BAUDRATE_DIV4   = (1 << 3),
    SPI_BAUDRATE_DIV8   = (2 << 3),
    SPI_BAUDRATE_DIV16  = (3 << 3),
    SPI_BAUDRATE_DIV32  = (4 << 3),
    SPI_BAUDRATE_DIV64  = (5 << 3),
    SPI_BAUDRATE_DIV128 = (6 << 3),
    SPI_BAUDRATE_DIV256 = (7 << 3)
} SPI_BaudRate_t;

typedef struct {
    SPI_Mode_t     mode;
    SPI_CPOL_t     cpol;
    SPI_CPHA_t     cpha;
    SPI_BaudRate_t baudrate;
    uint8_t        datasize_16bit; // 0: 8-bit (DFF=0), 1: 16-bit (DFF=1)
    uint8_t        lsb_first;      // 0: MSB First, 1: LSB First
} SPI_Config_t;

SPI_Status_t SPI_Init(SPI_TypeDef *SPIx, const SPI_Config_t *config);

void SPI_DeInit(SPI_TypeDef *SPIx);
void SPI_Enable(SPI_TypeDef *SPIx);
void SPI_Disable(SPI_TypeDef *SPIx);
void SPI_GPIO_Init(SPI_TypeDef *SPIx);

uint8_t SPI_TransmitReceive(SPI_TypeDef *SPIx, uint8_t data);
SPI_Status_t SPI_Transmit(SPI_TypeDef *SPIx, const uint8_t *pData, uint16_t size, uint32_t timeout);
SPI_Status_t SPI_Receive(SPI_TypeDef *SPIx, uint8_t *pData, uint16_t size, uint8_t dummy_byte, uint32_t timeout);
SPI_Status_t SPI_TransmitReceive_Buffer(SPI_TypeDef *SPIx, const uint8_t *pTxData, uint8_t *pRxData, uint16_t size, uint32_t timeout);

void SPI_CS_Init(GPIO_TypeDef *GPIOx, uint8_t pin);
void SPI_CS_Select(GPIO_TypeDef *GPIOx, uint8_t pin);
void SPI_CS_Deselect(GPIO_TypeDef *GPIOx, uint8_t pin);

SPI_Status_t SPI_Transmit_IT(SPI_TypeDef *SPIx, const uint8_t *pData, uint16_t size);
SPI_Status_t SPI_Receive_IT(SPI_TypeDef *SPIx, uint8_t *pData, uint16_t size);
void SPI_IRQHandler(SPI_TypeDef *SPIx); 
SPI_Status_t SPI_Transmit_DMA(SPI_TypeDef *SPIx, const uint8_t *pData, uint16_t size);

#endif /*SPI_H*/