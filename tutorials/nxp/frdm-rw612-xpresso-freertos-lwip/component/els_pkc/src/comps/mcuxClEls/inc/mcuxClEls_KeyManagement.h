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
 * @file  mcuxClEls_KeyManagement.h
 * @brief ELS header for key management.
 *
 * This header exposes functions that can be used to manage the keystore of ELS.
 * This includes:
 * - Importing keys
 * @if MCUXCL_FEATURE_ELS_KEY_MGMT_EXPORT
 * - Exporting keys
 * @endif
 * @if MCUXCL_FEATURE_ELS_KEY_MGMT_DELETE
 * - Deleting keys
 * @endif
 */

/**
 * @defgroup mcuxClEls_KeyManagement mcuxClEls_KeyManagement
 * @brief This part of the @ref mcuxClEls driver supports functionality for keys management
 * @ingroup mcuxClEls
 * @{
 */

#ifndef MCUXCLELS_KEYMANAGEMENT_H_
#define MCUXCLELS_KEYMANAGEMENT_H_

#include <mcuxClConfig.h> // Exported features flags header
#include <mcuxClEls_Common.h> // Common functionality

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************
 * CONSTANTS
 **********************************************/
/**
 * @defgroup mcuxClEls_KeyManagement_Macros mcuxClEls_KeyManagement_Macros
 * @brief Defines all macros of @ref mcuxClEls_KeyManagement
 * @ingroup mcuxClEls_KeyManagement
 * @{
 */

/**
 * @defgroup MCUXCLELS_KEYIMPORT_VALUE_KFMT_ MCUXCLELS_KEYIMPORT_VALUE_KFMT_
 * @brief Defines valid options (word value) to be used by #mcuxClEls_KeyImport_Async
 * @ingroup mcuxClEls_KeyManagement_Macros
 *
 * @{
 */

#define MCUXCLELS_KEYIMPORT_VALUE_KFMT_UDF      ((uint32_t) 0u<< 6u) ///< Key format UDF with shares in RTL or memory
#define MCUXCLELS_KEYIMPORT_VALUE_KFMT_RFC3394  ((uint32_t) 1u<< 6u) ///< Key format RFC3394 with shares in memory
#define MCUXCLELS_KEYIMPORT_VALUE_KFMT_PUF      ((uint32_t) 2u<< 6u) ///< Key from PUF
#ifdef MCUXCL_FEATURE_ELS_PUK_INTERNAL
#define MCUXCLELS_KEYIMPORT_VALUE_KFMT_PBK      ((uint32_t) 3u<< 6u) ///< Key from Public Key Certificate
#endif /* MCUXCL_FEATURE_ELS_PUK_INTERNAL */

/**
 * @}
 */

/**
 * @defgroup MCUXCLELS_KEYIMPORT_KFMT_ MCUXCLELS_KEYIMPORT_KFMT_
 * @brief Defines valid options (bit values) to be used by #mcuxClEls_KeyImport_Async
 * @ingroup mcuxClEls_KeyManagement_Macros
 *
 * @{
 */
#define MCUXCLELS_KEYIMPORT_KFMT_UDF             (0x00u) ///< Key format UDF with shares in RTL or memory
#define MCUXCLELS_KEYIMPORT_KFMT_RFC3394         (0x01u) ///< Key format RFC3394 with shares in memory
#define MCUXCLELS_KEYIMPORT_KFMT_PUF             (0x02u) ///< Key from PUF
#ifdef MCUXCL_FEATURE_ELS_PUK_INTERNAL
#define MCUXCLELS_KEYIMPORT_KFMT_PBK             (0x03u) ///< Key from Public Key Certificate
#endif /* MCUXCL_FEATURE_ELS_PUK_INTERNAL */

#ifdef MCUXCL_FEATURE_ELS_PUK_INTERNAL
#define MCUXCLELS_KEYIMPORT_REVERSEFETCH_ENABLE  (0x01U) ///< Reverse fetch enabled. For internal use
#define MCUXCLELS_KEYIMPORT_REVERSEFETCH_DISABLE (0x00U) ///< Reverse fetch disabled. For internal use
#endif /* MCUXCL_FEATURE_ELS_PUK_INTERNAL */

#define MCUXCLELS_RFC3394_OVERHEAD               ((size_t) 16u)     ///< Overhead between RFC3394 blob and key size
/**
 * @}
 */

/**
 * @defgroup MCUXCLELS_RFC3394_ MCUXCLELS_RFC3394_
 * @brief Defines specifying the length of RFC3394 containers
 * @ingroup mcuxClEls_KeyManagement_Macros
 *
 * @{
 */
