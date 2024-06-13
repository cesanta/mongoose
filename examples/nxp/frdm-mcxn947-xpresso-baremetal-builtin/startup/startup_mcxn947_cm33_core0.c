//*****************************************************************************
// MCXN947_cm33_core0 startup code for use with MCUXpresso IDE
//
// Version : 150923
//*****************************************************************************
//
// Copyright 2016-2023 NXP
// All rights reserved.
//
// SPDX-License-Identifier: BSD-3-Clause
//*****************************************************************************

#if defined (DEBUG)
#pragma GCC push_options
#pragma GCC optimize ("Og")
#endif // (DEBUG)

#if defined (__cplusplus)
#ifdef __REDLIB__
#error Redlib does not support C++
#else
//*****************************************************************************
//
// The entry point for the C++ library startup
//
//*****************************************************************************
extern "C" {
    extern void __libc_init_array(void);
}
#endif
#endif

#define WEAK __attribute__ ((weak))
#define WEAK_AV __attribute__ ((weak, section(".after_vectors")))
#define ALIAS(f) __attribute__ ((weak, alias (#f)))

//*****************************************************************************
#if defined (__cplusplus)
extern "C" {
#endif

//*****************************************************************************
// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
//*****************************************************************************
//*****************************************************************************
// Declaration of external SystemInit function
//*****************************************************************************
#if defined (__USE_CMSIS)
extern void SystemInit(void);
#endif // (__USE_CMSIS)

//*****************************************************************************
// Forward declaration of the core exception handlers.
// When the application defines a handler (with the same name), this will
// automatically take precedence over these weak definitions.
// If your application is a C++ one, then any interrupt handlers defined
// in C++ files within in your main application will need to have C linkage
// rather than C++ linkage. To do this, make sure that you are using extern "C"
// { .... } around the interrupt handler within your main application code.
//*****************************************************************************
     void ResetISR(void);
WEAK void NMI_Handler(void);
WEAK void HardFault_Handler(void);
WEAK void MemManage_Handler(void);
WEAK void BusFault_Handler(void);
WEAK void UsageFault_Handler(void);
WEAK void SecureFault_Handler(void);
WEAK void SVC_Handler(void);
WEAK void DebugMon_Handler(void);
WEAK void PendSV_Handler(void);
WEAK void SysTick_Handler(void);
WEAK void IntDefaultHandler(void);

//*****************************************************************************
// Forward declaration of the application IRQ handlers. When the application
// defines a handler (with the same name), this will automatically take
// precedence over weak definitions below
//*****************************************************************************
WEAK void OR_IRQHandler(void);
WEAK void EDMA_0_CH0_IRQHandler(void);
WEAK void EDMA_0_CH1_IRQHandler(void);
WEAK void EDMA_0_CH2_IRQHandler(void);
WEAK void EDMA_0_CH3_IRQHandler(void);
WEAK void EDMA_0_CH4_IRQHandler(void);
WEAK void EDMA_0_CH5_IRQHandler(void);
WEAK void EDMA_0_CH6_IRQHandler(void);
WEAK void EDMA_0_CH7_IRQHandler(void);
WEAK void EDMA_0_CH8_IRQHandler(void);
WEAK void EDMA_0_CH9_IRQHandler(void);
WEAK void EDMA_0_CH10_IRQHandler(void);
WEAK void EDMA_0_CH11_IRQHandler(void);
WEAK void EDMA_0_CH12_IRQHandler(void);
WEAK void EDMA_0_CH13_IRQHandler(void);
WEAK void EDMA_0_CH14_IRQHandler(void);
WEAK void EDMA_0_CH15_IRQHandler(void);
WEAK void GPIO00_IRQHandler(void);
WEAK void GPIO01_IRQHandler(void);
WEAK void GPIO10_IRQHandler(void);
WEAK void GPIO11_IRQHandler(void);
WEAK void GPIO20_IRQHandler(void);
WEAK void GPIO21_IRQHandler(void);
WEAK void GPIO30_IRQHandler(void);
WEAK void GPIO31_IRQHandler(void);
WEAK void GPIO40_IRQHandler(void);
WEAK void GPIO41_IRQHandler(void);
WEAK void GPIO50_IRQHandler(void);
WEAK void GPIO51_IRQHandler(void);
WEAK void UTICK0_IRQHandler(void);
WEAK void MRT0_IRQHandler(void);
WEAK void CTIMER0_IRQHandler(void);
WEAK void CTIMER1_IRQHandler(void);
WEAK void SCT0_IRQHandler(void);
WEAK void CTIMER2_IRQHandler(void);
WEAK void LP_FLEXCOMM0_IRQHandler(void);
WEAK void LP_FLEXCOMM1_IRQHandler(void);
WEAK void LP_FLEXCOMM2_IRQHandler(void);
WEAK void LP_FLEXCOMM3_IRQHandler(void);
WEAK void LP_FLEXCOMM4_IRQHandler(void);
WEAK void LP_FLEXCOMM5_IRQHandler(void);
WEAK void LP_FLEXCOMM6_IRQHandler(void);
WEAK void LP_FLEXCOMM7_IRQHandler(void);
WEAK void LP_FLEXCOMM8_IRQHandler(void);
WEAK void LP_FLEXCOMM9_IRQHandler(void);
WEAK void ADC0_IRQHandler(void);
WEAK void ADC1_IRQHandler(void);
WEAK void PINT0_IRQHandler(void);
WEAK void PDM_EVENT_IRQHandler(void);
WEAK void Reserved65_IRQHandler(void);
WEAK void USB0_FS_IRQHandler(void);
WEAK void USB0_DCD_IRQHandler(void);
WEAK void RTC_IRQHandler(void);
WEAK void SMARTDMA_IRQHandler(void);
WEAK void MAILBOX_IRQHandler(void);
WEAK void CTIMER3_IRQHandler(void);
WEAK void CTIMER4_IRQHandler(void);
WEAK void OS_EVENT_IRQHandler(void);
WEAK void FLEXSPI0_IRQHandler(void);
WEAK void SAI0_IRQHandler(void);
WEAK void SAI1_IRQHandler(void);
WEAK void USDHC0_IRQHandler(void);
WEAK void CAN0_IRQHandler(void);
WEAK void CAN1_IRQHandler(void);
WEAK void Reserved80_IRQHandler(void);
WEAK void Reserved81_IRQHandler(void);
WEAK void USB1_HS_PHY_IRQHandler(void);
WEAK void USB1_HS_IRQHandler(void);
WEAK void SEC_HYPERVISOR_CALL_IRQHandler(void);
WEAK void Reserved85_IRQHandler(void);
WEAK void PLU_IRQHandler(void);
WEAK void Freqme_IRQHandler(void);
WEAK void SEC_VIO_IRQHandler(void);
WEAK void ELS_IRQHandler(void);
WEAK void PKC_IRQHandler(void);
WEAK void PUF_IRQHandler(void);
WEAK void PQ_IRQHandler(void);
WEAK void EDMA_1_CH0_IRQHandler(void);
WEAK void EDMA_1_CH1_IRQHandler(void);
WEAK void EDMA_1_CH2_IRQHandler(void);
WEAK void EDMA_1_CH3_IRQHandler(void);
WEAK void EDMA_1_CH4_IRQHandler(void);
WEAK void EDMA_1_CH5_IRQHandler(void);
WEAK void EDMA_1_CH6_IRQHandler(void);
WEAK void EDMA_1_CH7_IRQHandler(void);
WEAK void EDMA_1_CH8_IRQHandler(void);
WEAK void EDMA_1_CH9_IRQHandler(void);
WEAK void EDMA_1_CH10_IRQHandler(void);
WEAK void EDMA_1_CH11_IRQHandler(void);
WEAK void EDMA_1_CH12_IRQHandler(void);
WEAK void EDMA_1_CH13_IRQHandler(void);
WEAK void EDMA_1_CH14_IRQHandler(void);
WEAK void EDMA_1_CH15_IRQHandler(void);
WEAK void CDOG0_IRQHandler(void);
WEAK void CDOG1_IRQHandler(void);
WEAK void I3C0_IRQHandler(void);
WEAK void I3C1_IRQHandler(void);
WEAK void NPU_IRQHandler(void);
WEAK void GDET_IRQHandler(void);
WEAK void VBAT0_IRQHandler(void);
WEAK void EWM0_IRQHandler(void);
WEAK void TSI_END_OF_SCAN_IRQHandler(void);
WEAK void TSI_OUT_OF_SCAN_IRQHandler(void);
WEAK void EMVSIM0_IRQHandler(void);
WEAK void EMVSIM1_IRQHandler(void);
WEAK void FLEXIO_IRQHandler(void);
WEAK void DAC0_IRQHandler(void);
WEAK void DAC1_IRQHandler(void);
WEAK void DAC2_IRQHandler(void);
WEAK void HSCMP0_IRQHandler(void);
WEAK void HSCMP1_IRQHandler(void);
WEAK void HSCMP2_IRQHandler(void);
WEAK void FLEXPWM0_RELOAD_ERROR_IRQHandler(void);
WEAK void FLEXPWM0_FAULT_IRQHandler(void);
WEAK void FLEXPWM0_SUBMODULE0_IRQHandler(void);
WEAK void FLEXPWM0_SUBMODULE1_IRQHandler(void);
WEAK void FLEXPWM0_SUBMODULE2_IRQHandler(void);
WEAK void FLEXPWM0_SUBMODULE3_IRQHandler(void);
WEAK void FLEXPWM1_RELOAD_ERROR_IRQHandler(void);
WEAK void FLEXPWM1_FAULT_IRQHandler(void);
WEAK void FLEXPWM1_SUBMODULE0_IRQHandler(void);
WEAK void FLEXPWM1_SUBMODULE1_IRQHandler(void);
WEAK void FLEXPWM1_SUBMODULE2_IRQHandler(void);
WEAK void FLEXPWM1_SUBMODULE3_IRQHandler(void);
WEAK void ENC0_COMPARE_IRQHandler(void);
WEAK void ENC0_HOME_IRQHandler(void);
WEAK void ENC0_WDG_SAB_IRQHandler(void);
WEAK void ENC0_IDX_IRQHandler(void);
WEAK void ENC1_COMPARE_IRQHandler(void);
WEAK void ENC1_HOME_IRQHandler(void);
WEAK void ENC1_WDG_SAB_IRQHandler(void);
WEAK void ENC1_IDX_IRQHandler(void);
WEAK void ITRC0_IRQHandler(void);
WEAK void BSP32_IRQHandler(void);
WEAK void ELS_ERR_IRQHandler(void);
WEAK void PKC_ERR_IRQHandler(void);
WEAK void ERM_SINGLE_BIT_ERROR_IRQHandler(void);
WEAK void ERM_MULTI_BIT_ERROR_IRQHandler(void);
WEAK void FMU0_IRQHandler(void);
WEAK void ETHERNET_IRQHandler(void);
WEAK void ETHERNET_PMT_IRQHandler(void);
WEAK void ETHERNET_MACLP_IRQHandler(void);
WEAK void SINC_FILTER_IRQHandler(void);
WEAK void LPTMR0_IRQHandler(void);
WEAK void LPTMR1_IRQHandler(void);
WEAK void SCG_IRQHandler(void);
WEAK void SPC_IRQHandler(void);
WEAK void WUU_IRQHandler(void);
WEAK void PORT_EFT_IRQHandler(void);
WEAK void ETB0_IRQHandler(void);
WEAK void SM3_IRQHandler(void);
WEAK void TRNG0_IRQHandler(void);
WEAK void WWDT0_IRQHandler(void);
WEAK void WWDT1_IRQHandler(void);
WEAK void CMC0_IRQHandler(void);
WEAK void CTI0_IRQHandler(void);

//*****************************************************************************
// Forward declaration of the driver IRQ handlers. These are aliased
// to the IntDefaultHandler, which is a 'forever' loop. When the driver
// defines a handler (with the same name), this will automatically take
// precedence over these weak definitions
//*****************************************************************************
void OR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_0_CH0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_0_CH1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_0_CH2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_0_CH3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_0_CH4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_0_CH5_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_0_CH6_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_0_CH7_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_0_CH8_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_0_CH9_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_0_CH10_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_0_CH11_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_0_CH12_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_0_CH13_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_0_CH14_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_0_CH15_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO00_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO01_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO10_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO11_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO20_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO21_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO30_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO31_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO40_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO41_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO50_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO51_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UTICK0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void MRT0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CTIMER0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CTIMER1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SCT0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CTIMER2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LP_FLEXCOMM0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LP_FLEXCOMM1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LP_FLEXCOMM2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LP_FLEXCOMM3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LP_FLEXCOMM4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LP_FLEXCOMM5_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LP_FLEXCOMM6_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LP_FLEXCOMM7_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LP_FLEXCOMM8_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LP_FLEXCOMM9_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ADC0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ADC1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PINT0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PDM_EVENT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved65_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void USB0_FS_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void USB0_DCD_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void RTC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SMARTDMA_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void MAILBOX_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CTIMER3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CTIMER4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void OS_EVENT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXSPI0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SAI0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SAI1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void USDHC0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CAN0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CAN1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved80_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved81_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void USB1_HS_PHY_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void USB1_HS_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SEC_HYPERVISOR_CALL_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved85_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PLU_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Freqme_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SEC_VIO_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ELS_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PKC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PUF_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PQ_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_1_CH0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_1_CH1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_1_CH2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_1_CH3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_1_CH4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_1_CH5_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_1_CH6_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_1_CH7_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_1_CH8_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_1_CH9_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_1_CH10_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_1_CH11_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_1_CH12_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_1_CH13_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_1_CH14_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EDMA_1_CH15_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CDOG0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CDOG1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void I3C0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void I3C1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void NPU_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GDET_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void VBAT0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EWM0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TSI_END_OF_SCAN_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TSI_OUT_OF_SCAN_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EMVSIM0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EMVSIM1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXIO_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DAC0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DAC1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DAC2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void HSCMP0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void HSCMP1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void HSCMP2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXPWM0_RELOAD_ERROR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXPWM0_FAULT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXPWM0_SUBMODULE0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXPWM0_SUBMODULE1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXPWM0_SUBMODULE2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXPWM0_SUBMODULE3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXPWM1_RELOAD_ERROR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXPWM1_FAULT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXPWM1_SUBMODULE0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXPWM1_SUBMODULE1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXPWM1_SUBMODULE2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXPWM1_SUBMODULE3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ENC0_COMPARE_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ENC0_HOME_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ENC0_WDG_SAB_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ENC0_IDX_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ENC1_COMPARE_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ENC1_HOME_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ENC1_WDG_SAB_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ENC1_IDX_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ITRC0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void BSP32_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ELS_ERR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PKC_ERR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ERM_SINGLE_BIT_ERROR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ERM_MULTI_BIT_ERROR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FMU0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ETHERNET_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ETHERNET_PMT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ETHERNET_MACLP_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SINC_FILTER_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPTMR0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPTMR1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SCG_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SPC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void WUU_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PORT_EFT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ETB0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SM3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TRNG0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void WWDT0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void WWDT1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CMC0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CTI0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);

