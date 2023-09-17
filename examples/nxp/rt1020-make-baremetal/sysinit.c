// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved
//
// This file contains essentials required by the CMSIS:
// uint32_t SystemCoreClock - holds the system core clock value
// SystemInit() - initialises the system, e.g. sets up clocks

#include "hal.h"

uint32_t SystemCoreClock = SYS_FREQUENCY;

void SystemInit(void) {  // Called automatically by startup code
  rng_init();            // Initialise random number generator
  SysTick_Config(SystemCoreClock / 1000);  // Sys tick every 1ms
}

void _start(void) {
  extern void main(void);
  main();
  for (;;) (void) 0;
}

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
