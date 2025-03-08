/*--------------------------------------------------------------------------*/
/* Copyright 2020-2024 NXP                                                  */
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
 * @file  mcuxClEls_Aead.h
 * @brief ELS header for Authenticated Encryption with Associated Data (AEAD).
 * 
 * This header exposes functions that enable using the ELS for Authenticated Encryption with Associated Data (AEAD).
 * The AEAD algorithm supported by ELS is AES in Galois/Counter Mode (GCM), as described in NIST Special Publication
 * 800-38D.
 */

 /**
 * @defgroup mcuxClEls_Aead mcuxClEls_Aead
 * @brief This part of the @ref mcuxClEls driver supports Authenticated Encryption with Associated Data (AEAD).
 * @ingroup mcuxClEls
 * @{
 */
#ifndef MCUXCLELS_AEAD_H_
#define MCUXCLELS_AEAD_H_

#include <mcuxClConfig.h> // Exported features flags header
#include <mcuxClEls_Common.h> // Common functionality

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************
 * CONSTANTS
 **********************************************/
/**
 * @defgroup mcuxClEls_Aead_Macros mcuxClEls_Aead_Macros
 * @brief Defines all macros of @ref mcuxClEls_Aead
 * @ingroup mcuxClEls_Aead
 * @{
 */
/**
 * @defgroup MCUXCLELS_AEAD_ MCUXCLELS_AEAD_
 * @brief Defines macros used to initialize #mcuxClEls_AeadOption_t
 * @ingroup mcuxClEls_Aead_Macros
 * @{
 */
#define MCUXCLELS_AEAD_ENCRYPT ((uint8_t)0x00U) ///< Set #mcuxClEls_AeadOption_t.dcrpt to this value to encrypt data
#define MCUXCLELS_AEAD_DECRYPT ((uint8_t)0x01U) ///< Set #mcuxClEls_AeadOption_t.dcrpt to this value to decrypt data

#define MCUXCLELS_AEAD_STATE_IN_DISABLE ((uint8_t)0x00U) ///< Set #mcuxClEls_AeadOption_t.acpsie to this value to load the GCM state from ELS
#define MCUXCLELS_AEAD_STATE_IN_ENABLE  ((uint8_t)0x01U) ///< Set #mcuxClEls_AeadOption_t.acpsie to this value to load the GCM state from the context

#define MCUXCLELS_AEAD_LASTINIT_TRUE  (0x01U)  ///< Set #mcuxClEls_AeadOption_t.lastinit to this value if this is the last call to init
#define MCUXCLELS_AEAD_LASTINIT_FALSE (0x00U)  ///< Set #mcuxClEls_AeadOption_t.lastinit to this value if this is not the last call to init

#define MCUXCLELS_AEAD_EXTERN_KEY ((uint8_t)0x01U)  ///< Set #mcuxClEls_AeadOption_t.extkey to this value to use an external key
#define MCUXCLELS_AEAD_INTERN_KEY ((uint8_t)0x00U)  ///< Set #mcuxClEls_AeadOption_t.extkey to this value to use a key from the ELS keystore

#define MCUXCLELS_AEAD_ACPMOD_INIT    ((uint8_t)0x00U) ///< Set #mcuxClEls_AeadOption_t.acpmod to this value for Init mode. For internal use
#define MCUXCLELS_AEAD_ACPMOD_AADPROC ((uint8_t)0x01U) ///< Set #mcuxClEls_AeadOption_t.acpmod to this value for Process Additional Authenticated Data mode. For internal use
#define MCUXCLELS_AEAD_ACPMOD_MSGPROC ((uint8_t)0x02U) ///< Set #mcuxClEls_AeadOption_t.acpmod to this value for Process Message mode. For internal use
#define MCUXCLELS_AEAD_ACPMOD_FINAL   ((uint8_t)0x03U) ///< Set #mcuxClEls_AeadOption_t.acpmod to this value for Finalize mode. For internal use

