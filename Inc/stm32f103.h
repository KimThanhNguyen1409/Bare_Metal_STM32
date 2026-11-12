#ifndef STM32F103_H
#define STM32F103_H
#include <stdint.h>
#include <stddef.h>
// Original
#define PERIPH_BASE        0x40000000UL
#define APB2PERIPH_BASE    (PERIPH_BASE + 0x10000UL)
#define APB1PERIPH_BASE    (PERIPH_BASE)
#define AHBPERIPH_BASE     (PERIPH_BASE + 0x20000UL)

#define RCC_PERIPH_BASE    (AHBPERIPH_BASE + 0x1000UL)

// GPIO's address
#define GPIOG_PERIPH_BASE  (APB2PERIPH_BASE + 0x2000UL)
#define GPIOF_PERIPH_BASE  (APB2PERIPH_BASE + 0x1C00UL)
#define GPIOE_PERIPH_BASE  (APB2PERIPH_BASE + 0x1800UL)
#define GPIOD_PERIPH_BASE  (APB2PERIPH_BASE + 0x1400UL)
#define GPIOC_PERIPH_BASE  (APB2PERIPH_BASE + 0x1000UL)
#define GPIOB_PERIPH_BASE  (APB2PERIPH_BASE + 0x0C00UL)
#define GPIOA_PERIPH_BASE  (APB2PERIPH_BASE + 0x0800UL)

// USART's address
#define USART1_PERIPH_BASE (APB2PERIPH_BASE + 0x3800UL)
#define USART2_PERIPH_BASE (APB1PERIPH_BASE + 0x4400UL)
#define USART3_PERIPH_BASE (APB1PERIPH_BASE + 0x4800UL)
#define UART4_PERIPH_BASE  (APB1PERIPH_BASE + 0x4C00UL)
#define UART5_PERIPH_BASE  (APB1PERIPH_BASE + 0x5000UL)

// TIMER's address
#define TIMER1_PERIPH_BASE (APB2PERIPH_BASE + 0x2C00UL)
#define TIMER2_PERIPH_BASE (APB1PERIPH_BASE + 0x0000UL)
#define TIMER3_PERIPH_BASE (APB1PERIPH_BASE + 0x0400UL)

// I2C's address
#define I2C1_PERIPH_BASE   (APB1PERIPH_BASE + 0x5400UL)
#define I2C2_PERIPH_BASE   (APB1PERIPH_BASE + 0x5800UL)

// SPI's address
#define SPI1_PERIPH_BASE   (APB2PERIPH_BASE + 0x3000UL)
#define SPI2_PERIPH_BASE   (APB1PERIPH_BASE + 0x3800UL)
#define SPI3_PERIPH_BASE   (APB1PERIPH_BASE + 0x3C00UL)

#define SYSTICK_BASE       0xE000E010UL

#define NVIC_PERIPH_BASE   0xE000E100UL

// AFIO's address
#define AFIO_PERIPH_BASE   (APB2PERIPH_BASE)

// EXTI's address
#define EXTI_PERIPH_BASE   (APB2PERIPH_BASE + 0x0400UL)

// Watch Dog's address
#define IWDG_PERIPH_BASE   (APB1PERIPH_BASE + 0x3000UL)
#define WWDG_PERIPH_BASE   (APB1PERIPH_BASE + 0x2C00UL)

// CoreDebug's address
#define DWT_PERIPH_BASE    0xE0001000UL
#define COREDEBUG_BASE     0xE000EDF0UL

typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
    volatile uint32_t AHBRSTR;
    volatile uint32_t CFGR2;
} RCC_TypeDef;

typedef struct
{
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_TypeDef;

typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_TypeDef;

typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    uint32_t          re0;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    uint32_t          re1;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
} TIMER_TypeDef;

typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t OAR1;
    volatile uint32_t OAR2;
    volatile uint32_t DR;
    volatile uint32_t SR1;
    volatile uint32_t SR2;
    volatile uint32_t CCR;
    volatile uint32_t TRISE;
} I2C_TypeDef;

typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t CRCPR;
    volatile uint32_t RXCRCR;
    volatile uint32_t TXCRCR;
    volatile uint32_t I2SCFGR;
    volatile uint32_t I2SPR;
} SPI_TypeDef;

typedef struct
{
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;
} Systick_TypeDef;

typedef struct
{
    volatile uint32_t EVCR;
    volatile uint32_t MAPR;
    volatile uint32_t EXTICR[4];
    uint32_t          RESERVED0;
    volatile uint32_t MAPR2;
} AFIO_TypeDef;

typedef struct
{
    volatile uint32_t IMR;
    volatile uint32_t EMR;
    volatile uint32_t RTSR;
    volatile uint32_t FTSR;
    volatile uint32_t SWIER;
    volatile uint32_t PR;
} EXTI_TypeDef;

