// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

// Startup code
__attribute__((naked, noreturn)) void _reset(void) {
  // Init stack
  asm("ldr sp, = _estack");

  // Initialise memory
  extern long _sbss, _ebss, _sdata, _edata, _sidata;
  for (long *src = &_sbss; src < &_ebss; src++) *src = 0;
  for (long *src = &_sdata, *dst = &_sidata; src < &_edata;) *src++ = *dst++;

  // Call main()
  extern void main(void);
  main();
  for (;;) (void) 0;
}

void __attribute__((weak)) DefaultIRQHandler(void) {
  for (;;) (void) 0;
}

void __attribute__((weak)) EXTI_IRQHandler(void) {
  for (;;) (void) 0;
}

#define WEAK_ALIAS __attribute__((weak, alias("DefaultIRQHandler")))

__attribute__((alias("EXTI_IRQHandler"))) void EXTI0_IRQHandler(void);
__attribute__((alias("EXTI_IRQHandler"))) void EXTI1_IRQHandler(void);
__attribute__((alias("EXTI_IRQHandler"))) void EXTI2_IRQHandler(void);
__attribute__((alias("EXTI_IRQHandler"))) void EXTI3_IRQHandler(void);
__attribute__((alias("EXTI_IRQHandler"))) void EXTI4_IRQHandler(void);
__attribute__((alias("EXTI_IRQHandler"))) void EXTI9_5_IRQHandler(void);
__attribute__((alias("EXTI_IRQHandler"))) void EXTI15_10_IRQHandler(void);

WEAK_ALIAS void NMI_Handler(void);
WEAK_ALIAS void HardFault_Handler(void);
WEAK_ALIAS void MemManage_Handler(void);
WEAK_ALIAS void BusFault_Handler(void);
WEAK_ALIAS void UsageFault_Handler(void);
WEAK_ALIAS void SVC_Handler(void);
WEAK_ALIAS void DebugMon_Handler(void);
WEAK_ALIAS void PendSV_Handler(void);
WEAK_ALIAS void SysTick_Handler(void);

