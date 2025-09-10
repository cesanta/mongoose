/*
 *  Copyright 2008-2020, 2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */
/*!\file wmlog.h
 *\brief This file contains macros to print logs.
 */

#ifndef __WMLOG_H__
#define __WMLOG_H__

#include <osa.h>

#ifdef __ZEPHYR__
#include "nxp_wifi.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#ifndef PRINTF
#define PRINTF printk
#endif
#else
#include "fsl_debug_console.h"
#endif

#if CONFIG_ENABLE_ERROR_LOGS
#define wmlog_e(_mod_name_, _fmt_, ...) (void)PRINTF("[%s]%s" _fmt_ "\n\r", _mod_name_, " Error: ", ##__VA_ARGS__)
#else
#define wmlog_e(...)
#endif /* CONFIG_ENABLE_ERROR_LOGS */

#if CONFIG_ENABLE_WARNING_LOGS
#define wmlog_w(_mod_name_, _fmt_, ...) (void)PRINTF("[%s]%s" _fmt_ "\n\r", _mod_name_, " Warn: ", ##__VA_ARGS__)
#else
#define wmlog_w(...)
#endif /* CONFIG_ENABLE_WARNING_LOGS */

/* General debug function. User can map his own debug functions to this
ne */
#define wmlog(_mod_name_, _fmt_, ...) (void)PRINTF("[%s] " _fmt_ "\n\r", _mod_name_, ##__VA_ARGS__)

/* Function entry */
#define wmlog_entry(_fmt_, ...) (void)PRINTF("> %s (" _fmt_ ")\n\r", __func__, ##__VA_ARGS__)

/* function exit */
#define wmlog_exit(_fmt_, ...) (void)PRINTF("< %s" _fmt_ "\n\r", __func__, ##__VA_ARGS__)

#endif /* __WMLOG_H__ */
