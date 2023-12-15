// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved
//
// This file contains essentials required by the CMSIS:
// uint32_t SystemCoreClock - holds the system core clock value
// SystemInit() - initialises the system, e.g. sets up clocks

#include "hal.h"

uint32_t SystemCoreClock = CPU_FREQUENCY;

static inline unsigned int div2prescval(unsigned int div) {
  // 0 --> /1; 8 --> /2 ... 11 --> /16;  12 --> /64 ... 15 --> /512
  if (div == 1) return 0;
  if (div > 16) div /= 2;
  unsigned int val = 7;
  while (div >>= 1) ++val;
  return val;
}

static inline unsigned int pllrge(unsigned int f) {
  unsigned int val = 0;
  while (f >>= 1) ++val;
  return val - 1;
}

void SystemInit(void) {  // Called automatically by startup code
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // Enable FPU
  __DSB();
  __ISB();
  // Set flash latency. RM0481, section 7.11.1, section 7.3.4 table 37
  SETBITS(FLASH->ACR, (FLASH_ACR_WRHIGHFREQ_Msk | FLASH_ACR_LATENCY_Msk),
          FLASH_ACR_LATENCY_7WS | FLASH_ACR_WRHIGHFREQ_1);
  SETBITS(
      RCC->D1CFGR, (0x0F << 8) | (7 << 4) | (0x0F << 0),
      (div2prescval(D1CPRE) << 8) | (D1PPRE << 4) | (div2prescval(HPRE) << 0));
  RCC->D2CFGR = (D2PPRE2 << 8) | (D2PPRE1 << 4);
  RCC->D3CFGR = (D3PPRE << 4);
  SETBITS(RCC->PLLCFGR, 3 << 2,
          pllrge(PLL1_HSI / PLL1_M)
              << 2);  // keep reset config (DIVP1EN, !PLL1VCOSEL), PLL1RGE
  SETBITS(RCC->PLL1DIVR, (0x7F << 9) | (0x1FF << 0),
          ((PLL1_P - 1) << 9) | ((PLL1_N - 1) << 0));  // Set PLL1_P PLL1_N
  SETBITS(RCC->PLLCKSELR, 0x3F << 4,
          PLL1_M << 4);  // Set PLL1_M (source defaults to HSI)
  RCC->CR |= BIT(24);    // Enable PLL1
  while ((RCC->CR & BIT(25)) == 0) spin(1);  // Wait until done
  RCC->CFGR |= (3 << 0);                     // Set clock source to PLL1
  while ((RCC->CFGR & (7 << 3)) != (3 << 3)) spin(1);  // Wait until done
  RCC->APB4ENR |= RCC_APB4ENR_SYSCFGEN;                // Enable SYSCFG
  rng_init();
  SysTick_Config(CPU_FREQUENCY / 1000);
}
