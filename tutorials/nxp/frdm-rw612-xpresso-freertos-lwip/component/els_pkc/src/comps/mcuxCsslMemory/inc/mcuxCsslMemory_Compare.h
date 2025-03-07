/*--------------------------------------------------------------------------*/
/* Copyright 2020-2024 NXP                                                  */
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
 * @file  mcuxCsslMemory_Compare.h
 * @brief Include file for constant time memory compare function
 */

#ifndef MCUXCSSLMEMORY_COMPARE_H
#define MCUXCSSLMEMORY_COMPARE_H

#include <stdint.h>
#include <mcuxCsslParamIntegrity.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxCsslFlowProtection_FunctionIdentifiers.h>

/**
 * @defgroup mcuxCsslMemory_Compare mcuxCssl Memory Compare
 * @brief Control Flow Protected Memory Compare Function
 *
 * @ingroup mcuxCsslMemory
 * @{
 */

/**
 * @defgroup mcuxCsslMemory_Compare_Functions mcuxCsslMemory_Compare Function Definitions
 * @brief mcuxCsslMemory_Compare Function Definitions
 *
 * @ingroup mcuxCsslMemory_Compare
 * @{
 */

/**
 * @brief Compares the two memory regions @p lhs and @p rhs
 *
 * The implementation is secure in the following aspects:
 * 
 * * Constant execution time: The execution sequence of the code is always identical for equal @p length parameters,
 *     i.e. no branches are performed based on the data in @p pLhs or @p pRhs.
 * * Parameter integrity protection: An incorrect parameter checksum makes the function return immediately.
 * * Code flow protection: The function call is protected. Additionally, the result depends on all steps of the calculation.
 * 
 * @param chk    The parameter checksum, generated with #mcuxCsslParamIntegrity_Protect.
 * @param pLhs   The left-hand side data to compare. Must not be NULL.
 * @param pRhs   The right-hand side data to compare. Must not be NULL.
 * @param length The number of bytes to compare.
 * @return A status code encapsulated in a flow-protection type.
 * @retval #MCUXCSSLMEMORY_STATUS_EQUAL If the contents of @p lhs and @p rhs are equal or if @p length is zero.
 * @retval #MCUXCSSLMEMORY_STATUS_NOT_EQUAL If the contents of @p lhs and @p rhs are not equal.
 * @retval #MCUXCSSLMEMORY_STATUS_INVALID_PARAMETER If one of the parameters was invalid (i.e. @p lhs or @p rhs was NULL).
 * @retval #MCUXCSSLMEMORY_STATUS_FAULT If a fault was detected.
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxCsslMemory_Compare)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxCsslMemory_Status_t) mcuxCsslMemory_Compare
(
    mcuxCsslParamIntegrity_Checksum_t chk,
    void const * pLhs,
    void const * pRhs,
    size_t length
);

/**
 * @}
 */

/**
 * @}
 */

#endif
