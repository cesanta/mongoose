/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __httpsrv_port_h__
#define __httpsrv_port_h__

#include "FreeRTOS.h"

#define httpsrv_mem_alloc(x) pvPortMalloc(x)
#define httpsrv_mem_free(x)  vPortFree(x)

#endif
