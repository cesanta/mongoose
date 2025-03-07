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

/** @file  mcuxClMemory_Copy_Reversed.h
 *  @brief Memory header for reversed copy functions.
 * This header exposes functions that enable using memory reversed copy function.
 */

/**
 * @defgroup mcuxClMemory_Copy_Reversed mcuxClMemory_Copy_Reversed
 * @brief This function copies a memory region from @p src to @p dst reversely.
 * @ingroup mcuxClMemory
 * @{
 */

#ifndef MCUXCLMEMORY_COPY_REVERSED_H_
#define MCUXCLMEMORY_COPY_REVERSED_H_

#include <mcuxClCore_Platform.h>
#include <mcuxClMemory_Types.h>
#include <mcuxCsslAnalysis.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************
 * FUNCTIONS
 **********************************************/

/**
 * Copies a memory buffer to another location reversely.
 *
 * If the destination buffer is too small, i.e. if bufLength < length,
 * then only bufLength bytes are copied reversely.
 *
 * @param[out] pDst        pointer to the buffer to be copied to.
 * @param[in]  pSrc        pointer to the buffer to copy.
 * @param[in]  length      size (in bytes) to be copied.
 * @param[in]  bufLength   buffer size (if bufLength < length, only bufLength bytes are copied reversely).
 *
 */

MCUX_CSSL_FP_FUNCTION_DECL(mcuxClMemory_copy_reversed)
MCUX_CSSL_FP_PROTECTED_TYPE(void) mcuxClMemory_copy_reversed (uint8_t *pDst, uint8_t const *pSrc, size_t length, size_t bufLength);


/**********************************************
 * MACROS
 **********************************************/

/** Helper macro to call #mcuxClMemory_copy_reversed with flow protection. */
#define MCUXCLMEMORY_FP_MEMORY_COPY_REVERSED(pTarget, pSource, byteLen)  \
    MCUX_CSSL_FP_FUNCTION_CALL_VOID(mcuxClMemory_copy_reversed((uint8_t *) (pTarget), (const uint8_t *) (pSource), byteLen, byteLen))

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLMEMORY_COPY_REVERSED_H_ */

/**
 * @}
 */
