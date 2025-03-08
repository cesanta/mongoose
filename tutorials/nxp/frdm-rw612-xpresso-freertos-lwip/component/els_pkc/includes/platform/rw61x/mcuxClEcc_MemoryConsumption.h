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
 * @file  mcuxClEcc_MemoryConsumption.h
 * @brief Defines the memory consumption for the mcuxClEcc component
 */

#ifndef MCUXCLECC_MEMORYCONSUMPTION_H_
#define MCUXCLECC_MEMORYCONSUMPTION_H_

#include <mcuxClCore_Macros.h>

/**
 * @defgroup mcuxClEcc_MemoryConsumption mcuxClEcc_MemoryConsumption
 * @brief Defines the memory consumption for the @ref mcuxClEcc component
 * @ingroup mcuxClEcc
 * @{
 */

/**
 * @addtogroup MCUXCLECC_WACPU_
 * @brief Define the CPU workarea size required by mcuxClEcc APIs.
 * @{
 */
#define MCUXCLECC_KEYGEN_WACPU_SIZE                      504u  ///< CPU workarea size (in bytes) for #mcuxClEcc_KeyGen.

#define MCUXCLECC_SIGN_WACPU_SIZE                        504u  ///< CPU workarea size (in bytes) for #mcuxClEcc_Sign.
#ifdef MCUXCL_FEATURE_ECC_ECDSA_DETERMINISTIC
#define MCUXCLECC_SIGN_DETERMINISTIC_ECDSA_WACPU_SIZE    $(mcuxClEcc_Sign_DeterministicECDSA_WaCPU_SIZE)u  ///< CPU workarea size (in bytes) for #mcuxClEcc_Sign.
#endif /* MCUXCL_FEATURE_ECC_ECDSA_DETERMINISTIC */

#define MCUXCLECC_VERIFY_WACPU_SIZE                      424u  ///< CPU workarea size (in bytes) for #mcuxClEcc_Verify.

#define MCUXCLECC_POINTMULT_WACPU_SIZE                    416u  ///< CPU workarea size (in bytes) for #mcuxClEcc_PointMult.





/**
 * @}
 */  /* MCUXCLECC_WACPU_ */

/**
 * @addtogroup MCUXCLECC_MONTDH_WACPU_
 * @brief Define the CPU workarea size required by mcuxClEcc MontDH APIs.
 * @{
 */

#define MCUXCLECC_MONTDH_GENERATEKEYPAIR_CURVE25519_WACPU_SIZE    444u  ///< CPU workarea size (in bytes) for #mcuxClEcc_MontDH_GenerateKeyPair.
#define MCUXCLECC_MONTDH_GENERATEKEYPAIR_CURVE448_WACPU_SIZE      468u  ///< CPU workarea size (in bytes) for #mcuxClEcc_MontDH_GenerateKeyPair.

#define MCUXCLECC_MONTDH_KEYAGREEMENT_CURVE25519_WACPU_SIZE     88u  ///< CPU workarea size (in bytes) for #mcuxClEcc_MontDH_KeyAgreement.
#define MCUXCLECC_MONTDH_KEYAGREEMENT_CURVE448_WACPU_SIZE       88u  ///< CPU workarea size (in bytes) for #mcuxClEcc_MontDH_KeyAgreement.

/**
 * @}
 */  /* MCUXCLECC_MONTDH_WACPU_ */

 /**
 * @addtogroup MCUXCLECC_EDDSA_WACPU_
 * @brief Define the CPU workarea size required by mcuxClEcc EdDSA APIs.
 * @{
 */

#define MCUXCLECC_EDDSA_GENERATEKEYPAIR_ED25519_WACPU_SIZE    680u  ///< CPU workarea size (in bytes) for #mcuxClEcc_EdDSA_GenerateKeyPair for Ed25519.
#define MCUXCLECC_EDDSA_GENERATESIGNATURE_ED25519_WACPU_SIZE  540u  ///< CPU workarea size (in bytes) for #mcuxClEcc_EdDSA_GenerateSignature for Ed25519.
#define MCUXCLECC_EDDSA_VERIFYSIGNATURE_ED25519_WACPU_SIZE    540u  ///< CPU workarea size (in bytes) for #mcuxClEcc_EdDSA_VerifySignature for Ed25519.


/**
 * @}
 */  /* MCUXCLECC_EDDSA_WACPU_ */

/**
 * @addtogroup MCUXCLECC_WAPKC_
 * @brief Define the PKC workarea size required by mcuxClEcc APIs.
 * @{
 */

/**
 * @brief PKC wordsize in ECC component.
 */
#define MCUXCLECC_PKC_WORDSIZE  8u

/**
 * PKC workarea size (in bytes) for #mcuxClEcc_KeyGen for arbitrary lengths of p and n.
 */
