/*--------------------------------------------------------------------------*/
/* Copyright 2020-2023 NXP                                                  */
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
 * @file  mcuxCsslMemory_Internal_SecureCompare_Stub.c
 * @brief C file that contains the stub implementation of the secure compare in C
 */

#include <stdint.h>
#include <stddef.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxCsslFlowProtection_FunctionIdentifiers.h>
#include <mcuxCsslMemory_Constants.h>
#include <mcuxCsslMemory_Types.h>
#ifdef CSSL_MEMORY_ENABLE_COMPARE
#include <mcuxCsslMemory_Compare.h>
#include <mcuxCsslParamIntegrity.h>
#endif /* CSSL_MEMORY_ENABLE_COMPARE */
#include <internal/mcuxCsslMemory_Internal_SecureCompare.h>


/**
 * @brief Stub comparison of the two memory regions @p lhs and @p rhs - internal use only
 *
 * The implementation calls mcuxCsslMemory_Compare when possible (function defined and pointers aligned)
 * else a code that contains no security countermeasure inside is called.
 *
 */
MCUX_CSSL_FP_FUNCTION_DEF(mcuxCsslMemory_Int_SecComp)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxCsslMemory_Status_t) mcuxCsslMemory_Int_SecComp
(
    const uint8_t * pLhs,
    const uint8_t * pRhs,
    uint32_t length
)
{
#ifdef CSSL_MEMORY_ENABLE_COMPARE
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxCsslMemory_Int_SecComp,
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Compare)
    );

    const uint32_t cpuWordSize = sizeof(uint32_t);
    if ((0u == ((uint32_t) pLhs & (cpuWordSize - 1u))) && (0u == ((uint32_t) pRhs & (cpuWordSize - 1u)))) {
        MCUX_CSSL_FP_FUNCTION_CALL(retval, mcuxCsslMemory_Compare(mcuxCsslParamIntegrity_Protect(3u, pLhs, pRhs, length), pLhs, pRhs, length));
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxCsslMemory_Int_SecComp, retval);
    }
#endif  /* CSSL_MEMORY_ENABLE_COMPARE */
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxCsslMemory_Int_SecComp);

    mcuxCsslMemory_Status_t retval = MCUXCSSLMEMORY_STATUS_EQUAL;
    for (uint32_t i = 0u; i < length; ++i) {
        if (pLhs[i] != pRhs[i]) {
            retval = MCUXCSSLMEMORY_STATUS_NOT_EQUAL;
        }
    }
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxCsslMemory_Int_SecComp, retval);
}
