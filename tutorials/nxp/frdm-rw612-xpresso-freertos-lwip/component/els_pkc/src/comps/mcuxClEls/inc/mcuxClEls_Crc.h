/*--------------------------------------------------------------------------*/
/* Copyright 2021-2024 NXP                                                  */
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
 * @file  mcuxClEls_Crc.h
 * @brief ELS header for Command CRC functionality.
 *
 * This header exposes functions that support the usage of the Command CRC feature for ELS.
 */
/**
 * @defgroup mcuxClEls_Crc mcuxClEls_Crc
 * @brief This part of the @ref mcuxClEls driver defines the Command CRC functionality
 * @ingroup mcuxClEls
 * @{
 */
#ifndef MCUXCLELS_CRC_H_
#define MCUXCLELS_CRC_H_

#include <mcuxClConfig.h> // Exported features flags header
#include <mcuxClEls_Types.h> // Common types
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************
 * CONSTANTS
 **********************************************/
/**
 * @defgroup mcuxClEls_Crc_Macros mcuxClEls_Crc_Macros
 * @brief Defines all macros of @ref mcuxClEls_Crc
 * @ingroup mcuxClEls_Crc
 * @{
 */

/**
 * @defgroup MCUXCLELS_CMD_CRC_ MCUXCLELS_CMD_CRC_
 * @brief Constants for ELS Command CRC
 * @ingroup mcuxClEls_Crc_Macros
 * @{ */
#define MCUXCLELS_CMD_CRC_VALUE_RESET    ((uint32_t) 0x1u) ///< Reset the Command CRC to initial value
#define MCUXCLELS_CMD_CRC_VALUE_ENABLE   ((uint32_t) 0x2u) ///< Enable update of Command CRC value by executing commands
#define MCUXCLELS_CMD_CRC_VALUE_DISABLE  ((uint32_t) 0x0u) ///< Disable update of Command CRC value by executing commands

#define MCUXCLELS_CMD_CRC_RESET          ((uint32_t) 0x1u) ///< Reset the Command CRC to initial value
#define MCUXCLELS_CMD_CRC_ENABLE         ((uint32_t) 0x1u) ///< Enable update of Command CRC value by executing commands
#define MCUXCLELS_CMD_CRC_DISABLE        ((uint32_t) 0x0u) ///< Disable update of Command CRC value by executing commands

#define MCUXCLELS_CMD_CRC_POLYNOMIAL     ((uint32_t) 0x04C11DB7u) ///< CRC polynomial for the Command CRC
#define MCUXCLELS_CMD_CRC_INITIAL_VALUE  ((uint32_t) 0xA5A5A5A5u) ///< Initial value for the Command CRC
/** @} */

/**
 * @defgroup MCUXCLELS_CMD_CRC_REFERENCE_ MCUXCLELS_CMD_CRC_REFERENCE_
 * @brief Macros for reference ELS Command CRC
 * @ingroup mcuxClEls_Crc_Macros
 * @{
 */

/**
 * @brief Initializes a reference CRC variable with the command CRC initial value.
 *        The new variable has the given name.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_INIT(crc)  \
    uint32_t (crc) = MCUXCLELS_CMD_CRC_INITIAL_VALUE

/**
 * @brief Resets the given reference CRC variable to the command CRC initial value.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_RESET(crc)  \
    (crc) = MCUXCLELS_CMD_CRC_INITIAL_VALUE

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_Aead_Init_Async.
 */
#ifndef MCUXCL_FEATURE_ELS_NO_INTERNAL_STATE_FLAGS
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_AEAD_INIT(crc, options)                \
  ({                                                                             \
    (options).bits.acpmod = MCUXCLELS_AEAD_ACPMOD_INIT;                           \
    MCUX_CSSL_ANALYSIS_START_PATTERN_0U_1U_ARE_UNSIGNED()                         \
    (options).bits.lastinit = MCUXCLELS_AEAD_LASTINIT_TRUE;                       \
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_0U_1U_ARE_UNSIGNED()                          \
    (options).bits.acpsie = MCUXCLELS_AEAD_STATE_IN_DISABLE;                      \
    (options).bits.acpsoe = MCUXCLELS_AEAD_STATE_OUT_ENABLE;                      \
    mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_AUTH_CIPHER, (options).word.value, &(crc)); \
	(retVal);                                                                      \
  })
