/*--------------------------------------------------------------------------*/
/* Copyright 2020-2022 NXP                                                  */
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

/** @file  mcuxClEls_GlitchDetector.h
 *  @brief ELS header for controlling the glitch detector.
 *
 * This header exposes functions that configure the ELS glitch detector. 
 */
 
#ifndef MCUXCLELS_GLITCHDETECTOR_H_
#define MCUXCLELS_GLITCHDETECTOR_H_

#include <mcuxClConfig.h> // Exported features flags header
#include <mcuxClEls_Common.h> // Common types & functionality

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup mcuxClEls_GlitchDetector mcuxClEls_GlitchDetector
 * @brief This part of the @ref mcuxClEls driver supports glitch detector functionality
 * @ingroup mcuxClEls
 * @{
 */
 
/**********************************************
 * CONSTANTS
 **********************************************/
/**
 * @defgroup mcuxClEls_GlitchDetector_Macros mcuxClEls_GlitchDetector_Macros
 * @brief Defines all macros of @ref mcuxClEls_GlitchDetector
 * @ingroup mcuxClEls_GlitchDetector
 * @{
 */

#define MCUXCLELS_GLITCHDETECTOR_CFG_SIZE  ((size_t) 0x18u) ///< Glitch detector configuration size

#define MCUXCLELS_GLITCHDETECTOR_TRIM_SIZE  ((size_t) 0x04u) ///< Glitch detector trim value size

/**
 * @}
 */ /* mcuxClEls_GlitchDetector_Macros */

/**********************************************
 * FUNCTIONS
 **********************************************/
/**
 * @defgroup mcuxClEls_GlitchDetector_Functions mcuxClEls_GlitchDetector_Functions
 * @brief Defines all functions of @ref mcuxClEls_GlitchDetector
 * @ingroup mcuxClEls_GlitchDetector
 * @{
 */

/**
 * @brief Loads a glitch detector configuration.
 *
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 * @param[in]   Pointer to the memory area which contains the glitch detector configuration. The size is fixed at #MCUXCLELS_GLITCHDETECTOR_CFG_SIZE bytes.
 *
 * @retval #MCUXCLELS_STATUS_SW_INVALID_PARAM    if invalid parameters were specified
 * @retval #MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT if a running operation prevented the request
 * @retval #MCUXCLELS_STATUS_OK_WAIT             on successful request 
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_GlitchDetector_LoadConfig_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GlitchDetector_LoadConfig_Async(
    uint8_t const * pInput
    );

/** 
 * @brief Calculates optimal ELS glitch detector configuration and writes it to system memory.
 *
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 * @param[out]  Pointer to the memory area which receives the glitch detector trim value. The size is fixed at #MCUXCLELS_GLITCHDETECTOR_TRIM_SIZE bytes.
 *
 * @retval #MCUXCLELS_STATUS_SW_INVALID_PARAM    if invalid parameters were specified
 * @retval #MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT if a running operation prevented the request
 * @retval #MCUXCLELS_STATUS_OK_WAIT             on successful request 
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 *
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_GlitchDetector_Trim_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GlitchDetector_Trim_Async(
    uint8_t * pOutput 
    );

/** 
 * @brief Reads the glitch detector's event counter
 * 
 * This function converts the event counter from Gray code to an unsigned number.
 *
 * @param[in]   Pointer to the word where the counter value will be stored.
 *
 * @retval #MCUXCLELS_STATUS_OK                  on successful request 
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_GlitchDetector_GetEventCounter)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GlitchDetector_GetEventCounter(
    uint8_t * pCount 
    );

/** 
 * @brief Resets the glitch detector's event counter
 *
 * @retval #MCUXCLELS_STATUS_OK                  on successful request
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_GlitchDetector_ResetEventCounter)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GlitchDetector_ResetEventCounter( void
    );
 
/**
 * @}
 */ /* mcuxClEls_GlitchDetector_Functions */
 
/**
 * @}
 */ /* mcuxClEls_GlitchDetector */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLELS_GLITCHDETECTOR_H_ */