#define MCUXCLELS_RFC3394_CONTAINER_SIZE_128     ((size_t) 256u/8u) ///< Size of RFC3394 container for 128 bit key
#define MCUXCLELS_RFC3394_CONTAINER_SIZE_256     ((size_t) 384u/8u) ///< Size of RFC3394 container for 256 bit key
#ifdef MCUXCL_FEATURE_ELS_PUK_INTERNAL
#define MCUXCLELS_RFC3394_CONTAINER_SIZE_P256    ((size_t) 640u/8u) ///< Size of RFC3394 container for P256 bit public key
#endif /* MCUXCL_FEATURE_ELS_PUK_INTERNAL */
/**
 * @}
 */

/**
 * @}
 */

/**********************************************
 * TYPEDEFS
 **********************************************/
/**
 * @defgroup mcuxClEls_KeyManagement_Types mcuxClEls_KeyManagement_Types
 * @brief Defines all types of @ref mcuxClEls_KeyManagement
 * @ingroup mcuxClEls_KeyManagement
 * @{
 */

/**
 * @brief Command option bit field for #mcuxClEls_KeyImport_Async
 *
 * Bit field to configure #mcuxClEls_KeyImport_Async. 
 * See @ref MCUXCLELS_KEYIMPORT_KFMT_ for possible options in case the struct is accessed bit-wise.
 * See @ref MCUXCLELS_KEYIMPORT_VALUE_KFMT_ for possible options in case the struct is accessed word-wise.
 */
typedef union
{
    struct
    {
        uint32_t value;     ///< Accesses the bit field as a full word; initialize with a combination of constants from @ref MCUXCLELS_KEYIMPORT_VALUE_KFMT_
    } word;                 ///< Access #mcuxClEls_KeyImportOption_t word-wise
    struct
    {
        uint32_t :4;        ///< RFU
        uint32_t revf :1;   ///< This field is managed internally
        uint32_t :1;        ///< RFU
        uint32_t kfmt :2;   ///< Defines the key import format, one of @ref MCUXCLELS_KEYIMPORT_KFMT_
        uint32_t :24;       ///< RFU
    } bits;                 ///< Access #mcuxClEls_KeyImportOption_t bit-wise
} mcuxClEls_KeyImportOption_t;

/**
 * @}
 */

/**********************************************
 * FUNCTIONS
 **********************************************/
/**
 * @defgroup mcuxClEls_KeyManagement_Functions mcuxClEls_KeyManagement_Functions
 * @brief Defines all functions of @ref mcuxClEls_KeyManagement
 * @ingroup mcuxClEls_KeyManagement
 * @{
 */

/** 
 * @brief Deletes a key from keystore at the given index.
 * 
 * Before execution, ELS will wait until #mcuxClEls_HwState_t.drbgentlvl == #MCUXCLELS_STATUS_DRBGENTLVL_LOW. This can lead to a delay if the DRBG is in a state with less security strength at the time of the call.
 *
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 * @param[in]    keyIdx  The index of the key to be deleted
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 * @retval #MCUXCLELS_STATUS_SW_INVALID_PARAM    if invalid parameters were specified
 * @retval #MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT if a running operation prevented the request
 * @retval #MCUXCLELS_STATUS_SW_FAULT            if a failure occurred
 * @retval #MCUXCLELS_STATUS_OK_WAIT             on successful request */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_KeyDelete_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_KeyDelete_Async(
        mcuxClEls_KeyIndex_t keyIdx
);



