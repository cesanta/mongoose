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

/** @file  mcuxClMemory_Set.h
 *  @brief Memory header for set function.
 * This header exposes functions that enable using memory set functions.
 */

/**
 * @defgroup mcuxClMemory_Set mcuxClMemory_Set
 * @brief This function sets all bytes in a memory region to a specified value.
 * @ingroup mcuxClMemory
 * @{
 */

#ifndef MCUXCLMEMORY_SET_H_
#define MCUXCLMEMORY_SET_H_

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
 * Sets all bytes of a memory buffer to a specified value.
 * 
 * If the destination buffer is too small, i.e. if bufLength < length, 
 * (length-bufLength) is added to the Flow Protection token (see @ref mcuxCsslFlowProtection).
 *  
 * @param[out]  pDst       pointer to the buffer to be set.
 * @param[in]   val        byte value to be set.
 * @param[in]   length     size (in bytes) to be set.
 * @param[in]   bufLength  buffer size (if bufLength < length, only bufLength bytes are set).
 *
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClMemory_set)
MCUX_CSSL_FP_PROTECTED_TYPE(void) mcuxClMemory_set (uint8_t *pDst, uint8_t val, size_t length, size_t bufLength);

/**********************************************
 * MACROS
 **********************************************/

/** Helper macro to call #mcuxClMemory_set with flow protection. */
#define MCUXCLMEMORY_FP_MEMORY_SET(pTarget, val, byteLen)  \
    MCUX_CSSL_FP_FUNCTION_CALL_VOID(mcuxClMemory_set((uint8_t *) (pTarget), val, byteLen, byteLen))

/** Helper macro to call #mcuxClMemory_set with flow protection with buffer. */
#define MCUXCLMEMORY_FP_MEMORY_SET_WITH_BUFF(pTarget, val, byteLen, buffLen)  \
        MCUX_CSSL_FP_FUNCTION_CALL_VOID(mcuxClMemory_set((uint8_t *) (pTarget), val, byteLen, buffLen))


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLMEMORY_SET_H_ */

/**
 * @}
 */
