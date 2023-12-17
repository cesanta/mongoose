// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved
//
// This file contains essentials required by the CMSIS:
// uint32_t SystemCoreClock - holds the system core clock value
// SystemInit() - initialises the system, e.g. sets up clocks
//
// Vendor CMSIS usually contain system_XXXX.c file with SystemInit()
// We don't use it - instead, we provide our own SystemInit().

#include "hal.h"

uint32_t SystemCoreClock;  // Holds system core clock in Hz. Required by CMSIS

void SystemInit(void) {             // Called automatically by startup code
  clock_init();                     // Core clock to 480 MHz
  rng_init();                       // Initialise random number generator
  SystemCoreClock = CPU_FREQUENCY;  // defined in hal.h
  SysTick_Config(SystemCoreClock / 1000);  // Sys tick every 1ms
}
