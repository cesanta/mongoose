/*--------------------------------------------------------------------------*/
/* Copyright 2021-2023 NXP                                                  */
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
 * @file  mcuxCsslMemory_Clear.h
 * @brief header file of memory clear function
 */


#ifndef MCUXCSSLMEMORY_CLEAR_H_
#define MCUXCSSLMEMORY_CLEAR_H_

#include <stdint.h>
#include <stddef.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxCsslFlowProtection_FunctionIdentifiers.h>
#include <mcuxCsslParamIntegrity.h>

/**
 * @defgroup mcuxCsslMemory_Clear mcuxCssl Memory Clear
 * @brief Control Flow Protected Memory Clear Function
 *
 * @ingroup mcuxCsslMemory
 * @{
 */

/**
 * @defgroup mcuxCsslMemory_Clear_Functions mcuxCsslMemory_Clear Function Definitions
 * @brief mcuxCsslMemory_Clear Function Definitions
 *
 * @ingroup mcuxCsslMemory_Clear
 * @{
 */

/**
 * @brief Clear @p length bytes of data at @p pDst
 *
 * The implementation is secure in the following aspects:
 * Parameter integrity protection: the function returns immediately in case of an incorrect parameter checksum.
 * Code flow protection: the function call is protected.
 * Buffer overflow protection: no data is written to @p pDst beyond @p dstLength bytes.
 *
 * @param[in]  chk       The parameter checksum, generated with #mcuxCsslParamIntegrity_Protect.
 * @param[in]  pDst      The destination pointer to buffer to be cleared. Must not be NULL.
 * @param[in]  dstLength The size of the destination data buffer in bytes.
 * @param[in]  length    The number of bytes to clear.
 *
 * @return A status code encapsulated in a flow-protection type.
 * @retval #MCUXCSSLMEMORY_STATUS_OK                If the contents in buffer at @p pDst is cleared.
 * @retval #MCUXCSSLMEMORY_STATUS_INVALID_PARAMETER If one of the parameters is invalid.
 * @retval #MCUXCSSLMEMORY_STATUS_FAULT             If a fault was detected, included invalid checksum @p chk.
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxCsslMemory_Clear)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxCsslMemory_Status_t) mcuxCsslMemory_Clear
(
    mcuxCsslParamIntegrity_Checksum_t chk,
    void * pDst,
    size_t dstLength,
    size_t length
);

/**
 * @}
 */

/**
 * @}
 */

#endif /* MCUXCSSLMEMORY_CLEAR_H_ */
