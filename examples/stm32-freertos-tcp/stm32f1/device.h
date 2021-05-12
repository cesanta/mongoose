// Copyright (c) 2018-2021 Cesanta Software Limited
// All rights reserved

#include "mcu.h"

// These are NUCLEO-F103RB settings - adjust for your specific board
#define RAM_SIZE 20480    // RAM size on this device, needed by link.ld
#define ROM_SIZE 131072   // Flash size for this device, needed by link.ld
#define LED1 PIN('A', 5)  // On-board LED pin

static inline void led_toggle(void) {
  gpio_toggle(LED1);
}

static inline void init_hardware(void) {
  RCC->CR |= (RCC_CR_HSEON);
  while (!(RCC->CR & RCC_CR_HSERDY)) (void) 0;
  RCC->CFGR &= ~(RCC_CFGR_SW);
  RCC->CFGR |= (RCC_CFGR_SW_HSE);
  RCC->CFGR &= ~(RCC_CFGR_PLLMULL);
  RCC->CFGR |= (RCC_CFGR_PLLMULL9);
  RCC->CR |= (RCC_CR_PLLON);
  while (!(RCC->CR & RCC_CR_PLLRDY)) (void) 0;
  RCC->CFGR &= ~(RCC_CFGR_SW);
  RCC->CFGR |= (RCC_CFGR_SW_PLL);

  RCC->APB2ENR |= BIT(2) | BIT(3) | BIT(4);  // Init GPIO banks A,B,C
  gpio_init(LED1, OUTPUT);                   // Set LED
}
