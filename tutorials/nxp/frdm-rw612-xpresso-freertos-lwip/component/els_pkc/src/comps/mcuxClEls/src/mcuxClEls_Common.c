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

/** @file  mcuxClEls_Common.c
 *  @brief ELS implementation for common functionality.
 *  This file implements the functions declared in mcuxClEls_Common.h and adds helper functions used by other implementation headers. */

#include <stdbool.h>
#include <platform_specific_headers.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h>
#include <mcuxClEls.h>
#include <internal/mcuxClEls_Internal.h>
#include <internal/mcuxClEls_Internal_Common.h>

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_GetHwVersion)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GetHwVersion(
    mcuxClEls_HwVersion_t * result)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_GetHwVersion);
    result->word.value = MCUXCLELS_SFR_READ(ELS_VERSION);
    uint32_t minorVersion = result->bits.minor;
    /* Encode in decimal representation */
    minorVersion = (minorVersion & 0xFu) + (((minorVersion >> 4u) & 0xFu) * 10u);
    result->bits.minor = (uint8_t)minorVersion;
#ifdef MCUXCL_FEATURE_ELS_GET_FW_VERSION
    uint32_t fwMinorVersion = result->bits.fw_minor;
    /* Encode in decimal representation */
    fwMinorVersion = (fwMinorVersion & 0xFu) + (((fwMinorVersion >> 4u) & 0xFu) * 10u);
    result->bits.fw_minor = (uint8_t)fwMinorVersion;
#endif // MCUXCL_FEATURE_ELS_GET_FW_VERSION
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_GetHwVersion, MCUXCLELS_STATUS_OK);
}

