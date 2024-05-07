// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#pragma once

#include "arch.h"

#define MG_DEVICE_NONE 0  // Dummy system

#define MG_DEVICE_STM32H5 1     // STM32 H5
#define MG_DEVICE_STM32H7 2     // STM32 H7
#define MG_DEVICE_CH32V307 100  // WCH CH32V307
#define MG_DEVICE_U2A 200       // Renesas U2A16, U2A8, U2A6
#define MG_DEVICE_RT1020 300    // IMXRT1020
#define MG_DEVICE_RT1060 301    // IMXRT1060
#define MG_DEVICE_CUSTOM 1000   // Custom implementation

#ifndef MG_DEVICE
#define MG_DEVICE MG_DEVICE_NONE
#endif

// Flash information
void *mg_flash_start(void);         // Return flash start address
size_t mg_flash_size(void);         // Return flash size
size_t mg_flash_sector_size(void);  // Return flash sector size
size_t mg_flash_write_align(void);  // Return flash write align, minimum 4
int mg_flash_bank(void);            // 0: not dual bank, 1: bank1, 2: bank2

// Write, erase, swap bank
bool mg_flash_write(void *addr, const void *buf, size_t len);
bool mg_flash_erase(void *sector);
bool mg_flash_swap_bank(void);

// Convenience functions to store data on a flash sector with wear levelling
// If `sector` is NULL, then the last sector of flash is used
bool mg_flash_load(void *sector, uint32_t key, void *buf, size_t len);
bool mg_flash_save(void *sector, uint32_t key, const void *buf, size_t len);

void mg_device_reset(void);  // Reboot device immediately
