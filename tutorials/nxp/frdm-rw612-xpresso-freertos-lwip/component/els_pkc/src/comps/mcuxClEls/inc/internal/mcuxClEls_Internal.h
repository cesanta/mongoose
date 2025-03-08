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

/** @file  mcuxClEls_Internal.h
 *  @brief Provide macros for mcuxClEls internal use.
 * This header declares internal macros to deduplicate code and support for internal use only. */

#ifndef MCUXCLELS_INTERNAL_H_
#define MCUXCLELS_INTERNAL_H_

#include <mcuxClCore_Platform.h>
#include <platform_specific_headers.h>
#include <stdint.h>
#include <stdbool.h>
#include <mcuxCsslFlowProtection.h>

#include <internal/mcuxClEls_Internal_mapping.h>
#include <internal/mcuxClEls_SfrAccess.h>
#include <mcuxCsslAnalysis.h>
#include <mcuxClEls_Common.h>
#include <mcuxClEls_Types.h>

#ifdef __cplusplus
extern "C" {
#endif

/****                                ****/
/**** ELS Hardware Abstraction Layer ****/
/****                                ****/


/** Asserts the correctness of the supplied parameters*/
#define MCUXCLELS_INPUT_PARAM_CHECK(x_) if((x_)) { return MCUXCLELS_STATUS_SW_INVALID_PARAM; }
#define MCUXCLELS_INPUT_PARAM_CHECK_PROTECTED(funcid, x_)                        \
do                                                                              \
{                                                                               \
    if ((x_))                                                                   \
    {                                                                           \
        MCUX_CSSL_FP_FUNCTION_EXIT(funcid, MCUXCLELS_STATUS_SW_INVALID_PARAM);    \
    }                                                                           \
MCUX_CSSL_ANALYSIS_START_PATTERN_BOOLEAN_TYPE_FOR_CONDITIONAL_EXPRESSION()       \
} while (false)                                                                 \
MCUX_CSSL_ANALYSIS_STOP_PATTERN_BOOLEAN_TYPE_FOR_CONDITIONAL_EXPRESSION()

#define ELS_CMD_BIG_ENDIAN ((uint8_t) 0x01U)    ///< ELS command option specifying big-endian byte order
#define ELS_CMD_LITTLE_ENDIAN ((uint8_t) 0x00U) ///< ELS command option specifying little-endian byte order

// Utility code of mcuxClEls implementation

/** Sets the variable-size input buffer from which the input 0 of the ELS operation will be transferred via DMA. */
static inline void mcuxClEls_setInput0(const uint8_t *pInput, uint32_t inputSize)
{
    MCUX_CSSL_ANALYSIS_START_PATTERN_STRING_NOT_MODIFIED()
    MCUXCLELS_SFR_WRITE(ELS_DMA_SRC0,     MCUXCL_HW_DMA_WORKAROUND(pInput));
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_STRING_NOT_MODIFIED()
    MCUXCLELS_SFR_WRITE(ELS_DMA_SRC0_LEN, inputSize);
}

/** Sets the fixed-size input buffer from which the input 0 of the ELS operation will be transferred via DMA. */
static inline void mcuxClEls_setInput0_fixedSize(const uint8_t *pInput)
{
    MCUX_CSSL_ANALYSIS_START_PATTERN_STRING_NOT_MODIFIED()
    MCUXCLELS_SFR_WRITE(ELS_DMA_SRC0, MCUXCL_HW_DMA_WORKAROUND(pInput));
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_STRING_NOT_MODIFIED()
}

/** Sets the fixed-size input buffer from which the input 1 of the ELS operation will be transferred via DMA. */
static inline void mcuxClEls_setInput1_fixedSize(const uint8_t *pInput)
{
    MCUX_CSSL_ANALYSIS_START_PATTERN_STRING_NOT_MODIFIED()
    MCUXCLELS_SFR_WRITE(ELS_DMA_SRC1, MCUXCL_HW_DMA_WORKAROUND(pInput));
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_STRING_NOT_MODIFIED()
}

/** Sets the variable-size input buffer from which the input 2 of the ELS operation will be transferred via DMA. */
static inline void mcuxClEls_setInput2(const uint8_t *pInput, uint32_t inputSize)
{
    MCUX_CSSL_ANALYSIS_START_PATTERN_STRING_NOT_MODIFIED()
    MCUXCLELS_SFR_WRITE(ELS_DMA_SRC2,     MCUXCL_HW_DMA_WORKAROUND(pInput));
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_STRING_NOT_MODIFIED()
    MCUXCLELS_SFR_WRITE(ELS_DMA_SRC2_LEN, inputSize);
}

/** Sets the fixed-size input buffer from which the input 2 of the ELS operation will be transferred via DMA. */
static inline void mcuxClEls_setInput2_fixedSize(const uint8_t * pInput)
{
    MCUX_CSSL_ANALYSIS_START_PATTERN_STRING_NOT_MODIFIED()
    MCUXCLELS_SFR_WRITE(ELS_DMA_SRC2, MCUXCL_HW_DMA_WORKAROUND(pInput));
    MCUX_CSSL_ANALYSIS_STOP_PATTERN_STRING_NOT_MODIFIED()
}

/** Sets the fixed-size input offset from which the input 2 of the ELS operation will be transferred via DMA. */
static inline void mcuxClEls_setInput2Offset_fixedSize(uint32_t inputOffset)
{
    MCUXCLELS_SFR_WRITE(ELS_DMA_SRC2, inputOffset);
}

/** Sets the variable-size output buffer to which the result of the ELS operation will be transferred via DMA. */
static inline void mcuxClEls_setOutput(uint8_t *pOutput, uint32_t outputSize)
{
    MCUXCLELS_SFR_WRITE(ELS_DMA_RES0,     MCUXCL_HW_DMA_WORKAROUND(pOutput));
    MCUXCLELS_SFR_WRITE(ELS_DMA_RES0_LEN, outputSize);
}

/** Sets the output buffer to which the result of the ELS operation will be transferred via DMA. */
static inline void mcuxClEls_setOutput_fixedSize(uint8_t *pOutput)
{
    MCUXCLELS_SFR_WRITE(ELS_DMA_RES0, MCUXCL_HW_DMA_WORKAROUND(pOutput));
}

/** Sets the ELS keystore index 0, for commands that access a single key. */
static inline void mcuxClEls_setKeystoreIndex0(uint32_t index)
{
    MCUXCLELS_SFR_WRITE(ELS_KIDX0, index);
}


/** Sets the ELS keystore index 1, for commands that access 2 keys. */
static inline void mcuxClEls_setKeystoreIndex1(uint32_t index)
{
    MCUXCLELS_SFR_WRITE(ELS_KIDX1, index);
}

#ifdef MCUXCL_FEATURE_ELS_PUK_INTERNAL
/** Sets the ELS keystore index 2, for commands that access 3 keys. */
static inline void mcuxClEls_setKeystoreIndex2(uint32_t index)
{
    MCUXCLELS_SFR_WRITE(ELS_KIDX2, index);
}
#endif /* MCUXCL_FEATURE_ELS_PUK_INTERNAL */

/** Sets the ELS requested key properties, for commands that create a key. */
static inline void mcuxClEls_setRequestedKeyProperties(uint32_t properties)
{
    MCUXCLELS_SFR_WRITE(ELS_KPROPIN, properties);
}

/** Starts an ELS command. */
static inline void mcuxClEls_startCommand(uint32_t command, uint32_t cmdcfg0, uint32_t byteOrder)
{
    uint32_t ctrl = MCUXCLELS_SFR_FIELD_FORMAT(ELS_CTRL, ELS_CMD, command)
                    | MCUXCLELS_SFR_FIELD_FORMAT(ELS_CTRL, ELS_START, 1u)
                    | MCUXCLELS_SFR_FIELD_FORMAT(ELS_CTRL, ELS_EN, 1u)
                    | MCUXCLELS_SFR_FIELD_FORMAT(ELS_CTRL, BYTE_ORDER, byteOrder);

    MCUXCLELS_SFR_WRITE(ELS_CMDCFG0, cmdcfg0);
    MCUXCLELS_SFR_WRITE(ELS_CTRL,    ctrl);
}


/** Gets a specific field in the given SFR value, according to the given mask and shift value.
 *  @retval @c value of the requested field in the given ELS SFR value */
static inline uint32_t mcuxClEls_getSfrField(uint32_t sfrValue, uint32_t mask, uint32_t shift)
{
    return ((uint32_t)(sfrValue & mask) >> shift);
}

/** Set a specific field in the given SFR value, according to the given mask and shift value.
  * The unrelated fields/bits will not be changed */
static inline void mcuxClEls_setSfrField(volatile uint32_t *pSfr, uint32_t value, uint32_t mask, uint32_t shift)
{
  MCUX_CSSL_ANALYSIS_START_SUPPRESS_HARDWARE_ACCESS("Sfr offset from address")
	/* get the current value of the SFR and clear the bits that will be set */
  uint32_t sfrValue = *pSfr & (~mask);
	/* set the bits and re-write the full value to the SFR */
  *pSfr = sfrValue | (((uint32_t)(value << shift)) & mask);
  MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_HARDWARE_ACCESS()
}

/** Tests if the ELS is in BUSY state.
 *  @retval @c true if the ELS is in BUSY state */
static inline bool mcuxClEls_isBusy(void)
{
    return (0u != MCUXCLELS_SFR_BITREAD(ELS_STATUS, ELS_BUSY) );
}


/** Macros to access the bit fields for the ELS_STATUS SFR */
#define MCUXCLELS_SFR_STATUS_ELS_BUSY            ELS_BUSY
#define MCUXCLELS_SFR_STATUS_ELS_IRQ             ELS_IRQ
#define MCUXCLELS_SFR_STATUS_ELS_ERR             ELS_ERR
#define MCUXCLELS_SFR_STATUS_PRNG_RDY            PRNG_RDY
#define MCUXCLELS_SFR_STATUS_ECDSA_VFY_STATUS    ECDSA_VFY_STATUS
#define MCUXCLELS_SFR_STATUS_PPROT               PPROT
#define MCUXCLELS_SFR_STATUS_DRBG_ENT_LVL        DRBG_ENT_LVL
#define MCUXCLELS_SFR_STATUS_DTRNG_BUSY          DTRNG_BUSY
#define MCUXCLELS_SFR_STATUS_ELS_LOCKED          ELS_LOCKED

/** Gets a specific field in the ELS_STATUS SFR.
 *  @param field: Any field name in MCUXCLELS_SFR_STATUS_* */
#define MCUXCLELS_GET_STATUS_FIELD(field) \
  mcuxClEls_getSfrField(MCUXCLELS_SFR_READ(ELS_STATUS), MCUXCLELS_SFR_FIELD_MASK(ELS_STATUS, field), MCUXCLELS_SFR_FIELD_SHIFT(ELS_STATUS, field))


/** Macros to access the bit fields for the ELS_CTRL SFR */
#define MCUXCLELS_SFR_CTRL_ELS_EN                ELS_EN
#define MCUXCLELS_SFR_CTRL_START                 ELS_START
#define MCUXCLELS_SFR_CTRL_RESET                 ELS_RESET
#define MCUXCLELS_SFR_CTRL_CMD                   ELS_CMD
#define MCUXCLELS_SFR_CTRL_BYTE_ORDER            BYTE_ORDER

/** Gets a specific field in the ELS_CTRL SFR.
 *  @param field: Any field name in MCUXCLELS_SFR_CTRL_* */
#define MCUXCLELS_GET_CTRL_FIELD(field) \
  mcuxClEls_getSfrField(MCUXCLELS_SFR_READ(ELS_CTRL), MCUXCLELS_SFR_FIELD_MASK(ELS_CTRL, field), MCUXCLELS_SFR_FIELD_SHIFT(ELS_CTRL, field))

/** Sets a specific field in the ELS_CTRL SFR. The unrelated fields/bits will not be changed
 *  @param field: Any field name in MCUXCLELS_SFR_CTRL_*
 *  @param value: The value to set the requested SFR field to */
#define MCUXCLELS_SET_CTRL_FIELD(field, value) \
  mcuxClEls_setSfrField(&MCUXCLELS_SFR_READ(ELS_CTRL), (value), MCUXCLELS_SFR_FIELD_MASK(ELS_CTRL, field), MCUXCLELS_SFR_FIELD_SHIFT(ELS_CTRL, field))


/** Macros to access the bit fields for the ELS_CFG SFR */
#define MCUXCLELS_SFR_CFG_ADCTRL                 ADCTRL
#define MCUXCLELS_SFR_CFG_SHA2_DIRECT            SHA2_DIRECT

/** Gets a specific field in the ELS_CFG SFR.
 *  @param field: Any field name in MCUXCLELS_SFR_CFG_* */
#define MCUXCLELS_GET_CFG_FIELD(field) \
  mcuxClEls_getSfrField(MCUXCLELS_SFR_READ(ELS_CFG), MCUXCLELS_SFR_FIELD_MASK(ELS_CFG, field), MCUXCLELS_SFR_FIELD_SHIFT(ELS_CFG, field))

/** Sets a specific field in the ELS_CFG SFR. The unrelated fields/bits will not be changed
 *  @param field: Any field name in MCUXCLELS_SFR_CFG_*
 *  @param value: The value to set the requested SFR field to */
#define MCUXCLELS_SET_CFG_FIELD(field, value) \
  mcuxClEls_setSfrField(&MCUXCLELS_SFR_READ(ELS_CFG), (value), MCUXCLELS_SFR_FIELD_MASK(ELS_CFG, field), MCUXCLELS_SFR_FIELD_SHIFT(ELS_CFG, field))


/** Macros to access the bit fields for the ELS_ERR_STATUS SFR */
#define MCUXCLELS_SFR_ERR_STATUS_BUS_ERR         BUS_ERR
#define MCUXCLELS_SFR_ERR_STATUS_OPN_ERR         OPN_ERR
#define MCUXCLELS_SFR_ERR_STATUS_ALG_ERR         ALG_ERR
#define MCUXCLELS_SFR_ERR_STATUS_ITG_ERR         ITG_ERR
#define MCUXCLELS_SFR_ERR_STATUS_FLT_ERR         FLT_ERR
#define MCUXCLELS_SFR_ERR_STATUS_PRNG_ERR        PRNG_ERR
#define MCUXCLELS_SFR_ERR_STATUS_ERR_LVL         ERR_LVL
#define MCUXCLELS_SFR_ERR_STATUS_DTRNG_ERR       DTRNG_ERR

/** Gets a specific field in the ELS_ERR_STATUS SFR.
 *  @param field: Any field name in MCUXCLELS_SFR_ERR_STATUS_* */
#define MCUXCLELS_GET_ERROR_STATUS_FIELD(field) \
  mcuxClEls_getSfrField(MCUXCLELS_SFR_READ(ELS_ERR_STATUS), MCUXCLELS_SFR_FIELD_MASK(ELS_ERR_STATUS, field), MCUXCLELS_SFR_FIELD_SHIFT(ELS_ERR_STATUS, field))

/** Checks if a specific error bit in the ELS_ERR_STATUS SFR is set.
 *  @retval @c true if the requested ELS error status bit is set */
#define MCUXCLELS_IS_ERROR_BIT_SET(field) \
    (1u == MCUXCLELS_GET_ERROR_STATUS_FIELD(field))


/** Macros to access the bit fields for the ELS_CMDCRC_CTRL SFR */
#define MCUXCLELS_SFR_CMDCRC_CTRL_CMDCRC_RST     CMDCRC_RST
#define MCUXCLELS_SFR_CMDCRC_CTRL_CMDCRC_EN      CMDCRC_EN

/** Sets a specific field in the ELS_CMDCRC_CTRL SFR. The unrelated fields/bits will not be changed
 *  @param field: Any field name in MCUXCLELS_SFR_CMDCRC_CTRL_*
 *  @param value: The value to set the requested SFR field to */
#define MCUXCLELS_SET_CMDCRC_CTRL_FIELD(field, value) \
  mcuxClEls_setSfrField(&MCUXCLELS_SFR_READ(ELS_CMDCRC_CTRL), (value), MCUXCLELS_SFR_FIELD_MASK(ELS_CMDCRC_CTRL, field), MCUXCLELS_SFR_FIELD_SHIFT(ELS_CMDCRC_CTRL, field))

/** Macros to access the bit fields for the ELS_SHA2_CTRL SFR */
#define MCUXCLELS_SFR_SHA2_CTRL_SHA2_START       SHA2_START
#define MCUXCLELS_SFR_SHA2_CTRL_SHA2_RST         SHA2_RST
#define MCUXCLELS_SFR_SHA2_CTRL_SHA2_INIT        SHA2_INIT
#define MCUXCLELS_SFR_SHA2_CTRL_SHA2_LOAD        SHA2_LOAD
#define MCUXCLELS_SFR_SHA2_CTRL_SHA2_MODE        SHA2_MODE
#define MCUXCLELS_SFR_SHA2_CTRL_SHA2_BYTE_ORDER  SHA2_BYTE_ORDER

/** Gets a specific field in the ELS_SHA2_CTRL SFR.
 *  @param field: Any field name in MCUXCLELS_SFR_SHA2_CTRL_* */
#define MCUXCLELS_GET_SHA2_CTRL_FIELD(field) \
  mcuxClEls_getSfrField(MCUXCLELS_SFR_READ(ELS_SHA2_CTRL), MCUXCLELS_SFR_FIELD_MASK(ELS_SHA2_CTRL, field), MCUXCLELS_SFR_FIELD_SHIFT(ELS_SHA2_CTRL, field))

/** Sets a specific field in the ELS_SHA2_CTRL SFR. The unrelated fields/bits will not be changed
 *  @param field: Any field name in MCUXCLELS_SFR_SHA2_CTRL_*
 *  @param value: The value to set the requested SFR field to */
#define MCUXCLELS_SET_SHA2_CTRL_FIELD(field, value) \
  mcuxClEls_setSfrField(&MCUXCLELS_SFR_READ(ELS_SHA2_CTRL), (value), MCUXCLELS_SFR_FIELD_MASK(ELS_SHA2_CTRL, field), MCUXCLELS_SFR_FIELD_SHIFT(ELS_SHA2_CTRL, field))


/** Macro to access the bit fields for the ELS_SHA2_STATUS SFR */
#define MCUXCLELS_SFR_SHA2_STATUS_SHA2_BUSY      SHA2_BUSY

/** Gets a specific field in the ELS_SHA2_STATUS SFR.
 *  @param field: Any field name in MCUXCLELS_SFR_SHA2_STATUS_* */
#define MCUXCLELS_GET_SHA2_STATUS_FIELD(field) \
  mcuxClEls_getSfrField(MCUXCLELS_SFR_READ(ELS_SHA2_STATUS), MCUXCLELS_SFR_FIELD_MASK(ELS_SHA2_STATUS, field), MCUXCLELS_SFR_FIELD_SHIFT(ELS_SHA2_STATUS, field))

#ifdef MCUXCL_FEATURE_ELS_GLITCHDETECTOR
/**
 * Macros to access the bit fields for the ELS_GDET_EVTCNT SFR
 * */
#define MCUXCLELS_SFR_GDET_EVTCNT_GDET_EVTCNT            GDET_EVTCNT
#define MCUXCLELS_SFR_GDET_EVTCNT_GDET_EVTCNT_CLR_DONE   GDET_EVTCNT_CLR_DONE

/** Gets a specific field in ELS_GDET_EVTCNT SFR.
 *  @param field: Any field name in MCUXCLELS_SFR_GDET_EVTCNT_* */
#define MCUXCLELS_GET_GDET_EVTCNT_FIELD(field) \
  mcuxClEls_getSfrField(MCUXCLELS_SFR_READ(ELS_GDET_EVTCNT), MCUXCLELS_SFR_FIELD_MASK(ELS_GDET_EVTCNT, field), MCUXCLELS_SFR_FIELD_SHIFT(ELS_GDET_EVTCNT, field))

#endif /* MCUXCL_FEATURE_ELS_GLITCHDETECTOR */

/** Macros to access the bit fields for the ELS_INT_ENABLE SFR */
#define MCUXCLELS_SFR_INT_ENABLE_INT_EN           INT_EN
#ifdef MCUXCL_FEATURE_ELS_GLITCHDETECTOR
#define MCUXCLELS_SFR_INT_ENABLE_GDET_INT_EN      GDET_INT_EN
#endif /* MCUXCL_FEATURE_ELS_GLITCHDETECTOR */

/** Gets a specific field in the ELS_INT_ENABLE SFR.
 *  @param field: Any field name in MCUXCLELS_SFR_INT_ENABLE_* */
#define MCUXCLELS_GET_INT_ENABLE_FIELD(field) \
  mcuxClEls_getSfrField(MCUXCLELS_SFR_READ(ELS_INT_ENABLE), MCUXCLELS_SFR_FIELD_MASK(ELS_INT_ENABLE, field), MCUXCLELS_SFR_FIELD_SHIFT(ELS_INT_ENABLE, field))


/* Total buffer size in output, which is used for cache maintenance */
#define MCUXCLELS_HASH_BUFFER_SIZE(options)  MCUXCLELS_HASH_BUFFER_SIZE_DIGEST(options) +  MCUXCLELS_HASH_BUFFER_SIZE_RTF(options)
#define MCUXCLELS_HASH_BUFFER_SIZE_RTF(options) ( (MCUXCLELS_HASH_RTF_OUTPUT_ENABLE == options.bits.rtfoe) ? MCUXCLELS_HASH_RTF_OUTPUT_SIZE : 0u )
#define MCUXCLELS_HASH_BUFFER_SIZE_DIGEST(options) ( (1u < options.bits.hashmd) ? MCUXCLELS_HASH_OUTPUT_SIZE_SHA_512 : MCUXCLELS_HASH_OUTPUT_SIZE_SHA_256 )

#ifdef MCUXCL_FEATURE_ELS_ITERATIVE_SEEDING
extern uint32_t mcuxClEls_rng_drbg_block_counter;

#define MCUXCLELS_RNG_DRBG_ITERATIVE_SEEDING_ITERATIONS  8u
#define MCUXCLELS_RNG_DRBG_BLOCK_COUNTER_THRESHOLD       4096u
#define MCUXCLELS_RNG_DRBG_ECCKEYGEN_INCREASE            10u
#define MCUXCLELS_RNG_DRBG_ECCSIGN_INCREASE              14u
#define MCUXCLELS_RNG_DRBG_ECCVERIFY_INCREASE            5u
#define MCUXCLELS_RNG_DRBG_KEYDELETE128_INCREASE         4u
#define MCUXCLELS_RNG_DRBG_KEYDELETE256_INCREASE         6u
#define MCUXCLELS_RNG_DRBG_DRBGREQUEST_INCREASE(outputLength) ((outputLength + 15u) / 16u)

/**
 * @brief This function resets the internal ELS DRBG block counter and reseeds the ELS DRBG
 *        using the iterative reseeding procedure
 *
 * @retval #MCUXCLELS_STATUS_SW_FAULT            if a failure occurred
 * @retval #MCUXCLELS_STATUS_OK                  on successful operation
 */
MCUX_CSSL_ANALYSIS_START_PATTERN_DESCRIPTIVE_IDENTIFIER()
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_Dtrng_IterativeReseeding_Reseed)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_Dtrng_IterativeReseeding_Reseed(const uint8_t *pDtrngConfig);
MCUX_CSSL_ANALYSIS_STOP_PATTERN_DESCRIPTIVE_IDENTIFIER()
#endif /* MCUXCL_FEATURE_ELS_ITERATIVE_SEEDING */


/**
 * @brief This function provides error handling for mcuxClEls_KeyExport_Async function
 *
 * @param[in]   pOutput              The memory address of the exported key which will be cleared
 * @param[in]   keyLength            The key length which will be cleared
 * @param[in]   interrupt_state_old  The interrupts state which will be restored
 *
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClEls_handleKeyExportError)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_handleKeyExportError(uint8_t *pOutput, size_t keyLength, mcuxClEls_InterruptOptionEn_t interrupt_state_old);

/* Functional macro to check for ELS Level 1 errors */
#define MCUXCLELS_LEVEL1_ERROR(returnCode) (MCUXCLELS_STATUS_HW_OPERATIONAL == (returnCode)) || (MCUXCLELS_STATUS_HW_ALGORITHM == (returnCode)) || (MCUXCLELS_STATUS_HW_BUS == (returnCode))

/** read from ELS PRNG SFR. */
static inline uint32_t mcuxClEls_readPrngOut(void)
{
  return MCUXCLELS_SFR_READ(ELS_PRNG_DATOUT);
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLELS_INTERNAL_H_ */
