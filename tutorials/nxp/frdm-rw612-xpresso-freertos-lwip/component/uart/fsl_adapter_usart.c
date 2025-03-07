/*
 * Copyright 2018 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_usart.h"
#include "fsl_flexcomm.h"

#include "fsl_adapter_uart.h"

#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
#include "fsl_component_timer_manager.h"
#include "fsl_usart_dma.h"
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
    uint8_t instance; /* USART instance */
    hal_uart_dma_transfer_callback_t dma_callback;
    void *dma_callback_param;
    usart_dma_handle_t dmaHandle;
    dma_handle_t txDmaHandle;
    dma_handle_t rxDmaHandle;
    hal_uart_dma_receive_state_t dma_rx;
    hal_uart_dma_send_state_t dma_tx;
} hal_uart_dma_state_t;

typedef struct _uart_dma_list
{
    TIMER_MANAGER_HANDLE_DEFINE(timerManagerHandle);
    hal_uart_dma_state_t *dma_list;
    volatile int8_t activeCount;
} hal_uart_dma_list_t;

static hal_uart_dma_list_t s_dmaHandleList;
#endif /* HAL_UART_DMA_ENABLE */

#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))
/*! @brief uart RX state structure. */
typedef struct _hal_uart_receive_state
{
    volatile uint8_t *buffer;
    volatile uint32_t bufferLength;
    volatile uint32_t bufferSofar;
} hal_uart_receive_state_t;

/*! @brief uart TX state structure. */
typedef struct _hal_uart_send_state
{
    volatile uint8_t *buffer;
    volatile uint32_t bufferLength;
    volatile uint32_t bufferSofar;
} hal_uart_send_state_t;
#endif
/*! @brief uart state structure. */
typedef struct _hal_uart_state
{
#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))
    hal_uart_transfer_callback_t callback;
    void *callbackParam;
#if (defined(HAL_UART_TRANSFER_MODE) && (HAL_UART_TRANSFER_MODE > 0U))
    usart_handle_t hardwareHandle;
#endif
    hal_uart_receive_state_t rx;
    hal_uart_send_state_t tx;
#endif
    uint8_t instance;
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
    hal_uart_dma_state_t *dmaHandle;
#endif /* HAL_UART_DMA_ENABLE */
#if (defined(HAL_UART_ADAPTER_LOWPOWER) && (HAL_UART_ADAPTER_LOWPOWER > 0U))
    hal_uart_config_t config;
#endif
} hal_uart_state_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static USART_Type *const s_UsartAdapterBase[] = USART_BASE_PTRS;

#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))

#if !(defined(HAL_UART_TRANSFER_MODE) && (HAL_UART_TRANSFER_MODE > 0U))
/* Array of USART IRQ number. */
static const IRQn_Type s_UsartIRQ[] = USART_IRQS;
#endif

#endif

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
        case kStatus_Success:
            uartStatus = kStatus_HAL_UartSuccess;
            break;
        case kStatus_USART_TxBusy:
            uartStatus = kStatus_HAL_UartTxBusy;
            break;
        case kStatus_USART_RxBusy:
            uartStatus = kStatus_HAL_UartRxBusy;
            break;
        case kStatus_USART_TxIdle:
            uartStatus = kStatus_HAL_UartTxIdle;
            break;
        case kStatus_USART_RxIdle:
            uartStatus = kStatus_HAL_UartRxIdle;
            break;
        case kStatus_USART_BaudrateNotSupport:
            uartStatus = kStatus_HAL_UartBaudrateNotSupport;
            break;
        case kStatus_USART_NoiseError:
        case kStatus_USART_FramingError:
        case kStatus_USART_ParityError:
            uartStatus = kStatus_HAL_UartProtocolError;
            break;
        default:
            /* This comments for MISRA C-2012 Rule 16.4 */
            break;
    }
    return uartStatus;
}
#else
static hal_uart_status_t HAL_UartGetStatus(status_t status)
{
    if (kStatus_Success == status)
    {
        return kStatus_HAL_UartSuccess;
    }
    else
    {
        return kStatus_HAL_UartError;
    }
}
#endif

#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))