#else
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_AEAD_INIT(crc, options)                \
  ({                                                                             \
    (options).bits.acpmod = MCUXCLELS_AEAD_ACPMOD_INIT;                           \
    MCUX_CSSL_ANALYSIS_START_PATTERN_0U_1U_ARE_UNSIGNED()                         \
    (options).bits.lastinit = MCUXCLELS_AEAD_LASTINIT_TRUE;                       \
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_0U_1U_ARE_UNSIGNED()                          \
    (options).bits.acpsie = MCUXCLELS_AEAD_STATE_IN_DISABLE;                      \
    mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_AUTH_CIPHER, (options).word.value, &(crc)); \
    (retVal);                                                                    \
  })
#endif /* MCUXCL_FEATURE_ELS_NO_INTERNAL_STATE_FLAGS */

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_Aead_PartialInit_Async.
 */
#ifndef MCUXCL_FEATURE_ELS_NO_INTERNAL_STATE_FLAGS
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_AEAD_PARTIALINIT(crc, options)         \
  ({                                                                             \
    (options).bits.acpmod = MCUXCLELS_AEAD_ACPMOD_INIT;                           \
    (options).bits.acpsoe = MCUXCLELS_AEAD_STATE_OUT_ENABLE;                      \
    mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_AUTH_CIPHER, (options).word.value, &(crc)); \
    (retVal);                                                                    \
  })
#else
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_AEAD_PARTIALINIT(crc, options)         \
  ({                                                                             \
    (options).bits.acpmod = MCUXCLELS_AEAD_ACPMOD_INIT;                           \
    mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_AUTH_CIPHER, (options).word.value, &(crc)); \
    (retVal);                                                                    \
  })
#endif /* MCUXCL_FEATURE_ELS_NO_INTERNAL_STATE_FLAGS */

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_Aead_UpdateAad_Async.
 */
#ifndef MCUXCL_FEATURE_ELS_NO_INTERNAL_STATE_FLAGS
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_AEAD_UPDATEAAD(crc, options)           \
  ({                                                                             \
    (options).bits.acpmod = MCUXCLELS_AEAD_ACPMOD_AADPROC                         \
    (options).bits.acpsie = MCUXCLELS_AEAD_STATE_IN_ENABLE;                       \
    (options).bits.acpsoe = MCUXCLELS_AEAD_STATE_OUT_ENABLE;                      \
    mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_AUTH_CIPHER, (options).word.value, &(crc)); \
    (retVal);                                                                    \
  })
#else
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_AEAD_UPDATEAAD(crc, options)           \
  ({                                                                             \
    (options).bits.acpmod = MCUXCLELS_AEAD_ACPMOD_AADPROC;                        \
    mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_AUTH_CIPHER, (options).word.value, &(crc)); \
    (retVal);                                                                    \
  })
#endif /* MCUXCL_FEATURE_ELS_NO_INTERNAL_STATE_FLAGS */

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_Aead_UpdateData_Async.
 */
#ifndef MCUXCL_FEATURE_ELS_NO_INTERNAL_STATE_FLAGS
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_AEAD_UPDATEDATA(crc, options)          \
  ({                                                                             \
    (options).bits.acpmod = MCUXCLELS_AEAD_ACPMOD_MSGPROC                         \
    (options).bits.acpsie = MCUXCLELS_AEAD_STATE_IN_ENABLE;                       \
    (options).bits.acpsoe = MCUXCLELS_AEAD_STATE_OUT_ENABLE;                      \
    mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_AUTH_CIPHER, (options).word.value, &(crc)); \
    (retVal);                                                                    \
  })
