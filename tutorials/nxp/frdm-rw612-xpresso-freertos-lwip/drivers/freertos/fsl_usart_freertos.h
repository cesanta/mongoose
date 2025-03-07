/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __FSL_USART_FREERTOS_H__
#define __FSL_USART_FREERTOS_H__

#include "fsl_usart.h"
#include <FreeRTOS.h>
#include <event_groups.h>
#include <semphr.h>

/*!
 * @addtogroup usart_freertos_driver
 * @{
 */

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief USART FreeRTOS driver version. */
#define FSL_USART_FREERTOS_DRIVER_VERSION (MAKE_VERSION(2, 7, 0))
/*@}*/

/*! @brief FLEX USART configuration structure */
struct rtos_usart_config
{
    USART_Type *base;                /*!< USART base address */
    uint32_t srcclk;                 /*!< USART source clock in Hz*/
    uint32_t baudrate;               /*!< Desired communication speed */
    usart_parity_mode_t parity;      /*!< Parity setting */
    usart_stop_bit_count_t stopbits; /*!< Number of stop bits to use */
    uint8_t *buffer;                 /*!< Buffer for background reception */
    uint32_t buffer_size;            /*!< Size of buffer for background reception */
    bool enableHardwareFlowControl;  /*!< Enable hardware control RTS/CTS */
};

/*! @brief FLEX USART FreeRTOS handle */
typedef struct _usart_rtos_handle
{
    USART_Type *base;              /*!< USART base address */
    usart_transfer_t txTransfer;   /*!< TX transfer structure */
    usart_transfer_t rxTransfer;   /*!< RX transfer structure */
    SemaphoreHandle_t rxSemaphore; /*!< RX semaphore for resource sharing */
    SemaphoreHandle_t txSemaphore; /*!< TX semaphore for resource sharing */
#define RTOS_USART_COMPLETE                0x1U
#define RTOS_USART_RING_BUFFER_OVERRUN     0x2U
#define RTOS_USART_HARDWARE_BUFFER_OVERRUN 0x4U
    EventGroupHandle_t rxEvent; /*!< RX completion event */
    EventGroupHandle_t txEvent; /*!< TX completion event */
    void *t_state;              /*!< Transactional state of the underlying driver */
#if (configSUPPORT_STATIC_ALLOCATION == 1)
    StaticSemaphore_t txSemaphoreBuffer; /*!< Statically allocated memory for txSemaphore */
    StaticSemaphore_t rxSemaphoreBuffer; /*!< Statically allocated memory for rxSemaphore */
    StaticEventGroup_t txEventBuffer;    /*!< Statically allocated memory for txEvent */
    StaticEventGroup_t rxEventBuffer;    /*!< Statically allocated memory for rxEvent */
#endif
} usart_rtos_handle_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name USART RTOS Operation
 * @{
 */

/*!
 * @brief Initializes a USART instance for operation in RTOS.
 *
 * @param handle The RTOS USART handle, the pointer to allocated space for RTOS context.
 * @param t_handle The pointer to allocated space where to store transactional layer internal state.
 * @param cfg The pointer to the parameters required to configure the USART after initialization.
 * @return 0 succeed, others fail.
 */
int USART_RTOS_Init(usart_rtos_handle_t *handle, usart_handle_t *t_handle, const struct rtos_usart_config *cfg);

/*!
 * @brief Deinitializes a USART instance for operation.
 *
 * This function deinitializes the USART module, sets all register values to reset value,
 * and releases the resources.
 *
 * @param handle The RTOS USART handle.
 */
int USART_RTOS_Deinit(usart_rtos_handle_t *handle);

/*!
 * @name USART transactional Operation
 * @{
 */

/*!
 * @brief Sends data in the background.
 *
 * This function sends data. It is a synchronous API.
 * If the hardware buffer is full, the task is in the blocked state.
 *
 * @param handle The RTOS USART handle.
 * @param buffer The pointer to buffer to send.
 * @param length The number of bytes to send.
 */
int USART_RTOS_Send(usart_rtos_handle_t *handle, uint8_t *buffer, uint32_t length);

/*!
 * @brief Receives data.
 *
 * This function receives data from USART. It is a synchronous API. If data is immediately available,
 * it is returned immediately and the number of bytes received.
 *
 * @param handle The RTOS USART handle.
 * @param buffer The pointer to buffer where to write received data.
 * @param length The number of bytes to receive.
 * @param received The pointer to a variable of size_t where the number of received data is filled.
 */
int USART_RTOS_Receive(usart_rtos_handle_t *handle, uint8_t *buffer, uint32_t length, size_t *received);

/* @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __FSL_USART_FREERTOS_H__ */
