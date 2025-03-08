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

/**
 * @file  mcuxCsslMemory_Constants.h
 * @brief Constants definitions for the mcuxCsslMemory component
 */

#ifndef MCUXCSSLMEMORY_CONSTANTS_H
#define MCUXCSSLMEMORY_CONSTANTS_H

#include <stdint.h>
#include <mcuxCsslMemory_Types.h>

/**********************************************
 * CONSTANTS
 **********************************************/

/**
 * @brief CSSL Memory Component mask value.
 */
#define MCUXCSSLMEMORY_COMPONENT_MASK              0x04240000u ///< Component mask value

/**
 * @defgroup MCUXCSSLMEMORY_STATUS_ MCUXCSSLMEMORY_STATUS_
 * @brief Defines valid mcuxCsslMemory function return codes
 * @ingroup mcuxCsslMemory_Constants
 * @{
 */
#define MCUXCSSLMEMORY_STATUS_OK                 ((mcuxCsslMemory_Status_t) 0x04242E03u) ///< The operation was successful
#define MCUXCSSLMEMORY_STATUS_EQUAL              ((mcuxCsslMemory_Status_t) 0x04242E47u) ///< The two contents of the Memory Compare are equal
#define MCUXCSSLMEMORY_STATUS_NOT_EQUAL          ((mcuxCsslMemory_Status_t) 0x042489B8u) ///< The two contents of the Memory Compare are not equal
#define MCUXCSSLMEMORY_STATUS_INVALID_PARAMETER  ((mcuxCsslMemory_Status_t) 0x0424533Cu) ///< A parameter was invalid
#define MCUXCSSLMEMORY_STATUS_FAULT              ((mcuxCsslMemory_Status_t) 0x0424F0F0u) ///< A fault occurred in the execution

#define MCUXCSSLMEMORY_KEEP_ORDER     ((uint32_t) 0x042439A5u)  ///< Data storing in destination buffer in original order.
#define MCUXCSSLMEMORY_REVERSE_ORDER  ((uint32_t) 0x0424395Au)  ///< Data storing in destination buffer with reversed order.
/**
 * @}
 */

#endif /* MCUXCSSLMEMORY_CONSTANTS_H */
