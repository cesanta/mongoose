/*--------------------------------------------------------------------------*/
/* Copyright 2020-2021, 2023 NXP                                            */
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

/** @file  mcuxClMemory_Types.h
 *  @brief Memory type header.
 * This header exposes types used by the @ref mcuxClMemory functions. */

/**
 * @defgroup mcuxClMemory_Types mcuxClMemory_Types
 * @brief Defines all types used by the @ref mcuxClMemory functions.
 * @ingroup mcuxClMemory
 * @{
 */

#ifndef MCUXCLMEMORY_TYPES_H
#define MCUXCLMEMORY_TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <mcuxClConfig.h> // Exported features flags header
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h>

/**********************************************
 * MACROS
 **********************************************/
/**
 * @defgroup mcuxClMemory_Types_Macros mcuxClMemory_Types_Macros
 * @brief Defines all macros of @ref mcuxClMemory_Types
 * @ingroup mcuxClMemory_Types
 * @{
 */
#define MCUXCLMEMORY_API extern  ///< Marks a function as a public API function of the mcuxClMemory component

/**
 * @}
 */

/**********************************************
 * TYPEDEFS
 **********************************************/
/**
 * @brief Type for error codes of mcuxClMemory component functions.
 * 
 * Type returned by mcuxClMemory functions. See @ref MCUXCLMEMORY_STATUS_ for possible options.
 */
typedef uint32_t mcuxClMemory_Status_t;

/**
 * @brief Deprecated type for error codes used by code-flow protected mcuxClMemory component functions.
 */
typedef MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClMemory_Status_t) mcuxClMemory_Status_Protected_t;

#endif /* #MCUXCLMEMORY_TYPES_H */

/**
 * @}
 */
