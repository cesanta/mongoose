/*
 * Copyright 2021-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FSL_GDMA_H_
#define FSL_GDMA_H_

#include "fsl_common.h"

/*!
 * @addtogroup gdma
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*! @{ */
#define FSL_GDMA_DRIVER_VERSION (MAKE_VERSION(2, 0, 3))
/*! @} */

/*!
 * @brief Macro for GDMA link list descriptor LLI.
 *
 * This macro constructs @ref gdma_descriptor_t::lli.
 *
 * @param linkListAddr Address of next link list descriptor item.
 * @param stopAfterDescFinished Stop or not after this descriptor transfer done.
 * @param enableDescInterrupt Generate interrupt after this descriptor transfer done.
 */
#define GDMA_DESC_LLI(linkListAddr, stopAfterDescFinished, enableDescInterrupt)                                 \
    (((uint32_t)(linkListAddr)&GDMA_LLI_LLI_MASK) | ((enableDescInterrupt) ? GDMA_LLI_DESC_INT_EN_MASK : 0UL) | \
     ((stopAfterDescFinished) ? GDMA_LLI_STOP_MASK : 0UL))
/*
 * @brief Macro for GDMA link list descriptor CTRL.
 *
 * This macro constructs @ref gdma_descriptor_t::ctrl.
 *
 * @param ahbProt GDMA AHB HPROT flags, it could be OR'ed value of @ref _gdma_ahb_prot.
 * @param srcAddrInc Increase source address on each successive access, use true or false.
 * @param destAddrInc Increase destination address on each successive access, use true or false.
 * @param srcWidth  Source transfer width, see @ref gdma_transfer_width_t.
 * @param destWidth Destination transfer width, see @ref gdma_transfer_width_t.
 * @param srcBurstSize  Source address burst size, see @ref gdma_burst_size_t.
 * @param destBurstSize Destination address burst size, see @ref gdma_burst_size_t.
 * @param len Transfer length in bytes, max value is 8 * 1024 - 1.
 */
#define GDMA_DESC_CTRL(ahbProt, srcAddrInc, destAddrInc, srcWidth, destWidth, srcBurstSize, destBurstSize, length) \
    (GDMA_CTRL_PROT(ahbProt) | ((srcAddrInc) ? GDMA_CTRL_SRCADDRINC_MASK : 0UL) |                                  \
     ((destAddrInc) ? GDMA_CTRL_DESTADDRINC_MASK : 0UL) | GDMA_CTRL_SRCWIDTH(srcWidth) |                           \
     GDMA_CTRL_DESTWIDTH(destWidth) | GDMA_CTRL_SRCBSIZE(srcBurstSize) | GDMA_CTRL_DESTBSIZE(destBurstSize) |      \
     GDMA_CTRL_LEN(length))

/*! @brief GDMA transfer width */
typedef enum _gdma_transfer_width
{
    kGDMA_TransferWidth1Byte = 1U, /*!< 1 byte.  */
    kGDMA_TransferWidth2Byte = 2U, /*!< 2 bytes. */
    kGDMA_TransferWidth4Byte = 3U, /*!< 4 bytes. */
} gdma_transfer_width_t;

/*! @brief GDMA burst size*/
typedef enum _gdma_burst_size
{
    kGDMA_BurstSize1      = 0U, /*!< Burst 1.  */
    kGDMA_BurstSize4      = 1U, /*!< Burst 4.  */
    kGDMA_BurstSize8      = 2U, /*!< Burst 8.  */
    kGDMA_BurstSize16     = 3U, /*!< Burst 16. */
    kGDMA_BurstSizeWrap4  = 5U, /*!< Wrap 4.   */
    kGDMA_BurstSizeWrap8  = 6U, /*!< Wrap 8.   */
    kGDMA_BurstSizeWrap16 = 7U, /*!< Wrap 16.  */
} gdma_burst_size_t;

/*!
 * @brief GDMA AHB HPROT flags.
 * @anchor _gdma_ahb_prot
 */
enum _gdma_ahb_prot
{
    kGDMA_ProtUserMode       = (0U << 0U), /*!< The access is in user mode. */
    kGDMA_ProtPrevilegedMode = (1U << 0U), /*!< The access is in previleged mode. */
    kGDMA_ProtUnbufferable   = (0U << 1U), /*!< The access is not bufferable. */
    kGDMA_ProtBufferable     = (1U << 1U), /*!< The access is bufferable. */
    kGDMA_ProtUncacheable    = (0U << 2U), /*!< The access is not cacheable. */
    kGDMA_ProtCacheable      = (1U << 2U), /*!< The access is cacheable. */
};

