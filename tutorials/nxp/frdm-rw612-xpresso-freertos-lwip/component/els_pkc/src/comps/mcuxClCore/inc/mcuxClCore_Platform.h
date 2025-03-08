/*--------------------------------------------------------------------------*/
/* Copyright 2021, 2024 NXP                                                 */
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

#ifndef MCUXCLCORE_PLATFORM_H_
#define MCUXCLCORE_PLATFORM_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <mcuxCsslAnalysis.h>

#include <mcuxClConfig.h> // Exported features flags header

#ifdef MCUXCL_FEATURE_HW_DMA_WORKAROUND

/** Convert the register address to the one used in secure mode. */
#define MCUXCL_MSDK_REG_SECURE_ADDR(x) \
  MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER("Convert pointer to address for bitwise operations needed in bit masking.") \
  ((uint32_t)(x) | (0x1UL << 28)) \
  MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER()

/** Convert the register address to the one used in non-secure mode. */
#define MCUXCL_MSDK_REG_NONSECURE_ADDR(x) \
  MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER("Convert pointer to address for bitwise operations needed in bit masking.") \
  ((uint32_t)(x) & ~(0x1UL << 28)) \
  MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER()

/** Address mapping to allow HW IP access 0x2xxx_xxxx memory range */
#define MCUXCL_HW_DMA_WORKAROUND(addr) \
  MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER("Convert pointer to address for bitwise operations needed in mapping calculations.") \
  (((uint32_t)(MCUXCL_MSDK_REG_NONSECURE_ADDR(addr)) < 0x00780000u) ? ((uint32_t)(addr) | 0x20000000u) : (uint32_t)(addr)) \
  MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER()

#else

#define MCUXCL_HW_DMA_WORKAROUND(addr) \
  MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER("Convert pointer to address for bitwise operations needed in mapping calculations. This instance needed for consistent macro behaviour.") \
  ((uint32_t)(addr)) \
  MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER()

#endif /* MCUXCL_FEATURE_DMA_WORKAROUND */

#endif /* MCUXCLCORE_PLATFORM_H_ */
