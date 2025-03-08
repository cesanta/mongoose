/*--------------------------------------------------------------------------*/
/* Copyright 2021, 2023 NXP                                                 */
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
 * @file  mcuxCsslMemory_Set.h
 * @brief header file of memory set function
 */


#ifndef MCUXCSSLMEMORY_SET_H_
#define MCUXCSSLMEMORY_SET_H_

#include <stdint.h>
#include <stddef.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxCsslFlowProtection_FunctionIdentifiers.h>
#include <mcuxCsslParamIntegrity.h>

/**
 * @defgroup mcuxCsslMemory_Set mcuxCssl Memory Set
 * @brief Control Flow Protected Memory Set Function
 *
 * @ingroup mcuxCsslMemory
 * @{
 */

/**
 * @defgroup mcuxCsslMemory_Set_Functions mcuxCsslMemory_Set Function Definitions
 * @brief mcuxCsslMemory_Set Function Definitions
 *
 * @ingroup mcuxCsslMemory_Set
 * @{
 */

/**
 * @brief Set @p length bytes of data at @p pDst
 *
 * The implementation is secure in the following aspects:
 * Parameter integrity protection: the function returns immediately in case of an incorrect parameter checksum.
 * Code flow protection: the function call is protected.
 * Buffer overflow protection: no data is written to @p pDst beyond @p bufLength bytes.
 *
 * @param[in]  chk          The parameter checksum, generated with #mcuxCsslParamIntegrity_Protect.
 * @param[in]  pDst         The destination pointer to buffer to be set. Must not be NULL.
 * @param[in]  val          The byte value to be set. 
 * @param[in]  length       The size in bytes to set.
 * @param[in]  bufLength    The buffer size (if bufLength < length, only bufLength bytes are set).
 *
 * @return A status code encapsulated in a flow-protection type.
 * @retval #MCUXCSSLMEMORY_STATUS_OK                 If @p val set @p length times at @p pDst.
 * @retval #MCUXCSSLMEMORY_STATUS_INVALID_PARAMETER  If one of the parameters is invalid.
 * @retval #MCUXCSSLMEMORY_STATUS_FAULT              If a fault was detected, included invalid checksum @p chk.
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxCsslMemory_Set)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxCsslMemory_Status_t) mcuxCsslMemory_Set
(
    mcuxCsslParamIntegrity_Checksum_t chk,
    void * pDst,
    uint8_t val,
    size_t length,
    size_t bufLength
);

/**
 * @}
 */

/**
 * @}
 */

#endif /* MCUXCSSLMEMORY_SET_H_ */
