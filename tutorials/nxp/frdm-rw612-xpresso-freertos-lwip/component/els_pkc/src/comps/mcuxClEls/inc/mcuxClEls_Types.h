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
 * @file  mcuxClEls_Types.h
 * @brief ELS type header.
 *
 * This header defines types that are used by other mcuxClEls headers.
 */
/**
 * @defgroup mcuxClEls_Types mcuxClEls_Types
 * @brief This part of the @ref mcuxClEls driver defines common types
 * @ingroup mcuxClEls
 * @{
 */
#ifndef MCUXCLELS_TYPES_H_
#define MCUXCLELS_TYPES_H_

#include <stdint.h>
#include <stddef.h>
#include <mcuxClConfig.h> // Exported features flags header
#include <mcuxCsslFlowProtection.h>

#include <mcuxClEls_mapping.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************
 * MACROS
 **********************************************/
/**
 * @defgroup mcuxClEls_Types_Macros mcuxClEls_Types_Macros
 * @brief Defines all macros of @ref mcuxClEls_Types
 * @ingroup mcuxClEls_Types
 * @{
 */

#define MCUXCLELS_KEY_SLOTS (20U) ///< Number of key slots in the ELS key store.

/** @defgroup MCUXCLELS_KEYPROPERTY_VALUE_ MCUXCLELS_KEYPROPERTY_VALUE_
 * @brief Constants for initalizing #mcuxClEls_KeyProp_t.word
 * @ingroup mcuxClEls_Types_Macros
 * @{
 */
