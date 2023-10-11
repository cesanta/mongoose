// SPDX-FileCopyrightText: 2022-2023 Cesanta Software Limited
// SPDX-License-Identifier: MIT

#include "hal.h"

void Reset_Handler(void);    // Defined below
void Dummy_Handler(void);    // Defined below
void SysTick_Handler(void);  // Defined in main.c
void SystemInit(void);       // Defined in main.c, called by reset handler
void _estack(void);          // Defined in link.ld

#define WEAK_ALIAS __attribute__((weak, alias("Default_Handler")))

WEAK_ALIAS void NMI_Handler(void);
WEAK_ALIAS void HardFault_Handler(void);
WEAK_ALIAS void MemoryManagement_Handler(void);
WEAK_ALIAS void BusFault_Handler(void);
WEAK_ALIAS void UsageFault_Handler(void);
WEAK_ALIAS void SVCall_Handler(void);
WEAK_ALIAS void DebugMonitor_Handler(void);
WEAK_ALIAS void PendSV_Handler(void);
WEAK_ALIAS void SysTick_Handler(void);

WEAK_ALIAS void PM_Handler(void);
WEAK_ALIAS void MCLK_Handler(void);
WEAK_ALIAS void OSCCTRL_XOSC0_Handler(void);
WEAK_ALIAS void OSCCTRL_XOSC1_Handler(void);
WEAK_ALIAS void OSCCTRL_DFLL_Handler(void);
WEAK_ALIAS void OSCCTRL_DPLL0_Handler(void);
WEAK_ALIAS void OSCCTRL_DPLL1_Handler(void);
WEAK_ALIAS void OSC32KCTRL_Handler(void);
WEAK_ALIAS void SUPC_OTHER_Handler(void);
WEAK_ALIAS void SUPC_BODDET_Handler(void);
WEAK_ALIAS void WDT_Handler(void);
WEAK_ALIAS void RTC_Handler(void);
WEAK_ALIAS void EIC_EXTINT_0_Handler(void);
WEAK_ALIAS void EIC_EXTINT_1_Handler(void);
WEAK_ALIAS void EIC_EXTINT_2_Handler(void);
WEAK_ALIAS void EIC_EXTINT_3_Handler(void);
WEAK_ALIAS void EIC_EXTINT_4_Handler(void);
WEAK_ALIAS void EIC_EXTINT_5_Handler(void);
WEAK_ALIAS void EIC_EXTINT_6_Handler(void);
WEAK_ALIAS void EIC_EXTINT_7_Handler(void);
WEAK_ALIAS void EIC_EXTINT_8_Handler(void);
WEAK_ALIAS void EIC_EXTINT_9_Handler(void);
WEAK_ALIAS void EIC_EXTINT_10_Handler(void);
WEAK_ALIAS void EIC_EXTINT_11_Handler(void);
WEAK_ALIAS void EIC_EXTINT_12_Handler(void);
WEAK_ALIAS void EIC_EXTINT_13_Handler(void);
WEAK_ALIAS void EIC_EXTINT_14_Handler(void);
WEAK_ALIAS void EIC_EXTINT_15_Handler(void);
WEAK_ALIAS void FREQM_Handler(void);
WEAK_ALIAS void NVMCTRL_0_Handler(void);
WEAK_ALIAS void NVMCTRL_1_Handler(void);
WEAK_ALIAS void DMAC_0_Handler(void);
WEAK_ALIAS void DMAC_1_Handler(void);
WEAK_ALIAS void DMAC_2_Handler(void);
WEAK_ALIAS void DMAC_3_Handler(void);
WEAK_ALIAS void DMAC_OTHER_Handler(void);
WEAK_ALIAS void EVSYS_0_Handler(void);
WEAK_ALIAS void EVSYS_1_Handler(void);
WEAK_ALIAS void EVSYS_2_Handler(void);
WEAK_ALIAS void EVSYS_3_Handler(void);
WEAK_ALIAS void EVSYS_OTHER_Handler(void);
WEAK_ALIAS void PAC_Handler(void);
WEAK_ALIAS void RAMECC_Handler(void);
WEAK_ALIAS void SERCOM0_0_Handler(void);
WEAK_ALIAS void SERCOM0_1_Handler(void);
WEAK_ALIAS void SERCOM0_2_Handler(void);
WEAK_ALIAS void SERCOM0_OTHER_Handler(void);
WEAK_ALIAS void SERCOM1_0_Handler(void);
WEAK_ALIAS void SERCOM1_1_Handler(void);
WEAK_ALIAS void SERCOM1_2_Handler(void);
WEAK_ALIAS void SERCOM1_OTHER_Handler(void);
WEAK_ALIAS void SERCOM2_0_Handler(void);
WEAK_ALIAS void SERCOM2_1_Handler(void);
WEAK_ALIAS void SERCOM2_2_Handler(void);
WEAK_ALIAS void SERCOM2_OTHER_Handler(void);
WEAK_ALIAS void SERCOM3_0_Handler(void);
WEAK_ALIAS void SERCOM3_1_Handler(void);
WEAK_ALIAS void SERCOM3_2_Handler(void);
WEAK_ALIAS void SERCOM3_OTHER_Handler(void);
WEAK_ALIAS void SERCOM4_0_Handler(void);
WEAK_ALIAS void SERCOM4_1_Handler(void);
WEAK_ALIAS void SERCOM4_2_Handler(void);
WEAK_ALIAS void SERCOM4_OTHER_Handler(void);
WEAK_ALIAS void SERCOM5_0_Handler(void);
WEAK_ALIAS void SERCOM5_1_Handler(void);
WEAK_ALIAS void SERCOM5_2_Handler(void);
WEAK_ALIAS void SERCOM5_OTHER_Handler(void);
WEAK_ALIAS void SERCOM6_0_Handler(void);
WEAK_ALIAS void SERCOM6_1_Handler(void);
WEAK_ALIAS void SERCOM6_2_Handler(void);
WEAK_ALIAS void SERCOM6_OTHER_Handler(void);
WEAK_ALIAS void SERCOM7_0_Handler(void);
WEAK_ALIAS void SERCOM7_1_Handler(void);
WEAK_ALIAS void SERCOM7_2_Handler(void);
WEAK_ALIAS void SERCOM7_OTHER_Handler(void);
WEAK_ALIAS void CAN0_Handler(void);
WEAK_ALIAS void CAN1_Handler(void);
WEAK_ALIAS void USB_OTHER_Handler(void);
WEAK_ALIAS void USB_SOF_HSOF_Handler(void);
WEAK_ALIAS void USB_TRCPT0_Handler(void);
WEAK_ALIAS void USB_TRCPT1_Handler(void);
WEAK_ALIAS void GMAC_Handler(void);
WEAK_ALIAS void TCC0_OTHER_Handler(void);
WEAK_ALIAS void TCC0_MC0_Handler(void);
WEAK_ALIAS void TCC0_MC1_Handler(void);
WEAK_ALIAS void TCC0_MC2_Handler(void);
WEAK_ALIAS void TCC0_MC3_Handler(void);
WEAK_ALIAS void TCC0_MC4_Handler(void);
WEAK_ALIAS void TCC0_MC5_Handler(void);
WEAK_ALIAS void TCC1_OTHER_Handler(void);
WEAK_ALIAS void TCC1_MC0_Handler(void);
WEAK_ALIAS void TCC1_MC1_Handler(void);
WEAK_ALIAS void TCC1_MC2_Handler(void);
WEAK_ALIAS void TCC1_MC3_Handler(void);
WEAK_ALIAS void TCC2_OTHER_Handler(void);
WEAK_ALIAS void TCC2_MC0_Handler(void);
WEAK_ALIAS void TCC2_MC1_Handler(void);
WEAK_ALIAS void TCC2_MC2_Handler(void);
WEAK_ALIAS void TCC3_OTHER_Handler(void);
WEAK_ALIAS void TCC3_MC0_Handler(void);
WEAK_ALIAS void TCC3_MC1_Handler(void);
WEAK_ALIAS void TCC4_OTHER_Handler(void);
WEAK_ALIAS void TCC4_MC0_Handler(void);
WEAK_ALIAS void TCC4_MC1_Handler(void);
WEAK_ALIAS void TC0_Handler(void);
WEAK_ALIAS void TC1_Handler(void);
WEAK_ALIAS void TC2_Handler(void);
WEAK_ALIAS void TC3_Handler(void);
WEAK_ALIAS void TC4_Handler(void);
WEAK_ALIAS void TC5_Handler(void);
WEAK_ALIAS void TC6_Handler(void);
WEAK_ALIAS void TC7_Handler(void);
WEAK_ALIAS void PDEC_OTHER_Handler(void);
WEAK_ALIAS void PDEC_MC0_Handler(void);
WEAK_ALIAS void PDEC_MC1_Handler(void);
WEAK_ALIAS void ADC0_OTHER_Handler(void);
WEAK_ALIAS void ADC0_RESRDY_Handler(void);
WEAK_ALIAS void ADC1_OTHER_Handler(void);
WEAK_ALIAS void ADC1_RESRDY_Handler(void);
WEAK_ALIAS void AC_Handler(void);
WEAK_ALIAS void DAC_OTHER_Handler(void);
WEAK_ALIAS void DAC_EMPTY_0_Handler(void);
WEAK_ALIAS void DAC_EMPTY_1_Handler(void);
WEAK_ALIAS void DAC_RESRDY_0_Handler(void);
WEAK_ALIAS void DAC_RESRDY_1_Handler(void);
WEAK_ALIAS void I2S_Handler(void);
WEAK_ALIAS void PCC_Handler(void);
WEAK_ALIAS void AES_Handler(void);
WEAK_ALIAS void TRNG_Handler(void);
WEAK_ALIAS void ICM_Handler(void);
WEAK_ALIAS void PUKCC_Handler(void);
WEAK_ALIAS void QSPI_Handler(void);
WEAK_ALIAS void SDHC0_Handler(void);
WEAK_ALIAS void SDHC1_Handler(void);

