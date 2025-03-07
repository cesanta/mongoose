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
 * \file  mcuxClBuffer.c
 * \brief Provides the internal implementation for the plain C pointer buffer types.
 */


#include <mcuxClToolchain.h>
#include <mcuxClCore_Platform.h>

#include <mcuxClBuffer.h>
#include <mcuxCsslDataIntegrity.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h>
#include <internal/mcuxClBuffer_Internal.h>

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClBuffer_export)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClBuffer_Status_t) mcuxClBuffer_export(mcuxCl_Buffer_t bufDst, uint32_t offset, const uint8_t *pSrc, uint32_t byteLength)
{
  MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClBuffer_export);

  MCUX_CSSL_FP_FUNCTION_CALL(status, mcuxClBuffer_write(bufDst, offset, pSrc, byteLength));

  MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClBuffer_export, status, MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClBuffer_write));
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClBuffer_import)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClBuffer_Status_t) mcuxClBuffer_import(mcuxCl_InputBuffer_t bufSrc, uint32_t offset, uint8_t *pDst, uint32_t byteLength)
{
  MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClBuffer_import);

  MCUX_CSSL_FP_FUNCTION_CALL(status, mcuxClBuffer_read(bufSrc, offset, pDst, byteLength));

  MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClBuffer_import, status, MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClBuffer_read));
}
