//*****************************************************************************
// RW612 startup code for use with MCUXpresso IDE
//
// Version : 120324
//*****************************************************************************
//
// Copyright 2016-2024 NXP
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
WEAK void WDT0_IRQHandler(void);
WEAK void DMA0_IRQHandler(void);
WEAK void GPIO_INTA_IRQHandler(void);
WEAK void GPIO_INTB_IRQHandler(void);
WEAK void PIN_INT0_IRQHandler(void);
WEAK void PIN_INT1_IRQHandler(void);
WEAK void PIN_INT2_IRQHandler(void);
WEAK void PIN_INT3_IRQHandler(void);
WEAK void UTICK_IRQHandler(void);
WEAK void MRT_IRQHandler(void);
WEAK void CTIMER0_IRQHandler(void);
WEAK void CTIMER1_IRQHandler(void);
WEAK void SCT0_IRQHandler(void);
WEAK void CTIMER3_IRQHandler(void);
WEAK void FLEXCOMM0_IRQHandler(void);
WEAK void FLEXCOMM1_IRQHandler(void);
WEAK void FLEXCOMM2_IRQHandler(void);
WEAK void FLEXCOMM3_IRQHandler(void);
WEAK void Reserved34_IRQHandler(void);
WEAK void Reserved35_IRQHandler(void);
WEAK void FLEXCOMM14_IRQHandler(void);
WEAK void Reserved37_IRQHandler(void);
WEAK void Reserved38_IRQHandler(void);
WEAK void GFMRT_IRQHandler(void);
WEAK void Reserved40_IRQHandler(void);
WEAK void DMIC_IRQHandler(void);
WEAK void WKDEEPSLEEP_IRQHandler(void);
WEAK void HYPERVISOR_IRQHandler(void);
WEAK void SECUREVIOLATION_IRQHandler(void);
WEAK void HWVAD_IRQHandler(void);
WEAK void Reserved46_IRQHandler(void);
WEAK void Reserved47_IRQHandler(void);
WEAK void RTC_IRQHandler(void);
WEAK void Reserved49_IRQHandler(void);
WEAK void Reserved50_IRQHandler(void);
WEAK void PIN_INT4_IRQHandler(void);
WEAK void PIN_INT5_IRQHandler(void);
WEAK void PIN_INT6_IRQHandler(void);
WEAK void PIN_INT7_IRQHandler(void);
WEAK void CTIMER2_IRQHandler(void);
WEAK void CTIMER4_IRQHandler(void);
WEAK void OS_EVENT_TIMER_IRQHandler(void);
WEAK void FLEXSPI_IRQHandler(void);
WEAK void Reserved59_IRQHandler(void);
WEAK void Reserved60_IRQHandler(void);
WEAK void Reserved61_IRQHandler(void);
WEAK void SDIO_IRQHandler(void);
WEAK void SGPIO_INTA_IRQHandler(void);
WEAK void SGPIO_INTB_IRQHandler(void);
WEAK void Reserved65_IRQHandler(void);
WEAK void USB_IRQHandler(void);
WEAK void Reserved67_IRQHandler(void);
WEAK void Reserved68_IRQHandler(void);
WEAK void Reserved69_IRQHandler(void);
WEAK void DMA1_IRQHandler(void);
WEAK void PUF_IRQHandler(void);
WEAK void POWERQUAD_IRQHandler(void);
WEAK void Reserved73_IRQHandler(void);
WEAK void Reserved74_IRQHandler(void);
WEAK void Reserved75_IRQHandler(void);
WEAK void Reserved76_IRQHandler(void);
WEAK void LCD_IRQHandler(void);
WEAK void CAPTIMER_IRQHandler(void);
WEAK void Reserved79_IRQHandler(void);
WEAK void W2MWKUP_DONE0_IRQHandler(void);
WEAK void W2MWKUP_DONE1_IRQHandler(void);
WEAK void W2MWKUP_DONE2_IRQHandler(void);
WEAK void W2MWKUP_DONE3_IRQHandler(void);
WEAK void W2MWKUP_DONE4_IRQHandler(void);
WEAK void W2MWKUP_DONE5_IRQHandler(void);
WEAK void W2MWKUP_DONE6_IRQHandler(void);
WEAK void W2MWKUP_DONE7_IRQHandler(void);
WEAK void W2MWKUP0_IRQHandler(void);
WEAK void W2MWKUP1_IRQHandler(void);
WEAK void WL_MCI_INT0_IRQHandler(void);
WEAK void WL_MCI_INT1_IRQHandler(void);
WEAK void WL_MCI_INT2_IRQHandler(void);
WEAK void WL_MCI_INT3_IRQHandler(void);
WEAK void WL_MCI_INT4_IRQHandler(void);
WEAK void WL_MCI_INT5_IRQHandler(void);
WEAK void WL_MCI_INT6_IRQHandler(void);
WEAK void WL_MCI_INT7_IRQHandler(void);
WEAK void B2MWKUP_DONE0_IRQHandler(void);
WEAK void B2MWKUP_DONE1_IRQHandler(void);
WEAK void B2MWKUP_DONE2_IRQHandler(void);
WEAK void B2MWKUP_DONE3_IRQHandler(void);
WEAK void B2MWKUP_DONE4_IRQHandler(void);
WEAK void B2MWKUP_DONE5_IRQHandler(void);
WEAK void B2MWKUP_DONE6_IRQHandler(void);
WEAK void B2MWKUP_DONE7_IRQHandler(void);
WEAK void B2MWKUP0_IRQHandler(void);
WEAK void B2MWKUP1_IRQHandler(void);
WEAK void BLE_MCI_INT0_IRQHandler(void);
WEAK void BLE_MCI_INT1_IRQHandler(void);
WEAK void BLE_MCI_INT2_IRQHandler(void);
WEAK void BLE_MCI_INT3_IRQHandler(void);
WEAK void BLE_MCI_INT4_IRQHandler(void);
WEAK void BLE_MCI_INT5_IRQHandler(void);
WEAK void BLE_MCI_INT6_IRQHandler(void);
WEAK void BLE_MCI_INT7_IRQHandler(void);
WEAK void PIN0_INT_IRQHandler(void);
WEAK void PIN1_INT_IRQHandler(void);
WEAK void ELS_IRQHandler(void);
WEAK void ELS_GDET_IRQHandler(void);
WEAK void ELS_GDET_UM_IRQHandler(void);
WEAK void PKC_INT_IRQHandler(void);
WEAK void PKC_ERR_IRQHandler(void);
WEAK void CDOG_INT_IRQHandler(void);
WEAK void GAU_DAC_IRQHandler(void);
WEAK void GAU_ACOMP_WKUP_IRQHandler(void);
WEAK void GAU_ACOMP_IRQHandler(void);
WEAK void GAU_ADC1_IRQHandler(void);
WEAK void GAU_ADC0_IRQHandler(void);
WEAK void USIM_IRQHandler(void);
WEAK void OTP_IRQHandler(void);
WEAK void ENET_IRQHandler(void);
WEAK void ENET_TIMER_IRQHandler(void);
WEAK void PMIP_IRQHandler(void);
WEAK void PMIP_CHANGE_IRQHandler(void);
WEAK void ITRC_IRQHandler(void);
WEAK void Reserved136_IRQHandler(void);
WEAK void Reserved137_IRQHandler(void);
WEAK void Reserved138_IRQHandler(void);
WEAK void TRNG_IRQHandler(void);
WEAK void ACC_C_INT_IRQHandler(void);
WEAK void ACC_S_INT_IRQHandler(void);
WEAK void WACC_IRQHandler(void);
WEAK void BACC_IRQHandler(void);
WEAK void GDMA_IRQHandler(void);

