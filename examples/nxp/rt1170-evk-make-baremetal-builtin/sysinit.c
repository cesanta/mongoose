// Copyright (c) 2024 Cesanta Software Limited
// All rights reserved
//
// This file contains essentials required by the CMSIS:
// uint32_t SystemCoreClock - holds the system core clock value
// SystemInit() - initialises the system, e.g. sets up clocks

#include "hal.h"
#include "fsl_clock.h"
#include "fsl_dcdc.h"
#include "fsl_pmu.h"

uint32_t SystemCoreClock = SYS_FREQUENCY;

// - 15.4 clock tree
// 15.4: M7_CLK_ROOT
// 15.5.3 Table 15-4; 15.9.1.2; select PLL_ARM_CLK
// 15.8.2 PLL Enable Sequence
// - Datasheet 4.1.3, Table 11: "Overdrive" run mode particulars
void SystemInit(void) {  // Called automatically by startup code (ints masked)
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // Enable FPU
  asm("DSB");
  asm("ISB");
  // 79.5.2: Disable watchdogS after reset (unlocked)
  RTWDOG3->CS &= ~RTWDOG_CS_EN_MASK;
  RTWDOG3->TOVAL = 0xFFFF;
  while (RTWDOG3->CS & RTWDOG_CS_ULK_MASK) spin(1);  // wait for lock
  while ((RTWDOG3->CS & RTWDOG_CS_RCS_MASK) == 0)
    spin(1);  // wait for new config
  RTWDOG4->CS &= ~RTWDOG_CS_EN_MASK;
  RTWDOG4->TOVAL = 0xFFFF;
  while (RTWDOG4->CS & RTWDOG_CS_ULK_MASK) spin(1);  // wait for lock
  while ((RTWDOG4->CS & RTWDOG_CS_RCS_MASK) == 0)
    spin(1);  // wait for new config
  ANADIG_OSC->OSC_24M_CTRL |= ANADIG_OSC_OSC_24M_CTRL_OSC_EN(1) | ANADIG_OSC_OSC_24M_CTRL_LP_EN(1);
  while ((ANADIG_OSC->OSC_24M_CTRL & ANADIG_OSC_OSC_24M_CTRL_OSC_24M_STABLE_MASK) == 0)
    spin(1);  // wait until it is stable
  DCDC_BootIntoDCM(DCDC);  // Enable "overdrive" mode as needed
  DCDC_SetVDD1P0BuckModeTargetVoltage(DCDC, kDCDC_1P0BuckTarget1P125V);
  PMU_EnableBodyBias(ANADIG_PMU, kPMU_FBB_CM7, (OCOTP->FUSEN[7].FUSE & BIT(4)) == 0);
  const clock_arm_pll_config_t armpll =  {.loopDivider = 166, .postDivider = 0};
  CLOCK_InitArmPll(&armpll);  // Set clock to 996MHz
  CCM->CLOCK_ROOT[0].CONTROL = CCM_CLOCK_ROOT_CONTROL_MUX(4); // /1

  // rng_init();  // TRNG is part or CAAM and there is no info on that
  // NXP startup code calls SystemInit BEFORE initializing RAM...
  SysTick_Config(SYS_FREQUENCY / 1000);  // Sys tick every 1ms
}
