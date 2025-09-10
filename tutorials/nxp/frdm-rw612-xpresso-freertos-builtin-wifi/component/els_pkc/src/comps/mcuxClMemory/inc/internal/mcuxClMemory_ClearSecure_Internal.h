/*--------------------------------------------------------------------------*/
/* Copyright 2023 NXP                                                       */
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

/** @file  mcuxClMemory_ClearSecure_Internal.h
 *  @brief Memory header for secure clear function.
 * This header exposes functions that enable secure memory clear function.
 */

/**
 * @defgroup mcuxClMemory_Clear_Secure_Internal mcuxClMemory_Clear_Secure_Internal
 * @brief This function clears all bytes in a memory region to null in a secure way 
 * when a secure clear is available.
 * @ingroup mcuxClMemory
 * @{
 */

#ifndef MCUXCLMEMORY_CLEARSECURE_INTERNAL_H_
#define MCUXCLMEMORY_CLEARSECURE_INTERNAL_H_

#include <mcuxClConfig.h>  // Exported features flags header

#include <mcuxClCore_Platform.h>
#include <mcuxClMemory_Constants.h>
#include <mcuxClMemory_Types.h>
#include <mcuxClToolchain.h>
#include <mcuxCsslAnalysis.h>
#include <mcuxCsslFlowProtection.h>
#include <internal/mcuxClMemory_SetSecure_Internal.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************
 * FUNCTIONS
 **********************************************/

/**
 * Sets all bytes of a memory buffer to a specified value.
 *
 * The two buffers must not overlap.
 * 
 * * Data Integrity: Record(pSrc + pDst + length)
 *  
 * @param[out]  pDst       pointer to the buffer to be set.
 * @param[in]   length     size (in bytes) to be set.
 *
 * @return A status code encapsulated in a flow-protection type.
 * @retval #MCUXCLMEMORY_STATUS_OK                 If @p length bytes cleared at @p pDst.
 * @retval #MCUXCLMEMORY_STATUS_FAULT
 */

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClMemory_clear_secure_int)
static inline MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClMemory_Status_t) mcuxClMemory_clear_secure_int
(
    uint8_t * pDst,
    uint32_t length
)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClMemory_clear_secure_int);

        MCUX_CSSL_FP_FUNCTION_CALL(retval, mcuxClMemory_set_secure_int(pDst, 0u, length));
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClMemory_clear_secure_int, retval, MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_set_secure_int));
}


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLMEMORY_CLEARSECURE_INTERNAL_H_ */

/**
 * @}
 */