/*! @brief GDMA channel link list descriptor structure */
typedef struct __ALIGNED(16) _gdma_descriptor
{
    uint32_t srcAddr; /*!< Source address. */
    uint32_t dstAddr; /*!< Destination address. */
    uint32_t lli;     /*!< Link list item. */
    uint32_t ctrl;    /*!< Transfer control. */
} gdma_descriptor_t;

/*! @brief GDMA channel priority */
typedef enum _gdma_priority
{
    kGDMA_ChannelPriority0 = 0, /*!< Lowest channel priority - priority 0   */
    kGDMA_ChannelPriority1,     /*!< Channel priority 1                     */
    kGDMA_ChannelPriority2,     /*!< Channel priority 2                     */
    kGDMA_ChannelPriority3,     /*!< Channel priority 3                     */
    kGDMA_ChannelPriority4,     /*!< Channel priority 4                     */
    kGDMA_ChannelPriority5,     /*!< Channel priority 5                     */
    kGDMA_ChannelPriority6,     /*!< Channel priority 6                     */
    kGDMA_ChannelPriority7,     /*!< Channel priority 7                     */
    kGDMA_ChannelPriority8,     /*!< Channel priority 8                     */
    kGDMA_ChannelPriority9,     /*!< Channel priority 9                     */
    kGDMA_ChannelPriority10,    /*!< Channel priority 10                    */
    kGDMA_ChannelPriority11,    /*!< Channel priority 11                    */
    kGDMA_ChannelPriority12,    /*!< Channel priority 12                    */
    kGDMA_ChannelPriority13,    /*!< Channel priority 13                    */
    kGDMA_ChannelPriority14,    /*!< Channel priority 14                    */
    kGDMA_ChannelPriority15,    /*!< Highest channel priority - priority 15 */
} gdma_priority_t;

/*!
 * @brief GDMA interrupts to enable
 * @anchor _gdma_interrupt_enable
 */
enum _gdma_interrupt_enable
{
    /*!
     * Descriptor transfer done interrupt. This happens when the descriptor
     * is configured to generate interrupt when transfer done.
     */
    kGDMA_DescriptorTransferDoneInterruptEnable = GDMA_CHNL_INT_MASK_DESC_TFRINT_MASK,

    /*! Channel source or destination address is not aligned to corresponding transfer width. */
    kGDMA_AddressErrorInterruptEnable = GDMA_CHNL_INT_MASK_ADDRERRINT_MASK,

    /*! AHB bus interrupt. */
    kGDMA_BusErrorInterruptEnable = GDMA_CHNL_INT_MASK_BUSERRINT_MASK,

    /*! DMA transfer done interrupt. */
    kGDMA_TransferDoneInterruptEnable = GDMA_CHNL_INT_MASK_TFRINT_MASK,

    /*! DMA block single/burst transfer done interrupt. */
    kGDMA_BlockTransferDoneInterruptEnable = GDMA_CHNL_INT_MASK_BLOCKINT_MASK,

    /*! All interrupt enable. */
    kGDMA_AllInterruptEnable = kGDMA_DescriptorTransferDoneInterruptEnable | kGDMA_AddressErrorInterruptEnable |
                               kGDMA_BusErrorInterruptEnable | kGDMA_TransferDoneInterruptEnable |
                               kGDMA_BlockTransferDoneInterruptEnable,
};

/*!
 * @brief GDMA interrupt status flags.
 * @anchor _gdma_interrupt_flags
 */
enum _gdma_interrupt_flags
{
    /*!
     * Descriptor transfer done interrupt. This happens when the descriptor
     * is configured to generate interrupt when transfer done.
     */
    kGDMA_DescriptorTransferDoneFlag = GDMA_CHNL_INT_DESC_STATUS_TFRINT_MASK,

    /*! OR of the content of the respective unmasked interrupt of channel. */
    kGDMA_ChannelInterruptFlag = GDMA_CHNL_INT_STATUS_CHLINT_MASK,

    /*! Channel source or destination address is not aligned to corresponding transfer width. */
    kGDMA_AddressErrorFlag = GDMA_CHNL_INT_STATUS_ADDRERRINT_MASK,

    /*! AHB bus interrupt. */
    kGDMA_BusErrorFlag = GDMA_CHNL_INT_STATUS_BUSERRINT_MASK,

