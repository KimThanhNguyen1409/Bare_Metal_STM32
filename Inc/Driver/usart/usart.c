#include "usart.h"
#include "gpio.h"

static USART_RingBuffer_t rx_buffer_1 = {{0}, 0, 0};

USART_Status_t            USART_Init(USART_TypeDef *USARTx, uint32_t APB_Clock, uint32_t baudrate)
{
    if (USARTx == NULL || baudrate == 0 || APB_Clock == 0)
    {
        return USART_ERR_NULL_PTR;
    }

    RCC->APB2ENR |= (1 << 0);

    switch ((uint32_t)USARTx)
    {
        case USART1_PERIPH_BASE:
            RCC->APB2ENR |= (1 << 14);
            RCC->APB2ENR |= (1 << 2);
            GPIO_SetPin(GPIOA, 9, GPIO_MODE_AF_PP_50MHZ);
            GPIO_SetPin(GPIOA, 10, GPIO_MODE_INPUT_FLOATING);
            break;
        case USART2_PERIPH_BASE:
            RCC->APB1ENR |= (1 << 17);
            RCC->APB2ENR |= (1 << 2);
            GPIO_SetPin(GPIOA, 2, GPIO_MODE_AF_PP_50MHZ);
            GPIO_SetPin(GPIOA, 3, GPIO_MODE_INPUT_FLOATING);
            break;
        case USART3_PERIPH_BASE:
            RCC->APB1ENR |= (1 << 18);
            RCC->APB2ENR |= (1 << 3);
            GPIO_SetPin(GPIOB, 10, GPIO_MODE_AF_PP_50MHZ);
            GPIO_SetPin(GPIOB, 11, GPIO_MODE_INPUT_FLOATING);
            break;
        case UART4_PERIPH_BASE:
            RCC->APB1ENR |= (1 << 19);
            RCC->APB2ENR |= (1 << 4);
            GPIO_SetPin(GPIOC, 10, GPIO_MODE_AF_PP_50MHZ);
            GPIO_SetPin(GPIOC, 11, GPIO_MODE_INPUT_FLOATING);
            break;
        case UART5_PERIPH_BASE:
            RCC->APB1ENR |= (1 << 20);
            RCC->APB2ENR |= (1 << 5);
            RCC->APB2ENR |= (1 << 4);
            GPIO_SetPin(GPIOC, 12, GPIO_MODE_AF_PP_50MHZ);
            GPIO_SetPin(GPIOD, 2, GPIO_MODE_INPUT_FLOATING);
            break;
        default:
            break;
    }
    USARTx->BRR = (APB_Clock + (baudrate / 2U)) / baudrate;
    USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE | USART_CR1_UE);

    return USART_OK;
}

USART_Status_t USART_SendChar(USART_TypeDef *USARTx, char ch, uint32_t timeout)
{
    if (USARTx == NULL)
        return USART_ERR_NULL_PTR;
    uint32_t tick = timeout;
    while (!(USARTx->SR & USART_SR_TXE))
    {
        if (timeout > 0 && --tick == 0)
        {
            return USART_ERR_TIMEOUT;
        }
    }
    USARTx->DR = (uint32_t)(ch & 0xFFU);
    return USART_OK;
}

USART_Status_t USART_SendString(USART_TypeDef *USARTx, const char *str, uint32_t timeout)
{
    if (USARTx == NULL || str == NULL)
        return USART_ERR_NULL_PTR;
    while (*str)
    {
        USART_Status_t status = USART_SendChar(USARTx, *str++, timeout);
        if (status != USART_OK)
        {
            return status;
        }
    }
    return USART_OK;
}

USART_Status_t USART_ReceiveChar(USART_TypeDef *USARTx, char *pChar, uint32_t timeout)
{
    if (USARTx == NULL || pChar == NULL)
        return USART_ERR_NULL_PTR;
    uint32_t tick = timeout;

    if (USARTx->SR & USART_SR_ORE)
    {
        (void)USARTx->SR;
        (void)USARTx->DR;
    }
    while (!(USARTx->SR & USART_SR_RXNE))
    {
        if (timeout > 0 && --tick == 0)
        {
            return USART_ERR_TIMEOUT;
        }
    }
    *pChar = (char)(USARTx->DR & 0xFFU);
    return USART_OK;
}

USART_Status_t USART_ReceiveString(USART_TypeDef *USARTx, char *buffer, uint16_t max_length, uint32_t timeout)
{
    if (USARTx == NULL || buffer == NULL)
        return USART_ERR_NULL_PTR;
    if (max_length < 2)
    {
        return USART_ERR_PARAM;
    }

    uint16_t i       = 0;
    char     rx_char = 0;
    while (i < max_length - 1)
    {
        USART_Status_t status = USART_ReceiveChar(USARTx, &rx_char, timeout);
        if (status != USART_OK)
        {
            buffer[i] = '\0';
            return status;
        }
        if (rx_char == '\n')
        {
            continue;
        }
        if (rx_char == '\r')
        {
            break;
        }
        buffer[i++] = rx_char;
    }
    buffer[i] = '\0';
    return USART_OK;
}

void USART_EnableRxInterrupt(USART_TypeDef *USARTx)
{
    if (USARTx != NULL)
    {
        USARTx->CR1 |= USART_CR1_RXNEIE;
    }
}

void USART_IRQHandler(USART_TypeDef *USARTx)
{

    if (USARTx->SR & USART_SR_RXNE)
    {
        uint8_t  data      = (uint8_t)(USARTx->DR & 0xFFU);
        uint16_t next_head = (rx_buffer_1.head + 1U) % USART_RX_BUFFER_SIZE;

        if (next_head != rx_buffer_1.tail)
        {
            rx_buffer_1.buffer[rx_buffer_1.head] = data;
            rx_buffer_1.head                     = next_head;
        }
    }
    if (USARTx->SR & USART_SR_ORE)
    {
        (void)USARTx->SR;
        (void)USARTx->DR;
    }
}

uint16_t USART_Available(void)
{
    if (rx_buffer_1.head >= rx_buffer_1.tail)
    {
        return (rx_buffer_1.head - rx_buffer_1.tail);
    }
    return (USART_RX_BUFFER_SIZE - rx_buffer_1.tail + rx_buffer_1.head);
}

int16_t USART_Read(void)
{
    if (rx_buffer_1.head == rx_buffer_1.tail)
    {
        return -1;
    }
    uint8_t data     = rx_buffer_1.buffer[rx_buffer_1.tail];
    rx_buffer_1.tail = (rx_buffer_1.tail + 1U) % USART_RX_BUFFER_SIZE;
    return (int16_t)data;
}
