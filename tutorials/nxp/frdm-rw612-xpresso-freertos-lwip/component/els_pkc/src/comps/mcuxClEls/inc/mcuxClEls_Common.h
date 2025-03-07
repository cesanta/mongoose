/*--------------------------------------------------------------------------*/
/* Copyright 2020-2023 NXP                                                  */
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
 * @file  mcuxClEls_Common.h
 * @brief ELS header for common functionality.
 *
 * This header exposes functions that support hardware state management for other ELS commands.
 */

/**
 * @defgroup mcuxClEls_Common mcuxClEls_Common
 * @brief This part of the @ref mcuxClEls driver supports common functionality
 * @ingroup mcuxClEls
 * @{
 */
#ifndef MCUXCLELS_COMMON_H_
#define MCUXCLELS_COMMON_H_

#include <mcuxClConfig.h> // Exported features flags header
#include <mcuxClEls_Types.h> // Common types
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup mcuxClEls_Common_Macros mcuxClEls_Common_Macros
 * @brief Defines all macros of @ref mcuxClEls_Common
 * @ingroup mcuxClEls_Common
 * @{
 */

#define MCUXCLELS_API    ///< Marks a function as a public API function of the mcuxClEls component

/**********************************************
 * CONSTANTS
 **********************************************/

#ifdef MCUXCL_FEATURE_ELS_DMA_FINAL_ADDRESS_READBACK
  #define MCUXCLELS_DMA_READBACK_PROTECTION_TOKEN MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_CompareDmaFinalOutputAddress)
#else
  #define MCUXCLELS_DMA_READBACK_PROTECTION_TOKEN (0u)
#endif


/**
 * @defgroup mcuxClEls_InterruptOptionEn_t_Macros mcuxClEls_InterruptOptionEn_t
 * @brief Defines interrupt enable option values
 * @ingroup mcuxClEls_Common_Macros
 * @{
 */
#define MCUXCLELS_ELS_INTERRUPT_ENABLE              (0x01U) ///< Set this option at #mcuxClEls_InterruptOptionEn_t.elsint to allow ELS to trigger an interrupt
#define MCUXCLELS_ELS_INTERRUPT_DISABLE             (0x00U) ///< Set this option at #mcuxClEls_InterruptOptionEn_t.elsint to prevent ELS from triggering an interrupt
#ifdef MCUXCL_FEATURE_ELS_GLITCHDETECTOR
#define MCUXCLELS_GLITCH_DETECTOR_INTERRUPT_ENABLE  ((uint32_t) 1U) ///< Set this option at #mcuxClEls_InterruptOptionEn_t.gdetint to allow the Glitch Detector to trigger an interrupt
#define MCUXCLELS_GLITCH_DETECTOR_INTERRUPT_DISABLE ((uint32_t) 0U) ///< Set this option at #mcuxClEls_InterruptOptionEn_t.gdetint to prevent the Glitch Detector from triggering an interrupt
#endif /* MCUXCL_FEATURE_ELS_GLITCHDETECTOR */
/**@}*/

/**
 * @defgroup mcuxClEls_InterruptOptionRst_t_Macros mcuxClEls_InterruptOptionRst_t
 * @brief Defines interrupt reset option values
 * @ingroup mcuxClEls_Common_Macros
 * @{
 */
#define MCUXCLELS_ELS_RESET_CLEAR             (0x01U) ///< Set this option at #mcuxClEls_InterruptOptionRst_t.elsint to reset the ELS interrupt flag
#define MCUXCLELS_ELS_RESET_KEEP              (0x00U) ///< Set this option at #mcuxClEls_InterruptOptionRst_t.elsint to keep the ELS interrupt flag
#ifdef MCUXCL_FEATURE_ELS_GLITCHDETECTOR
#define MCUXCLELS_GLITCH_DETECTOR_RESET_CLEAR ((uint32_t) 1U) ///< Set this option at #mcuxClEls_InterruptOptionRst_t.gdetint to reset the Glitch Detector interrupt flag
#define MCUXCLELS_GLITCH_DETECTOR_RESET_KEEP  ((uint32_t) 0U) ///< Set this option at #mcuxClEls_InterruptOptionRst_t.gdetint to keep the Glitch Detector interrupt flag
#endif /* MCUXCL_FEATURE_ELS_GLITCHDETECTOR */
/**@}*/

/**
 * @defgroup mcuxClEls_InterruptOptionSet_t_Macros mcuxClEls_InterruptOptionSet_t
 * @brief Defines interrupt set option values
 * @ingroup mcuxClEls_Common_Macros
 * @{
 */
#define MCUXCLELS_ELS_INTERRUPT_SET          (0x01U) ///< Set this option at #mcuxClEls_InterruptOptionSet_t.elsint to set the ELS interrupt flag
#define MCUXCLELS_ELS_INTERRUPT_KEEP         (0x00U) ///< Set this option at #mcuxClEls_InterruptOptionSet_t.elsint to leave the ELS interrupt flag unchanged
#ifdef MCUXCL_FEATURE_ELS_GLITCHDETECTOR
#define MCUXCLELS_GLITCH_DETECTOR_NEG_SET    (0x01U) ///< Set this option at #mcuxClEls_InterruptOptionSet_t.gdetint_neg to set the negative Glitch Detector interrupt flag
#define MCUXCLELS_GLITCH_DETECTOR_NEG_KEEP   (0x00U) ///< Set this option at #mcuxClEls_InterruptOptionSet_t.gdetint_neg to leave the negative Glitch Detector interrupt flag unchanged
#define MCUXCLELS_GLITCH_DETECTOR_POS_SET    (0x01U) ///< Set this option at #mcuxClEls_InterruptOptionSet_t.gdetint_pos to set the positive Glitch Detector interrupt flag
#define MCUXCLELS_GLITCH_DETECTOR_POS_KEEP   (0x00U) ///< Set this option at #mcuxClEls_InterruptOptionSet_t.gdetint_pos to leave the positive Glitch Detector interrupt flag unchanged
#endif /* MCUXCL_FEATURE_ELS_GLITCHDETECTOR */
/**@}*/

/**
 * @defgroup MCUXCLELS_ERROR_FLAGS_ MCUXCLELS_ERROR_FLAGS_
 * @brief Options for error flag clearing
 * @ingroup mcuxClEls_Common_Macros
 * @{
 */
#define MCUXCLELS_ERROR_FLAGS_KEEP  ((mcuxClEls_ErrorHandling_t) 0x0u) ///< Set this option at #mcuxClEls_ErrorHandling_t to not clear any error flags
#define MCUXCLELS_ERROR_FLAGS_CLEAR ((mcuxClEls_ErrorHandling_t) 0x1u) ///< Set this option at #mcuxClEls_ErrorHandling_t to clear all ELS error flags
/**@}*/

/**
 * @defgroup MCUXCLELS_RESET_ MCUXCLELS_RESET_
 * @brief Options for reset handling
 * @ingroup mcuxClEls_Common_Macros
 * @{
 */
#define MCUXCLELS_RESET_DO_NOT_CANCEL ((mcuxClEls_ResetOption_t) 0x0u) ///< Set this option at #mcuxClEls_ResetOption_t to abort the requested command if another ELS operation is still running
#define MCUXCLELS_RESET_CANCEL        ((mcuxClEls_ResetOption_t) 0x1u) ///< Set this option at #mcuxClEls_ResetOption_t to execute the requested command even if another ELS operation is still running
/**@}*/

/**
 * @defgroup MCUXCLELS_STATUS_PPROT_ MCUXCLELS_STATUS_PPROT_
 * @brief Values for the privilege/security level of ELS commands
 *
 * Note that some keys and memory areas may only be accessible when ELS is on a certain privilege/security level.
 *
 * The default value, before any command has been executed, is #MCUXCLELS_STATUS_PPROT_UNPRIVILEGED_NONSECURE.
 *
 * @ingroup mcuxClEls_Common_Macros
 * @{
 */
#define MCUXCLELS_STATUS_PPROT_UNPRIVILEGED_SECURE    ((uint32_t) 0x0u) ///< This value of #mcuxClEls_HwState_t.pprot means that the most recently started ELS operation was or is running in unprivileged secure mode
#define MCUXCLELS_STATUS_PPROT_PRIVILEGED_SECURE      ((uint32_t) 0x1u) ///< This value of #mcuxClEls_HwState_t.pprot means that the most recently started ELS operation was or is running in privileged secure mode
#define MCUXCLELS_STATUS_PPROT_UNPRIVILEGED_NONSECURE ((uint32_t) 0x2u) ///< This value of #mcuxClEls_HwState_t.pprot means that the most recently started ELS operation was or is running in unprivileged non-secure mode
#define MCUXCLELS_STATUS_PPROT_PRIVILEGED_NONSECURE   ((uint32_t) 0x3u) ///< This value of #mcuxClEls_HwState_t.pprot means that the most recently started ELS operation was or is running in privileged non-secure mode
/**@}*/

/**
 * @defgroup MCUXCLELS_STATUS_ECDSAVFY_ MCUXCLELS_STATUS_ECDSAVFY_
 * @brief ECDSA verify check values
 * @ingroup mcuxClEls_Common_Macros
 * @{
 */
#define MCUXCLELS_STATUS_ECDSAVFY_NORUN  ((uint32_t) 0x0u) ///< This value of #mcuxClEls_HwState_t.ecdsavfy means that no ECDSA verify operation has been executed
#define MCUXCLELS_STATUS_ECDSAVFY_FAIL   ((uint32_t) 0x1u) ///< This value of #mcuxClEls_HwState_t.ecdsavfy means that the most recently finished ECDSA signature verification failed
#define MCUXCLELS_STATUS_ECDSAVFY_OK     ((uint32_t) 0x2u) ///< This value of #mcuxClEls_HwState_t.ecdsavfy means that the most recently finished ECDSA signature verification passed
#define MCUXCLELS_STATUS_ECDSAVFY_ERROR  ((uint32_t) 0x3u) ///< This value of #mcuxClEls_HwState_t.ecdsavfy means that an error has occurred
/**@}*/

/**
 * @defgroup MCUXCLELS_STATUS_DRBGENTLVL_ MCUXCLELS_STATUS_DRBGENTLVL_
 * @brief Constants for Entropy quality of the current DRBG instance
 * @ingroup mcuxClEls_Common_Macros
 * @{ */
#define MCUXCLELS_STATUS_DRBGENTLVL_NONE ((uint32_t) 0x0u) ///< This value of #mcuxClEls_HwState_t.drbgentlvl means that the DRBG is not running
#define MCUXCLELS_STATUS_DRBGENTLVL_LOW  ((uint32_t) 0x1u) ///< This value of #mcuxClEls_HwState_t.drbgentlvl means that the DRBG can generate random numbers with a low security strength (sufficient for commands with a low DRBG security strength requirement, see the function description to check which level is required)
#define MCUXCLELS_STATUS_DRBGENTLVL_HIGH ((uint32_t) 0x2u) ///< This value of #mcuxClEls_HwState_t.drbgentlvl means that the DRBG can generate random numbers with 128 bits of security strength (sufficient for commands with a high DRBG security strength requirement, see the function description to check which level is required)
/** @} */

#ifdef MCUXCL_FEATURE_ELS_LOCKING
/**
 * @defgroup MCUXCLELS_LOCKING_ MCUXCLELS_LOCKING_
 * @brief Constants for ELS locking feature
 * @ingroup mcuxClEls_Common_Macros
 * @{ */
#define MCUXCLELS_MASTER_UNLOCK_ANY         ((uint32_t) 0x1Fu) ///< Any bus master ID can override ELS lock
/** @} */
#endif /* MCUXCL_FEATURE_ELS_LOCKING */



/**
 * @}
 */

/**********************************************
 * TYPEDEFS
 **********************************************/

/**
 * @defgroup mcuxClEls_Common_Types mcuxClEls_Common_Types
 * @brief Defines all types of @ref mcuxClEls_Common
 * @ingroup mcuxClEls_Common
 * @{
 */

/**
 * @brief Result type of #mcuxClEls_GetHwVersion
 *
 * Contains the ELS version value.
 */
typedef union
{
    struct
    {
        uint32_t value; ///< Accesses the bit field as a full word
    } word;             ///< Access #mcuxClEls_HwVersion_t word-wise
    struct
    {
        uint32_t revision :4; ///< Revision number
        uint32_t minor :8;    ///< Minor version
        uint32_t major :4;    ///< Major version
#ifndef MCUXCL_FEATURE_ELS_GET_FW_VERSION
        uint32_t level :4;    ///< Release level version
        uint32_t :12;         ///< RFU
#else /* MCUXCL_FEATURE_ELS_GET_FW_VERSION */
        uint32_t fw_revision :4; ///< Firmware Revision number
        uint32_t fw_minor :8;    ///< Firmware Minor version
        uint32_t fw_major :4;    ///< Firmware Major version
#endif /* MCUXCL_FEATURE_ELS_GET_FW_VERSION */
    } bits;                   ///< Access #mcuxClEls_HwVersion_t bit-wise
} mcuxClEls_HwVersion_t;

/**
 * @brief Result type of #mcuxClEls_GetHwState
 *
 * Contains ELS status information.
 */
typedef union
{
    struct
    {
        uint32_t value; ///< Accesses the bit field as a full word
    } word;             ///< Access #mcuxClEls_HwState_t word-wise
    struct
    {
        uint32_t busy :1;       ///< ELS is busy
        uint32_t irq :1;        ///< ELS interrupt activated
        uint32_t err :1;        ///< ELS is in error state
        uint32_t prngready :1;  ///< ELS PRNG is seeded and ready to use
        uint32_t ecdsavfy :2;   ///< ECDSA verify operation state (For possible values of this field, see @ref MCUXCLELS_STATUS_ECDSAVFY_)
        uint32_t pprot :2;      ///< The privilege/security level of the most recently started ELS command (For possible values of this field, see @ref MCUXCLELS_STATUS_PPROT_)
        uint32_t drbgentlvl :2; ///< Entropy quality of the current DRBG instance (For possible values of this field, see @ref MCUXCLELS_STATUS_DRBGENTLVL_)
        uint32_t dtrng_busy: 1; ///< Indicates the DTRNG is gathering entropy
#ifdef MCUXCL_FEATURE_ELS_GLITCHDETECTOR
        uint32_t gdet_pos :1;   ///< Glitch detector interrupt activated (positive)
        uint32_t gdet_neg :1;   ///< Glitch detector interrupt activated (negative)
#else
        uint32_t :2;            ///< RFU
#endif /* MCUXCL_FEATURE_ELS_GLITCHDETECTOR */
        uint32_t :3;            ///< RFU
#ifdef MCUXCL_FEATURE_ELS_LOCKING
        uint32_t els_locked :1; ///< ELS is locked
#else
        uint32_t :1;            ///< RFU
#endif /* MCUXCL_FEATURE_ELS_LOCKING */
        uint32_t :15;           ///< RFU
    } bits;                     ///< Access #mcuxClEls_HwState_t bit-wise
} mcuxClEls_HwState_t;

/**
 * @brief Type to handle ELS error clearing options
 *
 * For possible values, see @ref MCUXCLELS_ERROR_FLAGS_.
 */
typedef uint32_t mcuxClEls_ErrorHandling_t;

/**
 * @brief Type to handle ELS reset options
 *
 * For possible values, see @ref MCUXCLELS_RESET_.
 */
typedef uint32_t mcuxClEls_ResetOption_t;

/**
 * @brief Command option type for #mcuxClEls_SetIntEnableFlags and #mcuxClEls_GetIntEnableFlags
 *
 * Used to get/set ELS interrupt enable options.
 */
typedef union
{
    struct
    {
        uint32_t value; ///< Accesses the bit field as a full word
    } word;             ///< Access #mcuxClEls_InterruptOptionEn_t word-wise
    struct
    {
        uint32_t elsint :1;  ///< Whether ELS interrupt should be used. (For possible values of this field, see @ref mcuxClEls_InterruptOptionEn_t_Macros)
#ifdef MCUXCL_FEATURE_ELS_GLITCHDETECTOR
        uint32_t gdetint :1; ///< Whether Glitch detector interrupt should be used. (For possible values of this field, see @ref mcuxClEls_InterruptOptionEn_t_Macros)
#else
        uint32_t :1;         ///< RFU
#endif /* MCUXCL_FEATURE_ELS_GLITCHDETECTOR */
        uint32_t :30;        ///< RFU
    } bits;                  ///< Access #mcuxClEls_InterruptOptionEn_t bit-wise
} mcuxClEls_InterruptOptionEn_t;

/**
 * @brief Type to control which ELS interrupts should be reset when calling #mcuxClEls_ResetIntFlags
 */
typedef union
{
    struct
    {
        uint32_t value; ///< Accesses the bit field as a full word
    } word;             ///< Access #mcuxClEls_InterruptOptionRst_t word-wise
    struct
    {
        uint32_t elsint :1;  ///< Whether ELS interrupt should be reset. (For possible values of this field, see @ref mcuxClEls_InterruptOptionRst_t_Macros)
#ifdef MCUXCL_FEATURE_ELS_GLITCHDETECTOR
        uint32_t gdetint :1; ///< Whether Glitch detector interrupt should be reset. (For possible values of this field, see @ref mcuxClEls_InterruptOptionRst_t_Macros)
#else
        uint32_t :1;         ///< RFU
#endif /* MCUXCL_FEATURE_ELS_GLITCHDETECTOR */
        uint32_t :30;        ///< RFU
    } bits;                  ///< Access #mcuxClEls_InterruptOptionRst_t bit-wise
} mcuxClEls_InterruptOptionRst_t;

/**
 * @brief Type to control which ELS interrupts should be set when calling #mcuxClEls_SetIntFlags
 */
typedef union
{
    struct
    {
        uint32_t value; ///< Accesses the bit field as a full word
    } word;             ///< Access #mcuxClEls_InterruptOptionSet_t word-wise
    struct
    {
        uint32_t elsint :1;     ///< Whether ELS interrupt should be set. (For possible values of this field, see @ref mcuxClEls_InterruptOptionSet_t_Macros)
#ifdef MCUXCL_FEATURE_ELS_GLITCHDETECTOR
        uint32_t gdetint_neg :1;///< Whether Glitch detector neg interrupt should be set. (For possible values of this field, see @ref mcuxClEls_InterruptOptionSet_t_Macros)
        uint32_t gdetint_pos :1;///< Whether Glitch detector pos interrupt should be set. (For possible values of this field, see @ref mcuxClEls_InterruptOptionSet_t_Macros)
#else
        uint32_t :2;            ///< RFU
#endif /* MCUXCL_FEATURE_ELS_GLITCHDETECTOR */
        uint32_t :29;           ///< RFU
    } bits;                     ///< Access #mcuxClEls_InterruptOptionSet_t bit-wise
} mcuxClEls_InterruptOptionSet_t;

/**
 * @brief Result type of #mcuxClEls_GetHwConfig
 *
 * Contains ELS configuration values.
 */
typedef union
{
    struct
    {
        uint32_t value; ///< Accesses the bit field as a full word
    } word;             ///< Access #mcuxClEls_InterruptOptionSet_t word-wise
    struct
    {
        uint32_t ciphersup :1;       ///< Indicates whether the cipher command is supported
        uint32_t authciphersup :1;   ///< Indicates whether the auth_cipher command is supported
        uint32_t ecsignsup :1;       ///< Indicates whether the ecsign command is supported
        uint32_t ecvfysup :1;        ///< Indicates whether the ecvfy command is supported
        uint32_t eckxchsup :1;       ///< Indicates whether the dhkey_xch command is supported
        uint32_t keygensup :1;       ///< Indicates whether the keygen command is supported
        uint32_t keyinsup :1;        ///< Indicates whether the keyin command is supported
        uint32_t keyoutsup :1;       ///< Indicates whether the keyout command  is supported
        uint32_t kdeletesup :1;      ///< Indicates whether the kdelete command is supported
        uint32_t keyprovsup :1;      ///< Indicates whether the keyprov command is supported
        uint32_t ckdfsup :1;         ///< Indicates whether the ckdf command is supported
        uint32_t hkdfsup :1;         ///< Indicates whether the hkdf command is supported
        uint32_t tlsinitsup :1;      ///< Indicates whether the tls_init command is supported
        uint32_t hashsup :1;         ///< Indicates whether the hash command is supported
        uint32_t hmacsup :1;         ///< Indicates whether the hmac command is supported
        uint32_t cmacsup :1;         ///< Indicates whether the cmac command is supported
        uint32_t drbgreqsup :1;      ///< Indicates whether the drbg_req command is supported
        uint32_t drbgtestsup :1;     ///< Indicates whether the drbg_test command is supported
        uint32_t dtrgncfgloadsup :1; ///< Indicates whether the dtrng_cfg_load command is is supported
        uint32_t dtrngevalsup :1;    ///< Indicates whether the dtrng_eval command is supported
        uint32_t gdetcfgloadsup :1;  ///< Indicates whether the gdet_cfg_load command is supported
        uint32_t gdettrimsup :1;     ///< Indicates whether the gdet_trim command is supported
        uint32_t :10;                ///< RFU
    } bits;                          ///< Access #mcuxClEls_InterruptOptionSet_t bit-wise
} mcuxClEls_HwConfig_t;

#define drbgreqsub drbgreqsup ///< Deprecated name for #mcuxClEls_HwConfig_t.drbgreqsup


/**
 * @}
 */

/**********************************************
 * FUNCTIONS
 **********************************************/
/**
 * @defgroup mcuxClEls_Common_Functions mcuxClEls_Common_Functions
 * @brief Defines all functions of @ref mcuxClEls_Common
 * @ingroup mcuxClEls_Common
 * @{
 */
/**
 * @brief Determines the version of the underlying ELS hardware IP.
 *
 * @attention This header was delivered as part of a CLNS release which is compatible with a specific ELS hardware IP version.
 *
 * @param[out] result Pointer which will be filled with the ELS hardware version
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code is always #MCUXCLELS_STATUS_OK
 * @else
 *  @return An error code that is always #MCUXCLELS_STATUS_OK
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_GetHwVersion)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GetHwVersion(
    mcuxClEls_HwVersion_t * result
    );

#ifdef MCUXCL_FEATURE_ELS_HWCONFIG
/**
 * @brief Determines the hardware configuration of the underlying ELS hardware IP.
 *
 * @param[out] result Pointer which will be filled with the ELS hardware configuration
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code is always #MCUXCLELS_STATUS_OK
 * @else
 *  @return An error code that is always #MCUXCLELS_STATUS_OK
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_GetHwConfig)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GetHwConfig(
    mcuxClEls_HwConfig_t * result
    );
#endif /* MCUXCL_FEATURE_ELS_HWCONFIG */

/**
 * @brief Determines the current state of the ELS.
 *
 * @param[out] result Pointer which will be filled with the ELS status information
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code is always #MCUXCLELS_STATUS_OK
 * @else
 *  @return An error code that is always #MCUXCLELS_STATUS_OK
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_GetHwState)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GetHwState(
    mcuxClEls_HwState_t * result
    );

/**
 * @brief Enables the ELS.
 *
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code is always #MCUXCLELS_STATUS_OK_WAIT
 * @else
 *  @return An error code that is always #MCUXCLELS_STATUS_OK_WAIT
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_Enable_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_Enable_Async(
    void
    );

/**
 * @brief Perform a synchronous reset of the ELS.
 *
 * This means that:
 * - any running ELS command will be stopped,
 * - all errors will be cleared,
 * - all keys will be deleted,
 * - any RNG entropy will be discarded,
 * - the glitch detector will be reset and
 * - the run-time fingerprint will be restored to its default value.
 *
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 * @param[in] options A value indicating whether any running ELS operations shall be canceled
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 *
 * @retval #MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT if a running operation prevented the reset
 * @retval #MCUXCLELS_STATUS_OK_WAIT             on successful request
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_Reset_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_Reset_Async(
    mcuxClEls_ResetOption_t options
    );

/**
 * @brief Disable the ELS.
 *
 * This is useful as a power saving mechanism.
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code is always #MCUXCLELS_STATUS_OK
 * @else
 *  @return An error code that is always #MCUXCLELS_STATUS_OK
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_Disable)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_Disable(
    void
);

/**
 * @brief Set interrupt enable flags.
 *
 * @param[in] options The command options, determining which interrupts should be enabled or disabled. For more information, see #mcuxClEls_InterruptOptionEn_t.
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code is always #MCUXCLELS_STATUS_OK
 * @else
 *  @return An error code that is always #MCUXCLELS_STATUS_OK
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_SetIntEnableFlags)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_SetIntEnableFlags(
    mcuxClEls_InterruptOptionEn_t options
    );

/**
 * @brief Get interrupt enable flags.
 *
 * @param[out] result Pointer which is filled with the configuration of the interrupts enable register.
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code is always #MCUXCLELS_STATUS_OK
 * @else
 *  @return An error code that is always #MCUXCLELS_STATUS_OK
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_GetIntEnableFlags)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GetIntEnableFlags(
    mcuxClEls_InterruptOptionEn_t * result
    );

/**
 * @brief Clear the interrupt status register.
 *
 * @param[in] options The command options, determining which interrupt status bits should be cleared. For more information, see #mcuxClEls_InterruptOptionRst_t.
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code is always #MCUXCLELS_STATUS_OK
 * @else
 *  @return An error code that is always #MCUXCLELS_STATUS_OK
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_ResetIntFlags)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_ResetIntFlags(
    mcuxClEls_InterruptOptionRst_t options
    );

/**
 * @brief Set the interrupt status register, for debug and testing purposes.
 *
 * @param[in] options The command options, determining which interrupt status bits should be set. For more information, see #mcuxClEls_InterruptOptionSet_t.
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code is always #MCUXCLELS_STATUS_OK
 * @else
 *  @return An error code that is always #MCUXCLELS_STATUS_OK
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_SetIntFlags)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_SetIntFlags(
    mcuxClEls_InterruptOptionSet_t options
    );

/**
 * @brief Wait for an ELS operation and optionally clear the error status.
 *
 * If an ELS operation is active, this function waits for completion of that operation. For this, the
 * busy flag of ELS is polled. Additionally, this function checks and returns any applicable error indication.
 * If no operation is active, the function returns immediately.
 *
 * @param[in] errorHandling Define if error flags shall be cleared.
 *
 * @retval #MCUXCLELS_STATUS_OK if the last operation was successful, or no operation was active
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_WaitForOperation)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_WaitForOperation(
    mcuxClEls_ErrorHandling_t errorHandling
    );

/**
 * @brief Await the completion of an ELS operation for a limited amount of time and optionally clear the error status.
 *
 * If an ELS operation is active, this function waits for completion of that operation until a counter expires.
 * For this, the busy flag of ELS is polled. The counting mechanism behaves like a simple for-loop from
 * @p counterLimit to one. This counter does not have a well-defined relationship to real-world time.
 * Additionally, this function checks and returns any applicable error indication.
 * If no operation is active, the function returns immediately.
 *
 * @param[in] counterLimit The limit of the wait counter.
 * @param[in] errorHandling Define if error flags shall be cleared.
 *
 * @retval #MCUXCLELS_STATUS_OK if the last operation was successful, or no operation was active
 * @retval #MCUXCLELS_STATUS_SW_COUNTER_EXPIRED if the counter expired while waiting for the operation to complete
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_LimitedWaitForOperation)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_LimitedWaitForOperation(
    uint32_t counterLimit,
    mcuxClEls_ErrorHandling_t errorHandling
    );

/**
 * @brief Resets all error flags that have been set by a previous operation.
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code is always #MCUXCLELS_STATUS_OK
 * @else
 *  @return An error code that is always #MCUXCLELS_STATUS_OK
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_ResetErrorFlags)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_ResetErrorFlags(
    void);

/**
 * @brief Get the last ELS error code and optionally clear the error status.
 *
 * @param[in] errorHandling Define if error flags shall be cleared.
 *
 * @retval #MCUXCLELS_STATUS_OK 	if the last operation was successful or no operation was active
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_GetErrorCode)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GetErrorCode(
    mcuxClEls_ErrorHandling_t errorHandling
    );

/**
 * @brief Get the last ELS error code and level and optionally clear the error status.
 *
 * @param[in]  errorHandling    Define if error flags shall be cleared.
 * @param[out] errorLevel       Pointer to the location that will receive the value of the error level.
 *
 * @retval #MCUXCLELS_STATUS_OK  if the last operation was successful or no operation was active
 * @retval #MCUXCLELS_STATUS_    if the last operation resulted in an error
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_GetErrorLevel)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GetErrorLevel(
    mcuxClEls_ErrorHandling_t errorHandling,
    uint32_t *errorLevel
    );

/**
 * @brief Set the random start delay for AES based operations. This impacts mcuxClEls_Aead_*, mcuxClEls_Cipher_*, mcuxClEls_Cmac_*, ncpClEls_Ckdf_*, mcuxClEls_KeyImport_Async, mcuxClEls_KeyExport_Async
 *
 * @param[in] startDelay Define the max random start delay. Acceptable values are a power of 2 minus one, starting from 0 to 1023 (0, 1, 3, 7, ..., 1023).
 *
 * @retval #MCUXCLELS_STATUS_OK if the operation was successful
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_SetRandomStartDelay)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_SetRandomStartDelay(
    uint32_t startDelay
    );

/**
 * @brief Get the random start delay for AES based operations.
 *
 * @param[out] startDelay Pointer to store random start delay configuration.
 *
 * @retval #MCUXCLELS_STATUS_OK if the operation was successful
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_GetRandomStartDelay)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GetRandomStartDelay(
    uint32_t * startDelay
    );

#ifdef MCUXCL_FEATURE_ELS_LOCKING
/**
 * @brief Lock ELS to a session
 *
 * This operation locks the ELS, when the lock is obtained a nonzero value will be stored in @p sessionId, and the PPROT and bus Master ID
 * of the locking command will be recorded. Subsequent unlocking can only be done by one of the following:
 *      1.  ELS reset
 *      2.  Normal unlock: calling #mcuxClEls_ReleaseLock with the correct @p sessionId, from the same bus Master ID and using the same PPROT settings.
 *      3.  Privileged unlock: calling #mcuxClEls_ReleaseLock with any value of @p sessionId, from the bus Master ID set with the #mcuxClEls_SetMasterUnlock
 *          command and PPROT settings 'secure privileged'.
 * While ELS is locked read access is only permitted to ELS_STATUS and ELS_SESSION_ID and writes are only permitted to ELS_RESET, except for accesses that
 * have the same bus Master ID and PPROT settings.
 *
 * @param[out] pSessionId  The session identifier assigned to the lock, it is required to unlock the session, it will be zero when a lock could not be obtained.
 *
 * @retval #MCUXCLELS_STATUS_OK                  if the operation was successful
 * @retval #MCUXCLELS_STATUS_SW_LOCKING_FAILED   if the operation was not successful
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_GetLock)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GetLock(
    uint32_t * pSessionId
    );

/**
 * @brief Release ELS lock for the session
 *
 * This operation unlocks the ELS when following conditions are met:
 *      1.  Normal unlock: providing the correct @p sessionId, from the same bus Master ID and using the same PPROT settings.
 *      2.  Privileged unlock: providing any value of @p sessionId, from the bus Master ID set with the #mcuxClEls_SetMasterUnlock command
 *          and PPROT settings 'secure privileged'.
 * Invalid attempts to unlock ELS will result in a bus error.
 *
 * @param[in] sessionId  The session identifier obtained while locking ELS
 *
 * @retval #MCUXCLELS_STATUS_OK   if the operation was successful
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_ReleaseLock)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_ReleaseLock(
    uint32_t sessionId
    );

/**
 * @brief Check if ELS is locked
 *
 * This operation returns the locking status of ELS.
 *
 * @retval #MCUXCLELS_STATUS_OK                  if the operation was successful and ELS is not locked
 * @retval #MCUXCLELS_STATUS_SW_STATUS_LOCKED    if the operation was successful and ELS is locked
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_IsLocked)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_IsLocked(void);

/**
 * @brief Set the bus master ID that can unlock ELS
 *
 * This operation sets the bus master ID of the master that can override ELS lock. This command can only be executed once after reset.
 * Invalid attempts to set the bus master ID will result in a bus error.
 *
 * @param[in] masterId  The bus master identifier that can override the ELS lock. Special value #MCUXCLELS_MASTER_UNLOCK_ANY allows any bus master identifier to override the ELS lock.
 *
 * @retval #MCUXCLELS_STATUS_OK                   if the operation was successful
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_SetMasterUnlock)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_SetMasterUnlock(
    uint32_t masterId
    );

#endif /* MCUXCL_FEATURE_ELS_LOCKING */


#ifdef MCUXCL_FEATURE_ELS_DMA_ADDRESS_READBACK
/**
 * @brief Reads back the last address processed by the ELS DMA (security feature)
 *
 * @param[out] pLastAddress	Pointer to the last address read/written by the ELS DMA
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code is always #MCUXCLELS_STATUS_OK
 * @else
 *  @return An error code that is always #MCUXCLELS_STATUS_OK
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_GetLastDmaAddress)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GetLastDmaAddress(
    uint32_t* pLastAddress
    );
#endif /* MCUXCL_FEATURE_ELS_DMA_ADDRESS_READBACK */

#ifdef MCUXCL_FEATURE_ELS_DMA_FINAL_ADDRESS_READBACK
/**
 * @brief Compares the last address processed by the ELS DMA with the expected final address of the output buffer given to the last ELS command (security feature).
 *        The given address @p outputStartAddress and expected length @p expectedLength determine the expected final address.
 *        This function can be used to verify that the final DMA transfer of an ELS command has completed as expected.
 *
 * @param[in] outputStartAddress Pointer to the output buffer of the last ELS operation
 * @param[in] expectedLength     Expected length of the output buffer of the last ELS operation
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 *
 * @retval #MCUXCLELS_STATUS_SW_COMPARISON_FAILED if the comparison between the expected final address and the actual final address processed by ELS fails
 * @retval #MCUXCLELS_STATUS_OK_WAIT              if the comparison was successful
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_CompareDmaFinalOutputAddress)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_CompareDmaFinalOutputAddress(
        uint8_t *outputStartAddress,
        size_t expectedLength
        );
#endif /* MCUXCL_FEATURE_ELS_DMA_FINAL_ADDRESS_READBACK  */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLELS_COMMON_H_ */

/**
 * @}
 *
 * @}
 */