#define MCUXCLELS_KEYPROPERTY_VALUE_KEY_SIZE_128         ((uint32_t) 0u<< 0u) ///< 128-bit key
#define MCUXCLELS_KEYPROPERTY_VALUE_KEY_SIZE_256         ((uint32_t) 1u<< 0u) ///< 256-bit key
#ifdef MCUXCL_FEATURE_ELS_PUK_INTERNAL
#define MCUXCLELS_KEYPROPERTY_VALUE_KEY_SIZE_512         ((uint32_t) 3u<< 0u) ///< 512-bit key
#endif /* MCUXCL_FEATURE_ELS_PUK_INTERNAL */
#define MCUXCLELS_KEYPROPERTY_VALUE_ACTIVE               ((uint32_t) 1u<< 5u) ///< Key is active (loaded)
#define MCUXCLELS_KEYPROPERTY_VALUE_BASE_SLOT            ((uint32_t) 1u<< 6u) ///< First part of multi-slot key
#define MCUXCLELS_KEYPROPERTY_VALUE_GENERAL_PURPOSE_SLOT ((uint32_t) 1u<< 7u) ///< General purpose key slot
#define MCUXCLELS_KEYPROPERTY_VALUE_RETENTION_SLOT       ((uint32_t) 1u<< 8u) ///< Retention key slot
#define MCUXCLELS_KEYPROPERTY_VALUE_HW_OUT_SLOT          ((uint32_t) 1u<< 9u) ///< Hardware output key slot
#ifdef MCUXCL_FEATURE_ELS_PUK_INTERNAL
#define MCUXCLELS_KEYPROPERTY_VALUE_PUK                  ((uint32_t) 1u<<11u) ///< Trusted Public Key
#define MCUXCLELS_KEYPROPERTY_VALUE_TECDH                ((uint32_t) 1u<<12u) ///< Private key that can only be used in ECDH with Trusted Public Key
#endif /* MCUXCL_FEATURE_ELS_PUK_INTERNAL */
#define MCUXCLELS_KEYPROPERTY_VALUE_CMAC                 ((uint32_t) 1u<<13u) ///< CMAC key
#define MCUXCLELS_KEYPROPERTY_VALUE_KSK                  ((uint32_t) 1u<<14u) ///< Key signing key
#define MCUXCLELS_KEYPROPERTY_VALUE_RTF                  ((uint32_t) 1u<<15u) ///< RTF signing key
#define MCUXCLELS_KEYPROPERTY_VALUE_CKDF                 ((uint32_t) 1u<<16u) ///< CKDF signing key
#define MCUXCLELS_KEYPROPERTY_VALUE_HKDF                 ((uint32_t) 1u<<17u) ///< HKDF signing key
#define MCUXCLELS_KEYPROPERTY_VALUE_ECSGN                ((uint32_t) 1u<<18u) ///< ECC signing key
#define MCUXCLELS_KEYPROPERTY_VALUE_ECDH                 ((uint32_t) 1u<<19u) ///< ECC Diffie Hellman private key
#define MCUXCLELS_KEYPROPERTY_VALUE_AES                  ((uint32_t) 1u<<20u) ///< AES key
#define MCUXCLELS_KEYPROPERTY_VALUE_HMAC                 ((uint32_t) 1u<<21u) ///< HMAC key
#define MCUXCLELS_KEYPROPERTY_VALUE_KWK                  ((uint32_t) 1u<<22u) ///< Key Wrapping Key
#define MCUXCLELS_KEYPROPERTY_VALUE_KUOK                 ((uint32_t) 1u<<23u) ///< Key Unwrapping Only Key
#define MCUXCLELS_KEYPROPERTY_VALUE_TLS_PREMASTER_SECRET ((uint32_t) 1u<<24u) ///< TLS Premaster Secret
#define MCUXCLELS_KEYPROPERTY_VALUE_TLS_MASTER_SECRET    ((uint32_t) 1u<<25u) ///< TLS Master Secret
#define MCUXCLELS_KEYPROPERTY_VALUE_KGSRC                ((uint32_t) 1u<<26u) ///< Can provide key material input for ECC key generation
#define MCUXCLELS_KEYPROPERTY_VALUE_HW_OUT               ((uint32_t) 1u<<27u) ///< A key to be used in a hardware out key slot
#define MCUXCLELS_KEYPROPERTY_VALUE_WRPOK                ((uint32_t) 1u<<28u) ///< The key can be wrapped
#define MCUXCLELS_KEYPROPERTY_VALUE_DUK                  ((uint32_t) 1u<<29u) ///< Device Unique Key
#define MCUXCLELS_KEYPROPERTY_VALUE_PRIVILEGED           ((uint32_t) 1u<<30u) ///< Caller must be in privileged mode to use the key
#define MCUXCLELS_KEYPROPERTY_VALUE_NOTPRIVILEGED        ((uint32_t) 0u<<30u) ///< Caller does not have to be in privileged mode to use the key
#define MCUXCLELS_KEYPROPERTY_VALUE_SECURE               ((uint32_t) 0u<<31u) ///< Caller must be in secure mode to use the key
#define MCUXCLELS_KEYPROPERTY_VALUE_NOTSECURE            ((uint32_t) 1u<<31u) ///< Caller does not have to be in secure mode to use the key
/**
 * @}
 */

/** @defgroup MCUXCLELS_KEYPROPERTY_ MCUXCLELS_KEYPROPERTY_
 * @brief Constants for initalizing #mcuxClEls_KeyProp_t.bits
 * @ingroup mcuxClEls_Types_Macros
 * @{
 */
