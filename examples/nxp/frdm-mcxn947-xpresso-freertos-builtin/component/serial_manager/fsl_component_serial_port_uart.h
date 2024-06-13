/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SERIAL_PORT_UART_H__
#define __SERIAL_PORT_UART_H__

#include "fsl_adapter_uart.h"

/*!
 * @addtogroup serial_port_uart
 * @ingroup serialmanager
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief serial port uart handle size*/

#ifndef SERIAL_PORT_UART_DMA_RECEIVE_DATA_LENGTH
#define SERIAL_PORT_UART_DMA_RECEIVE_DATA_LENGTH (64U)
#endif

#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))

#define SERIAL_PORT_UART_HANDLE_SIZE       (76U + HAL_UART_HANDLE_SIZE)
#define SERIAL_PORT_UART_BLOCK_HANDLE_SIZE (HAL_UART_BLOCK_HANDLE_SIZE)
#else
#define SERIAL_PORT_UART_HANDLE_SIZE (HAL_UART_HANDLE_SIZE)
#endif

#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
#define SERIAL_PORT_UART_DMA_HANDLE_SIZE (76U + HAL_UART_DMA_HANDLE_SIZE + 132U)
#endif

#ifndef SERIAL_USE_CONFIGURE_STRUCTURE
#define SERIAL_USE_CONFIGURE_STRUCTURE (0U) /*!< Enable or disable the confgure structure pointer */
#endif

/*! @brief serial port uart parity mode*/
typedef enum _serial_port_uart_parity_mode
{
    kSerialManager_UartParityDisabled = 0x0U, /*!< Parity disabled */
    kSerialManager_UartParityEven     = 0x2U, /*!< Parity even enabled */
    kSerialManager_UartParityOdd      = 0x3U, /*!< Parity odd enabled */
} serial_port_uart_parity_mode_t;

/*! @brief serial port uart stop bit count*/
typedef enum _serial_port_uart_stop_bit_count
{
    kSerialManager_UartOneStopBit = 0U, /*!< One stop bit */
    kSerialManager_UartTwoStopBit = 1U, /*!< Two stop bits */
} serial_port_uart_stop_bit_count_t;

typedef struct _serial_port_uart_config
{
    uint32_t clockRate;                             /*!< clock rate  */
    uint32_t baudRate;                              /*!< baud rate  */
    serial_port_uart_parity_mode_t parityMode;      /*!< Parity mode, disabled (default), even, odd */
    serial_port_uart_stop_bit_count_t stopBitCount; /*!< Number of stop bits, 1 stop bit (default) or 2 stop bits  */

    uint8_t enableRx;                               /*!< Enable RX */
    uint8_t enableTx;                               /*!< Enable TX */
    uint8_t enableRxRTS;                            /*!< Enable RX RTS */
    uint8_t enableTxCTS;                            /*!< Enable TX CTS */
    uint8_t instance;                               /*!< Instance (0 - UART0, 1 - UART1, ...), detail information
                                                         please refer to the SOC corresponding RM. */

#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
    uint8_t txFifoWatermark;
    uint8_t rxFifoWatermark;
#endif
} serial_port_uart_config_t;
#if (defined(HAL_UART_DMA_ENABLE) && (HAL_UART_DMA_ENABLE > 0U))
typedef struct _serial_port_uart_dma_config
{
    uint32_t clockRate;                             /*!< clock rate  */
    uint32_t baudRate;                              /*!< baud rate  */
    serial_port_uart_parity_mode_t parityMode;      /*!< Parity mode, disabled (default), even, odd */
    serial_port_uart_stop_bit_count_t stopBitCount; /*!< Number of stop bits, 1 stop bit (default) or 2 stop bits  */

    uint8_t enableRx;                               /*!< Enable RX */
    uint8_t enableTx;                               /*!< Enable TX */
    uint8_t enableRxRTS;                            /*!< Enable RX RTS */
    uint8_t enableTxCTS;                            /*!< Enable TX CTS */
    uint8_t instance;                               /*!< Instance (0 - UART0, 1 - UART1, ...), detail information
                                                         please refer to the SOC corresponding RM. */
#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
    uint8_t txFifoWatermark;
    uint8_t rxFifoWatermark;
#endif
    uint8_t dma_instance;
    uint8_t rx_channel;
    uint8_t tx_channel;
    void *dma_mux_configure;
    void *dma_channel_mux_configure;

} serial_port_uart_dma_config_t;
#endif
/*! @} */
#endif /* __SERIAL_PORT_UART_H__ */
