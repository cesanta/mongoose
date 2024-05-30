#include "MCXN947_cm33_core0.h"

void Reset_Handler(void);    // Defined below
void Dummy_Handler(void);    // Defined below
void SysTick_Handler(void);  // Defined in main.c
void SystemInit(void);       // Defined in main.c, called by reset handler
void _estack(void);          // Defined in link.ld

#define WEAK_ALIAS __attribute__((weak, alias("Default_Handler")))

WEAK_ALIAS void NMI_Handler(void);
WEAK_ALIAS void HardFault_Handler(void);
WEAK_ALIAS void MemManage_Handler(void);
WEAK_ALIAS void BusFault_Handler(void);
WEAK_ALIAS void UsageFault_Handler(void);
WEAK_ALIAS void SecureFault_Handler(void);
WEAK_ALIAS void DebugMon_Handler(void);
WEAK_ALIAS void SVC_Handler(void);
WEAK_ALIAS void PendSV_Handler(void);
WEAK_ALIAS void SysTick_Handler(void);

WEAK_ALIAS void OR_IRQHandler(void);
WEAK_ALIAS void EDMA_0_CH0_IRQHandler(void);
WEAK_ALIAS void EDMA_0_CH1_IRQHandler(void);
WEAK_ALIAS void EDMA_0_CH2_IRQHandler(void);
WEAK_ALIAS void EDMA_0_CH3_IRQHandler(void);
WEAK_ALIAS void EDMA_0_CH4_IRQHandler(void);
WEAK_ALIAS void EDMA_0_CH5_IRQHandler(void);
WEAK_ALIAS void EDMA_0_CH6_IRQHandler(void);
WEAK_ALIAS void EDMA_0_CH7_IRQHandler(void);
WEAK_ALIAS void EDMA_0_CH8_IRQHandler(void);
WEAK_ALIAS void EDMA_0_CH9_IRQHandler(void);
WEAK_ALIAS void EDMA_0_CH10_IRQHandler(void);
WEAK_ALIAS void EDMA_0_CH11_IRQHandler(void);
WEAK_ALIAS void EDMA_0_CH12_IRQHandler(void);
WEAK_ALIAS void EDMA_0_CH13_IRQHandler(void);
WEAK_ALIAS void EDMA_0_CH14_IRQHandler(void);
WEAK_ALIAS void EDMA_0_CH15_IRQHandler(void);
WEAK_ALIAS void GPIO00_IRQHandler(void);
WEAK_ALIAS void GPIO01_IRQHandler(void);
WEAK_ALIAS void GPIO10_IRQHandler(void);
WEAK_ALIAS void GPIO11_IRQHandler(void);
WEAK_ALIAS void GPIO20_IRQHandler(void);
WEAK_ALIAS void GPIO21_IRQHandler(void);
WEAK_ALIAS void GPIO30_IRQHandler(void);
WEAK_ALIAS void GPIO31_IRQHandler(void);
WEAK_ALIAS void GPIO40_IRQHandler(void);
WEAK_ALIAS void GPIO41_IRQHandler(void);
WEAK_ALIAS void GPIO50_IRQHandler(void);
WEAK_ALIAS void GPIO51_IRQHandler(void);
WEAK_ALIAS void UTICK0_IRQHandler(void);
WEAK_ALIAS void MRT0_IRQHandler(void);
WEAK_ALIAS void CTIMER0_IRQHandler(void);
WEAK_ALIAS void CTIMER1_IRQHandler(void);
WEAK_ALIAS void SCT0_IRQHandler(void);
WEAK_ALIAS void CTIMER2_IRQHandler(void);
WEAK_ALIAS void LP_FLEXCOMM0_IRQHandler(void);
WEAK_ALIAS void LP_FLEXCOMM1_IRQHandler(void);
WEAK_ALIAS void LP_FLEXCOMM2_IRQHandler(void);
WEAK_ALIAS void LP_FLEXCOMM3_IRQHandler(void);
WEAK_ALIAS void LP_FLEXCOMM4_IRQHandler(void);
WEAK_ALIAS void LP_FLEXCOMM5_IRQHandler(void);
WEAK_ALIAS void LP_FLEXCOMM6_IRQHandler(void);
WEAK_ALIAS void LP_FLEXCOMM7_IRQHandler(void);
WEAK_ALIAS void LP_FLEXCOMM8_IRQHandler(void);
WEAK_ALIAS void LP_FLEXCOMM9_IRQHandler(void);
WEAK_ALIAS void ADC0_IRQHandler(void);
WEAK_ALIAS void ADC1_IRQHandler(void);
WEAK_ALIAS void PINT0_IRQHandler(void);
WEAK_ALIAS void PDM_EVENT_IRQHandler(void);
WEAK_ALIAS void Reserved65_IRQHandler(void);
WEAK_ALIAS void USB0_FS_IRQHandler(void);
WEAK_ALIAS void USB0_DCD_IRQHandler(void);
WEAK_ALIAS void RTC_IRQHandler(void);
WEAK_ALIAS void SMARTDMA_IRQHandler(void);
WEAK_ALIAS void MAILBOX_IRQHandler(void);
WEAK_ALIAS void CTIMER3_IRQHandler(void);
WEAK_ALIAS void CTIMER4_IRQHandler(void);
WEAK_ALIAS void OS_EVENT_IRQHandler(void);
WEAK_ALIAS void FLEXSPI0_IRQHandler(void);
WEAK_ALIAS void SAI0_IRQHandler(void);
WEAK_ALIAS void SAI1_IRQHandler(void);
WEAK_ALIAS void USDHC0_IRQHandler(void);
WEAK_ALIAS void CAN0_IRQHandler(void);
WEAK_ALIAS void CAN1_IRQHandler(void);
WEAK_ALIAS void Reserved80_IRQHandler(void);
WEAK_ALIAS void Reserved81_IRQHandler(void);
WEAK_ALIAS void USB1_HS_PHY_IRQHandler(void);
WEAK_ALIAS void USB1_HS_IRQHandler(void);
WEAK_ALIAS void SEC_HYPERVISOR_CALL_IRQHandler(void);
WEAK_ALIAS void Reserved85_IRQHandler(void);
WEAK_ALIAS void PLU_IRQHandler(void);
WEAK_ALIAS void Freqme_IRQHandler(void);
WEAK_ALIAS void SEC_VIO_IRQHandler(void);
WEAK_ALIAS void ELS_IRQHandler(void);
WEAK_ALIAS void PKC_IRQHandler(void);
WEAK_ALIAS void PUF_IRQHandler(void);
WEAK_ALIAS void PQ_IRQHandler(void);
WEAK_ALIAS void EDMA_1_CH0_IRQHandler(void);
WEAK_ALIAS void EDMA_1_CH1_IRQHandler(void);
WEAK_ALIAS void EDMA_1_CH2_IRQHandler(void);
WEAK_ALIAS void EDMA_1_CH3_IRQHandler(void);
WEAK_ALIAS void EDMA_1_CH4_IRQHandler(void);
WEAK_ALIAS void EDMA_1_CH5_IRQHandler(void);
WEAK_ALIAS void EDMA_1_CH6_IRQHandler(void);
WEAK_ALIAS void EDMA_1_CH7_IRQHandler(void);
WEAK_ALIAS void EDMA_1_CH8_IRQHandler(void);
WEAK_ALIAS void EDMA_1_CH9_IRQHandler(void);
WEAK_ALIAS void EDMA_1_CH10_IRQHandler(void);
WEAK_ALIAS void EDMA_1_CH11_IRQHandler(void);
WEAK_ALIAS void EDMA_1_CH12_IRQHandler(void);
WEAK_ALIAS void EDMA_1_CH13_IRQHandler(void);
WEAK_ALIAS void EDMA_1_CH14_IRQHandler(void);
WEAK_ALIAS void EDMA_1_CH15_IRQHandler(void);
WEAK_ALIAS void CDOG0_IRQHandler(void);
WEAK_ALIAS void CDOG1_IRQHandler(void);
WEAK_ALIAS void I3C0_IRQHandler(void);
WEAK_ALIAS void I3C1_IRQHandler(void);
WEAK_ALIAS void NPU_IRQHandler(void);
WEAK_ALIAS void GDET_IRQHandler(void);
WEAK_ALIAS void VBAT0_IRQHandler(void);
WEAK_ALIAS void EWM0_IRQHandler(void);
WEAK_ALIAS void TSI_END_OF_SCAN_IRQHandler(void);
WEAK_ALIAS void TSI_OUT_OF_SCAN_IRQHandler(void);
WEAK_ALIAS void EMVSIM0_IRQHandler(void);
WEAK_ALIAS void EMVSIM1_IRQHandler(void);
WEAK_ALIAS void FLEXIO_IRQHandler(void);
WEAK_ALIAS void DAC0_IRQHandler(void);
WEAK_ALIAS void DAC1_IRQHandler(void);
WEAK_ALIAS void DAC2_IRQHandler(void);
WEAK_ALIAS void HSCMP0_IRQHandler(void);
WEAK_ALIAS void HSCMP1_IRQHandler(void);
WEAK_ALIAS void HSCMP2_IRQHandler(void);
WEAK_ALIAS void FLEXPWM0_RELOAD_ERROR_IRQHandler(void);
WEAK_ALIAS void FLEXPWM0_FAULT_IRQHandler(void);
WEAK_ALIAS void FLEXPWM0_SUBMODULE0_IRQHandler(void);
WEAK_ALIAS void FLEXPWM0_SUBMODULE1_IRQHandler(void);
WEAK_ALIAS void FLEXPWM0_SUBMODULE2_IRQHandler(void);
WEAK_ALIAS void FLEXPWM0_SUBMODULE3_IRQHandler(void);
WEAK_ALIAS void FLEXPWM1_RELOAD_ERROR_IRQHandler(void);
WEAK_ALIAS void FLEXPWM1_FAULT_IRQHandler(void);
WEAK_ALIAS void FLEXPWM1_SUBMODULE0_IRQHandler(void);
WEAK_ALIAS void FLEXPWM1_SUBMODULE1_IRQHandler(void);
WEAK_ALIAS void FLEXPWM1_SUBMODULE2_IRQHandler(void);
WEAK_ALIAS void FLEXPWM1_SUBMODULE3_IRQHandler(void);
WEAK_ALIAS void ENC0_COMPARE_IRQHandler(void);
WEAK_ALIAS void ENC0_HOME_IRQHandler(void);
WEAK_ALIAS void ENC0_WDG_SAB_IRQHandler(void);
WEAK_ALIAS void ENC0_IDX_IRQHandler(void);
WEAK_ALIAS void ENC1_COMPARE_IRQHandler(void);
WEAK_ALIAS void ENC1_HOME_IRQHandler(void);
WEAK_ALIAS void ENC1_WDG_SAB_IRQHandler(void);
WEAK_ALIAS void ENC1_IDX_IRQHandler(void);
WEAK_ALIAS void ITRC0_IRQHandler(void);
WEAK_ALIAS void BSP32_IRQHandler(void);
WEAK_ALIAS void ELS_ERR_IRQHandler(void);
WEAK_ALIAS void PKC_ERR_IRQHandler(void);
WEAK_ALIAS void ERM_SINGLE_BIT_ERROR_IRQHandler(void);
WEAK_ALIAS void ERM_MULTI_BIT_ERROR_IRQHandler(void);
WEAK_ALIAS void FMU0_IRQHandler(void);
WEAK_ALIAS void ETHERNET_IRQHandler(void);
WEAK_ALIAS void ETHERNET_PMT_IRQHandler(void);
WEAK_ALIAS void ETHERNET_MACLP_IRQHandler(void);
WEAK_ALIAS void SINC_FILTER_IRQHandler(void);
WEAK_ALIAS void LPTMR0_IRQHandler(void);
WEAK_ALIAS void LPTMR1_IRQHandler(void);
WEAK_ALIAS void SCG_IRQHandler(void);
WEAK_ALIAS void SPC_IRQHandler(void);
WEAK_ALIAS void WUU_IRQHandler(void);
WEAK_ALIAS void PORT_EFT_IRQHandler(void);
WEAK_ALIAS void ETB0_IRQHandler(void);
WEAK_ALIAS void SM3_IRQHandler(void);
WEAK_ALIAS void TRNG0_IRQHandler(void);
WEAK_ALIAS void WWDT0_IRQHandler(void);
WEAK_ALIAS void WWDT1_IRQHandler(void);
WEAK_ALIAS void CMC0_IRQHandler(void);
WEAK_ALIAS void CTI0_IRQHandler(void);

