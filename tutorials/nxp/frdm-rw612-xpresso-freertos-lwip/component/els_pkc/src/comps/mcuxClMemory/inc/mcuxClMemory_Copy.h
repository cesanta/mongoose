/*--------------------------------------------------------------------------*/
/* Copyright 2020-2021, 2023 NXP                                            */
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

/** @file  mcuxClMemory_Copy.h
 *  @brief Memory header for copy functions.
 * This header exposes functions that enable using memory copy function.
 */

/**
 * @defgroup mcuxClMemory_Copy mcuxClMemory_Copy
 * @brief This function copies a memory region from @p src to @p dst.
 * @ingroup mcuxClMemory
 * @{
 */

#ifndef MCUXCLMEMORY_COPY_H_
#define MCUXCLMEMORY_COPY_H_

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
 * Copies a memory buffer to another location.
 *
 * The two buffers must not overlap.
 * 
 * If the destination buffer is too small, i.e. if bufLength < length, 
 * (length-bufLength) is added to the Flow Protection token (see @ref mcuxCsslFlowProtection).
 *  
 * @param[out] pDst        pointer to the buffer to be copied to.
 * @param[in]  pSrc        pointer to the buffer to copy.
 * @param[in]  length      size (in bytes) to be copied.
 * @param[in]  bufLength   buffer size (if bufLength < length, only bufLength bytes are copied).
 *
 */

MCUX_CSSL_FP_FUNCTION_DECL(mcuxClMemory_copy)
MCUX_CSSL_FP_PROTECTED_TYPE(void) mcuxClMemory_copy (uint8_t *pDst, uint8_t const *pSrc, size_t length, size_t bufLength);


/**********************************************
 * MACROS
 **********************************************/

/** Helper macro to call #mcuxClMemory_copy with flow protection. */
#define MCUXCLMEMORY_FP_MEMORY_COPY(pTarget, pSource, byteLen)  \
    MCUX_CSSL_FP_FUNCTION_CALL_VOID(mcuxClMemory_copy((uint8_t *) (pTarget), (const uint8_t *) (pSource), byteLen, byteLen))

/** Helper macro to call #mcuxClMemory_copy with flow protection with buffer. */
#define MCUXCLMEMORY_FP_MEMORY_COPY_WITH_BUFF(pTarget, pSource, byteLen, buffLen)  \
    MCUX_CSSL_FP_FUNCTION_CALL_VOID(mcuxClMemory_copy((uint8_t *) (pTarget), (const uint8_t *) (pSource), byteLen, buffLen))


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLMEMORY_COPY_H_ */

/**
 * @}
 */