//*****************************************************************************
// Forward declaration of the driver IRQ handlers. These are aliased
// to the IntDefaultHandler, which is a 'forever' loop. When the driver
// defines a handler (with the same name), this will automatically take
// precedence over these weak definitions
//*****************************************************************************
void WDT0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO_INTA_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GPIO_INTB_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PIN_INT0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PIN_INT1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PIN_INT2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PIN_INT3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UTICK_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void MRT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CTIMER0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CTIMER1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SCT0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CTIMER3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXCOMM0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXCOMM1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXCOMM2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXCOMM3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved34_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved35_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXCOMM14_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved37_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved38_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GFMRT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved40_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMIC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void WKDEEPSLEEP_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void HYPERVISOR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SECUREVIOLATION_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void HWVAD_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved46_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved47_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void RTC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved49_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved50_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PIN_INT4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PIN_INT5_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PIN_INT6_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PIN_INT7_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CTIMER2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CTIMER4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void OS_EVENT_TIMER_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXSPI_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved59_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved60_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved61_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SDIO_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SGPIO_INTA_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SGPIO_INTB_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved65_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void USB_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved67_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved68_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved69_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PUF_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void POWERQUAD_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved73_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved74_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved75_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved76_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LCD_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CAPTIMER_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved79_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void W2MWKUP_DONE0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void W2MWKUP_DONE1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void W2MWKUP_DONE2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void W2MWKUP_DONE3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void W2MWKUP_DONE4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void W2MWKUP_DONE5_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void W2MWKUP_DONE6_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void W2MWKUP_DONE7_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void W2MWKUP0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void W2MWKUP1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void WL_MCI_INT0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void WL_MCI_INT1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void WL_MCI_INT2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void WL_MCI_INT3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void WL_MCI_INT4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void WL_MCI_INT5_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void WL_MCI_INT6_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void WL_MCI_INT7_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void B2MWKUP_DONE0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void B2MWKUP_DONE1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void B2MWKUP_DONE2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void B2MWKUP_DONE3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void B2MWKUP_DONE4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void B2MWKUP_DONE5_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void B2MWKUP_DONE6_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void B2MWKUP_DONE7_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void B2MWKUP0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void B2MWKUP1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void BLE_MCI_INT0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void BLE_MCI_INT1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void BLE_MCI_INT2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void BLE_MCI_INT3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void BLE_MCI_INT4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void BLE_MCI_INT5_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void BLE_MCI_INT6_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void BLE_MCI_INT7_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PIN0_INT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PIN1_INT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ELS_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ELS_GDET_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ELS_GDET_UM_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PKC_INT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PKC_ERR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CDOG_INT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GAU_DAC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GAU_ACOMP_WKUP_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GAU_ACOMP_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GAU_ADC1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GAU_ADC0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void USIM_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void OTP_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ENET_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ENET_TIMER_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PMIP_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PMIP_CHANGE_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ITRC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved136_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved137_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved138_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TRNG_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ACC_C_INT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ACC_S_INT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void WACC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void BACC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void GDMA_DriverIRQHandler(void) ALIAS(IntDefaultHandler);

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
extern void _image_size(void);
//*****************************************************************************
// External declaration for the pointer to the stack base from the Linker Script
//*****************************************************************************
extern void _vStackBase(void);
//*****************************************************************************
// External declaration for image type and load address from  Linker Script
//*****************************************************************************
WEAK extern void __imghdr_loadaddress();
WEAK extern void __imghdr_imagetype();

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
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
    (void (*)())0x100000,                // Image length