typedef struct
{
    volatile uint32_t CTRL;
    volatile uint32_t CYCCNT;
} DWT_TypeDef;

typedef struct
{
    volatile uint32_t DHCSR;
    volatile uint32_t DCRSR;
    volatile uint32_t DCRDR;
    volatile uint32_t DEMCR;
} CoreDebug_TypeDef;

typedef struct
{
    volatile uint32_t ISER[8];
    uint32_t          RESERVED0[24];
    volatile uint32_t ICER[8];
    uint32_t          RESERVED1[24];
    volatile uint32_t ISPR[8];
    uint32_t          RESERVED2[24];
    volatile uint32_t ICPR[8];
    uint32_t          RESERVED3[24];
    volatile uint32_t IABR[8];
    uint32_t          RESERVED4[56];
    volatile uint8_t  IP[240];
} NVIC_Typedef;

typedef enum
{
    EXTI0_IRQn     = 6,
    EXTI1_IRQn     = 7,
    EXTI2_IRQn     = 8,
    EXTI3_IRQn     = 9,
    EXTI4_IRQn     = 10,
    EXTI9_5_IRQn   = 23,
    TIM1_UP_IRQn   = 25,
    TIM2_IRQn      = 28,
    TIM3_IRQn      = 29,
    TIM4_IRQn      = 30,
    I2C1_EV_IRQn   = 31,
    I2C1_ER_IRQn   = 32,
    I2C2_EV_IRQn   = 33,
    I2C2_ER_IRQn   = 34,
    SPI1_IRQn      = 35,
    SPI2_IRQn      = 36,
    USART1_IRQn    = 37,
    USART2_IRQn    = 38,
    USART3_IRQn    = 39,
    EXTI15_10_IRQn = 40
} IRQn_Type;

#define RCC       ((RCC_TypeDef *)RCC_PERIPH_BASE)

#define GPIOA     ((GPIO_TypeDef *)GPIOA_PERIPH_BASE)
#define GPIOB     ((GPIO_TypeDef *)GPIOB_PERIPH_BASE)
#define GPIOC     ((GPIO_TypeDef *)GPIOC_PERIPH_BASE)
#define GPIOD     ((GPIO_TypeDef *)GPIOD_PERIPH_BASE)
#define GPIOE     ((GPIO_TypeDef *)GPIOE_PERIPH_BASE)
#define GPIOF     ((GPIO_TypeDef *)GPIOF_PERIPH_BASE)
#define GPIOG     ((GPIO_TypeDef *)GPIOG_PERIPH_BASE)

#define USART1    ((USART_TypeDef *)USART1_PERIPH_BASE)
#define USART2    ((USART_TypeDef *)USART2_PERIPH_BASE)
#define USART3    ((USART_TypeDef *)USART3_PERIPH_BASE)
#define UART4     ((USART_TypeDef *)UART4_PERIPH_BASE)
#define UART5     ((USART_TypeDef *)UART5_PERIPH_BASE)

#define TIM1      ((TIMER_TypeDef *)TIMER1_PERIPH_BASE)
#define TIM2      ((TIMER_TypeDef *)TIMER2_PERIPH_BASE)
#define TIM3      ((TIMER_TypeDef *)TIMER3_PERIPH_BASE)

#define I2C1      ((I2C_TypeDef *)I2C1_PERIPH_BASE)
#define I2C2      ((I2C_TypeDef *)I2C2_PERIPH_BASE)

#define SPI1      ((SPI_TypeDef *)SPI1_PERIPH_BASE)
#define SPI2      ((SPI_TypeDef *)SPI2_PERIPH_BASE)
#define SPI3      ((SPI_TypeDef *)SPI3_PERIPH_BASE)

#define SYSTICK   ((Systick_TypeDef *)SYSTICK_BASE)
#define NVIC      ((NVIC_Typedef *)NVIC_PERIPH_BASE)

#define AFIO      ((AFIO_TypeDef *)AFIO_PERIPH_BASE)

#define EXTI      ((EXTI_TypeDef *)EXTI_PERIPH_BASE)

#define DWT       ((DWT_TypeDef *)DWT_PERIPH_BASE)

#define COREDEBUG ((CoreDebug_TypeDef *)COREDEBUG_BASE)

static inline void NVIC_EnableIRQ(IRQn_Type IRQn)
{
    NVIC->ISER[((uint32_t)IRQn) >> 5] = (1UL << (((uint32_t)IRQn) & 0x1FUL));
}
static inline void NVIC_DisableIRQ(IRQn_Type IRQn)
{
    NVIC->ICER[((uint32_t)IRQn) >> 5] = (1UL << (((uint32_t)IRQn) & 0x1FUL));
}

#endif