#include "spi.h"
#include "gpio.h"
#include <stddef.h>

// Bitmask thanh ghi SPI theo RM0008 (Section 25)
#define SPI_CR1_SPE      (1U << 6)
#define SPI_CR1_LSBFIRST (1U << 7)
#define SPI_CR1_SSI      (1U << 8)
#define SPI_CR1_SSM      (1U << 9)
#define SPI_CR1_DFF      (1U << 11)

#define SPI_CR2_ERRIE    (1U << 5)
#define SPI_CR2_RXNEIE   (1U << 6)
#define SPI_CR2_TXEIE    (1U << 7)

#define SPI_SR_RXNE      (1U << 0)
#define SPI_SR_TXE       (1U << 1)
#define SPI_SR_MODF      (1U << 5)
#define SPI_SR_OVR       (1U << 6)
#define SPI_SR_BSY       (1U << 7)

typedef struct
{
    const uint8_t    *pTxBuf;
    uint8_t          *pRxBuf;
    volatile uint16_t txCount;
    volatile uint16_t rxCount;
    volatile uint8_t  busy;
} SPI_IT_State_t;

static SPI_IT_State_t spi_state[3] = {0};

static int8_t         SPI_GetIndex(SPI_TypeDef *SPIx)
{
    if (SPIx == SPI1)
        return 0;
    if (SPIx == SPI2)
        return 1;
    if (SPIx == SPI3)
        return 2;
    return -1;
}

static SPI_Status_t SPI_WaitFlag(SPI_TypeDef *SPIx, uint32_t flag, uint8_t state, uint32_t timeout)
{
    uint32_t tick = timeout;
    while (((SPIx->SR & flag) != 0U) != state)
    {
        if (SPIx->SR & SPI_SR_MODF)
        {
            return SPI_ERR_MODF;
        }
        if (timeout > 0U && --tick == 0U)
        {
            return SPI_ERR_TIMEOUT;
        }
    }
    return SPI_OK;
}

static SPI_Status_t SPI_TransferByteTimeout(SPI_TypeDef *SPIx, uint8_t tx_data, uint8_t *rx_data, uint32_t timeout)
{
    SPI_Status_t st = SPI_WaitFlag(SPIx, SPI_SR_TXE, 1U, timeout);
    if (st != SPI_OK)
    {
        return st;
    }

    SPIx->DR = tx_data;

    st       = SPI_WaitFlag(SPIx, SPI_SR_RXNE, 1U, timeout);
    if (st != SPI_OK)
    {
        return st;
    }

    uint8_t received = (uint8_t)(SPIx->DR & 0xFFU);
    if (rx_data != NULL)
    {
        *rx_data = received;
    }
    return SPI_OK;
}

void SPI_GPIO_Init(SPI_TypeDef *SPIx)
{
    if (SPIx == SPI1)
    {
        GPIO_EnableClock(GPIOA);
        GPIO_SetPin(GPIOA, 5, GPIO_MODE_AF_PP_50MHZ);    // SCK
        GPIO_SetPin(GPIOA, 6, GPIO_MODE_INPUT_FLOATING); // MISO
        GPIO_SetPin(GPIOA, 7, GPIO_MODE_AF_PP_50MHZ);    // MOSI
    }
    else if (SPIx == SPI2)
    {
        GPIO_EnableClock(GPIOB);
        GPIO_SetPin(GPIOB, 13, GPIO_MODE_AF_PP_50MHZ);    // SCK
        GPIO_SetPin(GPIOB, 14, GPIO_MODE_INPUT_FLOATING); // MISO
        GPIO_SetPin(GPIOB, 15, GPIO_MODE_AF_PP_50MHZ);    // MOSI
    }
    else if (SPIx == SPI3)
    {
        GPIO_EnableClock(GPIOB);
        GPIO_SetPin(GPIOB, 3, GPIO_MODE_AF_PP_50MHZ);    // SCK (can tat JTAG qua AFIO->MAPR)
        GPIO_SetPin(GPIOB, 4, GPIO_MODE_INPUT_FLOATING); // MISO
        GPIO_SetPin(GPIOB, 5, GPIO_MODE_AF_PP_50MHZ);    // MOSI
    }
}

