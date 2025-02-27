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
  FLASH->ACR |= FLASH_LATENCY | BIT(8) | BIT(9);      // Flash latency, caches
  RCC->PLLCFGR &= ~((BIT(17) - 1) | (15U << 24));     // Clear PLL multipliers
  RCC->PLLCFGR |= (((PLL_P - 2) / 2) & 3) << 16;      // Set PLL_P
  RCC->PLLCFGR |= PLL_M | (PLL_N << 6) | (PLL_Q << 24);  // Set PLL_M and PLL_N
  RCC->CR |= BIT(24);                                    // Enable PLL
  while ((RCC->CR & BIT(25)) == 0) spin(1);              // Wait until done
  RCC->CFGR = (APB1_PRE << 10) | (APB2_PRE << 13);       // Set prescalers
  RCC->CFGR |= 2;                         // Set clock source to PLL
  while ((RCC->CFGR & 12) == 0) spin(1);  // Wait until done

  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;    // Enable SYSCFG
  rng_init();                              // Initialise random number generator
  SysTick_Config(SystemCoreClock / 1000);  // Sys tick every 1ms
}