#else
    (void (*)())((unsigned)_image_size), // Image length
#endif
    __imghdr_imagetype,                // Image type
    0,                                 // Reserved
    SVC_Handler,                       // SVCall Handler
    DebugMon_Handler,                  // Debug Monitor Handler
    (void (*)())g_pfnVectors,          // Image load address
    PendSV_Handler,                    // PendSV Handler
    SysTick_Handler,                   // SysTick Handler

    // Chip Level - RW612
    WDT0_IRQHandler,             // 16 : Windowed watchdog timer 0 (WDT0)(Cortex-M33 watchdog)
    DMA0_IRQHandler,             // 17 : Direct memory access (DMA) controller 0 (secure or Cortex-M33 DMA)
    GPIO_INTA_IRQHandler,        // 18 : GPIO interrupt A
    GPIO_INTB_IRQHandler,        // 19 : GPIO interrupt B
    PIN_INT0_IRQHandler,         // 20 : Pin interrupt 0 or pattern match engine slice 0
    PIN_INT1_IRQHandler,         // 21 : Pin interrupt 1 or pattern match engine slice 1
    PIN_INT2_IRQHandler,         // 22 : Pin interrupt 2 or pattern match engine slice 2
    PIN_INT3_IRQHandler,         // 23 : Pin interrupt 3 or pattern match engine slice 3
    UTICK_IRQHandler,            // 24 : Micro-tick Timer (UTICK)
    MRT_IRQHandler,              // 25 : Multi-Rate Timer (MRT). Global MRT interrupts
    CTIMER0_IRQHandler,          // 26 : Standard counter/timer CTIMER0
    CTIMER1_IRQHandler,          // 27 : Standard counter/timer CTIMER1
    SCT0_IRQHandler,             // 28 : SCTimer/PWM
    CTIMER3_IRQHandler,          // 29 : Standard counter/timer CTIMER3
    FLEXCOMM0_IRQHandler,        // 30 : Flexcomm Interface 0 (USART, SPI, I2C, I2S)
    FLEXCOMM1_IRQHandler,        // 31 : Flexcomm Interface 1 (USART, SPI, I2C, I2S)
    FLEXCOMM2_IRQHandler,        // 32 : Flexcomm Interface 2 (USART, SPI, I2C, I2S)
    FLEXCOMM3_IRQHandler,        // 33 : Flexcomm Interface 3 (USART, SPI, I2C, I2S)
    Reserved34_IRQHandler,       // 34 : Reserved interrupt
    Reserved35_IRQHandler,       // 35 : Reserved interrupt
    FLEXCOMM14_IRQHandler,       // 36 : Flexcomm Interface 14 (USART, SPI, I2C, I2S)
    Reserved37_IRQHandler,       // 37 : Reserved interrupt
    Reserved38_IRQHandler,       // 38 : Reserved interrupt
    GFMRT_IRQHandler,            // 39 : Free Multi-rate timer (GFMRT). Global MRT interrupts
    Reserved40_IRQHandler,       // 40 : Reserved interrupt
    DMIC_IRQHandler,             // 41 : Digital microphone (DMIC) and DMIC subsystem
    WKDEEPSLEEP_IRQHandler,      // 42 : Wake-up from deep sleep
    HYPERVISOR_IRQHandler,       // 43 : Hypervisor service software interrupt
    SECUREVIOLATION_IRQHandler,  // 44 : Secure violation
    HWVAD_IRQHandler,            // 45 : Hardware Voice Activity Detector
    Reserved46_IRQHandler,       // 46 : Reserved interrupt
    Reserved47_IRQHandler,       // 47 : Reserved interrupt
    RTC_IRQHandler,              // 48 : RTC alarm and wake-up
    Reserved49_IRQHandler,       // 49 : Reserved interrupt
    Reserved50_IRQHandler,       // 50 : Reserved interrupt
    PIN_INT4_IRQHandler,         // 51 : Pin interrupt 4 or pattern match engine slice 4
    PIN_INT5_IRQHandler,         // 52 : Pin interrupt 5 or pattern match engine slice 5
    PIN_INT6_IRQHandler,         // 53 : Pin interrupt 6 or pattern match engine slice 6
    PIN_INT7_IRQHandler,         // 54 : Pin interrupt 7 or pattern match engine slice 7
    CTIMER2_IRQHandler,          // 55 : Standard counter/timer CTIMER2
    CTIMER4_IRQHandler,          // 56 : Standard counter/timer CTIMER4
    OS_EVENT_TIMER_IRQHandler,   // 57 : OS event timer 0
    FLEXSPI_IRQHandler,          // 58 : FLEXSPI interface
    Reserved59_IRQHandler,       // 59 : Reserved interrupt
    Reserved60_IRQHandler,       // 60 : Reserved interrupt
    Reserved61_IRQHandler,       // 61 : Reserved interrupt
    SDIO_IRQHandler,             // 62 : The secure digital interface
    SGPIO_INTA_IRQHandler,       // 63 : Secure GPIO interrupt A
    SGPIO_INTB_IRQHandler,       // 64 : Secure GPIO interrupt B
    Reserved65_IRQHandler,       // 65 : Reserved interrupt
    USB_IRQHandler,              // 66 : High-speed USB device/host
    Reserved67_IRQHandler,       // 67 : Reserved interrupt
    Reserved68_IRQHandler,       // 68 : Reserved interrupt
    Reserved69_IRQHandler,       // 69 : Reserved interrupt
    DMA1_IRQHandler,             // 70 : DMA controller 1 (non-secure or HiFi 4 DMA)
    PUF_IRQHandler,              // 71 : Physical Unclonable Function
    POWERQUAD_IRQHandler,        // 72 : PowerQuad math coprocessor
    Reserved73_IRQHandler,       // 73 : Reserved interrupt
    Reserved74_IRQHandler,       // 74 : Reserved interrupt
    Reserved75_IRQHandler,       // 75 : Reserved interrupt
    Reserved76_IRQHandler,       // 76 : Reserved interrupt
    LCD_IRQHandler,              // 77 : LCDIC
    CAPTIMER_IRQHandler,         // 78 : Capture timer
    Reserved79_IRQHandler,       // 79 : Reserved interrupt
    W2MWKUP_DONE0_IRQHandler,    // 80 : Wi-Fi to MCU, wakeup done 0
    W2MWKUP_DONE1_IRQHandler,    // 81 : Wi-Fi to MCU, wakeup done 1
    W2MWKUP_DONE2_IRQHandler,    // 82 : Wi-Fi to MCU, wakeup done 2
    W2MWKUP_DONE3_IRQHandler,    // 83 : Wi-Fi to MCU, wakeup done 3
    W2MWKUP_DONE4_IRQHandler,    // 84 : Wi-Fi to MCU, wakeup done 4
    W2MWKUP_DONE5_IRQHandler,    // 85 : Wi-Fi to MCU, wakeup done 5
    W2MWKUP_DONE6_IRQHandler,    // 86 : Wi-Fi to MCU, wakeup done 6
    W2MWKUP_DONE7_IRQHandler,    // 87 : Wi-Fi to MCU, wakeup done 7
    W2MWKUP0_IRQHandler,         // 88 : Wi-Fi to MCU, wakeup signal 0
    W2MWKUP1_IRQHandler,         // 89 : Wi-Fi to MCU, wakueup signal 1
    WL_MCI_INT0_IRQHandler,      // 90 : Wi-Fi to MCU interrupt 0
    WL_MCI_INT1_IRQHandler,      // 91 : Reserved for Wi-Fi to MCU
    WL_MCI_INT2_IRQHandler,      // 92 : Reserved for Wi-Fi to MCU
    WL_MCI_INT3_IRQHandler,      // 93 : Reserved for Wi-Fi to MCU
    WL_MCI_INT4_IRQHandler,      // 94 : Reserved for Wi-Fi to MCU
    WL_MCI_INT5_IRQHandler,      // 95 : Reserved for Wi-Fi to MCU
    WL_MCI_INT6_IRQHandler,      // 96 : Reserved for Wi-Fi to MCU
    WL_MCI_INT7_IRQHandler,      // 97 : Reserved for Wi-Fi to MCU
    B2MWKUP_DONE0_IRQHandler,    // 98 : Bluetooth LE/802.15.4 radio to MCU, wakeup done 0
    B2MWKUP_DONE1_IRQHandler,    // 99 : Bluetooth LE/802.15.4 radio to MCU, wakeup done 1
    B2MWKUP_DONE2_IRQHandler,    // 100: Bluetooth LE/802.15.4 radio to MCU, wakeup done 2
    B2MWKUP_DONE3_IRQHandler,    // 101: Bluetooth LE/802.15.4 radio to MCU, wakeup done 3
    B2MWKUP_DONE4_IRQHandler,    // 102: Bluetooth LE/802.15.4 radio to MCU, wakeup done 4
    B2MWKUP_DONE5_IRQHandler,    // 103: Bluetooth LE/802.15.4 radio to MCU, wakeup done 5
    B2MWKUP_DONE6_IRQHandler,    // 104: Bluetooth LE/802.15.4 radio to MCU, wakeup done 6
    B2MWKUP_DONE7_IRQHandler,    // 105: Bluetooth LE/802.15.4 radio to MCU, wakeup done 7
    B2MWKUP0_IRQHandler,         // 106: Bluetooth LE/802.15.4 radio to MCU, wakeup signal 0
    B2MWKUP1_IRQHandler,         // 107: Bluetooth LE/802.15.4 radio to MCU, wakeup signal 1
    BLE_MCI_INT0_IRQHandler,     // 108: Bluetooth LE/802.15.4 radio to MCU interrupt 0
    BLE_MCI_INT1_IRQHandler,     // 109: Reserved for Bluetooth LE/802.15.4 radio to MCU
    BLE_MCI_INT2_IRQHandler,     // 110: Reserved for Bluetooth LE/802.15.4 radio to MCU
    BLE_MCI_INT3_IRQHandler,     // 111: Reserved for Bluetooth LE/802.15.4 radio to MCU
    BLE_MCI_INT4_IRQHandler,     // 112: Reserved for Bluetooth LE/802.15.4 radio to MCU
    BLE_MCI_INT5_IRQHandler,     // 113: Reserved for Bluetooth LE/802.15.4 radio to MCU
    BLE_MCI_INT6_IRQHandler,     // 114: Reserved for Bluetooth LE/802.15.4 radio to MCU
    BLE_MCI_INT7_IRQHandler,     // 115: Reserved for Bluetooth LE/802.15.4 radio to MCU
    PIN0_INT_IRQHandler,         // 116: From AON GPIO
    PIN1_INT_IRQHandler,         // 117: From AON GPIO
    ELS_IRQHandler,              // 118: EdgeLock subsystem (ELS)
    ELS_GDET_IRQHandler,         // 119: ELS IRQ line for GDET error
    ELS_GDET_UM_IRQHandler,      // 120: ELS un-gated latched error
    PKC_INT_IRQHandler,          // 121: Public key crypto-processor (PKC) interrupt
    PKC_ERR_IRQHandler,          // 122: PKC error
    CDOG_INT_IRQHandler,         // 123: Code watch dog timmer interrupt
    GAU_DAC_IRQHandler,          // 124: General analog unit (GAU) digital to analog converter (DAC)
    GAU_ACOMP_WKUP_IRQHandler,   // 125: GAU analog comparator (ACOMP) wake-up
    GAU_ACOMP_IRQHandler,        // 126: GAU analog comparator
    GAU_ADC1_IRQHandler,         // 127: GAU analog to digital converter 1 (ADC1)
    GAU_ADC0_IRQHandler,         // 128: GAU analog to digital converter 0 (ADC0)
    USIM_IRQHandler,             // 129: Universal subscriber identity module (USIM) interface
    OTP_IRQHandler,              // 130: One time programmable (OTP) memory interrupt
    ENET_IRQHandler,             // 131: Etheret interrupt
    ENET_TIMER_IRQHandler,       // 132: Ethernet timer interrupt
    PMIP_IRQHandler,             // 133: Power management IP (PMIP)
    PMIP_CHANGE_IRQHandler,      // 134: PMIP change from 1 to 0
    ITRC_IRQHandler,             // 135: Intrusion and tamper response controller (ITRC) interrupt request
    Reserved136_IRQHandler,      // 136: Reserved interrupt
    Reserved137_IRQHandler,      // 137: Reserved interrupt
    Reserved138_IRQHandler,      // 138: Reserved interrupt
    TRNG_IRQHandler,             // 139: TRNG interrupt request
    ACC_C_INT_IRQHandler,        // 140: AHB memory access checker - Cortex-M33 code bus
    ACC_S_INT_IRQHandler,        // 141: AHB memory access checker - Cortex-M33 sys bus
    WACC_IRQHandler,             // 142: Wi-Fi accessed during power off
    BACC_IRQHandler,             // 143: Bluetooth LE/802.15.4 radio accessed during power off
    GDMA_IRQHandler,             // 144: General purpose direct memory access (GDMA) interrupt
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
WEAK void WDT0_IRQHandler(void)
{   WDT0_DriverIRQHandler();
}

