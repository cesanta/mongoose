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
 * @file  mcuxClEls_Kdf.h
 * @brief ELS header for key derivation.
 *
 * This header exposes functions that enable using the ELS for various key derivation commands.
 * The supported key derivation algorithms are CKDF, HKDF, TLS
 */


/**
 * @defgroup mcuxClEls_Kdf mcuxClEls_Kdf
 * @brief This part of the @ref mcuxClEls driver supports functionality for key derivation
 * @ingroup mcuxClEls
 * @{
 */

/**
 * @defgroup mcuxClEls_Kdf_Macros mcuxClEls_Kdf_Macros
 * @brief Defines all macros of @ref mcuxClEls_Kdf
 * @ingroup mcuxClEls_Kdf
 * @{
 */

#ifndef MCUXCLELS_KDF_H_
#define MCUXCLELS_KDF_H_

#include <mcuxClConfig.h> // Exported features flags header





#include <mcuxClEls_Common.h> // Common functionality

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************
 * CONSTANTS
 **********************************************/

/**
 * @defgroup mcuxClEls_Kdf_Define mcuxClEls_Kdf_Define
 * @brief  constants
 * @ingroup mcuxClEls_Kdf_Macros
 * @{
 */


#define MCUXCLELS_CKDF_DERIVATIONDATA_SIZE               12u ///< Size of CKDF SP800-108 derivation data
#define MCUXCLELS_CKDF_ALGO_SP800108                     0x0u ///< Use SP800-108 algorithm




#define MCUXCLELS_HKDF_RFC5869_DERIVATIONDATA_SIZE 32u ///< Size of HKDF derivation data
#define MCUXCLELS_HKDF_SP80056C_TARGETKEY_SIZE     32u ///< Size of HKDF SP800-56C derived key

#define MCUXCLELS_HKDF_VALUE_RTF_DERIV             ((uint32_t) 1u<< 0u) ///< Use RTF as derivation input
#define MCUXCLELS_HKDF_VALUE_MEMORY_DERIV          ((uint32_t) 0u<< 0u) ///< Use derivation input from system memory

#define MCUXCLELS_HKDF_ALGO_RFC5869                0x0u ///< Use RFC5869 algorithm
#define MCUXCLELS_HKDF_ALGO_SP80056C               0x1u ///< Use SP800-56C algorithm

#define MCUXCLELS_HKDF_RTF_DERIV                   1U ///< Use RTF as derivation input
#define MCUXCLELS_HKDF_SYSTEM_MEMORY_DERIV         0U ///< Use derivation input from system memory

#define MCUXCLELS_TLS_DERIVATIONDATA_SIZE          ((size_t) 80u) ///< Size of TLS derivation data
#define MCUXCLELS_TLS_RANDOM_SIZE                  ((size_t) 32u) ///< Size of random bytes for TLS

#define MCUXCLELS_TLS_INIT                         0u ///< Perform master key generation
#define MCUXCLELS_TLS_FINALIZE                     1u ///< Perform session key generation

/**
 * @}
 */ /* mcuxClEls_Kdf_Define */

/**********************************************
 * TYPEDEFS
 **********************************************/

/**
 * @defgroup mcuxClEls_Kdf_Types mcuxClEls_Kdf_Types
 * @brief Defines all types of @ref mcuxClEls_Kdf
 * @ingroup mcuxClEls_Kdf
 * @{
 */

/** Internal command option bit field for CKDF functions. */
typedef union
{
    struct
    {
        uint32_t value;         ///< Accesses the bit field as a full word; initialize with a combination of constants from @ref MCUXCLELS_HKDF_VALUE_
    } word;
    struct
    {
        uint32_t :12;
        uint32_t ckdf_algo :2;  ///< Defines which algorithm and mode shall be used. This option is set internally and will be ignored:
                                ///< #MCUXCLELS_CKDF_ALGO_SP800108  = Use SP800-108 algorithm
        uint32_t :18;
    } bits;
} mcuxClEls_CkdfOption_t;

/** Command option bit field for #mcuxClEls_Hkdf_Rfc5869_Async. */
typedef union
{
    struct
    {
        uint32_t value;         ///< Accesses the bit field as a full word; initialize with a combination of constants from @ref MCUXCLELS_HKDF_VALUE_
    } word;
    struct
    {
        uint32_t rtfdrvdat :1;  ///< #MCUXCLELS_HKDF_SYSTEM_MEMORY_DERIV=use derivation input from system memory, #MCUXCLELS_HKDF_RTF_DERIV=use RTF (runtime fingerprint) as derivation input
        uint32_t hkdf_algo :1;  ///< Defines which algorithm shall be used. This option is set internally and will be ignored:
                                ///< #MCUXCLELS_HKDF_ALGO_RFC5869 = Use RFC5869 algorithm
                                ///< #MCUXCLELS_HKDF_ALGO_SP80056C = Use SP800-56C algorithm
        uint32_t :30;
    } bits;
} mcuxClEls_HkdfOption_t;