#else
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_AEAD_UPDATEDATA(crc, options)          \
  ({                                                                             \
    (options).bits.acpmod = MCUXCLELS_AEAD_ACPMOD_MSGPROC;                        \
    mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_AUTH_CIPHER, (options).word.value, &(crc)); \
    (retVal);                                                                    \
  })
#endif /* MCUXCL_FEATURE_ELS_NO_INTERNAL_STATE_FLAGS */

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_Aead_Finalize_Async.
 */
#ifndef MCUXCL_FEATURE_ELS_NO_INTERNAL_STATE_FLAGS
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_AEAD_FINALIZE(crc, options)            \
  ({                                                                             \
    (options).bits.acpmod = MCUXCLELS_AEAD_ACPMOD_FINAL                           \
    (options).bits.acpsie = MCUXCLELS_AEAD_STATE_IN_ENABLE;                       \
    mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_AUTH_CIPHER, (options).word.value, &(crc)); \
    (retVal);                                                                    \
  })
#else
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_AEAD_FINALIZE(crc, options)            \
  ({                                                                             \
    (options).bits.acpmod = MCUXCLELS_AEAD_ACPMOD_FINAL;                          \
    mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_AUTH_CIPHER, (options).word.value, &(crc)); \
    (retVal);                                                                    \
  })
#endif /* MCUXCL_FEATURE_ELS_NO_INTERNAL_STATE_FLAGS */

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_Cipher_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_CIPHER(crc, options)                   \
    mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_CIPHER, (options).word.value, &(crc))

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_Cmac_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_CMAC(crc, options)                     \
    mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_CMAC, (options).word.value, &(crc))


/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_EccKeyGen_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_ECCKEYGEN(crc, options)                \
  ({                                                                             \
    (options).bits.revf = MCUXCLELS_ECC_REVERSEFETCH_ENABLE;                      \
    mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_KEYGEN, (options).word.value, &(crc)); \
    (retVal);                                                                    \
  })

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_EccKeyExchange_Async.
 */
#ifdef MCUXCL_FEATURE_ELS_PUK_INTERNAL_BIT
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_ECCKEYEXCHANGE(crc)                                 \
    ({                                                                                        \
        mcuxClEls_EccKeyExchOption_t options = {0u};                                           \
        options.bits.revf = MCUXCLELS_ECC_REVERSEFETCH_ENABLE;                                 \
        options.bits.extkey = MCUXCLELS_ECC_EXTKEY_EXTERNAL;                                   \
        mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_ECKXH, options.word.value, &(crc));     \
        (retVal);                                                                             \
    })
#else
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_ECCKEYEXCHANGE(crc)                                 \
    ({                                                                                        \
        mcuxClEls_EccKeyExchOption_t options = {0u};                                           \
        options.bits.revf = MCUXCLELS_ECC_REVERSEFETCH_ENABLE;                                 \
        mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_ECKXH, options.word.value, &(crc));     \
        (retVal);                                                                             \
    })
#endif /* MCUXCL_FEATURE_ELS_PUK_INTERNAL_BIT */

#ifdef MCUXCL_FEATURE_ELS_PUK_INTERNAL
/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_EccKeyExchangeInt_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_ECCKEYEXCHANGEINT(crc)                              \
    ({                                                                                        \
        mcuxClEls_EccKeyExchOption_t options = {0u};                                           \
        options.bits.revf = MCUXCLELS_ECC_REVERSEFETCH_ENABLE;                                 \
        options.bits.extkey = MCUXCLELS_ECC_EXTKEY_INTERNAL;                                   \
        mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_ECKXH, options.word.value, &(crc));     \
        (retVal);                                                                             \
    })
#endif /* MCUXCL_FEATURE_ELS_PUK_INTERNAL */

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_EccSign_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_ECCSIGN(crc, options)                             \
  ({                                                                                        \
    (options).bits.revf = MCUXCLELS_ECC_REVERSEFETCH_ENABLE;                                 \
    mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_ECSIGN, (options).word.value, &(crc)); \
    (retVal);                                                                               \
  })

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_EccVerify_Async.
 */
