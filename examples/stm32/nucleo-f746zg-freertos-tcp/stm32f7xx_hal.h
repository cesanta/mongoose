#pragma once

// provide some STM32 HAL definitions needed by the ETH module without actually
// including STM32 HAL

#define HAL_ETH_MODULE_ENABLED
#define ETH_RX_BUF_SIZE ETH_MAX_PACKET_SIZE
#define ETH_TX_BUF_SIZE ETH_MAX_PACKET_SIZE
#define ETH_RXBUFNB ((uint32_t) 8)
#define ETH_TXBUFNB ((uint32_t) 4)

#include "hal.h"
#include "stm32f7xx.h"

typedef enum {
  HAL_OK = 0,
  HAL_ERROR = 1,
  HAL_BUSY = 2,
  HAL_TIMEOUT = 3
} HAL_StatusTypeDef;

typedef enum { HAL_UNLOCKED = 0, HAL_LOCKED = 1 } HAL_LockTypeDef;

#define __HAL_LOCK(h)                             \
  do {                                            \
    if ((h)->Lock == HAL_LOCKED) return HAL_BUSY; \
    (h)->Lock = HAL_LOCKED;                       \
  } while (0)

#define __HAL_UNLOCK(h)       \
  do {                        \
    (h)->Lock = HAL_UNLOCKED; \
  } while (0)

#ifndef __weak
#define __weak __attribute__((weak))
#endif
#ifndef __packed
#define __packed __attribute__((__packed__))
#endif

#define assert_param(expr) ((void) 0U)

#define PHY_READ_TO ((uint32_t) 0x0000FFFF)
#define PHY_WRITE_TO ((uint32_t) 0x0000FFFF)

// emulate STM32 HAL function calls

// we already enabled it in SystemInit() in sysinit.c
#define __HAL_RCC_SYSCFG_CLK_ENABLE()

// we set SystemCoreClock = SYS_FREQUENCY in sysinit.c
static inline uint32_t HAL_RCC_GetHCLKFreq(void) {
  return SYS_FREQUENCY;
}

static inline uint32_t HAL_GetTick(void) {
  return ticks_get();
}
