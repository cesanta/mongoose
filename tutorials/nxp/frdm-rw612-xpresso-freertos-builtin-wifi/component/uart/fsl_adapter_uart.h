/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __HAL_UART_ADAPTER_H__
#define __HAL_UART_ADAPTER_H__

#include "fsl_common.h"
#if defined(SDK_OS_FREE_RTOS)
#include "FreeRTOS.h"
#endif

/*!
 * @addtogroup UART_Adapter
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Enable or disable UART adapter non-blocking mode (1 - enable, 0 - disable) */
#ifdef DEBUG_CONSOLE_TRANSFER_NON_BLOCKING
#define UART_ADAPTER_NON_BLOCKING_MODE (1U)
#else
#ifndef SERIAL_MANAGER_NON_BLOCKING_MODE
#define UART_ADAPTER_NON_BLOCKING_MODE (0U)
#else
#define UART_ADAPTER_NON_BLOCKING_MODE SERIAL_MANAGER_NON_BLOCKING_MODE
#endif
#endif

#if defined(__GIC_PRIO_BITS)
#ifndef HAL_UART_ISR_PRIORITY
#define HAL_UART_ISR_PRIORITY (25U)
#endif
#else
#if defined(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY)
#ifndef HAL_UART_ISR_PRIORITY
#define HAL_UART_ISR_PRIORITY (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY)
#endif
#else
/* The default value 3 is used to support different ARM Core, such as CM0P, CM4, CM7, and CM33, etc.
 * The minimum number of priority bits implemented in the NVIC is 2 on these SOCs. The value of mininum
 * priority is 3 (2^2 - 1). So, the default value is 3.
 */
#ifndef HAL_UART_ISR_PRIORITY
#define HAL_UART_ISR_PRIORITY (3U)
#endif
#endif
#endif

#ifndef HAL_UART_ADAPTER_LOWPOWER
#define HAL_UART_ADAPTER_LOWPOWER (0U)
#endif /* HAL_UART_ADAPTER_LOWPOWER */

/*! @brief Enable or disable uart hardware FIFO mode (1 - enable, 0 - disable) */
#ifndef HAL_UART_ADAPTER_FIFO
#define HAL_UART_ADAPTER_FIFO (1U)
#endif /* HAL_UART_ADAPTER_FIFO */

#if (defined(SERIAL_PORT_TYPE_UART_DMA) && (SERIAL_PORT_TYPE_UART_DMA > 0U))
#ifndef HAL_UART_DMA_ENABLE
#define HAL_UART_DMA_ENABLE (1U)
#endif
#endif

#ifndef HAL_UART_DMA_ENABLE
#define HAL_UART_DMA_ENABLE (0U)
#endif /* HAL_UART_DMA_ENABLE */

/*! @brief Enable or disable uart DMA adapter int mode (1 - enable, 0 - disable) */
#ifndef HAL_UART_DMA_INIT_ENABLE
#define HAL_UART_DMA_INIT_ENABLE (1U)
#endif /* HAL_SPI_MASTER_DMA_INIT_ENABLE */

/*! @brief Definition of uart dma adapter software idleline detection timeout value in ms. */
#ifndef HAL_UART_DMA_IDLELINE_TIMEOUT
#define HAL_UART_DMA_IDLELINE_TIMEOUT (1U)
#endif /* HAL_UART_DMA_IDLELINE_TIMEOUT */

/*! @brief Definition of uart adapter handle size. */
#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))
#define HAL_UART_HANDLE_SIZE       (92U + HAL_UART_ADAPTER_LOWPOWER * 16U + HAL_UART_DMA_ENABLE * 4U)
#define HAL_UART_BLOCK_HANDLE_SIZE (8U + HAL_UART_ADAPTER_LOWPOWER * 16U + HAL_UART_DMA_ENABLE * 4U)
#else
#define HAL_UART_HANDLE_SIZE (8U + HAL_UART_ADAPTER_LOWPOWER * 16U + HAL_UART_DMA_ENABLE * 4U)
#endif

/*! @brief Definition of uart dma adapter handle size. */
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
#if (defined(FSL_FEATURE_SOC_DMA_COUNT) && (FSL_FEATURE_SOC_DMA_COUNT > 0U))
#define HAL_UART_DMA_HANDLE_SIZE (124U + HAL_UART_ADAPTER_LOWPOWER * 36U)
#elif (defined(FSL_FEATURE_SOC_EDMA_COUNT) && (FSL_FEATURE_SOC_EDMA_COUNT > 0U))
#define HAL_UART_DMA_HANDLE_SIZE (140U + HAL_UART_ADAPTER_LOWPOWER * 36U)
#else
#error This SOC does not have DMA or EDMA available!
#endif
#endif /* HAL_UART_DMA_ENABLE */

