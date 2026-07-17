#pragma once

#if MG_ARCH == MG_ARCH_ESP32

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <netdb.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include <esp_ota_ops.h>   // Use angle brackets to avoid
#include <esp_timer.h>     // amalgamation ditching them
#include <esp_system.h>
#include <esp_mac.h>
#include <esp_idf_version.h>
#include <esp_ota_ops.h>
#include <esp_task_wdt.h>
#include <soc/soc.h>
#include <soc/rtc_cntl_reg.h>

#define MG_PATH_MAX 128

#ifndef MG_ENABLE_POSIX_FS
#define MG_ENABLE_POSIX_FS 1
#endif

#ifndef MG_ENABLE_DIRLIST
#define MG_ENABLE_DIRLIST 1
#endif

#ifndef MG_OTA_ESP32_WDT_TIMEOUT_MS
#define MG_OTA_ESP32_WDT_TIMEOUT_MS 10000U
#endif

static inline void mg_ota_esp32_timer_start(void) {
#if ESP_IDF_VERSION_MAJOR >= 5
  esp_task_wdt_config_t cfg = {0};
  esp_err_t err;

  cfg.timeout_ms = MG_OTA_ESP32_WDT_TIMEOUT_MS;
  cfg.idle_core_mask = 0;
  cfg.trigger_panic = true;

  err = esp_task_wdt_init(&cfg);
  if (err == ESP_ERR_INVALID_STATE) esp_task_wdt_reconfigure(&cfg);
#else
  esp_task_wdt_init(MG_OTA_ESP32_WDT_TIMEOUT_MS / 1000U, true);
#endif
  esp_task_wdt_add(NULL);
  esp_task_wdt_reset();
}

static inline uint32_t mg_ota_esp32_state_set(uint32_t state) {
  if (state == 0) esp_ota_mark_app_valid_cancel_rollback();
  REG_WRITE(RTC_CNTL_STORE0_REG, state);
  return REG_READ(RTC_CNTL_STORE0_REG);
}

#define MG_OTA_STATE_GET() \
  REG_READ(RTC_CNTL_STORE0_REG)

#define MG_OTA_STATE_SET(v) mg_ota_esp32_state_set((uint32_t) (v))

#ifndef MG_OTA_ROLLBACK_TIMER_START
#define MG_OTA_ROLLBACK_TIMER_START() mg_ota_esp32_timer_start()
#endif

#ifndef MG_OTA_ROLLBACK_TIMER_FEED
#define MG_OTA_ROLLBACK_TIMER_FEED() \
  do {                               \
    (void) esp_task_wdt_reset();     \
  } while (0)
#endif

#ifndef MG_OTA_ROLLBACK
#define MG_OTA_ROLLBACK()                                  \
  do {                                                     \
    (void) esp_ota_mark_app_invalid_rollback_and_reboot(); \
  } while (0)
#endif

#endif