#ifdef MCUXCL_FEATURE_ELS_HWCONFIG
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_GetHwConfig)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GetHwConfig(
    mcuxClEls_HwConfig_t * result)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_GetHwConfig);
    result->word.value = MCUXCLELS_SFR_READ(ELS_CONFIG);
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_GetHwConfig, MCUXCLELS_STATUS_OK);
}
#endif /* MCUXCL_FEATURE_ELS_HWCONFIG */

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_GetHwState)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GetHwState(
    mcuxClEls_HwState_t * result)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_GetHwState);
    result->word.value = MCUXCLELS_SFR_READ(ELS_STATUS);
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_GetHwState, MCUXCLELS_STATUS_OK);
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_Enable_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_Enable_Async(
    void)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_Enable_Async);
    const uint32_t sfrVal =  MCUXCLELS_SFR_FIELD_FORMAT(ELS_CTRL, ELS_EN, 1u);
    MCUXCLELS_SFR_WRITE(ELS_CTRL, sfrVal);
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_Enable_Async, MCUXCLELS_STATUS_OK_WAIT);
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_Disable)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_Disable(
    void)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_Disable);
    MCUXCLELS_SET_CTRL_FIELD(MCUXCLELS_SFR_CTRL_ELS_EN, 0u);
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_Disable, MCUXCLELS_STATUS_OK);
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_GetErrorCode)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GetErrorCode(
    mcuxClEls_ErrorHandling_t errorHandling)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_GetErrorCode);

    mcuxClEls_Status_t result = MCUXCLELS_STATUS_SW_FAULT;
    if (1U == MCUXCLELS_GET_STATUS_FIELD(MCUXCLELS_SFR_STATUS_ELS_ERR))
    {
        if (MCUXCLELS_IS_ERROR_BIT_SET(MCUXCLELS_SFR_ERR_STATUS_FLT_ERR))
        {
            result = MCUXCLELS_STATUS_HW_FAULT;
        }
        else if (MCUXCLELS_IS_ERROR_BIT_SET(MCUXCLELS_SFR_ERR_STATUS_ITG_ERR))
        {
            result = MCUXCLELS_STATUS_HW_INTEGRITY;
        }
        else if (MCUXCLELS_IS_ERROR_BIT_SET(MCUXCLELS_SFR_ERR_STATUS_OPN_ERR))
        {
            result = MCUXCLELS_STATUS_HW_OPERATIONAL;
        }
        else if (MCUXCLELS_IS_ERROR_BIT_SET(MCUXCLELS_SFR_ERR_STATUS_ALG_ERR))
        {
            result = MCUXCLELS_STATUS_HW_ALGORITHM;
        }
        else if (MCUXCLELS_IS_ERROR_BIT_SET(MCUXCLELS_SFR_ERR_STATUS_BUS_ERR))
        {
            result = MCUXCLELS_STATUS_HW_BUS;
        }
        else if (MCUXCLELS_IS_ERROR_BIT_SET(MCUXCLELS_SFR_ERR_STATUS_PRNG_ERR))
        {
            result = MCUXCLELS_STATUS_HW_PRNG;
        }
        else if (MCUXCLELS_IS_ERROR_BIT_SET(MCUXCLELS_SFR_ERR_STATUS_DTRNG_ERR))
        {
            result = MCUXCLELS_STATUS_HW_DTRNG;
        }
        else
        {
            result = MCUXCLELS_STATUS_SW_FAULT;
        }
    }
    else
    {
        /* Double check, should result in addition of zero in normal case. */
        MCUX_CSSL_SC_ADD(MCUXCLELS_GET_STATUS_FIELD(MCUXCLELS_SFR_STATUS_ELS_ERR));

        result = MCUXCLELS_STATUS_OK;
    }

    if (MCUXCLELS_ERROR_FLAGS_CLEAR == errorHandling){
        MCUX_CSSL_FP_FUNCTION_CALL_VOID(mcuxClEls_ResetErrorFlags()); /* always returns MCUXCLELS_STATUS_OK. */

        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_GetErrorCode, result,
            MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_ResetErrorFlags));
    }

    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_GetErrorCode, result);
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_GetErrorLevel)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GetErrorLevel(
    mcuxClEls_ErrorHandling_t errorHandling,
    uint32_t *errorLevel)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_GetErrorLevel);

    *errorLevel = MCUXCLELS_GET_ERROR_STATUS_FIELD(MCUXCLELS_SFR_ERR_STATUS_ERR_LVL);

    MCUX_CSSL_FP_FUNCTION_CALL(result, mcuxClEls_GetErrorCode(errorHandling));

    /* Exit function with expectation: mcuxClEls_GetErrorCode was called unconditionally */
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_GetErrorLevel, result,
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_GetErrorCode));
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_WaitForOperation)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_WaitForOperation(
    mcuxClEls_ErrorHandling_t errorHandling)
{
    /* Enter flow-protected function with expectation: mcuxClEls_GetErrorCode will be called (unconditionally) */
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_WaitForOperation,
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_GetErrorCode));

    while (mcuxClEls_isBusy())
    {
        // Do nothing
    }

    /* Double check, should result in addition of zero in normal case. */
    MCUX_CSSL_SC_ADD(MCUXCLELS_GET_STATUS_FIELD(MCUXCLELS_SFR_STATUS_ELS_BUSY));

    MCUX_CSSL_FP_FUNCTION_CALL(result, mcuxClEls_GetErrorCode(errorHandling));

    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_WaitForOperation, result);
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_LimitedWaitForOperation)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_LimitedWaitForOperation(
    uint32_t counterLimit,
    mcuxClEls_ErrorHandling_t errorHandling)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_LimitedWaitForOperation);

    bool counterExpired = true;
    while (0U != counterLimit)
    {
        if (!mcuxClEls_isBusy())
        {
            counterExpired = false;
            break;
        }
        counterLimit--;
    }

    if (true == counterExpired)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_LimitedWaitForOperation, MCUXCLELS_STATUS_SW_COUNTER_EXPIRED);
    }

    /* Double check, should result in addition of zero in normal case. */
    MCUX_CSSL_SC_ADD(MCUXCLELS_SFR_BITREAD(ELS_STATUS, ELS_BUSY));

    MCUX_CSSL_FP_FUNCTION_CALL(result, mcuxClEls_GetErrorCode(errorHandling));

    /* Exit function with expectation: mcuxClEls_GetErrorCode was called */
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_LimitedWaitForOperation, result,
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_GetErrorCode));
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_ResetErrorFlags)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_ResetErrorFlags(
    void)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_ResetErrorFlags);
    const uint32_t sfrVal = MCUXCLELS_SFR_FIELD_FORMAT(ELS_ERR_STATUS, CLR_ERR_CLR, MCUXCLELS_ERROR_FLAGS_CLEAR);
    MCUXCLELS_SFR_WRITE(ELS_ERR_STATUS_CLR, sfrVal);
    // Poll error bit to be sure that error bits has been cleared. Required by HW spec.
    while(0u != MCUXCLELS_GET_STATUS_FIELD(MCUXCLELS_SFR_STATUS_ELS_ERR))
    {
        // Do nothing
    }
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_ResetErrorFlags, MCUXCLELS_STATUS_OK);
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_Reset_Async)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_Reset_Async(
    mcuxClEls_ResetOption_t options)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_Reset_Async);

    if (mcuxClEls_isBusy() && (MCUXCLELS_RESET_DO_NOT_CANCEL == options))
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_Reset_Async, MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT);
    }

