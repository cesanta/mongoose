/*
** ###################################################################
**     Processors:          RW612ETA2I
**                          RW612HNA2I
**                          RW612UKA2I
**
**     Compilers:           GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**                          Keil ARM C/C++ Compiler
**                          MCUXpresso Compiler
**
**     Reference manual:    RW61X User manual Rev. 0.95, June 2022
**     Version:             rev. 1.0, 2021-03-16
**     Build:               b231201
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2023 NXP
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 1.0 (2021-03-16)
**         Initial version.
**
** ###################################################################
*/

/*!
 * @file RW612
 * @version 1.0
 * @date 2023-12-01
 * @brief Device specific configuration file for RW612 (header file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */
#ifndef _SYSTEM_RW612_H_
#define _SYSTEM_RW612_H_                    /**< Symbol preventing repeated inclusion */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define CLK_XTAL_OSC_CLK_38400KHZ 38400000u /* XTAL OSC frequency 38.4MHz */
#define CLK_XTAL_OSC_CLK_40000KHZ 40000000u /* XTAL OSC frequency 40MHz */

#ifndef CLK_XTAL_OSC_CLK
#define CLK_XTAL_OSC_CLK \
    (((AON_SOC_CIU->STRAP_RDBK & 0x20U) == 0U) ? CLK_XTAL_OSC_CLK_38400KHZ : CLK_XTAL_OSC_CLK_40000KHZ)
#endif
#define CLK_RTC_32K_CLK 32768u /* RTC oscillator 32 kHz (32k_clk) */
#ifndef CLK_EXT_CLKIN
#define CLK_EXT_CLKIN 0u /* Default external CLKIN pin clock */
#endif
#define DEFAULT_SYSTEM_CLOCK 260000000u /* Default System clock value */

/**
 * @brief System clock frequency (core clock)
 *
 * The system clock frequency supplied to the SysTick timer and the processor
 * core clock. This variable can be used by the user application to setup the
 * SysTick timer or configure other parameters. It may also be used by debugger to
 * query the frequency of the debug timer or configure the trace clock speed
 * SystemCoreClock is initialized with a correct predefined value.
 */
extern uint32_t SystemCoreClock;

/**
 * @brief Setup the microcontroller system.
 *
 * Typically this function configures the oscillator (PLL) that is part of the
 * microcontroller device. For systems with variable clock speed it also updates
 * the variable SystemCoreClock. SystemInit is called from startup_device file.
 */
void SystemInit(void);

/**
 * @brief Updates the SystemCoreClock variable.
 *
 * It must be called whenever the core clock is changed during program
 * execution. SystemCoreClockUpdate() evaluates the clock register settings and calculates
 * the current core clock.
 */
void SystemCoreClockUpdate(void);

/**
 * @brief SystemInit function hook.
 *
 * This weak function allows to call specific initialization code during the
 * SystemInit() execution.This can be used when an application specific code needs
 * to be called as close to the reset entry as possible (for example the Multicore
 * Manager MCMGR_EarlyInit() function call).
 * NOTE: No global r/w variables can be used in this hook function because the
 * initialization of these variables happens after this function.
 */
void SystemInitHook(void);

#ifdef __cplusplus
}
#endif

#endif /* _SYSTEM_RW612_H_ */
