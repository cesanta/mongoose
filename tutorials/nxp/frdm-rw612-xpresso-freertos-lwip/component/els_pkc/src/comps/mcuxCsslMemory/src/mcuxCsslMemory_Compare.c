/*--------------------------------------------------------------------------*/
/* Copyright 2020-2023 NXP                                                  */
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

#include <mcuxCsslMemory.h>
#include <mcuxCsslSecureCounter.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxCsslFlowProtection_FunctionIdentifiers.h>
#include <mcuxCsslAnalysis.h>
#include <internal/mcuxCsslMemory_Internal_Compare_asm.h>

#define CLS_NORMAL      0x2E00u
#define CLS_MISMATCH    0x8900u
#define LSB_NOT_EQUAL   0xB8u

MCUX_CSSL_FP_FUNCTION_DEF(mcuxCsslMemory_Compare)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxCsslMemory_Status_t) mcuxCsslMemory_Compare
(
    mcuxCsslParamIntegrity_Checksum_t chk,
    void const * pLhs,
    void const * pRhs,
    size_t length
)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxCsslMemory_Compare,
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslParamIntegrity_Validate)
    );

    MCUX_CSSL_FP_FUNCTION_CALL(result, mcuxCsslParamIntegrity_Validate(chk, 3u, pLhs, pRhs, length));

    if( (result != MCUXCSSLPARAMINTEGRITY_CHECK_VALID)) {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxCsslMemory_Compare, MCUXCSSLMEMORY_STATUS_FAULT);
    }

   if((NULL == pLhs) || (NULL == pRhs)) {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxCsslMemory_Compare, MCUXCSSLMEMORY_STATUS_INVALID_PARAMETER);
    }

    uint32_t nwords = 0u;
    uint32_t retval = 0u;
    MCUX_CSSL_ANALYSIS_START_PATTERN_CAST_VOID()
    uint8_t const * cur_lhs = (uint8_t const *)pLhs;
    uint8_t const * cur_rhs = (uint8_t const *)pRhs;
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_CAST_VOID()
    uint32_t const notValid = ~(MCUXCSSLPARAMINTEGRITY_CHECK_VALID);
    uint32_t const errCode = (uint32_t)MCUXCSSLMEMORY_STATUS_NOT_EQUAL;

    /* Pre-calculate end pointers */
    uint8_t const * end_lhs = &cur_lhs[length];
    uint8_t const * end_rhs = &cur_rhs[length];

    MCUX_CSSL_ANALYSIS_START_SUPPRESS_REINTERPRET_MEMORY_BETWEEN_INAPT_ESSENTIAL_TYPES("Exception 9: re-interpreting the memory for word access")
    MCUXCSSLMEMORY_COMPARE_ASM_COMPARISON(retval, cur_lhs, cur_rhs, nwords, length, notValid, result);
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_REINTERPRET_MEMORY_BETWEEN_INAPT_ESSENTIAL_TYPES()

    MCUXCSSLMEMORY_COMPARE_ASM_CALC_RETVAL(retval, errCode);
    retval &= 0x000000FFu;  /* Isolate the byte value that we are interested in */
    retval |= MCUXCSSLMEMORY_COMPONENT_MASK | CLS_MISMATCH;  /* Defines the value to what is expected */
    retval ^= (CLS_MISMATCH ^ CLS_NORMAL) & ((retval ^ LSB_NOT_EQUAL) << 8);  /* If equal, then change the class to match the equal case */

    /* Check that the pointers reached the end */
    if((end_lhs != cur_lhs) || (end_rhs != cur_rhs)) {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxCsslMemory_Compare, MCUXCSSLMEMORY_STATUS_FAULT);
    }

    MCUX_CSSL_SC_ADD(nwords); // -> should be 0

    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxCsslMemory_Compare, retval);
}
