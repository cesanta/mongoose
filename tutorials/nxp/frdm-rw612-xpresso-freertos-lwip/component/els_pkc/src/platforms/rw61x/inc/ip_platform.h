/*--------------------------------------------------------------------------*/
/* Copyright 2022-2023 NXP                                                  */
/*                                                                          */
/* NXP Proprietary. This software is owned or controlled by NXP and may     */
/* only be used strictly in accordance with the applicable license terms.   */
/* By expressly accepting such terms or by downloading, installing,         */
/* activating and/or otherwise using the software, you are agreeing that    */
/* you have read, and that you agree to comply with and are bound by, such  */
/* license terms.  If you do not agree to be bound by the applicable        */
/* license terms, then you may not retain, install, activate or otherwise   */
/* use the software.                                                        */
/*--------------------------------------------------------------------------*/

/** @file  ip_platform.h
 *  @brief Include file for the IP.
 *
 * This file defines base addresses and types for all IP blocks used by CLNS. */

#ifndef IP_PLATFORM_H
#define IP_PLATFORM_H

#include "fsl_device_registers.h"

/* ================================================================================ */
/* ================             Peripheral declaration             ================ */
/* ================================================================================ */

// Define base address of PUF
#define PUF_SFR_BASE            PUF      ///< base of PUF SFRs
#define PUF_SFR_NAME(sfr)       sfr      ///< full name of SFR
#define PUF_SFR_PREFIX          PUF_     ///< sfr field name prefix
#define PUF_SFR_SUFFIX_MSK     _MASK       ///< sfr field name suffix for mask
#define PUF_SFR_SUFFIX_POS     _SHIFT      ///< sfr field name suffix for bit position


// Define base address of CSS
#define ELS_SFR_BASE            ELS         ///< base of CSS SFRs
#define ELS_SFR_NAME(sfr)       sfr         ///< full name of SFR
#define ELS_SFR_PREFIX          ELS_        ///< sfr field name prefix

// Define base address of PKC
#define PKC_SFR_BASE            PKC         ///< base of PKC SFRs
#define PKC_SFR_NAME(sfr)       PKC_ ## sfr ///< full name of SFR
#define PKC_SFR_PREFIX          PKC_PKC_    ///< sfr field name prefix
#define PKC_SFR_SUFFIX_MSK      _MASK       ///< sfr field name suffix for mask
#define PKC_SFR_SUFFIX_POS      _SHIFT      ///< sfr field name suffix for bit position

// PKC_RAM base address is not defined in any header file
#define PKC_RAM_ADDR ((uint32_t) 0x5015A000)
#define PKC_RAM_SIZE  ((uint32_t)0x2000u)
#define PKC_WORD_SIZE  8u

// Define base address of TRNG
#define TRNG_SFR_BASE           TRNG         ///< base of TRNG SFRs
#define TRNG_SFR_NAME(sfr)      sfr          ///< full name of SFR
#define TRNG_SFR_PREFIX         TRNG_        ///< sfr field name prefix
#define TRNG_SFR_SUFFIX_MSK     _MASK        ///< sfr field name suffix for mask
#define TRNG_SFR_SUFFIX_POS     _SHIFT       ///< sfr field name suffix for bit position

// ELS version
#define ELS_HW_VERSION_REVISION            1
#define ELS_HW_VERSION_MINOR               16
#define ELS_HW_VERSION_MAJOR               2
#define ELS_HW_VERSION_FW_REVISION         1
#define ELS_HW_VERSION_FW_MINOR            2
#define ELS_HW_VERSION_FW_MAJOR            1

// Interrupt definitions
#define CSS_INTERRUPT_BUSY_NUMBER          ELS_IRQn
#define CSS_INTERRUPT_ERR_NUMBER           ELS_IRQn
#define CSS_INTERRUPT_IRQ_NUMBER           ELS_IRQn
#define GDET_INTERRUPT_IRQ_NUMBER          ELS_GDET_IRQ_IRQn
#define GDET_INTERRUPT_ERR_NUMBER          ELS_GDET_ERR_IRQn


/* If we are supposed to determine the CSSv2 base address at link time, do not use the definitions from ip_css.h
 * Redefine CSS as an extern pointer.
 */
#undef CSS_BASE
extern void * ip_css_base;
#define CSS_BASE                     ip_css_base

#endif