/*!
 * @brief Defines the uart handle
 *
 * This macro is used to define a 4 byte aligned uart handle.
 * Then use "(hal_uart_handle_t)name" to get the uart handle.
 *
 * The macro should be global and could be optional. You could also define uart handle by yourself.
 *
 * This is an example,
 * @code
 * UART_HANDLE_DEFINE(uartHandle);
 * @endcode
 *
 * @param name The name string of the uart handle.
 */
#define UART_HANDLE_DEFINE(name) uint32_t name[((HAL_UART_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t))]

#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
#define UART_DMA_HANDLE_DEFINE(name) \
    uint32_t name[((HAL_UART_DMA_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t))]
#endif

/*! @brief Whether enable transactional function of the UART. (0 - disable, 1 - enable) */
#ifndef HAL_UART_TRANSFER_MODE
#define HAL_UART_TRANSFER_MODE (0U)
#endif

/*! @brief The handle of uart adapter. */
typedef void *hal_uart_handle_t;

/*! @brief The handle of uart dma adapter. */
typedef void *hal_uart_dma_handle_t;

/*! @brief UART status */
typedef enum _hal_uart_status
{
    kStatus_HAL_UartSuccess = kStatus_Success,                       /*!< Successfully */
    kStatus_HAL_UartTxBusy  = MAKE_STATUS(kStatusGroup_HAL_UART, 1), /*!< TX busy */
    kStatus_HAL_UartRxBusy  = MAKE_STATUS(kStatusGroup_HAL_UART, 2), /*!< RX busy */
    kStatus_HAL_UartTxIdle  = MAKE_STATUS(kStatusGroup_HAL_UART, 3), /*!< HAL UART transmitter is idle. */
    kStatus_HAL_UartRxIdle  = MAKE_STATUS(kStatusGroup_HAL_UART, 4), /*!< HAL UART receiver is idle */
    kStatus_HAL_UartBaudrateNotSupport =
        MAKE_STATUS(kStatusGroup_HAL_UART, 5), /*!< Baudrate is not support in current clock source */
    kStatus_HAL_UartProtocolError = MAKE_STATUS(
        kStatusGroup_HAL_UART,
        6),                                                        /*!< Error occurs for Noise, Framing, Parity, etc.
                                                                        For transactional transfer, The up layer needs to abort the transfer and then starts again */
    kStatus_HAL_UartError = MAKE_STATUS(kStatusGroup_HAL_UART, 7), /*!< Error occurs on HAL UART */
} hal_uart_status_t;

/*! @brief UART parity mode. */
typedef enum _hal_uart_parity_mode
{
    kHAL_UartParityDisabled = 0x0U, /*!< Parity disabled */
    kHAL_UartParityEven     = 0x2U, /*!< Parity even enabled */
    kHAL_UartParityOdd      = 0x3U, /*!< Parity odd enabled */
} hal_uart_parity_mode_t;

/*! @brief UART stop bit count. */
typedef enum _hal_uart_stop_bit_count
{
    kHAL_UartOneStopBit = 0U, /*!< One stop bit */
    kHAL_UartTwoStopBit = 1U, /*!< Two stop bits */
} hal_uart_stop_bit_count_t;

/*! @brief UART configuration structure. */
typedef struct _hal_uart_config
{
    uint32_t srcClock_Hz;                   /*!< Source clock */
    uint32_t baudRate_Bps;                  /*!< Baud rate  */
    hal_uart_parity_mode_t parityMode;      /*!< Parity mode, disabled (default), even, odd */
    hal_uart_stop_bit_count_t stopBitCount; /*!< Number of stop bits, 1 stop bit (default) or 2 stop bits  */
    uint8_t enableRx;                       /*!< Enable RX */
    uint8_t enableTx;                       /*!< Enable TX */
    uint8_t enableRxRTS;                    /*!< Enable RX RTS */
    uint8_t enableTxCTS;                    /*!< Enable TX CTS */
    uint8_t instance; /*!< Instance (0 - UART0, 1 - UART1, ...), detail information please refer to the
                           SOC corresponding RM.
                           Invalid instance value will cause initialization failure. */
#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
    uint8_t txFifoWatermark;
    uint8_t rxFifoWatermark;
#endif
} hal_uart_config_t;

