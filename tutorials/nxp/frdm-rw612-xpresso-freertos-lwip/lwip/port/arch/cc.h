/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 * Author: Stefano Oliveri <stefano.oliveri@st.com>
 *
 */

/*
 * Copyright (c) 2013-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018, 2020-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __CC_H__
#define __CC_H__

#ifdef __cplusplus
extern "C" {
#endif

// FSL
#ifdef __REDLIB__
#define LWIP_NO_INTTYPES_H 1
#endif

#ifndef LWIP_TIMEVAL_PRIVATE
/* This include is used just to get access to __NEWLIB__ macro when NEWLIB is
 * used. The header stdlib.h is contained in all C libraries currently supported
 * by MCUXpresso SDK. */
#include <stdlib.h>
#ifdef __NEWLIB__
#define LWIP_TIMEVAL_PRIVATE 0
#else
#define LWIP_TIMEVAL_PRIVATE 1
#endif /* __NEWLIB__ */
#endif /* LWIP_TIMEVAL_PRIVATE */

#if (LWIP_TIMEVAL_PRIVATE == 0)
#include <sys/time.h>
#endif

#ifndef LWIP_NO_STDINT_H
#define LWIP_NO_STDINT_H 0
#endif

// Typedefs for the types used by lwip
#if LWIP_NO_STDINT_H
typedef unsigned char u8_t;
typedef signed char s8_t;
typedef unsigned short u16_t;
typedef signed short s16_t;
typedef unsigned int u32_t;
typedef signed int s32_t;
typedef u32_t mem_ptr_t;
#endif

// Compiler hints for packing lwip's structures
// FSL: very important at high optimization level

#if defined(__arm__) && defined(__ARMCC_VERSION)

#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT __attribute__((packed, aligned(1)))
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x) __attribute__((packed, aligned(1))) x

#elif defined(__GNUC__) && (__GNUC__ != 0)

#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT __attribute__((__packed__))
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x) x

#elif defined(__IAR_SYSTEMS_ICC__)

#define PACK_STRUCT_BEGIN _Pragma("pack(1)")
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_END _Pragma("pack()")
#define PACK_STRUCT_FIELD(x) x

#else

#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x) x

#endif

// Platform specific diagnostic output
// FSL
#include "lwipopts.h"

#include "cmsis_compiler.h"

#ifndef LWIP_PLATFORM_DIAG
#include "fsl_debug_console.h"

// non-fatal, print a message.
#define LWIP_PLATFORM_DIAG(x)                                                  \
  do {                                                                         \
    PRINTF x;                                                                  \
  } while (0)
#endif /* LWIP_PLATFORM_DIAG */

#include "arch/sys_arch.h"

// fatal, print message and abandon execution.
#define LWIP_PLATFORM_ASSERT(x) sys_assert(x)

#ifdef __cplusplus
}
#endif

#endif /* __CC_H__ */