#ifndef MCUXCL_FEATURE_ELS_NO_INTERNAL_STATE_FLAGS
#define MCUXCLELS_AEAD_STATE_OUT_ENABLE  ((uint8_t)0x01U) ///< Set #mcuxClEls_AeadOption_t.acpsoe to this value to save the state to the context. For internal use
#endif /* MCUXCL_FEATURE_ELS_NO_INTERNAL_STATE_FLAGS */
/**
 * @}
 */

#define MCUXCLELS_AEAD_IV_BLOCK_SIZE  16U  ///< AES-GCM IV Granularity:  128 bit (16 bytes)
#define MCUXCLELS_AEAD_AAD_BLOCK_SIZE 16U  ///< AES-GCM AAD Granularity: 128 bit (16 bytes)
#define MCUXCLELS_AEAD_TAG_SIZE       16U  ///< tag size: Tag generation supports only a 128 bit wide tag (16 bytes)
#define MCUXCLELS_AEAD_CONTEXT_SIZE   80U  ///< context size: 512 bit (64 bytes) + 16 bytes for finalize
/**
 * @}
 */

/**********************************************
 * TYPEDEFS
 **********************************************/
/**
 * @defgroup mcuxClEls_Aead_Types mcuxClEls_Aead_Types
 * @brief Defines all types of @ref mcuxClEls_Aead
 * @ingroup mcuxClEls_Aead
 * @{
 */
/**
 * @brief Command option bit field for #mcuxClEls_Aead_Init_Async, #mcuxClEls_Aead_UpdateAad_Async, #mcuxClEls_Aead_UpdateData_Async and #mcuxClEls_Aead_Finalize_Async.
 */
typedef union
{
    struct
    {
        uint32_t value; ///< Accesses the bit field as a full word
    } word;             ///< Access #mcuxClEls_AeadOption_t word-wise
    struct
    {
        uint32_t :1;          ///< RFU
        uint32_t dcrpt :1;    ///< Defines if encryption or decryption shall be performed
        uint32_t acpmod :2;   ///< This field is managed internally
#ifndef MCUXCL_FEATURE_ELS_NO_INTERNAL_STATE_FLAGS
        uint32_t acpsoe :1;   ///< This field is managed internally
#else
        uint32_t :1;          ///< RFU
#endif /* MCUXCL_FEATURE_ELS_NO_INTERNAL_STATE_FLAGS */
        uint32_t acpsie :1;   ///< This field is managed internally
        uint32_t msgendw :4;  ///< The size of the last data block (plain/cipher text) in bytes, without padding
        uint32_t lastinit :1; ///< Defines whether this is the last call to init
        uint32_t :2;          ///< RFU
        uint32_t extkey :1;   ///< Defines whether an external key shall be used
        uint32_t :18;         ///< RFU
    } bits;                   ///< Access #mcuxClEls_AeadOption_t bit-wise
} mcuxClEls_AeadOption_t;
/**
 * @}
 */

/**********************************************
 * FUNCTIONS
 **********************************************/
/**
 * @defgroup mcuxClEls_Aead_Functions mcuxClEls_Aead_Functions
 * @brief Defines all functions of @ref mcuxClEls_Aead
 * @ingroup mcuxClEls_Aead
 * @{
 */

