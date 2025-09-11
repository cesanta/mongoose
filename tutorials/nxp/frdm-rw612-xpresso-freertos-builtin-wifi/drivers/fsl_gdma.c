/*
 * Copyright 2021-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_gdma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.gdma"
#endif

/* Is address aligned? */
#define GDMA_IS_ADDR_ALIGNED(addr, aligned_size) (((uint32_t)(addr) & ((aligned_size)-1UL)) == 0U)
/* Is the wrap used? */
#define GDMA_IS_WRAP_BURST(burstsize) (((uint8_t)(burstsize)&0x04U) != 0U)
/* Get real width from enum gdma_transfer_width_t. */
#define GDMA_REAL_XFER_WIDTH(width) (s_gdmaRealWidth[(uint8_t)(width)&0x03U])
/* Get real burst size from gdma_burst_size_t. */
#define GDMA_REAL_XFER_BUSTSIZE(burstsize) (s_gdmaRealBurstSize[(uint8_t)(burstsize)&0x03U])

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Typedef for interrupt handler. */
typedef void (*gdma_isr_t)(GDMA_Type *base);

/*!
 * @brief Verify the configuration.
 *
 * Verify the configuration, to make sure the parameters are valid.
 *
 * @param config Pointer to the transfer configuration.
 * @return Return true if the configuration is valid, otherwise return false.
 */
static bool GDMA_VerifyTransferConfig(const gdma_channel_xfer_config_t *config);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* @brief Pointers to transfer handle for each GDMA channel. */
static gdma_handle_t *s_gdmaHandles[FSL_FEATURE_GDMA_CHANNEL_NUM];

static const uint8_t s_gdmaRealBurstSize[] = {1U, 4U, 8U, 16U};
static const uint8_t s_gdmaRealWidth[]     = {1U, 1U, 2U, 4U};

/* ISR for transactional APIs. */
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
static gdma_isr_t s_gdmaIsr = (gdma_isr_t)DefaultISR;
#else
static gdma_isr_t s_gdmaIsr;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * brief Verify the configuration.
 *
 * Verify the configuration, to make sure the parameters are valid.
 *
 * param config Pointer to the transfer configuration.
 * return Return true if the configuration is valid, otherwise return false.
 */
static bool GDMA_VerifyTransferConfig(const gdma_channel_xfer_config_t *config)
{
    bool ret              = false;
    uint8_t srcWidth      = GDMA_REAL_XFER_WIDTH(config->srcWidth);
    uint8_t destWidth     = GDMA_REAL_XFER_WIDTH(config->destWidth);
    uint8_t srcBurstSize  = GDMA_REAL_XFER_BUSTSIZE(config->srcBurstSize);
    uint8_t destBurstSize = GDMA_REAL_XFER_BUSTSIZE(config->destBurstSize);
    uint8_t srcAddrAlignSize;
    uint8_t destAddrAlignSize;

    do
    {
        /*  SRCBSIZE * SRCWIDTH == DESTBSIZE * DESTWIDTH */
        if ((srcWidth * srcBurstSize) != (destWidth * destBurstSize))
        {
            break;
        }

        /*
         * Address alignment:
         *
         * From GDMA's view, the address only need to be aligned with the WIDTH (no matter wrap used or not).
         * When integrating with AHB and wrap is used, the address should be aligned to WIDTH * BURST_SIZE.
         */
        srcAddrAlignSize  = (GDMA_IS_WRAP_BURST(config->srcBurstSize)) ? (srcWidth * srcBurstSize) : srcWidth;
        destAddrAlignSize = (GDMA_IS_WRAP_BURST(config->destBurstSize)) ? (destWidth * destBurstSize) : destWidth;

        if (!((GDMA_IS_ADDR_ALIGNED(config->srcAddr, srcAddrAlignSize)) &&
              (GDMA_IS_ADDR_ALIGNED(config->destAddr, destAddrAlignSize))))
        {
            break;
        }

        ret = true;

    } while (false);

    return ret;
}

/*
 * brief Initializes GDMA peripheral.
 *
 * This function enable the GDMA clock, set descriptor table and
 * enable GDMA peripheral.
 *
 * param base GDMA peripheral base address.
 */
void GDMA_Init(GDMA_Type *base)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    CLOCK_EnableClock(kCLOCK_Gdma);
#endif
}