__attribute__((section(".vectors"))) void (*const tab[16 + 138])(void) = {
    _estack,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemoryManagement_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    NULL,
    NULL,
    NULL,
    NULL,
    SVCall_Handler,
    DebugMonitor_Handler,
    NULL,
    PendSV_Handler,
    SysTick_Handler,
    PM_Handler,  //  0 Power
    MCLK_Handler,  //  1 Main
    OSCCTRL_XOSC0_Handler,  //  2 Oscillators
    OSCCTRL_XOSC1_Handler,  //  3 Oscillators
    OSCCTRL_DFLL_Handler,  //  4 Oscillators
    OSCCTRL_DPLL0_Handler,  //  5 Oscillators
    OSCCTRL_DPLL1_Handler,  //  6 Oscillators
    OSC32KCTRL_Handler,  //  7 32kHz
    SUPC_OTHER_Handler,  //  8 Supply
    SUPC_BODDET_Handler,  //  9 Supply
    WDT_Handler,  //  10 Watchdog
    RTC_Handler,  //  11 Real-Time
    EIC_EXTINT_0_Handler,  //  12 External
    EIC_EXTINT_1_Handler,  //  13 External
    EIC_EXTINT_2_Handler,  //  14 External
    EIC_EXTINT_3_Handler,  //  15 External
    EIC_EXTINT_4_Handler,  //  16 External
    EIC_EXTINT_5_Handler,  //  17 External
    EIC_EXTINT_6_Handler,  //  18 External
    EIC_EXTINT_7_Handler,  //  19 External
    EIC_EXTINT_8_Handler,  //  20 External
    EIC_EXTINT_9_Handler,  //  21 External
    EIC_EXTINT_10_Handler,  //  22 External
    EIC_EXTINT_11_Handler,  //  23 External
    EIC_EXTINT_12_Handler,  //  24 External
    EIC_EXTINT_13_Handler,  //  25 External
    EIC_EXTINT_14_Handler,  //  26 External
    EIC_EXTINT_15_Handler,  //  27 External
    FREQM_Handler,  //  28 Frequency
    NVMCTRL_0_Handler,  //  29 Non-Volatile
    NVMCTRL_1_Handler,  //  30 Non-Volatile
    DMAC_0_Handler,  //  31 Direct
    DMAC_1_Handler,  //  32 Direct
    DMAC_2_Handler,  //  33 Direct
    DMAC_3_Handler,  //  34 Direct
    DMAC_OTHER_Handler,  //  35 Direct
    EVSYS_0_Handler,  //  36 Event
    EVSYS_1_Handler,  //  37 Event
    EVSYS_2_Handler,  //  38 Event
    EVSYS_3_Handler,  //  39 Event
    EVSYS_OTHER_Handler,  //  40 Event
    PAC_Handler,  //  41 Peripheral
    0,  //  42 Reserved
    0,  //  43 Reserved
    0,  //  44 Reserved
    RAMECC_Handler,  //  45 RAM
    SERCOM0_0_Handler,  //  46 Serial
    SERCOM0_1_Handler,  //  47 Serial
    SERCOM0_2_Handler,  //  48 Serial
    SERCOM0_OTHER_Handler,  //  49 Serial
    SERCOM1_0_Handler,  //  50 Serial
    SERCOM1_1_Handler,  //  51 Serial
    SERCOM1_2_Handler,  //  52 Serial
    SERCOM1_OTHER_Handler,  //  53 Serial
    SERCOM2_0_Handler,  //  54 Serial
    SERCOM2_1_Handler,  //  55 Serial
    SERCOM2_2_Handler,  //  56 Serial
    SERCOM2_OTHER_Handler,  //  57 Serial
    SERCOM3_0_Handler,  //  58 Serial
    SERCOM3_1_Handler,  //  59 Serial
    SERCOM3_2_Handler,  //  60 Serial
    SERCOM3_OTHER_Handler,  //  61 Serial
    SERCOM4_0_Handler,  //  62 Serial
    SERCOM4_1_Handler,  //  63 Serial
    SERCOM4_2_Handler,  //  64 Serial
    SERCOM4_OTHER_Handler,  //  65 Serial
    SERCOM5_0_Handler,  //  66 Serial
    SERCOM5_1_Handler,  //  67 Serial
    SERCOM5_2_Handler,  //  68 Serial
    SERCOM5_OTHER_Handler,  //  69 Serial
    SERCOM6_0_Handler,  //  70 Serial
    SERCOM6_1_Handler,  //  71 Serial
    SERCOM6_2_Handler,  //  72 Serial
    SERCOM6_OTHER_Handler,  //  73 Serial
    SERCOM7_0_Handler,  //  74 Serial
    SERCOM7_1_Handler,  //  75 Serial
    SERCOM7_2_Handler,  //  76 Serial
    SERCOM7_OTHER_Handler,  //  77 Serial
    CAN0_Handler,  //  78 Control
    CAN1_Handler,  //  79 Control
    USB_OTHER_Handler,  //  80 Universal
    USB_SOF_HSOF_Handler,  //  81 Universal
    USB_TRCPT0_Handler,  //  82 Universal
    USB_TRCPT1_Handler,  //  83 Universal
    GMAC_Handler,  //  84 Ethernet
    TCC0_OTHER_Handler,  //  85 Timer
    TCC0_MC0_Handler,  //  86 Timer
    TCC0_MC1_Handler,  //  87 Timer
    TCC0_MC2_Handler,  //  88 Timer
    TCC0_MC3_Handler,  //  89 Timer
    TCC0_MC4_Handler,  //  90 Timer
    TCC0_MC5_Handler,  //  91 Timer
    TCC1_OTHER_Handler,  //  92 Timer
    TCC1_MC0_Handler,  //  93 Timer
    TCC1_MC1_Handler,  //  94 Timer
    TCC1_MC2_Handler,  //  95 Timer
    TCC1_MC3_Handler,  //  96 Timer
    TCC2_OTHER_Handler,  //  97 Timer
    TCC2_MC0_Handler,  //  98 Timer
    TCC2_MC1_Handler,  //  99 Timer
    TCC2_MC2_Handler,  //  100 Timer
    TCC3_OTHER_Handler,  //  101 Timer
    TCC3_MC0_Handler,  //  102 Timer
    TCC3_MC1_Handler,  //  103 Timer
    TCC4_OTHER_Handler,  //  104 Timer
    TCC4_MC0_Handler,  //  105 Timer
    TCC4_MC1_Handler,  //  106 Timer
    TC0_Handler,  //  107 Basic
    TC1_Handler,  //  108 Basic
    TC2_Handler,  //  109 Basic
    TC3_Handler,  //  110 Basic
    TC4_Handler,  //  111 Basic
    TC5_Handler,  //  112 Basic
    TC6_Handler,  //  113 Basic
    TC7_Handler,  //  114 Basic
    PDEC_OTHER_Handler,  //  115 Quadrature
    PDEC_MC0_Handler,  //  116 Quadrature
    PDEC_MC1_Handler,  //  117 Quadrature
    ADC0_OTHER_Handler,  //  118 Analog
    ADC0_RESRDY_Handler,  //  119 Analog
    ADC1_OTHER_Handler,  //  120 Analog
    ADC1_RESRDY_Handler,  //  121 Analog
    AC_Handler,  //  122 Analog
    DAC_OTHER_Handler,  //  123 Digital-to-Analog
    DAC_EMPTY_0_Handler,  //  124 Digital-to-Analog
    DAC_EMPTY_1_Handler,  //  125 Digital-to-Analog
    DAC_RESRDY_0_Handler,  //  126 Digital-to-Analog
    DAC_RESRDY_1_Handler,  //  127 Digital-to-Analog
    I2S_Handler,  //  128 Inter-IC
    PCC_Handler,  //  129 Parallel
    AES_Handler,  //  130 Advanced
    TRNG_Handler,  //  131 True
    ICM_Handler,  //  132 Integrity
    PUKCC_Handler,  //  133 PUblic-Key
    QSPI_Handler,  //  134 Quad
    SDHC0_Handler,  //  135 SD/MMC
    SDHC1_Handler  //  136 SD/MMC
};

__attribute__((naked, noreturn)) void Reset_Handler(void) {
  // Clear BSS section, and copy data section from flash to RAM
  extern long _sbss, _ebss, _sdata, _edata, _sidata;
  for (long *src = &_sbss; src < &_ebss; src++) *src = 0;
  for (long *src = &_sdata, *dst = &_sidata; src < &_edata;) *src++ = *dst++;

  SCB->VTOR = (uint32_t) &tab;
  SystemInit();

  // Call main()
  extern void main(void);
  main();
  for (;;) (void) 0;  // Infinite loop
}

void Default_Handler(void) {
  for (;;) (void) 0;
}
