// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved
//
// This file contains essentials required by the CMSIS:
// uint32_t SystemCoreClock - holds the system core clock value
// SystemInit() - initialises the system, e.g. sets up clocks

#include "hal.h"

uint32_t SystemCoreClock = SYS_FREQUENCY;

// - 14.4, Figure 14-2: clock tree
// - 14.7.4: ARM_PODF defaults to /1
// - 14.7.5: AHB_PODF defaults to /1; PERIPH_CLK_SEL defaults to derive clock
// from pre_periph_clk_sel
// - 14.7.6: PRE_PERIPH_CLK_SEL defaults to derive clock from PLL2 PFD3.
// - (For 528MHz operation, we need to set it to derive clock from PLL2, but
// this chip max is 500 MHz).
// - 14.6.1.3.1 System PLL (PLL2); 13.3.2.2 PLLs; 14.6.1.4 Phase Fractional
// Dividers (PFD)
// - 14.8.2: PLL2 is powered off and bypassed to 24MHz
// - 14.8.11: PFD defaults to 18/16 but Figure 14-2 shows half the value
// ("divider")
// - For 500MHz operation, we need to set PRE_PERIPH_CLK_SEL to derive clock
// from divided PLL6
// - 14.8.9: configure PLL6 to generate a 500MHz clock
// - Datasheet 4.1.3: System frequency/Bus frequency max 500/125MHz respectively
// (AHB/IPG)
// - MCUXpresso: IPG_CLK_ROOT <= 125MHz; PERCLK_CLK_ROOT <= 62.5MHz
// - Datasheet 4.8.4.1.1/2: the processor clock frequency must exceed twice the
// ENET_RX_CLK/ENET_TX_CLK frequency.
// - Datasheet 4.8.4.2: no details for RMII (above is for MII), assumed 50MHz
// min processor clock
// - Datasheet 4.1.3, Table 11: "Overdrive" run mode requires 1.25V core voltage
// minimum
void SystemInit(void) {  // Called automatically by startup code (ints masked)
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // Enable FPU
  asm("DSB");
  asm("ISB");
  // 53.4.2: Disable watchdog after reset (unlocked)
  RTWDOG->CS &= ~RTWDOG_CS_EN_MASK;
  RTWDOG->TOVAL = 0xFFFF;
  while (RTWDOG->CS & RTWDOG_CS_ULK_MASK) spin(1);  // wait for lock
  while ((RTWDOG->CS & RTWDOG_CS_RCS_MASK) == 0)
    spin(1);  // wait for new config
  // Set VDD_SOC to 1.25V
  SETBITS(DCDC->REG3, DCDC_REG3_TRG_MASK, DCDC_REG3_TRG(0x12));
  while ((DCDC->REG0 & DCDC_REG0_STS_DC_OK_MASK) == 0)
    spin(1);  // Wait for DCDC_STS_DC_OK
  // 14.8.9 Init 500MHz reference, clock the M7 core with it, generate 50MHz for
  // ENET and RMII.
  SETBITS(CCM_ANALOG->PLL_ENET, CCM_ANALOG_PLL_ENET_BYPASS_CLK_SRC_MASK,
          CCM_ANALOG_PLL_ENET_BYPASS_MASK |
              CCM_ANALOG_PLL_ENET_BYPASS_CLK_SRC(0));  // bypass to 24MHz osc
  SETBITS(
      CCM_ANALOG->PLL_ENET,
      CCM_ANALOG_PLL_ENET_DIV_SELECT_MASK | CCM_ANALOG_PLL_ENET_POWERDOWN_MASK,
      CCM_ANALOG_PLL_ENET_DIV_SELECT(1) | CCM_ANALOG_PLL_ENET_ENABLE_MASK |
          CCM_ANALOG_PLL_ENET_ENET_500M_REF_EN_MASK);  // setup PLL
  while ((CCM_ANALOG->PLL_ENET & CCM_ANALOG_PLL_ENET_LOCK_MASK) == 0)
    spin(1);  // wait until it is stable
  CCM_ANALOG->PLL_ENET &=
      ~CCM_ANALOG_PLL_ENET_BYPASS_MASK;  // Disable Bypass (switch to PLL)
  SETBITS(CCM->CBCDR, CCM_CBCDR_IPG_PODF_MASK,
          CCM_CBCDR_IPG_PODF(3));  // Set IPG divider /4 (125MHz)
  SETBITS(CCM->CSCMR1, CCM_CSCMR1_PERCLK_PODF_MASK,
          CCM_CSCMR1_PERCLK_PODF(1));  // Set PERCLK divider /2 (62.5MHz)
  SETBITS(CCM->CBCMR, CCM_CBCMR_PRE_PERIPH_CLK_SEL_MASK,
          CCM_CBCMR_PRE_PERIPH_CLK_SEL(3));  // run from 500MHz clock
  // 14.5 Table 14-4: uart_clk_root
  // 14.4: uart_clk_root = PLL3/6 or OSC; CCM_CSCDR1 (14.7.9) defaults to
  // PLL3/6/1
  CCM_ANALOG->PLL_USB1 |= CCM_ANALOG_PLL_USB1_POWER_MASK;  // Power PLL on
  while ((CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_LOCK_MASK) == 0)
    spin(1);  // wait until it is stable
  CCM_ANALOG->PLL_USB1 &=
      ~CCM_ANALOG_PLL_USB1_BYPASS_MASK;  // Disable Bypass (switch to PLL)
  rng_init();                            // Initialise random number generator
  // NXP startup code calls SystemInit BEFORE initializing RAM...
  SysTick_Config(SYS_FREQUENCY / 1000);  // Sys tick every 1ms
}

#if 0
__attribute__((section(".cfg"), used)) uint32_t __cfg[] = {0x1234abcd};

extern uint32_t __isr_vector[];
extern uint32_t __ivt_boot_data[];

__attribute__((section(".ivt"), used)) uint32_t __ivt[8] = {
    0x412000d1,                  // header: 41 - version, 2000 size, d1 tag
    (uint32_t) __isr_vector,     // entry
    0,                           // reserved
    0,                           // dcd
    (uint32_t) __ivt_boot_data,  // boot data
    (uint32_t) __ivt,            // this is us - ivt absolute address
    0,                           // csf absolute address
    0,                           // reserved for HAB
};

__attribute__((section(".ivt"), used)) uint32_t __ivt_boot_data[] = {
    0,          // boot start location
    64 * 1024,  // size
    0,          // Plugin flag
    0Xffffffff  // empty - extra data word
};
#endif
