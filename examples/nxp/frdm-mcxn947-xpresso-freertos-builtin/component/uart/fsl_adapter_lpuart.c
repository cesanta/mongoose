/*
 * Copyright 2018, 2020, 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_lpuart.h"

#include "fsl_adapter_uart.h"

#if (defined(FSL_FEATURE_LPUART_IS_LPFLEXCOMM) && (FSL_FEATURE_LPUART_IS_LPFLEXCOMM > 0U))
#include "fsl_lpflexcomm.h"
#endif /* FSL_FEATURE_LPUART_IS_LPFLEXCOMM */

/*! @brief MACROs for whether a software idleline detection should be used. */
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))
#if (defined(HAL_UART_TRANSFER_MODE) && (HAL_UART_TRANSFER_MODE > 0U))
#define HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION (1)
#else /* HAL_UART_TRANSFER_MODE */
#define HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION (0)
#endif /* HAL_UART_TRANSFER_MODE */
#else  /* UART_ADAPTER_NON_BLOCKING_MODE */
#define HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION (1)
#endif /* UART_ADAPTER_NON_BLOCKING_MODE */
#endif /* HAL_UART_DMA_ENABLE */

#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
#if (defined(HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION) && (HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION > 0U))
#include "fsl_component_timer_manager.h"
#endif /* HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION */
#if (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
#include "fsl_lpuart_edma.h"
#elif (defined(FSL_FEATURE_SOC_DMA_COUNT) && (FSL_FEATURE_SOC_DMA_COUNT > 0U))
#include "fsl_lpuart_dma.h"
#endif
#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
#include "fsl_dmamux.h"
#endif
#ifdef DMA_IRQS
#define DMA_CHN_IRQS DMA_IRQS
#endif
#endif /* HAL_UART_DMA_ENABLE */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef NDEBUG
#if (defined(DEBUG_CONSOLE_ASSERT_DISABLE) && (DEBUG_CONSOLE_ASSERT_DISABLE > 0U))
#undef assert
#define assert(n)
#endif
#endif

#ifndef HAL_UART_ADAPTER_LOWPOWER_RESTORE
#define HAL_UART_ADAPTER_LOWPOWER_RESTORE (1)
#endif

#ifndef HAL_UART_DMA_RING_BUFFER_ENABLE
#define HAL_UART_DMA_RING_BUFFER_ENABLE (0U)
#endif /* HAL_UART_DMA_ENABLE */
#ifndef LPUART_RING_BUFFER_SIZE
#define LPUART_RING_BUFFER_SIZE (128U)
#endif

#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
/*! @brief uart RX state structure. */
typedef struct _hal_uart_dma_receive_state
{
    uint8_t *volatile buffer;
    volatile uint32_t bufferLength;
    volatile uint32_t bufferSofar;
    volatile uint32_t timeout;
    volatile bool receiveAll;
} hal_uart_dma_receive_state_t;

/*! @brief uart TX state structure. */
typedef struct _hal_uart_dma_send_state
{
    uint8_t *volatile buffer;
    volatile uint32_t bufferLength;
    volatile uint32_t bufferSofar;
    volatile uint32_t timeout;
} hal_uart_dma_send_state_t;

typedef struct _hal_uart_dma_state
{
    struct _hal_uart_dma_state *next;
    uint8_t instance; /* LPUART instance */
    hal_uart_dma_transfer_callback_t dma_callback;
    void *dma_callback_param;

#if (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
    lpuart_edma_handle_t edmaHandle;
    edma_handle_t txEdmaHandle;
    edma_handle_t rxEdmaHandle;
#elif (defined(FSL_FEATURE_SOC_DMA_COUNT) && (FSL_FEATURE_SOC_DMA_COUNT > 0U))
    lpuart_dma_handle_t dmaHandle;
    dma_handle_t txDmaHandle;
    dma_handle_t rxDmaHandle;
#endif
    hal_uart_dma_receive_state_t dma_rx;
    hal_uart_dma_send_state_t dma_tx;
#if (defined(HAL_UART_ADAPTER_LOWPOWER) && (HAL_UART_ADAPTER_LOWPOWER > 0U))
    dma_channel_mux_configure_t dma_channel_mux_configure;
    dma_mux_configure_t dma_mux_configure;
    hal_uart_dma_config_t hal_uart_dma_config;
#endif
} hal_uart_dma_state_t;

typedef struct _lpuart_dma_list
{
#if (defined(HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION) && (HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION > 0U))
    TIMER_MANAGER_HANDLE_DEFINE(timerManagerHandle);
#endif /* HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION */
    hal_uart_dma_state_t *dma_list;
    volatile int8_t activeCount;
} hal_lpuart_dma_list_t;

static hal_lpuart_dma_list_t s_dmaHandleList;
#endif /* HAL_UART_DMA_ENABLE */

#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))
/*! @brief uart RX state structure. */
typedef struct _hal_uart_receive_state
{
    uint8_t *volatile buffer;
    volatile uint32_t bufferLength;
    volatile uint32_t bufferSofar;
} hal_uart_receive_state_t;

/*! @brief uart TX state structure. */
typedef struct _hal_uart_send_state
{
    uint8_t *volatile buffer;
    volatile uint32_t bufferLength;
    volatile uint32_t bufferSofar;
} hal_uart_send_state_t;
#endif
/*! @brief uart state structure. */
typedef struct _hal_uart_state
{
    uint8_t instance;
#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))
    hal_uart_transfer_callback_t callback;
    void *callbackParam;
#if (defined(HAL_UART_TRANSFER_MODE) && (HAL_UART_TRANSFER_MODE > 0U))
    lpuart_handle_t hardwareHandle;
#endif
    hal_uart_receive_state_t rx;
    hal_uart_send_state_t tx;
#endif
#if (defined(HAL_UART_ADAPTER_LOWPOWER) && (HAL_UART_ADAPTER_LOWPOWER > 0U))
#if (defined(HAL_UART_ADAPTER_LOWPOWER_RESTORE) && (HAL_UART_ADAPTER_LOWPOWER_RESTORE > 0U))
    uint32_t reg_BAUD;
    uint32_t reg_CTRL;
    uint32_t reg_WATER;
    uint32_t reg_MODIR;
#else
    hal_uart_config_t config;
#endif
#endif
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
    hal_uart_dma_state_t *dmaHandle;
#endif /* HAL_UART_DMA_ENABLE */
} hal_uart_state_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static LPUART_Type *const s_LpuartAdapterBase[] = LPUART_BASE_PTRS;
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
#if (defined(HAL_UART_ADAPTER_LOWPOWER) && (HAL_UART_ADAPTER_LOWPOWER > 0U))
#if (defined(HAL_UART_ADAPTER_LOWPOWER_RESTORE) && (HAL_UART_ADAPTER_LOWPOWER_RESTORE > 0U))
static const clock_ip_name_t s_LpuartAdapterClock[] = LPUART_CLOCKS;
#endif /* HAL_UART_ADAPTER_LOWPOWER_RESTORE */
#endif /* HAL_UART_ADAPTER_LOWPOWER */
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))

#if !(defined(HAL_UART_TRANSFER_MODE) && (HAL_UART_TRANSFER_MODE > 0U))
/* Array of LPUART IRQ number. */
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
static const IRQn_Type s_LpuartRxIRQ[] = LPUART_RX_IRQS;
static const IRQn_Type s_LpuartTxIRQ[] = LPUART_TX_IRQS;
#else
static const IRQn_Type s_LpuartIRQ[] = LPUART_RX_TX_IRQS;
#endif
#endif

#if !(defined(HAL_UART_TRANSFER_MODE) && (HAL_UART_TRANSFER_MODE > 0U))
static hal_uart_state_t *s_UartState[sizeof(s_LpuartAdapterBase) / sizeof(LPUART_Type *)];
#endif
#endif

#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
#if (defined(HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION) && (HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION > 0U))
#else  /* HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION */
static hal_uart_dma_state_t *s_UartDmaState[sizeof(s_LpuartAdapterBase) / sizeof(LPUART_Type *)] = {0};
#endif /* HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION */
#if (defined(HAL_UART_DMA_RING_BUFFER_ENABLE) && (HAL_UART_DMA_RING_BUFFER_ENABLE > 0U))
/* allocate ring buffer section. */
AT_NONCACHEABLE_SECTION_INIT(
    static uint8_t s_ringBuffer[sizeof(s_LpuartAdapterBase) / sizeof(LPUART_Type *)][LPUART_RING_BUFFER_SIZE]) = {0};
/* Allocate TCD memory poll with ring buffer used. */
AT_NONCACHEABLE_SECTION_ALIGN(
    static edma_tcd_t tcdMemoryPoolPtr[sizeof(s_LpuartAdapterBase) / sizeof(LPUART_Type *)][1], sizeof(edma_tcd_t));

static volatile uint32_t ringBufferIndex[sizeof(s_LpuartAdapterBase) / sizeof(LPUART_Type *)] = {0U};
static void LPUART_StartRingBufferEDMA(hal_uart_handle_t handle);
static void LPUART_DMACallbacks(LPUART_Type *base, lpuart_edma_handle_t *handle, status_t status, void *userData);
#endif
#endif /* HAL_UART_DMA_ENABLE */

/*******************************************************************************
 * Code
 ******************************************************************************/

#if ((defined(HAL_UART_TRANSFER_MODE) && (HAL_UART_TRANSFER_MODE > 0U)) || \
     (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U)))
static hal_uart_status_t HAL_UartGetStatus(status_t status)
{
    hal_uart_status_t uartStatus = kStatus_HAL_UartError;
    switch (status)
    {
        case (int32_t)kStatus_Success:
            uartStatus = kStatus_HAL_UartSuccess;
            break;
        case (int32_t)kStatus_LPUART_TxBusy:
            uartStatus = kStatus_HAL_UartTxBusy;
            break;
        case (int32_t)kStatus_LPUART_RxBusy:
            uartStatus = kStatus_HAL_UartRxBusy;
            break;
        case (int32_t)kStatus_LPUART_TxIdle:
            uartStatus = kStatus_HAL_UartTxIdle;
            break;
        case (int32_t)kStatus_LPUART_RxIdle:
            uartStatus = kStatus_HAL_UartRxIdle;
            break;
        case (int32_t)kStatus_LPUART_BaudrateNotSupport:
            uartStatus = kStatus_HAL_UartBaudrateNotSupport;
            break;
        case (int32_t)kStatus_LPUART_NoiseError:
        case (int32_t)kStatus_LPUART_FramingError:
        case (int32_t)kStatus_LPUART_ParityError:
            uartStatus = kStatus_HAL_UartProtocolError;
            break;
        default:
            /*MISRA rule 16.4*/
            break;
    }
    return uartStatus;
}
#else
static hal_uart_status_t HAL_UartGetStatus(status_t status)
{
    hal_uart_status_t uartStatus;
    if ((int32_t)kStatus_Success == status)
    {
        uartStatus = kStatus_HAL_UartSuccess; /* Successfully */
    }
    else
    {
        uartStatus = kStatus_HAL_UartError; /* Error occurs on HAL uart */
    }
    return uartStatus;
}
#endif

