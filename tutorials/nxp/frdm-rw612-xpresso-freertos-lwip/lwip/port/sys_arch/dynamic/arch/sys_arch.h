/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
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
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*
 * Copyright (c) 2013-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017,2020,2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __ARCH_SYS_ARCH_H__
#define __ARCH_SYS_ARCH_H__

#include "lwip/opt.h"
#include "arch/cc.h"

#if !NO_SYS

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#endif

#define SYS_MBOX_NULL                  ((QueueHandle_t)NULL)
#define SYS_SEM_NULL                   ((SemaphoreHandle_t)NULL)
#define SYS_DEFAULT_THREAD_STACK_DEPTH configMINIMAL_STACK_SIZE
#if !NO_SYS
typedef SemaphoreHandle_t sys_sem_t;
typedef SemaphoreHandle_t sys_mutex_t;
typedef QueueHandle_t sys_mbox_t;
typedef TaskHandle_t sys_thread_t;

#define sys_mbox_valid(x)       (((*x) == NULL) ? pdFALSE : pdTRUE)
#define sys_mbox_set_invalid(x) ((*x) = NULL)
#define sys_sem_valid(x)        (((*x) == NULL) ? pdFALSE : pdTRUE)
#define sys_sem_set_invalid(x)  ((*x) = NULL)

#else /* NO_SYS */ /* Bare-metal */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void time_isr(void);
void time_init(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif

typedef unsigned long sys_prot_t;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void sys_assert(const char *pcMessage);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __ARCH_SYS_ARCH_H__ */