#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
/*! @brief UART DMA status */
typedef enum _hal_uart_dma_status
{
    kStatus_HAL_UartDmaSuccess  = 0U,
    kStatus_HAL_UartDmaRxIdle   = (1U << 1U),
    kStatus_HAL_UartDmaRxBusy   = (1U << 2U),
    kStatus_HAL_UartDmaTxIdle   = (1U << 3U),
    kStatus_HAL_UartDmaTxBusy   = (1U << 4U),
    kStatus_HAL_UartDmaIdleline = (1U << 5U),
    kStatus_HAL_UartDmaError    = (1U << 6U),
} hal_uart_dma_status_t;

typedef struct _dma_mux_configure_t
{
    union
    {
        struct
        {
            uint8_t dma_mux_instance;
            uint32_t rx_request;
            uint32_t tx_request;
        } dma_dmamux_configure;
    };
} dma_mux_configure_t;
typedef struct _dma_channel_mux_configure_t
{
    union
    {
        struct
        {
            uint32_t dma_rx_channel_mux;
            uint32_t dma_tx_channel_mux;
        } dma_dmamux_configure;
    };
} dma_channel_mux_configure_t;

typedef struct _hal_uart_dma_config_t
{
    uint8_t uart_instance;
    uint8_t dma_instance;
    uint8_t rx_channel;
    uint8_t tx_channel;
    void *dma_mux_configure;
    void *dma_channel_mux_configure;
} hal_uart_dma_config_t;
#endif /* HAL_UART_DMA_ENABLE */

/*! @brief UART transfer callback function. */
typedef void (*hal_uart_transfer_callback_t)(hal_uart_handle_t handle, hal_uart_status_t status, void *callbackParam);

#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
typedef struct _dma_callback_msg
{
    hal_uart_dma_status_t status;
    uint8_t *data;
    uint32_t dataSize;
} hal_dma_callback_msg_t;

/*! @brief UART transfer callback function. */
typedef void (*hal_uart_dma_transfer_callback_t)(hal_uart_dma_handle_t handle,
                                                 hal_dma_callback_msg_t *msg,
                                                 void *callbackParam);
#endif /* HAL_UART_DMA_ENABLE */

/*! @brief UART transfer structure. */
typedef struct _hal_uart_transfer
{
    uint8_t *data;   /*!< The buffer of data to be transfer.*/
    size_t dataSize; /*!< The byte count to be transfer. */
} hal_uart_transfer_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */

/*!
 * @name Initialization and deinitialization
 * @{
 */

/*!
 * @brief Initializes a UART instance with the UART handle and the user configuration structure.
 *
 * This function configures the UART module with user-defined settings. The user can configure the configuration
 * structure. The parameter handle is a pointer to point to a memory space of size #HAL_UART_HANDLE_SIZE allocated by
 * the caller. Example below shows how to use this API to configure the UART.
 *  @code
 *   UART_HANDLE_DEFINE(g_UartHandle);
 *   hal_uart_config_t config;
 *   config.srcClock_Hz = 48000000;
 *   config.baudRate_Bps = 115200U;
 *   config.parityMode = kHAL_UartParityDisabled;
 *   config.stopBitCount = kHAL_UartOneStopBit;
 *   config.enableRx = 1;
 *   config.enableTx = 1;
 *   config.enableRxRTS = 0;
 *   config.enableTxCTS = 0;
 *   config.instance = 0;
 *   HAL_UartInit((hal_uart_handle_t)g_UartHandle, &config);
 *  @endcode
 *
 * @param handle Pointer to point to a memory space of size #HAL_UART_HANDLE_SIZE allocated by the caller.
 * The handle should be 4 byte aligned, because unaligned access doesn't be supported on some devices.
 * You can define the handle in the following two ways:
 * #UART_HANDLE_DEFINE(handle);
 * or
 * uint32_t handle[((HAL_UART_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t))];
 * @param uart_config Pointer to user-defined configuration structure.
 * @retval kStatus_HAL_UartBaudrateNotSupport Baudrate is not support in current clock source.
 * @retval kStatus_HAL_UartSuccess UART initialization succeed
 */
hal_uart_status_t HAL_UartInit(hal_uart_handle_t handle, const hal_uart_config_t *uart_config);