#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
#if (defined(HAL_UART_DMA_RING_BUFFER_ENABLE) && (HAL_UART_DMA_RING_BUFFER_ENABLE > 0U))
static uint32_t HAL_UartGetDmaReceivedBytes(uint8_t instance)
{
    volatile uint32_t receivedBytes = 0U;
    uint32_t remainingBytes         = 0U;
    uint32_t newIndex               = 0U;

    hal_uart_dma_state_t *uartDmaHandle = s_UartDmaState[instance];

    remainingBytes =
        EDMA_GetRemainingMajorLoopCount(uartDmaHandle->rxEdmaHandle.base, uartDmaHandle->rxEdmaHandle.channel);
    if (remainingBytes == LPUART_RING_BUFFER_SIZE)
    {
        remainingBytes = 0;
    }
    else
    {
        newIndex = LPUART_RING_BUFFER_SIZE - remainingBytes;
    }

    if (newIndex < ringBufferIndex[instance])
    {
        newIndex = newIndex + LPUART_RING_BUFFER_SIZE;
    }
    receivedBytes = newIndex - ringBufferIndex[instance];

    return receivedBytes;
}
#endif
#if (defined(HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION) && (HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION > 0U))
#else /* HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION */
static void HAL_UartDMAIdlelineInterruptHandle(uint8_t instance)
{
    hal_uart_dma_state_t *uartDmaHandle = s_UartDmaState[instance];
    hal_dma_callback_msg_t dmaMsg;
#if (defined(HAL_UART_DMA_RING_BUFFER_ENABLE) && (HAL_UART_DMA_RING_BUFFER_ENABLE > 0U))
    uint32_t receiveLength  = 0;
    uint32_t callbackLength = 0;
    uint32_t remianLength   = 0;
    uint32_t key;
#endif /* HAL_UART_DMA_RING_BUFFER_ENABLE */
    assert(uartDmaHandle);
#if (defined(HAL_UART_DMA_RING_BUFFER_ENABLE) && (HAL_UART_DMA_RING_BUFFER_ENABLE > 0U))
    if ((NULL != uartDmaHandle->dma_callback) && (NULL != uartDmaHandle->dma_rx.buffer))
    {
        key           = DisableGlobalIRQ();
#if (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
        receiveLength = HAL_UartGetDmaReceivedBytes(instance);
#elif (defined(FSL_FEATURE_SOC_DMA_COUNT) && (FSL_FEATURE_SOC_DMA_COUNT > 0U))

#endif /* FSL_FEATURE_SOC_EDMA_COUNT */
        if (uartDmaHandle->dma_rx.receiveAll == true)
        {
            if (receiveLength < uartDmaHandle->dma_rx.bufferLength)
            {
                LPUART_EnableInterrupts(s_LpuartAdapterBase[instance], (uint32_t)kLPUART_IdleLineInterruptEnable);
                EnableGlobalIRQ(key);
                return;
            }
        }
        callbackLength =
            (receiveLength < uartDmaHandle->dma_rx.bufferLength) ? receiveLength : uartDmaHandle->dma_rx.bufferLength;
        if (callbackLength != 0U)
        {
            dmaMsg.status   = kStatus_HAL_UartDmaIdleline;
            dmaMsg.dataSize = callbackLength;

            if (ringBufferIndex[instance] + callbackLength < LPUART_RING_BUFFER_SIZE)
            {
                (void)memcpy(uartDmaHandle->dma_rx.buffer, &s_ringBuffer[instance][ringBufferIndex[instance]],
                             callbackLength);
                ringBufferIndex[instance] += callbackLength;
            }
            else
            {
                remianLength = callbackLength + ringBufferIndex[instance] - LPUART_RING_BUFFER_SIZE;
                (void)memcpy(uartDmaHandle->dma_rx.buffer, &s_ringBuffer[instance][ringBufferIndex[instance]],
                             (callbackLength - remianLength));
                (void)memcpy(uartDmaHandle->dma_rx.buffer + (callbackLength - remianLength), &s_ringBuffer[instance][0],
                             remianLength);
                ringBufferIndex[instance] = remianLength;
            }
            dmaMsg.data                     = uartDmaHandle->dma_rx.buffer;
            uartDmaHandle->dma_rx.buffer = NULL;

            uartDmaHandle->dma_callback(uartDmaHandle, &dmaMsg, uartDmaHandle->dma_callback_param);
        }
        EnableGlobalIRQ(key);
    }
#else
    if ((NULL != uartDmaHandle->dma_callback) && (NULL != uartDmaHandle->dma_rx.buffer))
    {
        /* HAL_UartDMAGetReceiveCount(uartDmaHandle, &msg.dataSize); */
        /* HAL_UartDMAAbortReceive(uartDmaHandle); */

#if (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
        (void)LPUART_TransferGetReceiveCountEDMA(s_LpuartAdapterBase[uartDmaHandle->instance],
                                                 &uartDmaHandle->edmaHandle, &dmaMsg.dataSize);
        LPUART_TransferAbortReceiveEDMA(s_LpuartAdapterBase[uartDmaHandle->instance], &uartDmaHandle->edmaHandle);
#elif (defined(FSL_FEATURE_SOC_DMA_COUNT) && (FSL_FEATURE_SOC_DMA_COUNT > 0U))
#endif /* FSL_FEATURE_SOC_EDMA_COUNT */
        dmaMsg.status                   = kStatus_HAL_UartDmaIdleline;
        dmaMsg.data                     = uartDmaHandle->dma_rx.buffer;
        uartDmaHandle->dma_rx.buffer = NULL;

        uartDmaHandle->dma_callback(uartDmaHandle, &dmaMsg, uartDmaHandle->dma_callback_param);
    }

#endif
}
#endif /* HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION */
#endif /* HAL_UART_DMA_ENABLE */

#if (defined(HAL_UART_TRANSFER_MODE) && (HAL_UART_TRANSFER_MODE > 0U))
static void HAL_UartCallback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *callbackParam)
{
    hal_uart_state_t *uartHandle;
    hal_uart_status_t uartStatus = HAL_UartGetStatus(status);
    assert(callbackParam);

    uartHandle = (hal_uart_state_t *)callbackParam;

    if (kStatus_HAL_UartProtocolError == uartStatus)
    {
        if (0U != uartHandle->hardwareHandle.rxDataSize)
        {
            uartStatus = kStatus_HAL_UartError;
        }
    }

    if (NULL != uartHandle->callback)
    {
        uartHandle->callback(uartHandle, uartStatus, uartHandle->callbackParam);
    }
}

#else /* HAL_UART_TRANSFER_MODE */