/** Internal command option bit field for #mcuxClEls_TlsGenerateMasterKeyFromPreMasterKey_Async, and #mcuxClEls_TlsGenerateMasterKeyFromPreMasterKey_Async. */
typedef union
{
    struct
    {
        uint32_t value;         ///< Accesses the bit field as a full word; initialize with a combination of constants from @ref MCUXCLELS_HKDF_VALUE_
    } word;
    struct
    {
        uint32_t :10;
        uint32_t mode :1;       ///< Defines which phase of the key generation is performed. This option is set internally and will be ignored:
                                ///< #MCUXCLELS_TLS_INIT  = Calculate master key from premaster key
                                ///< #MCUXCLELS_TLS_FINALIZE = Calculate session keys from master key
        uint32_t :21;
    } bits;
} mcuxClEls_TlsOption_t;

/**
 * @}
 */ /* mcuxClEls_Kdf_Types */

/**
 * @}
 */ /* mcuxClEls_Kdf_Macros */


/**********************************************
 * FUNCTIONS
 **********************************************/
/**
 * @defgroup mcuxClEls_Kdf_Functions mcuxClEls_Kdf_Functions
 * @brief Defines all functions of @ref mcuxClEls_Kdf
 * @ingroup mcuxClEls_Kdf
 * @{
 */


/**
 * @brief Derives a key using the HKDF (HMAC-based key derivation function) according to RFC5869.
 * @if MCUXCL_FEATURE_ELS_SHA_DIRECT
 * It must be ensured that SHA-Direct mode is disabled when calling this function (see #mcuxClEls_ShaDirect_Disable).
 * @endif
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 * @param[in] options The command options. For more information, see #mcuxClEls_HkdfOption_t.
 * @param[in] derivationKeyIdx Key index used for derivation. Must be a 256-bit key with HKDF property bit set to 1.
 * @param[in] targetKeyIdx Key bank number of the derived key. Will be a 256-bit key, the user must ensure there is enough space in the keystore to hold the derived key.
 * @param[in] targetKeyProperties Requested properties for the derived key. The ksize field will be ignored.
 * @param[in] pDerivationData The algorithm-specific derivation data, the length is #MCUXCLELS_HKDF_RFC5869_DERIVATIONDATA_SIZE bytes
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 *
 *
 * @retval #MCUXCLELS_STATUS_SW_INVALID_PARAM    if invalid parameters were specified
 * @retval #MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT if a running operation prevented the request
 * @retval #MCUXCLELS_STATUS_OK_WAIT             on successful request */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_Hkdf_Rfc5869_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_Hkdf_Rfc5869_Async(
    mcuxClEls_HkdfOption_t options,
    mcuxClEls_KeyIndex_t derivationKeyIdx,
    mcuxClEls_KeyIndex_t targetKeyIdx,
    mcuxClEls_KeyProp_t targetKeyProperties,
    uint8_t const * pDerivationData
    );

/** Derives a key using the HKDF (HMAC-based key derivation function) according to SP800-56C one-step approach with Sha2-256.
 * @if MCUXCL_FEATURE_ELS_SHA_DIRECT
 * It must be ensured that SHA-Direct mode is disabled when calling this function (see #mcuxClEls_ShaDirect_Disable).
 * @endif
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 * @param[in] derivationKeyIdx  Key index used for derivation. Must be a 256-bit key with HKDF property bit set to 1.
 * @param[out] pTagetKey Memory area to store the derived key. Will be a 256-bit key, the user must ensure there is enough space in the keystore to hold the derived key.
 * @param[in] pDerivationData The algorithm-specific derivation data
 * @param[in] derivationDataLength Length of the derivation data
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 * @retval #MCUXCLELS_STATUS_SW_INVALID_PARAM    if invalid parameters were specified
 * @retval #MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT if a running operation prevented the request
 * @retval #MCUXCLELS_STATUS_OK_WAIT             on successful request */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_Hkdf_Sp80056c_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_Hkdf_Sp80056c_Async(
    mcuxClEls_KeyIndex_t derivationKeyIdx,
    uint8_t * pTagetKey,
    uint8_t const * pDerivationData,
    size_t derivationDataLength
    );