#if (defined(HAL_UART_TRANSFER_MODE) && (HAL_UART_TRANSFER_MODE > 0U))
static void HAL_UartCallback(USART_Type *base, usart_handle_t *handle, status_t status, void *callbackParam)
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

#else
static void HAL_UartInterruptHandle(USART_Type *base, void *handle)
{
    hal_uart_state_t *uartHandle = (hal_uart_state_t *)handle;
    uint32_t status;
    uint8_t instance;

    if (NULL == uartHandle)
    {
        return;
    }
    instance = uartHandle->instance;

    status = USART_GetStatusFlags(s_UsartAdapterBase[instance]);

    /* Receive data register full */
    if ((0U != (USART_FIFOSTAT_RXNOTEMPTY_MASK & status)) &&
        (0U != (USART_GetEnabledInterrupts(s_UsartAdapterBase[instance]) & USART_FIFOINTENSET_RXLVL_MASK)))
    {
        if (NULL != uartHandle->rx.buffer)
        {
            uartHandle->rx.buffer[uartHandle->rx.bufferSofar++] = USART_ReadByte(s_UsartAdapterBase[instance]);
            if (uartHandle->rx.bufferSofar >= uartHandle->rx.bufferLength)
            {
                USART_DisableInterrupts(s_UsartAdapterBase[instance],
                                        USART_FIFOINTENCLR_RXLVL_MASK | USART_FIFOINTENCLR_RXERR_MASK);
                uartHandle->rx.buffer = NULL;
                if (NULL != uartHandle->callback)
                {
                    uartHandle->callback(uartHandle, kStatus_HAL_UartRxIdle, uartHandle->callbackParam);
                }
            }
        }
    }

    /* Send data register empty and the interrupt is enabled. */
    if ((0U != (USART_FIFOSTAT_TXNOTFULL_MASK & status)) &&
        (0U != (USART_GetEnabledInterrupts(s_UsartAdapterBase[instance]) & USART_FIFOINTENSET_TXLVL_MASK)))
    {
        if (NULL != uartHandle->tx.buffer)
        {
            USART_WriteByte(s_UsartAdapterBase[instance], uartHandle->tx.buffer[uartHandle->tx.bufferSofar++]);
            if (uartHandle->tx.bufferSofar >= uartHandle->tx.bufferLength)
            {
                USART_DisableInterrupts(s_UsartAdapterBase[instance], USART_FIFOINTENCLR_TXLVL_MASK);
                uartHandle->tx.buffer = NULL;
                if (NULL != uartHandle->callback)
                {
                    uartHandle->callback(uartHandle, kStatus_HAL_UartTxIdle, uartHandle->callbackParam);
                }
            }
        }
    }

#if 1
    USART_ClearStatusFlags(s_UsartAdapterBase[instance], status);
#endif
}

static void HAL_UartInterruptHandle_Wrapper(void *base, void *handle)
{
    HAL_UartInterruptHandle((USART_Type *)base, handle);
}
#endif

#endif

static hal_uart_status_t HAL_UartInitCommon(hal_uart_handle_t handle, const hal_uart_config_t *config)
{
    usart_config_t usartConfig;
    status_t status;

    assert(handle);
    assert(config);
    assert(config->instance < (sizeof(s_UsartAdapterBase) / sizeof(USART_Type *)));
    assert(s_UsartAdapterBase[config->instance]);
    assert(HAL_UART_HANDLE_SIZE >= sizeof(hal_uart_state_t));

    USART_GetDefaultConfig(&usartConfig);
    usartConfig.baudRate_Bps = config->baudRate_Bps;

    if ((0U != config->enableRxRTS) || (0U != config->enableTxCTS))
    {
        usartConfig.enableHardwareFlowControl = true;
    }

    if (kHAL_UartParityEven == config->parityMode)
    {
        usartConfig.parityMode = kUSART_ParityEven;
    }
    else if (kHAL_UartParityOdd == config->parityMode)
    {
        usartConfig.parityMode = kUSART_ParityOdd;
    }
    else
    {
        usartConfig.parityMode = kUSART_ParityDisabled;
    }

    if (kHAL_UartTwoStopBit == config->stopBitCount)
    {
        usartConfig.stopBitCount = kUSART_TwoStopBit;
    }
    else
    {
        usartConfig.stopBitCount = kUSART_OneStopBit;
    }
    usartConfig.enableRx    = (bool)config->enableRx;
    usartConfig.enableTx    = (bool)config->enableTx;
    usartConfig.txWatermark = kUSART_TxFifo0;
    usartConfig.rxWatermark = kUSART_RxFifo1;

    status = USART_Init(s_UsartAdapterBase[config->instance], &usartConfig, config->srcClock_Hz);

    if (kStatus_Success != status)
    {
        return HAL_UartGetStatus(status);
    }

    return kStatus_HAL_UartSuccess;
}

