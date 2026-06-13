// Firmware OTA updates
//
// This is a low-level OTA API, not intended to be called directly by users.
// Users are provided with a higher-level API:
// - defining MG_OTA_URL enables HTTP pull-based updates
// - mg_http_start_ota() enables push-based updates
//
// However, it is possible to use the mg_ota_* API directly. Below is the
// example code of a function that is called repeatedly. It expects the
// last chink of size 0, which marks the end of the OTA process
//
// ```c
// static void ota(size_t total, size_t offset, void *buf, size_t len) {
//   bool ok = false;
//   if (ofs == 0 && (ok = mg_ota_begin(total)) == false) {
//     mg_ota_end();
//   } else if (len > 0 && (ok = mg_ota_write(buf, len)) == false) {
//     mg_ota_end();
//   } else if (len == 0) {}
//     ok = mg_ota_end();
//   }
//   return ok;
// }
// ```

#pragma once

#include "arch.h"
#include "net.h"

// Supported OTA targets
#define MG_OTA_NONE 0               // No OTA support
#define MG_OTA_STM32H5 1            // STM32 H5
#define MG_OTA_STM32H7 2            // STM32 H7
#define MG_OTA_STM32H7_DUAL_CORE 3  // STM32 H7 dual core
#define MG_OTA_STM32F 4             // STM32 F7/F4/F2
#define MG_OTA_CH32V307 100         // WCH CH32V307
#define MG_OTA_U2A 200              // Renesas U2A16, U2A8, U2A6
#define MG_OTA_RT1020 300           // IMXRT1020
#define MG_OTA_RT1050 301           // IMXRT1050
#define MG_OTA_RT1060 302           // IMXRT1060
#define MG_OTA_RT1064 303           // IMXRT1064
#define MG_OTA_RT1170 304           // IMXRT1170
#define MG_OTA_MCXN 310             // MCXN947
#define MG_OTA_RW612 320            // FRDM-RW612
#define MG_OTA_FLASH 900            // OTA via internal flash
#define MG_OTA_ESP32 910            // ESP32 OTA implementation
#define MG_OTA_PICOSDK 920          // RP2040/2350 using Pico-SDK hardware_flash
#define MG_OTA_CUSTOM 1000          // Custom implementation

// OTA target. Settable in mongoose_config.h
#ifndef MG_OTA
#define MG_OTA MG_OTA_NONE
#else
#ifndef MG_IRAM
#if defined(__GNUC__)
// Places code block in .iram ELF section. A linker script can place it in RAM
#define MG_IRAM __attribute__((noinline, section(".iram")))
#else
#define MG_IRAM
#endif  // compiler
#endif  // IRAM
#endif  // OTA

// Starts the OTA process. Called once at the beginning of a firmware update.
bool mg_ota_begin(size_t new_firmware_size);

// Writes the next firmware chunk. Called repeatedly until all data is written.
bool mg_ota_write(const void *buf, size_t len);

// Ends the OTA process and commits the written firmware.
bool mg_ota_end(void);

// MG_OTA_ROLLBACK: swap firmware banks and reset. On flash-based OTA, the
// default calls swap_fn() (no-op on single-bank) then resets. Override in
// mongoose_config.h for platforms without mg_flash (ESP32, PicoSDK, etc.).
#ifndef MG_OTA_ROLLBACK
#define MG_OTA_ROLLBACK() \
  do {                    \
    mg_flash->swap_fn();  \
    NVIC_SystemReset();   \
  } while (0)
#endif

// Arm a hardware watchdog. mg_ota_poll() feeds it while in MG_OTA_TESTING state;
// if the event loop hangs, it fires, resets the device, and MG_OTA_FAILED
// triggers MG_OTA_ROLLBACK() on next boot. Timeout is arch-defined.
#ifndef MG_OTA_ROLLBACK_TIMER_START
#define MG_OTA_ROLLBACK_TIMER_START()
#endif

#ifndef MG_OTA_ROLLBACK_TIMER_FEED
#define MG_OTA_ROLLBACK_TIMER_FEED()
#endif

// OTA state values for MG_OTA_STATE_GET / MG_OTA_STATE_SET.
// MG_OTA_CONFIRMED:  firmware is committed, normal operation.
// MG_OTA_TESTING:    new firmware booted for the first time; IWDG is armed.
//                    If the device reboots while in this state, it transitions
//                    to MG_OTA_FAILED and rolls back on the next boot.
// MG_OTA_FAILED:     previous boot did not commit in time; rollback on next boot.
// MG_OTA_ROLLEDBACK: device rolled back to previous firmware. Sent to the OTA
//                    server as boot=3 on every poll until a new OTA succeeds,
//                    which overwrites this state with MG_OTA_TESTING.
enum {
  MG_OTA_CONFIRMED = 0,
  MG_OTA_TESTING = 1,
  MG_OTA_FAILED = 2,
  MG_OTA_ROLLEDBACK = 3
};