#define MCUXCLECC_KEYGEN_WAPKC_SIZE(pByteLen,nByteLen)  \
    (24u * (MCUXCLCORE_ALIGN_TO_WORDSIZE(MCUXCLECC_PKC_WORDSIZE, MCUXCLCORE_MAX(pByteLen,nByteLen)) + MCUXCLECC_PKC_WORDSIZE))

#define MCUXCLECC_KEYGEN_WAPKC_SIZE_128 (576u)
#define MCUXCLECC_KEYGEN_WAPKC_SIZE_256 (960u)
#define MCUXCLECC_KEYGEN_WAPKC_SIZE_384 (1344u)
#define MCUXCLECC_KEYGEN_WAPKC_SIZE_512 (1728u)
#define MCUXCLECC_KEYGEN_WAPKC_SIZE_640 (2112u)

/**
 * PKC workarea size (in bytes) for #mcuxClEcc_Sign for arbitrary lengths of p and n.
 */
#define MCUXCLECC_SIGN_WAPKC_SIZE(pByteLen,nByteLen)  \
    (24u * (MCUXCLCORE_ALIGN_TO_WORDSIZE(MCUXCLECC_PKC_WORDSIZE, MCUXCLCORE_MAX(pByteLen,nByteLen)) + MCUXCLECC_PKC_WORDSIZE))

#define MCUXCLECC_SIGN_WAPKC_SIZE_128 (576u )
#define MCUXCLECC_SIGN_WAPKC_SIZE_256 (960u )
#define MCUXCLECC_SIGN_WAPKC_SIZE_384 (1344u )
#define MCUXCLECC_SIGN_WAPKC_SIZE_512 (1728u )
#define MCUXCLECC_SIGN_WAPKC_SIZE_640 (2112u )

/**
 * PKC workarea size (in bytes) for #mcuxClEcc_Verify for arbitrary lengths of p and n.
 */

#define MCUXCLECC_VERIFY_WAPKC_SIZE(pByteLen,nByteLen)  \
    (28u * (MCUXCLCORE_ALIGN_TO_WORDSIZE(MCUXCLECC_PKC_WORDSIZE, MCUXCLCORE_MAX(pByteLen,nByteLen)) + MCUXCLECC_PKC_WORDSIZE))

#define MCUXCLECC_VERIFY_WAPKC_SIZE_128 (672u)
#define MCUXCLECC_VERIFY_WAPKC_SIZE_256 (1120u)
#define MCUXCLECC_VERIFY_WAPKC_SIZE_384 (1568u)
#define MCUXCLECC_VERIFY_WAPKC_SIZE_512 (2016u)
#define MCUXCLECC_VERIFY_WAPKC_SIZE_640 (2464u)

/**
 * PKC workarea size (in bytes) for #mcuxClEcc_PointMult for arbitrary lengths of p and n.
 */
#define MCUXCLECC_POINTMULT_WAPKC_SIZE(pByteLen,nByteLen)  \
    (24u * (MCUXCLCORE_ALIGN_TO_WORDSIZE(MCUXCLECC_PKC_WORDSIZE, MCUXCLCORE_MAX(pByteLen,nByteLen)) + MCUXCLECC_PKC_WORDSIZE))

#define MCUXCLECC_POINTMULT_WAPKC_SIZE_128 (576u)
#define MCUXCLECC_POINTMULT_WAPKC_SIZE_256 (960u)
#define MCUXCLECC_POINTMULT_WAPKC_SIZE_384 (1344u)
#define MCUXCLECC_POINTMULT_WAPKC_SIZE_512 (1728u)
#define MCUXCLECC_POINTMULT_WAPKC_SIZE_640 (2112u)





/**
 * @}
 */  /* MCUXCLECC_WAPKC_ */


/**
 * @addtogroup MCUXCLECC_MONTDH_WAPKC_
 * @brief Define the PKC workarea size required by mcuxClEcc_Mont APIs.
 * @{
 */

/**
 * PKC workarea size (in bytes) for #mcuxClEcc_MontDH_KeyGeneration.
 */
#define MCUXCLECC_MONTDH_GENERATEKEYPAIR_CURVE25519_WAPKC_SIZE  880u
#define MCUXCLECC_MONTDH_GENERATEKEYPAIR_CURVE448_WAPKC_SIZE  1408u

/**
 * PKC workarea size (in bytes) for #mcuxClEcc_MontDH_KeyAgreement.
 */
#define MCUXCLECC_MONTDH_KEYAGREEMENT_CURVE25519_WAPKC_SIZE  880u
#define MCUXCLECC_MONTDH_KEYAGREEMENT_CURVE448_WAPKC_SIZE  1408u

/**
 * @}
 */  /* MCUXCLECC_MONTDH_WAPKC_ */

