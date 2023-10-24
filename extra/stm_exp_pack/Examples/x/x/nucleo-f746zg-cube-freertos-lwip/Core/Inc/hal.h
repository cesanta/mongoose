// Copyright (c) 2022-23 Cesanta Software Limited
// All rights reserved
// https://www.st.com/resource/en/reference_manual/dm00124865-stm32f75xxx-and-stm32f74xxx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf

#pragma once

#include <stm32f746xx.h>

#ifndef UART_DEBUG
#define UART_DEBUG USART3
#endif


#define UUID ((uint8_t *) UID_BASE)  // Unique 96-bit chip ID. TRM 41.1

// Helper macro for MAC generation
#define GENERATE_LOCALLY_ADMINISTERED_MAC()                        \
  {                                                                \
    2, UUID[0] ^ UUID[1], UUID[2] ^ UUID[3], UUID[4] ^ UUID[5],    \
        UUID[6] ^ UUID[7] ^ UUID[8], UUID[9] ^ UUID[10] ^ UUID[11] \
  }
