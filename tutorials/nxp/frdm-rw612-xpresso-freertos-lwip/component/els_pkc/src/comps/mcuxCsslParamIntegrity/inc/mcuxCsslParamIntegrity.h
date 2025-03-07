/*--------------------------------------------------------------------------*/
/* Copyright 2020-2021 NXP                                                  */
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

/// @file  mcuxCsslParamIntegrity.h
/// @brief Top-level include file for the parameter integrity protection mechanism
///
/// The library exposes the following functions:
/// <ol>
///     <li> Generation of parameter checksums: #mcuxCsslParamIntegrity_Protect
///     <li> Validation of parameter checksums: #mcuxCsslParamIntegrity_Validate
/// </ol>

#ifndef MCUXCSSLPARAMINTEGRITY_H
#define MCUXCSSLPARAMINTEGRITY_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxCsslFlowProtection_FunctionIdentifiers.h>

/**
 * @defgroup mcuxCsslParamIntegrity Parameter Integrity API
 * @brief Functionality to ensure parameter integrity during function calls
 *
 * @ingroup mcuxCsslAPI
 * @{
 */

/**
 * @defgroup mcuxCsslParamIntegrity_Macros mcuxCsslParamIntegrity Macro Definitions
 * @brief Macros of mcuxCsslParamIntegrity component
 * @ingroup mcuxCsslParamIntegrity
 * @{
 */

#define MCUXCSSLPARAMINTEGRITY_BASE_CHECKSUM ((mcuxCsslParamIntegrity_Checksum_t)0xb7151628u) ///< First eight hex digits of Eulers number

#define MCUXCSSLPARAMINTEGRITY_CHECK_VALID ((mcuxCsslParamIntegrity_Checksum_t)0x6969u) ///< Return value of #mcuxCsslParamIntegrity_Validate if the parameter checksum was correct

#define MCUXCSSLPARAMINTEGRITY_CHECK_INVALID ((mcuxCsslParamIntegrity_Checksum_t)0x9696u) ///< Return value of #mcuxCsslParamIntegrity_Validate if the parameter checksum was incorrect

/**
 * @}
 */

/**
 * @defgroup mcuxCsslParamIntegrity_Types mcuxCsslParamIntegrity Type Definitions
 * @brief Types of mcuxCsslParamIntegrity component
 * @ingroup mcuxCsslParamIntegrity
 * @{
 */

#ifdef MCUXCL_FEATURE_CSSL_PI_64BIT
/**
* @brief Build time assertion to ensure CPU word size of 64 bit
*/
typedef void * mcuxCsslParamIntegrity_AssertionCpuWordSize_t[(8u == sizeof(size_t)) ? (+1) : (-1)];
#else
/**
* @brief Build time assertion to ensure CPU word size of 32 bit
*/
typedef void * mcuxCsslParamIntegrity_AssertionCpuWordSize_t[(4u == sizeof(size_t)) ? (+1) : (-1)];
#endif

/**
* @brief  Type of a parameter checksum.
*/
typedef uint32_t mcuxCsslParamIntegrity_Checksum_t;

/**
 * @}
 */

/**
 * @defgroup mcuxCsslParamIntegrity_Functions mcuxCsslParamIntegrity Function Definitions
 * @brief Functions of mcuxCsslParamIntegrity component
 * @ingroup mcuxCsslParamIntegrity
 * @{
 */

/**
 * @brief Calculates a parameter checksum.
 * 
 * @param nargs The number of parameters to be protected.
 * @param ... The parameters that should be protected. Note that parameters bigger than a single machine word are not supported.
 * @return checksum over the input parameters to be protected
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxCsslParamIntegrity_Protect)
mcuxCsslParamIntegrity_Checksum_t mcuxCsslParamIntegrity_Protect(size_t nargs, ...);

/**
 * @brief Verifies the correctness of a parameter checksum.
 * 
 * @param chk The parameter checksum.
 * @param nargs The number of parameters to be protected.
 * @param ... The parameters that were used to calculate the parameter checksum. Note that parameters bigger than a single machine word are not supported.
 * @return A status code encapsulated in a flow-protection type.
 * @retval #MCUXCSSLPARAMINTEGRITY_CHECK_VALID The parameter checksum was correct.
 * @retval #MCUXCSSLPARAMINTEGRITY_CHECK_INVALID The parameter checksum was incorrect.
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxCsslParamIntegrity_Validate)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxCsslParamIntegrity_Checksum_t) mcuxCsslParamIntegrity_Validate(mcuxCsslParamIntegrity_Checksum_t chk, size_t nargs, ...);

/**
 * @}
 */

/**
 * @}
 */

#endif