#ifdef MCUXCL_FEATURE_ELS_PUK_INTERNAL_BIT
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_ECCVERFIFY(crc, options)                          \
  ({                                                                                        \
    (options).bits.revf = MCUXCLELS_ECC_REVERSEFETCH_ENABLE;                                 \
    (options).bits.extkey = MCUXCLELS_ECC_EXTKEY_EXTERNAL;                                   \
    mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_ECVFY, (options).word.value, &(crc)); \
    (retVal);                                                                               \
  })
#else
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_ECCVERFIFY(crc, options)                          \
  ({                                                                                        \
    (options).bits.revf = MCUXCLELS_ECC_REVERSEFETCH_ENABLE;                                 \
    mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_ECVFY, (options).word.value, &(crc)); \
    (retVal);                                                                               \
  })
#endif /* MCUXCL_FEATURE_ELS_PUK_INTERNAL_BIT */

#ifdef MCUXCL_FEATURE_ELS_PUK_INTERNAL
/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_EccVerifyInt_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_ECCVERFIFYINT(crc, options)            \
  ({                                                                             \
    (options).bits.revf = MCUXCLELS_ECC_REVERSEFETCH_ENABLE;                      \
    (options).bits.extkey = MCUXCLELS_ECC_EXTKEY_INTERNAL;                        \
    mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_ECVFY, (options).word.value, &(crc)); \
    (retVal);                                                                    \
  })
#endif /* MCUXCL_FEATURE_ELS_PUK_INTERNAL */

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_GlitchDetector_LoadConfig_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_GLITCHDETECTOR_LOADCONFIG(crc)         \
    mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_GDET_CFG_LOAD, 0u, &(crc))

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_GlitchDetector_Trim_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_GLITCHDETECTOR_TRIM(crc)               \
    mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_GDET_TRIM, 0u, &(crc))

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_Hash_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_HASH(crc, options)                     \
    mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_HASH, (options).word.value, &(crc))

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_Hmac_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_HMAC(crc, options)                     \
    mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_HMAC, (options).word.value, &(crc))

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_Ckdf_Sp800108_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_CKDF_SP800108(crc)                                 \
    ({                                                                                       \
        mcuxClEls_CkdfOption_t options = {0u};                                                \
        options.bits.ckdf_algo = MCUXCLELS_CKDF_ALGO_SP800108;                                \
        mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_CKDF, options.word.value, &(crc));     \
        (retVal);                                                                            \
    })


/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_Hkdf_Rfc5869_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_HKDF_RFC5869(crc, options)                                               \
  ({                                                                                                               \
    (options).bits.hkdf_algo = MCUXCLELS_HKDF_ALGO_RFC5869;                                                         \
    mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_HKDF, (options).word.value, &(crc));    \
    (retVal);                                                                                                      \
  })

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_Hkdf_Sp80056c_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_HKDF_SP80056C(crc)                                     \
    ({                                                                                           \
        mcuxClEls_HkdfOption_t options = {0u};                                                    \
        options.bits.hkdf_algo = MCUXCLELS_HKDF_ALGO_SP80056C;                                    \
        mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_HKDF, options.word.value, &(crc));     \
        (retVal);                                                                                \
    })

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_TlsGenerateMasterKeyFromPreMasterKey_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_TLSGENERATEMASTERKEYFROMPREMASTERKEY(crc)              \
    ({                                                                                           \
        mcuxClEls_TlsOption_t options = {0u};                                                     \
        options.bits.mode = MCUXCLELS_TLS_INIT;                                                   \
        mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_TLS, options.word.value, &(crc));          \
        (retVal);                                                                                \
    })

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_TlsGenerateSessionKeysFromMasterKey_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_TLSGENERATESESSIONKEYSFROMMASTERKEY(crc)               \
    ({                                                                                           \
        mcuxClEls_TlsOption_t options = {0u};                                                     \
        options.bits.mode = MCUXCLELS_TLS_FINALIZE;                                               \
        mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_TLS, options.word.value, &(crc));          \
        (retVal);                                                                                \
    })

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_KeyDelete_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_KEYDELETE(crc)                         \
    mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_KDELETE, 0u, &(crc))