WEAK_ALIAS void WWDG_IRQHandler(void);
WEAK_ALIAS void PVD_IRQHandler(void);
WEAK_ALIAS void TAMP_STAMP_IRQHandler(void);
WEAK_ALIAS void RTC_WKUP_IRQHandler(void);
WEAK_ALIAS void FLASH_IRQHandler(void);
WEAK_ALIAS void RCC_IRQHandler(void);
WEAK_ALIAS void DMA1_Stream0_IRQHandler(void);
WEAK_ALIAS void DMA1_Stream1_IRQHandler(void);
WEAK_ALIAS void DMA1_Stream2_IRQHandler(void);
WEAK_ALIAS void DMA1_Stream3_IRQHandler(void);
WEAK_ALIAS void DMA1_Stream4_IRQHandler(void);
WEAK_ALIAS void DMA1_Stream5_IRQHandler(void);
WEAK_ALIAS void DMA1_Stream6_IRQHandler(void);
WEAK_ALIAS void ADC_IRQHandler(void);
WEAK_ALIAS void CAN1_TX_IRQHandler(void);
WEAK_ALIAS void CAN1_RX0_IRQHandler(void);
WEAK_ALIAS void CAN1_RX1_IRQHandler(void);
WEAK_ALIAS void CAN1_SCE_IRQHandler(void);
WEAK_ALIAS void TIM1_BRK_TIM9_IRQHandler(void);
WEAK_ALIAS void TIM1_UP_TIM10_IRQHandler(void);
WEAK_ALIAS void TIM1_TRG_COM_TIM11_IRQHandler(void);
WEAK_ALIAS void TIM1_CC_IRQHandler(void);
WEAK_ALIAS void TIM2_IRQHandler(void);
WEAK_ALIAS void TIM3_IRQHandler(void);
WEAK_ALIAS void TIM4_IRQHandler(void);
WEAK_ALIAS void I2C1_EV_IRQHandler(void);
WEAK_ALIAS void I2C1_ER_IRQHandler(void);
WEAK_ALIAS void I2C2_EV_IRQHandler(void);
WEAK_ALIAS void I2C2_ER_IRQHandler(void);
WEAK_ALIAS void SPI1_IRQHandler(void);
WEAK_ALIAS void SPI2_IRQHandler(void);
WEAK_ALIAS void USART1_IRQHandler(void);
WEAK_ALIAS void USART2_IRQHandler(void);
WEAK_ALIAS void USART3_IRQHandler(void);
WEAK_ALIAS void RTC_Alarm_IRQHandler(void);
WEAK_ALIAS void OTG_FS_WKUP_IRQHandler(void);
WEAK_ALIAS void TIM8_BRK_TIM12_IRQHandler(void);
WEAK_ALIAS void TIM8_UP_TIM13_IRQHandler(void);
WEAK_ALIAS void TIM8_TRG_COM_TIM14_IRQHandler(void);
WEAK_ALIAS void TIM8_CC_IRQHandler(void);
WEAK_ALIAS void DMA1_Stream7_IRQHandler(void);
WEAK_ALIAS void FMC_IRQHandler(void);
WEAK_ALIAS void SDMMC1_IRQHandler(void);
WEAK_ALIAS void TIM5_IRQHandler(void);
WEAK_ALIAS void SPI3_IRQHandler(void);
WEAK_ALIAS void UART4_IRQHandler(void);
WEAK_ALIAS void UART5_IRQHandler(void);
WEAK_ALIAS void TIM6_DAC_IRQHandler(void);
WEAK_ALIAS void TIM7_IRQHandler(void);
WEAK_ALIAS void DMA2_Stream0_IRQHandler(void);
WEAK_ALIAS void DMA2_Stream1_IRQHandler(void);
WEAK_ALIAS void DMA2_Stream2_IRQHandler(void);
WEAK_ALIAS void DMA2_Stream3_IRQHandler(void);
WEAK_ALIAS void DMA2_Stream4_IRQHandler(void);
WEAK_ALIAS void ETH_IRQHandler(void);
WEAK_ALIAS void ETH_WKUP_IRQHandler(void);
WEAK_ALIAS void CAN2_TX_IRQHandler(void);
WEAK_ALIAS void CAN2_RX0_IRQHandler(void);
WEAK_ALIAS void CAN2_RX1_IRQHandler(void);
WEAK_ALIAS void CAN2_SCE_IRQHandler(void);
WEAK_ALIAS void OTG_FS_IRQHandler(void);
WEAK_ALIAS void DMA2_Stream5_IRQHandler(void);
WEAK_ALIAS void DMA2_Stream6_IRQHandler(void);
WEAK_ALIAS void DMA2_Stream7_IRQHandler(void);
WEAK_ALIAS void USART6_IRQHandler(void);
WEAK_ALIAS void I2C3_EV_IRQHandler(void);
WEAK_ALIAS void I2C3_ER_IRQHandler(void);
WEAK_ALIAS void OTG_HS_EP1_OUT_IRQHandler(void);
WEAK_ALIAS void OTG_HS_EP1_IN_IRQHandler(void);
WEAK_ALIAS void OTG_HS_WKUP_IRQHandler(void);
WEAK_ALIAS void OTG_HS_IRQHandler(void);
WEAK_ALIAS void DCMI_IRQHandler(void);
WEAK_ALIAS void RNG_IRQHandler(void);
WEAK_ALIAS void FPU_IRQHandler(void);
WEAK_ALIAS void UART7_IRQHandler(void);
WEAK_ALIAS void UART8_IRQHandler(void);
WEAK_ALIAS void SPI4_IRQHandler(void);
WEAK_ALIAS void SPI5_IRQHandler(void);
WEAK_ALIAS void SPI6_IRQHandler(void);
WEAK_ALIAS void SAI1_IRQHandler(void);
WEAK_ALIAS void LTDC_IRQHandler(void);
WEAK_ALIAS void LTDC_ER_IRQHandler(void);
WEAK_ALIAS void DMA2D_IRQHandler(void);
WEAK_ALIAS void SAI2_IRQHandler(void);
WEAK_ALIAS void QUADSPI_IRQHandler(void);
WEAK_ALIAS void LPTIM1_IRQHandler(void);
WEAK_ALIAS void CEC_IRQHandler(void);
WEAK_ALIAS void I2C4_EV_IRQHandler(void);
WEAK_ALIAS void I2C4_ER_IRQHandler(void);
WEAK_ALIAS void SPDIF_RX_IRQHandler(void);

