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
 * @file mcuxClEls_Ecc.h
 * @brief ELS header for elliptic curve cryptography
 * This header exposes functions that enable using the ELS for elliptic curve cryptography.
 * All functions operate on the NIST P-256 curve.
 * The ECC operations supported are:
 * - ECC key generation
 * - ECC Diffie-Hellman key exchange
 * - ECDSA signature generation/verification
 */


#ifndef MCUXCLELS_ECC_H_
#define MCUXCLELS_ECC_H_

#include <mcuxClConfig.h> // Exported features flags header
#include <mcuxClEls_Common.h> // Common types & functionality

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup mcuxClEls_Ecc mcuxClEls_Ecc
 * @brief This part of the @ref mcuxClEls driver supports functionality for elliptic curve cryptography
 * @ingroup mcuxClEls
 * @{
 */


/**********************************************
 * CONSTANTS
 **********************************************/
/**
 * @defgroup mcuxClEls_Ecc_Macros mcuxClEls_Ecc_Macros
 * @brief Defines all macros of @ref mcuxClEls_Ecc
 * @ingroup mcuxClEls_Ecc
 * @{
 */

/**
 * @defgroup MCUXCLELS_ECC_VALUE_ MCUXCLELS_ECC (Sign and Verify) option word values
 * @brief Constants for #mcuxClEls_EccSignOption_t and #mcuxClEls_EccVerifyOption_t
 * @ingroup mcuxClEls_Ecc_Macros
 * @{
 */
#define MCUXCLELS_ECC_VALUE_HASHED              ((uint32_t) 0u<< 0u) ///< Set this option at #mcuxClEls_EccSignOption_t.value or #mcuxClEls_EccVerifyOption_t.value to specify input is the hash of the message
#define MCUXCLELS_ECC_VALUE_NOT_HASHED          ((uint32_t) 1u<< 0u) ///< Set this option at #mcuxClEls_EccSignOption_t.value or #mcuxClEls_EccVerifyOption_t.value to specify input is the plain message
#define MCUXCLELS_ECC_VALUE_RTF                 ((uint32_t) 1u<< 1u) ///< Set this option at #mcuxClEls_EccSignOption_t.value to include the RTF in the signature, only for #mcuxClEls_EccSignOption_t
#define MCUXCLELS_ECC_VALUE_NO_RTF              ((uint32_t) 0u<< 1u) ///< Set this option at #mcuxClEls_EccSignOption_t.value to not include the RTF in the signature, only for #mcuxClEls_EccSignOption_t
/**
 * @}
 */

/**
 * @defgroup MCUXCLELS_KEYGEN_VALUE_ MCUXCLELS_KEYGEN option word values
 * @brief Constants for #mcuxClEls_EccKeyGenOption_t
 * @ingroup mcuxClEls_Ecc_Macros
 * @{
 */
#define MCUXCLELS_KEYGEN_VALUE_SIGN_PUBLICKEY   ((uint32_t) 1u<< 0u) ///< Set this option at #mcuxClEls_EccKeyGenOption_t.value to sign the public key
#define MCUXCLELS_KEYGEN_VALUE_TYPE_SIGN        ((uint32_t) 0u<< 1u) ///< Set this option at #mcuxClEls_EccKeyGenOption_t.value to specify output key will be a signing key usable by #mcuxClEls_EccSign_Async
#define MCUXCLELS_KEYGEN_VALUE_TYPE_KEYEXCHANGE ((uint32_t) 1u<< 1u) ///< Set this option at #mcuxClEls_EccKeyGenOption_t.value to specify output key will be a Diffie Helman key usable by #mcuxClEls_EccKeyExchange_Async
#define MCUXCLELS_KEYGEN_VALUE_DETERMINISTIC    ((uint32_t) 0u<< 2u) ///< Set this option at #mcuxClEls_EccKeyGenOption_t.value to specify output key is deterministic
#define MCUXCLELS_KEYGEN_VALUE_RANDOM           ((uint32_t) 1u<< 2u) ///< Set this option at #mcuxClEls_EccKeyGenOption_t.value to specify output key is random
#define MCUXCLELS_KEYGEN_VALUE_GEN_PUB_KEY      ((uint32_t) 0u<< 3u) ///< Set this option at #mcuxClEls_EccKeyGenOption_t.value to generate a public key
#define MCUXCLELS_KEYGEN_VALUE_NO_PUB_KEY       ((uint32_t) 1u<< 3u) ///< Set this option at #mcuxClEls_EccKeyGenOption_t.value to not generate a public key
#define MCUXCLELS_KEYGEN_VALUE_NO_RANDOM_DATA   ((uint32_t) 0u<< 5u) ///< Set this option at #mcuxClEls_EccKeyGenOption_t.value to not use random data for signing the public key
#define MCUXCLELS_KEYGEN_VALUE_USE_RANDOM_DATA  ((uint32_t) 1u<< 5u) ///< Set this option at #mcuxClEls_EccKeyGenOption_t.value to use random data for signing the public key
/**
 * @}
 */

/**
 * @defgroup MCUXCLELS_ECC_VALUE_BITS MCUXCLELS_ECC (Sign and Verify) option bit field values
 * @brief Bit field constants for #mcuxClEls_EccSignOption_t and #mcuxClEls_EccVerifyOption_t
 * @ingroup mcuxClEls_Ecc_Macros
 * @{
 */
#define MCUXCLELS_ECC_HASHED                     (0U) ///< Set this option at #mcuxClEls_EccSignOption_t.echashchl or #mcuxClEls_EccVerifyOption_t.echashchl to specify input is the hash of the message
#define MCUXCLELS_ECC_NOT_HASHED                 (1U) ///< Set this option at #mcuxClEls_EccSignOption_t.echashchl or #mcuxClEls_EccVerifyOption_t.echashchl to specify input is the plain message

#define MCUXCLELS_ECC_RTF                        (1U) ///< Set this option at #mcuxClEls_EccSignOption_t.signrtf to include the RTF in the signature
#define MCUXCLELS_ECC_NO_RTF                     (0U) ///< Set this option at #mcuxClEls_EccSignOption_t.signrtf to not include the RTF in the signature
/**
 * @}
 */

/**
 * @defgroup MCUXCLELS_KEYGEN_VALUE_BITS MCUXCLELS_KEYGEN option bit field values
 * @brief Bit field constants for #mcuxClEls_EccKeyGenOption_t
 * @ingroup mcuxClEls_Ecc_Macros
 * @{
 */
#define MCUXCLELS_ECC_PUBLICKEY_SIGN_ENABLE      (1U) ///< Set this option at #mcuxClEls_EccKeyGenOption_t.kgsign to sign the public key (signature will be concatenated to the output public key)
#define MCUXCLELS_ECC_PUBLICKEY_SIGN_DISABLE     (0U) ///< Set this option at #mcuxClEls_EccKeyGenOption_t.kgsign to not sign the public key

#define MCUXCLELS_ECC_OUTPUTKEY_SIGN             (0U) ///< Set this option at #mcuxClEls_EccKeyGenOption_t.kgtypedh to specify output key will be a signing key usable by #mcuxClEls_EccSign_Async
#define MCUXCLELS_ECC_OUTPUTKEY_KEYEXCHANGE      (1U) ///< Set this option at #mcuxClEls_EccKeyGenOption_t.kgtypedh to specify output key will be a Diffie Helman key usable by #mcuxClEls_EccKeyExchange_Async

#define MCUXCLELS_ECC_OUTPUTKEY_DETERMINISTIC    (0U) ///< Set this option at #mcuxClEls_EccKeyGenOption_t.kgsrc to specify output key is deterministic
#define MCUXCLELS_ECC_OUTPUTKEY_RANDOM           (1U) ///< Set this option at #mcuxClEls_EccKeyGenOption_t.kgsrc to specify output key is random

#define MCUXCLELS_ECC_GEN_PUBLIC_KEY             (0U) ///< Set this option at #mcuxClEls_EccKeyGenOption_t.skip_pbk to generate a public key
#define MCUXCLELS_ECC_SKIP_PUBLIC_KEY            (1U) ///< Set this option at #mcuxClEls_EccKeyGenOption_t.skip_pbk to not generate a public key.
                                                   ///< If #MCUXCLELS_ECC_OUTPUTKEY_KEYEXCHANGE set, this option will be ignored and a public key will be generated.

#define MCUXCLELS_ECC_NO_RANDOM_DATA             (0U) ///< Set this option at #mcuxClEls_EccKeyGenOption_t.kgsign_rnd to not include user provided random data for the signature
#define MCUXCLELS_ECC_INCLUDE_RANDOM_DATA        (1U) ///< Set this option at #mcuxClEls_EccKeyGenOption_t.kgsign_rnd to include user provided random data for the signature.
                                                   ///< #MCUXCLELS_ECC_PUBLICKEY_SIGN_ENABLE must be set in this case.
/**
 * @}
 */

/**
 * @defgroup MCUXCLELS_INTERNAL_VALUE_BITS Option bit field values that are needed for internal use only
 * @brief Internal bit field constants for several option types.
 * @ingroup mcuxClEls_Ecc_Macros
 * @{
 */
#define MCUXCLELS_ECC_REVERSEFETCH_ENABLE        (0x01U) ///< Reverse Fetch enabled. For internal use
#define MCUXCLELS_ECC_REVERSEFETCH_DISABLE       (0x00U) ///< Reverse Fetch disabled. For internal use

#ifdef MCUXCL_FEATURE_ELS_PUK_INTERNAL_BIT
#define MCUXCLELS_ECC_EXTKEY_EXTERNAL        (0x01U) ///< Public key is taken from system memory. For internal use
#define MCUXCLELS_ECC_EXTKEY_INTERNAL        (0x00U) ///< Public key is taken from internal keystore. For internal use
#endif /* MCUXCL_FEATURE_ELS_PUK_INTERNAL_BIT */
/**
 * @}
 */

/**
 * @defgroup MCUXCLELS_ECC_SIZE MCUXCLELS_ECC_SIZE
 * @brief Defines size of public key and signature in bytes
 * @ingroup mcuxClEls_Ecc_Macros
 * @{
 */
#define MCUXCLELS_ECC_PUBLICKEY_SIZE     ((size_t) 64U) ///< Size of the public key
#define MCUXCLELS_ECC_SIGNATURE_SIZE     ((size_t) 64U) ///< Size of the signature
#define MCUXCLELS_ECC_SIGNATURE_R_SIZE   ((size_t) 32U) ///< Size of the signature part r
/**
 * @}
 */

/**
 * @}
 */ /* mcuxClEls_Ecc_Macros */


/**********************************************
 * TYPEDEFS
 **********************************************/
/**
 * @defgroup mcuxClEls_Ecc_Types mcuxClEls_Ecc_Types
 * @brief Defines all types of @ref mcuxClEls_Ecc
 * @ingroup mcuxClEls_Ecc
 * @{
 */

/**
 * @brief Data type for ECC parameters in ELS format
 * @deprecated All ELS ECC functions now operate on uint8_t. This type will be removed soon.
 */
typedef uint8_t mcuxClEls_EccByte_t;

/**
 * @brief Command option bit field for #mcuxClEls_EccSign_Async
 * Bit field to configure #mcuxClEls_EccSign_Async. See @ref MCUXCLELS_ECC_VALUE_BITS for possible options.
 */
typedef union
{
    struct
    {
        uint32_t value;         ///< Access the bit field as a full word; initialize with a combination of constants from @ref MCUXCLELS_ECC_VALUE_
    } word;                     ///< Access #mcuxClEls_EccSignOption_t word-wise
    struct
    {
        uint32_t echashchl :1;  ///< Define type of input, plain message or hash of message
        uint32_t signrtf :1;    ///< Define if signing the Run-Time Fingerprint
        uint32_t :2;            ///< RFU
        uint32_t revf :1;       ///< This field is managed internally
        uint32_t :27;           ///< RFU
    } bits;                     ///< Access #mcuxClEls_EccSignOption_t bit-wise
} mcuxClEls_EccSignOption_t;

/**
 * @brief Command option bit field for #mcuxClEls_EccVerify_Async
 * Bit field to configure #mcuxClEls_EccVerifyOption_t. See @ref MCUXCLELS_ECC_VALUE_BITS for possible options.
 */
typedef union
{
    struct
    {
        uint32_t value;         ///< Access the bit field as a full word; initialize with a combination of constants from @ref MCUXCLELS_ECC_VALUE_
    } word;                     ///< Access #mcuxClEls_EccVerifyOption_t word-wise
    struct
    {
        uint32_t echashchl :1;  ///< Define type of input, plain message or hash of message
        uint32_t :3;            ///< RFU
        uint32_t revf :1;       ///< This field is managed internally
#ifdef MCUXCL_FEATURE_ELS_PUK_INTERNAL_BIT
        uint32_t :8;            ///< RFU
        uint32_t extkey :1;     ///< This field is managed internally
        uint32_t :18;           ///< RFU
#else
        uint32_t :27;           ///< RFU
#endif /* MCUXCL_FEATURE_ELS_PUK_INTERNAL_BIT */
    } bits;                     ///< Access #mcuxClEls_EccVerifyOption_t bit-wise
} mcuxClEls_EccVerifyOption_t;

/**
 * @brief Command option bit field for #mcuxClEls_EccKeyGen_Async
 * Bit field to configure #mcuxClEls_EccKeyGenOption_t. See @ref MCUXCLELS_KEYGEN_VALUE_BITS for possible options.
 */
typedef union
{
    struct
    {
        uint32_t value;         ///< Accesses the bit field as a full word; initialize with a combination of constants from @ref MCUXCLELS_KEYGEN_VALUE_
    } word;                     ///< Access #mcuxClEls_EccKeyGenOption_t word-wise
    struct
    {
        uint32_t kgsign :1;     ///< Define if signing the output public key
        uint32_t kgtypedh :1;   ///< Define the usage of the output key
        uint32_t kgsrc :1;      ///< Define if the output key is deterministic or random
        uint32_t skip_pbk :1;   ///< Define if generating a public key
        uint32_t revf :1;       ///< This field is managed internally
        uint32_t kgsign_rnd :1; ///< Define if using user provided random data for the signature.
        uint32_t :26;           ///< RFU
    } bits;                     ///< Access #mcuxClEls_EccKeyGenOption_t bit-wise
} mcuxClEls_EccKeyGenOption_t;

/**
 * @brief Command option bit field for #mcuxClEls_EccKeyExchange_Async, for internal use only.
 * Bit field to configure #mcuxClEls_EccKeyExchOption_t.
 */
typedef union
{
    struct
    {
        uint32_t value;         ///< Accesses the bit field as a full word
    } word;                     ///< Access #mcuxClEls_EccKeyExchOption_t word-wise
    struct
    {
        uint32_t :4;            ///< RFU
        uint32_t revf :1;       ///< This field is managed internally
        uint32_t :8;            ///< RFU
        uint32_t extkey :1;     ///< This field is managed internally
        uint32_t :18;           ///< RFU
    } bits;                     ///< Access #mcuxClEls_EccKeyExchOption_t bit-wise
} mcuxClEls_EccKeyExchOption_t;

/**
 * @}
 */ /* mcuxClEls_Ecc_Types */


/**********************************************
 * FUNCTIONS
 **********************************************/
/**
 * @defgroup mcuxClEls_Ecc_Functions mcuxClEls_Ecc_Functions
 * @brief Defines all functions of @ref mcuxClEls_Ecc
 * @ingroup mcuxClEls_Ecc
 * @{
 */

/**
 * @brief Generates an ECC key pair on the NIST P-256 curve.
 * 
 * Before execution, ELS will wait until #mcuxClEls_HwState_t.drbgentlvl == #MCUXCLELS_STATUS_DRBGENTLVL_HIGH. This can lead to a delay if the DRBG is in a state with less security strength at the time of the call.
 *
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 * The public key will be stored in the standard ANSI X9.62 byte order (big-endian).
 *
 * @param[in]  options                The command options. For more information, see #mcuxClEls_EccKeyGenOption_t.
 * @param[in]  signingKeyIdx          The index of the key to sign the generated public key.
 * @param[in]  privateKeyIdx          Output key index.
 * @param[in]  generatedKeyProperties The desired key properties of the generated key.
 * @param[in]  pRandomData            Random data provided by the user.
 * @param[out] pPublicKey             Pointer to the memory area which receives the public key and optionally the key signature.
 *
 * <dl>
 *   <dt>Parameter properties</dt>
 *   <dd><dl>
 *     <dt>@p options.kgsign == #MCUXCLELS_ECC_PUBLICKEY_SIGN_DISABLE</dt>
 *       <dd>@p signingKeyIdx is ignored.</dd>
 *     <dt>@p options.kgsrc == #MCUXCLELS_ECC_OUTPUTKEY_DETERMINISTIC</dt>
 *       <dd>@p privateKeyIdx also defines the key index of the source key material.
 *           The source key material will be overwritten by the output public key.</dd>
 *     <dt>@p options.kgsign_rnd == #MCUXCLELS_ECC_NO_RANDOM_DATA</dt>
 *       <dd>@p pRandomData is ignored.</dd>
 *     <dt>@p pPublicKey must be aligned on a 4-byte boundary.</dt>
 *   </dl></dd>
 * </dl>
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 * @retval #MCUXCLELS_STATUS_SW_INVALID_PARAM    if invalid parameters were specified
 * @retval #MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT if a running operation prevented the request
 * @retval #MCUXCLELS_STATUS_OK_WAIT             on successful request
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_EccKeyGen_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_EccKeyGen_Async(
    mcuxClEls_EccKeyGenOption_t options,
    mcuxClEls_KeyIndex_t signingKeyIdx,
    mcuxClEls_KeyIndex_t privateKeyIdx,
    mcuxClEls_KeyProp_t generatedKeyProperties,
    uint8_t const * pRandomData,
    uint8_t * pPublicKey
    );

/**
 * @brief Performs a Diffie-Hellman key exchange with an internal ECC private key and an external ECC public key.
 * 
 * Before execution, ELS will wait until #mcuxClEls_HwState_t.drbgentlvl == #MCUXCLELS_STATUS_DRBGENTLVL_LOW. This can lead to a delay if the DRBG is in a state with less security strength at the time of the call.
 *
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 * The public key must be stored in the standard ANSI X9.62 byte order (big-endian).
 *
 * @param[in] privateKeyIdx          The private key index.
 * @param[in] pPublicKey             Pointer to the public key of a third party.
 * @param[in] sharedSecretIdx        The index in the ELS keystore that receives the shared secret that is generated by the ECDH operation.
 * @param[in] sharedSecretProperties The desired key properties of the shared secret.
 *
 * <dl>
 *   <dt>Parameter properties</dt>
 *   <dd><dl>
 *     <dt>@p pPublicKey </dt>
 *       <dd>The public key consists of the 256-bit X coordinate and the 256-bit Y coordinate.
 *           The point must lie on the NIST P-256 curve, be encoded in X9.62 format and aligned on a 4-byte boundary.</dd>
 *   </dl></dd>
 * </dl>
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 * @retval #MCUXCLELS_STATUS_SW_INVALID_PARAM    if invalid parameters were specified
 * @retval #MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT if a running operation prevented the request
 * @retval #MCUXCLELS_STATUS_OK_WAIT             on successful request
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_EccKeyExchange_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_EccKeyExchange_Async(
    mcuxClEls_KeyIndex_t privateKeyIdx,
    uint8_t const * pPublicKey,
    mcuxClEls_KeyIndex_t sharedSecretIdx,
    mcuxClEls_KeyProp_t sharedSecretProperties
    );

#ifdef MCUXCL_FEATURE_ELS_PUK_INTERNAL
/**
 * @brief Performs a Diffie-Hellman key exchange with an internal ECC private key and an internal ECC public key.
 * 
 * Before execution, ELS will wait until #mcuxClEls_HwState_t.drbgentlvl == #MCUXCLELS_STATUS_DRBGENTLVL_LOW. This can lead to a delay if the DRBG is in a state with less security strength at the time of the call.
 *
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 * @param[in] privateKeyIdx          The private key index.
 * @param[in] publicKeyIdx           The public key index.
 * @param[in] sharedSecretIdx        The index in the ELS keystore that receives the shared secret that is generated by the ECDH operation.
 * @param[in] sharedSecretProperties The desired key properties of the shared secret.
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 * @retval #MCUXCLELS_STATUS_SW_INVALID_PARAM    if invalid parameters were specified
 * @retval #MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT if a running operation prevented the request
 * @retval #MCUXCLELS_STATUS_OK_WAIT             on successful request
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_EccKeyExchangeInt_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_EccKeyExchangeInt_Async(
    mcuxClEls_KeyIndex_t privateKeyIdx,
    mcuxClEls_KeyIndex_t publicKeyIdx,
    mcuxClEls_KeyIndex_t sharedSecretIdx,
    mcuxClEls_KeyProp_t sharedSecretProperties
    );
#endif /* MCUXCL_FEATURE_ELS_PUK_INTERNAL */

/**
 * @brief Generates an ECDSA signature of a given message.
 *
 * The curve is NIST P-256.
 * The message hash, must be stored in the standard ANSI X9.62 format.
 * If the message is provided in plain, no prior conversion is necessary.
 * The signature will be stored in the standard ANSI X9.62 byte order (big-endian).
 * @if MCUXCL_FEATURE_ELS_SHA_DIRECT
 * No matter the value of @p options.echashchl, it must be ensured that SHA-Direct mode is disabled when calling this function (see #mcuxClEls_ShaDirect_Disable).
 * @endif
 * 
 * Before execution, ELS will wait until #mcuxClEls_HwState_t.drbgentlvl == #MCUXCLELS_STATUS_DRBGENTLVL_HIGH. This can lead to a delay if the DRBG is in a state with less security strength at the time of the call.
 * 
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 * @param[in]  options            The command options. For more information, see #mcuxClEls_EccSignOption_t.
 * @param[in]  keyIdx             The private key index.
 * @param[in]  pInputHash         The hash of the message to sign in X9.62 format.
 * @param[in]  pInputMessage      The message to sign.
 * @param[in]  inputMessageLength Size of @p pInputMessage in bytes.
 * @param[out] pOutput            Pointer to the memory area which receives the generated signature in X9.62 format. (64 bytes)
 *
 * <dl>
 *   <dt>Parameter properties</dt>
 *   <dd><dl>
 *     <dt>@p options.echashchl == #MCUXCLELS_ECC_HASHED</dt>
 *       <dd>@p pInputHash is used, and it must be aligned on a 4-byte boundary.
 *           @p pInputMessage is ignored.</dd>
 *     <dt>@p options.echashchl == #MCUXCLELS_ECC_NOT_HASHED</dt>
 *       <dd>@p pInputHash is ignored.
 *           @p pInputMessage and @p inputMessageLength are used.</dd>
 *     <dt>@p pOptput must be aligned on a 4-byte boundary.</dt>
 *   </dl></dd>
 * </dl>
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 * @retval #MCUXCLELS_STATUS_SW_INVALID_PARAM    if invalid parameters were specified
 * @retval #MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT if a running operation prevented the request
 * @retval #MCUXCLELS_STATUS_OK_WAIT             on successful request
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_EccSign_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_EccSign_Async(
    mcuxClEls_EccSignOption_t options,
    mcuxClEls_KeyIndex_t keyIdx,
    uint8_t const * pInputHash,
    uint8_t const * pInputMessage,
    size_t inputMessageLength,
    uint8_t * pOutput
    );

/**
 * @brief Verifies an ECDSA signature of a given message.
 *
 * The curve is NIST P-256.
 * The message hash, must be stored in the standard ANSI X9.62 format.
 * If the message is provided in plain, no prior conversion is necessary.
 * The signature and public key must be stored in the standard ANSI X9.62 byte order (big-endian).
 * @if MCUXCL_FEATURE_ELS_SHA_DIRECT
 * No matter the value of @p options.echashchl, it must be ensured that SHA-Direct mode is disabled when calling this function (see #mcuxClEls_ShaDirect_Disable).
 * @endif
 * 
 * Before execution, ELS will wait until #mcuxClEls_HwState_t.drbgentlvl == #MCUXCLELS_STATUS_DRBGENTLVL_LOW. This can lead to a delay if the DRBG is in a state with less security strength at the time of the call.
 * 
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 * @param[in]  options             The command options. For more information, see #mcuxClEls_EccVerifyOption_t.
 * @param[in]  pInputHash          The hash of the signed message in X9.62 format.
 * @param[in]  pInputMessage       The message to sign.
 * @param[in]  inputMessageLength  Size of @p pInputMessage in bytes.
 * @param[in]  pSignatureAndPubKey Pointer to the memory area which contains the concatenation of the signature and the public key.
 * @param[out] pOutput             Pointer to the memory area which will receive the recalculated value of the R component in case of a successful
 *                                 signature verification.
 *
 * <dl>
 *   <dt>Parameter properties</dt>
 *   <dd><dl>
 *     <dt>@p options.echashchl == #MCUXCLELS_ECC_HASHED</dt>
 *       <dd>@p pInputHash is used, and it must be aligned on a 4-byte boundary.
 *           @p pInputMessage is ignored.</dd>
 *     <dt>@p options.echashchl == #MCUXCLELS_ECC_NOT_HASHED</dt>
 *       <dd>@p pInputHash is ignored.
 *           @p pInputMessage and @p inputMessageLength are used.</dd>
 *     <dt>@p pSignatureAndPubKey </dt>
 *       <dd>It must be aligned on a 4-byte boundary.
 *           The signature to be verified consists of the 256-bit R component and the 256-bit S component.
 *           The public key is the one for verification. (Uncompressed, X and Y components)
 *           The signature and the public key are in X9.62 format.</dd>
 *     <dt>@p pOutput </dt>
 *       <dd>It must be aligned on a 4-byte boundary.
 *           The output shall be compared to the first 32 bytes stored at @p pSignatureAndPublicKey.</dd>
 *   </dl></dd>
 * </dl>
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 * @retval #MCUXCLELS_STATUS_SW_INVALID_PARAM    if invalid parameters were specified
 * @retval #MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT if a running operation prevented the request
 * @retval #MCUXCLELS_STATUS_OK_WAIT             on successful request
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_EccVerify_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_EccVerify_Async(
    mcuxClEls_EccVerifyOption_t options,
    uint8_t const * pInputHash,
    uint8_t const * pInputMessage,
    size_t inputMessageLength,
    uint8_t const * pSignatureAndPubKey,
    uint8_t * pOutput
    );

#ifdef MCUXCL_FEATURE_ELS_PUK_INTERNAL
/**
 * @brief Verifies an ECDSA signature of a given message.
 *
 * The curve is NIST P-256.
 * The message hash, must be stored in the standard ANSI X9.62 format.
 * If the message is provided in plain, no prior conversion is necessary.
 * The signature must be stored in the standard ANSI X9.62 byte order (big-endian).
 * @if MCUXCL_FEATURE_ELS_SHA_DIRECT
 * No matter the value of @p options.echashchl, it must be ensured that SHA-Direct mode is disabled when calling this function (see #mcuxClEls_ShaDirect_Disable).
 * @endif
 * 
 * Before execution, ELS will wait until #mcuxClEls_HwState_t.drbgentlvl == #MCUXCLELS_STATUS_DRBGENTLVL_LOW. This can lead to a delay if the DRBG is in a state with less security strength at the time of the call.
 * 
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 * @param[in]  options             The command options. For more information, see #mcuxClEls_EccVerifyOption_t.
 * @param[in]  publicKeyIdx        The public key index.
 * @param[in]  pInputHash          The hash of the signed message in X9.62 format.
 * @param[in]  pInputMessage       The message to sign.
 * @param[in]  inputMessageLength  Size of @p pInputMessage in bytes.
 * @param[in]  pSignature          Pointer to the memory area which contains the concatenation of the signature and the public key.
 * @param[out] pOutput             Pointer to the memory area which will receive the recalculated value of the R component in case of a successful
 *                                 signature verification.
 *
 * <dl>
 *   <dt>Parameter properties</dt>
 *   <dd><dl>
 *     <dt>@p options.echashchl == #MCUXCLELS_ECC_HASHED</dt>
 *       <dd>@p pInputHash is used, and it must be aligned on a 4-byte boundary.
 *           @p pInputMessage is ignored.</dd>
 *     <dt>@p options.echashchl == #MCUXCLELS_ECC_NOT_HASHED</dt>
 *       <dd>@p pInputHash is ignored.
 *           @p pInputMessage and @p inputMessageLength are used.</dd>
 *     <dt>@p pSignature</dt>
 *       <dd>It must be aligned on a 4-byte boundary.
 *           The signature to be verified consists of the 256-bit R component and the 256-bit S component.
 *           The signature is in X9.62 format.</dd>
 *     <dt>@p pOutput </dt>
 *       <dd>It must be aligned on a 4-byte boundary.
 *           The output shall be compared to the first 32 bytes stored at @p pSignatureAndPublicKey.</dd>
 *   </dl></dd>
 * </dl>
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 * @retval #MCUXCLELS_STATUS_SW_INVALID_PARAM    if invalid parameters were specified
 * @retval #MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT if a running operation prevented the request
 * @retval #MCUXCLELS_STATUS_OK_WAIT             on successful request
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_EccVerifyInt_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_EccVerifyInt_Async(
    mcuxClEls_EccVerifyOption_t options,
    mcuxClEls_KeyIndex_t publicKeyIdx,
    uint8_t const * pInputHash,
    uint8_t const * pInputMessage,
    size_t inputMessageLength,
    uint8_t const * pSignature,
    uint8_t * pOutput
    );

#endif /* MCUXCL_FEATURE_ELS_PUK_INTERNAL */

/**
 * @}
 */ /* mcuxClEls_Ecc_Functions */

/**
 * @}
 */ /* mcuxClEls_Ecc */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLELS_ECC_H_ */
