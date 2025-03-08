/*--------------------------------------------------------------------------*/
/* Copyright 2021-2024 NXP                                                  */
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
#include <mcuxClMemory_Copy.h>

MCUX_CSSL_FP_FUNCTION_DEF(mcuxCsslMemory_Copy)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxCsslMemory_Status_t) mcuxCsslMemory_Copy
(
    mcuxCsslParamIntegrity_Checksum_t chk,
    void const * pSrc,
    void * pDst,
    size_t dstLength,
    size_t length
)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxCsslMemory_Copy);

    if((NULL == pSrc) || (NULL == pDst) || (length > dstLength))
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxCsslMemory_Copy, MCUXCSSLMEMORY_STATUS_INVALID_PARAMETER);
    }

    mcuxCsslParamIntegrity_Checksum_t cmpChk = mcuxCsslParamIntegrity_Protect(3u, pSrc, pDst, length);

    MCUX_CSSL_FP_FUNCTION_CALL(integrityResult, mcuxCsslParamIntegrity_Validate(chk, 4u, pSrc, pDst, dstLength, length));

    if(MCUXCSSLPARAMINTEGRITY_CHECK_VALID != integrityResult)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxCsslMemory_Copy, MCUXCSSLMEMORY_STATUS_FAULT);
    }

    MCUX_CSSL_FP_FUNCTION_CALL_VOID(mcuxClMemory_copy(pDst, pSrc, length, dstLength));

    MCUX_CSSL_FP_FUNCTION_CALL(compareResult, mcuxCsslMemory_Compare(cmpChk, pSrc, pDst, length));

    if((MCUXCSSLMEMORY_STATUS_EQUAL == compareResult))
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxCsslMemory_Copy, MCUXCSSLMEMORY_STATUS_OK,
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_copy),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Compare),
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslParamIntegrity_Validate));
    }
    else
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxCsslMemory_Copy, MCUXCSSLMEMORY_STATUS_FAULT);
    }
}