WEAK void DMA0_IRQHandler(void)
{   DMA0_DriverIRQHandler();
}

WEAK void GPIO_INTA_IRQHandler(void)
{   GPIO_INTA_DriverIRQHandler();
}

WEAK void GPIO_INTB_IRQHandler(void)
{   GPIO_INTB_DriverIRQHandler();
}

WEAK void PIN_INT0_IRQHandler(void)
{   PIN_INT0_DriverIRQHandler();
}

WEAK void PIN_INT1_IRQHandler(void)
{   PIN_INT1_DriverIRQHandler();
}

WEAK void PIN_INT2_IRQHandler(void)
{   PIN_INT2_DriverIRQHandler();
}

WEAK void PIN_INT3_IRQHandler(void)
{   PIN_INT3_DriverIRQHandler();
}

WEAK void UTICK_IRQHandler(void)
{   UTICK_DriverIRQHandler();
}

WEAK void MRT_IRQHandler(void)
{   MRT_DriverIRQHandler();
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

WEAK void CTIMER3_IRQHandler(void)
{   CTIMER3_DriverIRQHandler();
}

WEAK void FLEXCOMM0_IRQHandler(void)
{   FLEXCOMM0_DriverIRQHandler();
}

WEAK void FLEXCOMM1_IRQHandler(void)
{   FLEXCOMM1_DriverIRQHandler();
}

WEAK void FLEXCOMM2_IRQHandler(void)
{   FLEXCOMM2_DriverIRQHandler();
}

WEAK void FLEXCOMM3_IRQHandler(void)
{   FLEXCOMM3_DriverIRQHandler();
}

WEAK void Reserved34_IRQHandler(void)
{   Reserved34_DriverIRQHandler();
}

WEAK void Reserved35_IRQHandler(void)
{   Reserved35_DriverIRQHandler();
}

WEAK void FLEXCOMM14_IRQHandler(void)
{   FLEXCOMM14_DriverIRQHandler();
}

WEAK void Reserved37_IRQHandler(void)
{   Reserved37_DriverIRQHandler();
}

WEAK void Reserved38_IRQHandler(void)
{   Reserved38_DriverIRQHandler();
}

WEAK void GFMRT_IRQHandler(void)
{   GFMRT_DriverIRQHandler();
}

WEAK void Reserved40_IRQHandler(void)
{   Reserved40_DriverIRQHandler();
}

WEAK void DMIC_IRQHandler(void)
{   DMIC_DriverIRQHandler();
}

WEAK void WKDEEPSLEEP_IRQHandler(void)
{   WKDEEPSLEEP_DriverIRQHandler();
}

WEAK void HYPERVISOR_IRQHandler(void)
{   HYPERVISOR_DriverIRQHandler();
}

WEAK void SECUREVIOLATION_IRQHandler(void)
{   SECUREVIOLATION_DriverIRQHandler();
}

WEAK void HWVAD_IRQHandler(void)
{   HWVAD_DriverIRQHandler();
}

WEAK void Reserved46_IRQHandler(void)
{   Reserved46_DriverIRQHandler();
}

WEAK void Reserved47_IRQHandler(void)
{   Reserved47_DriverIRQHandler();
}

WEAK void RTC_IRQHandler(void)
{   RTC_DriverIRQHandler();
}

WEAK void Reserved49_IRQHandler(void)
{   Reserved49_DriverIRQHandler();
}

WEAK void Reserved50_IRQHandler(void)
{   Reserved50_DriverIRQHandler();
}

WEAK void PIN_INT4_IRQHandler(void)
{   PIN_INT4_DriverIRQHandler();
}

WEAK void PIN_INT5_IRQHandler(void)
{   PIN_INT5_DriverIRQHandler();
}

WEAK void PIN_INT6_IRQHandler(void)
{   PIN_INT6_DriverIRQHandler();
}

WEAK void PIN_INT7_IRQHandler(void)
{   PIN_INT7_DriverIRQHandler();
}

WEAK void CTIMER2_IRQHandler(void)
{   CTIMER2_DriverIRQHandler();
}

WEAK void CTIMER4_IRQHandler(void)
{   CTIMER4_DriverIRQHandler();
}

WEAK void OS_EVENT_TIMER_IRQHandler(void)
{   OS_EVENT_TIMER_DriverIRQHandler();
}

WEAK void FLEXSPI_IRQHandler(void)
{   FLEXSPI_DriverIRQHandler();
}

WEAK void Reserved59_IRQHandler(void)
{   Reserved59_DriverIRQHandler();
}

WEAK void Reserved60_IRQHandler(void)
{   Reserved60_DriverIRQHandler();
}

WEAK void Reserved61_IRQHandler(void)
{   Reserved61_DriverIRQHandler();
}

WEAK void SDIO_IRQHandler(void)
{   SDIO_DriverIRQHandler();
}

WEAK void SGPIO_INTA_IRQHandler(void)
{   SGPIO_INTA_DriverIRQHandler();
}

WEAK void SGPIO_INTB_IRQHandler(void)
{   SGPIO_INTB_DriverIRQHandler();
}

WEAK void Reserved65_IRQHandler(void)
{   Reserved65_DriverIRQHandler();
}

WEAK void USB_IRQHandler(void)
{   USB_DriverIRQHandler();
}

WEAK void Reserved67_IRQHandler(void)
{   Reserved67_DriverIRQHandler();
}

WEAK void Reserved68_IRQHandler(void)
{   Reserved68_DriverIRQHandler();
}

WEAK void Reserved69_IRQHandler(void)
{   Reserved69_DriverIRQHandler();
}

WEAK void DMA1_IRQHandler(void)
{   DMA1_DriverIRQHandler();
}

WEAK void PUF_IRQHandler(void)
{   PUF_DriverIRQHandler();
}

WEAK void POWERQUAD_IRQHandler(void)
{   POWERQUAD_DriverIRQHandler();
}

WEAK void Reserved73_IRQHandler(void)
{   Reserved73_DriverIRQHandler();
}

WEAK void Reserved74_IRQHandler(void)
{   Reserved74_DriverIRQHandler();
}

WEAK void Reserved75_IRQHandler(void)
{   Reserved75_DriverIRQHandler();
}

WEAK void Reserved76_IRQHandler(void)
{   Reserved76_DriverIRQHandler();
}

WEAK void LCD_IRQHandler(void)
{   LCD_DriverIRQHandler();
}

WEAK void CAPTIMER_IRQHandler(void)
{   CAPTIMER_DriverIRQHandler();
}

WEAK void Reserved79_IRQHandler(void)
{   Reserved79_DriverIRQHandler();
}

WEAK void W2MWKUP_DONE0_IRQHandler(void)
{   W2MWKUP_DONE0_DriverIRQHandler();
}

WEAK void W2MWKUP_DONE1_IRQHandler(void)
{   W2MWKUP_DONE1_DriverIRQHandler();
}

WEAK void W2MWKUP_DONE2_IRQHandler(void)
{   W2MWKUP_DONE2_DriverIRQHandler();
}

WEAK void W2MWKUP_DONE3_IRQHandler(void)
{   W2MWKUP_DONE3_DriverIRQHandler();
}

WEAK void W2MWKUP_DONE4_IRQHandler(void)
{   W2MWKUP_DONE4_DriverIRQHandler();
}

WEAK void W2MWKUP_DONE5_IRQHandler(void)
{   W2MWKUP_DONE5_DriverIRQHandler();
}

WEAK void W2MWKUP_DONE6_IRQHandler(void)
{   W2MWKUP_DONE6_DriverIRQHandler();
}

WEAK void W2MWKUP_DONE7_IRQHandler(void)
{   W2MWKUP_DONE7_DriverIRQHandler();
}

WEAK void W2MWKUP0_IRQHandler(void)
{   W2MWKUP0_DriverIRQHandler();
}

WEAK void W2MWKUP1_IRQHandler(void)
{   W2MWKUP1_DriverIRQHandler();
}

WEAK void WL_MCI_INT0_IRQHandler(void)
{   WL_MCI_INT0_DriverIRQHandler();
}

WEAK void WL_MCI_INT1_IRQHandler(void)
{   WL_MCI_INT1_DriverIRQHandler();
}

WEAK void WL_MCI_INT2_IRQHandler(void)
{   WL_MCI_INT2_DriverIRQHandler();
}

WEAK void WL_MCI_INT3_IRQHandler(void)
{   WL_MCI_INT3_DriverIRQHandler();
}

WEAK void WL_MCI_INT4_IRQHandler(void)
{   WL_MCI_INT4_DriverIRQHandler();
}

WEAK void WL_MCI_INT5_IRQHandler(void)
{   WL_MCI_INT5_DriverIRQHandler();
}

WEAK void WL_MCI_INT6_IRQHandler(void)
{   WL_MCI_INT6_DriverIRQHandler();
}

WEAK void WL_MCI_INT7_IRQHandler(void)
{   WL_MCI_INT7_DriverIRQHandler();
}

WEAK void B2MWKUP_DONE0_IRQHandler(void)
{   B2MWKUP_DONE0_DriverIRQHandler();
}

WEAK void B2MWKUP_DONE1_IRQHandler(void)
{   B2MWKUP_DONE1_DriverIRQHandler();
}

WEAK void B2MWKUP_DONE2_IRQHandler(void)
{   B2MWKUP_DONE2_DriverIRQHandler();
}

WEAK void B2MWKUP_DONE3_IRQHandler(void)
{   B2MWKUP_DONE3_DriverIRQHandler();
}

WEAK void B2MWKUP_DONE4_IRQHandler(void)
{   B2MWKUP_DONE4_DriverIRQHandler();
}

WEAK void B2MWKUP_DONE5_IRQHandler(void)
{   B2MWKUP_DONE5_DriverIRQHandler();
}

WEAK void B2MWKUP_DONE6_IRQHandler(void)
{   B2MWKUP_DONE6_DriverIRQHandler();
}

WEAK void B2MWKUP_DONE7_IRQHandler(void)
{   B2MWKUP_DONE7_DriverIRQHandler();
}

WEAK void B2MWKUP0_IRQHandler(void)
{   B2MWKUP0_DriverIRQHandler();
}

WEAK void B2MWKUP1_IRQHandler(void)
{   B2MWKUP1_DriverIRQHandler();
}

WEAK void BLE_MCI_INT0_IRQHandler(void)
{   BLE_MCI_INT0_DriverIRQHandler();
}

WEAK void BLE_MCI_INT1_IRQHandler(void)
{   BLE_MCI_INT1_DriverIRQHandler();
}

WEAK void BLE_MCI_INT2_IRQHandler(void)
{   BLE_MCI_INT2_DriverIRQHandler();
}

WEAK void BLE_MCI_INT3_IRQHandler(void)
{   BLE_MCI_INT3_DriverIRQHandler();
}

WEAK void BLE_MCI_INT4_IRQHandler(void)
{   BLE_MCI_INT4_DriverIRQHandler();
}

WEAK void BLE_MCI_INT5_IRQHandler(void)
{   BLE_MCI_INT5_DriverIRQHandler();
}

WEAK void BLE_MCI_INT6_IRQHandler(void)
{   BLE_MCI_INT6_DriverIRQHandler();
}

WEAK void BLE_MCI_INT7_IRQHandler(void)
{   BLE_MCI_INT7_DriverIRQHandler();
}

WEAK void PIN0_INT_IRQHandler(void)
{   PIN0_INT_DriverIRQHandler();
}

WEAK void PIN1_INT_IRQHandler(void)
{   PIN1_INT_DriverIRQHandler();
}

WEAK void ELS_IRQHandler(void)
{   ELS_DriverIRQHandler();
}

WEAK void ELS_GDET_IRQHandler(void)
{   ELS_GDET_DriverIRQHandler();
}

WEAK void ELS_GDET_UM_IRQHandler(void)
{   ELS_GDET_UM_DriverIRQHandler();
}

WEAK void PKC_INT_IRQHandler(void)
{   PKC_INT_DriverIRQHandler();
}

WEAK void PKC_ERR_IRQHandler(void)
{   PKC_ERR_DriverIRQHandler();
}

WEAK void CDOG_INT_IRQHandler(void)
{   CDOG_INT_DriverIRQHandler();
}

WEAK void GAU_DAC_IRQHandler(void)
{   GAU_DAC_DriverIRQHandler();
}

WEAK void GAU_ACOMP_WKUP_IRQHandler(void)
{   GAU_ACOMP_WKUP_DriverIRQHandler();
}

WEAK void GAU_ACOMP_IRQHandler(void)
{   GAU_ACOMP_DriverIRQHandler();
}

WEAK void GAU_ADC1_IRQHandler(void)
{   GAU_ADC1_DriverIRQHandler();
}

WEAK void GAU_ADC0_IRQHandler(void)
{   GAU_ADC0_DriverIRQHandler();
}

WEAK void USIM_IRQHandler(void)
{   USIM_DriverIRQHandler();
}

WEAK void OTP_IRQHandler(void)
{   OTP_DriverIRQHandler();
}

WEAK void ENET_IRQHandler(void)
{   ENET_DriverIRQHandler();
}

WEAK void ENET_TIMER_IRQHandler(void)
{   ENET_TIMER_DriverIRQHandler();
}

WEAK void PMIP_IRQHandler(void)
{   PMIP_DriverIRQHandler();
}

WEAK void PMIP_CHANGE_IRQHandler(void)
{   PMIP_CHANGE_DriverIRQHandler();
}

WEAK void ITRC_IRQHandler(void)
{   ITRC_DriverIRQHandler();
}

WEAK void Reserved136_IRQHandler(void)
{   Reserved136_DriverIRQHandler();
}

WEAK void Reserved137_IRQHandler(void)
{   Reserved137_DriverIRQHandler();
}

WEAK void Reserved138_IRQHandler(void)
{   Reserved138_DriverIRQHandler();
}

WEAK void TRNG_IRQHandler(void)
{   TRNG_DriverIRQHandler();
}

WEAK void ACC_C_INT_IRQHandler(void)
{   ACC_C_INT_DriverIRQHandler();
}

WEAK void ACC_S_INT_IRQHandler(void)
{   ACC_S_INT_DriverIRQHandler();
}

WEAK void WACC_IRQHandler(void)
{   WACC_DriverIRQHandler();
}

WEAK void BACC_IRQHandler(void)
{   BACC_DriverIRQHandler();
}

WEAK void GDMA_IRQHandler(void)
{   GDMA_DriverIRQHandler();
}

//*****************************************************************************

#if defined (DEBUG)
#pragma GCC pop_options
#endif // (DEBUG)