/*!
 * @brief Deinitializes a UART instance.
 *
 * This function waits for TX complete, disables TX and RX, and disables the UART clock.
 *
 * @param handle UART handle pointer.
 * @retval kStatus_HAL_UartSuccess UART de-initialization succeed
 */
hal_uart_status_t HAL_UartDeinit(hal_uart_handle_t handle);

/*! @}*/

/*!
 * @name Blocking bus Operations
 * @{
 */

/*!
 * @brief Reads RX data register using a blocking method.
 *
 * This function polls the RX register, waits for the RX register to be full or for RX FIFO to
 * have data, and reads data from the RX register.
 *
 * @note The function #HAL_UartReceiveBlocking and the function HAL_UartTransferReceiveNonBlocking
 * cannot be used at the same time.
 * And, the function HAL_UartTransferAbortReceive cannot be used to abort the transmission of this function.
 *
 * @param handle UART handle pointer.
 * @param data Start address of the buffer to store the received data.
 * @param length Size of the buffer.
 * @retval kStatus_HAL_UartError An error occurred while receiving data.
 * @retval kStatus_HAL_UartParityError A parity error occurred while receiving data.
 * @retval kStatus_HAL_UartSuccess Successfully received all data.
 */
hal_uart_status_t HAL_UartReceiveBlocking(hal_uart_handle_t handle, uint8_t *data, size_t length);

/*!
 * @brief Writes to the TX register using a blocking method.
 *
 * This function polls the TX register, waits for the TX register to be empty or for the TX FIFO
 * to have room and writes data to the TX buffer.
 *
 * @note The function #HAL_UartSendBlocking and the function HAL_UartTransferSendNonBlocking
 * cannot be used at the same time.
 * And, the function HAL_UartTransferAbortSend cannot be used to abort the transmission of this function.
 *
 * @param handle UART handle pointer.
 * @param data Start address of the data to write.
 * @param length Size of the data to write.
 * @retval kStatus_HAL_UartSuccess Successfully sent all data.
 */
hal_uart_status_t HAL_UartSendBlocking(hal_uart_handle_t handle, const uint8_t *data, size_t length);

/*! @}*/

#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))
#if (defined(HAL_UART_TRANSFER_MODE) && (HAL_UART_TRANSFER_MODE > 0U))

/*!
 * @name Transactional
 * @note The transactional API and the functional API cannot be used at the same time. The macro
 * #HAL_UART_TRANSFER_MODE is used to set which one will be used. If #HAL_UART_TRANSFER_MODE is zero, the
 * functional API with non-blocking mode will be used. Otherwise, transactional API will be used.
 * @{
 */

/*!
 * @brief Installs a callback and callback parameter.
 *
 * This function is used to install the callback and callback parameter for UART module.
 * When any status of the UART changed, the driver will notify the upper layer by the installed callback
 * function. And the status is also passed as status parameter when the callback is called.
 *
 * @param handle UART handle pointer.
 * @param callback The callback function.
 * @param callbackParam The parameter of the callback function.
 * @retval kStatus_HAL_UartSuccess Successfully install the callback.
 */
hal_uart_status_t HAL_UartTransferInstallCallback(hal_uart_handle_t handle,
                                                  hal_uart_transfer_callback_t callback,
                                                  void *callbackParam);

/*!
 * @brief Receives a buffer of data using an interrupt method.
 *
 * This function receives data using an interrupt method. This is a non-blocking function, which
 * returns directly without waiting for all data to be received.
 * The receive request is saved by the UART driver.
 * When the new data arrives, the receive request is serviced first.
 * When all data is received, the UART driver notifies the upper layer
 * through a callback function and passes the status parameter @ref kStatus_HAL_UartRxIdle.
 *
 * @note The function #HAL_UartReceiveBlocking and the function #HAL_UartTransferReceiveNonBlocking
 * cannot be used at the same time.
 *
 * @param handle UART handle pointer.
 * @param transfer UART transfer structure, see #hal_uart_transfer_t.
 * @retval kStatus_HAL_UartSuccess Successfully queue the transfer into transmit queue.
 * @retval kStatus_HAL_UartRxBusy Previous receive request is not finished.
 * @retval kStatus_HAL_UartError An error occurred.
 */
hal_uart_status_t HAL_UartTransferReceiveNonBlocking(hal_uart_handle_t handle, hal_uart_transfer_t *transfer);