// Persistent OTA state storage. Override in mongoose_config.h to persist state across resets.
#ifndef MG_OTA_STATE_GET
#define MG_OTA_STATE_GET() 0
#endif
#ifndef MG_OTA_STATE_SET
#define MG_OTA_STATE_SET(val) (void) (val)
#endif

// Evaluation window for new firmware in seconds. mg_ota_poll() feeds the
// rollback watchdog while in MG_OTA_TESTING state and within this window;
// when it expires, auto-commits if the OTA server confirmed the same version,
// otherwise stops feeding and the watchdog triggers a rollback.
#ifndef MG_OTA_ROLLBACK_TIMEOUT_SECONDS
#define MG_OTA_ROLLBACK_TIMEOUT_SECONDS 120
#endif

// Call once at boot, after clock and backup-domain init (e.g. after HAL_Init()
// + SystemClock_Config() on CubeMX projects, or after hal_clock_init() on
// Mongoose tutorials). The backup domain must be write-accessible so the state
// transition TESTING → FAILED persists across resets.
// Arms the rollback watchdog if new firmware is in TESTING state, rolls back
// immediately if FAILED. mg_ota_poll() feeds the watchdog and auto-commits
// when the server confirms the same version within the window.
// boot (TESTING) → set FAILED → arm IWDG → run firmware
//  ├── server ok + window expires → set CONFIRMED → IWDG resets cleanly
//  └── no server ok + window expires → stop feeding → IWDG fires → FAILED → rollback
#define MG_OTA_BOOT_CHECK()                                        \
  do {                                                             \
    if (MG_OTA_STATE_GET() == MG_OTA_FAILED) {                     \
      MG_OTA_STATE_SET(MG_OTA_ROLLEDBACK);                         \
      MG_INFO(("Commit deadline expired, rolling back"));          \
      MG_OTA_ROLLBACK();                                           \
    } else if (MG_OTA_STATE_GET() == MG_OTA_TESTING) {             \
      MG_OTA_STATE_SET(MG_OTA_FAILED);                             \
      MG_INFO(("New firmware: commit within %u sec or rolls back", \
               (unsigned) MG_OTA_ROLLBACK_TIMEOUT_SECONDS));       \
      MG_OTA_ROLLBACK_TIMER_START();                               \
    }                                                              \
  } while (0)

// Pull-based OTA over HTTP. Fetches metadata_url, which must return a JSON
// object like: { "version": "1.2.3", "url": "FIRMWARE_URL", "size": 324645 }
// If the server version differs from MG_OTA_FIRMWARE_VERSION, downloads
// FIRMWARE_URL and performs the OTA update. fn is called on every outcome:
// NULL on successful flash, "Same version" when already up to date, or an
// error string on failure. Pass NULL to use the default handler which logs
// failures via MG_ERROR.
void mg_ota_url_check(struct mg_mgr *mgr, const char *metadata_url,
                      void (*fn)(const char *error_message));

// Firmware info URL for mg_ota_poll() which calls mg_ota_url_check().
// Example:  "http://mongoose.ws/ota/u/0/ota.json". See http://mongoose.ws/ota/
// Settable in mongoose_config.h
#ifndef MG_OTA_URL
#define MG_OTA_URL NULL
#endif

// OTA status callback function for mg_ota_poll()
#ifndef MG_OTA_STATUS_FN
#define MG_OTA_STATUS_FN NULL
#endif

// Firmware version for mg_ota_poll(). Settable in mongoose_config.h
#ifndef MG_OTA_FIRMWARE_VERSION
#define MG_OTA_FIRMWARE_VERSION "1.0.0"
#endif

// Maximum version string length
#ifndef MG_OTA_MAX_VERSION_LEN
#define MG_OTA_MAX_VERSION_LEN 64
#endif

// How often mg_ota_poll() checks for a firmware update, in seconds
#ifndef MG_OTA_PULL_INTERVAL_SECONDS
#define MG_OTA_PULL_INTERVAL_SECONDS 60
#endif

// Set to 1 to define a custom mg_ota_device_id(). Settable in mongoose_config.h
#ifndef MG_ENABLE_CUSTOM_DEVICE_ID
#define MG_ENABLE_CUSTOM_DEVICE_ID 0
#endif

// Populates buf with a unique device ID string.
// E.g. on STM32, uses the 96-bit MCU UID converted to a hex string.
void mg_ota_device_id(char *buf, size_t len);

// Checks for a firmware update over HTTP. Called automatically by mg_mgr_poll()
// when MG_OTA_URL is set in mongoose_config.h. Do not call directly.
void mg_ota_poll(struct mg_mgr *);
