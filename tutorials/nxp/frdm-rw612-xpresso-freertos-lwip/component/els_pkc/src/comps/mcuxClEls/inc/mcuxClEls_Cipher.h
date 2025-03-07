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
 * @file  mcuxClEls_Cipher.h
 * @brief ELS header for symmetric ciphers.
 *
 * This header exposes functions that enable using the ELS for symmetric encryption/decryption.
 * The cipher algorithm supported by ELS is AES in the following modes:
 * - Electronic Code Book (ECB) mode, 
 * - Cipher Block Chaining (CBC) mode, and 
 * - Counter (CTR) mode.
 * Supported key sizes are 128, 192, and 256 bits.
 */

/**
 * @defgroup mcuxClEls_Cipher mcuxClEls_Cipher
 * @brief This part of the @ref mcuxClEls driver supports functionality for symmetric ciphers
 * @ingroup mcuxClEls
 * @{
 */

#ifndef MCUXCLELS_CIPHER_H_
#define MCUXCLELS_CIPHER_H_

#include <mcuxClConfig.h> // Exported features flags header
#include <mcuxClEls_Common.h> // Common functionality

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************
 * CONSTANTS
 **********************************************/
/**
 * @defgroup mcuxClEls_Cipher_Macros mcuxClEls_Cipher_Macros
 * @brief Defines all macros of @ref mcuxClEls_Cipher
 * @ingroup mcuxClEls_Cipher
 * @{
 */

/**
 * @defgroup MCUXCLELS_CIPHER_ MCUXCLELS_CIPHER_
 * @brief Defines valid options to be used by #mcuxClEls_CipherOption_t
 * @ingroup mcuxClEls_Cipher_Macros
 *
 * Valid AES key sizes in bytes
 * @{
 */

#define MCUXCLELS_CIPHER_ENCRYPT 0U                      ///< Set this option at #mcuxClEls_CipherOption_t.dcrpt to perform an encryption
#define MCUXCLELS_CIPHER_DECRYPT 1U                      ///< Set this option at #mcuxClEls_CipherOption_t.dcrpt to perform a decryption

#define MCUXCLELS_CIPHER_STATE_OUT_ENABLE  1U            ///< Set this option at #mcuxClEls_CipherOption_t.cphsoe to export the internal ELS state to @p pIV
#define MCUXCLELS_CIPHER_STATE_OUT_DISABLE 0U            ///< Set this option at #mcuxClEls_CipherOption_t.cphsoe to not export the internal ELS state

#ifndef MCUXCL_FEATURE_ELS_NO_INTERNAL_STATE_FLAGS
#define MCUXCLELS_CIPHER_STATE_IN_ENABLE  1U             ///< Set this option at #mcuxClEls_CipherOption_t.cphsie to import an external ELS state from @p pIV
#define MCUXCLELS_CIPHER_STATE_IN_DISABLE 0U             ///< Set this option at #mcuxClEls_CipherOption_t.cphsie to not import an external ELS state
#endif /* MCUXCL_FEATURE_ELS_NO_INTERNAL_STATE_FLAGS */

#define MCUXCLELS_CIPHER_EXTERNAL_KEY 1U                 ///< Set this option at #mcuxClEls_CipherOption_t.extkey to use a key located in CPU memory provided by @p pKey
#define MCUXCLELS_CIPHER_INTERNAL_KEY 0U                 ///< Set this option at #mcuxClEls_CipherOption_t.extkey to use a key located in ELS keystore privded by @p keyIdx

#define MCUXCLELS_CIPHERPARAM_ALGORITHM_AES_ECB 0x00U    ///< Set this option at #mcuxClEls_CipherOption_t.cphmde to use AES engine in Electornic Code Book (ECB) mode
#define MCUXCLELS_CIPHERPARAM_ALGORITHM_AES_CBC 0x01U    ///< Set this option at #mcuxClEls_CipherOption_t.cphmde to use AES engine in Cipher Block Chaining (CBC) mode
#define MCUXCLELS_CIPHERPARAM_ALGORITHM_AES_CTR 0x02U    ///< Set this option at #mcuxClEls_CipherOption_t.cphmde to use AES engine in Counter (CTR) mode
/**
 * @}
 */


/**
 * @ingroup mcuxClEls_Cipher_Macros
 */
#define MCUXCLELS_CIPHER_BLOCK_SIZE_AES   ((size_t) 16U) ///< Size of an AES input block: 128 bit (16 bytes)

/**
 * @defgroup MCUXCLELS_CIPHER_KEY_SIZE_AES_ MCUXCLELS_CIPHER_KEY_SIZE_AES_
 * @brief Defines valid AES key sizes in bytes
 * @ingroup mcuxClEls_Cipher_Macros
 * @{
 */

#define MCUXCLELS_CIPHER_KEY_SIZE_AES_128 ((size_t) 16U) ///< Size of an AES128 key: 128 bit (16 bytes)
#define MCUXCLELS_CIPHER_KEY_SIZE_AES_192 ((size_t) 24U) ///< Size of an AES192 key: 192 bit (24 bytes)
#define MCUXCLELS_CIPHER_KEY_SIZE_AES_256 ((size_t) 32U) ///< Size of an AES192 key: 256 bit (32 bytes)
/**
 * @}
 *
 * @}
 */

/**********************************************
 * TYPEDEFS
 **********************************************/
/**
 * @defgroup mcuxClEls_Cipher_Types mcuxClEls_Cipher_Types
 * @brief Defines all types of @ref mcuxClEls_Cipher
 * @ingroup mcuxClEls_Cipher
 * @{
 */

