/*--------------------------------------------------------------------------*/
/* Copyright 2023-2024 NXP                                                  */
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

/** @file  mcuxClMemory_Xor.h
 *  @brief Memory header for xor functions.
 * This header exposes functions that enable using memory xor function.
 */

/**
 * @defgroup mcuxClMemory_xor mcuxClMemory_xor
 * @brief This function performs xor between @p src1 and @p src2, and saves result to @p dst.
 * @ingroup mcuxClMemory
 * @{
 */

#ifndef MCUXCLMEMORY_XOR_H_
#define MCUXCLMEMORY_XOR_H_

#include <mcuxClConfig.h> // Exported features flags header

#include <mcuxClMemory_Types.h>
#include <mcuxCsslAnalysis.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************
 * FUNCTIONS
 **********************************************/

/**
 * Perform xor for 2 memory buffers.
 *
 * Operation in place is allowed - one of the input buffer can also be the output buffer.
 *
 *
 * @param[out] pDst        pointer to the destination buffer.
 * @param[in]  pSrc1       pointer to the first source buffer.
 * @param[in]  pSrc2       pointer to the second source buffer.
 * @param[in]  length      size (in bytes) to be operated
 * @param[in]  bufLength   buffer size (if bufLength < length, only bufLength bytes are operated).
 *
 */

MCUX_CSSL_FP_FUNCTION_DECL(mcuxClMemory_xor)
MCUX_CSSL_FP_PROTECTED_TYPE(void) mcuxClMemory_xor(uint8_t *pDst, const uint8_t *pSrc1, const uint8_t *pSrc2, uint32_t length, size_t bufLength);


/**********************************************
 * MACROS
 **********************************************/

/** Helper macro to call #mcuxClMemory_xor with flow protection. */
#define MCUXCLMEMORY_FP_MEMORY_XOR(pDst, pSrc1, pSrc2, length)  \
    MCUX_CSSL_FP_FUNCTION_CALL_VOID(mcuxClMemory_xor(pDst, pSrc1, pSrc2, length, length))

/** Helper macro to call #mcuxClMemory_xor with flow protection with buffer. */
#define MCUXCLMEMORY_FP_MEMORY_XOR_WITH_BUFF(pDst, pSrc1, pSrc2, length, bufLen)  \
    MCUX_CSSL_FP_FUNCTION_CALL_VOID(mcuxClMemory_xor(pDst, pSrc1, pSrc2, length, bufLen))


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLMEMORY_XOR_H_ */

/**
 * @}
 */
