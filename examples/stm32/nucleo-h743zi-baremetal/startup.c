// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

// Startup code
__attribute__((naked, noreturn)) void _reset(void) {
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

#define WEAK_ALIAS __attribute__((weak, alias("DefaultIRQHandler")))

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
WEAK_ALIAS void PVD_AVD_IRQHandler(void);
WEAK_ALIAS void TAMP_STAMP_IRQHandler(void);
WEAK_ALIAS void RTC_WKUP_IRQHandler(void);
WEAK_ALIAS void FLASH_IRQHandler(void);
WEAK_ALIAS void RCC_IRQHandler(void);
WEAK_ALIAS void EXTI0_IRQHandler(void);
WEAK_ALIAS void EXTI1_IRQHandler(void);
WEAK_ALIAS void EXTI2_IRQHandler(void);
WEAK_ALIAS void EXTI3_IRQHandler(void);
WEAK_ALIAS void EXTI4_IRQHandler(void);
WEAK_ALIAS void DMA1_Stream0_IRQHandler(void);
WEAK_ALIAS void DMA1_Stream1_IRQHandler(void);
WEAK_ALIAS void DMA1_Stream2_IRQHandler(void);
WEAK_ALIAS void DMA1_Stream3_IRQHandler(void);
WEAK_ALIAS void DMA1_Stream4_IRQHandler(void);
WEAK_ALIAS void DMA1_Stream5_IRQHandler(void);
WEAK_ALIAS void DMA1_Stream6_IRQHandler(void);
WEAK_ALIAS void ADC_IRQHandler(void);
WEAK_ALIAS void FDCAN1_IT0_IRQHandler(void);
WEAK_ALIAS void FDCAN2_IT0_IRQHandler(void);
WEAK_ALIAS void FDCAN1_IT1_IRQHandler(void);
WEAK_ALIAS void FDCAN2_IT1_IRQHandler(void);
WEAK_ALIAS void EXTI9_5_IRQHandler(void);
WEAK_ALIAS void TIM1_BRK_IRQHandler(void);
WEAK_ALIAS void TIM1_UP_IRQHandler(void);
WEAK_ALIAS void TIM1_TRG_COM_IRQHandler(void);
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
WEAK_ALIAS void EXTI15_10_IRQHandler(void);
WEAK_ALIAS void RTC_Alarm_IRQHandler(void);
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
WEAK_ALIAS void FDCAN_CAL_IRQHandler(void);
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
WEAK_ALIAS void OTG_FS_EP1_OUT_IRQHandler(void);
WEAK_ALIAS void OTG_FS_EP1_IN_IRQHandler(void);
WEAK_ALIAS void OTG_FS_WKUP_IRQHandler(void);
WEAK_ALIAS void OTG_FS_IRQHandler(void);
WEAK_ALIAS void DMAMUX1_OVR_IRQHandler(void);
WEAK_ALIAS void HRTIM1_Master_IRQHandler(void);
WEAK_ALIAS void HRTIM1_TIMA_IRQHandler(void);
WEAK_ALIAS void HRTIM1_TIMB_IRQHandler(void);
WEAK_ALIAS void HRTIM1_TIMC_IRQHandler(void);
WEAK_ALIAS void HRTIM1_TIMD_IRQHandler(void);
WEAK_ALIAS void HRTIM1_TIME_IRQHandler(void);
WEAK_ALIAS void HRTIM1_FLT_IRQHandler(void);
WEAK_ALIAS void DFSDM1_FLT0_IRQHandler(void);
WEAK_ALIAS void DFSDM1_FLT1_IRQHandler(void);
WEAK_ALIAS void DFSDM1_FLT2_IRQHandler(void);
WEAK_ALIAS void DFSDM1_FLT3_IRQHandler(void);
WEAK_ALIAS void SAI3_IRQHandler(void);
WEAK_ALIAS void SWPMI1_IRQHandler(void);
WEAK_ALIAS void TIM15_IRQHandler(void);
WEAK_ALIAS void TIM16_IRQHandler(void);
WEAK_ALIAS void TIM17_IRQHandler(void);
WEAK_ALIAS void MDIOS_WKUP_IRQHandler(void);
WEAK_ALIAS void MDIOS_IRQHandler(void);
WEAK_ALIAS void JPEG_IRQHandler(void);
WEAK_ALIAS void MDMA_IRQHandler(void);
WEAK_ALIAS void SDMMC2_IRQHandler(void);
WEAK_ALIAS void HSEM1_IRQHandler(void);
WEAK_ALIAS void ADC3_IRQHandler(void);
WEAK_ALIAS void DMAMUX2_OVR_IRQHandler(void);
WEAK_ALIAS void BDMA_Channel0_IRQHandler(void);
WEAK_ALIAS void BDMA_Channel1_IRQHandler(void);
WEAK_ALIAS void BDMA_Channel2_IRQHandler(void);
WEAK_ALIAS void BDMA_Channel3_IRQHandler(void);
WEAK_ALIAS void BDMA_Channel4_IRQHandler(void);
WEAK_ALIAS void BDMA_Channel5_IRQHandler(void);
WEAK_ALIAS void BDMA_Channel6_IRQHandler(void);
WEAK_ALIAS void BDMA_Channel7_IRQHandler(void);
WEAK_ALIAS void COMP1_IRQHandler(void);
WEAK_ALIAS void LPTIM2_IRQHandler(void);
WEAK_ALIAS void LPTIM3_IRQHandler(void);
WEAK_ALIAS void LPTIM4_IRQHandler(void);
WEAK_ALIAS void LPTIM5_IRQHandler(void);
WEAK_ALIAS void LPUART1_IRQHandler(void);
WEAK_ALIAS void CRS_IRQHandler(void);
WEAK_ALIAS void ECC_IRQHandler(void);
WEAK_ALIAS void SAI4_IRQHandler(void);
WEAK_ALIAS void WAKEUP_PIN_IRQHandler(void);

// IRQ table
extern void _estack();
__attribute__((section(".vectors"))) void (*tab[16 + 150])(void) = {
    // Cortex interrupts
    _estack, _reset, NMI_Handler, HardFault_Handler, MemManage_Handler,
    BusFault_Handler, UsageFault_Handler, 0, 0, 0, 0, SVC_Handler,
    DebugMon_Handler, 0, PendSV_Handler, SysTick_Handler,

    // Interrupts from peripherals
    WWDG_IRQHandler, PVD_AVD_IRQHandler, TAMP_STAMP_IRQHandler,
    RTC_WKUP_IRQHandler, FLASH_IRQHandler, RCC_IRQHandler, EXTI0_IRQHandler,
    EXTI1_IRQHandler, EXTI2_IRQHandler, EXTI3_IRQHandler, EXTI4_IRQHandler,
    DMA1_Stream0_IRQHandler, DMA1_Stream1_IRQHandler, DMA1_Stream2_IRQHandler,
    DMA1_Stream3_IRQHandler, DMA1_Stream4_IRQHandler, DMA1_Stream5_IRQHandler,
    DMA1_Stream6_IRQHandler, ADC_IRQHandler, FDCAN1_IT0_IRQHandler,
    FDCAN2_IT0_IRQHandler, FDCAN1_IT1_IRQHandler, FDCAN2_IT1_IRQHandler,
    EXTI9_5_IRQHandler, TIM1_BRK_IRQHandler, TIM1_UP_IRQHandler,
    TIM1_TRG_COM_IRQHandler, TIM1_CC_IRQHandler, TIM2_IRQHandler,
    TIM3_IRQHandler, TIM4_IRQHandler, I2C1_EV_IRQHandler, I2C1_ER_IRQHandler,
    I2C2_EV_IRQHandler, I2C2_ER_IRQHandler, SPI1_IRQHandler, SPI2_IRQHandler,
    USART1_IRQHandler, USART2_IRQHandler, USART3_IRQHandler,
    EXTI15_10_IRQHandler, RTC_Alarm_IRQHandler, 0, TIM8_BRK_TIM12_IRQHandler,
    TIM8_UP_TIM13_IRQHandler, TIM8_TRG_COM_TIM14_IRQHandler, TIM8_CC_IRQHandler,
    DMA1_Stream7_IRQHandler, FMC_IRQHandler, SDMMC1_IRQHandler, TIM5_IRQHandler,
    SPI3_IRQHandler, UART4_IRQHandler, UART5_IRQHandler, TIM6_DAC_IRQHandler,
    TIM7_IRQHandler, DMA2_Stream0_IRQHandler, DMA2_Stream1_IRQHandler,
    DMA2_Stream2_IRQHandler, DMA2_Stream3_IRQHandler, DMA2_Stream4_IRQHandler,
    ETH_IRQHandler, ETH_WKUP_IRQHandler, FDCAN_CAL_IRQHandler, 0, 0, 0, 0,
    DMA2_Stream5_IRQHandler, DMA2_Stream6_IRQHandler, DMA2_Stream7_IRQHandler,
    USART6_IRQHandler, I2C3_EV_IRQHandler, I2C3_ER_IRQHandler,
    OTG_HS_EP1_OUT_IRQHandler, OTG_HS_EP1_IN_IRQHandler, OTG_HS_WKUP_IRQHandler,
    OTG_HS_IRQHandler, DCMI_IRQHandler, 0, RNG_IRQHandler, FPU_IRQHandler,
    UART7_IRQHandler, UART8_IRQHandler, SPI4_IRQHandler, SPI5_IRQHandler,
    SPI6_IRQHandler, SAI1_IRQHandler, LTDC_IRQHandler, LTDC_ER_IRQHandler,
    DMA2D_IRQHandler, SAI2_IRQHandler, QUADSPI_IRQHandler, LPTIM1_IRQHandler,
    CEC_IRQHandler, I2C4_EV_IRQHandler, I2C4_ER_IRQHandler, SPDIF_RX_IRQHandler,
    OTG_FS_EP1_OUT_IRQHandler, OTG_FS_EP1_IN_IRQHandler, OTG_FS_WKUP_IRQHandler,
    OTG_FS_IRQHandler, DMAMUX1_OVR_IRQHandler, HRTIM1_Master_IRQHandler,
    HRTIM1_TIMA_IRQHandler, HRTIM1_TIMB_IRQHandler, HRTIM1_TIMC_IRQHandler,
    HRTIM1_TIMD_IRQHandler, HRTIM1_TIME_IRQHandler, HRTIM1_FLT_IRQHandler,
    DFSDM1_FLT0_IRQHandler, DFSDM1_FLT1_IRQHandler, DFSDM1_FLT2_IRQHandler,
    DFSDM1_FLT3_IRQHandler, SAI3_IRQHandler, SWPMI1_IRQHandler,
    TIM15_IRQHandler, TIM16_IRQHandler, TIM17_IRQHandler, MDIOS_WKUP_IRQHandler,
    MDIOS_IRQHandler, JPEG_IRQHandler, MDMA_IRQHandler, 0, SDMMC2_IRQHandler,
    HSEM1_IRQHandler, 0, ADC3_IRQHandler, DMAMUX2_OVR_IRQHandler,
    BDMA_Channel0_IRQHandler, BDMA_Channel1_IRQHandler,
    BDMA_Channel2_IRQHandler, BDMA_Channel3_IRQHandler,
    BDMA_Channel4_IRQHandler, BDMA_Channel5_IRQHandler,
    BDMA_Channel6_IRQHandler, BDMA_Channel7_IRQHandler, COMP1_IRQHandler,
    LPTIM2_IRQHandler, LPTIM3_IRQHandler, LPTIM4_IRQHandler, LPTIM5_IRQHandler,
    LPUART1_IRQHandler, 0, CRS_IRQHandler, ECC_IRQHandler, SAI4_IRQHandler, 0,
    0, WAKEUP_PIN_IRQHandler
}
;