SPI_Status_t SPI_Init(SPI_TypeDef *SPIx, const SPI_Config_t *config)
{
    if (SPIx == NULL || config == NULL)
    {
        return SPI_ERR_NULL_PTR;
    }

    if (SPIx == SPI1)
    {
        RCC->APB2ENR |= (1U << 12);
    }
    else if (SPIx == SPI2)
    {
        RCC->APB1ENR |= (1U << 14);
    }
    else if (SPIx == SPI3)
    {
        RCC->APB1ENR |= (1U << 15);
    }
    else
    {
        return SPI_ERR_NULL_PTR;
    }

    SPI_GPIO_Init(SPIx);
    SPI_Disable(SPIx);

    uint32_t cr1 =
        (uint32_t)config->mode | (uint32_t)config->cpol | (uint32_t)config->cpha | (uint32_t)config->baudrate;

    // Master mode dung Software CS bat buoc phai set SSM = 1 va SSI = 1 de tranh loi MODF
    if (config->mode == SPI_MODE_MASTER)
    {
        cr1 |= (SPI_CR1_SSM | SPI_CR1_SSI);
    }
    if (config->datasize_16bit)
    {
        cr1 |= SPI_CR1_DFF;
    }
    if (config->lsb_first)
    {
        cr1 |= SPI_CR1_LSBFIRST;
    }

    SPIx->CR1 = cr1;
    SPI_Enable(SPIx);

    return SPI_OK;
}

void SPI_DeInit(SPI_TypeDef *SPIx)
{
    if (SPIx == SPI1)
    {
        RCC->APB2RSTR |= (1U << 12);
        RCC->APB2RSTR &= ~(1U << 12);
    }
    else if (SPIx == SPI2)
    {
        RCC->APB1RSTR |= (1U << 14);
        RCC->APB1RSTR &= ~(1U << 14);
    }
    else if (SPIx == SPI3)
    {
        RCC->APB1RSTR |= (1U << 15);
        RCC->APB1RSTR &= ~(1U << 15);
    }
}

void SPI_Enable(SPI_TypeDef *SPIx)
{
    if (SPIx != NULL)
    {
        SPIx->CR1 |= SPI_CR1_SPE;
    }
}

void SPI_Disable(SPI_TypeDef *SPIx)
{
    if (SPIx != NULL)
    {
        (void)SPI_WaitFlag(SPIx, SPI_SR_BSY, 0U, 100000U);
        SPIx->CR1 &= ~SPI_CR1_SPE;
    }
}

uint8_t SPI_TransmitReceive(SPI_TypeDef *SPIx, uint8_t data)
{
    uint8_t rx = 0xFFU;
    if (SPIx != NULL)
    {
        (void)SPI_TransferByteTimeout(SPIx, data, &rx, 100000U);
    }
    return rx;
}

SPI_Status_t SPI_Transmit(SPI_TypeDef *SPIx, const uint8_t *pData, uint16_t size, uint32_t timeout)
{
    if (SPIx == NULL || pData == NULL)
    {
        return SPI_ERR_NULL_PTR;
    }

    for (uint16_t i = 0; i < size; i++)
    {
        SPI_Status_t st = SPI_TransferByteTimeout(SPIx, pData[i], NULL, timeout);
        if (st != SPI_OK)
        {
            return st;
        }
    }

    return SPI_WaitFlag(SPIx, SPI_SR_BSY, 0U, timeout);
}

SPI_Status_t SPI_Receive(SPI_TypeDef *SPIx, uint8_t *pData, uint16_t size, uint8_t dummy_byte, uint32_t timeout)
{
    if (SPIx == NULL || pData == NULL)
    {
        return SPI_ERR_NULL_PTR;
    }

    for (uint16_t i = 0; i < size; i++)
    {
        SPI_Status_t st = SPI_TransferByteTimeout(SPIx, dummy_byte, &pData[i], timeout);
        if (st != SPI_OK)
        {
            return st;
        }
    }

    return SPI_WaitFlag(SPIx, SPI_SR_BSY, 0U, timeout);
}

SPI_Status_t
SPI_TransmitReceive_Buffer(SPI_TypeDef *SPIx, const uint8_t *pTxData, uint8_t *pRxData, uint16_t size, uint32_t timeout)
{
    if (SPIx == NULL || pTxData == NULL || pRxData == NULL)
    {
        return SPI_ERR_NULL_PTR;
    }

    for (uint16_t i = 0; i < size; i++)
    {
        SPI_Status_t st = SPI_TransferByteTimeout(SPIx, pTxData[i], &pRxData[i], timeout);
        if (st != SPI_OK)
        {
            return st;
        }
    }

    return SPI_WaitFlag(SPIx, SPI_SR_BSY, 0U, timeout);
}