/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_KeyImport_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_KEYIMPORT(crc, options)                \
    mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_KEYIN, (options).word.value, &(crc))

#ifdef MCUXCL_FEATURE_ELS_PUK_INTERNAL
/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_KeyImportPuk_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_KEYIMPORTPUK(crc)                                  \
    ({                                                                                       \
        mcuxClEls_KeyImportOption_t options = {0u};                                           \
        options.bits.revf = MCUXCLELS_KEYIMPORT_REVERSEFETCH_ENABLE;                          \
        options.bits.kfmt = MCUXCLELS_KEYIMPORT_KFMT_PBK;                                     \
        mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_KEYIN, options.word.value, &(crc));    \
        (retVal);                                                                            \
    })
#endif /* MCUXCL_FEATURE_ELS_PUK_INTERNAL */

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_KeyExport_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_KEYEXPORT(crc)                                                              \
    ({                                                                                                                \
        mcuxClEls_Status_t retVal = mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_KEYOUT, 0u, &(crc));                 \
        mcuxClEls_KeyImportOption_t import_options = {0u};                                                             \
        import_options.bits.kfmt = MCUXCLELS_KEYIMPORT_KFMT_RFC3394;                                                   \
        retVal = MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_KEYDELETE(crc);                                                    \
        retVal = MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_KEYIMPORT(crc, import_options);                         \
        (retVal);                                                                                                     \
    })

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_Rng_DrbgRequest_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_RNG_DRBGREQUEST(crc)                  \
    mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_RND_REQ, 0u, &(crc))

#ifdef MCUXCL_FEATURE_ELS_RND_RAW
/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_Rng_DrbgRequestRaw_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_RNG_DRBGREQUESTRAW(crc)               \
    mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_RND_REQ, MCUXCLELS_RNG_RND_REQ_RND_RAW, &(crc))
#endif /* MCUXCL_FEATURE_ELS_RND_RAW */

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_Rng_DrbgTestInstantiate_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_RNG_DRBGTESTINSTANTIATE(crc)                        \
    mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_DRBG_TEST, MCUXCLELS_RNG_DRBG_TEST_MODE_INSTANTIATE, &(crc))

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_Rng_DrbgTestExtract_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_RNG_DRBGTESTEXTRACT(crc)                             \
    mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_DRBG_TEST, MCUXCLELS_RNG_DRBG_TEST_MODE_EXTRACT, &(crc))

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_Rng_DrbgTestAesEcb_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_RNG_DRBGTESTAESECB(crc)                             \
    mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_DRBG_TEST, MCUXCLELS_RNG_DRBG_TEST_MODE_AES_ECB, &(crc))

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_Rng_DrbgTestAesCtr_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_RNG_DRBGTESTAESCTR(crc)                             \
    mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_DRBG_TEST, MCUXCLELS_RNG_DRBG_TEST_MODE_AES_CTR, &(crc))

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_Rng_Dtrng_ConfigLoad_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_RNG_DTRNG_CONFIGLOAD(crc)             \
    mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_DTRNG_CFG_LOAD, 0u, &(crc))

/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_Rng_Dtrng_ConfigEvaluate_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_RNG_DTRNG_CONFIGEVALUATE(crc)         \
    mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_DTRNG_EVAL, 0u, &(crc))

#ifdef MCUXCL_FEATURE_ELS_PRND_INIT
/**
 * @brief Updates given reference command CRC with command @ref mcuxClEls_Prng_Init_Async.
 */