static void HAL_UartInterruptHandle(uint8_t instance)
{
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
#if (defined(HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION) && (HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION > 0U))
#else  /* HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION */
    hal_dma_callback_msg_t dmaMsg;
    hal_uart_dma_state_t *uartDmaHandle = s_UartDmaState[instance];
    uint32_t sentCount = 0U;
#endif /* HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION */
#endif /* HAL_UART_DMA_ENABLE */
    hal_uart_state_t *uartHandle = s_UartState[instance];
    uint32_t status;
#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
    uint8_t count;
#endif

    assert(NULL != uartHandle);

    status = LPUART_GetStatusFlags(s_LpuartAdapterBase[instance]);

#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
#if (defined(HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION) && (HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION > 0U))
#else /* HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION */
    /* DMA send complete interrupt. */
    if ((NULL != uartDmaHandle) && (instance == uartDmaHandle->instance))
    {
        if (NULL != uartDmaHandle->dma_tx.buffer)
        {
            if ((0U != ((uint32_t)kLPUART_TransmissionCompleteFlag & status)) &&
                (0U != (LPUART_GetEnabledInterrupts(s_LpuartAdapterBase[instance]) &
                        (uint32_t)kLPUART_TransmissionCompleteFlag)))

            {
#if (defined(FSL_FEATURE_SOC_DMA_COUNT) && (FSL_FEATURE_SOC_DMA_COUNT > 0U))
#elif (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
                (void)LPUART_TransferGetSendCountEDMA(s_LpuartAdapterBase[uartDmaHandle->instance],
                                                &uartDmaHandle->edmaHandle, &sentCount);
#endif /* FSL_FEATURE_SOC_EDMA_COUNT */

                /* Disable tx complete interrupt */
                (void)LPUART_DisableInterrupts(s_LpuartAdapterBase[instance],
                                               (uint32_t)kLPUART_TransmissionCompleteFlag);
                uartDmaHandle->edmaHandle.txState = 0;
                dmaMsg.status                        = kStatus_HAL_UartDmaTxIdle;
                dmaMsg.data                          = uartDmaHandle->dma_tx.buffer;
                dmaMsg.dataSize                      = uartDmaHandle->dma_tx.bufferLength;
                uartDmaHandle->dma_tx.buffer      = NULL;
                uartDmaHandle->dma_callback(uartDmaHandle, &dmaMsg, uartDmaHandle->dma_callback_param);
            }
        }
        /* DMA receive Idleline interrupt. */
#if (defined(HAL_UART_DMA_RING_BUFFER_ENABLE) && (HAL_UART_DMA_RING_BUFFER_ENABLE > 0U))
        if (NULL != uartDmaHandle->dma_rx.buffer)
#else
        if ((NULL != uartDmaHandle->dma_rx.buffer) && (false == uartDmaHandle->dma_rx.receiveAll))
#endif
        {
            if ((0U != ((uint32_t)kLPUART_IdleLineFlag & status)) &&
                (0U != (LPUART_GetEnabledInterrupts(s_LpuartAdapterBase[instance]) &
                        (uint32_t)kLPUART_IdleLineInterruptEnable)))
            {
                HAL_UartDMAIdlelineInterruptHandle(instance);
                (void)LPUART_ClearStatusFlags(s_LpuartAdapterBase[instance], (uint32_t)kLPUART_IdleLineFlag);
            }
        }
    }

#endif /* HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION */
#endif /* HAL_UART_DMA_ENABLE */

#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
    /* If RX overrun. */
    if ((uint32_t)kLPUART_RxOverrunFlag == ((uint32_t)kLPUART_RxOverrunFlag & status))
    {
        /* Clear overrun flag, otherwise the RX does not work. */
        s_LpuartAdapterBase[instance]->STAT =
            ((s_LpuartAdapterBase[instance]->STAT & 0x3FE00000U) | LPUART_STAT_OR_MASK);
    }
#endif
    if ((0u != ((uint32_t)kLPUART_NoiseErrorFlag & status)) || (0u != ((uint32_t)kLPUART_FramingErrorFlag & status)) ||
        (0u != ((uint32_t)kLPUART_ParityErrorFlag & status)))
    {
        if (0u != ((uint32_t)kLPUART_NoiseErrorFlag & status))
        {
            (void)LPUART_ClearStatusFlags(s_LpuartAdapterBase[instance], (uint32_t)kLPUART_NoiseErrorFlag);
        }
        if (0u != ((uint32_t)kLPUART_FramingErrorFlag & status))
        {
            (void)LPUART_ClearStatusFlags(s_LpuartAdapterBase[instance], (uint32_t)kLPUART_FramingErrorFlag);
        }
        if (0u != ((uint32_t)kLPUART_ParityErrorFlag & status))
        {
            (void)LPUART_ClearStatusFlags(s_LpuartAdapterBase[instance], (uint32_t)kLPUART_ParityErrorFlag);
        }

        /*clean RDRF flag and drop the data with status error*/
        if (0u != ((uint32_t)(kLPUART_RxDataRegFullFlag)&status))
        {
            (void)LPUART_ReadByte(s_LpuartAdapterBase[instance]);
        }
        status = LPUART_GetStatusFlags(s_LpuartAdapterBase[instance]);
    }
#if (defined(FSL_FEATURE_LPUART_IS_LPFLEXCOMM) && (FSL_FEATURE_LPUART_IS_LPFLEXCOMM > 0U))
    if (((0U != ((uint32_t)kLPUART_RxDataRegFullFlag & status)) &&
#else
    /* Receive data register full */
    if (((0U != ((uint32_t)kLPUART_RxDataRegFullFlag & status)) &&
#endif /* FSL_FEATURE_LPUART_IS_LPFLEXCOMM */
         (0U != (LPUART_GetEnabledInterrupts(s_LpuartAdapterBase[instance]) &
                 (uint32_t)kLPUART_RxDataRegFullInterruptEnable)))
#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
        || ((0U != ((uint32_t)kLPUART_IdleLineFlag & status)) &&
            (0U !=
             (LPUART_GetEnabledInterrupts(s_LpuartAdapterBase[instance]) & (uint32_t)kLPUART_IdleLineInterruptEnable)))
#endif
    )
    {
        if (NULL != uartHandle->rx.buffer)
        {
#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
            /* Get the size that can be stored into buffer for this interrupt. */
#if defined(FSL_FEATURE_LPUART_HAS_FIFO) && FSL_FEATURE_LPUART_HAS_FIFO
            count = ((uint8_t)((s_LpuartAdapterBase[instance]->WATER & LPUART_WATER_RXCOUNT_MASK) >>
                               LPUART_WATER_RXCOUNT_SHIFT));
#else
            if (0U != (status & (uint32_t)kLPUART_RxDataRegFullInterruptEnable))
            {
                count = 1U;
            }
            else
            {
                count = 0U;
            }
#endif
            while (0u != count)
            {
                count--;
#endif
                uartHandle->rx.buffer[uartHandle->rx.bufferSofar++] = LPUART_ReadByte(s_LpuartAdapterBase[instance]);
                if (uartHandle->rx.bufferSofar >= uartHandle->rx.bufferLength)
                {
                    LPUART_DisableInterrupts(
                        s_LpuartAdapterBase[instance], (uint32_t)kLPUART_RxDataRegFullInterruptEnable |
                                                           (uint32_t)kLPUART_RxOverrunInterruptEnable
#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
                                                           | (uint32_t)kLPUART_IdleLineInterruptEnable
#endif
                    );
                    uartHandle->rx.buffer = NULL;
#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
                    count = 0u;
#endif
                    if (NULL != uartHandle->callback)
                    {
                        uartHandle->callback(uartHandle, kStatus_HAL_UartRxIdle, uartHandle->callbackParam);
                    }
                }
#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
            }
#endif
        }
#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
        if ((0U != ((uint32_t)kLPUART_IdleLineFlag & status)) &&
            (0U !=
             (LPUART_GetEnabledInterrupts(s_LpuartAdapterBase[instance]) & (uint32_t)kLPUART_IdleLineInterruptEnable)))
        {
            s_LpuartAdapterBase[instance]->STAT |= ((uint32_t)kLPUART_IdleLineFlag);
        }
#endif
    }

    /* Send data register empty and the interrupt is enabled. */
    if ((0U != (LPUART_STAT_TDRE_MASK & status)) && (0U != (LPUART_GetEnabledInterrupts(s_LpuartAdapterBase[instance]) &
                                                            (uint32_t)kLPUART_TxDataRegEmptyInterruptEnable)))
    {
        if (NULL != uartHandle->tx.buffer)
        {
#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
            /* Get the size that transmit buffer for this interrupt. */
#if defined(FSL_FEATURE_LPUART_HAS_FIFO) && FSL_FEATURE_LPUART_HAS_FIFO
            count = (uint8_t)FSL_FEATURE_LPUART_FIFO_SIZEn(s_LpuartAdapterBase[instance]) -
                    (uint8_t)((s_LpuartAdapterBase[instance]->WATER & LPUART_WATER_TXCOUNT_MASK) >>
                              LPUART_WATER_TXCOUNT_SHIFT);
#else
            count = 1u;
#endif
            while (0u != count)
            {
                count--;
#endif
                LPUART_WriteByte(s_LpuartAdapterBase[instance], uartHandle->tx.buffer[uartHandle->tx.bufferSofar++]);
                if (uartHandle->tx.bufferSofar >= uartHandle->tx.bufferLength)
                {
                    LPUART_DisableInterrupts(s_LpuartAdapterBase[instance],
                                             (uint32_t)kLPUART_TxDataRegEmptyInterruptEnable);
                    uartHandle->tx.buffer = NULL;
#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
                    count = 0u;
#endif
                    if (NULL != uartHandle->callback)
                    {
                        uartHandle->callback(uartHandle, kStatus_HAL_UartTxIdle, uartHandle->callbackParam);
                    }
                }
#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
            }
#endif
        }
    }

#if !(defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
    (void)LPUART_ClearStatusFlags(s_LpuartAdapterBase[instance], status);
#endif
}
#endif /* HAL_UART_TRANSFER_MODE */
#if (defined(FSL_FEATURE_LPUART_IS_LPFLEXCOMM) && (FSL_FEATURE_LPUART_IS_LPFLEXCOMM > 0U))
static void HAL_LpUartInterruptHandle_Wapper(uint32_t instance, void *handle)
{
    hal_uart_state_t *uartHandle = (hal_uart_state_t *)handle;
    HAL_UartInterruptHandle(uartHandle->instance);
}
#endif /* FSL_FEATURE_LPUART_IS_LPFLEXCOMM */

#endif /* UART_ADAPTER_NON_BLOCKING_MODE */

static hal_uart_status_t HAL_UartInitCommon(hal_uart_handle_t handle, const hal_uart_config_t *uart_config)
{
    lpuart_config_t lpuartConfig;
    status_t status;
    hal_uart_status_t uartStatus = kStatus_HAL_UartSuccess;

    LPUART_GetDefaultConfig(&lpuartConfig);
    lpuartConfig.baudRate_Bps = uart_config->baudRate_Bps;
    lpuartConfig.parityMode   = (lpuart_parity_mode_t)uart_config->parityMode;
    lpuartConfig.stopBitCount = (lpuart_stop_bit_count_t)uart_config->stopBitCount;
    lpuartConfig.enableRx     = (bool)uart_config->enableRx;
    lpuartConfig.enableTx     = (bool)uart_config->enableTx;
#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
#if defined(FSL_FEATURE_LPUART_HAS_FIFO) && FSL_FEATURE_LPUART_HAS_FIFO
    if (uart_config->txFifoWatermark > 0U)
    {
        lpuartConfig.txFifoWatermark =
            MIN(uart_config->txFifoWatermark,
                (uint8_t)FSL_FEATURE_LPUART_FIFO_SIZEn(s_LpuartAdapterBase[uart_config->instance])) -
            1U;
    }
    if (uart_config->rxFifoWatermark > 0U)
    {
        lpuartConfig.rxFifoWatermark =
            MIN(uart_config->rxFifoWatermark,
                (uint8_t)FSL_FEATURE_LPUART_FIFO_SIZEn(s_LpuartAdapterBase[uart_config->instance])) -
            1U;
    }
#endif
#endif
#if defined(FSL_FEATURE_LPUART_HAS_MODEM_SUPPORT) && FSL_FEATURE_LPUART_HAS_MODEM_SUPPORT
    lpuartConfig.enableRxRTS = (bool)uart_config->enableRxRTS;
    lpuartConfig.enableTxCTS = (bool)uart_config->enableTxCTS;
#endif /* FSL_FEATURE_LPUART_HAS_MODEM_SUPPORT */

    /* Idleline config */
#if (defined(HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION) && (HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION > 0U))
#else  /* HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION */
    lpuartConfig.rxIdleType   = kLPUART_IdleTypeStopBit;
    lpuartConfig.rxIdleConfig = kLPUART_IdleCharacter2;
#endif /* HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION */

    status = LPUART_Init(s_LpuartAdapterBase[uart_config->instance], (const lpuart_config_t *)&lpuartConfig, uart_config->srcClock_Hz);

    if ((int32_t)kStatus_Success != status)
    {
        uartStatus = HAL_UartGetStatus(status); /*Get current uart status*/
    }

    return uartStatus;
}

hal_uart_status_t HAL_UartInit(hal_uart_handle_t handle, const hal_uart_config_t *uart_config)
{
    hal_uart_state_t *uartHandle;
    hal_uart_status_t uartStatus;
    assert(NULL != handle);
    assert(NULL != uart_config);
    assert(uart_config->instance < (sizeof(s_LpuartAdapterBase) / sizeof(LPUART_Type *)));
    assert(NULL != s_LpuartAdapterBase[uart_config->instance]);
    assert(HAL_UART_HANDLE_SIZE >= sizeof(hal_uart_state_t));

    uartStatus = HAL_UartInitCommon(handle, uart_config);

    if (kStatus_HAL_UartSuccess == uartStatus)
    {
        uartHandle           = (hal_uart_state_t *)handle;
        uartHandle->instance = uart_config->instance;
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
        uartHandle->dmaHandle = NULL;
#endif /* HAL_UART_DMA_ENABLE */

#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))
#if (defined(HAL_UART_TRANSFER_MODE) && (HAL_UART_TRANSFER_MODE > 0U))
        LPUART_TransferCreateHandle(s_LpuartAdapterBase[uart_config->instance], &uartHandle->hardwareHandle,
                                    (lpuart_transfer_callback_t)HAL_UartCallback, handle);
#else
        s_UartState[uartHandle->instance] = uartHandle;
#if (defined(FSL_FEATURE_LPUART_IS_LPFLEXCOMM) && (FSL_FEATURE_LPUART_IS_LPFLEXCOMM > 0U))
        LP_FLEXCOMM_SetIRQHandler(uart_config->instance, HAL_LpUartInterruptHandle_Wapper, handle,
                                  LP_FLEXCOMM_PERIPH_LPUART);
#endif /* FSL_FEATURE_LPUART_IS_LPFLEXCOMM */

/* Enable interrupt in NVIC. */
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
        NVIC_SetPriority((IRQn_Type)s_LpuartRxIRQ[uartHandle->instance], HAL_UART_ISR_PRIORITY);
        EnableIRQ(s_LpuartRxIRQ[uartHandle->instance]);
        NVIC_SetPriority((IRQn_Type)s_LpuartTxIRQ[uartHandle->instance], HAL_UART_ISR_PRIORITY);
        EnableIRQ(s_LpuartTxIRQ[uartHandle->instance]);
#else
        NVIC_SetPriority((IRQn_Type)s_LpuartIRQ[uartHandle->instance], HAL_UART_ISR_PRIORITY);
        (void)EnableIRQ(s_LpuartIRQ[uartHandle->instance]);
#endif
#endif

#endif
#if (defined(HAL_UART_ADAPTER_LOWPOWER) && (HAL_UART_ADAPTER_LOWPOWER > 0U))
#if (defined(HAL_UART_ADAPTER_LOWPOWER_RESTORE) && (HAL_UART_ADAPTER_LOWPOWER_RESTORE > 0U))
        uartHandle->reg_BAUD  = s_LpuartAdapterBase[uartHandle->instance]->BAUD;
        uartHandle->reg_CTRL  = s_LpuartAdapterBase[uartHandle->instance]->CTRL;
        uartHandle->reg_WATER = s_LpuartAdapterBase[uartHandle->instance]->WATER;
        uartHandle->reg_MODIR = s_LpuartAdapterBase[uartHandle->instance]->MODIR;
#else
        (void)memcpy(&uartHandle->config, uart_config, sizeof(hal_uart_config_t));
#endif
#endif
    }

    return uartStatus;
}

hal_uart_status_t HAL_UartDeinit(hal_uart_handle_t handle)
{
    hal_uart_state_t *uartHandle;

    assert(NULL != handle);

    uartHandle = (hal_uart_state_t *)handle;

    LPUART_Deinit(s_LpuartAdapterBase[uartHandle->instance]); /*LPUART Deinitialization*/

#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))

#if !(defined(HAL_UART_TRANSFER_MODE) && (HAL_UART_TRANSFER_MODE > 0U))
    s_UartState[uartHandle->instance] = NULL;
#endif

#endif

    return kStatus_HAL_UartSuccess;
}

hal_uart_status_t HAL_UartReceiveBlocking(hal_uart_handle_t handle, uint8_t *data, size_t length)
{
    hal_uart_state_t *uartHandle;
    status_t status;
    assert(NULL != handle);
    assert(NULL != data);
    assert(length > 0U);

    uartHandle = (hal_uart_state_t *)handle;

    status = LPUART_ReadBlocking(s_LpuartAdapterBase[uartHandle->instance], data, length);

    return HAL_UartGetStatus(status);
}

hal_uart_status_t HAL_UartSendBlocking(hal_uart_handle_t handle, const uint8_t *data, size_t length)
{
    hal_uart_state_t *uartHandle;

    assert(NULL != handle);
    assert(NULL != data);
    assert(length > 0U);

    uartHandle = (hal_uart_state_t *)handle;

    (void)LPUART_WriteBlocking(s_LpuartAdapterBase[uartHandle->instance], data, length);

    return kStatus_HAL_UartSuccess;
}

hal_uart_status_t HAL_UartEnterLowpower(hal_uart_handle_t handle)
{
    assert(NULL != handle);

    return kStatus_HAL_UartSuccess;
}

hal_uart_status_t HAL_UartExitLowpower(hal_uart_handle_t handle)
{
#if (defined(HAL_UART_ADAPTER_LOWPOWER) && (HAL_UART_ADAPTER_LOWPOWER > 0U))
    hal_uart_state_t *uartHandle;
    assert(handle);

    uartHandle = (hal_uart_state_t *)handle;

#if (defined(HAL_UART_ADAPTER_LOWPOWER_RESTORE) && (HAL_UART_ADAPTER_LOWPOWER_RESTORE > 0U))
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable lpuart clock */
    CLOCK_EnableClock(s_LpuartAdapterClock[uartHandle->instance]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
    s_LpuartAdapterBase[uartHandle->instance]->BAUD  = uartHandle->reg_BAUD;
    s_LpuartAdapterBase[uartHandle->instance]->WATER = uartHandle->reg_WATER;
    s_LpuartAdapterBase[uartHandle->instance]->MODIR = uartHandle->reg_MODIR;
#if defined(FSL_FEATURE_LPUART_HAS_FIFO) && FSL_FEATURE_LPUART_HAS_FIFO
    /* Enable tx/rx FIFO */
    s_LpuartAdapterBase[uartHandle->instance]->FIFO |= (LPUART_FIFO_TXFE_MASK | LPUART_FIFO_RXFE_MASK);
    /* Flush FIFO */
    s_LpuartAdapterBase[uartHandle->instance]->FIFO |= (LPUART_FIFO_TXFLUSH_MASK | LPUART_FIFO_RXFLUSH_MASK);
#endif /* FSL_FEATURE_LPUART_HAS_FIFO */
    s_LpuartAdapterBase[uartHandle->instance]->CTRL = uartHandle->reg_CTRL;
#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))
    s_UartState[uartHandle->instance] = handle;
/* Enable interrupt in NVIC. */
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
    NVIC_SetPriority((IRQn_Type)s_LpuartRxIRQ[uartHandle->instance], HAL_UART_ISR_PRIORITY);
    EnableIRQ(s_LpuartRxIRQ[uartHandle->instance]);
    NVIC_SetPriority((IRQn_Type)s_LpuartTxIRQ[uartHandle->instance], HAL_UART_ISR_PRIORITY);
    EnableIRQ(s_LpuartTxIRQ[uartHandle->instance]);
#else
    NVIC_SetPriority((IRQn_Type)s_LpuartIRQ[uartHandle->instance], HAL_UART_ISR_PRIORITY);
    (void)EnableIRQ(s_LpuartIRQ[uartHandle->instance]);
#endif
    s_LpuartAdapterBase[uartHandle->instance]->CTRL |= LPUART_CTRL_RIE_MASK;
    HAL_UartIsrFunction(uartHandle);

#endif
#else
    (void)HAL_UartInit(handle, &uartHandle->config);
#endif
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
#if (defined(HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION) && (HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION > 0U))
#else
    /* Enable RX interrupt for detecting the IDLE line interrupt. */
    LPUART_EnableInterrupts(s_LpuartAdapterBase[uartHandle->instance], kLPUART_IdleLineInterruptEnable);
#endif /* HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION */

#if (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
    HAL_UartDMAInit(uartHandle, uartHandle->dmaHandle, &uartHandle->dmaHandle->hal_uart_dma_config);
    LPUART_StartRingBufferEDMA(handle);
    ringBufferIndex[uartHandle->instance] = 0;
#elif (defined(FSL_FEATURE_SOC_DMA_COUNT) && (FSL_FEATURE_SOC_DMA_COUNT > 0U))
#endif /* FSL_FEATURE_SOC_EDMA_COUNT */
#endif
#endif
    return kStatus_HAL_UartSuccess;
}

#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))

#if (defined(HAL_UART_TRANSFER_MODE) && (HAL_UART_TRANSFER_MODE > 0U))

hal_uart_status_t HAL_UartTransferInstallCallback(hal_uart_handle_t handle,
                                                  hal_uart_transfer_callback_t callback,
                                                  void *callbackParam)
{
    hal_uart_state_t *uartHandle;

    assert(handle);
    assert(0U != HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;

    uartHandle->callbackParam = callbackParam;
    uartHandle->callback      = callback;

    return kStatus_HAL_UartSuccess;
}

hal_uart_status_t HAL_UartTransferReceiveNonBlocking(hal_uart_handle_t handle, hal_uart_transfer_t *transfer)
{
    hal_uart_state_t *uartHandle;
    status_t status;
    assert(handle);
    assert(transfer);
    assert(0U != HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;

    status = LPUART_TransferReceiveNonBlocking(s_LpuartAdapterBase[uartHandle->instance], &uartHandle->hardwareHandle,
                                               (lpuart_transfer_t *)(void *)transfer, NULL);

    return HAL_UartGetStatus(status);
}

hal_uart_status_t HAL_UartTransferSendNonBlocking(hal_uart_handle_t handle, hal_uart_transfer_t *transfer)
{
    hal_uart_state_t *uartHandle;
    status_t status;
    assert(handle);
    assert(transfer);
    assert(0U != HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;

    status = LPUART_TransferSendNonBlocking(s_LpuartAdapterBase[uartHandle->instance], &uartHandle->hardwareHandle,
                                            (lpuart_transfer_t *)(void *)transfer);

    return HAL_UartGetStatus(status);
}

hal_uart_status_t HAL_UartTransferGetReceiveCount(hal_uart_handle_t handle, uint32_t *count)
{
    hal_uart_state_t *uartHandle;
    status_t status;
    assert(handle);
    assert(count);
    assert(0U != HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;

    status =
        LPUART_TransferGetReceiveCount(s_LpuartAdapterBase[uartHandle->instance], &uartHandle->hardwareHandle, count);

    return HAL_UartGetStatus(status);
}

hal_uart_status_t HAL_UartTransferGetSendCount(hal_uart_handle_t handle, uint32_t *count)
{
    hal_uart_state_t *uartHandle;
    status_t status;
    assert(handle);
    assert(count);
    assert(0U != HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;

    status = LPUART_TransferGetSendCount(s_LpuartAdapterBase[uartHandle->instance], &uartHandle->hardwareHandle, count);

    return HAL_UartGetStatus(status);
}

hal_uart_status_t HAL_UartTransferAbortReceive(hal_uart_handle_t handle)
{
    hal_uart_state_t *uartHandle;
    assert(handle);
    assert(0U != HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;

    LPUART_TransferAbortReceive(s_LpuartAdapterBase[uartHandle->instance], &uartHandle->hardwareHandle);

    return kStatus_HAL_UartSuccess;
}

hal_uart_status_t HAL_UartTransferAbortSend(hal_uart_handle_t handle)
{
    hal_uart_state_t *uartHandle;
    assert(handle);
    assert(0U != HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;

    LPUART_TransferAbortSend(s_LpuartAdapterBase[uartHandle->instance], &uartHandle->hardwareHandle);

    return kStatus_HAL_UartSuccess;
}

#else /* HAL_UART_TRANSFER_MODE */

/* None transactional API with non-blocking mode. */
hal_uart_status_t HAL_UartInstallCallback(hal_uart_handle_t handle,
                                          hal_uart_transfer_callback_t callback,
                                          void *callbackParam)
{
    hal_uart_state_t *uartHandle;

    assert(NULL != handle);
    assert(0U == HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;
    uartHandle->callbackParam = callbackParam;
    uartHandle->callback = callback;

    return kStatus_HAL_UartSuccess;
}

hal_uart_status_t HAL_UartReceiveNonBlocking(hal_uart_handle_t handle, uint8_t *data, size_t length)
{
    hal_uart_state_t *uartHandle;
    assert(NULL != handle);
    assert(NULL != data);
    assert(length > 0U);
    assert(0U == HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;
    if (NULL != uartHandle->rx.buffer)
    {
        return kStatus_HAL_UartRxBusy;
    }

    uartHandle->rx.bufferLength = length;
    uartHandle->rx.bufferSofar = 0;
    uartHandle->rx.buffer = data;
    LPUART_EnableInterrupts(s_LpuartAdapterBase[uartHandle->instance], (uint32_t)kLPUART_RxDataRegFullInterruptEnable |
                                                                           (uint32_t)kLPUART_RxOverrunInterruptEnable
#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
                                                                           | (uint32_t)kLPUART_IdleLineInterruptEnable
#endif
    );
    return kStatus_HAL_UartSuccess;
}

hal_uart_status_t HAL_UartSendNonBlocking(hal_uart_handle_t handle, uint8_t *data, size_t length)
{
    hal_uart_state_t *uartHandle;
    assert(NULL != handle);
    assert(NULL != data);
    assert(length > 0U);
    assert(0U == HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;
    if (NULL != uartHandle->tx.buffer)
    {
        return kStatus_HAL_UartTxBusy;
    }
    uartHandle->tx.bufferLength = length;
    uartHandle->tx.bufferSofar = 0;
    uartHandle->tx.buffer = data;
    LPUART_EnableInterrupts(s_LpuartAdapterBase[uartHandle->instance], (uint32_t)kLPUART_TxDataRegEmptyInterruptEnable);
    return kStatus_HAL_UartSuccess;
}

hal_uart_status_t HAL_UartGetReceiveCount(hal_uart_handle_t handle, uint32_t *reCount)
{
    hal_uart_state_t *uartHandle;
    assert(NULL != handle);
    assert(NULL != reCount);
    assert(0U == HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;
    if (NULL != uartHandle->rx.buffer)
    {
        *reCount = uartHandle->rx.bufferSofar;
        return kStatus_HAL_UartSuccess;
    }
    return kStatus_HAL_UartError;
}

hal_uart_status_t HAL_UartGetSendCount(hal_uart_handle_t handle, uint32_t *seCount)
{
    hal_uart_state_t *uartHandle;
    assert(NULL != handle);
    assert(NULL != seCount);
    assert(0U == HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;
    if (NULL != uartHandle->tx.buffer)
    {
        *seCount = uartHandle->tx.bufferSofar;
        return kStatus_HAL_UartSuccess;
    }
    return kStatus_HAL_UartError;
}

hal_uart_status_t HAL_UartAbortReceive(hal_uart_handle_t handle)
{
    hal_uart_state_t *uartHandle;
    assert(NULL != handle);
    assert(0U == HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;

    if (NULL != uartHandle->rx.buffer)
    {
        LPUART_DisableInterrupts(
            s_LpuartAdapterBase[uartHandle->instance],
            (uint32_t)kLPUART_RxDataRegFullInterruptEnable | (uint32_t)kLPUART_RxOverrunInterruptEnable);
        uartHandle->rx.buffer = NULL;
    }

    return kStatus_HAL_UartSuccess;
}

hal_uart_status_t HAL_UartAbortSend(hal_uart_handle_t handle)
{
    hal_uart_state_t *uartHandle;
    assert(NULL != handle);
    assert(0U == HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;

    if (NULL != uartHandle->tx.buffer)
    {
        LPUART_DisableInterrupts(s_LpuartAdapterBase[uartHandle->instance],
                                 (uint32_t)kLPUART_TxDataRegEmptyInterruptEnable);
        uartHandle->tx.buffer = NULL;
    }

    return kStatus_HAL_UartSuccess;
}

#endif /* HAL_UART_TRANSFER_MODE */

#if (defined(HAL_UART_TRANSFER_MODE) && (HAL_UART_TRANSFER_MODE > 0U))

void HAL_UartIsrFunction(hal_uart_handle_t handle)
{
    hal_uart_state_t *uartHandle;
    assert(NULL != handle);
    assert(0U != HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;

#if 0
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
    DisableIRQ(s_LpuartRxIRQ[uartHandle->instance]);
    DisableIRQ(s_LpuartTxIRQ[uartHandle->instance]);
#else
    DisableIRQ(s_LpuartIRQ[uartHandle->instance]);
#endif
#endif
    LPUART_TransferHandleIRQ(s_LpuartAdapterBase[uartHandle->instance], &uartHandle->hardwareHandle);
#if 0
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
    NVIC_SetPriority((IRQn_Type)s_LpuartRxIRQ[uartHandle->instance], HAL_UART_ISR_PRIORITY);
    EnableIRQ(s_LpuartRxIRQ[uartHandle->instance]);
    NVIC_SetPriority((IRQn_Type)s_LpuartTxIRQ[uartHandle->instance], HAL_UART_ISR_PRIORITY);
    EnableIRQ(s_LpuartTxIRQ[uartHandle->instance]);
#else
    NVIC_SetPriority((IRQn_Type)s_LpuartIRQ[uartHandle->instance], HAL_UART_ISR_PRIORITY);
    EnableIRQ(s_LpuartIRQ[uartHandle->instance]);
#endif
#endif
}

#else /* HAL_UART_TRANSFER_MODE */

void HAL_UartIsrFunction(hal_uart_handle_t handle)
{
    hal_uart_state_t *uartHandle;
    assert(NULL != handle);
    assert(0U == HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
    hal_uart_dma_state_t *uartDmaHandle = s_UartDmaState[uartHandle->instance];
#endif
#if 0
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
    DisableIRQ(s_LpuartRxIRQ[uartHandle->instance]);
    DisableIRQ(s_LpuartTxIRQ[uartHandle->instance]);
#else
    DisableIRQ(s_LpuartIRQ[uartHandle->instance]);
#endif
#endif
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
#if (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
    if ((NULL != uartDmaHandle) && (uartDmaHandle->dma_tx.buffer != NULL))
    {
        EDMA_HandleIRQ(&uartHandle->dmaHandle->txEdmaHandle);
    }
#endif
#endif

    HAL_UartInterruptHandle(uartHandle->instance);

#if 0
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
    NVIC_SetPriority((IRQn_Type)s_LpuartRxIRQ[uartHandle->instance], HAL_UART_ISR_PRIORITY);
    EnableIRQ(s_LpuartRxIRQ[uartHandle->instance]);
    NVIC_SetPriority((IRQn_Type)s_LpuartTxIRQ[uartHandle->instance], HAL_UART_ISR_PRIORITY);
    EnableIRQ(s_LpuartTxIRQ[uartHandle->instance]);
#else
    NVIC_SetPriority((IRQn_Type)s_LpuartIRQ[uartHandle->instance], HAL_UART_ISR_PRIORITY);
    EnableIRQ(s_LpuartIRQ[uartHandle->instance]);
#endif
#endif
}

#if defined(FSL_FEATURE_LPUART_HAS_SHARED_IRQ0_IRQ1) && FSL_FEATURE_LPUART_HAS_SHARED_IRQ0_IRQ1
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
void LPUART0_LPUART1_RX_IRQHandler(void)
{
    if ((s_UartState[0]))
    {
        if ((LPUART_STAT_OR_MASK & LPUART0->STAT) ||
            ((LPUART_STAT_RDRF_MASK & LPUART0->STAT) && (LPUART_CTRL_RIE_MASK & LPUART0->CTRL)) ||
            ((LPUART_STAT_IDLE_MASK & LPUART0->STAT) && (LPUART_STAT_IDLE_MASK & LPUART0->CTRL)))
        {
            HAL_UartInterruptHandle(0);
        }
    }
    if ((s_UartState[1]))
    {
        if ((LPUART_STAT_OR_MASK & LPUART1->STAT) ||
            ((LPUART_STAT_RDRF_MASK & LPUART1->STAT) && (LPUART_CTRL_RIE_MASK & LPUART1->CTRL)) ||
            ((LPUART_STAT_IDLE_MASK & LPUART1->STAT) && (LPUART_STAT_IDLE_MASK & LPUART1->CTRL)))
        {
            HAL_UartInterruptHandle(1);
        }
    }
    SDK_ISR_EXIT_BARRIER;
}
void LPUART0_LPUART1_TX_IRQHandler(void)
{
    if ((s_UartState[0]))
    {
        if ((LPUART_STAT_OR_MASK & LPUART0->STAT) ||
            ((LPUART0->STAT & LPUART_STAT_TDRE_MASK) && (LPUART0->CTRL & LPUART_CTRL_TIE_MASK)) ||
            ((LPUART_CTRL_TCIE_MASK & LPUART0->STAT) && (LPUART_CTRL_TCIE_MASK & LPUART0->CTRL)))
        {
            HAL_UartInterruptHandle(0);
        }
    }
    if ((s_UartState[1]))
    {
        if ((LPUART_STAT_OR_MASK & LPUART1->STAT) ||
            ((LPUART1->STAT & LPUART_STAT_TDRE_MASK) && (LPUART1->CTRL & LPUART_CTRL_TIE_MASK)) ||
            ((LPUART_CTRL_TCIE_MASK & LPUART1->STAT) && (LPUART_CTRL_TCIE_MASK & LPUART1->CTRL)))
        {
            HAL_UartInterruptHandle(1);
        }
    }
    SDK_ISR_EXIT_BARRIER;
}
#else  /* FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ */
void LPUART0_LPUART1_IRQHandler(void);
void LPUART0_LPUART1_IRQHandler(void)
{
    uint32_t orMask;
    uint32_t rdrfMask;
    uint32_t rieMask;
    uint32_t tdreMask;
    uint32_t tieMask;
    uint32_t ilieMask;
    uint32_t tcieMask;
    if (NULL != (s_UartState[0]))
    {
        orMask = LPUART_STAT_OR_MASK & LPUART0->STAT;
        rdrfMask = LPUART_STAT_RDRF_MASK & LPUART0->STAT;
        rieMask = LPUART_CTRL_RIE_MASK & LPUART0->CTRL;
        tdreMask = LPUART0->STAT & LPUART_STAT_TDRE_MASK;
        tieMask = LPUART0->CTRL & LPUART_CTRL_TIE_MASK;
        ilieMask = LPUART0->STAT & LPUART_CTRL_ILIE_MASK;
        tcieMask = LPUART0->STAT & LPUART_CTRL_TCIE_MASK;
        if ((bool)orMask || ((bool)rdrfMask && (bool)rieMask) || ((bool)tdreMask && (bool)tieMask) || (bool)ilieMask ||
            (bool)tcieMask)
        {
            HAL_UartInterruptHandle(0);
        }
    }
    if (NULL != (s_UartState[1]))
    {
        orMask = LPUART_STAT_OR_MASK & LPUART1->STAT;
        rdrfMask = LPUART_STAT_RDRF_MASK & LPUART1->STAT;
        rieMask = LPUART_CTRL_RIE_MASK & LPUART1->CTRL;
        tdreMask = LPUART1->STAT & LPUART_STAT_TDRE_MASK;
        tieMask = LPUART1->CTRL & LPUART_CTRL_TIE_MASK;
        ilieMask = LPUART1->STAT & LPUART_CTRL_ILIE_MASK;
        tcieMask = LPUART1->STAT & LPUART_CTRL_TCIE_MASK;
        if ((bool)orMask || ((bool)rdrfMask && (bool)rieMask) || ((bool)tdreMask && (bool)tieMask) || (bool)ilieMask ||
            (bool)tcieMask)
        {
            HAL_UartInterruptHandle(1);
        }
    }
    SDK_ISR_EXIT_BARRIER;
}
#endif /* FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ */
#endif /* FSL_FEATURE_LPUART_HAS_SHARED_IRQ0_IRQ1 */

#if defined(LPUART0)
#if !(defined(FSL_FEATURE_LPUART_HAS_SHARED_IRQ0_IRQ1) && FSL_FEATURE_LPUART_HAS_SHARED_IRQ0_IRQ1)
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
void LPUART0_TX_IRQHandler(void)
{
    HAL_UartInterruptHandle(0);
    SDK_ISR_EXIT_BARRIER;
}
void LPUART0_RX_IRQHandler(void)
{
    HAL_UartInterruptHandle(0);
    SDK_ISR_EXIT_BARRIER;
}
#else
void LPUART0_IRQHandler(void);
void LPUART0_IRQHandler(void)
{
    HAL_UartInterruptHandle(0);
    SDK_ISR_EXIT_BARRIER;
}
#endif /* FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ */
#endif /* FSL_FEATURE_LPUART_HAS_SHARED_IRQ0_IRQ1 */
#endif /* LPUART0 */

#if defined(LPUART1)
#if !(defined(FSL_FEATURE_LPUART_HAS_SHARED_IRQ0_IRQ1) && FSL_FEATURE_LPUART_HAS_SHARED_IRQ0_IRQ1)
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
void LPUART1_TX_IRQHandler(void)
{
    HAL_UartInterruptHandle(1);
    SDK_ISR_EXIT_BARRIER;
}
void LPUART1_RX_IRQHandler(void)
{
    HAL_UartInterruptHandle(1);
    SDK_ISR_EXIT_BARRIER;
}
#else  /* FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ */
void LPUART1_IRQHandler(void);
void LPUART1_IRQHandler(void)
{
    HAL_UartInterruptHandle(1);
    SDK_ISR_EXIT_BARRIER;
}
#endif /* FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ */
#endif /* FSL_FEATURE_LPUART_HAS_SHARED_IRQ0_IRQ1 */
#endif /* LPUART1 */

#if defined(LPUART2)
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
void LPUART2_TX_IRQHandler(void)
{
    HAL_UartInterruptHandle(2);
    SDK_ISR_EXIT_BARRIER;
}
void LPUART2_RX_IRQHandler(void)
{
    HAL_UartInterruptHandle(2);
    SDK_ISR_EXIT_BARRIER;
}
#else
void LPUART2_IRQHandler(void);
void LPUART2_IRQHandler(void)
{
    HAL_UartInterruptHandle(2);
    SDK_ISR_EXIT_BARRIER;
}
#endif
#endif /* LPUART2 */

#if defined(LPUART3)
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
void LPUART3_TX_IRQHandler(void)
{
    HAL_UartInterruptHandle(3);
    SDK_ISR_EXIT_BARRIER;
}
void LPUART3_RX_IRQHandler(void)
{
    HAL_UartInterruptHandle(3);
    SDK_ISR_EXIT_BARRIER;
}
#else
void LPUART3_IRQHandler(void);
void LPUART3_IRQHandler(void)
{
    HAL_UartInterruptHandle(3);
    SDK_ISR_EXIT_BARRIER;
}
#endif
#endif /* LPUART3 */

#if defined(LPUART4)
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
void LPUART4_TX_IRQHandler(void)
{
    HAL_UartInterruptHandle(4);
    SDK_ISR_EXIT_BARRIER;
}
void LPUART4_RX_IRQHandler(void)
{
    HAL_UartInterruptHandle(4);
    SDK_ISR_EXIT_BARRIER;
}
#else
void LPUART4_IRQHandler(void);
void LPUART4_IRQHandler(void)
{
    HAL_UartInterruptHandle(4);
    SDK_ISR_EXIT_BARRIER;
}
#endif
#endif /* LPUART4 */

#if defined(LPUART5)
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
void LPUART5_TX_IRQHandler(void)
{
    HAL_UartInterruptHandle(5);
    SDK_ISR_EXIT_BARRIER;
}
void LPUART5_RX_IRQHandler(void)
{
    HAL_UartInterruptHandle(5);
    SDK_ISR_EXIT_BARRIER;
}
#else
void LPUART5_IRQHandler(void);
void LPUART5_IRQHandler(void)
{
    HAL_UartInterruptHandle(5);
    SDK_ISR_EXIT_BARRIER;
}
#endif
#endif /* LPUART5 */

#if defined(LPUART6)
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
void LPUART6_TX_IRQHandler(void)
{
    HAL_UartInterruptHandle(6);
    SDK_ISR_EXIT_BARRIER;
}
void LPUART6_RX_IRQHandler(void)
{
    HAL_UartInterruptHandle(6);
    SDK_ISR_EXIT_BARRIER;
}
#else
void LPUART6_IRQHandler(void);
void LPUART6_IRQHandler(void)
{
    HAL_UartInterruptHandle(6);
    SDK_ISR_EXIT_BARRIER;
}
#endif
#endif /* LPUART6 */

#if defined(LPUART7)
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
void LPUART7_TX_IRQHandler(void)
{
    HAL_UartInterruptHandle(7);
    SDK_ISR_EXIT_BARRIER;
}
void LPUART7_RX_IRQHandler(void)
{
    HAL_UartInterruptHandle(7);
    SDK_ISR_EXIT_BARRIER;
}
#else
void LPUART7_IRQHandler(void);
void LPUART7_IRQHandler(void)
{
    HAL_UartInterruptHandle(7);
    SDK_ISR_EXIT_BARRIER;
}
#endif
#endif /* LPUART7 */

#if defined(LPUART8)
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
void LPUART8_TX_IRQHandler(void)
{
    HAL_UartInterruptHandle(8);
    SDK_ISR_EXIT_BARRIER;
}
void LPUART8_RX_IRQHandler(void)
{
    HAL_UartInterruptHandle(8);
    SDK_ISR_EXIT_BARRIER;
}
#else
void LPUART8_IRQHandler(void);
void LPUART8_IRQHandler(void)
{
    HAL_UartInterruptHandle(8);
    SDK_ISR_EXIT_BARRIER;
}
#endif
#endif /* LPUART8 */

#if defined(LPUART9)
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
void LPUART9_TX_IRQHandler(void)
{
    HAL_UartInterruptHandle(9);
    SDK_ISR_EXIT_BARRIER;
}
void LPUART9_RX_IRQHandler(void)
{
    HAL_UartInterruptHandle(9);
    SDK_ISR_EXIT_BARRIER;
}
#else
void LPUART9_IRQHandler(void);
void LPUART9_IRQHandler(void)
{
    HAL_UartInterruptHandle(9);
    SDK_ISR_EXIT_BARRIER;
}
#endif
#endif /* LPUART9 */

#if defined(LPUART10)
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
void LPUART10_TX_IRQHandler(void)
{
    HAL_UartInterruptHandle(10);
    SDK_ISR_EXIT_BARRIER;
}
void LPUART10_RX_IRQHandler(void)
{
    HAL_UartInterruptHandle(10);
    SDK_ISR_EXIT_BARRIER;
}
#else
void LPUART10_IRQHandler(void);
void LPUART10_IRQHandler(void)
{
    HAL_UartInterruptHandle(10);
    SDK_ISR_EXIT_BARRIER;
}
#endif
#endif /* LPUART10 */

#if defined(LPUART11)
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
void LPUART11_TX_IRQHandler(void)
{
    HAL_UartInterruptHandle(11);
    SDK_ISR_EXIT_BARRIER;
}
void LPUART11_RX_IRQHandler(void)
{
    HAL_UartInterruptHandle(11);
    SDK_ISR_EXIT_BARRIER;
}
#else
void LPUART11_IRQHandler(void);
void LPUART11_IRQHandler(void)
{
    HAL_UartInterruptHandle(11);
    SDK_ISR_EXIT_BARRIER;
}
#endif
#endif /* LPUART11 */

#if defined(LPUART12)
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
void LPUART12_TX_IRQHandler(void)
{
    HAL_UartInterruptHandle(12);
    SDK_ISR_EXIT_BARRIER;
}
void LPUART12_RX_IRQHandler(void)
{
    HAL_UartInterruptHandle(12);
    SDK_ISR_EXIT_BARRIER;
}
#else
void LPUART12_IRQHandler(void);
void LPUART12_IRQHandler(void)
{
    HAL_UartInterruptHandle(12);
    SDK_ISR_EXIT_BARRIER;
}
#endif
#endif /* LPUART12 */

#if defined(CM4_0__LPUART)
void M4_0_LPUART_IRQHandler(void);
void M4_0_LPUART_IRQHandler(void)
{
    HAL_UartInterruptHandle(LPUART_GetInstance(CM4_0__LPUART));
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(CM4_1__LPUART)
void M4_1_LPUART_IRQHandler(void);
void M4_1_LPUART_IRQHandler(void)
{
    HAL_UartInterruptHandle(LPUART_GetInstance(CM4_1__LPUART));
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(CM4__LPUART)
void M4_LPUART_IRQHandler(void);
void M4_LPUART_IRQHandler(void)
{
    HAL_UartInterruptHandle((uint8_t)LPUART_GetInstance(CM4__LPUART));
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(DMA__LPUART0)
void DMA_UART0_INT_IRQHandler(void)
{
    HAL_UartInterruptHandle(LPUART_GetInstance(DMA__LPUART0));
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(DMA__LPUART1)
void DMA_UART1_INT_IRQHandler(void)
{
    HAL_UartInterruptHandle(LPUART_GetInstance(DMA__LPUART1));
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(DMA__LPUART2)
void DMA_UART2_INT_IRQHandler(void)
{
    HAL_UartInterruptHandle(LPUART_GetInstance(DMA__LPUART2));
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(DMA__LPUART3)
void DMA_UART3_INT_IRQHandler(void)
{
    HAL_UartInterruptHandle(LPUART_GetInstance(DMA__LPUART3));
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(DMA__LPUART4)
void DMA_UART4_INT_IRQHandler(void)
{
    HAL_UartInterruptHandle(LPUART_GetInstance(DMA__LPUART4));
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(ADMA__LPUART0)
void ADMA_UART0_INT_IRQHandler(void);
void ADMA_UART0_INT_IRQHandler(void)
{
    HAL_UartInterruptHandle((uint8_t)LPUART_GetInstance(ADMA__LPUART0));
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(ADMA__LPUART1)
void ADMA_UART1_INT_IRQHandler(void);
void ADMA_UART1_INT_IRQHandler(void)
{
    HAL_UartInterruptHandle((uint8_t)LPUART_GetInstance(ADMA__LPUART1));
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(ADMA__LPUART2)
void ADMA_UART2_INT_IRQHandler(void);
void ADMA_UART2_INT_IRQHandler(void)
{
    HAL_UartInterruptHandle((uint8_t)LPUART_GetInstance(ADMA__LPUART2));
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(ADMA__LPUART3)
void ADMA_UART3_INT_IRQHandler(void);
void ADMA_UART3_INT_IRQHandler(void)
{
    HAL_UartInterruptHandle((uint8_t)LPUART_GetInstance(ADMA__LPUART3));
    SDK_ISR_EXIT_BARRIER;
}
#endif

#endif /* HAL_UART_TRANSFER_MODE */

#endif /* UART_ADAPTER_NON_BLOCKING_MODE */

#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
#if (defined(HAL_UART_DMA_RING_BUFFER_ENABLE) && (HAL_UART_DMA_RING_BUFFER_ENABLE > 0U))
static volatile uint32_t ringBufferFlag = 0U;
/* LPUART RX EDMA call back. */
static void LPUART_RxEDMACallback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if (true == transferDone)
    {
        ringBufferFlag++;
    }
}

/* Start ring buffer. */
static void LPUART_StartRingBufferEDMA(hal_uart_handle_t handle)
{
    hal_uart_state_t *uartHandle;
    hal_uart_dma_state_t *uartDmaHandle;

    assert(handle);

    uartHandle    = (hal_uart_state_t *)handle;
    uartDmaHandle = uartHandle->dmaHandle;

    edma_transfer_config_t xferConfig;

    /* Install TCD memory for using only one TCD queue. */
    EDMA_InstallTCDMemory(&uartDmaHandle->rxEdmaHandle, (edma_tcd_t *)&tcdMemoryPoolPtr[uartHandle->instance], 1U);

    /* Prepare transfer to receive data to ring buffer. */
    EDMA_PrepareTransfer(&xferConfig,
                         (void *)(uint32_t *)LPUART_GetDataRegisterAddress(s_LpuartAdapterBase[uartHandle->instance]),
                         sizeof(uint8_t), &s_ringBuffer[uartHandle->instance], sizeof(uint8_t), sizeof(uint8_t),
                         LPUART_RING_BUFFER_SIZE, kEDMA_PeripheralToMemory);

    /* Submit transfer. */
    uartDmaHandle->rxEdmaHandle.tcdUsed = 1U;
    uartDmaHandle->rxEdmaHandle.tail    = 0U;
    EDMA_TcdReset(&uartDmaHandle->rxEdmaHandle.tcdPool[0U]);
    EDMA_TcdSetTransferConfig(&uartDmaHandle->rxEdmaHandle.tcdPool[0U], &xferConfig,
                              tcdMemoryPoolPtr[uartHandle->instance]);

    /* Enable major interrupt for counting received bytes. */
    uartDmaHandle->rxEdmaHandle.tcdPool[0U].CSR |= 0x2U;

    /* There is no live chain, TCD block need to be installed in TCD registers. */
    EDMA_InstallTCD(uartDmaHandle->rxEdmaHandle.base, uartDmaHandle->rxEdmaHandle.channel,
                    &uartDmaHandle->rxEdmaHandle.tcdPool[0U]);

    /* Setup call back function. */
    EDMA_SetCallback(&uartDmaHandle->rxEdmaHandle, LPUART_RxEDMACallback, NULL);

    /* Start EDMA transfer. */
    EDMA_StartTransfer(&uartDmaHandle->rxEdmaHandle);

    /* Enable LPUART RX EDMA. */
    LPUART_EnableRxDMA(s_LpuartAdapterBase[uartHandle->instance], true);

    /* Enable RX interrupt for detecting the IDLE line interrupt. */
    LPUART_EnableInterrupts(s_LpuartAdapterBase[uartHandle->instance], (uint32_t)kLPUART_IdleLineInterruptEnable);
    //   EnableIRQ(s_LpuartRxIRQ[uartHandle->instance]);
}
#endif
#if (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
static void LPUART_DMACallbacks(LPUART_Type *base, lpuart_edma_handle_t *handle, status_t status, void *userData)
{
    hal_uart_dma_state_t *uartDmaHandle;
    hal_uart_status_t uartStatus = HAL_UartGetStatus(status);
    hal_dma_callback_msg_t dmaMsg;
    assert(handle);

    uartDmaHandle = (hal_uart_dma_state_t *)userData;

    if (NULL != uartDmaHandle->dma_callback)
    {
        if (kStatus_HAL_UartTxIdle == uartStatus)
        {
            dmaMsg.status                   = kStatus_HAL_UartDmaTxIdle;
            dmaMsg.data                     = uartDmaHandle->dma_tx.buffer;
            dmaMsg.dataSize                 = uartDmaHandle->dma_tx.bufferLength;
            uartDmaHandle->dma_tx.buffer = NULL;
        }
        else if (kStatus_HAL_UartRxIdle == uartStatus)
        {
            dmaMsg.status                   = kStatus_HAL_UartDmaRxIdle;
            dmaMsg.data                     = uartDmaHandle->dma_rx.buffer;
            dmaMsg.dataSize                 = uartDmaHandle->dma_rx.bufferLength;
            uartDmaHandle->dma_rx.buffer = NULL;
        }
        else
        {
            /* MISRA */
        }

        uartDmaHandle->dma_callback(uartDmaHandle, &dmaMsg, uartDmaHandle->dma_callback_param);
    }
}
#endif
#if (defined(HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION) && (HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION > 0U))
static void TimeoutTimer_Callbcak(void *param)
{
    hal_lpuart_dma_list_t *uartDmaHandleList;
    hal_uart_dma_state_t *uartDmaHandle;
    hal_dma_callback_msg_t dmaMsg;
    uint32_t newReceived = 0U;

    uartDmaHandleList = (hal_lpuart_dma_list_t *)param;
    uartDmaHandle     = uartDmaHandleList->dma_list;

    while (NULL != uartDmaHandle)
    {
        if ((NULL != uartDmaHandle->dma_rx.buffer) && (false == uartDmaHandle->dma_rx.receiveAll))
        {
            /* HAL_UartDMAGetReceiveCount(uartDmaHandle, &msg.dataSize); */
#if (defined(FSL_FEATURE_SOC_DMA_COUNT) && (FSL_FEATURE_SOC_DMA_COUNT > 0U))

#elif (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
            LPUART_TransferGetReceiveCountEDMA(s_LpuartAdapterBase[uartDmaHandle->instance], &uartDmaHandle->edmaHandle,
                                               &dmaMsg.dataSize);
#endif /* FSL_FEATURE_SOC_EDMA_COUNT */
            newReceived                       = dmaMsg.dataSize - uartDmaHandle->dma_rx.bufferSofar;
            uartDmaHandle->dma_rx.bufferSofar = dmaMsg.dataSize;

            /* 1, If it is in idle state. */
            if ((0U == newReceived) && (0U < uartDmaHandle->dma_rx.bufferSofar))
            {
                uartDmaHandle->dma_rx.timeout++;
                if (uartDmaHandle->dma_rx.timeout >= HAL_UART_DMA_IDLELINE_TIMEOUT)
                {
                    /* HAL_UartDMAAbortReceive(uartDmaHandle); */
#if (defined(FSL_FEATURE_SOC_DMA_COUNT) && (FSL_FEATURE_SOC_DMA_COUNT > 0U))

#elif (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
                    LPUART_TransferAbortReceiveEDMA(s_LpuartAdapterBase[uartDmaHandle->instance],
                                                    &uartDmaHandle->edmaHandle);
#endif /* FSL_FEATURE_SOC_EDMA_COUNT */
                    dmaMsg.data                     = uartDmaHandle->dma_rx.buffer;
                    dmaMsg.status                   = kStatus_HAL_UartDmaIdleline;
                    uartDmaHandle->dma_rx.buffer = NULL;
                    uartDmaHandle->dma_callback(uartDmaHandle, &dmaMsg, uartDmaHandle->dma_callback_param);
                }
            }
            /* 2, If got new data again. */
            if ((0U < newReceived) && (0U < uartDmaHandle->dma_rx.bufferSofar))
            {
                uartDmaHandle->dma_rx.timeout = 0U;
            }
        }

        uartDmaHandle = uartDmaHandle->next;
    }
}
#endif /* HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION */

hal_uart_dma_status_t HAL_UartDMAInit(hal_uart_handle_t handle,
                                      hal_uart_dma_handle_t dmaHandle,
                                      hal_uart_dma_config_t *dmaConfig)
{
    hal_uart_state_t *uartHandle;
    hal_uart_dma_state_t *uartDmaHandle;
#if (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
#if (defined(HAL_UART_DMA_INIT_ENABLE) && (HAL_UART_DMA_INIT_ENABLE > 0U))
    edma_config_t config;
#endif /* HAL_UART_DMA_INIT_ENABLE > 0 */
#endif
    assert(handle);

    uartHandle            = (hal_uart_state_t *)handle;
    uartDmaHandle         = (hal_uart_dma_state_t *)dmaHandle;
    uartHandle->dmaHandle = uartDmaHandle;

    /* DMA init process. */
    uartDmaHandle->instance = dmaConfig->uart_instance;

#if (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
#if (defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && (FSL_FEATURE_SOC_DMAMUX_COUNT > 0U))
    dma_mux_configure_t *dmaMux = dmaConfig->dma_mux_configure;
    /* Set channel for LPUART */
    DMAMUX_Type *dmaMuxBases[] = DMAMUX_BASE_PTRS;
    DMAMUX_Init(dmaMuxBases[dmaMux->dma_dmamux_configure.dma_mux_instance]);
    DMAMUX_SetSource(dmaMuxBases[dmaMux->dma_dmamux_configure.dma_mux_instance], dmaConfig->tx_channel,
                     (int32_t)dmaMux->dma_dmamux_configure.tx_request);
    DMAMUX_SetSource(dmaMuxBases[dmaMux->dma_dmamux_configure.dma_mux_instance], dmaConfig->rx_channel,
                     (int32_t)dmaMux->dma_dmamux_configure.rx_request);
    DMAMUX_EnableChannel(dmaMuxBases[dmaMux->dma_dmamux_configure.dma_mux_instance], dmaConfig->tx_channel);
    DMAMUX_EnableChannel(dmaMuxBases[dmaMux->dma_dmamux_configure.dma_mux_instance], dmaConfig->rx_channel);
#if (defined(HAL_UART_ADAPTER_LOWPOWER) && (HAL_UART_ADAPTER_LOWPOWER > 0U))
    (void)memcpy(&uartDmaHandle->dma_mux_configure, dmaConfig->dma_mux_configure, sizeof(dma_mux_configure_t));
#endif /* HAL_UART_ADAPTER_LOWPOWER */
#endif /* FSL_FEATURE_SOC_DMAMUX_COUNT */
    /* Init the EDMA module */
#if defined(EDMA_BASE_PTRS)
    EDMA_Type *dmaBases[]                                             = EDMA_BASE_PTRS;
    IRQn_Type s_edmaIRQNumbers[][FSL_FEATURE_EDMA_MODULE_CHANNEL] = EDMA_CHN_IRQS;
#elif (defined(FSL_FEATURE_LPUART_IS_LPFLEXCOMM) && (FSL_FEATURE_LPUART_IS_LPFLEXCOMM > 0U))
    DMA_Type *dmaBases[]                                              = DMA_BASE_PTRS;
    IRQn_Type s_edmaIRQNumbers[][FSL_FEATURE_EDMA_MODULE_CHANNEL] = DMA_CHN_IRQS;
#else
    DMA_Type *dmaBases[]                                          = DMA_BASE_PTRS;
    IRQn_Type s_edmaIRQNumbers[][FSL_FEATURE_EDMA_MODULE_CHANNEL] = DMA_CHN_IRQS;
#endif

#if (defined(HAL_UART_DMA_INIT_ENABLE) && (HAL_UART_DMA_INIT_ENABLE > 0U))
    EDMA_GetDefaultConfig(&config);
#if defined FSL_FEATURE_EDMA_HAS_CHANNEL_CONFIG && FSL_FEATURE_EDMA_HAS_CHANNEL_CONFIG
    edma_channel_config_t channelConfig = {
        .enableMasterIDReplication = true,
#if !(defined(FSL_FEATURE_EDMA_HAS_NO_CH_SBR_SEC) && FSL_FEATURE_EDMA_HAS_NO_CH_SBR_SEC)
        .securityLevel             = kEDMA_ChannelSecurityLevelSecure,
#endif
        .protectionLevel           = kEDMA_ChannelProtectionLevelPrivileged,
    };

    config.enableMasterIdReplication            = true;
    config.channelConfig[dmaConfig->tx_channel] = &channelConfig;
    config.channelConfig[dmaConfig->rx_channel] = &channelConfig;
#endif
    EDMA_Init(dmaBases[dmaConfig->dma_instance], &config);
#endif /* HAL_UART_DMA_INIT_ENABLE > 0 */
    EDMA_CreateHandle(&uartDmaHandle->txEdmaHandle, dmaBases[dmaConfig->dma_instance], dmaConfig->tx_channel);
    EDMA_CreateHandle(&uartDmaHandle->rxEdmaHandle, dmaBases[dmaConfig->dma_instance], dmaConfig->rx_channel);
#if (defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && (FSL_FEATURE_EDMA_HAS_CHANNEL_MUX > 0U))
    dma_channel_mux_configure_t *dmaChannelMux = dmaConfig->dma_channel_mux_configure;
    EDMA_SetChannelMux(dmaBases[dmaConfig->dma_instance], dmaConfig->tx_channel,
                       (int32_t)dmaChannelMux->dma_dmamux_configure.dma_tx_channel_mux);
    EDMA_SetChannelMux(dmaBases[dmaConfig->dma_instance], dmaConfig->rx_channel,
                       (int32_t)dmaChannelMux->dma_dmamux_configure.dma_rx_channel_mux);
#if (defined(HAL_UART_ADAPTER_LOWPOWER) && (HAL_UART_ADAPTER_LOWPOWER > 0U))
    (void)memcpy(&uartDmaHandle->dma_channel_mux_configure, dmaConfig->dma_channel_mux_configure,
                 sizeof(dma_channel_mux_configure_t));
#endif /* HAL_UART_ADAPTER_LOWPOWER */
#endif /* FSL_FEATURE_EDMA_HAS_CHANNEL_MUX */
#if (defined(HAL_UART_ADAPTER_LOWPOWER) && (HAL_UART_ADAPTER_LOWPOWER > 0U))
    (void)memcpy(&uartDmaHandle->hal_uart_dma_config, dmaConfig, sizeof(hal_uart_dma_config_t));
    uartDmaHandle->hal_uart_dma_config.dma_mux_configure         = &uartDmaHandle->dma_mux_configure;
    uartDmaHandle->hal_uart_dma_config.dma_channel_mux_configure = &uartDmaHandle->dma_channel_mux_configure;
#endif /* HAL_UART_ADAPTER_LOWPOWER */
#elif (defined(FSL_FEATURE_SOC_DMA_COUNT) && (FSL_FEATURE_SOC_DMA_COUNT > 0U))
#endif /* FSL_FEATURE_SOC_EDMA_COUNT */
#if (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
    NVIC_SetPriority(s_edmaIRQNumbers[dmaConfig->dma_instance][dmaConfig->tx_channel], HAL_UART_ISR_PRIORITY);
    NVIC_SetPriority(s_edmaIRQNumbers[dmaConfig->dma_instance][dmaConfig->rx_channel], HAL_UART_ISR_PRIORITY);
#endif
#if (defined(HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION) && (HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION > 0U))
#else /* HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION */
    s_UartDmaState[uartDmaHandle->instance] = uartDmaHandle;

    /* Enable RX interrupt for detecting the IDLE line interrupt. */
    LPUART_EnableInterrupts(s_LpuartAdapterBase[uartHandle->instance], (uint32_t)kLPUART_IdleLineInterruptEnable);
#if defined(FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ) && FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ
    (void)EnableIRQ(s_LpuartRxIRQ[uartHandle->instance]);
#else  /* FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ */
    (void)EnableIRQ(s_LpuartIRQ[uartHandle->instance]);
#endif /* FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ */
#endif /* HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION */

    if (0 == s_dmaHandleList.activeCount)
    {
        s_dmaHandleList.dma_list = uartDmaHandle;
        uartDmaHandle->next      = NULL;
        s_dmaHandleList.activeCount++;

#if (defined(HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION) && (HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION > 0U))
        timer_status_t timerStatus;
        timerStatus = TM_Open((timer_handle_t)s_dmaHandleList.timerManagerHandle);
        assert(kStatus_TimerSuccess == timerStatus);

        timerStatus = TM_InstallCallback((timer_handle_t)s_dmaHandleList.timerManagerHandle, TimeoutTimer_Callbcak,
                                         &s_dmaHandleList);
        assert(kStatus_TimerSuccess == timerStatus);

        (void)TM_Start((timer_handle_t)s_dmaHandleList.timerManagerHandle, (uint8_t)kTimerModeIntervalTimer, 1);

        (void)timerStatus;
#endif /* HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION */
    }
    else
    {
        uartDmaHandle->next      = s_dmaHandleList.dma_list;
        s_dmaHandleList.dma_list = uartDmaHandle;
    }

    return kStatus_HAL_UartDmaSuccess;
}

hal_uart_dma_status_t HAL_UartDMADeinit(hal_uart_handle_t handle)
{
    hal_uart_state_t *uartHandle;
    hal_uart_dma_state_t *uartDmaHandle;
    hal_uart_dma_state_t *prev;
    hal_uart_dma_state_t *curr;

    assert(handle);

    uartHandle    = (hal_uart_state_t *)handle;
    uartDmaHandle = uartHandle->dmaHandle;

    uartHandle->dmaHandle = NULL;

    assert(uartDmaHandle);

    /* Abort rx/tx */

#if (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
    /* Here we should not abort before create transfer handle. */
    if (NULL != uartDmaHandle->edmaHandle.rxEdmaHandle)
    {
        LPUART_TransferAbortReceiveEDMA(s_LpuartAdapterBase[uartDmaHandle->instance], &uartDmaHandle->edmaHandle);
    }
    if (NULL != uartDmaHandle->edmaHandle.txEdmaHandle)
    {
        LPUART_TransferAbortSendEDMA(s_LpuartAdapterBase[uartDmaHandle->instance], &uartDmaHandle->edmaHandle);
    }
#elif (defined(FSL_FEATURE_SOC_DMA_COUNT) && (FSL_FEATURE_SOC_DMA_COUNT > 0U))

#endif /* FSL_FEATURE_SOC_EDMA_COUNT */

    /* Disable rx/tx channels */

    /* Remove handle from list */
    prev = NULL;
    curr = s_dmaHandleList.dma_list;
    while (curr != NULL)
    {
        if (curr == uartDmaHandle)
        {
            /* 1, if it is the first one */
            if (prev == NULL)
            {
                s_dmaHandleList.dma_list = curr->next;
            }
            /* 2, if it is the last one */
            else if (curr->next == NULL)
            {
                prev->next = NULL;
            }
            /* 3, if it is in the middle */
            else
            {
                prev->next = curr->next;
            }
            break;
        }

        prev = curr;
        curr = curr->next;
    }

    /* Reset all handle data. */
    (void)memset(uartDmaHandle, 0, sizeof(hal_uart_dma_state_t));

    s_dmaHandleList.activeCount = (s_dmaHandleList.activeCount > 0) ? (s_dmaHandleList.activeCount - 1) : 0;
#if (defined(HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION) && (HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION > 0U))
    if (0 == s_dmaHandleList.activeCount)
    {
        (void)TM_Close((timer_handle_t)s_dmaHandleList.timerManagerHandle);
    }
#endif /* HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION */

    return kStatus_HAL_UartDmaSuccess;
}

hal_uart_dma_status_t HAL_UartDMATransferInstallCallback(hal_uart_handle_t handle,
                                                         hal_uart_dma_transfer_callback_t callback,
                                                         void *callbackParam)
{
    hal_uart_state_t *uartHandle;
    hal_uart_dma_state_t *uartDmaHandle;

    assert(handle);

    uartHandle    = (hal_uart_state_t *)handle;
    uartDmaHandle = uartHandle->dmaHandle;

    assert(uartDmaHandle);

    uartDmaHandle->dma_callback       = callback;
    uartDmaHandle->dma_callback_param = callbackParam;

#if (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
    LPUART_TransferCreateHandleEDMA(s_LpuartAdapterBase[uartDmaHandle->instance], &uartDmaHandle->edmaHandle,
                                    LPUART_DMACallbacks, uartDmaHandle, &uartDmaHandle->txEdmaHandle,
                                    &uartDmaHandle->rxEdmaHandle);

#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))
#if (defined(FSL_FEATURE_LPUART_IS_LPFLEXCOMM) && (FSL_FEATURE_LPUART_IS_LPFLEXCOMM > 0U))
    LP_FLEXCOMM_SetIRQHandler(uartHandle->instance, HAL_LpUartInterruptHandle_Wapper, handle,
                              LP_FLEXCOMM_PERIPH_LPUART);
#endif /* FSL_FEATURE_LPUART_IS_LPFLEXCOMM */
#endif /* UART_ADAPTER_NON_BLOCKING_MODE */

#if (defined(HAL_UART_DMA_RING_BUFFER_ENABLE) && (HAL_UART_DMA_RING_BUFFER_ENABLE > 0U))
    LPUART_StartRingBufferEDMA(handle);
#endif /* HAL_UART_DMA_RING_BUFFER_ENABLE */
#elif (defined(FSL_FEATURE_SOC_DMA_COUNT) && (FSL_FEATURE_SOC_DMA_COUNT > 0U))
#endif /* FSL_FEATURE_SOC_EDMA_COUNT */

    return kStatus_HAL_UartDmaSuccess;
}

hal_uart_dma_status_t HAL_UartDMATransferReceive(hal_uart_handle_t handle,
                                                 uint8_t *data,
                                                 size_t length,
                                                 bool receiveAll)
{
    hal_uart_state_t *uartHandle;
    hal_uart_dma_state_t *uartDmaHandle;

    assert(handle);
    assert(data);

    uartHandle    = (hal_uart_state_t *)handle;
    uartDmaHandle = uartHandle->dmaHandle;

    assert(uartDmaHandle);

    if (NULL == uartDmaHandle->dma_rx.buffer)
    {
        uartDmaHandle->dma_rx.buffer       = data;
        uartDmaHandle->dma_rx.bufferLength = length;
        uartDmaHandle->dma_rx.bufferSofar  = 0U;
        uartDmaHandle->dma_rx.timeout      = 0U;
        uartDmaHandle->dma_rx.receiveAll   = receiveAll;
    }
    else
    {
        /* Already in reading process. */
        return kStatus_HAL_UartDmaRxBusy;
    }
#if (defined(HAL_UART_DMA_RING_BUFFER_ENABLE) && (HAL_UART_DMA_RING_BUFFER_ENABLE > 0U))

#if (defined(HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION) && (HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION > 0U))
#else /* HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION */
    HAL_UartDMAIdlelineInterruptHandle(uartHandle->instance);
#endif
#else /* HAL_UART_DMA_RING_BUFFER_ENABLE */
#if (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
    lpuart_transfer_t xfer;
    xfer.data = data;
    xfer.dataSize = length;
#endif
#if (defined(FSL_FEATURE_SOC_DMA_COUNT) && (FSL_FEATURE_SOC_DMA_COUNT > 0U))

#elif (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
    (void)LPUART_ReceiveEDMA(s_LpuartAdapterBase[uartDmaHandle->instance], &uartDmaHandle->edmaHandle, &xfer);
#endif /* FSL_FEATURE_SOC_EDMA_COUNT */

#endif
#if (defined(HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION) && (HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION > 0U))
#else
    /* Enable RX interrupt for detecting the IDLE line interrupt. */
    LPUART_EnableInterrupts(s_LpuartAdapterBase[uartHandle->instance], (uint32_t)kLPUART_IdleLineInterruptEnable);
#endif /* HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION */
    return kStatus_HAL_UartDmaSuccess;
}

hal_uart_dma_status_t HAL_UartDMATransferSend(hal_uart_handle_t handle, uint8_t *data, size_t length)
{
    hal_uart_state_t *uartHandle;
    hal_uart_dma_state_t *uartDmaHandle;
#if (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
    lpuart_transfer_t xfer;
#endif
    assert(handle);
    assert(data);

    uartHandle    = (hal_uart_state_t *)handle;
    uartDmaHandle = uartHandle->dmaHandle;

    assert(uartDmaHandle);

    if (NULL == uartDmaHandle->dma_tx.buffer)
    {
        uartDmaHandle->dma_tx.buffer       = data;
        uartDmaHandle->dma_tx.bufferLength = length;
        uartDmaHandle->dma_tx.bufferSofar  = 0U;
    }
    else
    {
        /* Already in writing process. */
        return kStatus_HAL_UartDmaTxBusy;
    }

#if (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
    xfer.data     = data;
    xfer.dataSize = length;
    (void)LPUART_SendEDMA(s_LpuartAdapterBase[uartDmaHandle->instance], &uartDmaHandle->edmaHandle, &xfer);
#elif (defined(FSL_FEATURE_SOC_DMA_COUNT) && (FSL_FEATURE_SOC_DMA_COUNT > 0U))
#endif /* FSL_FEATURE_SOC_EDMA_COUNT */

#if (defined(HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION) && (HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION > 0U))
#else
    /* Enable RX interrupt for detecting the IDLE line interrupt. */
    LPUART_EnableInterrupts(s_LpuartAdapterBase[uartHandle->instance], (uint32_t)kLPUART_IdleLineInterruptEnable);
#endif /* HAL_UART_DMA_USE_SOFTWARE_IDLELINE_DETECTION */

    return kStatus_HAL_UartDmaSuccess;
}

hal_uart_dma_status_t HAL_UartDMAGetReceiveCount(hal_uart_handle_t handle, uint32_t *reCount)
{
    hal_uart_state_t *uartHandle;
    hal_uart_dma_state_t *uartDmaHandle;

    assert(handle);

    uartHandle    = (hal_uart_state_t *)handle;
    uartDmaHandle = uartHandle->dmaHandle;

    assert(uartDmaHandle);
    (void)uartDmaHandle;
#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))
#if (defined(HAL_UART_DMA_RING_BUFFER_ENABLE) && (HAL_UART_DMA_RING_BUFFER_ENABLE > 0U))

#if (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
    *reCount = HAL_UartGetDmaReceivedBytes(uartDmaHandle->instance);
#elif (defined(FSL_FEATURE_SOC_DMA_COUNT) && (FSL_FEATURE_SOC_DMA_COUNT > 0U))
#endif /* FSL_FEATURE_SOC_EDMA_COUNT */
#else  /* HAL_UART_DMA_RING_BUFFER_ENABLE */
#if (defined(FSL_FEATURE_SOC_DMA_COUNT) && (FSL_FEATURE_SOC_DMA_COUNT > 0U))
    if (kStatus_Success != LPUART_TransferGetReceiveCountEDMA(s_LpuartAdapterBase[uartDmaHandle->instance],
                                                              &uartDmaHandle->edmaHandle, reCount))
    {
        return kStatus_HAL_UartDmaError;
    }
#elif (defined(FSL_FEATURE_SOC_DMA_COUNT) && (FSL_FEATURE_SOC_DMA_COUNT > 0U))
#endif /* FSL_FEATURE_SOC_DMA_COUNT */

#endif
#else
    *reCount = 0;

#endif

    return kStatus_HAL_UartDmaSuccess;
}

hal_uart_dma_status_t HAL_UartDMAGetSendCount(hal_uart_handle_t handle, uint32_t *seCount)
{
    hal_uart_state_t *uartHandle;
    hal_uart_dma_state_t *uartDmaHandle;

    assert(handle);

    uartHandle    = (hal_uart_state_t *)handle;
    uartDmaHandle = uartHandle->dmaHandle;

    assert(uartDmaHandle);

#if (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
    if (kStatus_Success != LPUART_TransferGetSendCountEDMA(s_LpuartAdapterBase[uartDmaHandle->instance],
                                                           &uartDmaHandle->edmaHandle, seCount))
    {
        return kStatus_HAL_UartDmaError;
    }
#elif (defined(FSL_FEATURE_SOC_DMA_COUNT) && (FSL_FEATURE_SOC_DMA_COUNT > 0U))

#endif /* FSL_FEATURE_SOC_EDMA_COUNT */

    return kStatus_HAL_UartDmaSuccess;
}

hal_uart_dma_status_t HAL_UartDMAAbortReceive(hal_uart_handle_t handle)
{
    hal_uart_state_t *uartHandle;
    hal_uart_dma_state_t *uartDmaHandle;

    assert(handle);

    uartHandle    = (hal_uart_state_t *)handle;
    uartDmaHandle = uartHandle->dmaHandle;

    assert(uartDmaHandle);
	
#if (defined(HAL_UART_DMA_RING_BUFFER_ENABLE) && (HAL_UART_DMA_RING_BUFFER_ENABLE > 0U))
    /* Make sure to re-initialize the ring bufferIndex */
    ringBufferIndex[uartDmaHandle->instance] = 0U;
#endif

#if (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
    LPUART_TransferAbortReceiveEDMA(s_LpuartAdapterBase[uartDmaHandle->instance], &uartDmaHandle->edmaHandle);
#elif (defined(FSL_FEATURE_SOC_DMA_COUNT) && (FSL_FEATURE_SOC_DMA_COUNT > 0U))
#endif /* FSL_FEATURE_SOC_EDMA_COUNT */

    return kStatus_HAL_UartDmaSuccess;
}

hal_uart_dma_status_t HAL_UartDMAAbortSend(hal_uart_handle_t handle)
{
    hal_uart_state_t *uartHandle;
    hal_uart_dma_state_t *uartDmaHandle;

    assert(handle);

    uartHandle    = (hal_uart_state_t *)handle;
    uartDmaHandle = uartHandle->dmaHandle;

    assert(uartDmaHandle);

#if (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
    LPUART_TransferAbortSendEDMA(s_LpuartAdapterBase[uartDmaHandle->instance], &uartDmaHandle->edmaHandle);
#elif (defined(FSL_FEATURE_SOC_DMA_COUNT) && (FSL_FEATURE_SOC_DMA_COUNT > 0U))
#endif /* FSL_FEATURE_SOC_EDMA_COUNT */

    return kStatus_HAL_UartDmaSuccess;
}
#endif /* HAL_UART_DMA_ENABLE */