hal_uart_status_t HAL_UartInit(hal_uart_handle_t handle, const hal_uart_config_t *uart_config)
{
    hal_uart_state_t *uartHandle;
    hal_uart_status_t status;

    /* Init serial port */
    status = HAL_UartInitCommon(handle, uart_config);
    if (kStatus_HAL_UartSuccess != status)
    {
        return status;
    }

    uartHandle           = (hal_uart_state_t *)handle;
    uartHandle->instance = uart_config->instance;
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
    uartHandle->dmaHandle = NULL;
#endif /* HAL_UART_DMA_ENABLE */
#if (defined(HAL_UART_ADAPTER_LOWPOWER) && (HAL_UART_ADAPTER_LOWPOWER > 0U))
    (void)memcpy(&uartHandle->config, uart_config, sizeof(hal_uart_config_t));
#endif

#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))

#if (defined(HAL_UART_TRANSFER_MODE) && (HAL_UART_TRANSFER_MODE > 0U))
    USART_TransferCreateHandle(s_UsartAdapterBase[uart_config->instance], &uartHandle->hardwareHandle,
                               (usart_transfer_callback_t)HAL_UartCallback, handle);
#else
    /* Enable interrupt in NVIC. */
    FLEXCOMM_SetIRQHandler(s_UsartAdapterBase[uart_config->instance], HAL_UartInterruptHandle_Wrapper, handle);
    NVIC_SetPriority((IRQn_Type)s_UsartIRQ[uart_config->instance], HAL_UART_ISR_PRIORITY);
    (void)EnableIRQ(s_UsartIRQ[uart_config->instance]);
#endif

#endif

    return kStatus_HAL_UartSuccess;
}

hal_uart_status_t HAL_UartDeinit(hal_uart_handle_t handle)
{
    hal_uart_state_t *uartHandle;

    assert(handle);

    uartHandle = (hal_uart_state_t *)handle;

    USART_Deinit(s_UsartAdapterBase[uartHandle->instance]);

    return kStatus_HAL_UartSuccess;
}

hal_uart_status_t HAL_UartReceiveBlocking(hal_uart_handle_t handle, uint8_t *data, size_t length)
{
    hal_uart_state_t *uartHandle;
    status_t status;
    assert(handle);
    assert(data);
    assert(length);

    uartHandle = (hal_uart_state_t *)handle;

#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))
    if (NULL != uartHandle->rx.buffer)
    {
        return kStatus_HAL_UartRxBusy;
    }
#endif

    status = USART_ReadBlocking(s_UsartAdapterBase[uartHandle->instance], data, length);

    return HAL_UartGetStatus(status);
}

hal_uart_status_t HAL_UartSendBlocking(hal_uart_handle_t handle, const uint8_t *data, size_t length)
{
    hal_uart_state_t *uartHandle;
    assert(handle);
    assert(data);
    assert(length);

    uartHandle = (hal_uart_state_t *)handle;

#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))
    if (NULL != uartHandle->tx.buffer)
    {
        return kStatus_HAL_UartTxBusy;
    }
#endif

    (void)USART_WriteBlocking(s_UsartAdapterBase[uartHandle->instance], data, length);

    return kStatus_HAL_UartSuccess;
}

hal_uart_status_t HAL_UartEnterLowpower(hal_uart_handle_t handle)
{
    assert(handle);

    return kStatus_HAL_UartSuccess;
}