/*!
 * @brief Transmits a buffer of data using the interrupt method.
 *
 * This function sends data using an interrupt method. This is a non-blocking function, which
 * returns directly without waiting for all data to be written to the TX register. When
 * all data is written to the TX register in the ISR, the UART driver calls the callback
 * function and passes the @ref kStatus_HAL_UartTxIdle as status parameter.
 *
 * @note The function #HAL_UartSendBlocking and the function #HAL_UartTransferSendNonBlocking
 * cannot be used at the same time.
 *
 * @param handle UART handle pointer.
 * @param transfer UART transfer structure. See #hal_uart_transfer_t.
 * @retval kStatus_HAL_UartSuccess Successfully start the data transmission.
 * @retval kStatus_HAL_UartTxBusy Previous transmission still not finished; data not all written to TX register yet.
 * @retval kStatus_HAL_UartError An error occurred.
 */
hal_uart_status_t HAL_UartTransferSendNonBlocking(hal_uart_handle_t handle, hal_uart_transfer_t *transfer);

/*!
 * @brief Gets the number of bytes that have been received.
 *
 * This function gets the number of bytes that have been received.
 *
 * @param handle UART handle pointer.
 * @param count Receive bytes count.
 * @retval kStatus_HAL_UartError An error occurred.
 * @retval kStatus_Success Get successfully through the parameter \p count.
 */
hal_uart_status_t HAL_UartTransferGetReceiveCount(hal_uart_handle_t handle, uint32_t *count);

/*!
 * @brief Gets the number of bytes written to the UART TX register.
 *
 * This function gets the number of bytes written to the UART TX
 * register by using the interrupt method.
 *
 * @param handle UART handle pointer.
 * @param count Send bytes count.
 * @retval kStatus_HAL_UartError An error occurred.
 * @retval kStatus_Success Get successfully through the parameter \p count.
 */
hal_uart_status_t HAL_UartTransferGetSendCount(hal_uart_handle_t handle, uint32_t *count);

/*!
 * @brief Aborts the interrupt-driven data receiving.
 *
 * This function aborts the interrupt-driven data receiving. The user can get the remainBytes to know
 * how many bytes are not received yet.
 *
 * @note The function #HAL_UartTransferAbortReceive cannot be used to abort the transmission of
 * the function #HAL_UartReceiveBlocking.
 *
 * @param handle UART handle pointer.
 * @retval kStatus_Success Get successfully abort the receiving.
 */
hal_uart_status_t HAL_UartTransferAbortReceive(hal_uart_handle_t handle);

/*!
 * @brief Aborts the interrupt-driven data sending.
 *
 * This function aborts the interrupt-driven data sending. The user can get the remainBytes to find out
 * how many bytes are not sent out.
 *
 * @note The function #HAL_UartTransferAbortSend cannot be used to abort the transmission of
 * the function #HAL_UartSendBlocking.
 *
 * @param handle UART handle pointer.
 * @retval kStatus_Success Get successfully abort the sending.
 */
hal_uart_status_t HAL_UartTransferAbortSend(hal_uart_handle_t handle);

/*! @}*/

#else

/*!
 * @name Functional API with non-blocking mode.
 * @note The functional API and the transactional API cannot be used at the same time. The macro
 * #HAL_UART_TRANSFER_MODE is used to set which one will be used. If #HAL_UART_TRANSFER_MODE is zero, the
 * functional API with non-blocking mode will be used. Otherwise, transactional API will be used.
 * @{
 */

/*!
 * @brief Installs a callback and callback parameter.
 *
 * This function is used to install the callback and callback parameter for UART module.
 * When non-blocking sending or receiving finished, the adapter will notify the upper layer by the installed callback
 * function. And the status is also passed as status parameter when the callback is called.
 *
 * @param handle UART handle pointer.
 * @param callback The callback function.
 * @param callbackParam The parameter of the callback function.
 * @retval kStatus_HAL_UartSuccess Successfully install the callback.
 */
hal_uart_status_t HAL_UartInstallCallback(hal_uart_handle_t handle,
                                          hal_uart_transfer_callback_t callback,
                                          void *callbackParam);

