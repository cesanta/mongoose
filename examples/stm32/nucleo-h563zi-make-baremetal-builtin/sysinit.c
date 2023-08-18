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
  RCC->CR = RCC_CR_HSION;  // Clear HSI clock divisor. SYS clock 64Mhz
  while ((RCC->CR & RCC_CR_HSIRDY) == 0) spin(1);  // Wait until done
  rng_init();                            // Initialise random number generator
  SysTick_Config(CPU_FREQUENCY / 1000);  // Sys tick every 1ms
}