#define MCUXCLELS_CMD_CRC_REFERENCE_UPDATE_PRNG_INIT(crc)                               \
    mcuxClEls_UpdateRefCRC(MCUXCLELS_CMD_CRC_CMD_ID_RND_REQ, MCUXCLELS_RNG_RND_REQ_PRND_INIT, &(crc))
#endif /* MCUXCL_FEATURE_ELS_PRND_INIT */
/**
 * @}
 */

/**
 * @defgroup MCUXCLELS_CMD_CRC_CMD_ID_ MCUXCLELS_CMD_CRC_CMD_ID_
 * @brief Constants for ELS Command IDs
 * @ingroup mcuxClEls_Crc_Macros
 * @{
 */
#define MCUXCLELS_CMD_CRC_CMD_ID_CIPHER          0 ///< ELS Command ID for CIPHER command
#define MCUXCLELS_CMD_CRC_CMD_ID_AUTH_CIPHER     1 ///< ELS Command ID for AUTH_CIPHER command
#define MCUXCLELS_CMD_CRC_CMD_ID_CHAL_RESP_GEN   3 ///< ELS Command ID for CHAL_RESP_GEN command
#define MCUXCLELS_CMD_CRC_CMD_ID_ECSIGN          4 ///< ELS Command ID for ECSIGN command
#define MCUXCLELS_CMD_CRC_CMD_ID_ECVFY           5 ///< ELS Command ID for ECVFY command
#define MCUXCLELS_CMD_CRC_CMD_ID_ECKXH           6 ///< ELS Command ID for ECKXH command
#define MCUXCLELS_CMD_CRC_CMD_ID_KEYGEN          8 ///< ELS Command ID for KEYGEN command
#define MCUXCLELS_CMD_CRC_CMD_ID_KEYIN           9 ///< ELS Command ID for KEYIN command
#define MCUXCLELS_CMD_CRC_CMD_ID_KEYOUT         10 ///< ELS Command ID for KEYOUT command
#define MCUXCLELS_CMD_CRC_CMD_ID_KDELETE        11 ///< ELS Command ID for KDELETE command
#define MCUXCLELS_CMD_CRC_CMD_ID_KEYPROV        12 ///< ELS Command ID for KEYPROV command
#define MCUXCLELS_CMD_CRC_CMD_ID_CKDF           16 ///< ELS Command ID for CKDF command
#define MCUXCLELS_CMD_CRC_CMD_ID_HKDF           17 ///< ELS Command ID for HKDF command
#define MCUXCLELS_CMD_CRC_CMD_ID_TLS            18 ///< ELS Command ID for TLS command
#define MCUXCLELS_CMD_CRC_CMD_ID_HASH           20 ///< ELS Command ID for HASH command
#define MCUXCLELS_CMD_CRC_CMD_ID_HMAC           21 ///< ELS Command ID for HMAC command
#define MCUXCLELS_CMD_CRC_CMD_ID_CMAC           22 ///< ELS Command ID for CMAC command
#define MCUXCLELS_CMD_CRC_CMD_ID_RND_REQ        24 ///< ELS Command ID for RND_REQ command
#define MCUXCLELS_CMD_CRC_CMD_ID_DRBG_TEST      25 ///< ELS Command ID for DRBG_TEST command
#define MCUXCLELS_CMD_CRC_CMD_ID_DTRNG_CFG_LOAD 28 ///< ELS Command ID for DTRNG_CFG_LOAD command
#define MCUXCLELS_CMD_CRC_CMD_ID_DTRNG_EVAL     29 ///< ELS Command ID for DTRNG_EVAL command
#define MCUXCLELS_CMD_CRC_CMD_ID_GDET_CFG_LOAD  30 ///< ELS Command ID for GDET_CFG_LOAD command
#define MCUXCLELS_CMD_CRC_CMD_ID_GDET_TRIM      31 ///< ELS Command ID for GDET_TRIM command
/**
 * @}
 *
 * @}
 */