/*!
 * @brief Receives a buffer of data using an interrupt method.
 *
 * This function receives data using an interrupt method. This is a non-blocking function, which
 * returns directly without waiting for all data to be received.
 * The receive request is saved by the UART adapter.
 * When the new data arrives, the receive request is serviced first.
 * When all data is received, the UART adapter notifies the upper layer
 * through a callback function and passes the status parameter @ref kStatus_HAL_UartRxIdle.
 *
 * @note The function #HAL_UartReceiveBlocking and the function #HAL_UartReceiveNonBlocking
 * cannot be used at the same time.
 *
 * @param handle UART handle pointer.
 * @param data Start address of the data to write.
 * @param length Size of the data to write.
 * @retval kStatus_HAL_UartSuccess Successfully queue the transfer into transmit queue.
 * @retval kStatus_HAL_UartRxBusy Previous receive request is not finished.
 * @retval kStatus_HAL_UartError An error occurred.
 */
hal_uart_status_t HAL_UartReceiveNonBlocking(hal_uart_handle_t handle, uint8_t *data, size_t length);

/*!
 * @brief Transmits a buffer of data using the interrupt method.
 *
 * This function sends data using an interrupt method. This is a non-blocking function, which
 * returns directly without waiting for all data to be written to the TX register. When
 * all data is written to the TX register in the ISR, the UART driver calls the callback
 * function and passes the @ref kStatus_HAL_UartTxIdle as status parameter.
 *
 * @note The function #HAL_UartSendBlocking and the function #HAL_UartSendNonBlocking
 * cannot be used at the same time.
 *
 * @param handle UART handle pointer.
 * @param data Start address of the data to write.
 * @param length Size of the data to write.
 * @retval kStatus_HAL_UartSuccess Successfully start the data transmission.
 * @retval kStatus_HAL_UartTxBusy Previous transmission still not finished; data not all written to TX register yet.
 * @retval kStatus_HAL_UartError An error occurred.
 */
hal_uart_status_t HAL_UartSendNonBlocking(hal_uart_handle_t handle, uint8_t *data, size_t length);

/*!
 * @brief Gets the number of bytes that have been received.
 *
 * This function gets the number of bytes that have been received.
 *
 * @param handle UART handle pointer.
 * @param reCount Receive bytes count.
 * @retval kStatus_HAL_UartError An error occurred.
 * @retval kStatus_Success Get successfully through the parameter \p count.
 */
hal_uart_status_t HAL_UartGetReceiveCount(hal_uart_handle_t handle, uint32_t *reCount);

/*!
 * @brief Gets the number of bytes written to the UART TX register.
 *
 * This function gets the number of bytes written to the UART TX
 * register by using the interrupt method.
 *
 * @param handle UART handle pointer.
 * @param seCount Send bytes count.
 * @retval kStatus_HAL_UartError An error occurred.
 * @retval kStatus_Success Get successfully through the parameter \p count.
 */
hal_uart_status_t HAL_UartGetSendCount(hal_uart_handle_t handle, uint32_t *seCount);

/*!
 * @brief Aborts the interrupt-driven data receiving.
 *
 * This function aborts the interrupt-driven data receiving. The user can get the remainBytes to know
 * how many bytes are not received yet.
 *
 * @note The function #HAL_UartAbortReceive cannot be used to abort the transmission of
 * the function #HAL_UartReceiveBlocking.
 *
 * @param handle UART handle pointer.
 * @retval kStatus_Success Get successfully abort the receiving.
 */
hal_uart_status_t HAL_UartAbortReceive(hal_uart_handle_t handle);

/*!
 * @brief Aborts the interrupt-driven data sending.
 *
 * This function aborts the interrupt-driven data sending. The user can get the remainBytes to find out
 * how many bytes are not sent out.
 *
 * @note The function #HAL_UartAbortSend cannot be used to abort the transmission of
 * the function #HAL_UartSendBlocking.
 *
 * @param handle UART handle pointer.
 * @retval kStatus_Success Get successfully abort the sending.
 */
hal_uart_status_t HAL_UartAbortSend(hal_uart_handle_t handle);

/*! @}*/

#endif
#endif

#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))

