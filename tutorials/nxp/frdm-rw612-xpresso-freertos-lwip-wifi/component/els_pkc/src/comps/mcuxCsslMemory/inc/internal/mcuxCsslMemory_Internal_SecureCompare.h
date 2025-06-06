/*--------------------------------------------------------------------------*/
/* Copyright 2023-2024 NXP                                                  */
/*                                                                          */
/* NXP Confidential. This software is owned or controlled by NXP and may    */
/* only be used strictly in accordance with the applicable license terms.   */
/* By expressly accepting such terms or by downloading, installing,         */
/* activating and/or otherwise using the software, you are agreeing that    */
/* you have read, and that you agree to comply with and are bound by, such  */
/* license terms. If you do not agree to be bound by the applicable license */
/* terms, then you may not retain, install, activate or otherwise use the   */
/* software.                                                                */
/*--------------------------------------------------------------------------*/

/**
 * @file  mcuxCsslMemory_Internal_SecureCompare.h
 * @brief Internal header of mcuxCsslMemory_SecureCompare
 */


#ifndef MCUXCSSLMEMORY_INTERNAL_SECURECOMPARE_H_
#define MCUXCSSLMEMORY_INTERNAL_SECURECOMPARE_H_

#include <stdint.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxCsslFlowProtection_FunctionIdentifiers.h>

/**
 * @brief Securely compares the two memory regions @p lhs and @p rhs - internal use only
 *
 * The implementation is secure in the following aspects:
 *
 * * Constant execution time: The execution sequence of the code is always identical for equal @p length parameters,
 *     i.e. no branches are performed based on the data in @p pLhs or @p pRhs.
 * * Code flow protection: The function call is protected. Additionally, the result depends on all steps of the calculation.
 * * Random order memory access: an attacker shall not be able to distinguish the position of the difference between the two compared buffers.
 * * Blinded word compare: SPA protection.
 * * Integrity of the result is ensured. The accumulator of differences is checked twice when generating the return status (EQUAL or NOT_EQUAL).
 * * Data Integrity: Expunge(pLhs + pRhs + length)
 *
 * @param pLhs   The left-hand side data to compare. Must not be NULL.
 * @param pRhs   The right-hand side data to compare. Must not be NULL.
 * @param length The number of bytes to compare.
 * @return A status code encapsulated in a flow-protection type.
 * @retval #MCUXCSSLMEMORY_STATUS_EQUAL If the contents of @p lhs and @p rhs are equal.
 * @retval #MCUXCSSLMEMORY_STATUS_NOT_EQUAL If the contents of @p lhs and @p rhs are not equal.
 * @retval #MCUXCSSLMEMORY_STATUS_FAULT If a fault was detected.
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxCsslMemory_Int_SecComp)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxCsslMemory_Status_t) mcuxCsslMemory_Int_SecComp(
    const uint8_t * pLhs,
    const uint8_t * pRhs,
    uint32_t length
);

#endif  /* MCUXCSSLMEMORY_INTERNAL_SECURECOMPARE_H */
