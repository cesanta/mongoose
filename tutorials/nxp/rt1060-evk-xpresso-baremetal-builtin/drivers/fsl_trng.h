/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018, 2020-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _FSL_TRNG_DRIVER_H_
#define _FSL_TRNG_DRIVER_H_

#include "fsl_common.h"

#if defined(FSL_FEATURE_SOC_TRNG_COUNT) && FSL_FEATURE_SOC_TRNG_COUNT

/*!
 * @addtogroup trng
 * @{
 */

/*******************************************************************************
 * Definitions
 *******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief TRNG driver version 2.0.16.
 *
 * Current version: 2.0.16
 *

 * Change log:
 * - version 2.0.16
 *   - Added support for Dual oscillator mode.
 * - version 2.0.15
 *   - Changed TRNG_USER_CONFIG_DEFAULT_XXX values according to latest reccomended by design team.
 * - version 2.0.14
 *   - add support for RW610 and RW612
 * - version 2.0.13
 *   - After deepsleep it might return error, added clearing bits in TRNG_GetRandomData() and generating new entropy.
 *   - Modified reloading entropy in TRNG_GetRandomData(), for some data length it doesn't reloading entropy correctly.
 * - version 2.0.12
 *   - For KW34A4_SERIES, KW35A4_SERIES, KW36A4_SERIES set TRNG_USER_CONFIG_DEFAULT_OSC_DIV to kTRNG_RingOscDiv8.
 * - version 2.0.11
 *   - Add clearing pending errors in TRNG_Init().
 * - version 2.0.10
 *   - Fixed doxygen issues.
 * - version 2.0.9
 *   - Fix HIS_CCM metrics issues.
 * - version 2.0.8
 *   - For K32L2A41A_SERIES set TRNG_USER_CONFIG_DEFAULT_OSC_DIV to kTRNG_RingOscDiv4.
 * - version 2.0.7
 *   - Fix MISRA 2004 issue rule 12.5.
 * - version 2.0.6
 *   - For KW35Z4_SERIES set TRNG_USER_CONFIG_DEFAULT_OSC_DIV to kTRNG_RingOscDiv8.
 * - version 2.0.5
 *   - Add possibility to define default TRNG configuration by device specific preprocessor macros
 *     for FRQMIN, FRQMAX and OSCDIV.
 * - version 2.0.4
 *   - Fix MISRA-2012 issues.
 * - Version 2.0.3
 *   - update TRNG_Init to restart entropy generation
 * - Version 2.0.2
 *   - fix MISRA issues
 * - Version 2.0.1
 *   - add support for KL8x and KL28Z
 *   - update default OSCDIV for K81 to divide by 2
 */
#define FSL_TRNG_DRIVER_VERSION (MAKE_VERSION(2, 0, 16))
/*@}*/

/*! @brief TRNG sample mode. Used by trng_config_t. */
typedef enum _trng_sample_mode
{
    kTRNG_SampleModeVonNeumann = 0U, /*!< Use von Neumann data in both Entropy shifter and Statistical Checker. */
    kTRNG_SampleModeRaw        = 1U, /*!< Use raw data into both Entropy shifter and Statistical Checker. */
    kTRNG_SampleModeVonNeumannRaw =
        2U /*!< Use von Neumann data in Entropy shifter. Use raw data into Statistical Checker. */
} trng_sample_mode_t;

/*! @brief TRNG clock mode. Used by trng_config_t. */
typedef enum _trng_clock_mode
{
    kTRNG_ClockModeRingOscillator = 0U, /*!< Ring oscillator is used to operate the TRNG (default). */
    kTRNG_ClockModeSystem         = 1U  /*!< System clock is used to operate the TRNG. This is for test use only, and
                                           indeterminate results may occur. */
} trng_clock_mode_t;

/*! @brief TRNG ring oscillator divide. Used by trng_config_t. */
typedef enum _trng_ring_osc_div
{
    kTRNG_RingOscDiv0 = 0U, /*!< Ring oscillator with no divide */
    kTRNG_RingOscDiv2 = 1U, /*!< Ring oscillator divided-by-2. */
    kTRNG_RingOscDiv4 = 2U, /*!< Ring oscillator divided-by-4. */
    kTRNG_RingOscDiv8 = 3U  /*!< Ring oscillator divided-by-8. */
} trng_ring_osc_div_t;