//*****************************************************************************
// The entry point for the application.
// __main() is the entry point for Redlib based applications
// main() is the entry point for Newlib based applications
//*****************************************************************************
#if defined (__REDLIB__)
extern void __main(void);
#endif
extern int main(void);

//*****************************************************************************
// External declaration for the pointer to the stack top from the Linker Script
//*****************************************************************************
extern void _vStackTop(void);
extern void _vStackBase(void);
//*****************************************************************************
#if defined (__cplusplus)
} // extern "C"
#endif
//*****************************************************************************
// The vector table.
// This relies on the linker script to place at correct location in memory.
//*****************************************************************************

extern void (* const g_pfnVectors[])(void);
extern void * __Vectors __attribute__ ((alias ("g_pfnVectors")));

__attribute__ ((used, section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    // Core Level - CM33
    &_vStackTop,                       // The initial stack pointer
    ResetISR,                          // The reset handler
    NMI_Handler,                       // NMI Handler
    HardFault_Handler,                 // Hard Fault Handler
    MemManage_Handler,                 // MPU Fault Handler
    BusFault_Handler,                  // Bus Fault Handler
    UsageFault_Handler,                // Usage Fault Handler
    SecureFault_Handler,               // Secure Fault Handler
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    SVC_Handler,                       // SVCall Handler
    DebugMon_Handler,                  // Debug Monitor Handler
    0,                                 // Reserved
    PendSV_Handler,                    // PendSV Handler
    SysTick_Handler,                   // SysTick Handler

    // Chip Level - MCXN947_cm33_core0
    OR_IRQHandler,                     // 16 : OR IRQ
    EDMA_0_CH0_IRQHandler,             // 17 : eDMA_0_CH0 error or transfer complete
    EDMA_0_CH1_IRQHandler,             // 18 : eDMA_0_CH1 error or transfer complete
    EDMA_0_CH2_IRQHandler,             // 19 : eDMA_0_CH2 error or transfer complete
    EDMA_0_CH3_IRQHandler,             // 20 : eDMA_0_CH3 error or transfer complete
    EDMA_0_CH4_IRQHandler,             // 21 : eDMA_0_CH4 error or transfer complete
    EDMA_0_CH5_IRQHandler,             // 22 : eDMA_0_CH5 error or transfer complete
    EDMA_0_CH6_IRQHandler,             // 23 : eDMA_0_CH6 error or transfer complete
    EDMA_0_CH7_IRQHandler,             // 24 : eDMA_0_CH7 error or transfer complete
    EDMA_0_CH8_IRQHandler,             // 25 : eDMA_0_CH8 error or transfer complete
    EDMA_0_CH9_IRQHandler,             // 26 : eDMA_0_CH9 error or transfer complete
    EDMA_0_CH10_IRQHandler,            // 27 : eDMA_0_CH10 error or transfer complete
    EDMA_0_CH11_IRQHandler,            // 28 : eDMA_0_CH11 error or transfer complete
    EDMA_0_CH12_IRQHandler,            // 29 : eDMA_0_CH12 error or transfer complete
    EDMA_0_CH13_IRQHandler,            // 30 : eDMA_0_CH13 error or transfer complete
    EDMA_0_CH14_IRQHandler,            // 31 : eDMA_0_CH14 error or transfer complete
    EDMA_0_CH15_IRQHandler,            // 32 : eDMA_0_CH15 error or transfer complete
    GPIO00_IRQHandler,                 // 33 : GPIO0 interrupt 0
    GPIO01_IRQHandler,                 // 34 : GPIO0 interrupt 1
    GPIO10_IRQHandler,                 // 35 : GPIO1 interrupt 0
    GPIO11_IRQHandler,                 // 36 : GPIO1 interrupt 1
    GPIO20_IRQHandler,                 // 37 : GPIO2 interrupt 0
    GPIO21_IRQHandler,                 // 38 : GPIO2 interrupt 1
    GPIO30_IRQHandler,                 // 39 : GPIO3 interrupt 0
    GPIO31_IRQHandler,                 // 40 : GPIO3 interrupt 1
    GPIO40_IRQHandler,                 // 41 : GPIO4 interrupt 0
    GPIO41_IRQHandler,                 // 42 : GPIO4 interrupt 1
    GPIO50_IRQHandler,                 // 43 : GPIO5 interrupt 0
    GPIO51_IRQHandler,                 // 44 : GPIO5 interrupt 1
    UTICK0_IRQHandler,                 // 45 : Micro-Tick Timer interrupt
    MRT0_IRQHandler,                   // 46 : Multi-Rate Timer interrupt
    CTIMER0_IRQHandler,                // 47 : Standard counter/timer 0 interrupt
    CTIMER1_IRQHandler,                // 48 : Standard counter/timer 1 interrupt
    SCT0_IRQHandler,                   // 49 : SCTimer/PWM interrupt
    CTIMER2_IRQHandler,                // 50 : Standard counter/timer 2 interrupt
    LP_FLEXCOMM0_IRQHandler,           // 51 : LP_FLEXCOMM0 (LPSPI interrupt or LPI2C interrupt or LPUART Receive/Transmit interrupt)
    LP_FLEXCOMM1_IRQHandler,           // 52 : LP_FLEXCOMM1 (LPSPI interrupt or LPI2C interrupt or LPUART Receive/Transmit interrupt)
    LP_FLEXCOMM2_IRQHandler,           // 53 : LP_FLEXCOMM2 (LPSPI interrupt or LPI2C interrupt or LPUART Receive/Transmit interrupt)
    LP_FLEXCOMM3_IRQHandler,           // 54 : LP_FLEXCOMM3 (LPSPI interrupt or LPI2C interrupt or LPUART Receive/Transmit interrupt)
    LP_FLEXCOMM4_IRQHandler,           // 55 : LP_FLEXCOMM4 (LPSPI interrupt or LPI2C interrupt or LPUART Receive/Transmit interrupt)
    LP_FLEXCOMM5_IRQHandler,           // 56 : LP_FLEXCOMM5 (LPSPI interrupt or LPI2C interrupt or LPUART Receive/Transmit interrupt)
    LP_FLEXCOMM6_IRQHandler,           // 57 : LP_FLEXCOMM6 (LPSPI interrupt or LPI2C interrupt or LPUART Receive/Transmit interrupt)
    LP_FLEXCOMM7_IRQHandler,           // 58 : LP_FLEXCOMM7 (LPSPI interrupt or LPI2C interrupt or LPUART Receive/Transmit interrupt)
    LP_FLEXCOMM8_IRQHandler,           // 59 : LP_FLEXCOMM8 (LPSPI interrupt or LPI2C interrupt or LPUART Receive/Transmit interrupt)
    LP_FLEXCOMM9_IRQHandler,           // 60 : LP_FLEXCOMM9 (LPSPI interrupt or LPI2C interrupt or LPUART Receive/Transmit interrupt)
    ADC0_IRQHandler,                   // 61 : Analog-to-Digital Converter 0 - General Purpose interrupt
    ADC1_IRQHandler,                   // 62 : Analog-to-Digital Converter 1 - General Purpose interrupt
    PINT0_IRQHandler,                  // 63 : Pin Interrupt Pattern Match Interrupt
    PDM_EVENT_IRQHandler,              // 64 : Microphone Interface interrupt
    Reserved65_IRQHandler,             // 65 : Reserved interrupt
    USB0_FS_IRQHandler,                // 66 : Universal Serial Bus - Full Speed interrupt
    USB0_DCD_IRQHandler,               // 67 : Universal Serial Bus - Device Charge Detect interrupt
    RTC_IRQHandler,                    // 68 : RTC Subsystem interrupt (RTC interrupt or Wake timer interrupt)
    SMARTDMA_IRQHandler,               // 69 : SmartDMA_IRQ
    MAILBOX_IRQHandler,                // 70 : Inter-CPU Mailbox interrupt0 for CPU0 Inter-CPU Mailbox interrupt1 for CPU1
    CTIMER3_IRQHandler,                // 71 : Standard counter/timer 3 interrupt
    CTIMER4_IRQHandler,                // 72 : Standard counter/timer 4 interrupt
    OS_EVENT_IRQHandler,               // 73 : OS event timer interrupt
    FLEXSPI0_IRQHandler,               // 74 : Flexible Serial Peripheral Interface interrupt
    SAI0_IRQHandler,                   // 75 : Serial Audio Interface 0 interrupt
    SAI1_IRQHandler,                   // 76 : Serial Audio Interface 1 interrupt
    USDHC0_IRQHandler,                 // 77 : Ultra Secured Digital Host Controller interrupt
    CAN0_IRQHandler,                   // 78 : Controller Area Network 0 interrupt
    CAN1_IRQHandler,                   // 79 : Controller Area Network 1 interrupt
    Reserved80_IRQHandler,             // 80 : Reserved interrupt
    Reserved81_IRQHandler,             // 81 : Reserved interrupt
    USB1_HS_PHY_IRQHandler,            // 82 : USBHS DCD or USBHS Phy interrupt
    USB1_HS_IRQHandler,                // 83 : USB High Speed OTG Controller interrupt
    SEC_HYPERVISOR_CALL_IRQHandler,    // 84 : AHB Secure Controller hypervisor call interrupt
    Reserved85_IRQHandler,             // 85 : Reserved interrupt
    PLU_IRQHandler,                    // 86 : Programmable Logic Unit interrupt
    Freqme_IRQHandler,                 // 87 : Frequency Measurement interrupt
    SEC_VIO_IRQHandler,                // 88 : Secure violation interrupt (Memory Block Checker interrupt or secure AHB matrix violation interrupt)
    ELS_IRQHandler,                    // 89 : ELS interrupt
    PKC_IRQHandler,                    // 90 : PKC interrupt
    PUF_IRQHandler,                    // 91 : Physical Unclonable Function interrupt
    PQ_IRQHandler,                     // 92 : Power Quad interrupt
    EDMA_1_CH0_IRQHandler,             // 93 : eDMA_1_CH0 error or transfer complete
    EDMA_1_CH1_IRQHandler,             // 94 : eDMA_1_CH1 error or transfer complete
    EDMA_1_CH2_IRQHandler,             // 95 : eDMA_1_CH2 error or transfer complete
    EDMA_1_CH3_IRQHandler,             // 96 : eDMA_1_CH3 error or transfer complete
    EDMA_1_CH4_IRQHandler,             // 97 : eDMA_1_CH4 error or transfer complete
    EDMA_1_CH5_IRQHandler,             // 98 : eDMA_1_CH5 error or transfer complete
    EDMA_1_CH6_IRQHandler,             // 99 : eDMA_1_CH6 error or transfer complete
    EDMA_1_CH7_IRQHandler,             // 100: eDMA_1_CH7 error or transfer complete
    EDMA_1_CH8_IRQHandler,             // 101: eDMA_1_CH8 error or transfer complete
    EDMA_1_CH9_IRQHandler,             // 102: eDMA_1_CH9 error or transfer complete
    EDMA_1_CH10_IRQHandler,            // 103: eDMA_1_CH10 error or transfer complete
    EDMA_1_CH11_IRQHandler,            // 104: eDMA_1_CH11 error or transfer complete
    EDMA_1_CH12_IRQHandler,            // 105: eDMA_1_CH12 error or transfer complete
    EDMA_1_CH13_IRQHandler,            // 106: eDMA_1_CH13 error or transfer complete
    EDMA_1_CH14_IRQHandler,            // 107: eDMA_1_CH14 error or transfer complete
    EDMA_1_CH15_IRQHandler,            // 108: eDMA_1_CH15 error or transfer complete
    CDOG0_IRQHandler,                  // 109: Code Watchdog Timer 0 interrupt
    CDOG1_IRQHandler,                  // 110: Code Watchdog Timer 1 interrupt
    I3C0_IRQHandler,                   // 111: Improved Inter Integrated Circuit interrupt 0
    I3C1_IRQHandler,                   // 112: Improved Inter Integrated Circuit interrupt 1
    NPU_IRQHandler,                    // 113: NPU interrupt
    GDET_IRQHandler,                   // 114: Digital Glitch Detect 0 interrupt  or Digital Glitch Detect 1 interrupt
    VBAT0_IRQHandler,                  // 115: VBAT interrupt( VBAT interrupt or digital tamper interrupt)
    EWM0_IRQHandler,                   // 116: External Watchdog Monitor interrupt
    TSI_END_OF_SCAN_IRQHandler,        // 117: TSI End of Scan interrupt
    TSI_OUT_OF_SCAN_IRQHandler,        // 118: TSI Out of Scan interrupt
    EMVSIM0_IRQHandler,                // 119: EMVSIM0 interrupt
    EMVSIM1_IRQHandler,                // 120: EMVSIM1 interrupt
    FLEXIO_IRQHandler,                 // 121: Flexible Input/Output interrupt
    DAC0_IRQHandler,                   // 122: Digital-to-Analog Converter 0 - General Purpose interrupt
    DAC1_IRQHandler,                   // 123: Digital-to-Analog Converter 1 - General Purpose interrupt
    DAC2_IRQHandler,                   // 124: 14-bit Digital-to-Analog Converter interrupt
    HSCMP0_IRQHandler,                 // 125: High-Speed comparator0 interrupt
    HSCMP1_IRQHandler,                 // 126: High-Speed comparator1 interrupt
    HSCMP2_IRQHandler,                 // 127: High-Speed comparator2 interrupt
    FLEXPWM0_RELOAD_ERROR_IRQHandler,  // 128: FlexPWM0_reload_error interrupt
    FLEXPWM0_FAULT_IRQHandler,         // 129: FlexPWM0_fault interrupt
    FLEXPWM0_SUBMODULE0_IRQHandler,    // 130: FlexPWM0 Submodule 0 capture/compare/reload interrupt
    FLEXPWM0_SUBMODULE1_IRQHandler,    // 131: FlexPWM0 Submodule 1 capture/compare/reload interrupt
    FLEXPWM0_SUBMODULE2_IRQHandler,    // 132: FlexPWM0 Submodule 2 capture/compare/reload interrupt
    FLEXPWM0_SUBMODULE3_IRQHandler,    // 133: FlexPWM0 Submodule 3 capture/compare/reload interrupt
    FLEXPWM1_RELOAD_ERROR_IRQHandler,  // 134: FlexPWM1_reload_error interrupt
    FLEXPWM1_FAULT_IRQHandler,         // 135: FlexPWM1_fault interrupt
    FLEXPWM1_SUBMODULE0_IRQHandler,    // 136: FlexPWM1 Submodule 0 capture/compare/reload interrupt
    FLEXPWM1_SUBMODULE1_IRQHandler,    // 137: FlexPWM1 Submodule 1 capture/compare/reload interrupt
    FLEXPWM1_SUBMODULE2_IRQHandler,    // 138: FlexPWM1 Submodule 2 capture/compare/reload interrupt
    FLEXPWM1_SUBMODULE3_IRQHandler,    // 139: FlexPWM1 Submodule 3 capture/compare/reload interrupt
    ENC0_COMPARE_IRQHandler,           // 140: ENC0_Compare interrupt
    ENC0_HOME_IRQHandler,              // 141: ENC0_Home interrupt
    ENC0_WDG_SAB_IRQHandler,           // 142: ENC0_WDG_IRQ/SAB interrupt
    ENC0_IDX_IRQHandler,               // 143: ENC0_IDX interrupt
    ENC1_COMPARE_IRQHandler,           // 144: ENC1_Compare interrupt
    ENC1_HOME_IRQHandler,              // 145: ENC1_Home interrupt
    ENC1_WDG_SAB_IRQHandler,           // 146: ENC1_WDG_IRQ/SAB interrupt
    ENC1_IDX_IRQHandler,               // 147: ENC1_IDX interrupt
    ITRC0_IRQHandler,                  // 148: Intrusion and Tamper Response Controller interrupt
    BSP32_IRQHandler,                  // 149: CoolFlux BSP32 interrupt
    ELS_ERR_IRQHandler,                // 150: ELS error interrupt
    PKC_ERR_IRQHandler,                // 151: PKC error interrupt
    ERM_SINGLE_BIT_ERROR_IRQHandler,   // 152: ERM Single Bit error interrupt
    ERM_MULTI_BIT_ERROR_IRQHandler,    // 153: ERM Multi Bit error interrupt
    FMU0_IRQHandler,                   // 154: Flash Management Unit interrupt
    ETHERNET_IRQHandler,               // 155: Ethernet QoS interrupt
    ETHERNET_PMT_IRQHandler,           // 156: Ethernet QoS power management interrupt
    ETHERNET_MACLP_IRQHandler,         // 157: Ethernet QoS MAC interrupt
    SINC_FILTER_IRQHandler,            // 158: SINC Filter interrupt
    LPTMR0_IRQHandler,                 // 159: Low Power Timer 0 interrupt
    LPTMR1_IRQHandler,                 // 160: Low Power Timer 1 interrupt
    SCG_IRQHandler,                    // 161: System Clock Generator interrupt
    SPC_IRQHandler,                    // 162: System Power Controller interrupt
    WUU_IRQHandler,                    // 163: Wake Up Unit interrupt
    PORT_EFT_IRQHandler,               // 164: PORT0~5 EFT interrupt
    ETB0_IRQHandler,                   // 165: ETB counter expires interrupt
    SM3_IRQHandler,                    // 166: Secure Generic Interface (SGI) SAFO interrupt
    TRNG0_IRQHandler,                  // 167: True Random Number Generator interrupt
    WWDT0_IRQHandler,                  // 168: Windowed Watchdog Timer 0 interrupt
    WWDT1_IRQHandler,                  // 169: Windowed Watchdog Timer 1 interrupt
    CMC0_IRQHandler,                   // 170: Core Mode Controller interrupt
    CTI0_IRQHandler,                   // 171: Cross Trigger Interface interrupt
}; /* End of g_pfnVectors */

//*****************************************************************************
// Functions to carry out the initialization of RW and BSS data sections. These
// are written as separate functions rather than being inlined within the
// ResetISR() function in order to cope with MCUs with multiple banks of
// memory.
//*****************************************************************************
__attribute__ ((section(".after_vectors.init_data")))
void data_init(unsigned int romstart, unsigned int start, unsigned int len) {
    unsigned int *pulDest = (unsigned int*) start;
    unsigned int *pulSrc = (unsigned int*) romstart;
    unsigned int loop;
    for (loop = 0; loop < len; loop = loop + 4)
        *pulDest++ = *pulSrc++;
}

__attribute__ ((section(".after_vectors.init_bss")))
void bss_init(unsigned int start, unsigned int len) {
    unsigned int *pulDest = (unsigned int*) start;
    unsigned int loop;
    for (loop = 0; loop < len; loop = loop + 4)
        *pulDest++ = 0;
}

//*****************************************************************************
// The following symbols are constructs generated by the linker, indicating
// the location of various points in the "Global Section Table". This table is
// created by the linker via the Code Red managed linker script mechanism. It
// contains the load address, execution address and length of each RW data
// section and the execution and length of each BSS (zero initialized) section.
//*****************************************************************************
extern unsigned int __data_section_table;
extern unsigned int __data_section_table_end;
extern unsigned int __bss_section_table;
extern unsigned int __bss_section_table_end;

//*****************************************************************************
// Reset entry point for your code.
// Sets up a simple runtime environment and initializes the C/C++
// library.
//*****************************************************************************
__attribute__ ((naked, section(".after_vectors.reset")))
void ResetISR(void) {
    // Disable interrupts
    __asm volatile ("cpsid i");
    // Config VTOR & MSPLIM register
    __asm volatile ("LDR R0, =0xE000ED08  \n"
                    "STR %0, [R0]         \n"
                    "LDR R1, [%0]         \n"
                    "MSR MSP, R1          \n"
                    "MSR MSPLIM, %1       \n"
                    :
                    : "r"(g_pfnVectors), "r"(_vStackBase)
                    : "r0", "r1");

#if defined (__USE_CMSIS)
// If __USE_CMSIS defined, then call CMSIS SystemInit code
    SystemInit();

#endif // (__USE_CMSIS)

    //
    // Copy the data sections from flash to SRAM.
    //
    unsigned int LoadAddr, ExeAddr, SectionLen;
    unsigned int *SectionTableAddr;

    // Load base address of Global Section Table
    SectionTableAddr = &__data_section_table;

    // Copy the data sections from flash to SRAM.
    while (SectionTableAddr < &__data_section_table_end) {
        LoadAddr = *SectionTableAddr++;
        ExeAddr = *SectionTableAddr++;
        SectionLen = *SectionTableAddr++;
        data_init(LoadAddr, ExeAddr, SectionLen);
    }

    // At this point, SectionTableAddr = &__bss_section_table;
    // Zero fill the bss segment
    while (SectionTableAddr < &__bss_section_table_end) {
        ExeAddr = *SectionTableAddr++;
        SectionLen = *SectionTableAddr++;
        bss_init(ExeAddr, SectionLen);
    }

#if !defined (__USE_CMSIS)
// Assume that if __USE_CMSIS defined, then CMSIS SystemInit code
// will setup the VTOR register

    // Check to see if we are running the code from a non-zero
    // address (eg RAM, external flash), in which case we need
    // to modify the VTOR register to tell the CPU that the
    // vector table is located at a non-0x0 address.
    unsigned int * pSCB_VTOR = (unsigned int *) 0xE000ED08;
    if ((unsigned int *)g_pfnVectors!=(unsigned int *) 0x00000000) {
        *pSCB_VTOR = (unsigned int)g_pfnVectors;
    }
#endif // (__USE_CMSIS)
#if defined (__cplusplus)
    //
    // Call C++ library initialisation
    //
    __libc_init_array();
#endif

    // Reenable interrupts
    __asm volatile ("cpsie i");

#if defined (__REDLIB__)
    // Call the Redlib library, which in turn calls main()
    __main();
#else
    main();
#endif

    //
    // main() shouldn't return, but if it does, we'll just enter an infinite loop
    //
    while (1) {
        ;
    }
}

//*****************************************************************************
// Default core exception handlers. Override the ones here by defining your own
// handler routines in your application code.
//*****************************************************************************
WEAK_AV void NMI_Handler(void)
{ while(1) {}
}

WEAK_AV void HardFault_Handler(void)
{ while(1) {}
}

WEAK_AV void MemManage_Handler(void)
{ while(1) {}
}

WEAK_AV void BusFault_Handler(void)
{ while(1) {}
}

WEAK_AV void UsageFault_Handler(void)
{ while(1) {}
}

WEAK_AV void SecureFault_Handler(void)
{ while(1) {}
}

WEAK_AV void SVC_Handler(void)
{ while(1) {}
}

WEAK_AV void DebugMon_Handler(void)
{ while(1) {}
}

WEAK_AV void PendSV_Handler(void)
{ while(1) {}
}

WEAK_AV void SysTick_Handler(void)
{ while(1) {}
}

//*****************************************************************************
// Processor ends up here if an unexpected interrupt occurs or a specific
// handler is not present in the application code.
//*****************************************************************************
WEAK_AV void IntDefaultHandler(void)
{ while(1) {}
}

//*****************************************************************************
// Default application exception handlers. Override the ones here by defining
// your own handler routines in your application code. These routines call
// driver exception handlers or IntDefaultHandler() if no driver exception
// handler is included.
//*****************************************************************************
WEAK void OR_IRQHandler(void)
{   OR_DriverIRQHandler();
}

WEAK void EDMA_0_CH0_IRQHandler(void)
{   EDMA_0_CH0_DriverIRQHandler();
}

WEAK void EDMA_0_CH1_IRQHandler(void)
{   EDMA_0_CH1_DriverIRQHandler();
}

WEAK void EDMA_0_CH2_IRQHandler(void)
{   EDMA_0_CH2_DriverIRQHandler();
}

WEAK void EDMA_0_CH3_IRQHandler(void)
{   EDMA_0_CH3_DriverIRQHandler();
}

WEAK void EDMA_0_CH4_IRQHandler(void)
{   EDMA_0_CH4_DriverIRQHandler();
}

WEAK void EDMA_0_CH5_IRQHandler(void)
{   EDMA_0_CH5_DriverIRQHandler();
}

WEAK void EDMA_0_CH6_IRQHandler(void)
{   EDMA_0_CH6_DriverIRQHandler();
}

WEAK void EDMA_0_CH7_IRQHandler(void)
{   EDMA_0_CH7_DriverIRQHandler();
}

WEAK void EDMA_0_CH8_IRQHandler(void)
{   EDMA_0_CH8_DriverIRQHandler();
}

WEAK void EDMA_0_CH9_IRQHandler(void)
{   EDMA_0_CH9_DriverIRQHandler();
}

WEAK void EDMA_0_CH10_IRQHandler(void)
{   EDMA_0_CH10_DriverIRQHandler();
}

WEAK void EDMA_0_CH11_IRQHandler(void)
{   EDMA_0_CH11_DriverIRQHandler();
}

WEAK void EDMA_0_CH12_IRQHandler(void)
{   EDMA_0_CH12_DriverIRQHandler();
}

WEAK void EDMA_0_CH13_IRQHandler(void)
{   EDMA_0_CH13_DriverIRQHandler();
}

WEAK void EDMA_0_CH14_IRQHandler(void)
{   EDMA_0_CH14_DriverIRQHandler();
}

WEAK void EDMA_0_CH15_IRQHandler(void)
{   EDMA_0_CH15_DriverIRQHandler();
}

WEAK void GPIO00_IRQHandler(void)
{   GPIO00_DriverIRQHandler();
}

WEAK void GPIO01_IRQHandler(void)
{   GPIO01_DriverIRQHandler();
}

WEAK void GPIO10_IRQHandler(void)
{   GPIO10_DriverIRQHandler();
}

WEAK void GPIO11_IRQHandler(void)
{   GPIO11_DriverIRQHandler();
}

WEAK void GPIO20_IRQHandler(void)
{   GPIO20_DriverIRQHandler();
}

WEAK void GPIO21_IRQHandler(void)
{   GPIO21_DriverIRQHandler();
}

WEAK void GPIO30_IRQHandler(void)
{   GPIO30_DriverIRQHandler();
}

WEAK void GPIO31_IRQHandler(void)
{   GPIO31_DriverIRQHandler();
}

WEAK void GPIO40_IRQHandler(void)
{   GPIO40_DriverIRQHandler();
}

WEAK void GPIO41_IRQHandler(void)
{   GPIO41_DriverIRQHandler();
}

WEAK void GPIO50_IRQHandler(void)
{   GPIO50_DriverIRQHandler();
}

WEAK void GPIO51_IRQHandler(void)
{   GPIO51_DriverIRQHandler();
}

WEAK void UTICK0_IRQHandler(void)
{   UTICK0_DriverIRQHandler();
}

WEAK void MRT0_IRQHandler(void)
{   MRT0_DriverIRQHandler();
}

WEAK void CTIMER0_IRQHandler(void)
{   CTIMER0_DriverIRQHandler();
}

WEAK void CTIMER1_IRQHandler(void)
{   CTIMER1_DriverIRQHandler();
}

WEAK void SCT0_IRQHandler(void)
{   SCT0_DriverIRQHandler();
}

WEAK void CTIMER2_IRQHandler(void)
{   CTIMER2_DriverIRQHandler();
}

WEAK void LP_FLEXCOMM0_IRQHandler(void)
{   LP_FLEXCOMM0_DriverIRQHandler();
}

WEAK void LP_FLEXCOMM1_IRQHandler(void)
{   LP_FLEXCOMM1_DriverIRQHandler();
}

WEAK void LP_FLEXCOMM2_IRQHandler(void)
{   LP_FLEXCOMM2_DriverIRQHandler();
}

WEAK void LP_FLEXCOMM3_IRQHandler(void)
{   LP_FLEXCOMM3_DriverIRQHandler();
}

WEAK void LP_FLEXCOMM4_IRQHandler(void)
{   LP_FLEXCOMM4_DriverIRQHandler();
}

WEAK void LP_FLEXCOMM5_IRQHandler(void)
{   LP_FLEXCOMM5_DriverIRQHandler();
}

WEAK void LP_FLEXCOMM6_IRQHandler(void)
{   LP_FLEXCOMM6_DriverIRQHandler();
}

WEAK void LP_FLEXCOMM7_IRQHandler(void)
{   LP_FLEXCOMM7_DriverIRQHandler();
}

WEAK void LP_FLEXCOMM8_IRQHandler(void)
{   LP_FLEXCOMM8_DriverIRQHandler();
}

WEAK void LP_FLEXCOMM9_IRQHandler(void)
{   LP_FLEXCOMM9_DriverIRQHandler();
}

WEAK void ADC0_IRQHandler(void)
{   ADC0_DriverIRQHandler();
}

WEAK void ADC1_IRQHandler(void)
{   ADC1_DriverIRQHandler();
}

WEAK void PINT0_IRQHandler(void)
{   PINT0_DriverIRQHandler();
}

WEAK void PDM_EVENT_IRQHandler(void)
{   PDM_EVENT_DriverIRQHandler();
}

WEAK void Reserved65_IRQHandler(void)
{   Reserved65_DriverIRQHandler();
}

WEAK void USB0_FS_IRQHandler(void)
{   USB0_FS_DriverIRQHandler();
}

WEAK void USB0_DCD_IRQHandler(void)
{   USB0_DCD_DriverIRQHandler();
}

WEAK void RTC_IRQHandler(void)
{   RTC_DriverIRQHandler();
}

WEAK void SMARTDMA_IRQHandler(void)
{   SMARTDMA_DriverIRQHandler();
}

WEAK void MAILBOX_IRQHandler(void)
{   MAILBOX_DriverIRQHandler();
}

WEAK void CTIMER3_IRQHandler(void)
{   CTIMER3_DriverIRQHandler();
}

WEAK void CTIMER4_IRQHandler(void)
{   CTIMER4_DriverIRQHandler();
}

WEAK void OS_EVENT_IRQHandler(void)
{   OS_EVENT_DriverIRQHandler();
}

WEAK void FLEXSPI0_IRQHandler(void)
{   FLEXSPI0_DriverIRQHandler();
}

WEAK void SAI0_IRQHandler(void)
{   SAI0_DriverIRQHandler();
}

WEAK void SAI1_IRQHandler(void)
{   SAI1_DriverIRQHandler();
}

WEAK void USDHC0_IRQHandler(void)
{   USDHC0_DriverIRQHandler();
}

WEAK void CAN0_IRQHandler(void)
{   CAN0_DriverIRQHandler();
}

WEAK void CAN1_IRQHandler(void)
{   CAN1_DriverIRQHandler();
}

WEAK void Reserved80_IRQHandler(void)
{   Reserved80_DriverIRQHandler();
}

WEAK void Reserved81_IRQHandler(void)
{   Reserved81_DriverIRQHandler();
}

WEAK void USB1_HS_PHY_IRQHandler(void)
{   USB1_HS_PHY_DriverIRQHandler();
}

WEAK void USB1_HS_IRQHandler(void)
{   USB1_HS_DriverIRQHandler();
}

WEAK void SEC_HYPERVISOR_CALL_IRQHandler(void)
{   SEC_HYPERVISOR_CALL_DriverIRQHandler();
}

WEAK void Reserved85_IRQHandler(void)
{   Reserved85_DriverIRQHandler();
}

WEAK void PLU_IRQHandler(void)
{   PLU_DriverIRQHandler();
}

WEAK void Freqme_IRQHandler(void)
{   Freqme_DriverIRQHandler();
}

WEAK void SEC_VIO_IRQHandler(void)
{   SEC_VIO_DriverIRQHandler();
}

WEAK void ELS_IRQHandler(void)
{   ELS_DriverIRQHandler();
}

WEAK void PKC_IRQHandler(void)
{   PKC_DriverIRQHandler();
}

WEAK void PUF_IRQHandler(void)
{   PUF_DriverIRQHandler();
}

WEAK void PQ_IRQHandler(void)
{   PQ_DriverIRQHandler();
}

WEAK void EDMA_1_CH0_IRQHandler(void)
{   EDMA_1_CH0_DriverIRQHandler();
}

WEAK void EDMA_1_CH1_IRQHandler(void)
{   EDMA_1_CH1_DriverIRQHandler();
}

WEAK void EDMA_1_CH2_IRQHandler(void)
{   EDMA_1_CH2_DriverIRQHandler();
}

WEAK void EDMA_1_CH3_IRQHandler(void)
{   EDMA_1_CH3_DriverIRQHandler();
}

WEAK void EDMA_1_CH4_IRQHandler(void)
{   EDMA_1_CH4_DriverIRQHandler();
}

WEAK void EDMA_1_CH5_IRQHandler(void)
{   EDMA_1_CH5_DriverIRQHandler();
}

WEAK void EDMA_1_CH6_IRQHandler(void)
{   EDMA_1_CH6_DriverIRQHandler();
}

WEAK void EDMA_1_CH7_IRQHandler(void)
{   EDMA_1_CH7_DriverIRQHandler();
}

WEAK void EDMA_1_CH8_IRQHandler(void)
{   EDMA_1_CH8_DriverIRQHandler();
}

WEAK void EDMA_1_CH9_IRQHandler(void)
{   EDMA_1_CH9_DriverIRQHandler();
}

WEAK void EDMA_1_CH10_IRQHandler(void)
{   EDMA_1_CH10_DriverIRQHandler();
}

WEAK void EDMA_1_CH11_IRQHandler(void)
{   EDMA_1_CH11_DriverIRQHandler();
}

WEAK void EDMA_1_CH12_IRQHandler(void)
{   EDMA_1_CH12_DriverIRQHandler();
}

WEAK void EDMA_1_CH13_IRQHandler(void)
{   EDMA_1_CH13_DriverIRQHandler();
}

WEAK void EDMA_1_CH14_IRQHandler(void)
{   EDMA_1_CH14_DriverIRQHandler();
}

WEAK void EDMA_1_CH15_IRQHandler(void)
{   EDMA_1_CH15_DriverIRQHandler();
}

WEAK void CDOG0_IRQHandler(void)
{   CDOG0_DriverIRQHandler();
}

WEAK void CDOG1_IRQHandler(void)
{   CDOG1_DriverIRQHandler();
}

WEAK void I3C0_IRQHandler(void)
{   I3C0_DriverIRQHandler();
}

WEAK void I3C1_IRQHandler(void)
{   I3C1_DriverIRQHandler();
}

WEAK void NPU_IRQHandler(void)
{   NPU_DriverIRQHandler();
}

WEAK void GDET_IRQHandler(void)
{   GDET_DriverIRQHandler();
}

WEAK void VBAT0_IRQHandler(void)
{   VBAT0_DriverIRQHandler();
}

WEAK void EWM0_IRQHandler(void)
{   EWM0_DriverIRQHandler();
}

WEAK void TSI_END_OF_SCAN_IRQHandler(void)
{   TSI_END_OF_SCAN_DriverIRQHandler();
}

WEAK void TSI_OUT_OF_SCAN_IRQHandler(void)
{   TSI_OUT_OF_SCAN_DriverIRQHandler();
}

WEAK void EMVSIM0_IRQHandler(void)
{   EMVSIM0_DriverIRQHandler();
}

WEAK void EMVSIM1_IRQHandler(void)
{   EMVSIM1_DriverIRQHandler();
}

WEAK void FLEXIO_IRQHandler(void)
{   FLEXIO_DriverIRQHandler();
}

WEAK void DAC0_IRQHandler(void)
{   DAC0_DriverIRQHandler();
}

WEAK void DAC1_IRQHandler(void)
{   DAC1_DriverIRQHandler();
}

WEAK void DAC2_IRQHandler(void)
{   DAC2_DriverIRQHandler();
}

WEAK void HSCMP0_IRQHandler(void)
{   HSCMP0_DriverIRQHandler();
}

WEAK void HSCMP1_IRQHandler(void)
{   HSCMP1_DriverIRQHandler();
}

WEAK void HSCMP2_IRQHandler(void)
{   HSCMP2_DriverIRQHandler();
}

WEAK void FLEXPWM0_RELOAD_ERROR_IRQHandler(void)
{   FLEXPWM0_RELOAD_ERROR_DriverIRQHandler();
}

WEAK void FLEXPWM0_FAULT_IRQHandler(void)
{   FLEXPWM0_FAULT_DriverIRQHandler();
}

WEAK void FLEXPWM0_SUBMODULE0_IRQHandler(void)
{   FLEXPWM0_SUBMODULE0_DriverIRQHandler();
}

WEAK void FLEXPWM0_SUBMODULE1_IRQHandler(void)
{   FLEXPWM0_SUBMODULE1_DriverIRQHandler();
}

WEAK void FLEXPWM0_SUBMODULE2_IRQHandler(void)
{   FLEXPWM0_SUBMODULE2_DriverIRQHandler();
}

WEAK void FLEXPWM0_SUBMODULE3_IRQHandler(void)
{   FLEXPWM0_SUBMODULE3_DriverIRQHandler();
}

WEAK void FLEXPWM1_RELOAD_ERROR_IRQHandler(void)
{   FLEXPWM1_RELOAD_ERROR_DriverIRQHandler();
}

WEAK void FLEXPWM1_FAULT_IRQHandler(void)
{   FLEXPWM1_FAULT_DriverIRQHandler();
}

WEAK void FLEXPWM1_SUBMODULE0_IRQHandler(void)
{   FLEXPWM1_SUBMODULE0_DriverIRQHandler();
}

WEAK void FLEXPWM1_SUBMODULE1_IRQHandler(void)
{   FLEXPWM1_SUBMODULE1_DriverIRQHandler();
}

WEAK void FLEXPWM1_SUBMODULE2_IRQHandler(void)
{   FLEXPWM1_SUBMODULE2_DriverIRQHandler();
}

WEAK void FLEXPWM1_SUBMODULE3_IRQHandler(void)
{   FLEXPWM1_SUBMODULE3_DriverIRQHandler();
}

WEAK void ENC0_COMPARE_IRQHandler(void)
{   ENC0_COMPARE_DriverIRQHandler();
}

WEAK void ENC0_HOME_IRQHandler(void)
{   ENC0_HOME_DriverIRQHandler();
}

WEAK void ENC0_WDG_SAB_IRQHandler(void)
{   ENC0_WDG_SAB_DriverIRQHandler();
}

WEAK void ENC0_IDX_IRQHandler(void)
{   ENC0_IDX_DriverIRQHandler();
}

WEAK void ENC1_COMPARE_IRQHandler(void)
{   ENC1_COMPARE_DriverIRQHandler();
}

WEAK void ENC1_HOME_IRQHandler(void)
{   ENC1_HOME_DriverIRQHandler();
}

WEAK void ENC1_WDG_SAB_IRQHandler(void)
{   ENC1_WDG_SAB_DriverIRQHandler();
}

WEAK void ENC1_IDX_IRQHandler(void)
{   ENC1_IDX_DriverIRQHandler();
}

WEAK void ITRC0_IRQHandler(void)
{   ITRC0_DriverIRQHandler();
}

WEAK void BSP32_IRQHandler(void)
{   BSP32_DriverIRQHandler();
}

WEAK void ELS_ERR_IRQHandler(void)
{   ELS_ERR_DriverIRQHandler();
}

WEAK void PKC_ERR_IRQHandler(void)
{   PKC_ERR_DriverIRQHandler();
}

WEAK void ERM_SINGLE_BIT_ERROR_IRQHandler(void)
{   ERM_SINGLE_BIT_ERROR_DriverIRQHandler();
}

WEAK void ERM_MULTI_BIT_ERROR_IRQHandler(void)
{   ERM_MULTI_BIT_ERROR_DriverIRQHandler();
}

WEAK void FMU0_IRQHandler(void)
{   FMU0_DriverIRQHandler();
}

WEAK void ETHERNET_IRQHandler(void)
{   ETHERNET_DriverIRQHandler();
}

WEAK void ETHERNET_PMT_IRQHandler(void)
{   ETHERNET_PMT_DriverIRQHandler();
}

WEAK void ETHERNET_MACLP_IRQHandler(void)
{   ETHERNET_MACLP_DriverIRQHandler();
}

WEAK void SINC_FILTER_IRQHandler(void)
{   SINC_FILTER_DriverIRQHandler();
}

WEAK void LPTMR0_IRQHandler(void)
{   LPTMR0_DriverIRQHandler();
}

WEAK void LPTMR1_IRQHandler(void)
{   LPTMR1_DriverIRQHandler();
}

WEAK void SCG_IRQHandler(void)
{   SCG_DriverIRQHandler();
}

WEAK void SPC_IRQHandler(void)
{   SPC_DriverIRQHandler();
}

WEAK void WUU_IRQHandler(void)
{   WUU_DriverIRQHandler();
}

WEAK void PORT_EFT_IRQHandler(void)
{   PORT_EFT_DriverIRQHandler();
}

WEAK void ETB0_IRQHandler(void)
{   ETB0_DriverIRQHandler();
}

WEAK void SM3_IRQHandler(void)
{   SM3_DriverIRQHandler();
}

WEAK void TRNG0_IRQHandler(void)
{   TRNG0_DriverIRQHandler();
}

WEAK void WWDT0_IRQHandler(void)
{   WWDT0_DriverIRQHandler();
}

WEAK void WWDT1_IRQHandler(void)
{   WWDT1_DriverIRQHandler();
}

WEAK void CMC0_IRQHandler(void)
{   CMC0_DriverIRQHandler();
}

WEAK void CTI0_IRQHandler(void)
{   CTI0_DriverIRQHandler();
}

//*****************************************************************************

#if defined (DEBUG)
#pragma GCC pop_options
#endif // (DEBUG)
