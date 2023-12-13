// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved
//
// This file contains essentials required by the CMSIS:
// uint32_t SystemCoreClock - holds the system core clock value
// SystemInit() - initialises the system, e.g. sets up clocks

#include "hal.h"

uint32_t SystemCoreClock = CPU_FREQUENCY;

void SystemInit(void) {  // Called automatically by startup code
  SCB->CPACR |= ((3UL << 20U) | (3UL << 22U));  // Enable FPU
  asm("DSB");
  asm("ISB");

  // Set flash latency. RM0481, section 7.11.1, section 7.3.4 table 37
  SETBITS(FLASH->ACR, (FLASH_ACR_WRHIGHFREQ_Msk | FLASH_ACR_LATENCY_Msk),
          FLASH_ACR_LATENCY_5WS | FLASH_ACR_WRHIGHFREQ_1);

  if (ldo_is_on()) {
    PWR->VOSCR = PWR_VOSCR_VOS_0 | PWR_VOSCR_VOS_1;  // Select VOS0
  } else {
    PWR->VOSCR = PWR_VOSCR_VOS_1;  // Select VOS1
  }
  uint32_t f = PWR->VOSCR;  // fake read to wait for bus clocking
  while ((PWR->VOSSR & PWR_VOSSR_ACTVOSRDY) == 0) spin(1);
  (void) f;
  RCC->CR = RCC_CR_HSION;                          // Clear HSI clock divisor
  while ((RCC->CR & RCC_CR_HSIRDY) == 0) spin(1);  // Wait until done
  RCC->CFGR2 = (PPRE3 << 12) | (PPRE2 << 8) | (PPRE1 << 4) | (HPRE << 0);
  RCC->PLL1DIVR =
      ((PLL1_P - 1) << 9) | ((PLL1_N - 1) << 0);  // Set PLL1_P PLL1_N
  // Enable P and Q divider outputs; set PLL1_M, select HSI as source,
  // !PLL1VCOSEL, PLL1RGE=0
  RCC->PLL1CFGR =
      RCC_PLL1CFGR_PLL1QEN | RCC_PLL1CFGR_PLL1PEN | (PLL1_M << 8) | (1 << 0);
  RCC->CR |= RCC_CR_PLL1ON;                         // Enable PLL1
  while ((RCC->CR & RCC_CR_PLL1RDY) == 0) spin(1);  // Wait until done
  RCC->CFGR1 |= (3 << 0);                           // Set clock source to PLL1
  while ((RCC->CFGR1 & (7 << 3)) != (3 << 3)) spin(1);  // Wait until done

  rng_init();                            // Initialise random number generator
  SysTick_Config(CPU_FREQUENCY / 1000);  // Sys tick every 1ms
}
