// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved
//
// This file contains essentials required by the CMSIS:
// uint32_t SystemCoreClock - holds the system core clock value
// SystemInit() - initialises the system, e.g. sets up clocks

#include "hal.h"

uint32_t SystemCoreClock = SYS_FREQUENCY;

// - 14.4, Figure 14-2: clock tree
// - 14.7.4: ARM_PODF defaults to /2
// - 14.7.5: AHB_PODF defaults to /1; IPG_PODF defaults to /4; PERIPH_CLK_SEL
// defaults to derive clock from pre_periph_clk_sel
// - 14.7.6: PRE_PERIPH_CLK_SEL defaults to derive clock from divided PLL1.
// - (For 528MHz operation, we need to set it to derive clock from PLL2).
// - 14.6.1.3.1 ARM PLL (PLL1); 13.3.2.2 PLLs
// - 14.8.1: PLL1 is powered off and bypassed to 24MHz. Fout = 24MHz *
// div_select/2
// - For 600MHz operation, we need to set PLL1 on
// - Datasheet 4.1.3: System frequency/Bus frequency max 600/150MHz respectively
// (AHB/IPG)
// - MCUXpresso: IPG_CLK_ROOT <= 150MHz; PERCLK_CLK_ROOT <= 75MHz
// - Datasheet 4.9.4.1.1/2: the processor clock frequency must exceed twice the
// ENET_RX_CLK/ENET_TX_CLK frequency.
// - Datasheet 4.9.4.2: no details for RMII (above is for MII), assumed 50MHz
// min processor clock
// - Datasheet 4.1.3, Table 10: "Overdrive" run mode requires 1.25V core voltage
// minimum; 528 MHz does not.
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
  SETBITS(CCM->CSCMR1, CCM_CSCMR1_PERCLK_PODF_MASK,
          CCM_CSCMR1_PERCLK_PODF(1));  // Set PERCLK divider /2 (75MHz)
  // Set clock to 600 MHz. Power PLL on and configure divider
  SETBITS(CCM_ANALOG->PLL_ARM,
          CCM_ANALOG_PLL_ARM_POWERDOWN_MASK |
              CCM_ANALOG_PLL_ARM_SET_DIV_SELECT_MASK,
          CCM_ANALOG_PLL_ARM_SET_DIV_SELECT(100));
  while ((CCM_ANALOG->PLL_ARM & CCM_ANALOG_PLL_ARM_LOCK_MASK) == 0)
    spin(1);  // wait until it is stable
  CCM_ANALOG->PLL_ARM &=
      ~CCM_ANALOG_PLL_ARM_BYPASS_MASK;  // Disable Bypass (switch to PLL)
  // 14.5 Table 14-4: uart_clk_root
  // 14.4: uart_clk_root = PLL3/6 or OSC; CCM_CSCDR1 (14.7.9) defaults to
  // PLL3/6/1
  CCM_ANALOG->PLL_USB1 |= CCM_ANALOG_PLL_USB1_POWER_MASK;  // Power PLL on
  while ((CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_LOCK_MASK) == 0)
    spin(1);  // wait until it is stable
  CCM_ANALOG->PLL_USB1 &=
      ~CCM_ANALOG_PLL_USB1_BYPASS_MASK;  // Disable Bypass (switch to PLL)
  rng_init();  // Initialise random number generator
  // NXP startup code calls SystemInit BEFORE initializing RAM...
  SysTick_Config(SYS_FREQUENCY / 1000);  // Sys tick every 1ms
}