void SPI_CS_Init(GPIO_TypeDef *GPIOx, uint8_t pin)
{
    if (GPIOx == NULL)
    {
        return;
    }
    GPIO_EnableClock(GPIOx);
    GPIO_SetPin(GPIOx, pin, GPIO_MODE_OUTPUT_PULL_50MHZ);
    GPIO_WritePin(GPIOx, pin, 1U);
}

void SPI_CS_Select(GPIO_TypeDef *GPIOx, uint8_t pin)
{
    if (GPIOx != NULL)
    {
        GPIO_WritePin(GPIOx, pin, 0U);
    }
}

void SPI_CS_Deselect(GPIO_TypeDef *GPIOx, uint8_t pin)
{
    if (GPIOx != NULL)
    {
        GPIO_WritePin(GPIOx, pin, 1U);
    }
}

SPI_Status_t SPI_Transmit_IT(SPI_TypeDef *SPIx, const uint8_t *pData, uint16_t size)
{
    int8_t idx = SPI_GetIndex(SPIx);
    if (idx < 0 || pData == NULL)
    {
        return SPI_ERR_NULL_PTR;
    }
    if (spi_state[idx].busy)
    {
        return SPI_ERR_BUSY;
    }
    if (size == 0U)
    {
        return SPI_OK;
    }

    spi_state[idx].pTxBuf  = pData;
    spi_state[idx].pRxBuf  = NULL;
    spi_state[idx].txCount = size;
    spi_state[idx].rxCount = size;
    spi_state[idx].busy    = 1U;

    SPIx->CR2 |= (SPI_CR2_TXEIE | SPI_CR2_RXNEIE | SPI_CR2_ERRIE);
    return SPI_OK;
}

SPI_Status_t SPI_Receive_IT(SPI_TypeDef *SPIx, uint8_t *pData, uint16_t size)
{
    int8_t idx = SPI_GetIndex(SPIx);
    if (idx < 0 || pData == NULL)
    {
        return SPI_ERR_NULL_PTR;
    }
    if (spi_state[idx].busy)
    {
        return SPI_ERR_BUSY;
    }
    if (size == 0U)
    {
        return SPI_OK;
    }

    spi_state[idx].pTxBuf  = NULL;
    spi_state[idx].pRxBuf  = pData;
    spi_state[idx].txCount = size;
    spi_state[idx].rxCount = size;
    spi_state[idx].busy    = 1U;

    SPIx->CR2 |= (SPI_CR2_TXEIE | SPI_CR2_RXNEIE | SPI_CR2_ERRIE);
    return SPI_OK;
}

void SPI_IRQHandler(SPI_TypeDef *SPIx)
{
    int8_t idx = SPI_GetIndex(SPIx);
    if (idx < 0)
    {
        return;
    }

    SPI_IT_State_t *st = &spi_state[idx];

    if ((SPIx->SR & SPI_SR_TXE) && (SPIx->CR2 & SPI_CR2_TXEIE))
    {
        if (st->txCount > 0U)
        {
            SPIx->DR = (st->pTxBuf != NULL) ? *st->pTxBuf++ : 0xFFU;
            st->txCount--;
        }
        if (st->txCount == 0U)
        {
            SPIx->CR2 &= ~SPI_CR2_TXEIE;
        }
    }

    if ((SPIx->SR & SPI_SR_RXNE) && (SPIx->CR2 & SPI_CR2_RXNEIE))
    {
        uint8_t rx = (uint8_t)(SPIx->DR & 0xFFU);
        if (st->rxCount > 0U)
        {
            if (st->pRxBuf != NULL)
            {
                *st->pRxBuf++ = rx;
            }
            st->rxCount--;
        }
        if (st->rxCount == 0U)
        {
            SPIx->CR2 &= ~(SPI_CR2_RXNEIE | SPI_CR2_ERRIE);
            st->busy = 0U;
        }
    }

    if (SPIx->SR & SPI_SR_OVR)
    {
        (void)SPIx->DR;
        (void)SPIx->SR;
        SPIx->CR2 &= ~(SPI_CR2_TXEIE | SPI_CR2_RXNEIE | SPI_CR2_ERRIE);
        st->busy = 0U;
    }
}

SPI_Status_t SPI_Transmit_DMA(SPI_TypeDef *SPIx, const uint8_t *pData, uint16_t size)
{
    (void)SPIx;
    (void)pData;
    (void)size;
    // Chua co dinh nghia DMA1 trong stm32f103.h
    return SPI_ERR_BUSY;
}