/** @brief Imports a key from external storage to an internal key register.
 * 
 * @if ELS_AES_WITH_SIDE_CHANNEL_PROTECTION
 * Before execution, ELS will wait until #mcuxClEls_HwState_t.drbgentlvl == #MCUXCLELS_STATUS_DRBGENTLVL_LOW. This can lead to a delay if the DRBG is in a state with less security strength at the time of the call.
 * @endif
 *
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 * @param[in]    options          One of @ref MCUXCLELS_KEYIMPORT_KFMT_
 * @param[in]    pImportKey       Pointer to the RFC3394 container of the key to be imported
 * @param[in]    importKeyLength  Length of the RFC3394 container of the key to be imported
 * @param[in]    wrappingKeyIdx   Index of the key wrapping key, if importing RFC3394 format
 * @param[in]    targetKeyIdx     The desired key index of the imported key
 *
 *  <dl>
 *   <dt>Parameter properties</dt>
 *   <dd><dl>
 *     <dt>@p options.kfmt != #MCUXCLELS_KEYIMPORT_KFMT_RFC3394</dt><dd>
 *       <ul style="list-style: none;">
 *         <li>@p pImportKey is ignored.</li>
 *         <li>@p importKeyLength is ignored.</li>
 *         <li>@p wrappingKeyIdx is ignored.</li>
 *         <li>@p targetKeyIdx is ignored. The unpacked key is automatically stored in key slots 0, 1.</li>
 *       </ul></dd>
 *     </dt>
 *   </dl></dd>
 *  </dl>
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 * @retval #MCUXCLELS_STATUS_SW_INVALID_PARAM    if invalid parameters were specified
 * @retval #MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT if a running operation prevented the request
 * @retval #MCUXCLELS_STATUS_OK_WAIT             on successful request */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_KeyImport_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_KeyImport_Async(
    mcuxClEls_KeyImportOption_t options,
    uint8_t const * pImportKey,
    size_t importKeyLength,
    mcuxClEls_KeyIndex_t wrappingKeyIdx,
    mcuxClEls_KeyIndex_t targetKeyIdx
    );

#ifdef MCUXCL_FEATURE_ELS_PUK_INTERNAL
/** @brief Imports a public key to an internal key register if the signature verification of the provided public key against
 *         the provided signature is correct.
 * 
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 * @param[in]    pCertificate       Pointer to the Certificate structure
 * @param[in]    certificateLength  Length of the Certificate structure
 * @param[in]    publicKeyOffset    Offset of the Public key to be imported within @p pCertificate
 * @param[in]    pSignature         Signed challenge used to authenticate the imported key. Must be word aligned
 * @param[in]    verifyingKeyIdx    The key index of the verifying public key
 * @param[in]    keyProperties      The desired key properties of the imported key
 * @param[in]    targetKeyIdx       The desired key index of the imported key
 * @param[out]   pOutput            Pointer to the memory area which will receive the recalculated value of the R component in case of a successful
 *                                  certificate verification. Must be word aligned
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 * @retval #MCUXCLELS_STATUS_SW_INVALID_PARAM    if invalid parameters were specified
 * @retval #MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT if a running operation prevented the request
 * @retval #MCUXCLELS_STATUS_OK_WAIT             on successful request */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_KeyImportPuk_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_KeyImportPuk_Async(
    uint8_t const * pCertificate,
    size_t certificateLength,
    size_t publicKeyOffset,
    uint8_t const * pSignature,
    mcuxClEls_KeyIndex_t verifyingKeyIdx,
    mcuxClEls_KeyProp_t keyProperties,
    mcuxClEls_KeyIndex_t targetKeyIdx,
    uint8_t * pOutput	
    );
#endif /* MCUXCL_FEATURE_ELS_PUK_INTERNAL */

/** @brief Exports a key from an internal key register to external storage, using a wrapping key.
 * 
 * @if ELS_AES_WITH_SIDE_CHANNEL_PROTECTION
 * Before execution, ELS will wait until #mcuxClEls_HwState_t.drbgentlvl == #MCUXCLELS_STATUS_DRBGENTLVL_LOW. This can lead to a delay if the DRBG is in a state with less security strength at the time of the call.
 * @endif
 *
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 * @param[in]    wrappingKeyIdx     The key used for key wrapping
 * @param[in]    exportKeyIdx       The key to export
 * @param[out]   pOutput            The memory address of the exported key
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 * @retval #MCUXCLELS_STATUS_SW_INVALID_PARAM    if invalid parameters were specified
 * @retval #MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT if a running operation prevented the request
 * @retval #MCUXCLELS_STATUS_OK_WAIT             on successful request */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_KeyExport_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_KeyExport_Async(
    mcuxClEls_KeyIndex_t wrappingKeyIdx, ///< [in]  The key used for key wrapping
    mcuxClEls_KeyIndex_t exportKeyIdx,   ///< [in]  The key to export
    uint8_t * pOutput                   ///< [out] The memory address of the exported key
    );

/** @brief Exports the properties of the keys stored in the ELS internal keystore
 *
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 * @param[in]    keyIdx     Request key properties of the index defined here
 * @param[out]   pKeyProp   Key properties of the index provided
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 * @retval #MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT if a running operation prevented the request
 * @retval #MCUXCLELS_STATUS_OK                  on successful request */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_GetKeyProperties)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GetKeyProperties(
    mcuxClEls_KeyIndex_t keyIdx,
    mcuxClEls_KeyProp_t * pKeyProp
    );

/**
 * @}
 */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLELS_KEYMANAGEMENT_H_ */

/**
 * @}
 */
