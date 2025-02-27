/*
 * Copyright 2014-2016 Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __FSL_DEVICE_REGISTERS_H__
#define __FSL_DEVICE_REGISTERS_H__

/*
 * Include the cpu specific register header files.
 *
 * The CPU macro should be declared in the project or makefile.
 */
#if (defined(CPU_MIMXRT1062CVJ5A) || defined(CPU_MIMXRT1062CVJ5B) || defined(CPU_MIMXRT1062CVL5A) || \
    defined(CPU_MIMXRT1062CVL5B) || defined(CPU_MIMXRT1062DVJ6A) || defined(CPU_MIMXRT1062DVJ6B) || \
    defined(CPU_MIMXRT1062DVL6A) || defined(CPU_MIMXRT1062DVL6B) || defined(CPU_MIMXRT1062DVN6B) || \
    defined(CPU_MIMXRT1062XVN5B))

#define MIMXRT1062_SERIES

/* CMSIS-style register definitions */
#include "MIMXRT1062.h"
/* CPU specific feature definitions */
#include "MIMXRT1062_features.h"

#else
    #error "No valid CPU defined!"
#endif

#endif /* __FSL_DEVICE_REGISTERS_H__ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