/**
 * @brief AES-GCM initialization
 *
 * This is the first stage of AEAD encryption/decryption. This generates the initial context out of the IV @p pIV and the key (@p pKey or @p keyIdx).
 *
 * @if ELS_AES_WITH_SIDE_CHANNEL_PROTECTION
 * Before execution, ELS will wait until #mcuxClEls_HwState_t.drbgentlvl == #MCUXCLELS_STATUS_DRBGENTLVL_LOW. This can lead to a delay if the DRBG is in a state with less security strength at the time of the call.
 * @endif
 *
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 * @param[in]      options   The AEAD command options. For more information, see #mcuxClEls_AeadOption_t.
 * @param[in]      keyIdx    Index of the key inside the ELS keystore
 * @param[in]      pKey      Pointer to the key
 * @param[in]      keyLength Size of @p pKey in bytes
 * @param[in]      pIV       Pointer to memory area that contains the IV
 * @param[in]      ivLength  Size of @p pIV in bytes, with padding
 * @param    [out] pAeadCtx  Pointer to the memory area that receives the AEAD context structure. Must be at least #MCUXCLELS_AEAD_CONTEXT_SIZE bytes long.
 *
 * The properties of some parameters change with respect to selected options.
 *
 * <dl>
 *  <dt>Parameter properties</dt>
 *
 *  <dd><dl>
 *      <dt>@p options.extkey == #MCUXCLELS_AEAD_EXTERN_KEY</dt>
 *          <dd>@p keyIdx is ignored.
 *
 *          @p pKey must be a valid AES key and @p keyLength a valid AES key size (see @ref MCUXCLELS_CIPHER_KEY_SIZE_AES_).</dd>
 *
 *      <dt>@p options.extkey == #MCUXCLELS_AEAD_INTERN_KEY</dt>
 *          <dd>@p keyIdx must be a valid key index with the correct usage rights.
 *
 *          @p pKey and @p keyLength are ignored.</dd>
 *  </dl></dd>
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
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_Aead_Init_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_Aead_Init_Async(
    mcuxClEls_AeadOption_t options,
    mcuxClEls_KeyIndex_t keyIdx,
    uint8_t const * pKey,
    size_t keyLength,
    uint8_t const * pIV,
    size_t ivLength,
    uint8_t * pAeadCtx
    );

/**
 * @brief AES-GCM partial initialization
 * 
 * This is the first stage of AEAD encryption/decryption. This generates the initial context out of the IV @p pIV and the key (@p pKey or @p keyIdx).
 * 
 * @if ELS_AES_WITH_SIDE_CHANNEL_PROTECTION
 * Before execution, ELS will wait until #mcuxClEls_HwState_t.drbgentlvl == #MCUXCLELS_STATUS_DRBGENTLVL_LOW. This can lead to a delay if the DRBG is in a state with less security strength at the time of the call.
 * @endif
 * 
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 * @param[in]      options   The AEAD command options. For more information, see #mcuxClEls_AeadOption_t.
 * @param[in]      keyIdx    Index of the key inside the ELS keystore
 * @param[in]      pKey      Pointer to the key
 * @param[in]      keyLength Size of @p pKey in bytes
 * @param[in]      pIV       Pointer to memory area that contains the IV
 * @param[in]      ivLength  Size of @p pIV in bytes, with padding
 * @param    [out] pAeadCtx  Pointer to the memory area that receives the AEAD context structure. Must be at least #MCUXCLELS_AEAD_CONTEXT_SIZE bytes long.
 * 
 * The properties of some parameters change with respect to selected options.
 * 
 * <dl>
 *  <dt>Parameter properties</dt>
 *
 *  <dd><dl>
 *      <dt>@p options.extkey == #MCUXCLELS_AEAD_EXTERN_KEY</dt>
 *          <dd>@p keyIdx is ignored.
 *
 *          @p pKey must be a valid AES key and @p keyLength a valid AES key size (see @ref MCUXCLELS_CIPHER_KEY_SIZE_AES_).</dd>
 *
 *      <dt>@p options.extkey == #MCUXCLELS_AEAD_INTERN_KEY</dt>
 *          <dd>@p keyIdx must be a valid key index with the correct usage rights.
 *
 *          @p pKey and @p keyLength are ignored.</dd>
 *
 *      <dt>@p options.msgendw</dt>
 *          <dd>This field is ignored</dd>
 *  </dl></dd>
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
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_Aead_PartialInit_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_Aead_PartialInit_Async(
    mcuxClEls_AeadOption_t options,
    mcuxClEls_KeyIndex_t keyIdx,
    uint8_t const * pKey,
    size_t keyLength,
    uint8_t const * pIV,
    size_t ivLength,
    uint8_t * pAeadCtx
    );

/**
 * @brief AES-GCM update of the Additional Authenticated Data (AAD)
 * 
 * This is the second stage of AEAD encryption/decryption. This updates the internal authentication tag with the AAD.
 * 
 * #mcuxClEls_Aead_Init_Async must have been called before calling this function.
 * 
 * @if ELS_AES_WITH_SIDE_CHANNEL_PROTECTION
 * Before execution, ELS will wait until #mcuxClEls_HwState_t.drbgentlvl == #MCUXCLELS_STATUS_DRBGENTLVL_LOW. This can lead to a delay if the DRBG is in a state with less security strength at the time of the call.
 * @endif
 * 
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 * @param[in]      options   The AEAD command options. For more information, see #mcuxClEls_AeadOption_t.
 * @param[in]      keyIdx    Index of the key inside the ELS keystore
 * @param[in]      pKey      Pointer to the key
 * @param[in]      keyLength Size of @p pKey in bytes
 * @param[in]      pAad      Memory area that contains the AAD
 * @param[in]      aadLength Length of the @p pAad in bytes with padding
 * @param[in, out] pAeadCtx  Pointer to the AEAD context structure. Must be at least #MCUXCLELS_AEAD_CONTEXT_SIZE bytes long.
 * 
 * The properties of some parameters change with respect to selected options.
 * 
 * <dl>
 *  <dt>Parameter properties</dt>
 *
 *  <dd><dl>
 *      <dt>@p options.extkey == #MCUXCLELS_AEAD_EXTERN_KEY</dt>
 *          <dd>@p keyIdx is ignored.
 *
 *          @p pKey must be a valid AES key and @p keyLength a valid AES key size (see @ref MCUXCLELS_CIPHER_KEY_SIZE_AES_).</dd>
 *
 *      <dt>@p options.extkey == #MCUXCLELS_AEAD_INTERN_KEY</dt>
 *          <dd>@p keyIdx must be a valid key index with the correct usage rights.
 *
 *          @p pKey and @p keyLength are ignored.</dd>
 *
 *      <dt>@p options.msgendw</dt>
 *          <dd>This field is ignored</dd>
 *  </dl></dd>
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
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_Aead_UpdateAad_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_Aead_UpdateAad_Async(
    mcuxClEls_AeadOption_t options,
    mcuxClEls_KeyIndex_t keyIdx,
    uint8_t const * pKey,
    size_t keyLength,
    uint8_t const * pAad,
    size_t aadLength,
    uint8_t * pAeadCtx
    );

/**
 * @brief AES-GCM update of the encrypted data
 * 
 * This is the third stage of AEAD encryption/decryption. This processes the given plaintext (in case of encryption)
 * or ciphertext (in case of decryption) and outputs the ciphertext (in case of encryption) or plaintext (in case of decryption).
 * 
 * #mcuxClEls_Aead_Init_Async, #mcuxClEls_Aead_UpdateAad_Async must have been called before calling this function.
 *
 * @if ELS_AES_WITH_SIDE_CHANNEL_PROTECTION
 * Before execution, ELS will wait until #mcuxClEls_HwState_t.drbgentlvl == #MCUXCLELS_STATUS_DRBGENTLVL_LOW. This can lead to a delay if the DRBG is in a state with less security strength at the time of the call.
 * @endif
 * 
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 * @param[in]      options     The AEAD command options. For more information, see #mcuxClEls_AeadOption_t.
 * @param[in]      keyIdx      Index of the key inside the ELS keystore
 * @param[in]      pKey        Pointer to the key
 * @param[in]      keyLength   Size of @p pKey in bytes
 * @param[in]      pInput      Pointer to the memory location of the data to be processed
 * @param[in]      inputLength Size of @p pInput in bytes with padding
 * @param    [out] pOutput     Pointer to the processed data memory location
 * @param[in, out] pAeadCtx    Pointer to the AEAD context structure. Must be at least #MCUXCLELS_AEAD_CONTEXT_SIZE bytes long.
 * 
 * The properties of some parameters change with respect to selected options.
 * 
 * <dl>
 *  <dt>Parameter properties</dt>
 *
 *  <dd><dl>
 *      <dt>@p options.extkey == #MCUXCLELS_AEAD_EXTERN_KEY</dt>
 *          <dd>@p keyIdx is ignored.
 *
 *          @p pKey must be a valid AES key and @p keyLength a valid AES key size (see @ref MCUXCLELS_CIPHER_KEY_SIZE_AES_).</dd>
 *
 *      <dt>@p options.extkey == #MCUXCLELS_AEAD_INTERN_KEY</dt>
 *          <dd>@p keyIdx must be a valid key index with the correct usage rights.
 *
 *          @p pKey and @p keyLength are ignored.</dd>
 *
 *      <dt>@p options.msgendw</dt>
 *          <dd>This field has to be set to the size of the last data block (plain/cipher text) in bytes, without padding.
 *              In case the last block is a full block, this field has to be set to 0.</dd>
 *  </dl></dd>
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
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_Aead_UpdateData_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_Aead_UpdateData_Async(
    mcuxClEls_AeadOption_t options,
    mcuxClEls_KeyIndex_t keyIdx,
    uint8_t const * pKey,
    size_t keyLength,
    uint8_t const * pInput,
    size_t inputLength,
    uint8_t * pOutput,
    uint8_t * pAeadCtx
    );

/**
 * @brief AES-GCM final encryption/decryption
 * 
 * This is the fourth stage of AEAD encryption/decryption. This updates the authentication tag with the final data
 * length block and outputs the tag at the desired location.
 * 
 * #mcuxClEls_Aead_Init_Async, #mcuxClEls_Aead_UpdateAad_Async and #mcuxClEls_Aead_UpdateData_Async must have been called
 * before calling this function.
 *
 * @if ELS_AES_WITH_SIDE_CHANNEL_PROTECTION
 * Before execution, ELS will wait until #mcuxClEls_HwState_t.drbgentlvl == #MCUXCLELS_STATUS_DRBGENTLVL_LOW. This can lead to a delay if the DRBG is in a state with less security strength at the time of the call.
 * @endif
 * 
 * Call #mcuxClEls_WaitForOperation to complete the operation.
 *
 * @param[in]      options    The AEAD command options. For more information, see #mcuxClEls_AeadOption_t.
 * @param[in]      keyIdx     Index of the key inside the ELS keystore
 * @param[in]      pKey       Pointer to the key
 * @param[in]      keyLength  Size of @p pKey in bytes
 * @param[in]      aadLength  Length of the complete Additional Authenticated Data (AAD) in bytes, without padding.
 * @param[in]      dataLength Length of the complete plaintext/ciphertext in bytes, without padding.
 * @param    [out] pTag       Pointer where the resulting tag will be stored
 * @param[in]      pAeadCtx   Pointer to the AEAD context structure. Must be at least #MCUXCLELS_AEAD_CONTEXT_SIZE bytes long.
 * 
 * The properties of some parameters change with respect to selected options.
 * 
 * <dl>
 *  <dt>Parameter properties</dt>
 *
 *  <dd><dl>
 *      <dt>@p options.extkey == #MCUXCLELS_AEAD_EXTERN_KEY</dt>
 *          <dd>@p keyIdx is ignored.
 *
 *          @p pKey must be a valid AES key and @p keyLength a valid AES key size (see @ref MCUXCLELS_CIPHER_KEY_SIZE_AES_).</dd>
 *
 *      <dt>@p options.extkey == #MCUXCLELS_AEAD_INTERN_KEY</dt>
 *          <dd>@p keyIdx must be a valid key index with the correct usage rights.
 *
 *          @p pKey and @p keyLength are ignored.</dd>
 *
 *      <dt>@p options.msgendw</dt>
 *          <dd>This field is ignored</dd>
 *  </dl></dd>
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
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_Aead_Finalize_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_Aead_Finalize_Async(
    mcuxClEls_AeadOption_t options,
    mcuxClEls_KeyIndex_t keyIdx,
    uint8_t const * pKey,
    size_t keyLength,
    size_t aadLength,
    size_t dataLength,
    uint8_t * pTag,
    uint8_t * pAeadCtx
    );

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLELS_AEAD_H_ */

/**
 * @}
 * 
 * @}
 */