/** Derives a key using the NIST SP 800-108 CMAC-based Extract-and-Expand Key Derivation Function.
 * 
 * @if ELS_AES_WITH_SIDE_CHANNEL_PROTECTION
 * Before execution, ELS will wait until #mcuxClEls_HwState_t.drbgentlvl == #MCUXCLELS_STATUS_DRBGENTLVL_LOW. This can lead to a delay if the DRBG is in a state with less security strength at the time of the call.
 * @endif
 * 
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 *   @param[in] derivationKeyIdx Key index used for derivation
 *   @param[in] targetKeyIdx Key bank number of the derived key
 *   @param[in] targetKeyProperties Requested properties for the derived key. Only set usage bits.
 *   @param[in] pDerivationData The algorithm-specific derivation data, the length is #MCUXCLELS_CKDF_DERIVATIONDATA_SIZE bytes
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 * @retval #MCUXCLELS_STATUS_SW_INVALID_PARAM    if invalid parameters were specified
 * @retval #MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT if a running operation prevented the request
 * @retval #MCUXCLELS_STATUS_OK_WAIT             on successful request */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_Ckdf_Sp800108_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_Ckdf_Sp800108_Async(
    mcuxClEls_KeyIndex_t derivationKeyIdx,
    mcuxClEls_KeyIndex_t targetKeyIdx,
    mcuxClEls_KeyProp_t targetKeyProperties,
    uint8_t const * pDerivationData
    );



/** Generates a TLS master key based on a pre-master key and derivation data, according to the TLS 1.2 specification.
 * The pre-master key is overwritten in this operation.
 * @if MCUXCL_FEATURE_ELS_SHA_DIRECT
 * It must be ensured that SHA-Direct mode is disabled when calling this function (see #mcuxClEls_ShaDirect_Disable).
 * @endif
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 * @param[in] pDerivationData The TLS derivation data, consisting of Label, Client Random and Server Random from the TLS 1.2 specification.
 *                            Note: The order is different from #mcuxClEls_TlsGenerateSessionKeysFromMasterKey_Async.
 * @param[in] keyProperties Desired key properties. Only #mcuxClEls_KeyProp_t::upprot_priv and #mcuxClEls_KeyProp_t::upprot_sec are used, the rest are ignored.
 * @param[in] keyIdx The index of the TLS pre-master key, which is overwritten with the master key
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 * @retval #MCUXCLELS_STATUS_SW_INVALID_PARAM    if invalid parameters were specified
 * @retval #MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT if a running operation prevented the request
 * @retval #MCUXCLELS_STATUS_OK_WAIT             on successful request */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_TlsGenerateMasterKeyFromPreMasterKey_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_TlsGenerateMasterKeyFromPreMasterKey_Async(
    uint8_t const * pDerivationData,
    mcuxClEls_KeyProp_t keyProperties,
    mcuxClEls_KeyIndex_t keyIdx
    );

/** Generates TLS session keys based on a master key and derivation data, according to the TLS 1.2 specification.
 * The master key and the following five key indices are overwritten in this operation.
 * The keys are written in the following order:
 * <ol>
 *     <li> Client Encryption Key
 *     <li> Client Message Authentication Key
 *     <li> Server Encryption Key
 *     <li> Server Message Authentication Key
 * </ol>
 * @if MCUXCL_FEATURE_ELS_SHA_DIRECT
 * It must be ensured that SHA-Direct mode is disabled when calling this function (see #mcuxClEls_ShaDirect_Disable).
 * @endif
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 * @param[in] pDerivationData The TLS derivation data, consisting of Label, Server Random and Client Random from the TLS 1.2 specification.
 *                            Note: The order is different from #mcuxClEls_TlsGenerateMasterKeyFromPreMasterKey_Async.
 * @param[in] keyProperties Desired key properties. Only #mcuxClEls_KeyProp_t::upprot_priv and #mcuxClEls_KeyProp_t::upprot_sec are used, the rest are ignored.
 * @param[in]  keyIdx  The index of the TLS master key, which is overwritten with one of the session keys.
 *                     There must be three further consecutive unoccupied key indices following this index.
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 * @retval #MCUXCLELS_STATUS_SW_INVALID_PARAM    if invalid parameters were specified
 * @retval #MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT if a running operation prevented the request
 * @retval #MCUXCLELS_STATUS_OK_WAIT             on successful request */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_TlsGenerateSessionKeysFromMasterKey_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_TlsGenerateSessionKeysFromMasterKey_Async(
    uint8_t const * pDerivationData,
    mcuxClEls_KeyProp_t keyProperties,
    mcuxClEls_KeyIndex_t keyIdx
    );

/**
 * @}
 */ /* mcuxClEls_Kdf_Functions */

/**
 * @}
 */ /* mcuxClEls_Kdf */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLELS_KDF_H_ */