/*
 * brief Deinitializes GDMA peripheral.
 *
 * This function gates the GDMA clock.
 *
 * param base GDMA peripheral base address.
 */
void GDMA_Deinit(GDMA_Type *base)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    CLOCK_DisableClock(kCLOCK_Gdma);
#endif
}

/*
 * brief Set channel transfer configuration..
 *
 * This function configures the channel transfer, after configured, GDMA_StartChannel
 * could be called to start the transfer.
 *
 * This function must be called when previous transfer finished. Application can use
 * GDMA_IsChannelBusy to check whether the channel has finished the previous work.
 *
 * param base GDMA base address.
 * param channel GDMA channel number.
 * config Pointer to the transfer configuration.
 * retval kStatus_Fail GDMA is busy with previous transfer.
 * retval kStatus_Success Configuration set successfully.
 * retval kStatus_InvalidArgument Configuration wrong.
 */
status_t GDMA_SetChannelTransferConfig(GDMA_Type *base, uint8_t channel, const gdma_channel_xfer_config_t *config)
{
    assert(NULL != config);

    if (!GDMA_VerifyTransferConfig(config))
    {
        return kStatus_InvalidArgument;
    }

    if (GDMA_IsChannelBusy(base, channel))
    {
        return kStatus_Fail;
    }

    GDMA_ClearChannelInterruptFlags(base, channel, (uint32_t)kGDMA_AllInterruptFlag);

    base->CH[channel].SADR = config->srcAddr;
    base->CH[channel].DADR = config->destAddr;

    base->CH[channel].CTRL =
        GDMA_CTRL_PROT(config->ahbProt)                            /* Protection info for AHB master bus */
        | (config->srcAddrInc ? GDMA_CTRL_SRCADDRINC_MASK : 0UL)   /* Source address increment. */
        | (config->destAddrInc ? GDMA_CTRL_DESTADDRINC_MASK : 0UL) /* Destination address increment. */
        | GDMA_CTRL_SRCWIDTH(config->srcWidth)                     /* Source peripheral/memory transfer width. */
        | GDMA_CTRL_DESTWIDTH(config->destWidth)                   /* Destination peripheral/memory transfer width. */
        | GDMA_CTRL_SRCBSIZE(config->srcBurstSize)                 /* Source peripheral/memory transfer burst size */
        | GDMA_CTRL_DESTBSIZE(config->destBurstSize) /* Destination peripheral/memory transfer burst size */
        | GDMA_CTRL_LEN(config->transferLen);        /* Length of the transfer in bytes */

    if (config->enableLinkList)
    {
        base->CH[channel].LLI =
            (config->linkListAddr & GDMA_LLI_LLI_MASK)                        /* LLI address. */
            | (config->enableDescInterrupt ? GDMA_LLI_DESC_INT_EN_MASK : 0UL) /* Enable descriptor interrupt. */
            | (config->stopAfterDescFinished ? GDMA_LLI_STOP_MASK : 0UL);     /* Stop after descriptor finished. */
        base->CH[channel].CONFIG |= GDMA_CONFIG_LLE_MASK;
    }
    else
    {
        base->CH[channel].LLI = GDMA_LLI_STOP_MASK;
        base->CH[channel].CONFIG &= ~GDMA_CONFIG_LLE_MASK;
    }

    return kStatus_Success;
}

/*
 * brief Creates the GDMA handle.
 *
 * This function is called if using transaction API for GDMA. This function
 * initializes the internal state of GDMA handle.
 *
 * param handle GDMA handle pointer. It stores callback function and parameters.
 * param base GDMA peripheral base address.
 * param channel GDMA channel number.
 */
void GDMA_CreateHandle(gdma_handle_t *handle, GDMA_Type *base, uint8_t channel)
{
    assert(NULL != handle);

    (void)memset(handle, 0, sizeof(*handle));

    handle->gdma    = base;
    handle->channel = channel;

    s_gdmaHandles[channel] = handle;
    s_gdmaIsr              = GDMA_IRQHandle;

    (void)EnableIRQ(GDMA_IRQn);
}

/*
 * brief Installs a callback function for the GDMA transfer.
 *
 * This callback is called in GDMA IRQ handler to inform user the interrupt status.
 *
 * param handle GDMA handle pointer.
 * param callback GDMA callback function pointer.
 * param userData Parameter for callback function.
 */