    /*! DMA transfer done interrupt. */
    kGDMA_TransferDoneFlag = GDMA_CHNL_INT_STATUS_TFRINT_MASK,

    /*! DMA block single/burst transfer done interrupt. */
    kGDMA_BlockTransferDoneFlag = GDMA_CHNL_INT_STATUS_BLOCKINT_MASK,

    /*! All interrupt flags. */
    kGDMA_AllInterruptFlag = kGDMA_DescriptorTransferDoneFlag | kGDMA_ChannelInterruptFlag | kGDMA_AddressErrorFlag |
                             kGDMA_BusErrorFlag | kGDMA_TransferDoneFlag | kGDMA_BlockTransferDoneFlag,
};

/*!
 * @brief GDMA channel transfer configuration.
 *
 * @note The transfer configuration must follow the requirements:
 *   - SRCBSIZE * SRCWIDTH == DESTBSIZE * DESTWIDTH
 *   - If wrap not used, the address should align with WIDTH
 *   - If wrap used, the address should align with WIDTH * BURST_SIZE.
 */
typedef struct _gdma_channel_xfer_config
{
    uint32_t srcAddr;                /*!< Source data address */
    uint32_t destAddr;               /*!< Destination data address */
    uint8_t ahbProt;                 /*!< GDMA AHB HPROT flags, it could be OR'ed value of @ref _gdma_ahb_prot. */
    gdma_burst_size_t srcBurstSize;  /*!< Source address burst size. */
    gdma_burst_size_t destBurstSize; /*!< Destination address burst size. */
    gdma_transfer_width_t srcWidth;  /*!< Source transfer width. */
    gdma_transfer_width_t destWidth; /*!< Destination transfer width. */
    bool srcAddrInc;                 /*!< Increase source address on each successive access. */
    bool destAddrInc;                /*!< Increase destination address on each successive access. */
    uint16_t transferLen; /*!< Transfer length in bytes, max value is 8 * 1024 - 1, should align with transfer size. */
    bool enableLinkList;  /*!< Enable link list or not. */

    /*! Generate interrupt when descriptor transfer finished, only used when @ref enableLinkList is true. */
    bool enableDescInterrupt;

    /*! Stop channel when descriptor transfer finished, only used when @ref enableLinkList is true. */
    bool stopAfterDescFinished;

    uint32_t linkListAddr; /*!< Link list address, only used when @ref enableLinkList is true. */
} gdma_channel_xfer_config_t;

/*! @brief Driver handle for GDMA */
struct _gdma_handle;

/*! @brief Define Callback function for GDMA.
 *
 * handle: Pointer to the GDMA driver handle.
 * userData: The userData registered using @ref GDMA_SetCallback.
 * interrupts: The interrupts flags of the specific channel.
 */
typedef void (*gdma_callback_t)(struct _gdma_handle *handle, void *userData, uint32_t interrupts);

/*! @brief GDMA transfer handle structure */
typedef struct _gdma_handle
{
    GDMA_Type *gdma;          /*!< GDMA peripheral base address */
    uint8_t channel;          /*!< GDMA channel number */
    gdma_callback_t callback; /*!< Callback function. Invoked interrupt happens. */
    void *userData;           /*!< Callback function parameter */
} gdma_handle_t;

/*******************************************************************************
 * APIs
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 * @name GDMA initialization and De-initialization
 * @{
 */

/*!
 * @brief Initializes GDMA peripheral.
 *
 * It ungates the GDMA access clock, after this function, the GDMA module is
 * ready to be used.
 *
 * @param base GDMA peripheral base address.
 */
void GDMA_Init(GDMA_Type *base);

/*!
 * @brief Deinitializes GDMA peripheral.
 *
 * @param base GDMA peripheral base address.
 */
void GDMA_Deinit(GDMA_Type *base);

/*! @} */

/*!
 * @name GDMA Channel Operation
 * @{
 */

/*!
 * @brief Set GDMA channel source address.
 *
 * @param base GDMA peripheral base address.
 * @param channel GDMA channel number.
 * @param addr Source address.
 */
static inline void GDMA_SetChannelSourceAddress(GDMA_Type *base, uint8_t channel, uint32_t addr)
{
    base->CH[channel].SADR = addr;
}

/*!
 * @brief Set GDMA channel destination address.
 *
 * @param base GDMA peripheral base address.
 * @param channel GDMA channel number.
 * @param addr Destination address.
 */