// IRQ table
__attribute__((section(".vectors"))) void (*tab[16 + 98])(void) = {
    // Cortex interrupts
    0, _reset, NMI_Handler, HardFault_Handler, MemManage_Handler,
    BusFault_Handler, UsageFault_Handler, 0, 0, 0, 0, SVC_Handler,
    DebugMon_Handler, 0, PendSV_Handler, SysTick_Handler,

    // Interrupts from peripherals
    WWDG_IRQHandler, PVD_IRQHandler, TAMP_STAMP_IRQHandler, RTC_WKUP_IRQHandler,
    FLASH_IRQHandler, RCC_IRQHandler, EXTI0_IRQHandler, EXTI1_IRQHandler,
    EXTI2_IRQHandler, EXTI3_IRQHandler, EXTI4_IRQHandler,
    DMA1_Stream0_IRQHandler, DMA1_Stream1_IRQHandler, DMA1_Stream2_IRQHandler,
    DMA1_Stream3_IRQHandler, DMA1_Stream4_IRQHandler, DMA1_Stream5_IRQHandler,
    DMA1_Stream6_IRQHandler, ADC_IRQHandler, CAN1_TX_IRQHandler,
    CAN1_RX0_IRQHandler, CAN1_RX1_IRQHandler, CAN1_SCE_IRQHandler,
    EXTI9_5_IRQHandler, TIM1_BRK_TIM9_IRQHandler, TIM1_UP_TIM10_IRQHandler,
    TIM1_TRG_COM_TIM11_IRQHandler, TIM1_CC_IRQHandler, TIM2_IRQHandler,
    TIM3_IRQHandler, TIM4_IRQHandler, I2C1_EV_IRQHandler, I2C1_ER_IRQHandler,
    I2C2_EV_IRQHandler, I2C2_ER_IRQHandler, SPI1_IRQHandler, SPI2_IRQHandler,
    USART1_IRQHandler, USART2_IRQHandler, USART3_IRQHandler,
    EXTI15_10_IRQHandler, RTC_Alarm_IRQHandler, OTG_FS_WKUP_IRQHandler,
    TIM8_BRK_TIM12_IRQHandler, TIM8_UP_TIM13_IRQHandler,
    TIM8_TRG_COM_TIM14_IRQHandler, TIM8_CC_IRQHandler, DMA1_Stream7_IRQHandler,
    FMC_IRQHandler, SDMMC1_IRQHandler, TIM5_IRQHandler, SPI3_IRQHandler,
    UART4_IRQHandler, UART5_IRQHandler, TIM6_DAC_IRQHandler, TIM7_IRQHandler,
    DMA2_Stream0_IRQHandler, DMA2_Stream1_IRQHandler, DMA2_Stream2_IRQHandler,
    DMA2_Stream3_IRQHandler, DMA2_Stream4_IRQHandler, ETH_IRQHandler,
    ETH_WKUP_IRQHandler, CAN2_TX_IRQHandler, CAN2_RX0_IRQHandler,
    CAN2_RX1_IRQHandler, CAN2_SCE_IRQHandler, OTG_FS_IRQHandler,
    DMA2_Stream5_IRQHandler, DMA2_Stream6_IRQHandler, DMA2_Stream7_IRQHandler,
    USART6_IRQHandler, I2C3_EV_IRQHandler, I2C3_ER_IRQHandler,
    OTG_HS_EP1_OUT_IRQHandler, OTG_HS_EP1_IN_IRQHandler, OTG_HS_WKUP_IRQHandler,
    OTG_HS_IRQHandler, DCMI_IRQHandler, 0, RNG_IRQHandler, FPU_IRQHandler,
    UART7_IRQHandler, UART8_IRQHandler, SPI4_IRQHandler, SPI5_IRQHandler,
    SPI6_IRQHandler, SAI1_IRQHandler, LTDC_IRQHandler, LTDC_ER_IRQHandler,
    DMA2D_IRQHandler, SAI2_IRQHandler, QUADSPI_IRQHandler, LPTIM1_IRQHandler,
    CEC_IRQHandler, I2C4_EV_IRQHandler, I2C4_ER_IRQHandler,
    SPDIF_RX_IRQHandler};