hal_uart_status_t HAL_UartExitLowpower(hal_uart_handle_t handle)
{
#if (defined(HAL_UART_ADAPTER_LOWPOWER) && (HAL_UART_ADAPTER_LOWPOWER > 0U))
    hal_uart_state_t *uartHandle;
    assert(handle);

    uartHandle = (hal_uart_state_t *)handle;

    (void)HAL_UartInit(handle, &uartHandle->config);
#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))
    USART_EnableInterrupts(s_UsartAdapterBase[uartHandle->instance], USART_FIFOINTENSET_RXLVL_MASK);
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

    status = USART_TransferReceiveNonBlocking(s_UsartAdapterBase[uartHandle->instance], &uartHandle->hardwareHandle,
                                              (usart_transfer_t *)transfer, NULL);

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

    status = USART_TransferSendNonBlocking(s_UsartAdapterBase[uartHandle->instance], &uartHandle->hardwareHandle,
                                           (usart_transfer_t *)transfer);

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
        USART_TransferGetReceiveCount(s_UsartAdapterBase[uartHandle->instance], &uartHandle->hardwareHandle, count);

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

    status = USART_TransferGetSendCount(s_UsartAdapterBase[uartHandle->instance], &uartHandle->hardwareHandle, count);

    return HAL_UartGetStatus(status);
}

