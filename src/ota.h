// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#pragma once

#include "arch.h"

#define MG_OTA_NONE 0      // No OTA support
#define MG_OTA_STM32H5 1   // STM32 H5 series
#define MG_OTA_CUSTOM 100  // Custom implementation

#define MG_OTA_MAGIC 0xb07afeed

#ifndef MG_OTA
#define MG_OTA MG_OTA_NONE
#endif

// Firmware update API
bool mg_ota_begin(size_t new_firmware_size);     // Start writing
bool mg_ota_write(const void *buf, size_t len);  // Write firmware chunk
bool mg_ota_end(void);                           // Stop writing
void mg_sys_reset(void);                         // Reboot device

struct mg_ota_data {
  uint32_t magic;   // Must be MG_OTA_MAGIC
  uint32_t crc32;   // Checksum of the current firmware
  uint32_t size;    // Firware size
  uint32_t time;    // Flashing timestamp. Unix epoch, seconds since 1970
  uint32_t booted;  // -1: not yet booted before, otherwise booted
  uint32_t golden;  // -1: not yet comitted, otherwise clean, committed
};

bool mg_ota_status(struct mg_ota_data[2]);  // Get status for curr and prev fw
bool mg_ota_commit(void);                   // Commit current firmware
bool mg_ota_rollback(void);                 // Rollback to prev firmware