/**
 * @addtogroup MCUXCLECC_EDDSA_WAPKC_
 * @brief Define the PKC workarea size required by mcuxClEcc EdDSA APIs.
 * @{
 */

/**
 * PKC workarea size (in bytes) for #mcuxClEcc_EdDSA_GenerateKeyPair.
 */
#define MCUXCLECC_EDDSA_GENERATEKEYPAIR_ED25519_WAPKC_SIZE    1760u  ///< PKC workarea size (in bytes) for #mcuxClEcc_EdDSA_GenerateKeyPair for Ed25519.

/**
 * PKC workarea size (in bytes) for #mcuxClEcc_EdDSA_GenerateSignature.
 */
#define MCUXCLECC_EDDSA_GENERATESIGNATURE_ED25519_WAPKC_SIZE  1760u  ///< PKC workarea size (in bytes) for #mcuxClEcc_EdDSA_GenerateSignature for Ed25519.

/**
 * PKC workarea size (in bytes) for #mcuxClEcc_EdDSA_VerifySignature.
 */
#define MCUXCLECC_EDDSA_VERIFYSIGNATURE_ED25519_WAPKC_SIZE    1760u  ///< PKC workarea size (in bytes) for #mcuxClEcc_EdDSA_VerifySignature for Ed25519.

/**
 * @}
 */  /* MCUXCLECC_EDDSA_WAPKC_ */


/**
 * @addtogroup MCUXCLECC_EDDSA_GENKEYPAIR_DESC_SIZE_
 * @brief Define for the EdDSA key pair generation descriptor size.
 * @{
 */
#define MCUXCLECC_EDDSA_GENERATEKEYPAIR_DESCRIPTOR_SIZE    8u  ///< EdDSA key pair generation descriptor size in bytes.
#define MCUXCLECC_EDDSA_GENERATEKEYPAIR_DESCRIPTOR_SIZE_IN_WORDS MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(MCUXCLECC_EDDSA_GENERATEKEYPAIR_DESCRIPTOR_SIZE ) ///< EdDSA key pair generation descriptor size in words.

/**
 * @}
 */  /* MCUXCLECC_EDDSA_GENKEYPAIR_DESC_SIZE_ */

/**
 * @addtogroup MCUXCLECC_EDDSA_SIGNATURE_PROTOCOL_DESC_SIZE_
 * @brief Define for the EdDSA signature protocol descriptor size.
 * @{
 */
#define MCUXCLECC_EDDSA_SIGNATURE_PROTOCOL_DESCRIPTOR_SIZE 20u  ///< EdDSA signature generation descriptor size.

#define MCUXCLECC_EDDSA_ED25519_SIZE_SIGNATURE_PROTOCOL_DESCRIPTOR(contextLen) \
    (MCUXCLECC_EDDSA_SIGNATURE_PROTOCOL_DESCRIPTOR_SIZE + MCUXCLECC_EDDSA_SIZE_HASH_PREFIX(MCUXCLECC_EDDSA_ED25519_DOMPREFIXLEN, (contextLen)))  ///< Byte length of an Ed25519 signature protocol descriptor.
#define MCUXCLECC_EDDSA_ED25519_SIGNATURE_PROTOCOL_DESCRIPTOR_SIZE_IN_WORD(contextLen) \
        MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(MCUXCLECC_EDDSA_ED25519_SIZE_SIGNATURE_PROTOCOL_DESCRIPTOR(contextLen))


/**
 * @}
 */  /* MCUXCLECC_EDDSA_SIGNATURE_PROTOCOL_DESC_SIZE_ */



#ifdef MCUXCL_FEATURE_ECC_ECDSA_DETERMINISTIC
/**
 * @addtogroup MCUXCLECC_ECDSA_SIGNATURE_PROTOCOL_DESC_SIZE_
 * @brief Define for the ECDSA signature protocol descriptor size.
 * @{
 */
#define MCUXCLECC_ECDSA_SIGNATURE_PROTOCOL_DESCRIPTOR_SIZE 12u  ///< ECDSA signature generation descriptor size in bytes.
#define MCUXCLECC_ECDSA_SIGNATURE_PROTOCOL_DESCRIPTOR_SIZE_IN_WORDS MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(MCUXCLECC_ECDSA_SIGNATURE_PROTOCOL_DESCRIPTOR_SIZE) ///< ECDSA signature generation descriptor size in words.
/**
 * @}
 */  /* MCUXCLECC_EDDSA_SIGNATURE_PROTOCOL_DESC_SIZE_ */
 #endif /* MCUXCL_FEATURE_ECC_ECDSA_DETERMINISTIC */


/**
 * @}
 */  /* mcuxClEcc_MemoryConsumption */

#endif /* MCUXCLECC_MEMORYCONSUMPTION_H_ */
