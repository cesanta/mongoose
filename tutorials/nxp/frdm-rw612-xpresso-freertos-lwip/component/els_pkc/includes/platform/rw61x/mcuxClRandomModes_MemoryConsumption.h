/*--------------------------------------------------------------------------*/
/* Copyright 2022-2024 NXP                                                  */
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
 * @file  mcuxClRandomModes_MemoryConsumption.h
 * @brief Defines the memory consumption for the mcuxClRandom component
 */

#ifndef MCUXCLRANDOMMODES_MEMORYCONSUMPTION_H_
#define MCUXCLRANDOMMODES_MEMORYCONSUMPTION_H_

#include <mcuxClCore_Macros.h>

/**
 * @defgroup mcuxClRandomModes_MemoryConsumption mcuxClRandomModes_MemoryConsumption
 * @brief Defines the memory consumption for the @ref mcuxClRandom component
 * @ingroup mcuxClRandom
 * @{
 */

#define MCUXCLRANDOMMODES_PATCHMODE_DESCRIPTOR_SIZE          (20u)
#define MCUXCLRANDOMMODES_PATCHMODE_DESCRIPTOR_SIZE_IN_WORDS (MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(MCUXCLRANDOMMODES_PATCHMODE_DESCRIPTOR_SIZE))

#ifdef MCUXCL_FEATURE_RANDOMMODES_TESTMODE
#define MCUXCLRANDOMMODES_TESTMODE_DESCRIPTOR_SIZE          (20u)
#define MCUXCLRANDOMMODES_TESTMODE_DESCRIPTOR_SIZE_IN_WORDS (MCUXCLCORE_NUM_OF_CPUWORDS_CEIL(MCUXCLRANDOMMODES_TESTMODE_DESCRIPTOR_SIZE))
#endif

#define MCUXCLRANDOMMODES_MAX_CPU_WA_BUFFER_SIZE                      (324u)

#define MCUXCLRANDOMMODES_INIT_WACPU_SIZE                             (312u)
#define MCUXCLRANDOMMODES_RESEED_WACPU_SIZE                           (280u)
#define MCUXCLRANDOMMODES_GENERATE_WACPU_SIZE                         (280u)
#define MCUXCLRANDOMMODES_SELFTEST_WACPU_SIZE                         (324u)
#define MCUXCLRANDOMMODES_UNINIT_WACPU_SIZE                           (0u)
#define MCUXCLRANDOMMODES_CHECKSECURITYSTRENGTH_WACPU_SIZE            (0u)
#define MCUXCLRANDOMMODES_NCINIT_WACPU_SIZE                           (0u)
#define MCUXCLRANDOMMODES_NCGENERATE_WACPU_SIZE                       (0u)
#define MCUXCLRANDOMMODES_CREATEPATCHMODE_WACPU_SIZE                  (0u)
#define MCUXCLRANDOMMODES_CREATETESTMODEFROMNORMALMODE_WACPU_SIZE     (0u)

#ifdef MCUXCL_FEATURE_RANDOMMODES_CTRDRBG



#ifdef MCUXCL_FEATURE_RANDOMMODES_SECSTRENGTH_256
#define MCUXCLRANDOMMODES_CTR_DRBG_AES256_CONTEXT_SIZE (64u)
#define MCUXCLRANDOMMODES_CTR_DRBG_AES256_CONTEXT_SIZE_IN_WORDS ((MCUXCLRANDOMMODES_CTR_DRBG_AES256_CONTEXT_SIZE + sizeof(uint32_t) - 1u) / sizeof(uint32_t))
#endif // MCUXCL_FEATURE_RANDOMMODES_SECSTRENGTH_256



#ifdef MCUXCL_FEATURE_RANDOMMODES_SECSTRENGTH_256
#define MCUXCLRANDOMMODES_TESTMODE_CTR_DRBG_AES256_INIT_ENTROPY_SIZE     (71u)
#define MCUXCLRANDOMMODES_TESTMODE_CTR_DRBG_AES256_RESEED_ENTROPY_SIZE   (55u)
#endif // MCUXCL_FEATURE_RANDOMMODES_SECSTRENGTH_256

#endif /* MCUXCL_FEATURE_RANDOMMODES_CTRDRBG */


/**
 * @}
 */ /* mcuxClRandomModes_MemoryConsumption */

#endif /* MCUXCLRANDOMMODES_MEMORYCONSUMPTION_H_ */
