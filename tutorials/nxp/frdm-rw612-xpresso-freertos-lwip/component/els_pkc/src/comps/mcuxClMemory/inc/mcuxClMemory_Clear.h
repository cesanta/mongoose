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

/** @file  mcuxClMemory_Clear.h
 *  @brief Memory header for clear functions.
 * This header exposes functions that enable using memory clear function.
 */


/**
 * @defgroup mcuxClMemory_Clear mcuxClMemory_Clear
 * @brief This function clears a memory region.
 * @ingroup mcuxClMemory
 * @{
 */


#ifndef MCUXCLMEMORY_CLEAR_H_
#define MCUXCLMEMORY_CLEAR_H_

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
 * Overwrites a memory buffer with null bytes. 
 * 
 * If the destination buffer is too small, i.e. if bufLength < length, 
 * (length-bufLength) is added to the Flow Protection token (see @ref mcuxCsslFlowProtection).
 * 
 * @param[out]  pDst        Pointer to the buffer to be cleared.
 * @param[in]   length      size (in bytes) to be cleared.
 * @param[in]   bufLength   buffer size (if bufLength < length, only bufLength bytes are cleared).
 *
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClMemory_clear)
MCUX_CSSL_FP_PROTECTED_TYPE(void) mcuxClMemory_clear (uint8_t *pDst, size_t length, size_t bufLength);


/**********************************************
 * MACROS
 **********************************************/

/** Helper macro to call #mcuxClMemory_clear with flow protection. */
#define MCUXCLMEMORY_FP_MEMORY_CLEAR(pTarget, byteLen)  \
    MCUX_CSSL_FP_FUNCTION_CALL_VOID(mcuxClMemory_clear((uint8_t *) (pTarget), byteLen, byteLen))

/** Helper macro to call #mcuxClMemory_clear with flow protection with buffer. */
#define MCUXCLMEMORY_FP_MEMORY_CLEAR_WITH_BUFF(pTarget, byteLen, buffLen)  \
    MCUX_CSSL_FP_FUNCTION_CALL_VOID(mcuxClMemory_clear((uint8_t *) (pTarget), byteLen, buffLen))


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLMEMORY_CLEAR_H_ */

/**
 * @}
 */