#if defined(FSL_FEATURE_TRNG_HAS_DUAL_OSCILATORS) && (FSL_FEATURE_TRNG_HAS_DUAL_OSCILATORS > 0)
/*! @brief TRNG oscillator mode . Used by trng_config_t. */
typedef enum trng_oscillator_mode_t
{
    kTRNG_SingleOscillatorModeOsc1 = 0U, /*!< Single oscillator mode, using OSC1 (default)*/
    kTRNG_DualOscillatorMode       = 1U, /*!< Dual oscillator mode*/
    kTRNG_SingleOscillatorModeOsc2 = 2U, /*!< Single oscillator mode, using OSC2 */
} trng_oscillator_mode_t;
#endif /* FSL_FEATURE_TRNG_HAS_DUAL_OSCILATORS */

/*! @brief Data structure for definition of statistical check limits. Used by trng_config_t. */
typedef struct _trng_statistical_check_limit
{
    uint32_t maximum; /*!< Maximum limit.*/
    uint32_t minimum; /*!< Minimum limit.*/
} trng_statistical_check_limit_t;

/*!
 * @brief Data structure for the TRNG initialization
 *
 * This structure initializes the TRNG by calling the TRNG_Init() function.
 * It contains all TRNG configurations.
 */
typedef struct _trng_user_config
{
    bool lock;                      /*!< @brief Disable programmability of TRNG registers.  */
    trng_clock_mode_t clockMode;    /*!< @brief Clock mode used to operate TRNG.*/
    trng_ring_osc_div_t ringOscDiv; /*!< @brief Ring oscillator divide used by TRNG. */
    trng_sample_mode_t sampleMode;  /*!< @brief Sample mode of the TRNG ring oscillator. */
#if defined(FSL_FEATURE_TRNG_HAS_DUAL_OSCILATORS) && (FSL_FEATURE_TRNG_HAS_DUAL_OSCILATORS > 0)
    trng_oscillator_mode_t oscillatorMode; /*!< @brief TRNG oscillator mode . */
    trng_ring_osc_div_t ringOsc2Div;       /*!< @brief Divider used for Ring oscillator 2. */
#endif                                     /* FSL_FEATURE_TRNG_HAS_DUAL_OSCILATORS */
    /* Seed Control*/
    uint16_t
        entropyDelay; /*!< @brief Entropy Delay. Defines the length (in system clocks) of each Entropy sample taken. */
    uint16_t sampleSize; /*!< @brief Sample Size. Defines the total number of Entropy samples that will be taken during
                            Entropy generation. */
    uint16_t sparseBitLimit; /*!< @brief Sparse Bit Limit which defines the maximum number of
                              * consecutive samples that may be discarded before an error is generated.
                              * This limit is used only for during von Neumann sampling (enabled by
                              * TRNG_HAL_SetSampleMode()). Samples are discarded if two consecutive raw samples are both
                              * 0 or both 1. If this discarding occurs for a long period of time, it indicates that
                              * there is insufficient Entropy. */
    /* Statistical Check Parameters.*/
    uint8_t retryCount;      /*!< @brief Retry count. It defines the number of times a statistical check may fails
                              * during the TRNG Entropy Generation before generating an error. */
    uint8_t longRunMaxLimit; /*!< @brief Largest allowable number of consecutive samples of all 1, or all 0,
                              * that is allowed during the Entropy generation. */
    trng_statistical_check_limit_t monobitLimit; /*!< @brief Maximum and minimum limits for statistical check of number
                                                    of ones/zero detected during entropy generation. */
    trng_statistical_check_limit_t runBit1Limit; /*!< @brief Maximum and minimum limits for statistical check of number
                                                    of runs of length 1 detected during entropy generation. */
    trng_statistical_check_limit_t runBit2Limit; /*!< @brief Maximum and minimum limits for statistical check of number
                                                    of runs of length 2 detected during entropy generation. */
    trng_statistical_check_limit_t runBit3Limit; /*!< @brief Maximum and minimum limits for statistical check of number
                                                    of runs of length 3 detected during entropy generation. */
    trng_statistical_check_limit_t runBit4Limit; /*!< @brief Maximum and minimum limits for statistical check of number
                                                    of runs of length 4 detected during entropy generation. */
    trng_statistical_check_limit_t runBit5Limit; /*!< @brief Maximum and minimum limits for statistical check of number
                                                    of runs of length 5 detected during entropy generation. */
    trng_statistical_check_limit_t runBit6PlusLimit; /*!< @brief Maximum and minimum limits for statistical check of
                                                        number of runs of length 6 or more detected during entropy
                                                        generation. */
    trng_statistical_check_limit_t
        pokerLimit; /*!< @brief Maximum and minimum limits for statistical check of "Poker Test". */
    trng_statistical_check_limit_t frequencyCountLimit; /*!< @brief Maximum and minimum limits for statistical check of
                                                           entropy sample frequency count. */
} trng_config_t;

