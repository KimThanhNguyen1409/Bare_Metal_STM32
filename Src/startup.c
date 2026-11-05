#include <stdint.h>

extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

extern int main(void);

void Reset_Handler(void);
void Default_Handler(void);

void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void Hardfault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVCall_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void Systick_Handler(void) __attribute__((weak, alias("Default_Handler")));
// RAM
void WWDG_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void PVD_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TAMPER_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void RTC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void FLASH_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void RCC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI4_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel4_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel5_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel6_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel7_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void ADC1_2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USB_HP_CAN1_TX_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USB_LP_CAN1_RX0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void CAN1_RX1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void CAN1_SCE_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI9_5_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_BRK_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_UP_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_TRG_COM_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM4_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void I2C1_EV_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void I2C1_ER_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void I2C2_EV_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void I2C2_ER_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void SPI1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void SPI2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USART1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USART2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USART3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI15_10_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void RTC_Alarm_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USBWakeUp_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
uint32_t* isr_vector[] __attribute__((section(".isr_vector"))) = {
    (uint32_t*)&_estack,            // intial SP value
    (uint32_t*)&Reset_Handler,      // Reset
    (uint32_t*)&NMI_Handler,        // NMI
    (uint32_t*)&Hardfault_Handler,  // Hard fault
    (uint32_t*)&MemManage_Handler,  // Memory
    (uint32_t*)&BusFault_Handler,   // Bus
    (uint32_t*)&UsageFault_Handler, // Usage
    0,
    0,
    0,
    0,
    (uint32_t*)&SVCall_Handler, // SVCall
    0,
    0,
    (uint32_t*)&PendSV_Handler,  // PendSV
    (uint32_t*)&Systick_Handler, // Systicks
    // RAM
    (uint32_t*)&WWDG_IRQHandler,
    (uint32_t*)&PVD_IRQHandler,
    (uint32_t*)&TAMPER_IRQHandler,
    (uint32_t*)&RTC_IRQHandler,
    (uint32_t*)&FLASH_IRQHandler,
    (uint32_t*)&RCC_IRQHandler,
    (uint32_t*)&EXTI0_IRQHandler,
    (uint32_t*)&EXTI1_IRQHandler,
    (uint32_t*)&EXTI2_IRQHandler,
    (uint32_t*)&EXTI3_IRQHandler,
    (uint32_t*)&EXTI4_IRQHandler,
    (uint32_t*)&DMA1_Channel1_IRQHandler,
    (uint32_t*)&DMA1_Channel2_IRQHandler,
    (uint32_t*)&DMA1_Channel3_IRQHandler,
    (uint32_t*)&DMA1_Channel4_IRQHandler,
    (uint32_t*)&DMA1_Channel5_IRQHandler,
    (uint32_t*)&DMA1_Channel6_IRQHandler,
    (uint32_t*)&DMA1_Channel7_IRQHandler,
    (uint32_t*)&ADC1_2_IRQHandler,
    (uint32_t*)&USB_HP_CAN1_TX_IRQHandler,
    (uint32_t*)&USB_LP_CAN1_RX0_IRQHandler,
    (uint32_t*)&CAN1_RX1_IRQHandler,
    (uint32_t*)&CAN1_SCE_IRQHandler,
    (uint32_t*)&EXTI9_5_IRQHandler,
    (uint32_t*)&TIM1_BRK_IRQHandler,
    (uint32_t*)&TIM1_UP_IRQHandler,
    (uint32_t*)&TIM1_TRG_COM_IRQHandler,
    (uint32_t*)&TIM1_CC_IRQHandler,
    (uint32_t*)&TIM2_IRQHandler,
    (uint32_t*)&TIM3_IRQHandler,
    (uint32_t*)&TIM4_IRQHandler,
    (uint32_t*)&I2C1_EV_IRQHandler,
    (uint32_t*)&I2C1_ER_IRQHandler,
    (uint32_t*)&I2C2_EV_IRQHandler,
    (uint32_t*)&I2C2_ER_IRQHandler,
    (uint32_t*)&SPI1_IRQHandler,
    (uint32_t*)&SPI2_IRQHandler,
    (uint32_t*)&USART1_IRQHandler,
    (uint32_t*)&USART2_IRQHandler,
    (uint32_t*)&USART3_IRQHandler,
    (uint32_t*)&EXTI15_10_IRQHandler,
    (uint32_t*)&RTC_Alarm_IRQHandler,
    (uint32_t*)&USBWakeUp_IRQHandler,
};
void Default_Handler(void)
{
    while (1)
        ;
}
void Reset_Handler(void)
{
    uint32_t* pSrc  = &_sidata; // Ghi tren Flash
    uint32_t* pDest = &_sdata;  // Ghi tren RAM
    while (pDest < &_edata)     // toi vung ket thuc data
    {
        *pDest++ = *pSrc++; /*Lay gia o vung Flash nhet vao RAM,
                            nhet xong roi nhich ca 2 con tro*/
    }
    pDest = &_sbss;
    while (pDest < &_ebss) // toi vung ket thuc cua bss
    {
        *pDest++ = 0; /*con trỏ pDest trỏ vào đầu vùng (_sbss) rồi điền một rọc số 0
                       cho đến khi chạm đít (_ebss) là hoàn thành chỉ tiêu*/
    }
    main();
    while (1)
        ;
}