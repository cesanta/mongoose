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
 * @brief Device specific configuration file for RW612 (implementation file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#include <stdint.h>
#include "fsl_device_registers.h"

#define SYSTEM_IS_XIP_FLEXSPI()                                                                               \
    ((((uint32_t)SystemCoreClockUpdate >= 0x08000000U) && ((uint32_t)SystemCoreClockUpdate < 0x10000000U)) || \
     (((uint32_t)SystemCoreClockUpdate >= 0x18000000U) && ((uint32_t)SystemCoreClockUpdate < 0x20000000U)))

#define CLOCK_KHZ(freq) ((freq)*1000UL)
#define CLOCK_MHZ(freq) (CLOCK_KHZ(freq) * 1000UL)

/* ----------------------------------------------------------------------------
   -- Core clock
   ---------------------------------------------------------------------------- */

uint32_t SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

/* ----------------------------------------------------------------------------
   -- SystemInit()
   ---------------------------------------------------------------------------- */

__attribute__((weak)) void SystemInit(void)
{
#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10, CP11 Full Access in Secure mode */
#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    SCB_NS->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10, CP11 Full Access in Non-secure mode */
#endif                                                    /* (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */
#endif                                                    /* ((__FPU_PRESENT == 1) && (__FPU_USED == 1)) */

    SCB->CPACR |= ((3UL << 0 * 2) | (3UL << 1 * 2)); /* set CP0, CP1 Full Access in Secure mode (enable PowerQuad) */

#if defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    SCB_NS->CPACR |=
        ((3UL << 0 * 2) | (3UL << 1 * 2)); /* set CP0, CP1 Full Access in Non-secure mode (enable PowerQuad) */
#endif                                     /* (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) */

    SCB->NSACR |= ((3UL << 0) | (3UL << 10)); /* enable CP0, CP1, CP10, CP11 Non-secure Access */

    if (SYSTEM_IS_XIP_FLEXSPI() && (CACHE64_POLSEL0->POLSEL == 0U)) /* Enable cache to accelerate boot. */
    {
        /* set command to invalidate all ways and write GO bit to initiate command */
        CACHE64_CTRL0->CCR = CACHE64_CTRL_CCR_INVW1_MASK | CACHE64_CTRL_CCR_INVW0_MASK;
        CACHE64_CTRL0->CCR |= CACHE64_CTRL_CCR_GO_MASK;
        /* Wait until the command completes */
        while ((CACHE64_CTRL0->CCR & CACHE64_CTRL_CCR_GO_MASK) != 0U)
        {
        }
        /* Enable cache, enable write buffer */
        CACHE64_CTRL0->CCR = (CACHE64_CTRL_CCR_ENWRBUF_MASK | CACHE64_CTRL_CCR_ENCACHE_MASK);

        /* Set whole FlexSPI0 space to write through. */
        CACHE64_POLSEL0->REG0_TOP = 0x07FFFC00U;
        CACHE64_POLSEL0->REG1_TOP = 0x0U;
        CACHE64_POLSEL0->POLSEL   = 0x1U;

        __ISB();
        __DSB();
    }

    SystemInitHook();
}

/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */

static uint32_t getT3PllMciIrcClkFreq(void)
{
    uint32_t freq = 0U;

    if ((SYSPLL_T3->CLKTREE_CTRL_SIX_REG & 0xFU) == 0x5U)
    {
        freq = CLOCK_MHZ(2560UL) / 43UL;
    }
    else if ((SYSPLL_T3->CLKTREE_CTRL_SIX_REG & 0xFU) == 0xAU)
    {
        freq = CLOCK_MHZ(2560UL) / 53UL;
    }
    else
    {
        /* Only 48MHz and 60MHz is allowed */
    }

    return freq;
}

static uint32_t getTcpuFvcoFreq(void)
{
    uint32_t freq = 0UL;
    uint32_t steps;

    steps = (SYSCTL2->PLL_CTRL & SYSCTL2_PLL_CTRL_TCPU_FBDIV_MASK) >> SYSCTL2_PLL_CTRL_TCPU_FBDIV_SHIFT;

    if ((CLK_XTAL_OSC_CLK == CLK_XTAL_OSC_CLK_40000KHZ) && (steps >= 75UL) && (steps <= 96UL))
    {
        /* Fbdiv from 75 to 96, step 40MHz */
        steps -= 75UL;
        freq = CLOCK_MHZ(3000UL) + steps * CLOCK_MHZ(40UL);
    }
    else if ((CLK_XTAL_OSC_CLK == CLK_XTAL_OSC_CLK_38400KHZ) && (steps >= 78UL) && (steps <= 100UL))
    {
        /* Fbdiv from 78 to 100, step 38.4MHz */
        steps -= 78UL;
        freq = CLOCK_KHZ(2995200UL) + steps * CLOCK_KHZ(38400UL);
    }
    else
    {
        /* Not valid path */
    }

    return freq;
}

static uint32_t getT3PllMci256mClkFreq(void)
{
    uint32_t freq = CLOCK_MHZ(256UL);
    return freq;
}

static uint32_t getTcpuMciClkFreq(void)
{
    uint32_t freq = getTcpuFvcoFreq() / 12UL;
    return freq;
}

static uint32_t getSysOscFreq(void)
{
    return (CLKCTL0->SYSOSCBYPASS == 0U) ? CLK_XTAL_OSC_CLK : ((CLKCTL0->SYSOSCBYPASS == 1U) ? CLK_EXT_CLKIN : 0U);
}

static uint32_t getFFroFreq(void)
{
    return getT3PllMciIrcClkFreq();
}

static uint32_t getLpOscFreq(void)
{
    return CLK_XTAL_OSC_CLK / 40U;
}

static uint32_t getSFroFreq(void)
{
    return getT3PllMci256mClkFreq() / 16U;
}

static uint32_t getMainPllClkFreq(void)
{
    return getTcpuMciClkFreq() / ((CLKCTL0->MAINPLLCLKDIV & CLKCTL0_MAINPLLCLKDIV_DIV_MASK) + 1U);
}

void SystemCoreClockUpdate(void)
{
    uint32_t freq = 0U;

    switch ((CLKCTL0->MAINCLKSELB) & CLKCTL0_MAINCLKSELB_SEL_MASK)
    {
        case CLKCTL0_MAINCLKSELB_SEL(0):
            switch ((CLKCTL0->MAINCLKSELA) & CLKCTL0_MAINCLKSELA_SEL_MASK)
            {
                case CLKCTL0_MAINCLKSELA_SEL(0):
                    freq = getSysOscFreq();
                    break;
                case CLKCTL0_MAINCLKSELA_SEL(1):
                    freq = getFFroFreq() / 4U;
                    break;
                case CLKCTL0_MAINCLKSELA_SEL(2):
                    freq = getLpOscFreq();
                    break;
                case CLKCTL0_MAINCLKSELA_SEL(3):
                    freq = getFFroFreq();
                    break;
                default:
                    freq = 0U;
                    break;
            }
            break;

        case CLKCTL0_MAINCLKSELB_SEL(1):
            freq = getSFroFreq();
            break;

        case CLKCTL0_MAINCLKSELB_SEL(2):
            freq = getMainPllClkFreq();
            break;

        case CLKCTL0_MAINCLKSELB_SEL(3):
            freq = CLK_RTC_32K_CLK;
            break;

        default:
            freq = 0U;
            break;
    }

    SystemCoreClock = freq / ((CLKCTL0->SYSCPUAHBCLKDIV & CLKCTL0_SYSCPUAHBCLKDIV_DIV_MASK) + 1U);
}

/* ----------------------------------------------------------------------------
   -- SystemInitHook()
   ---------------------------------------------------------------------------- */

__attribute__((weak)) void SystemInitHook(void)
{
    /* Void implementation of the weak function. */
}