void GDMA_SetCallback(gdma_handle_t *handle, gdma_callback_t callback, void *userData)
{
    assert(handle != NULL);

    handle->callback = callback;
    handle->userData = userData;
}

/*
 * brief Submits the GDMA channel transfer request.
 *
 * After this function, user could call GDMA_StartTransfer to start GDMA transfer.
 *
 * This function must be called when previous transfer finished. Application can use
 * GDMA_IsChannelBusy to check whether the channel has finished the previous work.
 *
 * param handle GDMA handle pointer.
 * param config Pointer to GDMA transfer configuration structure.
 * retval kStatus_Fail GDMA is busy with previous transfer.
 * retval kStatus_Success Configuration set successfully.
 * retval kStatus_InvalidArgument Configuration wrong.
 */
status_t GDMA_SubmitTransfer(gdma_handle_t *handle, gdma_channel_xfer_config_t *config)
{
    status_t status;

    assert(NULL != handle);

    status = GDMA_SetChannelTransferConfig(handle->gdma, handle->channel, config);

    if (status == kStatus_Success)
    {
        GDMA_EnableChannelInterrupts(
            handle->gdma, handle->channel,
            (uint32_t)kGDMA_DescriptorTransferDoneInterruptEnable | (uint32_t)kGDMA_AddressErrorInterruptEnable |
                (uint32_t)kGDMA_BusErrorInterruptEnable | (uint32_t)kGDMA_TransferDoneInterruptEnable);
    }

    return status;
}

/*
 * brief GDMA start transfer.
 *
 * User can call this function after GDMA_SubmitTransfer.
 *
 * param handle GDMA handle pointer.
 */
void GDMA_StartTransfer(gdma_handle_t *handle)
{
    assert(NULL != handle);

    GDMA_StartChannel(handle->gdma, handle->channel);
}

/*
 * brief Abort running transfer by handle.
 *
 * This function aborts GDMA transfer specified by handle.
 *
 * param handle GDMA handle pointer.
 */
void GDMA_AbortTransfer(gdma_handle_t *handle)
{
    assert(NULL != handle);

    GDMA_StopChannel(handle->gdma, handle->channel);

    GDMA_DisableChannelInterrupts(handle->gdma, handle->channel, (uint32_t)kGDMA_AllInterruptEnable);
}

/*
 * brief GDMA IRQ handler.
 *
 * This function checks all GDMA channel interrupts and inform application
 * the interrupt flags through user registered callback.
 *
 * param base GDMA peripheral.
 */
void GDMA_IRQHandle(GDMA_Type *base)
{
    uint8_t channel;
    uint32_t interrupts;
    gdma_handle_t *handle;

    for (channel = 0U; channel < (uint8_t)FSL_FEATURE_GDMA_CHANNEL_NUM; channel++)
    {
        interrupts = GDMA_GetChannelInterruptFlags(base, channel);

        /*
         * If channels unmasked interrupt happened. This flag is only assert
         * only when any unmasked interrupt happened.
         */
        if (0U != (interrupts & (uint32_t)kGDMA_ChannelInterruptFlag))
        {
            GDMA_ClearChannelInterruptFlags(base, channel, interrupts);

            handle = s_gdmaHandles[channel];

            if (NULL != handle)
            {
                /* If error happened or transfer finished successfully. */
                if (0U != (interrupts & ((uint32_t)kGDMA_AddressErrorFlag | (uint32_t)kGDMA_BusErrorFlag |
                                         (uint32_t)kGDMA_TransferDoneFlag)))
                {
                    GDMA_DisableChannelInterrupts(base, channel, (uint32_t)kGDMA_AllInterruptEnable);
                }

                if (NULL != handle->callback)
                {
                    handle->callback(handle, handle->userData, interrupts);
                }
            }
            else
            {
                /*
                 * If interrupt occurs but handle not created, then disable the interrupts,
                 * because to use the transactional APIs, user must create handle, then
                 * setup configuration and enable interrupts for data transfer.
                 *
                 * Generally will not reach here.
                 */
                GDMA_DisableChannelInterrupts(base, channel, (uint32_t)kGDMA_AllInterruptEnable);
            }
        }
    }
}

void GDMA_DriverIRQHandler(void);
void GDMA_DriverIRQHandler(void)
{
    s_gdmaIsr(GDMA);
    SDK_ISR_EXIT_BARRIER;
}