#ifdef MCUXCL_FEATURE_ELS_ITERATIVE_SEEDING
    /* Set the drbg_block_counter to a value triggering a reseed after the upcoming RESET operation via interrupt */
    mcuxClEls_rng_drbg_block_counter = MCUXCLELS_RNG_DRBG_BLOCK_COUNTER_THRESHOLD;
#endif /* MCUXCL_FEATURE_ELS_ITERATIVE_SEEDING */

    MCUXCLELS_SET_CTRL_FIELD(MCUXCLELS_SFR_CTRL_RESET, 1u);

    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_Reset_Async, MCUXCLELS_STATUS_OK_WAIT);
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_SetIntEnableFlags)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_SetIntEnableFlags(
    mcuxClEls_InterruptOptionEn_t options)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_SetIntEnableFlags);
    MCUXCLELS_SFR_WRITE(ELS_INT_ENABLE, options.word.value);
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_SetIntEnableFlags, MCUXCLELS_STATUS_OK);
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_GetIntEnableFlags)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GetIntEnableFlags(
    mcuxClEls_InterruptOptionEn_t * result)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_GetIntEnableFlags);
    result->word.value = MCUXCLELS_SFR_READ(ELS_INT_ENABLE);
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_GetIntEnableFlags, MCUXCLELS_STATUS_OK);
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_ResetIntFlags)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_ResetIntFlags(
    mcuxClEls_InterruptOptionRst_t options)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_ResetIntFlags);
    MCUXCLELS_SFR_WRITE(ELS_INT_STATUS_CLR, options.word.value);
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_ResetIntFlags, MCUXCLELS_STATUS_OK);
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_SetIntFlags)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_SetIntFlags(
    mcuxClEls_InterruptOptionSet_t options)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_SetIntFlags);
    MCUXCLELS_SFR_WRITE(ELS_INT_STATUS_SET, options.word.value);
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_SetIntFlags, MCUXCLELS_STATUS_OK);
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_SetRandomStartDelay)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_SetRandomStartDelay(
    uint32_t startDelay)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_SetRandomStartDelay);
    MCUXCLELS_INPUT_PARAM_CHECK_PROTECTED(mcuxClEls_SetRandomStartDelay, 1024u < startDelay);

    if (mcuxClEls_isBusy())
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_SetRandomStartDelay, MCUXCLELS_STATUS_SW_CANNOT_INTERRUPT);
    }

    MCUXCLELS_SET_CFG_FIELD(MCUXCLELS_SFR_CFG_ADCTRL, startDelay);

    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_SetRandomStartDelay, MCUXCLELS_STATUS_OK);
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_GetRandomStartDelay)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GetRandomStartDelay(
    uint32_t *startDelay)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_GetRandomStartDelay);

    *startDelay = MCUXCLELS_GET_CFG_FIELD(MCUXCLELS_SFR_CFG_ADCTRL);

    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_GetRandomStartDelay, MCUXCLELS_STATUS_OK);
}

