/*--------------------------------------------------------------------------*/
/* Copyright 2020, 2022 NXP                                                 */
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
 * @file  mcuxClEls.h
 * @brief Top-level include file for the ELS driver
 *
 * This includes headers for all of the functionality provided by the ELS IP.
 *
 * @defgroup mcuxClEls mcuxClEls
 * @brief ELS driver
 *
 * This component abstracts the hardware access to the ELS IP.
 * The library exposes the following hardware functionality:
 * <ol>
 *      <li> COMMON
 *          <ul>
 *              <li> Determine information of the underlying ELS hardware IP
 *                  <ul> <li> #mcuxClEls_GetHwVersion </ul>
 *                  @if MCUXCL_FEATURE_ELS_HWCONFIG
 *                  <ul> <li> #mcuxClEls_GetHwConfig </ul>
 *                  @endif
 *                  <ul> <li> #mcuxClEls_GetHwState </ul>
 *              <li> ELS enabling, disabling, and software reset
 *                  <ul> <li> #mcuxClEls_Enable_Async </ul>
 *                  <ul> <li> #mcuxClEls_Reset_Async </ul>
 *                  <ul> <li> #mcuxClEls_Disable </ul>
 *              <li> Interrupt management
 *                  <ul> <li> #mcuxClEls_SetIntEnableFlags </ul>
 *                  <ul> <li> #mcuxClEls_GetIntEnableFlags </ul>
 *                  <ul> <li> #mcuxClEls_ResetIntFlags </ul>
 *                  <ul> <li> #mcuxClEls_SetIntFlags </ul>
 *              <li> Wait for completion of an ELS operation
 *                  <ul> <li> #mcuxClEls_WaitForOperation </ul>
 *                  <ul> <li> #mcuxClEls_LimitedWaitForOperation </ul>
 *              <li> Error handling
 *                  <ul> <li> #mcuxClEls_ResetErrorFlags </ul>
 *                  <ul> <li> #mcuxClEls_GetErrorCode </ul>
 *                  <ul> <li> #mcuxClEls_GetErrorLevel </ul>
 *              <li> Random delay feature for AES based operations
 *                  <ul> <li> #mcuxClEls_SetRandomStartDelay </ul>
 *                  <ul> <li> #mcuxClEls_GetRandomStartDelay </ul>
 *              @if MCUXCL_FEATURE_ELS_LOCKING
 *              <li> ELS Locking
 *                  <ul> <li> #mcuxClEls_GetLock </ul>
 *                  <ul> <li> #mcuxClEls_ReleaseLock </ul>
 *                  <ul> <li> #mcuxClEls_IsLocked </ul>
 *                  <ul> <li> #mcuxClEls_SetMasterUnlock </ul>
 *              @endif
 *              @if MCUXCL_FEATURE_ELS_RESP_GEN
 *              <li> Calculate response to a hardware generated challenge
 *                  <ul> <li> #mcuxClEls_RespGen_Async </ul>
 *              @endif
 *              @if MCUXCL_FEATURE_ELS_DMA_ADDRESS_READBACK
 *              <li> Final Address Readback (security feature)
 *                  <ul> <li> #mcuxClEls_GetLastDmaAddress </ul>
 *              @endif
 *              @if MCUXCL_FEATURE_ELS_DMA_FINAL_ADDRESS_READBACK
 *              <li> Final Address Compare (security feature)
 *                  <ul> <li> #mcuxClEls_CompareDmaFinalOutputAddress </ul>
 *              @endif
 *          </ul>
 *      <li> CRC
 *          <ul>
 *              <li> Command CRC checks
 *                  <ul> <li> #mcuxClEls_ConfigureCommandCRC </ul>
 *                  <ul> <li> #mcuxClEls_GetCommandCRC </ul>
 *                  <ul> <li> #mcuxClEls_VerifyVsRefCRC </ul>
 *                  <ul> <li> #mcuxClEls_UpdateRefCRC </ul>
 *          </ul>
 *      <li> HASH
 *          <ul>
 *              <li> SHA-2 hashing
 *                  <ul> <li> #mcuxClEls_Hash_Async </ul>
 *          @if MCUXCL_FEATURE_ELS_SHA_DIRECT
 *              <li> SHA-2 hashing in direct mode
 *                  <ul> <li> #mcuxClEls_ShaDirect_Enable </ul>
 *                  <ul> <li> #mcuxClEls_ShaDirect_Disable </ul>
 *                  <ul> <li> #mcuxClEls_Hash_ShaDirect </ul>
 *          @endif
 *          </ul>
 *      @if MCUXCL_FEATURE_ELS_HMAC
 *      <li> HMAC (Keyed-Hash Message Authentication Code)
 *          <ul>
 *              <li> HMAC
 *                  <ul> <li> #mcuxClEls_Hmac_Async </ul>
 *          </ul>
 *      @endif
 *      @if MCUXCL_FEATURE_ELS_CMAC
 *      <li> CMAC (Cipher-Based Message Authentication Code)
 *          <ul>
 *              <li> CMAC
 *                  <ul> <li> #mcuxClEls_Cmac_Async </ul>
 *          </ul>
 *      @endif
 *      <li> CIPHER (Symmetric Encryption)
 *          <ul>
 *              <li> AES
 *                  <ul> <li> #mcuxClEls_Cipher_Async </ul>
 *          </ul>
 *      @if MCUXCL_FEATURE_ELS_AEAD
 *      <li> AEAD (Authenticated Encryption with Associated Data)
 *          <ul>
 *              <li> Authenticated Encryption with Associated Data
 *                  <ul>
 *                      <li> #mcuxClEls_Aead_Init_Async
 *                      <li> #mcuxClEls_Aead_UpdateAad_Async
 *                      <li> #mcuxClEls_Aead_UpdateData_Async
 *                      <li> #mcuxClEls_Aead_Finalize_Async
 *                  </ul>
 *          </ul>
 *      @endif
 *      <li> KEY MANAGEMENT
 *          <ul>
 *          @if MCUXCL_FEATURE_ELS_KEY_MGMT_DELETE
 *              <li> Key deletion
 *                  <ul> <li> #mcuxClEls_KeyDelete_Async </ul>
 *          @endif
 *          @if MCUXCL_FEATURE_ELS_KEY_MGMT_KEYPROV
 *              <li> Key provisioning
 *                  <ul> <li> #mcuxClEls_KeyProvision_Async </ul>
 *          @endif
 *          @if MCUXCL_FEATURE_ELS_KEY_MGMT_KEYPROV_ROM
 *              <li> Key provisioning (ROM)
 *                  <ul> <li> #mcuxClEls_KeyProvisionRom_Async </ul>
 *          @endif
 *              <li> Key import
 *                  <ul> <li> #mcuxClEls_KeyImport_Async </ul>
 *          @if MCUXCL_FEATURE_ELS_PUK_INTERNAL
 *              <li> Public key import
 *                  <ul> <li> #mcuxClEls_KeyImportPuk_Async </ul>
 *          @endif
 *          @if MCUXCL_FEATURE_ELS_KEY_MGMT_EXPORT
 *              <li> Key export
 *                  <ul> <li> #mcuxClEls_KeyExport_Async </ul>
 *          @endif
 *              <li> Key properties
 *                  <ul> <li> #mcuxClEls_GetKeyProperties </ul>
 *          </ul>
 *      @if MCUXCL_FEATURE_ELS_RNG
 *      <li> RNG
 *          <ul>
 *              <li> Random data generation using DRBG
 *                  <ul> <li> #mcuxClEls_Rng_DrbgRequest_Async </ul>
 *              @if MCUXCL_FEATURE_ELS_RND_RAW
 *              <li> Get raw (unprocessed) random data from the DTRNG
 *                  <ul> <li> #mcuxClEls_Rng_DrbgRequestRaw_Async </ul>
 *              @endif
 *              <li> FIPS CAVP test mode
 *                  <ul> <li> #mcuxClEls_Rng_DrbgTestInstantiate_Async </ul>
 *                  <ul> <li> #mcuxClEls_Rng_DrbgTestExtract_Async </ul>
 *                  <ul> <li> #mcuxClEls_Rng_DrbgTestAesEcb_Async </ul>
 *                  <ul> <li> #mcuxClEls_Rng_DrbgTestAesCtr_Async </ul>
 *              <li> Configuration of the DTRNG
 *                  <ul> <li> #mcuxClEls_Rng_Dtrng_ConfigLoad_Async </ul>
 *                  <ul> <li> #mcuxClEls_Rng_Dtrng_ConfigEvaluate_Async </ul>
 *              <li> PRNG
 *              @if MCUXCL_FEATURE_ELS_PRND_INIT
 *                  <ul> <li> #mcuxClEls_Prng_Init_Async </ul>
 *              @endif
 *                  <ul> <li> #mcuxClEls_Prng_GetRandomWord </ul>
 *                  <ul> <li> #mcuxClEls_Prng_GetRandom </ul>
 *          </ul>
 *      @endif
 *      <li> ECC (Elliptic Curve Cryptography)
 *          <ul>
 *              <li> ECC Key generation
 *                  <ul> <li> #mcuxClEls_EccKeyGen_Async </ul>
 *              @if MCUXCL_FEATURE_ELS_ECC_KEY_EXCHANGE
 *              <li> ECC key exchange
 *                  <ul> <li> #mcuxClEls_EccKeyExchange_Async </ul>
 *                  @if MCUXCL_FEATURE_ELS_PUK_INTERNAL
 *                  <ul> <li> #mcuxClEls_EccKeyExchangeInt_Async </ul>
 *                  @endif
 *              @endif
 *              <li> ECC signature generation
 *                  <ul> <li> #mcuxClEls_EccSign_Async </ul>
 *              <li> ECC signature verification
 *                  <ul> <li> #mcuxClEls_EccVerify_Async </ul>
 *              @if MCUXCL_FEATURE_ELS_PUK_INTERNAL
 *                  <ul> <li> #mcuxClEls_EccVerifyInt_Async </ul>
 *              @endif
 *          </ul>
 *      <li> KEY DERIVATION
 *          <ul>
 *              <li> Key derivation
 *                  <ul>
 *                      @if MCUXCL_FEATURE_ELS_CKDF
 *                      <li> #mcuxClEls_Ckdf_Sp800108_Async
 *                      @if  MCUXCL_FEATURE_ELS_CKDF_SP80056C
 *                      <li> #mcuxClEls_Ckdf_Sp80056c_Extract_Async
 *                      <li> #mcuxClEls_Ckdf_Sp80056c_Expand_Async
 *                      @endif
 *                      @endif
 *                      @if MCUXCL_FEATURE_ELS_HKDF
 *                      <li> #mcuxClEls_Hkdf_Rfc5869_Async
 *                      <li> #mcuxClEls_Hkdf_Sp80056c_Async
 *                      @endif
 *                  </ul>
 *              @if MCUXCL_FEATURE_ELS_TLS
 *              <li> Master Key and Session Key derivation
 *                  <ul>
 *                      <li> #mcuxClEls_TlsGenerateMasterKeyFromPreMasterKey_Async
 *                      <li> #mcuxClEls_TlsGenerateSessionKeysFromMasterKey_Async
 *                  </ul>
 *              @endif
 *          </ul>
 *      @if MCUXCL_FEATURE_ELS_GLITCHDETECTOR
 *      <li> ELS Glitch Detector control
 *          <ul>
 *              <li> #mcuxClEls_GlitchDetector_LoadConfig_Async
 *              <li> #mcuxClEls_GlitchDetector_Trim_Async
 *              <li> #mcuxClEls_GlitchDetector_GetEventCounter
 *              <li> #mcuxClEls_GlitchDetector_ResetEventCounter
 *          </ul>
 *      @endif
 *  </ol>
 *
 *  After each call to a function ending in <tt>_Async</tt>, one of the waiting functions #mcuxClEls_WaitForOperation or #mcuxClEls_LimitedWaitForOperation must be called to ensure completion.
 *  The waiting functions may fail, e.g., when the ELS enters an error state.
 */

#ifndef MCUXCLELS_H_
#define MCUXCLELS_H_

#include <mcuxClConfig.h> // Exported features flags header

#include <mcuxClEls_Types.h>
#include <mcuxClEls_Common.h>
#ifdef MCUXCL_FEATURE_ELS_CMD_CRC
#include <mcuxClEls_Crc.h>
#endif /* MCUXCL_FEATURE_ELS_CMD_CRC */
#include <mcuxClEls_Hash.h>
#include <mcuxClEls_Hmac.h>
#include <mcuxClEls_Cmac.h>
#include <mcuxClEls_Cipher.h>
#include <mcuxClEls_Aead.h>
#include <mcuxClEls_KeyManagement.h>
#include <mcuxClEls_Rng.h>
#include <mcuxClEls_Ecc.h>
#include <mcuxClEls_Kdf.h>
#ifdef MCUXCL_FEATURE_ELS_GLITCHDETECTOR
#include <mcuxClEls_GlitchDetector.h>
#endif /* MCUXCL_FEATURE_ELS_GLITCHDETECTOR */

#endif /* MCUXCLELS_H_ */
