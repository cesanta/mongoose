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

/** @file  mcuxClMemory_CompareDPASecure_Internal.h
 *  @brief Memory header for dpa secure compare function.
 * This header exposes functions that enable dpa secure memory compare function.
 */

/**
 * @defgroup mcuxClMemory_Compare_DPASecure_Internal mcuxClMemory_Compare_DPASecure_Internal
 * @brief This function compares two memory region @p lhs and @p rhs.
 * @ingroup mcuxClMemory
 * @{
 */

#ifndef MCUXCLMEMORY_COMPARE_DPA_SECURE_INTERNAL_H_
#define MCUXCLMEMORY_COMPARE_DPA_SECURE_INTERNAL_H_

#include <mcuxClConfig.h>  // Exported features flags header

#include <mcuxClCore_Platform.h>
#include <mcuxClMemory_Constants.h>
#include <mcuxClMemory_Types.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxClSession_Types.h>
#include <internal/mcuxClMemory_CompareSecure_Internal.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************
 * FUNCTIONS
 **********************************************/

/**
 * Compares two memory buffers with security against fault and DPA.
 *
 * The two buffers must not overlap.
 * 
 * * Data Integrity: Record(pLhs + pRhs + length)
 *  
 * @param[in]  session     Handle for the current CL session.
 * @param[in]  pLhs        pointer to the left buffer to be compared.
 * @param[in]  pRhs        pointer to the right buffer to be compared.
 * @param[in]  length      size (in bytes) to be compared.
 * 
 * @return A status code encapsulated in a flow-protection type.
 * @retval #MCUXCLMEMORY_STATUS_EQUAL                 If length bytes of Lhs and Rhs are equal.
 * @retval #MCUXCLMEMORY_STATUS_NOT_EQUAL             If at least one bytes differ between the two.
 * @retval #MCUXCLMEMORY_STATUS_FAULT
 */

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClMemory_compare_dpasecure_int)
static inline MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClMemory_Status_t) mcuxClMemory_compare_dpasecure_int
(
    mcuxClSession_Handle_t session,
    const uint8_t * pLhs,
    const uint8_t * pRhs,
    uint32_t length
)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClMemory_compare_dpasecure_int);

        (void)session;
        MCUX_CSSL_FP_FUNCTION_CALL(retClCompare, mcuxClMemory_compare_secure_int(pLhs, pRhs, length));
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClMemory_compare_dpasecure_int, retClCompare, MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMemory_compare_secure_int));
}


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLMEMORY_COMPARE_DPA_SECURE_INTERNAL_H_ */

/**
 * @}
 */