#ifdef MCUXCL_FEATURE_ELS_LOCKING
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_GetLock)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GetLock(
    uint32_t * pSessionId)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_GetLock);

    *pSessionId = MCUXCLELS_SFR_READ(ELS_SESSION_ID);
    if(0u == *pSessionId)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_GetLock, MCUXCLELS_STATUS_SW_LOCKING_FAILED);
    }

    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_GetLock, MCUXCLELS_STATUS_OK);
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_ReleaseLock)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_ReleaseLock(
    uint32_t sessionId)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_ReleaseLock);
    MCUXCLELS_SFR_WRITE(ELS_SESSION_ID, sessionId);
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_ReleaseLock, MCUXCLELS_STATUS_OK);
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_IsLocked)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_IsLocked(
    void)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_IsLocked);

    if(1u == MCUXCLELS_GET_STATUS_FIELD(MCUXCLELS_SFR_STATUS_ELS_LOCKED))
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_IsLocked, MCUXCLELS_STATUS_SW_STATUS_LOCKED);
    }

    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_IsLocked, MCUXCLELS_STATUS_OK);
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_SetMasterUnlock)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_SetMasterUnlock(
    uint32_t masterId)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_SetMasterUnlock);
    MCUXCLELS_SFR_WRITE(ELS_MASTER_ID, masterId);
    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_SetMasterUnlock, MCUXCLELS_STATUS_OK);
}
#endif /* MCUXCL_FEATURE_ELS_LOCKING */


#ifdef MCUXCL_FEATURE_ELS_DMA_ADDRESS_READBACK
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_GetLastDmaAddress)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_GetLastDmaAddress(uint32_t* pLastAddress)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_GetLastDmaAddress);

    *pLastAddress = MCUXCLELS_SFR_READ(ELS_DMA_FIN_ADDR);

    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_GetLastDmaAddress, MCUXCLELS_STATUS_OK);

}
#endif /* MCUXCL_FEATURE_ELS_DMA_ADDRESS_READBACK */

#ifdef MCUXCL_FEATURE_ELS_DMA_FINAL_ADDRESS_READBACK
MCUX_CSSL_FP_FUNCTION_DEF(mcuxClEls_CompareDmaFinalOutputAddress)
MCUXCLELS_API MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClEls_Status_t) mcuxClEls_CompareDmaFinalOutputAddress(
        uint8_t *outputStartAddress,
        size_t expectedLength)
{
    MCUX_CSSL_FP_FUNCTION_ENTRY(mcuxClEls_CompareDmaFinalOutputAddress,
                               MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClEls_GetLastDmaAddress));

    /* Calculate the expected final address from the input */
    uint8_t* expectedFinalAddress = (uint8_t *)MCUXCL_HW_DMA_WORKAROUND(outputStartAddress) + expectedLength;

    /* Get the actual final address from ELS - no result check as function always returns OK */
    uint32_t finalAddress;
    MCUX_CSSL_FP_FUNCTION_CALL_VOID(mcuxClEls_GetLastDmaAddress(&finalAddress));
    MCUX_CSSL_ANALYSIS_START_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER("Adresses are represented as integer objects when read from SFR registers")
    uint8_t *pFinalAddress = (uint8_t *)finalAddress;
    MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TYPECAST_BETWEEN_INTEGER_AND_POINTER()

    /* Compare the expected address to the actual one */
    if(pFinalAddress != expectedFinalAddress)
    {
        MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_CompareDmaFinalOutputAddress, MCUXCLELS_STATUS_SW_COMPARISON_FAILED);
    }

    MCUX_CSSL_FP_FUNCTION_EXIT(mcuxClEls_CompareDmaFinalOutputAddress, MCUXCLELS_STATUS_OK);

}
#endif /* MCUXCL_FEATURE_ELS_DMA_FINAL_ADDRESS_READBACK */
