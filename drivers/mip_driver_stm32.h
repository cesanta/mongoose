// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#pragma once

#include <stdbool.h>
#include <stddef.h>

void mip_driver_stm32_init(void *userdata);
bool mip_driver_stm32_status(void *);
void mip_driver_stm32_setrx(void (*rx)(void *, size_t, void *), void *);
size_t mip_driver_stm32_tx(const void *buf, size_t len, void *);
