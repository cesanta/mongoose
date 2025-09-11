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

/** @file  mcuxClMemory_Compare_Internal.h
 *  @brief Memory header for internal compare function.
 * This header exposes functions that enable memory compare function.
 */

/**
 * @defgroup mcuxClMemory_Compare_Internal mcuxClMemory_Compare_Internal
 * @brief This function compares two memory region @p lhs and @p rhs.
 * @ingroup mcuxClMemory
 * @{
 */

#ifndef MCUXCLMEMORY_COMPARE_INTERNAL_H_
#define MCUXCLMEMORY_COMPARE_INTERNAL_H_

#include <mcuxClConfig.h>  // Exported features flags header

#include <mcuxClCore_Platform.h>
#include <mcuxClMemory_Constants.h>
#include <mcuxClMemory_Types.h>
#include <mcuxCsslMemory.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxCsslDataIntegrity.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************
 * FUNCTIONS
 **********************************************/

/**
 * Compares two memory buffer with security agains faults.
 *
 * The two buffers must not overlap.
 * 
 * * Data Integrity: Record(pLhs + pRhs + length)
 *  
 * @param[in]  pLhs        pointer to the left buffer to be compared.
 * @param[in]  pRhs        pointer to the right buffer to be compared.
 * @param[in]  length      size (in bytes) to be compared.
 * 
 * @return A status code encapsulated in a flow-protection type.
 * @retval #MCUXCLMEMORY_STATUS_EQUAL                 If length bytes of Lhs and Rhs are equal.
 * @retval #MCUXCLMEMORY_STATUS_NOT_EQUAL             If at least one bytes differ between the two.
 * @retval #MCUXCLMEMORY_STATUS_FAULT
 */

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClMemory_compare_int)
static inline MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClMemory_Status_t) mcuxClMemory_compare_int
(
    const uint8_t * pLhs,
    const uint8_t * pRhs,
    uint32_t length
)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClMemory_compare_int);

    mcuxClMemory_Status_t retval = MCUXCLMEMORY_STATUS_FAULT;

        MCUX_CSSL_FP_FUNCTION_CALL(csslRetval, 
            mcuxCsslMemory_Compare(mcuxCsslParamIntegrity_Protect(3u,  pLhs, pRhs, length),
            pLhs, pRhs, length));
        retval = (mcuxClMemory_Status_t) csslRetval ^ (MCUXCSSLMEMORY_COMPONENT_MASK ^ MCUXCLMEMORY_COMPONENT_MASK);  // May return invalid parameters too.
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClMemory_compare_int, retval, MCUX_CSSL_FP_FUNCTION_CALLED(mcuxCsslMemory_Compare));
}


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLMEMORY_COMPARE_INTERNAL_H_ */

/**
 * @}
 */