/*!
 * @brief Initializes a UART dma instance with the UART dma handle and the user configuration structure.
 *
 * This function configures the UART dma module with user-defined settings. The user can configure the configuration
 * structure. The parameter handle is a pointer to point to a memory space of size #HAL_UART_DMA_HANDLE_SIZE allocated
 * by the caller. Example below shows how to use this API to configure the UART.
 *  @code
 *
 *  Init TimerManager, only used in UART without Idleline interrupt
 *  timer_config_t timerConfig;
 *  timerConfig.srcClock_Hz    = 16000000;
 *  timerConfig.instance       = 0;
 *  TM_Init(&timerConfig);
 *
 *  Init the DMA module
 *  DMA_Init(DMA0);
 *
 *  Define a uart dma handle
 *  UART_HANDLE_DEFINE(g_uartHandle);
 *  UART_DMA_HANDLE_DEFINE(g_UartDmaHandle);
 *
 *  Configure uart settings
 *  hal_uart_config_t uartConfig;
 *  uartConfig.srcClock_Hz  = 48000000;
 *  uartConfig.baudRate_Bps = 115200;
 *  uartConfig.parityMode   = kHAL_UartParityDisabled;
 *  uartConfig.stopBitCount = kHAL_UartOneStopBit;
 *  uartConfig.enableRx     = 1;
 *  uartConfig.enableTx     = 1;
 *  uartConfig.enableRxRTS  = 0;
 *  uartConfig.enableTxCTS  = 0;
 *  uartConfig.instance     = 0;
 *
 *  Init uart
 *  HAL_UartInit((hal_uart_handle_t *)g_uartHandle, &uartConfig);
 *
 *  Configure uart dma settings
 *  hal_uart_dma_config_t dmaConfig;
 *  dmaConfig.uart_instance = 0;
 *  dmaConfig.dma_instance  = 0;
 *  dmaConfig.rx_channel    = 0;
 *  dmaConfig.tx_channel    = 1;
 *
 *  Init uart dma
 *  HAL_UartDMAInit((hal_uart_handle_t *)g_uartHandle, (hal_uart_dma_handle_t *)g_uartDmaHandle, &dmaConfig);
 *  @endcode
 *
 * @param handle UART handle pointer.
 * @param dmaHandle Pointer to point to a memory space of size #HAL_UART_DMA_HANDLE_SIZE allocated by the caller.
 * The handle should be 4 byte aligned, because unaligned access doesn't be supported on some devices.
 * You can define the handle in the following two ways:
 * #UART_DMA_HANDLE_DEFINE(handle);
 * or
 * uint32_t handle[((HAL_UART_DMA_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t))];
 * @param dmaConfig Pointer to user-defined configuration structure.
 * @retval kStatus_HAL_UartDmaError UART dma initialization failed.
 * @retval kStatus_HAL_UartDmaSuccess UART dma initialization succeed.
 */
hal_uart_dma_status_t HAL_UartDMAInit(hal_uart_handle_t handle,
                                      hal_uart_dma_handle_t dmaHandle,
                                      hal_uart_dma_config_t *dmaConfig);

/*!
 * @brief Deinitializes a UART DMA instance.
 *
 * This function will abort uart dma receive/send transfer and deinitialize UART.
 *
 * @param handle UART handle pointer.
 * @retval kStatus_HAL_UartDmaSuccess UART DMA de-initialization succeed
 */
hal_uart_dma_status_t HAL_UartDMADeinit(hal_uart_handle_t handle);

/*!
 * @brief Installs a callback and callback parameter.
 *
 * This function is used to install the callback and callback parameter for UART DMA module.
 * When any status of the UART DMA changed, the driver will notify the upper layer by the installed callback
 * function. And the status is also passed as status parameter when the callback is called.
 *
 * @param handle UART handle pointer.
 * @param callback The callback function.
 * @param callbackParam The parameter of the callback function.
 * @retval kStatus_HAL_UartDmaSuccess Successfully install the callback.
 */
hal_uart_dma_status_t HAL_UartDMATransferInstallCallback(hal_uart_handle_t handle,
                                                         hal_uart_dma_transfer_callback_t callback,
                                                         void *callbackParam);

/*!
 * @brief Receives a buffer of data using an dma method.
 *
 * This function receives data using an dma method. This is a non-blocking function, which
 * returns directly without waiting for all data to be received.
 * The receive request is saved by the UART DMA driver.
 * When all data is received, the UART DMA adapter notifies the upper layer
 * through a callback function and passes the status parameter @ref kStatus_HAL_UartDmaRxIdle.
 *
 * When an idleline is detected, the UART DMA adapter notifies the upper layer through a callback function,
 * and passes the status parameter @ref kStatus_HAL_UartDmaIdleline. For the UARTs without hardware idleline
 * interrupt(like usart), it will use a software idleline detection method with the help of TimerManager.
 *
 * When the soc support cache, uplayer should do cache maintain operations for transfer buffer before call this API.
 *
 * @param handle UART handle pointer.
 * @param data data Start address of the buffer to store the received data.
 * @param length Size of the buffer.
 * @param receiveAll Idleline interrupt will not end transfer process if set true.
 * @retval kStatus_HAL_UartDmaSuccess Successfully start the data receive.
 * @retval kStatus_HAL_UartDmaRxBusy Previous receive request is not finished.
 */
