#ifndef USART_H
#define USART_H

#include "stm32f103.h"

#define USART_RX_BUFFER_SIZE 128

typedef enum
{
    USART_OK = 0,
    USART_ERR_PARAM,
    USART_ERR_TIMEOUT,
    USART_ERR_NULL_PTR,
    USART_ERR_OVERRUN,
    USART_ERR_BUSY
} USART_Status_t;

#define USART_SR_PE      (1U << 0)  // Parity Error
#define USART_SR_FE      (1U << 1)  // Framing Error
#define USART_SR_NE      (1U << 2)  // Noise Error
#define USART_SR_ORE     (1U << 3)  // Overrun Error
#define USART_SR_IDLE    (1U << 4)  // IDLE line detected
#define USART_SR_RXNE    (1U << 5)  // Read data register not empty
#define USART_SR_TC      (1U << 6)  // Transmission complete
#define USART_SR_TXE     (1U << 7)  // Transmit data register empty
#define USART_CR1_RE     (1U << 2)  // Receiver Enable
#define USART_CR1_TE     (1U << 3)  // Transmitter Enable
#define USART_CR1_RXNEIE (1U << 5)  // RXNE Interrupt Enable
#define USART_CR1_TCIE   (1U << 6)  // Transmission Complete Interrupt Enable
#define USART_CR1_TXEIE  (1U << 7)  // TXE Interrupt Enable
#define USART_CR1_UE     (1U << 13) // USART Enable

typedef struct
{
    uint8_t           buffer[USART_RX_BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
} USART_RingBuffer_t;

USART_Status_t USART_Init(USART_TypeDef *USARTx, uint32_t APB_Clock, uint32_t baudrate);

USART_Status_t USART_SendChar(USART_TypeDef *USARTx, char ch, uint32_t timeout);
USART_Status_t USART_SendString(USART_TypeDef *USARTx, const char *str, uint32_t timeout);
USART_Status_t USART_SendBuffer(USART_TypeDef *USARTx, const uint8_t *pData, uint16_t size, uint32_t timeout);

USART_Status_t USART_ReceiveChar(USART_TypeDef *USARTx, char *pChar, uint32_t timeout);
USART_Status_t USART_ReceiveString(USART_TypeDef *USARTx, char *buffer, uint16_t max_length, uint32_t timeout);

void           USART_EnableRxInterrupt(USART_TypeDef *USARTx);
uint16_t       USART_Available(void);
int16_t        USART_Read(void);
void           USART_IRQHandler(USART_TypeDef *USARTx);
#endif // USART_H