/*******************************************************************************
 * API
 *******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the user configuration structure to default values.
 *
 * This function initializes the configuration structure to default values. The default
 * values are as follows.
 * @code
 *     userConfig->lock = 0;
 *     userConfig->clockMode = kTRNG_ClockModeRingOscillator;
 *     userConfig->ringOscDiv = kTRNG_RingOscDiv0;  Or  to other kTRNG_RingOscDiv[2|8] depending on the platform.
 *     userConfig->sampleMode = kTRNG_SampleModeRaw;
 *     userConfig->entropyDelay = 3200;
 *     userConfig->sampleSize = 2500;
 *     userConfig->sparseBitLimit = 63;
 *     userConfig->retryCount = 1;
 *     userConfig->longRunMaxLimit = 32;
 *     userConfig->monobitLimit.maximum = 317;
 *     userConfig->monobitLimit.minimum = 195;
 *     userConfig->runBit1Limit.maximum = 107;
 *     userConfig->runBit1Limit.minimum = 27;
 *     userConfig->runBit2Limit.maximum = 62;
 *     userConfig->runBit2Limit.minimum = 7;
 *     userConfig->runBit3Limit.maximum = 39;
 *     userConfig->runBit3Limit.minimum = 0;
 *     userConfig->runBit4Limit.maximum = 26;
 *     userConfig->runBit4Limit.minimum = 0;
 *     userConfig->runBit5Limit.maximum = 18;
 *     userConfig->runBit5Limit.minimum = 0;
 *     userConfig->runBit6PlusLimit.maximum = 17;
 *     userConfig->runBit6PlusLimit.minimum = 0;
 *     userConfig->pokerLimit.maximum = 1600;
 *     userConfig->pokerLimit.minimum = 1030;
 *     userConfig->frequencyCountLimit.maximum = 30000;
 *     userConfig->frequencyCountLimit.minimum = 1600;
 * @endcode
 *
 * @param userConfig   User configuration structure.
 * @return If successful, returns the kStatus_TRNG_Success. Otherwise, it returns an error.
 */
status_t TRNG_GetDefaultConfig(trng_config_t *userConfig);

/*!
 * @brief Initializes the TRNG.
 *
 * This function initializes the TRNG.
 * When called, the TRNG entropy generation starts immediately.
 *
 * @param base  TRNG base address
 * @param userConfig    Pointer to the initialization configuration structure.
 * @return If successful, returns the kStatus_TRNG_Success. Otherwise, it returns an error.
 */
status_t TRNG_Init(TRNG_Type *base, const trng_config_t *userConfig);

/*!
 * @brief Shuts down the TRNG.
 *
 * This function shuts down the TRNG.
 *
 * @param base  TRNG base address.
 */
void TRNG_Deinit(TRNG_Type *base);

/*!
 * @brief Gets random data.
 *
 * This function gets random data from the TRNG.
 *
 * @param base  TRNG base address.
 * @param data  Pointer address used to store random data.
 * @param dataSize  Size of the buffer pointed by the data parameter.
 * @return random data
 */
status_t TRNG_GetRandomData(TRNG_Type *base, void *data, size_t dataSize);

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* FSL_FEATURE_SOC_TRNG_COUNT */
#endif /*_FSL_TRNG_H_*/