static inline void GDMA_SetChannelDestAddress(GDMA_Type *base, uint8_t channel, uint32_t addr)
{
    base->CH[channel].DADR = addr;
}

/*!
 * @brief Start GDMA channel to work.
 *
 * @param base GDMA peripheral base address.
 * @param channel GDMA channel number.
 */
static inline void GDMA_StartChannel(GDMA_Type *base, uint8_t channel)
{
    base->CH[channel].CHL_EN |= GDMA_CHL_EN_CHL_EN_MASK;
}

/*!
 * @brief Stop GDMA channel.
 *
 * @param base GDMA peripheral base address.
 * @param channel GDMA channel number.
 */
static inline void GDMA_StopChannel(GDMA_Type *base, uint8_t channel)
{
    base->CH[channel].CHL_STOP = GDMA_CHL_STOP_CHL_STOP_MASK;
}

/*!
 * @brief Return whether GDMA channel is processing transfer
 *
 * When @ref GDMA_StopChannel is called, if the channel is on service,
 * it does not stop immediately, application could call this API to check
 * whether the channel is stopped.
 *
 * @param base GDMA peripheral base address.
 * @param channel GDMA channel number.
 * @return True if the channel is busy, false if not.
 */
static inline bool GDMA_IsChannelBusy(GDMA_Type *base, uint8_t channel)
{
    return ((base->CH[channel].CHL_EN & GDMA_CHL_EN_CHL_EN_MASK) != 0UL);
}

/*!
 * @brief Enables the interrupt for the GDMA transfer.
 *
 * @param base GDMA peripheral base address.
 * @param channel GDMA channel number.
 * @param interrupts The interrupts to enable, it is OR'ed value of @ref _gdma_interrupt_enable.
 */
static inline void GDMA_EnableChannelInterrupts(GDMA_Type *base, uint8_t channel, uint32_t interrupts)
{
    base->CH[channel].CHNL_INT |= interrupts;
}

/*!
 * @brief Disables the interrupt for the GDMA transfer.
 *
 * @param base GDMA peripheral base address.
 * @param channel GDMA channel number.
 * @param interrupts The interrupts to disable, it is OR'ed value of @ref _gdma_interrupt_enable.
 */
static inline void GDMA_DisableChannelInterrupts(GDMA_Type *base, uint8_t channel, uint32_t interrupts)
{
    base->CH[channel].CHNL_INT &= ~interrupts;
}

/*!
 * @brief Get the GDMA channel interrupt flags.
 *
 * @param base GDMA peripheral base address.
 * @param channel GDMA channel number.
 * @return The interrupt flags, it is OR'ed value of @ref _gdma_interrupt_flags.
 */
static inline uint32_t GDMA_GetChannelInterruptFlags(GDMA_Type *base, uint8_t channel)
{
    return base->CH[channel].CHNL_INT & (uint32_t)kGDMA_AllInterruptFlag;
}

/*!
 * @brief Clear the GDMA channel interrupt flags.
 *
 * The @ref kGDMA_ChannelInterruptFlag is OR'ed status of all other unmasked interrupt flags,
 * it could not be clear directly, it should be cleared by clear all other flags.
 *
 * @param base GDMA peripheral base address.
 * @param channel GDMA channel number.
 * @param flags The interrupt flags to clear, it is OR'ed value of @ref _gdma_interrupt_flags.
 */
static inline void GDMA_ClearChannelInterruptFlags(GDMA_Type *base, uint8_t channel, uint32_t flags)
{
    base->CH[channel].CHNL_INT = (base->CH[channel].CHNL_INT & ~(uint32_t)kGDMA_AllInterruptFlag) | flags;
}

/*!
 * @brief Get the number of finished descriptor.
 *
 * The counter increases when an item of descriptor is done in linklist mode.
 *
 * @param base GDMA peripheral base address.
 * @param channel GDMA channel number.
 * @return Number of finished descriptor.
 */
static inline uint32_t GDMA_GetChannelFinishedDescriptorNumber(GDMA_Type *base, uint8_t channel)
{
    return base->CH[channel].NUM_OF_DESCRIPTOR;
}

/*!
 * @brief Clear the number of finished descriptor.
 *
 * @param base GDMA peripheral base address.
 * @param channel GDMA channel number.
 */
static inline void GDMA_ClearChannelFinishedDescriptorNumber(GDMA_Type *base, uint8_t channel)
{
    base->CH[channel].NUM_OF_DESCRIPTOR = 0UL;
}