/**********************************************
 * TYPEDEFS
 **********************************************/
/**
 * @defgroup mcuxClEls_Crc_Types mcuxClEls_Crc_Types
 * @brief Defines all types of @ref mcuxClEls_Crc
 * @ingroup mcuxClEls_Crc
 * @{
 */

/**
 * @brief Type to control ELS Command CRC
 */
typedef union
{
    struct
    {
        uint32_t value;         ///< Accesses the bit field as a full word
    } word;
    struct
    {
        uint32_t reset :1;      ///< Reset the Command CRC to initial value, set by #MCUXCLELS_CMD_CRC_RESET
        uint32_t enable :1;     ///< Enable/Disable update of Command CRC value by executing commands, set with #MCUXCLELS_CMD_CRC_ENABLE / #MCUXCLELS_CMD_CRC_DISABLE
        uint32_t : 30;          ///< RFU
    } bits;                     ///< Access #mcuxClEls_CommandCrcConfig_t bit-wise
} mcuxClEls_CommandCrcConfig_t;

/**
 * @}
 */

/**********************************************
 * FUNCTIONS
 **********************************************/
/**
 * @defgroup mcuxClEls_Crc_Functions mcuxClEls_Crc_Functions
 * @brief Defines all functions of @ref mcuxClEls_Crc
 * @ingroup mcuxClEls_Crc
 * @{
 */

/**
 * @brief Set command CRC flags.
 *
 * @param[in] options    The command CRC options. For more information, see #mcuxClEls_CommandCrcConfig_t.
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code is always #MCUXCLELS_STATUS_OK
 * @else
 *  @return An error code that is always #MCUXCLELS_STATUS_OK
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_ConfigureCommandCRC)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_ConfigureCommandCRC(
    mcuxClEls_CommandCrcConfig_t options
    );

/**
 * @brief Get the current command CRC value.
 *
 * @param[out] commandCrc    The command CRC value.
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection).
 *  @retval MCUXCLELS_STATUS_OK                Operation successful
 *  @retval MCUXCLELS_STATUS_SW_INVALID_PARAM  Parameter commandCRC points to NULL
 * @else
 *  @return An error code
 *  @retval MCUXCLELS_STATUS_OK                Operation successful
 *  @retval MCUXCLELS_STATUS_SW_INVALID_PARAM  Parameter commandCRC points to NULL
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_GetCommandCRC)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GetCommandCRC(
    uint32_t* commandCrc
    );

/**
 * @brief Verifies a reference CRC against the computed ELS command CRC.
 *
 * @param[in] refCrc The reference CRC value.
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection). The error code can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @else
 *  @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_VerifyVsRefCRC)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_VerifyVsRefCRC(
    uint32_t refCrc
    );

/**
 * @brief Updates a reference CRC with the parameters of an ELS command.
 *        This can be used to verify against the ELS command CRC.
 *
 * @param[in]     command  The ELS command ID.
 * @param[in]     options  The command options for the given ELS command.
 * @param[in,out] refCrc   The current reference CRC value to update.
 *
 * @if (MCUXCL_FEATURE_CSSL_FP_USE_SECURE_COUNTER && MCUXCL_FEATURE_CSSL_SC_USE_SW_LOCAL)
 *  @return A code-flow protected error code (see @ref mcuxCsslFlowProtection).
 *  @retval MCUXCLELS_STATUS_OK                Operation successful
 *  @retval MCUXCLELS_STATUS_SW_INVALID_PARAM  Parameter crc points to NULL
 * @else
 *  @return An error code
 *  @retval MCUXCLELS_STATUS_OK                Operation successful
 *  @retval MCUXCLELS_STATUS_SW_INVALID_PARAM  Parameter crc points to NULL
 * @endif
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_UpdateRefCRC)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_UpdateRefCRC(
    uint8_t   command,
    uint32_t  options,
    uint32_t* refCrc
    );

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLELS_CRC_H_ */

/**
 * @}
 *
 * @}
 */
