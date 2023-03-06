// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved
//
// This file contains essentials required by the CMSIS:
// uint32_t SystemCoreClock - holds the system core clock value
// SystemInit() - initialises the system, e.g. sets up clocks

#include "hal.h"

uint32_t SystemCoreClock = SYS_FREQUENCY;

void SystemInit(void) {  // Called automatically by startup code
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // Enable FPU
  asm("DSB");
  asm("ISB");
  SETBITS(SYSCTL->MOSCCTL, BIT(3) | BIT(2),
          BIT(4));  // Enable MOSC circuit (clear NOXTAL and PWRDN, set >10MHz
                    // range)
  SETBITS(SYSCTL->MEMTIM0,
          BIT(21) | BIT(5) | 0x1F << 21 | 0xF << 16 | 0x1F << 5 | 0xF << 0,
          FLASH_CLKHIGH << 22 | FLASH_WAITST << 16 | FLASH_CLKHIGH << 5 |
              FLASH_WAITST << 0);  // Configure flash timing (not yet applied)
  SETBITS(SYSCTL->RSCLKCFG, 0xF << 24 | (BIT(9) - 1),
          3 << 24);  // Clear PLL divider, set MOSC as PLL source
  SYSCTL->PLLFREQ1 = (PLL_Q - 1) << 8 | (PLL_N - 1)
                                            << 0;  // Set PLL_Q and PLL_N
  SYSCTL->PLLFREQ0 =
      BIT(23) | PLL_M << 0;  // Set PLL_Q, power up PLL (if it were on, we'd
                             // need to set NEWFREQ in RSCLKCFG instead)
  while ((SYSCTL->PLLSTAT & BIT(0)) == 0) spin(1);  // Wait for lock
  SYSCTL->RSCLKCFG |=
      BIT(31) | BIT(28) |
      (PSYSDIV - 1) << 0;  // Update memory timing, use PLL, set clock divisor

  SysTick_Config(SystemCoreClock / 1000);  // Sys tick every 1ms
}