hal_uart_status_t HAL_UartTransferAbortReceive(hal_uart_handle_t handle)
{
    hal_uart_state_t *uartHandle;
    assert(handle);
    assert(0U != HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;

    USART_TransferAbortReceive(s_UsartAdapterBase[uartHandle->instance], &uartHandle->hardwareHandle);

    return kStatus_HAL_UartSuccess;
}

hal_uart_status_t HAL_UartTransferAbortSend(hal_uart_handle_t handle)
{
    hal_uart_state_t *uartHandle;
    assert(handle);
    assert(0U != HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;

    USART_TransferAbortSend(s_UsartAdapterBase[uartHandle->instance], &uartHandle->hardwareHandle);

    return kStatus_HAL_UartSuccess;
}

#else

/* None transactional API with non-blocking mode. */
hal_uart_status_t HAL_UartInstallCallback(hal_uart_handle_t handle,
                                          hal_uart_transfer_callback_t callback,
                                          void *callbackParam)
{
    hal_uart_state_t *uartHandle;

    assert(handle);
    assert(0U == HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;

    uartHandle->callbackParam = callbackParam;
    uartHandle->callback      = callback;

    return kStatus_HAL_UartSuccess;
}

hal_uart_status_t HAL_UartReceiveNonBlocking(hal_uart_handle_t handle, uint8_t *data, size_t length)
{
    hal_uart_state_t *uartHandle;
    assert(handle);
    assert(data);
    assert(length);
    assert(0U == HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;

    if (NULL != uartHandle->rx.buffer)
    {
        return kStatus_HAL_UartRxBusy;
    }

    uartHandle->rx.bufferLength = length;
    uartHandle->rx.bufferSofar  = 0;
    uartHandle->rx.buffer       = data;
    USART_EnableInterrupts(s_UsartAdapterBase[uartHandle->instance], USART_FIFOINTENSET_RXLVL_MASK);
    return kStatus_HAL_UartSuccess;
}

hal_uart_status_t HAL_UartSendNonBlocking(hal_uart_handle_t handle, uint8_t *data, size_t length)
{
    hal_uart_state_t *uartHandle;
    assert(handle);
    assert(data);
    assert(length);
    assert(0U == HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;

    if (NULL != uartHandle->tx.buffer)
    {
        return kStatus_HAL_UartTxBusy;
    }
    uartHandle->tx.bufferLength = length;
    uartHandle->tx.bufferSofar  = 0;
    uartHandle->tx.buffer       = (volatile uint8_t *)data;
    USART_EnableInterrupts(s_UsartAdapterBase[uartHandle->instance], USART_FIFOINTENSET_TXLVL_MASK);
    return kStatus_HAL_UartSuccess;
}

hal_uart_status_t HAL_UartGetReceiveCount(hal_uart_handle_t handle, uint32_t *reCount)
{
    hal_uart_state_t *uartHandle;
    assert(handle);
    assert(reCount);
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
    assert(handle);
    assert(seCount);
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
    assert(handle);
    assert(0U == HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;

    if (NULL != uartHandle->rx.buffer)
    {
        USART_DisableInterrupts(s_UsartAdapterBase[uartHandle->instance],
                                USART_FIFOINTENCLR_RXLVL_MASK | USART_FIFOINTENCLR_RXERR_MASK);
        uartHandle->rx.buffer = NULL;
    }

    return kStatus_HAL_UartSuccess;
}

hal_uart_status_t HAL_UartAbortSend(hal_uart_handle_t handle)
{
    hal_uart_state_t *uartHandle;
    assert(handle);
    assert(0U == HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;

    if (NULL != uartHandle->tx.buffer)
    {
        USART_DisableInterrupts(s_UsartAdapterBase[uartHandle->instance], USART_FIFOINTENCLR_TXLVL_MASK);
        uartHandle->tx.buffer = NULL;
    }

    return kStatus_HAL_UartSuccess;
}

#endif

#if (defined(HAL_UART_TRANSFER_MODE) && (HAL_UART_TRANSFER_MODE > 0U))

void HAL_UartIsrFunction(hal_uart_handle_t handle)
{
    hal_uart_state_t *uartHandle;
    assert(handle);
    assert(0U != HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;

#if 0
    DisableIRQ(s_UsartIRQ[uartHandle->instance]);
#endif
    USART_TransferHandleIRQ(s_UsartAdapterBase[uartHandle->instance], &uartHandle->hardwareHandle);
#if 0
    NVIC_SetPriority((IRQn_Type)s_UsartIRQ[uartHandle->instance], HAL_UART_ISR_PRIORITY);
    EnableIRQ(s_UsartIRQ[uartHandle->instance]);
#endif
}

#else

void HAL_UartIsrFunction(hal_uart_handle_t handle)
{
    hal_uart_state_t *uartHandle;
    assert(handle);
    assert(0U == HAL_UART_TRANSFER_MODE);

    uartHandle = (hal_uart_state_t *)handle;

#if 0
    DisableIRQ(s_UsartIRQ[uartHandle->instance]);
#endif
    HAL_UartInterruptHandle(s_UsartAdapterBase[uartHandle->instance], (void *)uartHandle);
#if 0
    NVIC_SetPriority((IRQn_Type)s_UsartIRQ[uartHandle->instance], HAL_UART_ISR_PRIORITY);
    EnableIRQ(s_UsartIRQ[uartHandle->instance]);
#endif
}

#endif

#endif

#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
static void USART_DMACallbacks(USART_Type *base, usart_dma_handle_t *handle, status_t status, void *userData)
{
    hal_uart_dma_state_t *uartDmaHandle;
    hal_uart_status_t uartStatus = HAL_UartGetStatus(status);
    hal_dma_callback_msg_t msg;
    assert(handle);

    uartDmaHandle = (hal_uart_dma_state_t *)userData;

    if (NULL != uartDmaHandle->dma_callback)
    {
        if (kStatus_HAL_UartTxIdle == uartStatus)
        {
            msg.status                   = kStatus_HAL_UartDmaTxIdle;
            msg.data                     = uartDmaHandle->dma_tx.buffer;
            msg.dataSize                 = uartDmaHandle->dma_tx.bufferLength;
            uartDmaHandle->dma_tx.buffer = NULL;
        }
        else if (kStatus_HAL_UartRxIdle == uartStatus)
        {
            msg.status                   = kStatus_HAL_UartDmaRxIdle;
            msg.data                     = uartDmaHandle->dma_rx.buffer;
            msg.dataSize                 = uartDmaHandle->dma_rx.bufferLength;
            uartDmaHandle->dma_rx.buffer = NULL;
        }

        uartDmaHandle->dma_callback(uartDmaHandle, &msg, uartDmaHandle->dma_callback_param);
    }
}

static void TimeoutTimer_Callbcak(void *param)
{
    hal_uart_dma_list_t *uartDmaHandleList;
    hal_uart_dma_state_t *uartDmaHandle;
    hal_dma_callback_msg_t msg;
    uint32_t newReceived = 0U;

    uartDmaHandleList = &s_dmaHandleList;
    uartDmaHandle     = uartDmaHandleList->dma_list;

    while (NULL != uartDmaHandle)
    {
        if ((NULL != uartDmaHandle->dma_rx.buffer) && (false == uartDmaHandle->dma_rx.receiveAll))
        {
            /* HAL_UartDMAGetReceiveCount(uartDmaHandle, &msg.dataSize); */
            USART_TransferGetReceiveCountDMA(s_UsartAdapterBase[uartDmaHandle->instance], &uartDmaHandle->dmaHandle,
                                             &msg.dataSize);
            newReceived                       = msg.dataSize - uartDmaHandle->dma_rx.bufferSofar;
            uartDmaHandle->dma_rx.bufferSofar = msg.dataSize;

            /* 1, If it is in idle state. */
            if ((0U == newReceived) && (0U < uartDmaHandle->dma_rx.bufferSofar))
            {
                uartDmaHandle->dma_rx.timeout++;
                if (uartDmaHandle->dma_rx.timeout >= HAL_UART_DMA_IDLELINE_TIMEOUT)
                {
                    /* HAL_UartDMAAbortReceive(uartDmaHandle); */
                    USART_TransferAbortReceiveDMA(s_UsartAdapterBase[uartDmaHandle->instance],
                                                  &uartDmaHandle->dmaHandle);
                    msg.data                     = uartDmaHandle->dma_rx.buffer;
                    msg.status                   = kStatus_HAL_UartDmaIdleline;
                    uartDmaHandle->dma_rx.buffer = NULL;
                    uartDmaHandle->dma_callback(uartDmaHandle, &msg, uartDmaHandle->dma_callback_param);
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

hal_uart_dma_status_t HAL_UartDMAInit(hal_uart_handle_t handle,
                                      hal_uart_dma_handle_t dmaHandle,
                                      hal_uart_dma_config_t *dmaConfig)
{
    hal_uart_state_t *uartHandle;
    hal_uart_dma_state_t *uartDmaHandle;

    assert(handle);
    assert(dmaHandle);

    /* DMA init process. */
    uartHandle    = (hal_uart_state_t *)handle;
    uartDmaHandle = (hal_uart_dma_state_t *)dmaHandle;

    uartHandle->dmaHandle = uartDmaHandle;

    uartDmaHandle->instance = dmaConfig->uart_instance;

    DMA_Type *dmaBases[] = DMA_BASE_PTRS;
    DMA_EnableChannel(dmaBases[dmaConfig->dma_instance], dmaConfig->tx_channel);
    DMA_EnableChannel(dmaBases[dmaConfig->dma_instance], dmaConfig->rx_channel);

    DMA_CreateHandle(&uartDmaHandle->txDmaHandle, dmaBases[dmaConfig->dma_instance], dmaConfig->tx_channel);
    DMA_CreateHandle(&uartDmaHandle->rxDmaHandle, dmaBases[dmaConfig->dma_instance], dmaConfig->rx_channel);

    /* Timeout timer init. */
    if (0U == s_dmaHandleList.activeCount)
    {
        s_dmaHandleList.dma_list = uartDmaHandle;
        uartDmaHandle->next      = NULL;
        s_dmaHandleList.activeCount++;

        timer_status_t timerStatus;
        timerStatus = TM_Open((timer_handle_t)s_dmaHandleList.timerManagerHandle);
        assert(kStatus_TimerSuccess == timerStatus);

        timerStatus =
            TM_InstallCallback((timer_handle_t)s_dmaHandleList.timerManagerHandle, TimeoutTimer_Callbcak, NULL);
        assert(kStatus_TimerSuccess == timerStatus);

        (void)TM_Start((timer_handle_t)s_dmaHandleList.timerManagerHandle, (uint8_t)kTimerModeIntervalTimer, 1);

        (void)timerStatus;
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
    /* Here we should not abort before create transfer handle. */
    if (NULL != uartDmaHandle->dmaHandle.txDmaHandle)
    {
        USART_TransferAbortSendDMA(s_UsartAdapterBase[uartDmaHandle->instance], &uartDmaHandle->dmaHandle);
    }
    if (NULL != uartDmaHandle->dmaHandle.rxDmaHandle)
    {
        USART_TransferAbortReceiveDMA(s_UsartAdapterBase[uartDmaHandle->instance], &uartDmaHandle->dmaHandle);
    }

    /* Disable rx/tx channels */
    /* Here we should not disable before create transfer handle. */
    if (NULL != uartDmaHandle->dmaHandle.txDmaHandle)
    {
        DMA_DisableChannel(uartDmaHandle->txDmaHandle.base, uartDmaHandle->txDmaHandle.channel);
    }
    if (NULL != uartDmaHandle->dmaHandle.rxDmaHandle)
    {
        DMA_DisableChannel(uartDmaHandle->rxDmaHandle.base, uartDmaHandle->rxDmaHandle.channel);
    }

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
    if (0 == s_dmaHandleList.activeCount)
    {
        (void)TM_Close((timer_handle_t)s_dmaHandleList.timerManagerHandle);
    }

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

    USART_TransferCreateHandleDMA(s_UsartAdapterBase[uartDmaHandle->instance], &uartDmaHandle->dmaHandle,
                                  USART_DMACallbacks, uartDmaHandle, &uartDmaHandle->txDmaHandle,
                                  &uartDmaHandle->rxDmaHandle);

    return kStatus_HAL_UartDmaSuccess;
}

hal_uart_dma_status_t HAL_UartDMATransferReceive(hal_uart_handle_t handle,
                                                 uint8_t *data,
                                                 size_t length,
                                                 bool receiveAll)
{
    hal_uart_state_t *uartHandle;
    hal_uart_dma_state_t *uartDmaHandle;
    usart_transfer_t xfer;

    assert(handle);
    assert(data);

    uartHandle    = (hal_uart_state_t *)handle;
    uartDmaHandle = uartHandle->dmaHandle;

    assert(uartDmaHandle);

    if (NULL == uartDmaHandle->dma_rx.buffer)
    {
        uartDmaHandle->dma_rx.buffer       = data;
        uartDmaHandle->dma_rx.bufferLength = length;
        uartDmaHandle->dma_rx.timeout      = 0U;
        uartDmaHandle->dma_rx.receiveAll   = receiveAll;
    }
    else
    {
        /* Already in reading process. */
        return kStatus_HAL_UartDmaRxBusy;
    }

    xfer.data     = data;
    xfer.dataSize = length;

    USART_TransferReceiveDMA(s_UsartAdapterBase[uartDmaHandle->instance], &uartDmaHandle->dmaHandle, &xfer);

    return kStatus_HAL_UartDmaSuccess;
}

hal_uart_dma_status_t HAL_UartDMATransferSend(hal_uart_handle_t handle, uint8_t *data, size_t length)
{
    hal_uart_state_t *uartHandle;
    hal_uart_dma_state_t *uartDmaHandle;
    usart_transfer_t xfer;

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
        uartDmaHandle->dma_tx.timeout      = 0U;
    }
    else
    {
        /* Already in writing process. */
        return kStatus_HAL_UartDmaTxBusy;
    }

    xfer.data     = data;
    xfer.dataSize = length;

    USART_TransferSendDMA(s_UsartAdapterBase[uartDmaHandle->instance], &uartDmaHandle->dmaHandle, &xfer);

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

    if (kStatus_Success != USART_TransferGetReceiveCountDMA(s_UsartAdapterBase[uartDmaHandle->instance],
                                                            &uartDmaHandle->dmaHandle, reCount))
    {
        return kStatus_HAL_UartDmaError;
    }

    return kStatus_HAL_UartDmaSuccess;
}

hal_uart_dma_status_t HAL_UartDMAGetSendCount(hal_uart_handle_t handle, uint32_t *seCount)
{
    /* No get send count API */
    return kStatus_HAL_UartDmaError;
}

hal_uart_dma_status_t HAL_UartDMAAbortReceive(hal_uart_handle_t handle)
{
    hal_uart_state_t *uartHandle;
    hal_uart_dma_state_t *uartDmaHandle;

    assert(handle);

    uartHandle    = (hal_uart_state_t *)handle;
    uartDmaHandle = uartHandle->dmaHandle;

    assert(uartDmaHandle);

    USART_TransferAbortReceiveDMA(s_UsartAdapterBase[uartDmaHandle->instance], &uartDmaHandle->dmaHandle);

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

    USART_TransferAbortSendDMA(s_UsartAdapterBase[uartDmaHandle->instance], &uartDmaHandle->dmaHandle);

    return kStatus_HAL_UartDmaSuccess;
}
#endif /* HAL_UART_DMA_ENABLE */