hal_uart_dma_status_t HAL_UartDMATransferReceive(hal_uart_handle_t handle,
                                                 uint8_t *data,
                                                 size_t length,
                                                 bool receiveAll);

/*!
 * @brief Transmits a buffer of data using an dma method.
 *
 * This function sends data using an dma method. This is a non-blocking function, which
 * returns directly without waiting for all data to be written to the TX register. When
 * all data is written to the TX register by DMA, the UART DMA driver calls the callback
 * function and passes the @ref kStatus_HAL_UartDmaTxIdle as status parameter.
 *
 * When the soc support cache, uplayer should do cache maintain operations for transfer buffer before call this API.
 *
 * @param handle UART handle pointer.
 * @param data data Start address of the data to write.
 * @param length Size of the data to write.
 * @retval kStatus_HAL_UartDmaSuccess Successfully start the data transmission.
 * @retval kStatus_HAL_UartDmaTxBusy Previous send request is not finished.
 */
hal_uart_dma_status_t HAL_UartDMATransferSend(hal_uart_handle_t handle, uint8_t *data, size_t length);

/*!
 * @brief Gets the number of bytes that have been received.
 *
 * This function gets the number of bytes that have been received.
 *
 * @param handle UART handle pointer.
 * @param reCount Receive bytes count.
 * @retval kStatus_HAL_UartDmaError An error occurred.
 * @retval kStatus_HAL_UartDmaSuccess Get successfully through the parameter \p reCount.
 */
hal_uart_dma_status_t HAL_UartDMAGetReceiveCount(hal_uart_handle_t handle, uint32_t *reCount);

/*!
 * @brief Gets the number of bytes written to the UART TX register.
 *
 * This function gets the number of bytes written to the UART TX
 * register by using the DMA method.
 *
 * @param handle UART handle pointer.
 * @param seCount Send bytes count.
 * @retval kStatus_HAL_UartDmaError An error occurred.
 * @retval kStatus_HAL_UartDmaSuccess Get successfully through the parameter \p seCount.
 */
hal_uart_dma_status_t HAL_UartDMAGetSendCount(hal_uart_handle_t handle, uint32_t *seCount);

/*!
 * @brief Aborts the DMA-driven data receiving.
 *
 * This function aborts the DMA-driven data receiving.
 *
 * @param handle UART handle pointer.
 * @retval kStatus_HAL_UartDmaSuccess Get successfully abort the receiving.
 */
hal_uart_dma_status_t HAL_UartDMAAbortReceive(hal_uart_handle_t handle);

/*!
 * @brief Aborts the DMA-driven data sending.
 *
 * This function aborts the DMA-driven data sending.
 *
 * @param handle UART handle pointer.
 * @retval kStatus_Success Get successfully abort the sending.
 */
hal_uart_dma_status_t HAL_UartDMAAbortSend(hal_uart_handle_t handle);
#endif /* HAL_UART_DMA_ENABLE */

/*!
 * @brief Prepares to enter low power consumption.
 *
 * This function is used to prepare to enter low power consumption.
 *
 * @param handle UART handle pointer.
 * @retval kStatus_HAL_UartSuccess Successful operation.
 * @retval kStatus_HAL_UartError An error occurred.
 */
hal_uart_status_t HAL_UartEnterLowpower(hal_uart_handle_t handle);

/*!
 * @brief Restores from low power consumption.
 *
 * This function is used to restore from low power consumption.
 *
 * @param handle UART handle pointer.
 * @retval kStatus_HAL_UartSuccess Successful operation.
 * @retval kStatus_HAL_UartError An error occurred.
 */
hal_uart_status_t HAL_UartExitLowpower(hal_uart_handle_t handle);

#if (defined(UART_ADAPTER_NON_BLOCKING_MODE) && (UART_ADAPTER_NON_BLOCKING_MODE > 0U))
/*!
 * @brief UART IRQ handle function.
 *
 * This function handles the UART transmit and receive IRQ request.
 *
 * @param handle UART handle pointer.
 */
void HAL_UartIsrFunction(hal_uart_handle_t handle);
#endif

#if defined(__cplusplus)
}
#endif
/*! @}*/
#endif /* __HAL_UART_ADAPTER_H__ */
