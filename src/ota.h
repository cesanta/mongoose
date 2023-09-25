// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#pragma once

#include "arch.h"

#define MG_OTA_NONE 0      // No OTA support
#define MG_OTA_FLASH 1     // OTA via an internal flash
#define MG_OTA_CUSTOM 100  // Custom implementation

#ifndef MG_OTA
#define MG_OTA MG_OTA_NONE
#endif

// Firmware update API
bool mg_ota_begin(size_t new_firmware_size);     // Start writing
bool mg_ota_write(const void *buf, size_t len);  // Write chunk, aligned to 1k
bool mg_ota_end(void);                           // Stop writing

enum {
  MG_OTA_UNAVAILABLE = 0,  // No OTA information is present
  MG_OTA_FIRST_BOOT = 1,   // Device booting the first time after the OTA
  MG_OTA_UNCOMMITTED = 2,  // Ditto, but marking us for the rollback
  MG_OTA_COMMITTED = 3,    // The firmware is good
};
enum { MG_FIRMWARE_CURRENT = 0, MG_FIRMWARE_PREVIOUS = 1 };

int mg_ota_status(int firmware);          // Return firmware status MG_OTA_*
uint32_t mg_ota_crc32(int firmware);      // Return firmware checksum
uint32_t mg_ota_timestamp(int firmware);  // Firmware timestamp, UNIX UTC epoch
size_t mg_ota_size(int firmware);         // Firmware size

bool mg_ota_commit(void);    // Commit current firmware
bool mg_ota_rollback(void);  // Rollback to the previous firmware
