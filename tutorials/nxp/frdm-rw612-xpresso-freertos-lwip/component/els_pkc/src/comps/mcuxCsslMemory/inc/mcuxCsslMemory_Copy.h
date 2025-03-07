/*--------------------------------------------------------------------------*/
/* Copyright 2021, 2023 NXP                                                 */
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
 * @file  mcuxCsslMemory_Copy.h
 * @brief Include file for memory copy function
 */

#ifndef MCUXCSSLMEMORY_COPY_H_
#define MCUXCSSLMEMORY_COPY_H_

/**
 * @defgroup mcuxCsslMemory_Copy mcuxCssl Memory Copy
 * @brief Control Flow Protected Memory Copy Function
 *
 * @ingroup mcuxCsslMemory
 * @{
 */

/**
 * @defgroup mcuxCsslMemory_Copy_Functions mcuxCsslMemory_Copy Function Definitions
 * @brief mcuxCsslMemory_Copy Function Definitions
 *
 * @ingroup mcuxCsslMemory_Copy
 * @{
 */

/**
 * @brief Copies @p length bytes of data from @p pSrc to @p pDst
 *
 * The implementation is secure in the following aspects:
 *
 * * Constant execution time: If @p pSrc and @p pDst have the same offset to the nearest 16-byte boundary, and if @p length
 *     is the same, the execution sequence of the code is always identical.
 * * Parameter integrity protection: An incorrect parameter checksum makes the function return immediately.
 * * Code flow protection: The function call is protected. Additionally, the result depends on all steps of the calculation.
 * * Buffer overflow protection: No data is written to @p pDst beyond @p dstLength bytes.
 *
 * @param[in]     chk        The parameter checksum, generated with #mcuxCsslParamIntegrity_Protect.
 * @param[in]     pSrc       The data to be copied. Must not be NULL. Must not overlap with @p pDst.
 * @param[out]    pDst       The destination pointer. Must not be NULL. Must not overlap with @p pSrc.
 * @param[in]     dstLength  The size of the destination data buffer in bytes.
 * @param[in]     length     The number of bytes to copy.
 * @return A status code encapsulated in a flow-protection type.
 * @retval #MCUXCSSLMEMORY_STATUS_OK If the operation was successful.
 * @retval #MCUXCSSLMEMORY_STATUS_INVALID_PARAMETER If one of the parameters was invalid (i.e. @p pSrc or @p pDst was NULL or @p length was zero).
 * @retval #MCUXCSSLMEMORY_STATUS_FAULT If a fault was detected.
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxCsslMemory_Copy)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxCsslMemory_Status_t) mcuxCsslMemory_Copy
(
    mcuxCsslParamIntegrity_Checksum_t chk,
    void const * pSrc,
    void * pDst,
    size_t dstLength,
    size_t length
);

/**
 * @}
 */

/**
 * @}
 */

#endif /* MCUXCSSLMEMORY_COPY_H_ */