/*!
 * @brief Set priority of channel.
 *
 * @param base GDMA peripheral base address.
 * @param channel GDMA channel number.
 * @param priority Channel priority value.
 */
static inline void GDMA_SetChannelPriority(GDMA_Type *base, uint8_t channel, gdma_priority_t priority)
{
    base->CH[channel].CHL_EN =
        (base->CH[channel].CHL_EN & ~(GDMA_CHL_EN_CHL_PRIORITY_WEIGHT_MASK | GDMA_CHL_EN_CHL_EN_MASK)) |
        (uint32_t)priority;
}

/*!
 * @brief Set channel transfer configuration..
 *
 * This function configures the channel transfer, after configured, @ref GDMA_StartChannel
 * could be called to start the transfer.
 *
 * This function must be called when previous transfer finished. Application can use
 * @ref GDMA_IsChannelBusy to check whether the channel has finished the previous work.
 *
 * @note The transfer configuration must follow the requirements:
 *   - SRCBSIZE * SRCWIDTH == DESTBSIZE * DESTWIDTH
 *   - If wrap not used, the address should align with WIDTH
 *   - If wrap used, the address should align with WIDTH * BURST_SIZE.
 *
 * @param base GDMA base address.
 * @param channel GDMA channel number.
 * @config Pointer to the transfer configuration.
 * @retval kStatus_Fail GDMA is busy with previous transfer.
 * @retval kStatus_Success Configuration set successfully.
 * @retval kStatus_InvalidArgument Configuration wrong.
 */
status_t GDMA_SetChannelTransferConfig(GDMA_Type *base, uint8_t channel, const gdma_channel_xfer_config_t *config);

/*! @} */

/*!
 * @name GDMA Transactional Operation
 * @{
 */

/*!
 * @brief Creates the GDMA handle.
 *
 * This function is called if using transaction API for GDMA. This function
 * initializes the internal state of GDMA handle.
 *
 * @param handle GDMA handle pointer. It stores callback function and parameters.
 * @param base GDMA peripheral base address.
 * @param channel GDMA channel number.
 */
void GDMA_CreateHandle(gdma_handle_t *handle, GDMA_Type *base, uint8_t channel);

/*!
 * @brief Installs a callback function for the GDMA transfer.
 *
 * This callback is called in GDMA IRQ handler to inform user the interrupt status.
 *
 * @param handle GDMA handle pointer.
 * @param callback GDMA callback function pointer.
 * @param userData Parameter for callback function.
 */
void GDMA_SetCallback(gdma_handle_t *handle, gdma_callback_t callback, void *userData);

/*!
 * @brief Submits the GDMA channel transfer request.
 *
 * After this function, user could call @ref GDMA_StartTransfer to start GDMA transfer.
 *
 * This function must be called when previous transfer finished. Application can use
 * @ref GDMA_IsChannelBusy to check whether the channel has finished the previous work.
 *
 * @note The transfer configuration must follow the requirements:
 *   - SRCBSIZE * SRCWIDTH == DESTBSIZE * DESTWIDTH
 *   - If wrap not used, the address should align with WIDTH
 *   - If wrap used, the address should align with WIDTH * BURST_SIZE.
 *
 * @param handle GDMA handle pointer.
 * @param config Pointer to GDMA transfer configuration structure.
 * @retval kStatus_Fail GDMA is busy with previous transfer.
 * @retval kStatus_Success Configuration set successfully.
 * @retval kStatus_InvalidArgument Configuration wrong.
 */
status_t GDMA_SubmitTransfer(gdma_handle_t *handle, gdma_channel_xfer_config_t *config);

/*!
 * @brief GDMA start transfer.
 *
 * User can call this function after @ref GDMA_SubmitTransfer.
 *
 * @param handle GDMA handle pointer.
 */
void GDMA_StartTransfer(gdma_handle_t *handle);

/*!
 * @brief Abort running transfer by handle.
 *
 * When this function is called, if the channel is on service, it only
 * stops when service finished.
 *
 * @param handle GDMA handle pointer.
 */
void GDMA_AbortTransfer(gdma_handle_t *handle);

/*!
 * @brief GDMA IRQ handler.
 *
 * This function checks all GDMA channel interrupts and inform application
 * the interrupt flags through user registered callback.
 *
 * @param base GDMA peripheral.
 */
void GDMA_IRQHandle(GDMA_Type *base);

/*! @} */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/*! @} */

#endif /*FSL_GDMA_H_*/