#define MCUXCLELS_KEYPROPERTY_KEY_SIZE_128               (0U) ///< This value of #mcuxClEls_KeyProp_t.ksize indicates a 128 bit key
#define MCUXCLELS_KEYPROPERTY_KEY_SIZE_256               (1U) ///< This value of #mcuxClEls_KeyProp_t.ksize indicates a 256 bit key
#ifdef MCUXCL_FEATURE_ELS_PUK_INTERNAL
#define MCUXCLELS_KEYPROPERTY_KEY_SIZE_512               (3U) ///< This value of #mcuxClEls_KeyProp_t.ksize indicates a 512 bit key
#endif /* MCUXCL_FEATURE_ELS_PUK_INTERNAL */
#define MCUXCLELS_KEYPROPERTY_ACTIVE_TRUE                (1U) ///< This value of #mcuxClEls_KeyProp_t.kactv indicates that the slot contains an active key
#define MCUXCLELS_KEYPROPERTY_ACTIVE_FALSE               (0U) ///< This value of #mcuxClEls_KeyProp_t.kactv indicates that the slot does not contain active key
#define MCUXCLELS_KEYPROPERTY_BASE_SLOT                  (1U) ///< This value of #mcuxClEls_KeyProp_t.kbase indicates that the slot is the base slot of a 2-slot key
#define MCUXCLELS_KEYPROPERTY_SECOND_SLOT                (0U) ///< This value of #mcuxClEls_KeyProp_t.kbase indicates that the slot is the second slot of a 2-slot key
#define MCUXCLELS_KEYPROPERTY_GENERAL_PURPOSE_SLOT_TRUE  (1U) ///< This value of #mcuxClEls_KeyProp_t.fgp indicates that the slot is a retention key slot or a hardware out key slot
#define MCUXCLELS_KEYPROPERTY_GENERAL_PURPOSE_SLOT_FALSE (0U) ///< This value of #mcuxClEls_KeyProp_t.fgp indicates that the slot is a neither retention key slot nor hardware out key slot
#define MCUXCLELS_KEYPROPERTY_RETENTION_SLOT_TRUE        (1U) ///< This value of #mcuxClEls_KeyProp_t.frtn indicates that the slot is a retention key slot
#define MCUXCLELS_KEYPROPERTY_RETENTION_SLOT_FALSE       (0U) ///< This value of #mcuxClEls_KeyProp_t.frtn indicates that the slot is not a retention key slot
#define MCUXCLELS_KEYPROPERTY_HW_OUT_SLOT_TRUE           (1U) ///< This value of #mcuxClEls_KeyProp_t.fhwo indicates that the slot is a hardware out key slot
#define MCUXCLELS_KEYPROPERTY_HW_OUT_SLOT_FALSE          (0U) ///< This value of #mcuxClEls_KeyProp_t.fhwo indicates that the slot is not a hardware out key slot
#ifdef MCUXCL_FEATURE_ELS_PUK_INTERNAL
#define MCUXCLELS_KEYPROPERTY_PUK_TRUE                   (1U) ///< This value of #mcuxClEls_KeyProp_t.upuk indicates that the slot is a Trusted Public Key
#define MCUXCLELS_KEYPROPERTY_PUK_FALSE                  (0U) ///< This value of #mcuxClEls_KeyProp_t.upuk indicates that the slot is not a Trusted Public Key
#define MCUXCLELS_KEYPROPERTY_TECDH_TRUE                 (1U) ///< This value of #mcuxClEls_KeyProp_t.utecdh indicates that the slot is a Private key that can only be used in ECDH with Trusted Public Key
#define MCUXCLELS_KEYPROPERTY_TECDH_FALSE                (0U) ///< This value of #mcuxClEls_KeyProp_t.utecdh indicates that the slot is not a Private key that can only be used in ECDH with Trusted Public Key
#endif /* MCUXCL_FEATURE_ELS_PUK_INTERNAL */
#define MCUXCLELS_KEYPROPERTY_CMAC_TRUE                  (1U) ///< This value of #mcuxClEls_KeyProp_t.ucmac indicates that the key can be used for CMAC
#define MCUXCLELS_KEYPROPERTY_CMAC_FALSE                 (0U) ///< This value of #mcuxClEls_KeyProp_t.ucmac indicates that the key cannot be used for CMAC
#define MCUXCLELS_KEYPROPERTY_KSK_TRUE                   (1U) ///< This value of #mcuxClEls_KeyProp_t.uksk indicates that the key can be used for key signing
#define MCUXCLELS_KEYPROPERTY_KSK_FALSE                  (0U) ///< This value of #mcuxClEls_KeyProp_t.uksk indicates that the key cannot be used for key signing
#define MCUXCLELS_KEYPROPERTY_RTF_TRUE                   (1U) ///< This value of #mcuxClEls_KeyProp_t.urtf indicates that the key can be used for RTF signing
#define MCUXCLELS_KEYPROPERTY_RTF_FALSE                  (0U) ///< This value of #mcuxClEls_KeyProp_t.urtf indicates that the key cannot be used for RTF signing
#define MCUXCLELS_KEYPROPERTY_CKDF_TRUE                  (1U) ///< This value of #mcuxClEls_KeyProp_t.uckdf indicates that the key can be used for CKDF
#define MCUXCLELS_KEYPROPERTY_CKDF_FALSE                 (0U) ///< This value of #mcuxClEls_KeyProp_t.uckdf indicates that the key cannot be used for CKDF
#define MCUXCLELS_KEYPROPERTY_HKDF_TRUE                  (1U) ///< This value of #mcuxClEls_KeyProp_t.uhkdf indicates that the key can be used for HKDF
#define MCUXCLELS_KEYPROPERTY_HKDF_FALSE                 (0U) ///< This value of #mcuxClEls_KeyProp_t.uhkdf indicates that the key cannot be used for HKDF
#define MCUXCLELS_KEYPROPERTY_ECC_TRUE                   (1U) ///< This value of #mcuxClEls_KeyProp_t.uecsg indicates that the key can be used for ECC signing
#define MCUXCLELS_KEYPROPERTY_ECC_FALSE                  (0U) ///< This value of #mcuxClEls_KeyProp_t.uecsg indicates that the key cannot be used for ECC signing
#define MCUXCLELS_KEYPROPERTY_ECC_DH_PRIVATE_TRUE        (1U) ///< This value of #mcuxClEls_KeyProp_t.uecdh indicates that the key is a ECC Diffie Hellman private key
#define MCUXCLELS_KEYPROPERTY_ECC_DH_PRIVATE_FALSE       (0U) ///< This value of #mcuxClEls_KeyProp_t.uecdh indicates that the key is not an ECC Diffie Hellman private key
#define MCUXCLELS_KEYPROPERTY_AES_TRUE                   (1U) ///< This value of #mcuxClEls_KeyProp_t.uaes indicates that the key is an AES key
#define MCUXCLELS_KEYPROPERTY_AES_FALSE                  (0U) ///< This value of #mcuxClEls_KeyProp_t.uaes indicates that the key is not an AES key
#define MCUXCLELS_KEYPROPERTY_HMAC_TRUE                  (1U) ///< This value of #mcuxClEls_KeyProp_t.uhmac indicates that the key is an HMAC key
#define MCUXCLELS_KEYPROPERTY_HMAC_FALSE                 (0U) ///< This value of #mcuxClEls_KeyProp_t.uhmac indicates that the key is not an HMAC key
#define MCUXCLELS_KEYPROPERTY_KWK_TRUE                   (1U) ///< This value of #mcuxClEls_KeyProp_t.ukwk indicates that the key is a Key Wrapping Key
#define MCUXCLELS_KEYPROPERTY_KWK_FALSE                  (0U) ///< This value of #mcuxClEls_KeyProp_t.ukwk indicates that the key is not a Key Wrapping Key
#define MCUXCLELS_KEYPROPERTY_KUOK_TRUE                  (1U) ///< This value of #mcuxClEls_KeyProp_t.ukuok indicates that the key is a Key Unwrapping Only Key
#define MCUXCLELS_KEYPROPERTY_KUOK_FALSE                 (0U) ///< This value of #mcuxClEls_KeyProp_t.ukuok indicates that the key is not a Key Unwrapping Only Key
#define MCUXCLELS_KEYPROPERTY_TLS_PREMASTER_SECRET_TRUE  (1U) ///< This value of #mcuxClEls_KeyProp_t.utlspms indicates that the key is a TLS Premaster Secret
#define MCUXCLELS_KEYPROPERTY_TLS_PREMASTER_SECRET_FALSE (0U) ///< This value of #mcuxClEls_KeyProp_t.utlspms indicates that the key is not a TLS Premaster Secret
#define MCUXCLELS_KEYPROPERTY_TLS_MASTER_SECRET_TRUE     (1U) ///< This value of #mcuxClEls_KeyProp_t.utlsms indicates that the key is a TLS Master Secret
#define MCUXCLELS_KEYPROPERTY_TLS_MASTER_SECRET_FALSE    (0U) ///< This value of #mcuxClEls_KeyProp_t.utlsms indicates that the key is not a TLS Master Secret
#define MCUXCLELS_KEYPROPERTY_INPUT_FOR_ECC_TRUE         (1U) ///< This value of #mcuxClEls_KeyProp_t.ukgsrc indicates that the key can be used as key material input for ECC key generation
#define MCUXCLELS_KEYPROPERTY_INPUT_FOR_ECC_FALSE        (0U) ///< This value of #mcuxClEls_KeyProp_t.ukgsrc indicates that the key cannot be used as key material input for ECC key generation
#define MCUXCLELS_KEYPROPERTY_HW_OUT_TRUE                (1U) ///< This value of #mcuxClEls_KeyProp_t.uhwo indicates that the key can be used in a hardware out key slot
#define MCUXCLELS_KEYPROPERTY_HW_OUT_FALSE               (0U) ///< This value of #mcuxClEls_KeyProp_t.uhwo indicates that the key cannot be used in a hardware out key slot
#define MCUXCLELS_KEYPROPERTY_WRAP_TRUE                  (1U) ///< This value of #mcuxClEls_KeyProp_t.wrpok indicates that the key can be wrapped
#define MCUXCLELS_KEYPROPERTY_WRAP_FALSE                 (0U) ///< This value of #mcuxClEls_KeyProp_t.wrpok indicates that the key cannot be wrapped
#define MCUXCLELS_KEYPROPERTY_DEVICE_UNIQUE_TRUE         (1U) ///< This value of #mcuxClEls_KeyProp_t.duk indicates that the key is a Device Unique Key
#define MCUXCLELS_KEYPROPERTY_DEVICE_UNIQUE_FALSE        (0U) ///< This value of #mcuxClEls_KeyProp_t.duk indicates that the key is not a Device Unique Key
#define MCUXCLELS_KEYPROPERTY_PRIVILEGED_TRUE            (1U) ///< This value of #mcuxClEls_KeyProp_t.upprot_priv indicates that the caller must be in privileged mode to use the key
#define MCUXCLELS_KEYPROPERTY_PRIVILEGED_FALSE           (0U) ///< This value of #mcuxClEls_KeyProp_t.upprot_priv indicates that the caller does not need to be in privileged mode to use the key
#define MCUXCLELS_KEYPROPERTY_SECURE_TRUE                (0U) ///< This value of #mcuxClEls_KeyProp_t.upprot_sec indicates that the caller must be in secure mode to use the key
#define MCUXCLELS_KEYPROPERTY_SECURE_FALSE               (1U) ///< This value of #mcuxClEls_KeyProp_t.upprot_sec indicates that the caller does not need to be in secure mode to use the key

