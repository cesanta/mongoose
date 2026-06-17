#pragma once

#if MG_ARCH == MG_ARCH_PICOSDK
#if !defined(MG_ENABLE_LWIP) || !MG_ENABLE_LWIP
#include <errno.h>
#endif
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <pico/stdlib.h>
#include <pico/rand.h>
#include <pico/unique_id.h>
int mkdir(const char *, mode_t);

#if MG_OTA == MG_OTA_PICOSDK
#include <hardware/flash.h>
#include <hardware/watchdog.h>
#include <hardware/structs/watchdog.h>
#include <pico/bootrom.h>

#ifndef MG_OTA_WATCHDOG_MS
#if PICO_RP2040
#define MG_OTA_WATCHDOG_MS 8000U // max delay on RP2040 is 8.3 seconds
#else
#define MG_OTA_WATCHDOG_MS 10000U // max delay on RP2350 is 16 seconds
#endif
#endif

#ifndef MG_OTA_ROLLBACK_TIMER_START
#define MG_OTA_ROLLBACK_TIMER_START() watchdog_enable(MG_OTA_WATCHDOG_MS, false)
#endif

#ifndef MG_OTA_ROLLBACK_TIMER_FEED
#define MG_OTA_ROLLBACK_TIMER_FEED() watchdog_update()
#endif

#define MG_OTA_STATE_GET() ((uint32_t) watchdog_hw->scratch[0])
#define MG_OTA_STATE_SET(v) \
  (watchdog_hw->scratch[0] = (uint32_t) (v), (uint32_t) watchdog_hw->scratch[0])

#if PICO_RP2040
#define MG_OTA_ROLLBACK()                                      \
  do {                                                         \
    mg_flash->swap_fn();                                       \
    *(volatile unsigned long *) 0xe000ed0c = 0x5fa0004;         \
  } while (0)
#else
#define MG_OTA_ROLLBACK()                                      \
  do {                                                         \
    mg_flash->swap_fn();                                       \
    ((rom_reboot_fn) rom_func_lookup(ROM_FUNC_REBOOT))(         \
        BOOT_TYPE_NORMAL | 0x100, 1, 0, 0);                    \
  } while (0)
#endif
#endif

#endif