/**
 * @brief Command option bit field for #mcuxClEls_Cipher_Async
 *
 * Bit field to configure #mcuxClEls_Cipher_Async. See @ref MCUXCLELS_CIPHER_ for possible options.
 */
typedef union
{
    struct
    {
        uint32_t value;     ///< Accesses the bit field as a full word
    } word;                 ///< Access #mcuxClEls_CipherOption_t word-wise
    struct
    {
        uint32_t :1;        ///< RFU
        uint32_t dcrpt :1;  ///< Define operation mode
        uint32_t cphmde :2; ///< Define cipher mode
        uint32_t cphsoe :1; ///< Define whether the ELS internal cipher state should be extracted to external memory or kept internally
#ifndef MCUXCL_FEATURE_ELS_NO_INTERNAL_STATE_FLAGS
        uint32_t cphsie :1; ///< Define whether an external provided cipher state should be imported from external memory
        uint32_t :7;        ///< RFU
#else
        uint32_t :8;        ///< RFU
#endif /* MCUXCL_FEATURE_ELS_NO_INTERNAL_STATE_FLAGS */
        uint32_t extkey :1; ///< Define whether an external key from memory or ELS internal key should be used
        uint32_t :18;       ///< RFU
    } bits;                 ///< Access #mcuxClEls_CipherOption_t bit-wise
} mcuxClEls_CipherOption_t;
/**
 * @}
 */

/**********************************************
 * FUNCTIONS
 **********************************************/
/**
 * @defgroup mcuxClEls_Cipher_Functions mcuxClEls_Cipher_Functions
 * @brief Defines all functions of @ref mcuxClEls_Cipher
 * @ingroup mcuxClEls_Cipher
 * @{
 */

 /**
 * @brief Performs AES encryption/decryption.
 * 
 * @if ELS_AES_WITH_SIDE_CHANNEL_PROTECTION
 * Before execution, ELS will wait until #mcuxClEls_HwState_t.drbgentlvl == #MCUXCLELS_STATUS_DRBGENTLVL_LOW. This can lead to a delay if the DRBG is in a state with less security strength at the time of the call.
 * @endif
 *
 * Performs an AES encryption/decryption. Call #mcuxClEls_WaitForOperation to complete the operation.
 * @param[in]       options     Encryption/decryption command options. For detailed information, see #mcuxClEls_CipherOption_t.
 * @param[in]       keyIdx      Index of the key inside the ELS keystore. See parameter properties section in function description.
 * @param[in]       pKey        Memory area that contains the key. See parameter properties section in function description.
 * @param[in]       keyLength   Size of @p pKey in bytes. Must be a valid key size of @ref MCUXCLELS_CIPHER_KEY_SIZE_AES_. See parameter properties section in function description.
 * @param[in]       pInput      Pointer to the input data to be encrypted/decrypted. Padding must be already applied.
 * @param[in]       inputLength Size of @p pInput in bytes, must be a multiple of the block size.
 * @param[in, out]  pIV         A pointer to the memory location which contains/receives the IV/state of cipher. See parameter properties section in function description.
 * @param[out]      pOutput     Pointer to the output buffer to store encrypted/decrypted data.
 *
 * The properties of some parameters change with respect to selected options.
 *
 * <dl>
 *  <dt>Parameter properties</dt>
 *
 *  <dd><dl>
 *      <dt>@p options.cphmde == #MCUXCLELS_CIPHERPARAM_ALGORITHM_AES_ECB</dt>
 *          <dd>@p pIV is ignored.
 *
 *      <dt>@p options.cphmde == #MCUXCLELS_CIPHERPARAM_ALGORITHM_AES_CBC</dt>
 *          <dd>@p pIV must be set to the IV (when encrypting the first block) or to the last block of the ciphertext of the previous operation.
 *          ELS will always read and write to this location.
 *
 * @ifnot ELS_NO_INTERNAL_STATE_FLAGS
 *          @p options.cphsie is ignored.
 * @endif
 *
 *          @p options.cphsoe is ignored.</dd>
 *
 *      <dt>@p options.cphmde == #MCUXCLELS_CIPHERPARAM_ALGORITHM_AES_CTR</dt>
 *          <dd>@p pIV must be set to the IV (when encrypting the first block) or to the state output of the previous
 *          encryption/decryption operation. ELS will write to this location if @p options.cphsoe == #MCUXCLELS_CIPHER_STATE_OUT_ENABLE.</dd>
 *
 *      <dt>@p options.extkey == #MCUXCLELS_CIPHER_EXTERNAL_KEY</dt>
 *          <dd>@p keyIdx is ignored.</dd>
 *
 *      <dt>@p options.extkey == #MCUXCLELS_CIPHER_INTERNAL_KEY</dt>
 *          <dd>@p pKey is ignored.
 *
 *          @p keyLength is ignored.</dd>
 *
 *  </dl></dd>
 * </dl>
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 * @retval #MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT if a running operation prevented the request
 * @retval #MCUXCLELS_STATUS_OK_WAIT             on successful request
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_Cipher_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_Cipher_Async(
    mcuxClEls_CipherOption_t options,
    mcuxClEls_KeyIndex_t keyIdx,
    uint8_t const * pKey,
    size_t keyLength,
    uint8_t const * pInput,
    size_t inputLength,
    uint8_t * pIV,
    uint8_t * pOutput
    );

/**
 * @}
 */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLELS_CIPHER_H_ */

/**
 * @}
 */