/**
 * @}
 */

/**
 * @defgroup MCUXCLELS_STATUS_ MCUXCLELS_STATUS_
 * @brief Return code definitions
 * @ingroup mcuxClEls_Types_Macros
 * @{
 */
#define MCUXCLELS_STATUS_OK                   ((mcuxClEls_Status_t) 0x05552E03u) ///< No error occurred
#define MCUXCLELS_STATUS_OK_WAIT              ((mcuxClEls_Status_t) 0x05552E07u) ///< An <tt>_Async</tt> function successfully started an ELS command. Call #mcuxClEls_WaitForOperation to complete it
#define MCUXCLELS_STATUS_HW_FAULT             ((mcuxClEls_Status_t) 0x05555330u) ///< ELS hardware detected a fault
#define MCUXCLELS_STATUS_HW_ALGORITHM         ((mcuxClEls_Status_t) 0x05555334u) ///< An algorithm failed in hardware
#define MCUXCLELS_STATUS_HW_OPERATIONAL       ((mcuxClEls_Status_t) 0x05555338u) ///< ELS was operated incorrectly
#define MCUXCLELS_STATUS_HW_BUS               ((mcuxClEls_Status_t) 0x0555533Cu) ///< A bus access failed
#define MCUXCLELS_STATUS_HW_INTEGRITY         ((mcuxClEls_Status_t) 0x05555370u) ///< An integrity check failed in hardware
#define MCUXCLELS_STATUS_HW_PRNG              ((mcuxClEls_Status_t) 0x05555374u) ///< Read access to PRNG output while PRNG is not in ready state
#define MCUXCLELS_STATUS_HW_DTRNG             ((mcuxClEls_Status_t) 0x05555378u) ///< Unable to get entropy from dTRNG with current configuration
#define MCUXCLELS_STATUS_SW_FAULT             ((mcuxClEls_Status_t) 0x0555F0F0u) ///< Software detected a fault
#define MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT  ((mcuxClEls_Status_t) 0x055553B0u) ///< an ELS command was started while the ELS was still busy, or a SHA-Direct command was started while the SHA kernel was still busy
#define MCUXCLELS_STATUS_SW_INVALID_PARAM     ((mcuxClEls_Status_t) 0x055553F8u) ///< Incorrect parameters were supplied
#define MCUXCLELS_STATUS_SW_INVALID_STATE     ((mcuxClEls_Status_t) 0x055553B8u) ///< This can happen when ELS is in a wrong state for the requested ELS command
#define MCUXCLELS_STATUS_SW_COUNTER_EXPIRED   ((mcuxClEls_Status_t) 0x055553BCu) ///< A software counter expired while waiting for an ELS operation to finish
#define MCUXCLELS_STATUS_SW_COMPARISON_FAILED ((mcuxClEls_Status_t) 0x05558930u) ///< A comparison between an ELS flag and its expected value failed
#ifdef MCUXCL_FEATURE_ELS_LOCKING
#define MCUXCLELS_STATUS_SW_LOCKING_FAILED    ((mcuxClEls_Status_t) 0x055553F4u) ///< Unable to obtain ELS lock
#define MCUXCLELS_STATUS_SW_STATUS_LOCKED     ((mcuxClEls_Status_t) 0x05552E0Bu) ///< ELS status is locked
#endif /* MCUXCL_FEATURE_ELS_LOCKING */
/** @} */

