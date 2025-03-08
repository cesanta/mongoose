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

/**
 * @file  mcuxCsslMemory_Clear.c
 * @brief mcuxCsslMemory: implementation of memory clear function
 */


#include <mcuxCsslFlowProtection.h>
#include <mcuxCsslFlowProtection_FunctionIdentifiers.h>
#include <mcuxCsslSecureCounter.h>
#include <mcuxCsslParamIntegrity.h>
#include <mcuxCsslMemory.h>
#include <mcuxCsslMemory_Clear.h>

MCUX_CSSL_FP_FUNCTION_DEF(mcuxCsslMemory_Clear)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxCsslMemory_Status_t) mcuxCsslMemory_Clear
(
    mcuxCsslParamIntegrity_Checksum_t chk,
    void * pDst,
    size_t dstLength,
    size_t length
)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxCsslMemory_Clear,
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslParamIntegrity_Validate) );

    MCUX_CSSL_FP_FUNCTION_CALL(retCode_paramIntegrityValidate, mcuxCsslParamIntegrity_Validate(chk, 3u, pDst, dstLength, length));
    if ((retCode_paramIntegrityValidate != MCUXCSSLPARAMINTEGRITY_CHECK_VALID))
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxCsslMemory_Clear, MCUXCSSLMEMORY_STATUS_FAULT);
    }

    if (length > dstLength)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxCsslMemory_Clear, MCUXCSSLMEMORY_STATUS_INVALID_PARAMETER);
    }

    MCUX_CSSL_FP_FUNCTION_CALL(retCode_memSet, mcuxCsslMemory_Set(mcuxCsslParamIntegrity_Protect(4u, pDst, 0u, length, dstLength), pDst, 0u, length, dstLength) );

    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxCsslMemory_Clear, retCode_memSet,
      MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Set ));
}
