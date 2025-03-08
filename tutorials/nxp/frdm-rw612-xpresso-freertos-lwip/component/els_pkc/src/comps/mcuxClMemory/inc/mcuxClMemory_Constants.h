/*--------------------------------------------------------------------------*/
/* Copyright 2023 NXP                                                       */
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

/** @file  mcuxClMemory_Constants.h
 *  @brief Memory constant header.
 * This header exposes constants used by the @ref mcuxClMemory functions. */

/**
 * @defgroup mcuxClMemory_Constants mcuxClMemory_Constants
 * @brief Defines all constants used by the @ref mcuxClMemory functions.
 * @ingroup mcuxClMemory
 * @{
 */

#ifndef MCUXCLMEMORY_CONSTANTS_H
#define MCUXCLMEMORY_CONSTANTS_H

/**********************************************
 * CONSTANTS
 **********************************************/

#define MCUXCLMEMORY_COMPONENT_MASK              0x09990000u ///< Component mask value

/**
 * @defgroup MCUXCLMEMORY_STATUS_ MCUXCLMEMORY_STATUS_
 * @brief Defines valid mcuxClMemory function return codes
 * @ingroup mcuxClMemory_Types_Macros
 * @{
 */
#define MCUXCLMEMORY_STATUS_OK                   ((mcuxClMemory_Status_t) 0x09992E03u) ///< Memory operation successful
#define MCUXCLMEMORY_STATUS_EQUAL                ((mcuxClMemory_Status_t) 0x09992E47u) ///< The two contents of the Memory Compare are equal
#define MCUXCLMEMORY_STATUS_NOT_EQUAL            ((mcuxClMemory_Status_t) 0x099989B8u) ///< The two contents of the Memory Compare are not equal
#define MCUXCLMEMORY_STATUS_INVALID_PARAMETER    ((mcuxClMemory_Status_t) 0x0999533Cu) ///< A parameter was invalid
#define MCUXCLMEMORY_STATUS_FAULT                ((mcuxClMemory_Status_t) 0x0999F0F0u) ///< A fault occurred in the execution

/**
 * @}
 */

#define MCUXCLMEMORY_ERRORCODE_OK MCUXCLMEMORY_STATUS_OK ///< Memory operation successful
                                                       ///< @deprecated Please use #MCUXCLMEMORY_STATUS_OK instead

#endif  /* MCUXCLMEMORY_CONSTANTS_H */

/**
 * @}
 */