#define MCUXCLELS_STATUS_IS_HW_ERROR(x_) ((((mcuxClEls_Status_t) (x_)) & 0x0000FF00U) == 0x0000E100U) ///< Checks whether an error code is a hardware error. Indicates that an error was reported by ELS hardware.

#define MCUXCLELS_STATUS_IS_SW_ERROR(x_) ((((mcuxClEls_Status_t) (x_)) & 0x0000FF00U) == 0x0000F000U) ///< Checks whether an error code is a software error. Indicates that the error was detected by the driver software and not by ELS hardware.

/**
 * @}
 */

/**********************************************
 * TYPEDEFS
 **********************************************/
/**
 * @defgroup mcuxClEls_Types_Types mcuxClEls_Types_Types
 * @brief Defines all types of @ref mcuxClEls_Types
 * @ingroup mcuxClEls_Types
 * @{
 */
/**
 * @brief Type for ELS driver status codes
 */
typedef uint32_t mcuxClEls_Status_t;

/**
 * @brief Deprecated type for ELS driver protected status codes
 */
typedef MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_Status_Protected_t;

/**
 * @brief Type for ELS keystore indices
 */
typedef uint32_t mcuxClEls_KeyIndex_t;

/** Type for ELS key store key properties */
typedef union
{
    struct
    {
        uint32_t value;          ///< Accesses the bit field as a full word; initialize with a combination of constants from @ref MCUXCLELS_KEYPROPERTY_VALUE_
    } word;                      ///< Access #mcuxClEls_KeyProp_t word-wise
    struct
    {
#ifdef MCUXCL_FEATURE_ELS_PUK_INTERNAL
        uint32_t ksize :2;       ///< Key size
        uint32_t :3;             ///< RFU
#else
        uint32_t ksize :1;       ///< Key size
        uint32_t :4;             ///< RFU
#endif
        uint32_t kactv :1;       ///< Status flag to indicate whether the key slot contains an active key or not
        uint32_t kbase :1;       ///< Status flag to indicate whether the key slot is a base slot or the second slot of a 256-bit key
        uint32_t fgp :1;         ///< Hardware feature flag: General purpose key slot
        uint32_t frtn :1;        ///< Hardware feature flag: Retention key slot
        uint32_t fhwo :1;        ///< Hardware feature flag: Hardware-out key slot
#ifdef MCUXCL_FEATURE_ELS_PUK_INTERNAL
        uint32_t :1;             ///< RFU
        uint32_t upuk :1;        ///< Usage permission for Trusted Public Key
        uint32_t utecdh :1;      ///< Usage permission for Private key that can only be used in ECDH with Trusted Public Key
#else
        uint32_t :3;             ///< RFU
#endif /* MCUXCL_FEATURE_ELS_PUK_INTERNAL */
        uint32_t ucmac :1;       ///< Usage permission for CMAC
        uint32_t uksk :1;        ///< Usage permission for key signing
        uint32_t urtf :1;        ///< Usage permission for RTF signing
        uint32_t uckdf :1;       ///< Usage permission for CKDF
        uint32_t uhkdf :1;       ///< Usage permission for HKDF
        uint32_t uecsg :1;       ///< Usage permission for ECDSA signing
        uint32_t uecdh :1;       ///< Usage permission for Elliptic Curve Diffie-Hellman
        uint32_t uaes :1;        ///< Usage permission for AES
        uint32_t uhmac :1;       ///< Usage permission for HMAC
        uint32_t ukwk :1;        ///< Usage permission for key wrapping
        uint32_t ukuok :1;       ///< Usage permission for key unwrapping, but not for key wrapping
        uint32_t utlspms :1;     ///< Usage permission as a TLS premaster secret
        uint32_t utlsms :1;      ///< Usage permission as a TLS master secret
        uint32_t ukgsrc :1;      ///< Usage permission as input for ECC key generation
        uint32_t uhwo :1;        ///< Usage permission in a hardware-out key slot
        uint32_t wrpok :1;       ///< Usage permission to wrap
        uint32_t duk :1;         ///< Device-unique key flag
        uint32_t upprot_priv :1; ///< Access restriction to privileged mode
        uint32_t upprot_sec :1;  ///< Access restriction to TrustZone secure mode
    } bits;                      ///< Access #mcuxClEls_KeyProp_t bit-wise
} mcuxClEls_KeyProp_t;


#define utlpsms utlspms ///< Deprecated name for #mcuxClEls_KeyProp_t.utlspms

/**
 * @brief Function type for transfer of data to a memory-mapped register
 *
 * This function type is used as a callback for handling data transfer from memory to a memory-mapped register.
 * Such a function shall read data from the @c uint8_t array source, and write data via a sequence of writes to @p destRegister.
 * Further specification of this function's behavior can be found in the documentation of the function that accepts this function as a callback parameter.
 * 
 * @param [out] pDestRegister  Memory-mapped register that the output data shall be written to
 * @param [in]  pSource        Array containing the input data
 * @param [in]  sourceLength   Size of @p source in bytes
 * @param [in, out] pCallerData   Custom pointer that is provided by the caller and forwarded to the callback function by the operation
 * @return An error code that can be any error code in @ref MCUXCLELS_STATUS_, see individual documentation for more information
 */
typedef mcuxClEls_Status_t (*mcuxClEls_TransferToRegisterFunction_t)(
    uint32_t volatile * pDestRegister,
    uint8_t const * pSource,
    size_t sourceLength,
    void * pCallerData);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLELS_TYPES_H_ */

/**
 * @}
 *
 * @}
 */