__attribute__((section(".vectors"))) void (*const tab[16 + 156])(void) = {
    _estack, Reset_Handler,
    NMI_Handler,              // NMI Handler
    HardFault_Handler,        // Hard Fault Handler
    MemManage_Handler,        // MPU Fault Handler
    BusFault_Handler,         // Bus Fault Handler
    UsageFault_Handler,       // Usage Fault Handler
    SecureFault_Handler,      // Secure Fault Handler
    0,                        // Reserved
    0,                        // Reserved
    0,                        // Reserved
    SVC_Handler,              // SVCall Handler
    DebugMon_Handler,         // Debug Monitor Handler
    0,                        // Reserved
    PendSV_Handler,           // PendSV Handler
    SysTick_Handler,          // SysTick Handler
                              // Chip Level - MCXN947_cm33_core0
    OR_IRQHandler,            // 16 : OR IRQ
    EDMA_0_CH0_IRQHandler,    // 17 : eDMA_0_CH0 error or transfer complete
    EDMA_0_CH1_IRQHandler,    // 18 : eDMA_0_CH1 error or transfer complete
    EDMA_0_CH2_IRQHandler,    // 19 : eDMA_0_CH2 error or transfer complete
    EDMA_0_CH3_IRQHandler,    // 20 : eDMA_0_CH3 error or transfer complete
    EDMA_0_CH4_IRQHandler,    // 21 : eDMA_0_CH4 error or transfer complete
    EDMA_0_CH5_IRQHandler,    // 22 : eDMA_0_CH5 error or transfer complete
    EDMA_0_CH6_IRQHandler,    // 23 : eDMA_0_CH6 error or transfer complete
    EDMA_0_CH7_IRQHandler,    // 24 : eDMA_0_CH7 error or transfer complete
    EDMA_0_CH8_IRQHandler,    // 25 : eDMA_0_CH8 error or transfer complete
    EDMA_0_CH9_IRQHandler,    // 26 : eDMA_0_CH9 error or transfer complete
    EDMA_0_CH10_IRQHandler,   // 27 : eDMA_0_CH10 error or transfer complete
    EDMA_0_CH11_IRQHandler,   // 28 : eDMA_0_CH11 error or transfer complete
    EDMA_0_CH12_IRQHandler,   // 29 : eDMA_0_CH12 error or transfer complete
    EDMA_0_CH13_IRQHandler,   // 30 : eDMA_0_CH13 error or transfer complete
    EDMA_0_CH14_IRQHandler,   // 31 : eDMA_0_CH14 error or transfer complete
    EDMA_0_CH15_IRQHandler,   // 32 : eDMA_0_CH15 error or transfer complete
    GPIO00_IRQHandler,        // 33 : GPIO0 interrupt 0
    GPIO01_IRQHandler,        // 34 : GPIO0 interrupt 1
    GPIO10_IRQHandler,        // 35 : GPIO1 interrupt 0
    GPIO11_IRQHandler,        // 36 : GPIO1 interrupt 1
    GPIO20_IRQHandler,        // 37 : GPIO2 interrupt 0
    GPIO21_IRQHandler,        // 38 : GPIO2 interrupt 1
    GPIO30_IRQHandler,        // 39 : GPIO3 interrupt 0
    GPIO31_IRQHandler,        // 40 : GPIO3 interrupt 1
    GPIO40_IRQHandler,        // 41 : GPIO4 interrupt 0
    GPIO41_IRQHandler,        // 42 : GPIO4 interrupt 1
    GPIO50_IRQHandler,        // 43 : GPIO5 interrupt 0
    GPIO51_IRQHandler,        // 44 : GPIO5 interrupt 1
    UTICK0_IRQHandler,        // 45 : Micro-Tick Timer interrupt
    MRT0_IRQHandler,          // 46 : Multi-Rate Timer interrupt
    CTIMER0_IRQHandler,       // 47 : Standard counter/timer 0 interrupt
    CTIMER1_IRQHandler,       // 48 : Standard counter/timer 1 interrupt
    SCT0_IRQHandler,          // 49 : SCTimer/PWM interrupt
    CTIMER2_IRQHandler,       // 50 : Standard counter/timer 2 interrupt
    LP_FLEXCOMM0_IRQHandler,  // 51 : LP_FLEXCOMM0 (LPSPI interrupt or LPI2C
                              // interrupt or LPUART Receive/Transmit interrupt)
    LP_FLEXCOMM1_IRQHandler,  // 52 : LP_FLEXCOMM1 (LPSPI interrupt or LPI2C
                              // interrupt or LPUART Receive/Transmit interrupt)
    LP_FLEXCOMM2_IRQHandler,  // 53 : LP_FLEXCOMM2 (LPSPI interrupt or LPI2C
                              // interrupt or LPUART Receive/Transmit interrupt)
    LP_FLEXCOMM3_IRQHandler,  // 54 : LP_FLEXCOMM3 (LPSPI interrupt or LPI2C
                              // interrupt or LPUART Receive/Transmit interrupt)
    LP_FLEXCOMM4_IRQHandler,  // 55 : LP_FLEXCOMM4 (LPSPI interrupt or LPI2C
                              // interrupt or LPUART Receive/Transmit interrupt)
    LP_FLEXCOMM5_IRQHandler,  // 56 : LP_FLEXCOMM5 (LPSPI interrupt or LPI2C
                              // interrupt or LPUART Receive/Transmit interrupt)
    LP_FLEXCOMM6_IRQHandler,  // 57 : LP_FLEXCOMM6 (LPSPI interrupt or LPI2C
                              // interrupt or LPUART Receive/Transmit interrupt)
    LP_FLEXCOMM7_IRQHandler,  // 58 : LP_FLEXCOMM7 (LPSPI interrupt or LPI2C
                              // interrupt or LPUART Receive/Transmit interrupt)
    LP_FLEXCOMM8_IRQHandler,  // 59 : LP_FLEXCOMM8 (LPSPI interrupt or LPI2C
                              // interrupt or LPUART Receive/Transmit interrupt)
    LP_FLEXCOMM9_IRQHandler,  // 60 : LP_FLEXCOMM9 (LPSPI interrupt or LPI2C
                              // interrupt or LPUART Receive/Transmit interrupt)
    ADC0_IRQHandler,   // 61 : Analog-to-Digital Converter 0 - General Purpose
                       // interrupt
    ADC1_IRQHandler,   // 62 : Analog-to-Digital Converter 1 - General Purpose
                       // interrupt
    PINT0_IRQHandler,  // 63 : Pin Interrupt Pattern Match Interrupt
    PDM_EVENT_IRQHandler,   // 64 : Microphone Interface interrupt
    Reserved65_IRQHandler,  // 65 : Reserved interrupt
    USB0_FS_IRQHandler,     // 66 : Universal Serial Bus - Full Speed interrupt
    USB0_DCD_IRQHandler,    // 67 : Universal Serial Bus - Device Charge Detect
                            // interrupt
    RTC_IRQHandler,       // 68 : RTC Subsystem interrupt (RTC interrupt or Wake
                          // timer interrupt)
    SMARTDMA_IRQHandler,  // 69 : SmartDMA_IRQ
    MAILBOX_IRQHandler,  // 70 : Inter-CPU Mailbox interrupt0 for CPU0 Inter-CPU
                         // Mailbox interrupt1 for CPU1
    CTIMER3_IRQHandler,  // 71 : Standard counter/timer 3 interrupt
    CTIMER4_IRQHandler,  // 72 : Standard counter/timer 4 interrupt
    OS_EVENT_IRQHandler,  // 73 : OS event timer interrupt
    FLEXSPI0_IRQHandler,  // 74 : Flexible Serial Peripheral Interface interrupt
    SAI0_IRQHandler,      // 75 : Serial Audio Interface 0 interrupt
    SAI1_IRQHandler,      // 76 : Serial Audio Interface 1 interrupt
    USDHC0_IRQHandler,  // 77 : Ultra Secured Digital Host Controller interrupt
    CAN0_IRQHandler,    // 78 : Controller Area Network 0 interrupt
    CAN1_IRQHandler,    // 79 : Controller Area Network 1 interrupt
    Reserved80_IRQHandler,   // 80 : Reserved interrupt
    Reserved81_IRQHandler,   // 81 : Reserved interrupt
    USB1_HS_PHY_IRQHandler,  // 82 : USBHS DCD or USBHS Phy interrupt
    USB1_HS_IRQHandler,      // 83 : USB High Speed OTG Controller interrupt
    SEC_HYPERVISOR_CALL_IRQHandler,  // 84 : AHB Secure Controller hypervisor
                                     // call interrupt
    Reserved85_IRQHandler,           // 85 : Reserved interrupt
    PLU_IRQHandler,                  // 86 : Programmable Logic Unit interrupt
    Freqme_IRQHandler,               // 87 : Frequency Measurement interrupt
    SEC_VIO_IRQHandler,      // 88 : Secure violation interrupt (Memory Block
                             // Checker interrupt or secure AHB matrix violation
                             // interrupt)
    ELS_IRQHandler,          // 89 : ELS interrupt
    PKC_IRQHandler,          // 90 : PKC interrupt
    PUF_IRQHandler,          // 91 : Physical Unclonable Function interrupt
    PQ_IRQHandler,           // 92 : Power Quad interrupt
    EDMA_1_CH0_IRQHandler,   // 93 : eDMA_1_CH0 error or transfer complete
    EDMA_1_CH1_IRQHandler,   // 94 : eDMA_1_CH1 error or transfer complete
    EDMA_1_CH2_IRQHandler,   // 95 : eDMA_1_CH2 error or transfer complete
    EDMA_1_CH3_IRQHandler,   // 96 : eDMA_1_CH3 error or transfer complete
    EDMA_1_CH4_IRQHandler,   // 97 : eDMA_1_CH4 error or transfer complete
    EDMA_1_CH5_IRQHandler,   // 98 : eDMA_1_CH5 error or transfer complete
    EDMA_1_CH6_IRQHandler,   // 99 : eDMA_1_CH6 error or transfer complete
    EDMA_1_CH7_IRQHandler,   // 100: eDMA_1_CH7 error or transfer complete
    EDMA_1_CH8_IRQHandler,   // 101: eDMA_1_CH8 error or transfer complete
    EDMA_1_CH9_IRQHandler,   // 102: eDMA_1_CH9 error or transfer complete
    EDMA_1_CH10_IRQHandler,  // 103: eDMA_1_CH10 error or transfer complete
    EDMA_1_CH11_IRQHandler,  // 104: eDMA_1_CH11 error or transfer complete
    EDMA_1_CH12_IRQHandler,  // 105: eDMA_1_CH12 error or transfer complete
    EDMA_1_CH13_IRQHandler,  // 106: eDMA_1_CH13 error or transfer complete
    EDMA_1_CH14_IRQHandler,  // 107: eDMA_1_CH14 error or transfer complete
    EDMA_1_CH15_IRQHandler,  // 108: eDMA_1_CH15 error or transfer complete
    CDOG0_IRQHandler,        // 109: Code Watchdog Timer 0 interrupt
    CDOG1_IRQHandler,        // 110: Code Watchdog Timer 1 interrupt
    I3C0_IRQHandler,   // 111: Improved Inter Integrated Circuit interrupt 0
    I3C1_IRQHandler,   // 112: Improved Inter Integrated Circuit interrupt 1
    NPU_IRQHandler,    // 113: NPU interrupt
    GDET_IRQHandler,   // 114: Digital Glitch Detect 0 interrupt  or Digital
                       // Glitch Detect 1 interrupt
    VBAT0_IRQHandler,  // 115: VBAT interrupt( VBAT interrupt or digital tamper
                       // interrupt)
    EWM0_IRQHandler,   // 116: External Watchdog Monitor interrupt
    TSI_END_OF_SCAN_IRQHandler,  // 117: TSI End of Scan interrupt
    TSI_OUT_OF_SCAN_IRQHandler,  // 118: TSI Out of Scan interrupt
    EMVSIM0_IRQHandler,          // 119: EMVSIM0 interrupt
    EMVSIM1_IRQHandler,          // 120: EMVSIM1 interrupt
    FLEXIO_IRQHandler,           // 121: Flexible Input/Output interrupt
    DAC0_IRQHandler,    // 122: Digital-to-Analog Converter 0 - General Purpose
                        // interrupt
    DAC1_IRQHandler,    // 123: Digital-to-Analog Converter 1 - General Purpose
                        // interrupt
    DAC2_IRQHandler,    // 124: 14-bit Digital-to-Analog Converter interrupt
    HSCMP0_IRQHandler,  // 125: High-Speed comparator0 interrupt
    HSCMP1_IRQHandler,  // 126: High-Speed comparator1 interrupt
    HSCMP2_IRQHandler,  // 127: High-Speed comparator2 interrupt
    FLEXPWM0_RELOAD_ERROR_IRQHandler,  // 128: FlexPWM0_reload_error interrupt
    FLEXPWM0_FAULT_IRQHandler,         // 129: FlexPWM0_fault interrupt
    FLEXPWM0_SUBMODULE0_IRQHandler,    // 130: FlexPWM0 Submodule 0
                                       // capture/compare/reload interrupt
    FLEXPWM0_SUBMODULE1_IRQHandler,    // 131: FlexPWM0 Submodule 1
                                       // capture/compare/reload interrupt
    FLEXPWM0_SUBMODULE2_IRQHandler,    // 132: FlexPWM0 Submodule 2
                                       // capture/compare/reload interrupt
    FLEXPWM0_SUBMODULE3_IRQHandler,    // 133: FlexPWM0 Submodule 3
                                       // capture/compare/reload interrupt
    FLEXPWM1_RELOAD_ERROR_IRQHandler,  // 134: FlexPWM1_reload_error interrupt
    FLEXPWM1_FAULT_IRQHandler,         // 135: FlexPWM1_fault interrupt
    FLEXPWM1_SUBMODULE0_IRQHandler,    // 136: FlexPWM1 Submodule 0
                                       // capture/compare/reload interrupt
    FLEXPWM1_SUBMODULE1_IRQHandler,    // 137: FlexPWM1 Submodule 1
                                       // capture/compare/reload interrupt
    FLEXPWM1_SUBMODULE2_IRQHandler,    // 138: FlexPWM1 Submodule 2
                                       // capture/compare/reload interrupt
    FLEXPWM1_SUBMODULE3_IRQHandler,    // 139: FlexPWM1 Submodule 3
                                       // capture/compare/reload interrupt
    ENC0_COMPARE_IRQHandler,           // 140: ENC0_Compare interrupt
    ENC0_HOME_IRQHandler,              // 141: ENC0_Home interrupt
    ENC0_WDG_SAB_IRQHandler,           // 142: ENC0_WDG_IRQ/SAB interrupt
    ENC0_IDX_IRQHandler,               // 143: ENC0_IDX interrupt
    ENC1_COMPARE_IRQHandler,           // 144: ENC1_Compare interrupt
    ENC1_HOME_IRQHandler,              // 145: ENC1_Home interrupt
    ENC1_WDG_SAB_IRQHandler,           // 146: ENC1_WDG_IRQ/SAB interrupt
    ENC1_IDX_IRQHandler,               // 147: ENC1_IDX interrupt
    ITRC0_IRQHandler,    // 148: Intrusion and Tamper Response Controller
                         // interrupt
    BSP32_IRQHandler,    // 149: CoolFlux BSP32 interrupt
    ELS_ERR_IRQHandler,  // 150: ELS error interrupt
    PKC_ERR_IRQHandler,  // 151: PKC error interrupt
    ERM_SINGLE_BIT_ERROR_IRQHandler,  // 152: ERM Single Bit error interrupt
    ERM_MULTI_BIT_ERROR_IRQHandler,   // 153: ERM Multi Bit error interrupt
    FMU0_IRQHandler,                  // 154: Flash Management Unit interrupt
    ETHERNET_IRQHandler,              // 155: Ethernet QoS interrupt
    ETHERNET_PMT_IRQHandler,    // 156: Ethernet QoS power management interrupt
    ETHERNET_MACLP_IRQHandler,  // 157: Ethernet QoS MAC interrupt
    SINC_FILTER_IRQHandler,     // 158: SINC Filter interrupt
    LPTMR0_IRQHandler,          // 159: Low Power Timer 0 interrupt
    LPTMR1_IRQHandler,          // 160: Low Power Timer 1 interrupt
    SCG_IRQHandler,             // 161: System Clock Generator interrupt
    SPC_IRQHandler,             // 162: System Power Controller interrupt
    WUU_IRQHandler,             // 163: Wake Up Unit interrupt
    PORT_EFT_IRQHandler,        // 164: PORT0~5 EFT interrupt
    ETB0_IRQHandler,            // 165: ETB counter expires interrupt
    SM3_IRQHandler,    // 166: Secure Generic Interface (SGI) SAFO interrupt
    TRNG0_IRQHandler,  // 167: True Random Number Generator interrupt
    WWDT0_IRQHandler,  // 168: Windowed Watchdog Timer 0 interrupt
    WWDT1_IRQHandler,  // 169: Windowed Watchdog Timer 1 interrupt
    CMC0_IRQHandler,   // 170: Core Mode Controller interrupt
    CTI0_IRQHandler,   // 171: Cross Trigger Interface interrupt
};

extern unsigned char _end[];  // End of data section, start of heap. See link.ld
__attribute__((naked, noreturn)) void Reset_Handler(void) {
  __asm("cpsid i");  // Disable interrupts
  // set SPLIM to somewhere, trick the linker
  __asm volatile("MSR MSPLIM, %1       \n"
                 :
                 : "r"(tab), "r"(_end)
                 : "r0", "r1");
  SYSCON->ECC_ENABLE_CTRL = 0;  // disable RAM ECC, must do for this link.ld
  // Clear BSS section, and copy data section from flash to RAM
  extern long _sbss, _ebss, _sdata, _edata, _sidata;
  for (long *dst = &_sbss; dst < &_ebss; dst++) *dst = 0;
  for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

  SystemInit();

  __asm("cpsie i");  // Reenable interrupts

  // Call main()
  extern void main(void);
  main();
  for (;;) (void) 0;  // Infinite loop
}

void Default_Handler(void) {
  for (;;) (void) 0;
}
