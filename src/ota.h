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

// MG_OTA_ROLLBACK_TIMER_START(seconds): arm a hardware watchdog as the rollback
// deadline. If it fires before MG_OTA_STATE_SET(MG_OTA_CONFIRMED), the device
// resets and MG_OTA_STATE_GET() == MG_OTA_FAILED triggers MG_OTA_ROLLBACK().
#ifndef MG_OTA_ROLLBACK_TIMER_START
#define MG_OTA_ROLLBACK_TIMER_START(seconds) (void) (seconds)
#endif

// OTA state values for MG_OTA_STATE_GET / MG_OTA_STATE_SET.
// MG_OTA_CONFIRMED: firmware is committed, normal operation.
// MG_OTA_TESTING:   new firmware booted for the first time; IWDG is armed.
//                   If the device reboots while in this state, it transitions
//                   to MG_OTA_FAILED and rolls back on the next boot.
// MG_OTA_FAILED:    previous boot did not commit in time; rollback on next boot.
enum { MG_OTA_CONFIRMED = 0, MG_OTA_TESTING = 1, MG_OTA_FAILED = 2 };

// Persistent OTA state storage. Override in mongoose_config.h to persist state across resets.
#ifndef MG_OTA_STATE_GET
#define MG_OTA_STATE_GET() 0
#endif
#ifndef MG_OTA_STATE_SET
#define MG_OTA_STATE_SET(val) (void) (val)
#endif

// Call once at boot. Arms the rollback watchdog for (seconds) seconds when
// the new firmware is in TESTING state. Rolls back immediately if FAILED.
// boot (TESTING) → set FAILED → arm IWDG → run firmware
//  ├── commit called → set CONFIRMED → NVIC_SystemReset() → clean boot
//  └── IWDG fires → hard reset → boot → state is still FAILED → rollback
#define MG_OTA_BOOT_CHECK(seconds)                                 \
  do {                                                             \
    if (MG_OTA_STATE_GET() == MG_OTA_FAILED) {                     \
      MG_OTA_STATE_SET(MG_OTA_CONFIRMED);                          \
      MG_INFO(("Commit deadline expired, rolling back"));          \
      MG_OTA_ROLLBACK();                                           \
    } else if (MG_OTA_STATE_GET() == MG_OTA_TESTING) {             \
      MG_OTA_STATE_SET(MG_OTA_FAILED);                             \
      MG_INFO(("New firmware: commit within %u sec or rolls back", \
               (unsigned) (seconds)));                             \
      MG_OTA_ROLLBACK_TIMER_START(seconds);                        \
    }                                                              \
  } while (0)

// Pull-based OTA over HTTP. Fetches the given URL, which should return a JSON
// object like: { "version": "1.2.3", "url": "FIRMWARE_URL", "size": 324645 }
// If the current firmware version differs from the JSON version, downloads
// FIRMWARE_URL and performs the OTA update.
void mg_ota_url_check(struct mg_mgr *mgr, const char *current_version,
                      const char *metadata_url, void (*fn)(const char *status));

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

// Scannable "MG_VERSION:<MG_OTA_FIRMWARE_VERSION>" tag embedded in every
// firmware binary, for server-side extraction. mg_mgr_init() stashes its
// address in mgr->userdata, which is what keeps it from being stripped by
// -Wl,--gc-sections on builds that never poll for OTAs
extern const char mg_fw_version[];

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
