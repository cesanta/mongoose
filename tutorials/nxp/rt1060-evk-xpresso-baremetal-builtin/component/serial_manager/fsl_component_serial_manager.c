/*
 * Copyright 2018-2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include "fsl_component_serial_manager.h"
#include "fsl_component_serial_port_internal.h"
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))

#include "fsl_component_generic_list.h"

/*
 * The OSA_USED macro can only be defined when the OSA component is used.
 * If the source code of the OSA component does not exist, the OSA_USED cannot be defined.
 * OR, If OSA component is not added into project event the OSA source code exists, the OSA_USED
 * also cannot be defined.
 * The source code path of the OSA component is <MCUXpresso_SDK>/components/osa.
 *
 */
#if defined(OSA_USED)

#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))
#include "fsl_component_common_task.h"
#else
#include "fsl_os_abstraction.h"
#endif

#endif

#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#ifndef NDEBUG
#if (defined(DEBUG_CONSOLE_ASSERT_DISABLE) && (DEBUG_CONSOLE_ASSERT_DISABLE > 0U))
#undef assert
#define assert(n)
#else
/* MISRA C-2012 Rule 17.2 */
#undef assert
#define assert(n) \
    while (!(n))  \
    {             \
        ;         \
    }
#endif
#endif

/* Weak function. */
#if defined(__GNUC__)
#define __WEAK_FUNC __attribute__((weak))
#elif defined(__ICCARM__)
#define __WEAK_FUNC __weak
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
#define __WEAK_FUNC __attribute__((weak))
#elif defined(__DSC__) || defined(__CW__)
#define __WEAK_FUNC __attribute__((weak))
#endif

#define SERIAL_EVENT_DATA_RECEIVED   (0U)
#define SERIAL_EVENT_DATA_SENT       (1U)
#define SERIAL_EVENT_DATA_START_SEND (2U)
#define SERIAL_EVENT_DATA_RX_NOTIFY  (3U)
#define SERIAL_EVENT_DATA_NUMBER     (4U)

#define SERIAL_MANAGER_WRITE_TAG 0xAABB5754U
#define SERIAL_MANAGER_READ_TAG  0xBBAA5244U

#ifndef RINGBUFFER_WATERMARK_THRESHOLD
#define RINGBUFFER_WATERMARK_THRESHOLD 95U / 100U
#endif

#ifndef gSerialManagerLpConstraint_c
#define gSerialManagerLpConstraint_c 0
#endif

#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
typedef enum _serial_manager_transmission_mode
{
    kSerialManager_TransmissionBlocking    = 0x0U, /*!< Blocking transmission*/
    kSerialManager_TransmissionNonBlocking = 0x1U, /*!< None blocking transmission*/
} serial_manager_transmission_mode_t;

/* TX transfer structure */
typedef struct _serial_manager_transfer
{
    uint8_t *buffer;
    volatile uint32_t length;
    volatile uint32_t soFar;
    serial_manager_transmission_mode_t mode;
    serial_manager_status_t status;
} serial_manager_transfer_t;
#endif

/* write handle structure */
typedef struct _serial_manager_send_handle
{
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
    list_element_t link; /*!< list element of the link */
    serial_manager_transfer_t transfer;
#endif
    struct _serial_manager_handle *serialManagerHandle;
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
    serial_manager_callback_t callback;
    void *callbackParam;
    uint32_t tag;
#endif
} serial_manager_write_handle_t;
typedef struct _serial_manager_send_block_handle
{
    struct _serial_manager_handle *serialManagerHandle;

} serial_manager_write_block_handle_t;

typedef serial_manager_write_handle_t serial_manager_read_handle_t;
typedef serial_manager_write_block_handle_t serial_manager_read_block_handle_t;

#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
/* receive state structure */
typedef struct _serial_manager_read_ring_buffer
{
    uint8_t *ringBuffer;
    uint32_t ringBufferSize;
    volatile uint32_t ringHead;
    volatile uint32_t ringTail;
} serial_manager_read_ring_buffer_t;

#if defined(__CC_ARM)
#pragma anon_unions
#endif
typedef struct _serial_manager_block_handle
{
    serial_manager_type_t handleType;
    serial_port_type_t type;
    serial_manager_read_handle_t *volatile openedReadHandleHead;
    volatile uint32_t openedWriteHandleCount;
    union
    {
        uint32_t lowLevelhandleBuffer[1];
#if (defined(SERIAL_PORT_TYPE_UART) && (SERIAL_PORT_TYPE_UART > 0U))
        uint8_t uartHandleBuffer[SERIAL_PORT_UART_BLOCK_HANDLE_SIZE];
#endif
    };

} serial_manager_block_handle_t;
#endif

/* The serial manager handle structure */
typedef struct _serial_manager_handle
{
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
    serial_manager_type_t handleType;
#endif
    serial_port_type_t serialPortType;
    serial_manager_read_handle_t *volatile openedReadHandleHead;
    volatile uint32_t openedWriteHandleCount;
    union
    {
        uint32_t lowLevelhandleBuffer[1];
#if (defined(SERIAL_PORT_TYPE_UART) && (SERIAL_PORT_TYPE_UART > 0U))
        uint8_t uartHandleBuffer[SERIAL_PORT_UART_HANDLE_SIZE];
#endif
#if (defined(SERIAL_PORT_TYPE_UART_DMA) && (SERIAL_PORT_TYPE_UART_DMA > 0U))
        uint8_t uartDmaHandleBuffer[SERIAL_PORT_UART_DMA_HANDLE_SIZE];
#endif
#if (defined(SERIAL_PORT_TYPE_USBCDC) && (SERIAL_PORT_TYPE_USBCDC > 0U))
        uint8_t usbcdcHandleBuffer[SERIAL_PORT_USB_CDC_HANDLE_SIZE];
#endif
#if (defined(SERIAL_PORT_TYPE_SWO) && (SERIAL_PORT_TYPE_SWO > 0U))
        uint8_t swoHandleBuffer[SERIAL_PORT_SWO_HANDLE_SIZE];
#endif
#if (defined(SERIAL_PORT_TYPE_VIRTUAL) && (SERIAL_PORT_TYPE_VIRTUAL > 0U))
        uint8_t usbcdcVirtualHandleBuffer[SERIAL_PORT_VIRTUAL_HANDLE_SIZE];
#endif
#if (defined(SERIAL_PORT_TYPE_RPMSG) && (SERIAL_PORT_TYPE_RPMSG > 0U))
        uint8_t rpmsgHandleBuffer[SERIAL_PORT_RPMSG_HANDLE_SIZE];
#endif
#if (defined(SERIAL_PORT_TYPE_SPI_MASTER) && (SERIAL_PORT_TYPE_SPI_MASTER > 0U))
        uint8_t spiMasterHandleBuffer[SERIAL_PORT_SPI_MASTER_HANDLE_SIZE];
#endif
#if (defined(SERIAL_PORT_TYPE_SPI_SLAVE) && (SERIAL_PORT_TYPE_SPI_SLAVE > 0U))
        uint8_t spiSlaveHandleBuffer[SERIAL_PORT_SPI_SLAVE_HANDLE_SIZE];
#endif
#if (defined(SERIAL_PORT_TYPE_BLE_WU) && (SERIAL_PORT_TYPE_BLE_WU > 0U))
        uint8_t bleWuHandleBuffer[SERIAL_PORT_BLE_WU_HANDLE_SIZE];
#endif
    };
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
    serial_manager_read_ring_buffer_t ringBuffer;
#endif

#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))

#if defined(OSA_USED)

#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))
    common_task_message_t commontaskMsg;
#else
    OSA_SEMAPHORE_HANDLE_DEFINE(serSemaphore); /*!< Semaphore instance */
    OSA_TASK_HANDLE_DEFINE(taskId);            /*!< Task handle */
#endif
    uint8_t serialManagerState[SERIAL_EVENT_DATA_NUMBER]; /*!< Used to indicate the serial mnager state */

#endif

#endif
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
    list_label_t runningWriteHandleHead;   /*!< The queue of running write handle */
    list_label_t completedWriteHandleHead; /*!< The queue of completed write handle */
#endif

} serial_manager_handle_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
static void SerialManager_Task(void *param);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))

#if defined(OSA_USED)

#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))

#else
                                               /*
                                                * \brief Defines the serial manager task's stack
                                                */
static OSA_TASK_DEFINE(SerialManager_Task, SERIAL_MANAGER_TASK_PRIORITY, 1, SERIAL_MANAGER_TASK_STACK_SIZE, false);
#endif

#endif

#endif
static const serial_manager_lowpower_critical_CBs_t *s_pfserialLowpowerCriticalCallbacks = NULL;
/*******************************************************************************
 * Code
 ******************************************************************************/

#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
static void SerialManager_AddTail(list_label_t *queue, serial_manager_write_handle_t *node)
{
    (void)LIST_AddTail(queue, &node->link);
}

static void SerialManager_RemoveHead(list_label_t *queue)
{
    (void)LIST_RemoveHead(queue);
}

static int32_t SerialManager_SetLpConstraint(int32_t power_mode)
{
    int32_t status = -1;
    if ((s_pfserialLowpowerCriticalCallbacks != NULL) &&
        (s_pfserialLowpowerCriticalCallbacks->serialEnterLowpowerCriticalFunc != NULL))
    {
        status = s_pfserialLowpowerCriticalCallbacks->serialEnterLowpowerCriticalFunc(power_mode);
    }
    return status;
}
static int32_t SerialManager_ReleaseLpConstraint(int32_t power_mode)
{
    int32_t status = -1;
    if ((s_pfserialLowpowerCriticalCallbacks != NULL) &&
        (s_pfserialLowpowerCriticalCallbacks->serialExitLowpowerCriticalFunc != NULL))
    {
        status = s_pfserialLowpowerCriticalCallbacks->serialExitLowpowerCriticalFunc(power_mode);
    }
    return status;
}
#endif

#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))

static serial_manager_status_t SerialManager_StartWriting(serial_manager_handle_t *handle)
{
    serial_manager_status_t status = kStatus_SerialManager_Error;
    serial_manager_write_handle_t *writeHandle =
        (serial_manager_write_handle_t *)(void *)LIST_GetHead(&handle->runningWriteHandleHead);

    if (writeHandle != NULL)
    {
        (void)SerialManager_SetLpConstraint(gSerialManagerLpConstraint_c);
        switch (handle->serialPortType)
        {
#if (defined(SERIAL_PORT_TYPE_UART) && (SERIAL_PORT_TYPE_UART > 0U))
            case kSerialPort_Uart:
                status = Serial_UartWrite(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                          writeHandle->transfer.buffer, writeHandle->transfer.length);
                break;
#endif
#if (defined(SERIAL_PORT_TYPE_UART_DMA) && (SERIAL_PORT_TYPE_UART_DMA > 0U))
            case kSerialPort_UartDma:
                status = Serial_UartDmaWrite(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                             writeHandle->transfer.buffer, writeHandle->transfer.length);
                break;
#endif
#if (defined(SERIAL_PORT_TYPE_USBCDC) && (SERIAL_PORT_TYPE_USBCDC > 0U))
            case kSerialPort_UsbCdc:
                status = Serial_UsbCdcWrite(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                            writeHandle->transfer.buffer, writeHandle->transfer.length);
                break;
#endif
#if (defined(SERIAL_PORT_TYPE_SWO) && (SERIAL_PORT_TYPE_SWO > 0U))
            case kSerialPort_Swo:
                status = Serial_SwoWrite(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                         writeHandle->transfer.buffer, writeHandle->transfer.length);
                break;
#endif
#if (defined(SERIAL_PORT_TYPE_VIRTUAL) && (SERIAL_PORT_TYPE_VIRTUAL > 0U))
            case kSerialPort_Virtual:
                status = Serial_PortVirtualWrite(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                                 writeHandle->transfer.buffer, writeHandle->transfer.length);
                break;
#endif
#if (defined(SERIAL_PORT_TYPE_RPMSG) && (SERIAL_PORT_TYPE_RPMSG > 0U))
            case kSerialPort_Rpmsg:
                status = Serial_RpmsgWrite(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                           writeHandle->transfer.buffer, writeHandle->transfer.length);
                break;
#endif
#if (defined(SERIAL_PORT_TYPE_SPI_MASTER) && (SERIAL_PORT_TYPE_SPI_MASTER > 0U))
            case kSerialPort_SpiMaster:
                status = Serial_SpiMasterWrite(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                               writeHandle->transfer.buffer, writeHandle->transfer.length);
                break;
#endif
#if (defined(SERIAL_PORT_TYPE_SPI_SLAVE) && (SERIAL_PORT_TYPE_SPI_SLAVE > 0U))
            case kSerialPort_SpiSlave:
                status = Serial_SpiSlaveWrite(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                              writeHandle->transfer.buffer, writeHandle->transfer.length);
                break;
#endif
#if (defined(SERIAL_PORT_TYPE_BLE_WU) && (SERIAL_PORT_TYPE_BLE_WU > 0U))
            case kSerialPort_BleWu:
                status = Serial_PortBleWuWrite(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                                 writeHandle->transfer.buffer, writeHandle->transfer.length);
                break;
#endif

            default:
                status = kStatus_SerialManager_Error;
                break;
        }
    }
    return status;
}

static serial_manager_status_t SerialManager_StartReading(serial_manager_handle_t *handle,
                                                          serial_manager_read_handle_t *readHandle,
                                                          uint8_t *buffer,
                                                          uint32_t length)
{
    serial_manager_status_t status = kStatus_SerialManager_Error;

    if (NULL != readHandle)
    {
#if (defined(SERIAL_PORT_TYPE_UART) && (SERIAL_PORT_TYPE_UART > 0U))
        if (kSerialPort_Uart == handle->serialPortType) /* Serial port UART */
        {
            status = Serial_UartRead(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), buffer, length);
        }
#endif
#if (defined(SERIAL_PORT_TYPE_USBCDC) && (SERIAL_PORT_TYPE_USBCDC > 0U))
        if (handle->serialPortType == kSerialPort_UsbCdc)
        {
            status = Serial_UsbCdcRead(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), buffer, length);
        }
#endif
#if (defined(SERIAL_PORT_TYPE_VIRTUAL) && (SERIAL_PORT_TYPE_VIRTUAL > 0U))
        if (handle->serialPortType == kSerialPort_Virtual)
        {
            status = Serial_PortVirtualRead(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), buffer, length);
        }
#endif
#if (defined(SERIAL_PORT_TYPE_SPI_MASTER) && (SERIAL_PORT_TYPE_SPI_MASTER > 0U))
        if (handle->serialPortType == kSerialPort_SpiMaster)
        {
            status = Serial_SpiMasterRead(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), buffer, length);
        }
#endif
#if (defined(SERIAL_PORT_TYPE_SPI_SLAVE) && (SERIAL_PORT_TYPE_SPI_SLAVE > 0U))
        if (handle->serialPortType == kSerialPort_SpiSlave)
        {
            status = Serial_SpiSlaveRead(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), buffer, length);
        }
#endif

#if 0
#if (defined(SERIAL_PORT_TYPE_RPMSG) && (SERIAL_PORT_TYPE_RPMSG > 0U))
        if (handle->serialPortType == kSerialPort_Rpmsg)
        {
            status = Serial_RpmsgRead(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), buffer, length);
        }
#endif
#endif
#if (defined(SERIAL_PORT_TYPE_BLE_WU) && (SERIAL_PORT_TYPE_BLE_WU > 0U))
        if (handle->serialPortType == kSerialPort_BleWu)
        {
            status = Serial_PortBleWuRead(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), buffer, length);
        }
#endif
    }
    return status;
}

#else /*SERIAL_MANAGER_NON_BLOCKING_MODE > 0U*/

static serial_manager_status_t SerialManager_StartWriting(serial_manager_handle_t *handle,
                                                          serial_manager_write_handle_t *writeHandle,
                                                          uint8_t *buffer,
                                                          uint32_t length)
{
    serial_manager_status_t status = kStatus_SerialManager_Error;

#if (defined(SERIAL_PORT_TYPE_UART) && (SERIAL_PORT_TYPE_UART > 0U))
    if (kSerialPort_Uart == handle->serialPortType) /* Serial port UART */
    {
        status = Serial_UartWrite(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), buffer, length);
    }
    else
#endif
#if (defined(SERIAL_PORT_TYPE_USBCDC) && (SERIAL_PORT_TYPE_USBCDC > 0U))
        if (kSerialPort_UsbCdc == handle->serialPortType) /* Serial port UsbCdc */
    {
        status = Serial_UsbCdcWrite(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), buffer, length);
    }
    else
#endif
#if (defined(SERIAL_PORT_TYPE_SWO) && (SERIAL_PORT_TYPE_SWO > 0U))
        if (kSerialPort_Swo == handle->serialPortType) /* Serial port SWO */
    {
        status = Serial_SwoWrite(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), buffer, length);
    }
    else
#endif
#if (defined(SERIAL_PORT_TYPE_VIRTUAL) && (SERIAL_PORT_TYPE_VIRTUAL > 0U))
        if (kSerialPort_Virtual == handle->serialPortType) /* Serial port UsbCdcVirtual */
    {
        status = Serial_PortVirtualWrite(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), buffer, length);
    }
    else
#endif
#if (defined(SERIAL_PORT_TYPE_RPMSG) && (SERIAL_PORT_TYPE_RPMSG > 0U))
        if (kSerialPort_Rpmsg == handle->serialPortType) /* Serial port Rpmsg */
    {
        status = Serial_RpmsgWrite(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), buffer, length);
    }
    else
#endif
#if (defined(SERIAL_PORT_TYPE_SPI_MASTER) && (SERIAL_PORT_TYPE_SPI_MASTER > 0U))
        if (kSerialPort_SpiMaster == handle->serialPortType) /* Serial port Spi Master */
    {
        status = Serial_SpiMasterWrite(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), buffer, length);
    }
    else
#endif
#if (defined(SERIAL_PORT_TYPE_BLE_WU) && (SERIAL_PORT_TYPE_BLE_WU > 0U))
        if (kSerialPort_BleWu == handle->serialPortType) /* Serial port BLE WU */
    {
        status = Serial_PortBleWuWrite(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), buffer, length);
    }
    else
#endif
    {
        /*MISRA rule*/
    }
    return status;
}

static serial_manager_status_t SerialManager_StartReading(serial_manager_handle_t *handle,
                                                          serial_manager_read_handle_t *readHandle,
                                                          uint8_t *buffer,
                                                          uint32_t length)
{
    serial_manager_status_t status = kStatus_SerialManager_Error;

#if (defined(SERIAL_PORT_TYPE_UART) && (SERIAL_PORT_TYPE_UART > 0U))
    if (kSerialPort_Uart == handle->serialPortType) /* Serial port UART */
    {
        status = Serial_UartRead(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), buffer, length);
    }
    else
#endif
#if (defined(SERIAL_PORT_TYPE_USBCDC) && (SERIAL_PORT_TYPE_USBCDC > 0U))
        if (kSerialPort_UsbCdc == handle->serialPortType) /* Serial port UsbCdc */
    {
        status = Serial_UsbCdcRead(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), buffer, length);
    }
    else
#endif
#if (defined(SERIAL_PORT_TYPE_SWO) && (SERIAL_PORT_TYPE_SWO > 0U))
        if (kSerialPort_Swo == handle->serialPortType) /* Serial port SWO */
    {
        status = Serial_SwoRead(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), buffer, length);
    }
    else
#endif
#if (defined(SERIAL_PORT_TYPE_VIRTUAL) && (SERIAL_PORT_TYPE_VIRTUAL > 0U))
        if (kSerialPort_Virtual == handle->serialPortType) /* Serial port UsbCdcVirtual */
    {
        status = Serial_PortVirtualRead(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), buffer, length);
    }
    else
#endif
#if (defined(SERIAL_PORT_TYPE_RPMSG) && (SERIAL_PORT_TYPE_RPMSG > 0U))
        if (kSerialPort_Rpmsg == handle->serialPortType) /* Serial port UsbCdcVirtual */
    {
        status = Serial_RpmsgRead(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), buffer, length);
    }
    else
#endif
#if (defined(SERIAL_PORT_TYPE_SPI_MASTER) && (SERIAL_PORT_TYPE_SPI_MASTER > 0U))
        if (kSerialPort_SpiMaster == handle->serialPortType) /* Serial port Spi Master */
    {
        status = Serial_SpiMasterRead(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), buffer, length);
    }
    else
#endif
#if (defined(SERIAL_PORT_TYPE_BLE_WU) && (SERIAL_PORT_TYPE_BLE_WU > 0U))
        if (kSerialPort_BleWu == handle->serialPortType) /* Serial port BLE WU */
    {
        status = Serial_PortBleWuRead(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), buffer, length);
    }
    else
#endif
    {
        /*MISRA rule*/
    }
    return status;
}
#endif /*SERIAL_MANAGER_NON_BLOCKING_MODE > 0U*/

#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
static void SerialManager_IsrFunction(serial_manager_handle_t *handle)
{
    uint32_t regPrimask = DisableGlobalIRQ();
    switch (handle->serialPortType)
    {
#if (defined(SERIAL_PORT_TYPE_UART) && (SERIAL_PORT_TYPE_UART > 0U))
        case kSerialPort_Uart:
            Serial_UartIsrFunction(((serial_handle_t)&handle->lowLevelhandleBuffer[0]));
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_UART_DMA) && (SERIAL_PORT_TYPE_UART_DMA > 0U))
        case kSerialPort_UartDma:
            Serial_UartIsrFunction(((serial_handle_t)&handle->lowLevelhandleBuffer[0]));
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_USBCDC) && (SERIAL_PORT_TYPE_USBCDC > 0U))
        case kSerialPort_UsbCdc:
            Serial_UsbCdcIsrFunction(((serial_handle_t)&handle->lowLevelhandleBuffer[0]));
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_SWO) && (SERIAL_PORT_TYPE_SWO > 0U))
        case kSerialPort_Swo:
            Serial_SwoIsrFunction(((serial_handle_t)&handle->lowLevelhandleBuffer[0]));
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_VIRTUAL) && (SERIAL_PORT_TYPE_VIRTUAL > 0U))
        case kSerialPort_Virtual:
            Serial_PortVirtualIsrFunction(((serial_handle_t)&handle->lowLevelhandleBuffer[0]));
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_BLE_WU) && (SERIAL_PORT_TYPE_BLE_WU > 0U))
        case kSerialPort_BleWu:
            Serial_PortBleWuIsrFunction(((serial_handle_t)&handle->lowLevelhandleBuffer[0]));
            break;
#endif
        default:
            /*MISRA rule 16.4*/
            break;
    }
    EnableGlobalIRQ(regPrimask);
}

static void SerialManager_Task(void *param)
{
    serial_manager_handle_t *handle = (serial_manager_handle_t *)param;
    serial_manager_write_handle_t *serialWriteHandle;
    serial_manager_read_handle_t *serialReadHandle;
    uint32_t primask;
    serial_manager_callback_message_t msg;
#if (defined(SERIAL_MANAGER_TASK_HANDLE_RX_AVAILABLE_NOTIFY) && (SERIAL_MANAGER_TASK_HANDLE_RX_AVAILABLE_NOTIFY > 0U))
    uint32_t ringBufferLength;
#endif /* SERIAL_MANAGER_TASK_HANDLE_RX_AVAILABLE_NOTIFY */

    if (NULL != handle)
    {
#if defined(OSA_USED)

#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))
#else

        do
        {
            if (KOSA_StatusSuccess == OSA_SemaphoreWait((osa_semaphore_handle_t)handle->serSemaphore, osaWaitForever_c))
            {
#endif
#endif
#if defined(OSA_USED)

#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))
#else
                primask = DisableGlobalIRQ();
                uint8_t *ev = handle->serialManagerState;
                EnableGlobalIRQ(primask);
                if (0U != (ev[SERIAL_EVENT_DATA_START_SEND]))
#endif
#endif
        {
            (void)SerialManager_StartWriting(handle);
#if defined(OSA_USED)
#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))
#else
                    primask = DisableGlobalIRQ();
                    handle->serialManagerState[SERIAL_EVENT_DATA_START_SEND]--;
                    EnableGlobalIRQ(primask);
#endif
#endif
        }
#if defined(OSA_USED)

#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))
#else
                if (0U != (ev[SERIAL_EVENT_DATA_SENT]))
#endif

#endif
        {
            serialWriteHandle =
                (serial_manager_write_handle_t *)(void *)LIST_GetHead(&handle->completedWriteHandleHead);
            while (NULL != serialWriteHandle)
            {
                SerialManager_RemoveHead(&handle->completedWriteHandleHead);
                msg.buffer                         = serialWriteHandle->transfer.buffer;
                msg.length                         = serialWriteHandle->transfer.soFar;
                serialWriteHandle->transfer.buffer = NULL;
                if (NULL != serialWriteHandle->callback)
                {
                    serialWriteHandle->callback(serialWriteHandle->callbackParam, &msg,
                                                serialWriteHandle->transfer.status);
                }
                serialWriteHandle =
                    (serial_manager_write_handle_t *)(void *)LIST_GetHead(&handle->completedWriteHandleHead);
                (void)SerialManager_ReleaseLpConstraint(gSerialManagerLpConstraint_c);
            }
#if defined(OSA_USED)
#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))
#else
                    primask = DisableGlobalIRQ();
                    handle->serialManagerState[SERIAL_EVENT_DATA_SENT]--;
                    EnableGlobalIRQ(primask);
#endif
#endif
        }
#if defined(OSA_USED)

#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))
#else
                if (0U != (ev[SERIAL_EVENT_DATA_RECEIVED]))
#endif

#endif
        {
            primask          = DisableGlobalIRQ();
            serialReadHandle = handle->openedReadHandleHead;
            EnableGlobalIRQ(primask);

            if (NULL != serialReadHandle)
            {
                if (NULL != serialReadHandle->transfer.buffer)
                {
                    if (serialReadHandle->transfer.soFar >= serialReadHandle->transfer.length)
                    {
                        msg.buffer                        = serialReadHandle->transfer.buffer;
                        msg.length                        = serialReadHandle->transfer.soFar;
                        serialReadHandle->transfer.buffer = NULL;
                        if (NULL != serialReadHandle->callback)
                        {
                            serialReadHandle->callback(serialReadHandle->callbackParam, &msg,
                                                       serialReadHandle->transfer.status);
                        }
                    }
                }
            }
#if defined(OSA_USED)
#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))
#else
                    primask = DisableGlobalIRQ();
                    handle->serialManagerState[SERIAL_EVENT_DATA_RECEIVED]--;
                    EnableGlobalIRQ(primask);
#endif
#endif
        }

#if (defined(SERIAL_MANAGER_TASK_HANDLE_RX_AVAILABLE_NOTIFY) && (SERIAL_MANAGER_TASK_HANDLE_RX_AVAILABLE_NOTIFY > 0U))
#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))
#else
                if (0U != (ev[SERIAL_EVENT_DATA_RX_NOTIFY]))
#endif
        {
            primask                                                 = DisableGlobalIRQ();
            handle->serialManagerState[SERIAL_EVENT_DATA_RX_NOTIFY] = 0;
            ringBufferLength =
                handle->ringBuffer.ringHead + handle->ringBuffer.ringBufferSize - handle->ringBuffer.ringTail;
            ringBufferLength = ringBufferLength % handle->ringBuffer.ringBufferSize;
            EnableGlobalIRQ(primask);
            /* Notify there are data in ringbuffer */
            if (0U != ringBufferLength)
            {
                msg.buffer = NULL;
                msg.length = ringBufferLength;
                if ((NULL != handle->openedReadHandleHead) && (NULL != handle->openedReadHandleHead->callback))
                {
                    handle->openedReadHandleHead->callback(handle->openedReadHandleHead->callbackParam, &msg,
                                                           kStatus_SerialManager_Notify);
                }
            }
        }
#endif /* SERIAL_MANAGER_TASK_HANDLE_RX_AVAILABLE_NOTIFY */

#if defined(OSA_USED)

#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))
#else
            }
        } while (0U != gUseRtos_c);
#endif

#endif
    }
}
#endif

#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
static void SerialManager_TxCallback(void *callbackParam,
                                     serial_manager_callback_message_t *message,
                                     serial_manager_status_t status)
{
    serial_manager_handle_t *handle;
    serial_manager_write_handle_t *writeHandle;
#if (defined(OSA_USED))
#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))
    /* Need to support common_task. */
#else /* SERIAL_MANAGER_USE_COMMON_TASK */
    uint32_t primask;
#endif
#endif
    assert(NULL != callbackParam);
    assert(NULL != message);

    handle = (serial_manager_handle_t *)callbackParam;

    writeHandle = (serial_manager_write_handle_t *)(void *)LIST_GetHead(&handle->runningWriteHandleHead);

    if (NULL != writeHandle)
    {
        SerialManager_RemoveHead(&handle->runningWriteHandleHead);

#if (defined(OSA_USED) && defined(SERIAL_MANAGER_TASK_HANDLE_TX) && (SERIAL_MANAGER_TASK_HANDLE_TX == 1))
#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))
        /* Need to support common_task. */
#else /* SERIAL_MANAGER_USE_COMMON_TASK */
        primask = DisableGlobalIRQ();
        handle->serialManagerState[SERIAL_EVENT_DATA_START_SEND]++;
        EnableGlobalIRQ(primask);
        (void)OSA_SemaphorePost((osa_semaphore_handle_t)handle->serSemaphore);

#endif /* SERIAL_MANAGER_USE_COMMON_TASK */
#else  /* OSA_USED && SERIAL_MANAGER_TASK_HANDLE_TX */
        (void)SerialManager_StartWriting(handle);
#endif /* OSA_USED && SERIAL_MANAGER_TASK_HANDLE_TX */

        writeHandle->transfer.soFar  = message->length;
        writeHandle->transfer.status = status;
        if (kSerialManager_TransmissionNonBlocking == writeHandle->transfer.mode)
        {
            SerialManager_AddTail(&handle->completedWriteHandleHead, writeHandle);
#if defined(OSA_USED)

#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))
            handle->commontaskMsg.callback      = SerialManager_Task;
            handle->commontaskMsg.callbackParam = handle;
            COMMON_TASK_post_message(&handle->commontaskMsg);
#else
            primask = DisableGlobalIRQ();
            handle->serialManagerState[SERIAL_EVENT_DATA_SENT]++;
            EnableGlobalIRQ(primask);
            (void)OSA_SemaphorePost((osa_semaphore_handle_t)handle->serSemaphore);
#endif

#else
            SerialManager_Task(handle);
#endif
        }
        else
        {
            writeHandle->transfer.buffer = NULL;
            (void)SerialManager_ReleaseLpConstraint(gSerialManagerLpConstraint_c);
        }
    }
}

void SerialManager_RxCallback(void *callbackParam,
                              serial_manager_callback_message_t *message,
                              serial_manager_status_t status);
void SerialManager_RxCallback(void *callbackParam,
                              serial_manager_callback_message_t *message,
                              serial_manager_status_t status)
{
    serial_manager_handle_t *handle;
#if (!((defined(SERIAL_PORT_TYPE_SPI_MASTER) && (SERIAL_PORT_TYPE_SPI_MASTER > 0U))) && \
     !((defined(SERIAL_PORT_TYPE_SPI_SLAVE) && (SERIAL_PORT_TYPE_SPI_SLAVE > 0U))))
    uint32_t ringBufferLength = 0;
    uint32_t primask;
#endif
    assert(NULL != callbackParam);
    assert(NULL != message);

    handle = (serial_manager_handle_t *)callbackParam;
#if ((defined(SERIAL_PORT_TYPE_SPI_MASTER) && (SERIAL_PORT_TYPE_SPI_MASTER > 0U)) || \
     (defined(SERIAL_PORT_TYPE_SPI_SLAVE) && (SERIAL_PORT_TYPE_SPI_SLAVE > 0U)))
    handle->openedReadHandleHead->transfer.status = kStatus_SerialManager_Success;
    handle->openedReadHandleHead->transfer.soFar  = message->length;
    handle->openedReadHandleHead->transfer.length = message->length;
    handle->openedReadHandleHead->transfer.buffer = message->buffer;
#if defined(OSA_USED)

#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))
    handle->commontaskMsg.callback      = SerialManager_Task;
    handle->commontaskMsg.callbackParam = handle;
    COMMON_TASK_post_message(&handle->commontaskMsg);
#else
    primask = DisableGlobalIRQ();
    handle->serialManagerState[SERIAL_EVENT_DATA_RECEIVED]++;
    EnableGlobalIRQ(primask);
    (void)OSA_SemaphorePost((osa_semaphore_handle_t)handle->serSemaphore);
#endif

#else
    SerialManager_Task(handle);
#endif
#else
    status = kStatus_SerialManager_Notify;

    primask = DisableGlobalIRQ();

    /* If wrap around is expected copy byte one after the other. Note that this could also be done with 2 memcopy for
     * better efficiency. */
    if (handle->ringBuffer.ringHead + message->length >= handle->ringBuffer.ringBufferSize)
    {
        for (uint32_t i = 0; i < message->length; i++)
        {
            handle->ringBuffer.ringBuffer[handle->ringBuffer.ringHead++] = message->buffer[i];

            if (handle->ringBuffer.ringHead >= handle->ringBuffer.ringBufferSize)
            {
                handle->ringBuffer.ringHead = 0U;
            }
            if (handle->ringBuffer.ringHead == handle->ringBuffer.ringTail)
            {
                status = kStatus_SerialManager_RingBufferOverflow;
                handle->ringBuffer.ringTail++;
                if (handle->ringBuffer.ringTail >= handle->ringBuffer.ringBufferSize)
                {
                    handle->ringBuffer.ringTail = 0U;
                }
            }
        }
    }
    else /*No wrap is expected so do a memcpy*/
    {
        (void)memcpy(&handle->ringBuffer.ringBuffer[handle->ringBuffer.ringHead], message->buffer, message->length);
        handle->ringBuffer.ringHead += message->length;
    }

    ringBufferLength = handle->ringBuffer.ringHead + handle->ringBuffer.ringBufferSize - handle->ringBuffer.ringTail;
    ringBufferLength = ringBufferLength % handle->ringBuffer.ringBufferSize;

    if ((NULL != handle->openedReadHandleHead) && (NULL != handle->openedReadHandleHead->transfer.buffer))
    {
        if (handle->openedReadHandleHead->transfer.length > handle->openedReadHandleHead->transfer.soFar)
        {
            uint32_t remainLength =
                handle->openedReadHandleHead->transfer.length - handle->openedReadHandleHead->transfer.soFar;
            for (uint32_t i = 0; i < MIN(ringBufferLength, remainLength); i++)
            {
                handle->openedReadHandleHead->transfer.buffer[handle->openedReadHandleHead->transfer.soFar] =
                    handle->ringBuffer.ringBuffer[handle->ringBuffer.ringTail];
                handle->ringBuffer.ringTail++;
                handle->openedReadHandleHead->transfer.soFar++;
                if (handle->ringBuffer.ringTail >= handle->ringBuffer.ringBufferSize)
                {
                    handle->ringBuffer.ringTail = 0U;
                }
            }
            ringBufferLength = ringBufferLength - MIN(ringBufferLength, remainLength);
        }

        if (handle->openedReadHandleHead->transfer.length > handle->openedReadHandleHead->transfer.soFar)
        {
        }
        else
        {
            if (kSerialManager_TransmissionBlocking == handle->openedReadHandleHead->transfer.mode)
            {
                handle->openedReadHandleHead->transfer.buffer = NULL;
            }
            else
            {
                handle->openedReadHandleHead->transfer.status = kStatus_SerialManager_Success;

#if defined(OSA_USED)

#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))
                handle->commontaskMsg.callback                = SerialManager_Task;
                handle->commontaskMsg.callbackParam           = handle;
                COMMON_TASK_post_message(&handle->commontaskMsg);
#else
                handle->serialManagerState[SERIAL_EVENT_DATA_RECEIVED]++;
                (void)OSA_SemaphorePost((osa_semaphore_handle_t)handle->serSemaphore);
#endif

#else
                SerialManager_Task(handle);
#endif
            }
        }
    }
#if (defined(SERIAL_MANAGER_RING_BUFFER_FLOWCONTROL) && (SERIAL_MANAGER_RING_BUFFER_FLOWCONTROL > 0U))
    uint32_t ringBufferWaterMark =
        handle->ringBuffer.ringHead + handle->ringBuffer.ringBufferSize - handle->ringBuffer.ringTail;
    ringBufferWaterMark = ringBufferWaterMark % handle->ringBuffer.ringBufferSize;
    if (ringBufferWaterMark < (uint32_t)(handle->ringBuffer.ringBufferSize * RINGBUFFER_WATERMARK_THRESHOLD))
    {
        (void)SerialManager_StartReading(handle, handle->openedReadHandleHead, NULL, ringBufferLength);
    }
#else
    (void)SerialManager_StartReading(handle, handle->openedReadHandleHead, NULL, ringBufferLength);
#endif
    if (0U != ringBufferLength)
    {
#if (defined(SERIAL_MANAGER_TASK_HANDLE_RX_AVAILABLE_NOTIFY) && (SERIAL_MANAGER_TASK_HANDLE_RX_AVAILABLE_NOTIFY > 0U))
        if (handle->serialManagerState[SERIAL_EVENT_DATA_RX_NOTIFY] == 0)
        {
            handle->serialManagerState[SERIAL_EVENT_DATA_RX_NOTIFY]++;
            (void)OSA_SemaphorePost((osa_semaphore_handle_t)handle->serSemaphore);
        }

        (void)status; /* Fix "set but never used" warning. */
#else  /* !SERIAL_MANAGER_TASK_HANDLE_RX_AVAILABLE_NOTIFY */
        message->buffer = NULL;
        message->length = ringBufferLength;
        if ((NULL != handle->openedReadHandleHead) && (NULL != handle->openedReadHandleHead->callback))
        {
            handle->openedReadHandleHead->callback(handle->openedReadHandleHead->callbackParam, message, status);
        }
#endif /* SERIAL_MANAGER_TASK_HANDLE_RX_AVAILABLE_NOTIFY */
    }

#if (!((defined(SERIAL_PORT_TYPE_USBCDC) && (SERIAL_PORT_TYPE_USBCDC > 0U))) && \
     !((defined(SERIAL_PORT_TYPE_VIRTUAL) && (SERIAL_PORT_TYPE_VIRTUAL > 0U))))
    if (kSerialManager_Blocking ==
        handle->handleType) /* No need to check for (NULL != handle->openedReadHandleHead) condition as it is already
                               done in SerialManager_StartReading() */
#else
    if (NULL != handle->openedReadHandleHead)
#endif
    {
        ringBufferLength = handle->ringBuffer.ringBufferSize - 1U - ringBufferLength;
        (void)SerialManager_StartReading(handle, handle->openedReadHandleHead, NULL, ringBufferLength);
    }
    EnableGlobalIRQ(primask);
#endif
}

/*
 * This function is used for perdiodic check if the transfer is complete, and will be called in blocking transfer at
 * non-blocking mode. The perdiodic unit is ms and default value is define by
 * SERIAL_MANAGER_WRITE_TIME_DELAY_DEFAULT_VALUE/SERIAL_MANAGER_READ_TIME_DELAY_DEFAULT_VALUE. The function
 * SerialManager_WriteTimeDelay()/SerialManager_ReadTimeDelay() is a weak function, so it could be re-implemented by
 * upper layer.
 */
__WEAK_FUNC void SerialManager_WriteTimeDelay(uint32_t ms);
__WEAK_FUNC void SerialManager_WriteTimeDelay(uint32_t ms)
{
#if defined(OSA_USED)
    OSA_TimeDelay(ms);
#endif
}

__WEAK_FUNC void SerialManager_ReadTimeDelay(uint32_t ms);
__WEAK_FUNC void SerialManager_ReadTimeDelay(uint32_t ms)
{
#if defined(OSA_USED)
    OSA_TimeDelay(ms);
#endif
}

static serial_manager_status_t SerialManager_Write(serial_write_handle_t writeHandle,
                                                   uint8_t *buffer,
                                                   uint32_t length,
                                                   serial_manager_transmission_mode_t mode)
{
    serial_manager_write_handle_t *serialWriteHandle;
    serial_manager_handle_t *handle;

#if (defined(OSA_USED) && defined(SERIAL_MANAGER_TASK_HANDLE_TX) && (SERIAL_MANAGER_TASK_HANDLE_TX == 1))
#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))
    /* Need to support common_task. */
#else  /* SERIAL_MANAGER_USE_COMMON_TASK */
    /* Do nothing. */
#endif /* SERIAL_MANAGER_USE_COMMON_TASK */
#else  /* OSA_USED && SERIAL_MANAGER_TASK_HANDLE_TX */
    serial_manager_status_t status = kStatus_SerialManager_Success;
#endif /* OSA_USED && SERIAL_MANAGER_TASK_HANDLE_TX */

    uint32_t primask;
    uint8_t isEmpty = 0U;

    assert(NULL != writeHandle);
    assert(NULL != buffer);
    assert(length > 0U);

    serialWriteHandle = (serial_manager_write_handle_t *)writeHandle;
    handle            = serialWriteHandle->serialManagerHandle;
    assert(NULL != handle);

    assert(SERIAL_MANAGER_WRITE_TAG == serialWriteHandle->tag);
    assert(!((kSerialManager_TransmissionNonBlocking == mode) && (NULL == serialWriteHandle->callback)));

    primask = DisableGlobalIRQ();
    if (NULL != serialWriteHandle->transfer.buffer)
    {
        EnableGlobalIRQ(primask);
        return kStatus_SerialManager_Busy;
    }
    serialWriteHandle->transfer.buffer = buffer;
    serialWriteHandle->transfer.length = length;
    serialWriteHandle->transfer.soFar  = 0U;
    serialWriteHandle->transfer.mode   = mode;

    if (NULL == LIST_GetHead(&handle->runningWriteHandleHead))
    {
        isEmpty = 1U;
    }
    SerialManager_AddTail(&handle->runningWriteHandleHead, serialWriteHandle);
    EnableGlobalIRQ(primask);

    if (0U != isEmpty)
    {
#if (defined(OSA_USED) && defined(SERIAL_MANAGER_TASK_HANDLE_TX) && (SERIAL_MANAGER_TASK_HANDLE_TX == 1))
#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))
        /* Need to support common_task. */
#else /* SERIAL_MANAGER_USE_COMMON_TASK */
        primask = DisableGlobalIRQ();
        handle->serialManagerState[SERIAL_EVENT_DATA_START_SEND]++;
        EnableGlobalIRQ(primask);
        (void)OSA_SemaphorePost((osa_semaphore_handle_t)handle->serSemaphore);

#endif /* SERIAL_MANAGER_USE_COMMON_TASK */
#else  /* OSA_USED && SERIAL_MANAGER_TASK_HANDLE_TX */
        status = SerialManager_StartWriting(handle);
        if ((serial_manager_status_t)kStatus_SerialManager_Success != status)
        {
#if (defined(USB_CDC_SERIAL_MANAGER_RUN_NO_HOST) && (USB_CDC_SERIAL_MANAGER_RUN_NO_HOST == 1))
            if (status == kStatus_SerialManager_NotConnected)
            {
                SerialManager_RemoveHead(&handle->runningWriteHandleHead);
                serialWriteHandle->transfer.buffer = 0U;
                serialWriteHandle->transfer.length = 0U;
            }
#endif /* USB_CDC_SERIAL_MANAGER_RUN_NO_HOST == 1 */
            return status;
        }
#endif /* OSA_USED && SERIAL_MANAGER_TASK_HANDLE_TX */
    }

    if (kSerialManager_TransmissionBlocking == mode)
    {
        while (serialWriteHandle->transfer.length > serialWriteHandle->transfer.soFar)
        {
            if (SerialManager_needPollingIsr())
            {
                SerialManager_IsrFunction(handle);
            }
            else
            {
                SerialManager_WriteTimeDelay(SERIAL_MANAGER_WRITE_TIME_DELAY_DEFAULT_VALUE);
            }
        }
    }
    return kStatus_SerialManager_Success;
}

static serial_manager_status_t SerialManager_Read(serial_read_handle_t readHandle,
                                                  uint8_t *buffer,
                                                  uint32_t length,
                                                  serial_manager_transmission_mode_t mode,
                                                  uint32_t *receivedLength)
{
    serial_manager_read_handle_t *serialReadHandle;
    serial_manager_handle_t *handle;
    uint32_t dataLength;
    uint32_t primask;

    assert(NULL != readHandle);
    assert(NULL != buffer);
    assert(length > 0U);

    serialReadHandle = (serial_manager_read_handle_t *)readHandle;

    handle = serialReadHandle->serialManagerHandle;
    assert(NULL != handle);

    assert(SERIAL_MANAGER_READ_TAG == serialReadHandle->tag);
    assert(!((kSerialManager_TransmissionNonBlocking == mode) && (NULL == serialReadHandle->callback)));

    primask = DisableGlobalIRQ();
    if (NULL != serialReadHandle->transfer.buffer)
    {
        EnableGlobalIRQ(primask);
        return kStatus_SerialManager_Busy;
    }
    serialReadHandle->transfer.buffer = buffer;
    serialReadHandle->transfer.length = length;
    serialReadHandle->transfer.soFar  = 0U;
    serialReadHandle->transfer.mode   = mode;

    /* This code is reached if (handle->handleType != kSerialManager_Blocking)*/
#if (!((defined(SERIAL_PORT_TYPE_USBCDC) && (SERIAL_PORT_TYPE_USBCDC > 0U))) && \
     !((defined(SERIAL_PORT_TYPE_VIRTUAL) && (SERIAL_PORT_TYPE_VIRTUAL > 0U))))
    if (length == 1U)
    {
        if (handle->ringBuffer.ringHead != handle->ringBuffer.ringTail)
        {
            buffer[serialReadHandle->transfer.soFar++] = handle->ringBuffer.ringBuffer[handle->ringBuffer.ringTail];
            handle->ringBuffer.ringTail++;
            if (handle->ringBuffer.ringTail >= handle->ringBuffer.ringBufferSize)
            {
                handle->ringBuffer.ringTail = 0U;
            }
        }
    }
    else
#endif /*(!defined(SERIAL_PORT_TYPE_USBCDC) && !defined(SERIAL_PORT_TYPE_VIRTUAL))*/
    {
        dataLength = handle->ringBuffer.ringHead + handle->ringBuffer.ringBufferSize - handle->ringBuffer.ringTail;
        dataLength = dataLength % handle->ringBuffer.ringBufferSize;

        for (serialReadHandle->transfer.soFar = 0U; serialReadHandle->transfer.soFar < MIN(dataLength, length);
             serialReadHandle->transfer.soFar++)
        {
            buffer[serialReadHandle->transfer.soFar] = handle->ringBuffer.ringBuffer[handle->ringBuffer.ringTail];
            handle->ringBuffer.ringTail++;
            if (handle->ringBuffer.ringTail >= handle->ringBuffer.ringBufferSize)
            {
                handle->ringBuffer.ringTail = 0U;
            }
        }

        dataLength = handle->ringBuffer.ringHead + handle->ringBuffer.ringBufferSize - handle->ringBuffer.ringTail;
        dataLength = dataLength % handle->ringBuffer.ringBufferSize;
        dataLength = handle->ringBuffer.ringBufferSize - 1U - dataLength;

        (void)SerialManager_StartReading(handle, readHandle, NULL, dataLength);
    }

    if (NULL != receivedLength)
    {
        *receivedLength                   = serialReadHandle->transfer.soFar;
        serialReadHandle->transfer.buffer = NULL;
        EnableGlobalIRQ(primask);
    }
    else
    {
        if (serialReadHandle->transfer.soFar >= serialReadHandle->transfer.length)
        {
            serialReadHandle->transfer.buffer = NULL;
            EnableGlobalIRQ(primask);
            if (kSerialManager_TransmissionNonBlocking == mode)
            {
                if (NULL != serialReadHandle->callback)
                {
                    serial_manager_callback_message_t msg;
                    msg.buffer = buffer;
                    msg.length = serialReadHandle->transfer.soFar;
                    serialReadHandle->callback(serialReadHandle->callbackParam, &msg, kStatus_SerialManager_Success);
                }
            }
        }
        else
        {
            EnableGlobalIRQ(primask);
        }

        if (kSerialManager_TransmissionBlocking == mode)
        {
            while (serialReadHandle->transfer.length > serialReadHandle->transfer.soFar)
            {
                SerialManager_ReadTimeDelay(SERIAL_MANAGER_READ_TIME_DELAY_DEFAULT_VALUE);
            }
        }
    }
#if (defined(SERIAL_MANAGER_RING_BUFFER_FLOWCONTROL) && (SERIAL_MANAGER_RING_BUFFER_FLOWCONTROL > 0U))
    uint32_t ringBufferWaterMark =
        handle->ringBuffer.ringHead + handle->ringBuffer.ringBufferSize - handle->ringBuffer.ringTail;
    ringBufferWaterMark = ringBufferWaterMark % handle->ringBuffer.ringBufferSize;
    if (ringBufferWaterMark < (uint32_t)(handle->ringBuffer.ringBufferSize * RINGBUFFER_WATERMARK_THRESHOLD))
    {
        (void)SerialManager_StartReading(handle, handle->openedReadHandleHead, NULL, serialReadHandle->transfer.length);
    }
#endif
    return kStatus_SerialManager_Success;
}

#else

static serial_manager_status_t SerialManager_Write(serial_write_handle_t writeHandle, uint8_t *buffer, uint32_t length)
{
    serial_manager_write_handle_t *serialWriteHandle;
    serial_manager_handle_t *handle;

    assert(writeHandle);
    assert(buffer);
    assert(length);

    serialWriteHandle = (serial_manager_write_handle_t *)writeHandle;
    handle            = serialWriteHandle->serialManagerHandle;

    assert(handle);

    return SerialManager_StartWriting(handle, serialWriteHandle, buffer, length);
}

static serial_manager_status_t SerialManager_Read(serial_read_handle_t readHandle, uint8_t *buffer, uint32_t length)
{
    serial_manager_read_handle_t *serialReadHandle;
    serial_manager_handle_t *handle;

    assert(readHandle);
    assert(buffer);
    assert(length);

    serialReadHandle = (serial_manager_read_handle_t *)readHandle;
    handle           = serialReadHandle->serialManagerHandle;

    assert(handle);

    return SerialManager_StartReading(handle, serialReadHandle, buffer, length);
}
#endif

serial_manager_status_t SerialManager_Init(serial_handle_t serialHandle, const serial_manager_config_t *config)
{
    serial_manager_handle_t *handle;
    serial_manager_status_t status = kStatus_SerialManager_Error;

    assert(NULL != config);

    assert(NULL != serialHandle);
    assert(SERIAL_MANAGER_HANDLE_SIZE >= sizeof(serial_manager_handle_t));

    handle = (serial_manager_handle_t *)serialHandle;
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))

    assert(NULL != config->ringBuffer);
    assert(config->ringBufferSize > 0U);
    (void)memset(handle, 0, SERIAL_MANAGER_HANDLE_SIZE);
    handle->handleType = config->blockType;
#else
    (void)memset(handle, 0, SERIAL_MANAGER_HANDLE_SIZE);
#endif
    handle->serialPortType = config->type;
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
    handle->ringBuffer.ringBuffer     = config->ringBuffer;
    handle->ringBuffer.ringBufferSize = config->ringBufferSize;
#endif

#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
#if defined(OSA_USED)

#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))

    COMMON_TASK_init();

#else
    if (KOSA_StatusSuccess != OSA_SemaphoreCreate((osa_semaphore_handle_t)handle->serSemaphore, 1U))
    {
        return kStatus_SerialManager_Error;
    }

    if (KOSA_StatusSuccess != OSA_TaskCreate((osa_task_handle_t)handle->taskId, OSA_TASK(SerialManager_Task), handle))
    {
        return kStatus_SerialManager_Error;
    }
#endif

#endif

#endif
    
    switch (config->type)
    {
#if (defined(SERIAL_PORT_TYPE_UART) && (SERIAL_PORT_TYPE_UART > 0U))
        case kSerialPort_Uart:
            status = Serial_UartInit(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), config->portConfig);
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
            if ((serial_manager_status_t)kStatus_SerialManager_Success == status)
            {
                (void)Serial_UartInstallTxCallback(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                                   SerialManager_TxCallback, handle);

                (void)Serial_UartInstallRxCallback(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                                   SerialManager_RxCallback, handle);
            }
#endif
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_UART_DMA) && (SERIAL_PORT_TYPE_UART_DMA > 0U))
        case kSerialPort_UartDma:
            status = Serial_UartDmaInit(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), config->portConfig);
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
            if ((serial_manager_status_t)kStatus_SerialManager_Success == status)
            {
                (void)Serial_UartDmaInstallTxCallback(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                                      SerialManager_TxCallback, handle);

                (void)Serial_UartDmaInstallRxCallback(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                                      SerialManager_RxCallback, handle);
            }
#endif
            break;
#endif

#if (defined(SERIAL_PORT_TYPE_USBCDC) && (SERIAL_PORT_TYPE_USBCDC > 0U))
        case kSerialPort_UsbCdc:
            status = Serial_UsbCdcInit(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), config->portConfig);
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))

            if (kStatus_SerialManager_Success == status)
            {
                status = Serial_UsbCdcInstallTxCallback(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                                        SerialManager_TxCallback, handle);
                if (kStatus_SerialManager_Success == status)
                {
                    status = Serial_UsbCdcInstallRxCallback(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                                            SerialManager_RxCallback, handle);
                }
            }
#endif
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_SWO) && (SERIAL_PORT_TYPE_SWO > 0U))
        case kSerialPort_Swo:
            status = Serial_SwoInit(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), config->portConfig);
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
            if (kStatus_SerialManager_Success == status)
            {
                status = Serial_SwoInstallTxCallback(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                                     SerialManager_TxCallback, handle);
            }
#endif
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_VIRTUAL) && (SERIAL_PORT_TYPE_VIRTUAL > 0U))
        case kSerialPort_Virtual:
            status = Serial_PortVirtualInit(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), config->portConfig);
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
            if (kStatus_SerialManager_Success == status)
            {
                status = Serial_PortVirtualInstallTxCallback(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                                             SerialManager_TxCallback, handle);
                if (kStatus_SerialManager_Success == status)
                {
                    status = Serial_PortVirtualInstallRxCallback(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                                                 SerialManager_RxCallback, handle);
                }
            }
#endif
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_RPMSG) && (SERIAL_PORT_TYPE_RPMSG > 0U))
        case kSerialPort_Rpmsg:
            status = Serial_RpmsgInit(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), (void *)config);
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
            if (kStatus_SerialManager_Success == status)
            {
                status = Serial_RpmsgInstallTxCallback(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                                       SerialManager_TxCallback, handle);
                if (kStatus_SerialManager_Success == status)
                {
                    status = Serial_RpmsgInstallRxCallback(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                                           SerialManager_RxCallback, handle);
                }
            }
#endif
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_SPI_MASTER) && (SERIAL_PORT_TYPE_SPI_MASTER > 0U))
        case kSerialPort_SpiMaster:
            status = Serial_SpiMasterInit(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), config->portConfig);
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
            if (kStatus_SerialManager_Success == status)
            {
                status = Serial_SpiMasterInstallTxCallback(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                                           SerialManager_TxCallback, handle);
                if (kStatus_SerialManager_Success == status)
                {
                    status = Serial_SpiMasterInstallRxCallback(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                                               SerialManager_RxCallback, handle);
                }
            }
#endif
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_SPI_SLAVE) && (SERIAL_PORT_TYPE_SPI_SLAVE > 0U))
        case kSerialPort_SpiSlave:
            status = Serial_SpiSlaveInit(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), config->portConfig);
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
            if (kStatus_SerialManager_Success == status)
            {
                status = Serial_SpiSlaveInstallTxCallback(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                                          SerialManager_TxCallback, handle);
                if (kStatus_SerialManager_Success == status)
                {
                    status = Serial_SpiSlaveInstallRxCallback(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                                              SerialManager_RxCallback, handle);
                }
            }
#endif
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_BLE_WU) && (SERIAL_PORT_TYPE_BLE_WU > 0U))
        case kSerialPort_BleWu:
            status = Serial_PortBleWuInit(((serial_handle_t)&handle->lowLevelhandleBuffer[0]), config->portConfig);
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
            if (kStatus_SerialManager_Success == status)
            {
                status = Serial_PortBleWuInstallTxCallback(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                                             SerialManager_TxCallback, handle);
                if (kStatus_SerialManager_Success == status)
                {
                    status = Serial_PortBleWuInstallRxCallback(((serial_handle_t)&handle->lowLevelhandleBuffer[0]),
                                                                 SerialManager_RxCallback, handle);
                }
            }
#endif
            break;
#endif 
        default:
            /*MISRA rule 16.4*/
            break;
    }

    return status;
}

serial_manager_status_t SerialManager_Deinit(serial_handle_t serialHandle)
{
    serial_manager_handle_t *handle;

    serial_manager_status_t serialManagerStatus = kStatus_SerialManager_Success;

    assert(NULL != serialHandle);

    handle = (serial_manager_handle_t *)serialHandle;

    if ((NULL != handle->openedReadHandleHead) || (0U != handle->openedWriteHandleCount))
    {
        serialManagerStatus = kStatus_SerialManager_Busy; /*Serial Manager Busy*/
    }
    else
    {
        switch (handle->serialPortType) /*serial port type*/
        {
#if (defined(SERIAL_PORT_TYPE_UART) && (SERIAL_PORT_TYPE_UART > 0U))
            case kSerialPort_Uart:
                (void)Serial_UartDeinit(((serial_handle_t)&handle->lowLevelhandleBuffer[0]));
                break;
#endif
#if (defined(SERIAL_PORT_TYPE_USBCDC) && (SERIAL_PORT_TYPE_USBCDC > 0U))
            case kSerialPort_UsbCdc:
                (void)Serial_UsbCdcDeinit(((serial_handle_t)&handle->lowLevelhandleBuffer[0]));
                break;
#endif
#if (defined(SERIAL_PORT_TYPE_SWO) && (SERIAL_PORT_TYPE_SWO > 0U))
            case kSerialPort_Swo:
                (void)Serial_SwoDeinit(((serial_handle_t)&handle->lowLevelhandleBuffer[0]));
                break;
#endif
#if (defined(SERIAL_PORT_TYPE_VIRTUAL) && (SERIAL_PORT_TYPE_VIRTUAL > 0U))
            case kSerialPort_Virtual:
                (void)Serial_PortVirtualDeinit(((serial_handle_t)&handle->lowLevelhandleBuffer[0]));
                break;
#endif
#if (defined(SERIAL_PORT_TYPE_RPMSG) && (SERIAL_PORT_TYPE_RPMSG > 0U))
            case kSerialPort_Rpmsg:
                (void)Serial_RpmsgDeinit(((serial_handle_t)&handle->lowLevelhandleBuffer[0]));
                break;
#endif
#if (defined(SERIAL_PORT_TYPE_SPI_SLAVE) && (SERIAL_PORT_TYPE_SPI_SLAVE > 0U))
            case kSerialPort_SpiSlave:
                (void)Serial_SpiSlaveDeinit(((serial_handle_t)&handle->lowLevelhandleBuffer[0]));
                break;
#endif
#if (defined(SERIAL_PORT_TYPE_SPI_MASTER) && (SERIAL_PORT_TYPE_SPI_MASTER > 0U))
            case kSerialPort_SpiMaster:
                (void)Serial_SpiMasterDeinit(((serial_handle_t)&handle->lowLevelhandleBuffer[0]));
                break;
#endif
#if (defined(SERIAL_PORT_TYPE_BLE_WU) && (SERIAL_PORT_TYPE_BLE_WU > 0U))
            case kSerialPort_BleWu:
                (void)Serial_PortBleWuDeinit(((serial_handle_t)&handle->lowLevelhandleBuffer[0]));
                break;
#endif
            default:
                /*MISRA rule 16.4*/
                break;
        }
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))

#if defined(OSA_USED)

#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))
#else
        (void)OSA_SemaphoreDestroy((osa_event_handle_t)handle->serSemaphore);
        (void)OSA_TaskDestroy((osa_task_handle_t)handle->taskId);
#endif

#endif

#endif
    }
    return serialManagerStatus;
}

serial_manager_status_t SerialManager_OpenWriteHandle(serial_handle_t serialHandle, serial_write_handle_t writeHandle)
{
    serial_manager_handle_t *handle;
    serial_manager_write_handle_t *serialWriteHandle;
    uint32_t primask;

    assert(NULL != serialHandle);
    assert(NULL != writeHandle);
    assert(SERIAL_MANAGER_WRITE_HANDLE_SIZE >= sizeof(serial_manager_write_handle_t));

    handle            = (serial_manager_handle_t *)serialHandle;
    serialWriteHandle = (serial_manager_write_handle_t *)writeHandle;

    primask = DisableGlobalIRQ();
    handle->openedWriteHandleCount++;
    EnableGlobalIRQ(primask);
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
    if (handle->handleType == kSerialManager_Blocking)
    {
        serialWriteHandle->serialManagerHandle = handle;
        return kStatus_SerialManager_Success;
    }
    else
#endif
    {
        (void)memset(writeHandle, 0, SERIAL_MANAGER_WRITE_HANDLE_SIZE);
    }

    serialWriteHandle->serialManagerHandle = handle;

#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
    serialWriteHandle->tag = SERIAL_MANAGER_WRITE_TAG;
#endif

    return kStatus_SerialManager_Success;
}

serial_manager_status_t SerialManager_CloseWriteHandle(serial_write_handle_t writeHandle)
{
    serial_manager_handle_t *handle;
    serial_manager_write_handle_t *serialWriteHandle;
    uint32_t primask;

    assert(NULL != writeHandle);

    serialWriteHandle = (serial_manager_write_handle_t *)writeHandle;
    handle            = (serial_manager_handle_t *)(void *)serialWriteHandle->serialManagerHandle;

    assert(NULL != handle);
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
    assert(SERIAL_MANAGER_WRITE_TAG == serialWriteHandle->tag);
#endif

#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
    (void)SerialManager_CancelWriting(writeHandle);
#endif
    primask = DisableGlobalIRQ();
    if (handle->openedWriteHandleCount > 0U)
    {
        handle->openedWriteHandleCount--;
    }
    EnableGlobalIRQ(primask);

    (void)memset(writeHandle, 0, SERIAL_MANAGER_WRITE_HANDLE_SIZE);

    return kStatus_SerialManager_Success;
}

serial_manager_status_t SerialManager_OpenReadHandle(serial_handle_t serialHandle, serial_read_handle_t readHandle)
{
    serial_manager_handle_t *handle;
    serial_manager_read_handle_t *serialReadHandle; /* read handle structure */
    serial_manager_status_t serialManagerStatus = kStatus_SerialManager_Success;
    uint32_t primask;

    assert(NULL != serialHandle);
    assert(NULL != readHandle);
    assert(SERIAL_MANAGER_READ_HANDLE_SIZE >= sizeof(serial_manager_read_handle_t));

    handle           = (serial_manager_handle_t *)serialHandle;
    serialReadHandle = (serial_manager_read_handle_t *)readHandle;
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
    if (handle->handleType == kSerialManager_Blocking)
    {
        serialReadHandle->serialManagerHandle = handle;
        return kStatus_SerialManager_Success;
    }
#endif
    primask = DisableGlobalIRQ();
    if (handle->openedReadHandleHead != NULL)
    {
        serialManagerStatus = kStatus_SerialManager_Busy;
    }
    else
    {
        handle->openedReadHandleHead = serialReadHandle;

        (void)memset(readHandle, 0, SERIAL_MANAGER_READ_HANDLE_SIZE);

        serialReadHandle->serialManagerHandle = handle;
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
        serialReadHandle->tag = SERIAL_MANAGER_READ_TAG;
#endif
    }
    EnableGlobalIRQ(primask);
    return serialManagerStatus;
}

serial_manager_status_t SerialManager_CloseReadHandle(serial_read_handle_t readHandle)
{
    serial_manager_handle_t *handle;
    serial_manager_read_handle_t *serialReadHandle;
    uint32_t primask;

    assert(NULL != readHandle);

    serialReadHandle = (serial_manager_read_handle_t *)readHandle;
    handle           = (serial_manager_handle_t *)(void *)serialReadHandle->serialManagerHandle;

    assert((NULL != handle) && (handle->openedReadHandleHead == serialReadHandle));
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
    assert(SERIAL_MANAGER_READ_TAG == serialReadHandle->tag);
#endif

#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
    (void)SerialManager_CancelReading(readHandle);
#endif

    primask                      = DisableGlobalIRQ();
    handle->openedReadHandleHead = NULL;
    EnableGlobalIRQ(primask);

    (void)memset(readHandle, 0, SERIAL_MANAGER_READ_HANDLE_SIZE);

    return kStatus_SerialManager_Success;
}

serial_manager_status_t SerialManager_WriteBlocking(serial_write_handle_t writeHandle, uint8_t *buffer, uint32_t length)
{
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
    return SerialManager_Write(writeHandle, buffer, length, kSerialManager_TransmissionBlocking);
#else
    return SerialManager_Write(writeHandle, buffer, length);
#endif
}

serial_manager_status_t SerialManager_ReadBlocking(serial_read_handle_t readHandle, uint8_t *buffer, uint32_t length)
{
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
    return SerialManager_Read(readHandle, buffer, length, kSerialManager_TransmissionBlocking, NULL);
#else
    return SerialManager_Read(readHandle, buffer, length);
#endif
}

#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
serial_manager_status_t SerialManager_WriteNonBlocking(serial_write_handle_t writeHandle,
                                                       uint8_t *buffer,
                                                       uint32_t length)
{
    return SerialManager_Write(writeHandle, buffer, length, kSerialManager_TransmissionNonBlocking);
}

serial_manager_status_t SerialManager_ReadNonBlocking(serial_read_handle_t readHandle, uint8_t *buffer, uint32_t length)
{
#if ((defined(SERIAL_PORT_TYPE_SPI_MASTER) && (SERIAL_PORT_TYPE_SPI_MASTER > 0U)) || \
     (defined(SERIAL_PORT_TYPE_SPI_SLAVE) && (SERIAL_PORT_TYPE_SPI_SLAVE > 0U)))

    serial_manager_read_handle_t *serialReadHandle;
    serialReadHandle = (serial_manager_read_handle_t *)readHandle;

    return (serial_manager_status_t)SerialManager_StartReading(serialReadHandle->serialManagerHandle, readHandle,
                                                               buffer, length);
#else
    return SerialManager_Read(readHandle, buffer, length, kSerialManager_TransmissionNonBlocking, NULL);
#endif
}

serial_manager_status_t SerialManager_CancelWriting(serial_write_handle_t writeHandle)
{
    serial_manager_write_handle_t *serialWriteHandle;
    uint32_t primask;
    uint8_t isNotUsed        = 0U;
    uint8_t isNotNeed2Cancel = 0U;

    assert(NULL != writeHandle);

    serialWriteHandle = (serial_manager_write_handle_t *)writeHandle;

    assert(NULL != serialWriteHandle->serialManagerHandle);
    assert(SERIAL_MANAGER_WRITE_TAG == serialWriteHandle->tag);

    if ((NULL != serialWriteHandle->transfer.buffer) &&
        (kSerialManager_TransmissionBlocking == serialWriteHandle->transfer.mode))
    {
        return kStatus_SerialManager_Error;
    }

    primask = DisableGlobalIRQ();
    if (serialWriteHandle != (serial_manager_write_handle_t *)(void *)LIST_GetHead(
                                 &serialWriteHandle->serialManagerHandle->runningWriteHandleHead))
    {
        if (kLIST_Ok == LIST_RemoveElement(&serialWriteHandle->link))
        {
            isNotUsed = 1U;
        }
        else
        {
            isNotNeed2Cancel = 1U;
        }
    }
    EnableGlobalIRQ(primask);

    if (0U == isNotNeed2Cancel)
    {
        if (0U != isNotUsed)
        {
            serialWriteHandle->transfer.soFar  = 0;
            serialWriteHandle->transfer.status = kStatus_SerialManager_Canceled;

            SerialManager_AddTail(&serialWriteHandle->serialManagerHandle->completedWriteHandleHead, serialWriteHandle);
#if defined(OSA_USED)

#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))
            serialWriteHandle->serialManagerHandle->commontaskMsg.callback = SerialManager_Task;
            serialWriteHandle->serialManagerHandle->commontaskMsg.callbackParam =
                serialWriteHandle->serialManagerHandle;
            COMMON_TASK_post_message(&serialWriteHandle->serialManagerHandle->commontaskMsg);
#else
            primask = DisableGlobalIRQ();
            serialWriteHandle->serialManagerHandle->serialManagerState[SERIAL_EVENT_DATA_SENT]++;
            EnableGlobalIRQ(primask);
            (void)OSA_SemaphorePost((osa_semaphore_handle_t)serialWriteHandle->serialManagerHandle->serSemaphore);
#endif

#else
            SerialManager_Task(serialWriteHandle->serialManagerHandle);
#endif
        }
        else
        {
            switch (serialWriteHandle->serialManagerHandle->serialPortType)
            {
#if (defined(SERIAL_PORT_TYPE_UART) && (SERIAL_PORT_TYPE_UART > 0U))
                case kSerialPort_Uart:
                    (void)Serial_UartCancelWrite(
                        ((serial_handle_t)&serialWriteHandle->serialManagerHandle->lowLevelhandleBuffer[0]));
                    break;
#endif
#if (defined(SERIAL_PORT_TYPE_USBCDC) && (SERIAL_PORT_TYPE_USBCDC > 0U))
                case kSerialPort_UsbCdc:
                    (void)Serial_UsbCdcCancelWrite(
                        ((serial_handle_t)&serialWriteHandle->serialManagerHandle->lowLevelhandleBuffer[0]));
                    break;
#endif
#if (defined(SERIAL_PORT_TYPE_SWO) && (SERIAL_PORT_TYPE_SWO > 0U))
                case kSerialPort_Swo:
                    (void)Serial_SwoCancelWrite(
                        ((serial_handle_t)&serialWriteHandle->serialManagerHandle->lowLevelhandleBuffer[0]));
                    break;
#endif
#if (defined(SERIAL_PORT_TYPE_VIRTUAL) && (SERIAL_PORT_TYPE_VIRTUAL > 0U))
                case kSerialPort_Virtual:
                    (void)Serial_PortVirtualCancelWrite(
                        ((serial_handle_t)&serialWriteHandle->serialManagerHandle->lowLevelhandleBuffer[0]));
                    break;
#endif
#if (defined(SERIAL_PORT_TYPE_SPI_MASTER) && (SERIAL_PORT_TYPE_SPI_MASTER > 0U))
                case kSerialPort_SpiMaster:
                    (void)Serial_SpiMasterCancelWrite(
                        ((serial_handle_t)&serialWriteHandle->serialManagerHandle->lowLevelhandleBuffer[0]));
                    break;
#endif
#if (defined(SERIAL_PORT_TYPE_SPI_SLAVE) && (SERIAL_PORT_TYPE_SPI_SLAVE > 0U))
                case kSerialPort_SpiSlave:
                    (void)Serial_SpiSlaveCancelWrite(
                        ((serial_handle_t)&serialWriteHandle->serialManagerHandle->lowLevelhandleBuffer[0]));
                    break;
#endif
#if (defined(SERIAL_PORT_TYPE_BLE_WU) && (SERIAL_PORT_TYPE_BLE_WU > 0U))
                case kSerialPort_BleWu:
                    (void)Serial_PortBleWuCancelWrite(
                        ((serial_handle_t)&serialWriteHandle->serialManagerHandle->lowLevelhandleBuffer[0]));
                    break;
#endif
                default:
                    /*MISRA rule 16.4*/
                    break;
            }
        }

#if (defined(OSA_USED) && defined(SERIAL_MANAGER_TASK_HANDLE_TX) && (SERIAL_MANAGER_TASK_HANDLE_TX == 1))
#if (defined(SERIAL_MANAGER_USE_COMMON_TASK) && (SERIAL_MANAGER_USE_COMMON_TASK > 0U))
        /* Need to support common_task. */
#else /* SERIAL_MANAGER_USE_COMMON_TASK */
        primask = DisableGlobalIRQ();
        serialWriteHandle->serialManagerHandle->serialManagerState[SERIAL_EVENT_DATA_START_SEND]++;
        EnableGlobalIRQ(primask);
        (void)OSA_SemaphorePost((osa_semaphore_handle_t)serialWriteHandle->serialManagerHandle->serSemaphore);

#endif /* SERIAL_MANAGER_USE_COMMON_TASK */
#else  /* OSA_USED && SERIAL_MANAGER_TASK_HANDLE_TX */
        (void)SerialManager_StartWriting(serialWriteHandle->serialManagerHandle);
#endif /* OSA_USED && SERIAL_MANAGER_TASK_HANDLE_TX */
    }

    return kStatus_SerialManager_Success;
}

serial_manager_status_t SerialManager_CancelReading(serial_read_handle_t readHandle)
{
    serial_manager_read_handle_t *serialReadHandle;
    serial_manager_callback_message_t msg;
    uint8_t *buffer;
    uint32_t primask;

    assert(NULL != readHandle);

    serialReadHandle = (serial_manager_read_handle_t *)readHandle;

    assert(SERIAL_MANAGER_READ_TAG == serialReadHandle->tag);

    if ((NULL != serialReadHandle->transfer.buffer) &&
        (kSerialManager_TransmissionBlocking == serialReadHandle->transfer.mode))
    {
        return kStatus_SerialManager_Error;
    }

    primask                           = DisableGlobalIRQ();
    buffer                            = serialReadHandle->transfer.buffer;
    serialReadHandle->transfer.buffer = NULL;
    serialReadHandle->transfer.length = 0;
    msg.buffer                        = buffer;
    msg.length                        = serialReadHandle->transfer.soFar;
    EnableGlobalIRQ(primask);

    if (NULL != buffer)
    {
        if (NULL != serialReadHandle->callback)
        {
            serialReadHandle->callback(serialReadHandle->callbackParam, &msg, kStatus_SerialManager_Canceled);
        }
    }
    return kStatus_SerialManager_Success;
}

serial_manager_status_t SerialManager_TryRead(serial_read_handle_t readHandle,
                                              uint8_t *buffer,
                                              uint32_t length,
                                              uint32_t *receivedLength)
{
    assert(NULL != receivedLength);

    return SerialManager_Read(readHandle, buffer, length, kSerialManager_TransmissionBlocking, receivedLength);
}

serial_manager_status_t SerialManager_InstallTxCallback(serial_write_handle_t writeHandle,
                                                        serial_manager_callback_t callback,
                                                        void *callbackParam)
{
    serial_manager_write_handle_t *serialWriteHandle;

    assert(NULL != writeHandle);

    serialWriteHandle = (serial_manager_write_handle_t *)writeHandle;

    assert(SERIAL_MANAGER_WRITE_TAG == serialWriteHandle->tag);

    serialWriteHandle->callbackParam = callbackParam;
    serialWriteHandle->callback      = callback;

    return kStatus_SerialManager_Success;
}

serial_manager_status_t SerialManager_InstallRxCallback(serial_read_handle_t readHandle,
                                                        serial_manager_callback_t callback,
                                                        void *callbackParam)
{
    serial_manager_read_handle_t *serialReadHandle;

    assert(NULL != readHandle);

    serialReadHandle = (serial_manager_read_handle_t *)readHandle;

    assert(SERIAL_MANAGER_READ_TAG == serialReadHandle->tag);

    serialReadHandle->callbackParam = callbackParam;
    serialReadHandle->callback      = callback;

    return kStatus_SerialManager_Success;
}
#endif

serial_manager_status_t SerialManager_EnterLowpower(serial_handle_t serialHandle)
{
    serial_manager_handle_t *handle;
    serial_manager_status_t status = kStatus_SerialManager_Error;

    assert(NULL != serialHandle);

    handle = (serial_manager_handle_t *)serialHandle;

    switch (handle->serialPortType)
    {
#if (defined(SERIAL_PORT_TYPE_UART) && (SERIAL_PORT_TYPE_UART > 0U))
        case kSerialPort_Uart:
            status = Serial_UartEnterLowpower(((serial_handle_t)&handle->lowLevelhandleBuffer[0]));
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_UART_DMA) && (SERIAL_PORT_TYPE_UART_DMA > 0U))
        case kSerialPort_UartDma:
            status = Serial_UartDmaEnterLowpower(((serial_handle_t)&handle->lowLevelhandleBuffer[0]));
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_USBCDC) && (SERIAL_PORT_TYPE_USBCDC > 0U))
        case kSerialPort_UsbCdc:
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_SWO) && (SERIAL_PORT_TYPE_SWO > 0U))
        case kSerialPort_Swo:
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_VIRTUAL) && (SERIAL_PORT_TYPE_VIRTUAL > 0U))
        case kSerialPort_Virtual:
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_VIRTUAL) && (SERIAL_PORT_TYPE_VIRTUAL > 0U))
        case kSerialPort_Rpmsg:
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_SPI_MASTER) && (SERIAL_PORT_TYPE_SPI_MASTER > 0U))
        case kSerialPort_SpiMaster:
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_SPI_SLAVE) && (SERIAL_PORT_TYPE_SPI_SLAVE > 0U))
        case kSerialPort_SpiSlave:
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_BLE_WU) && (SERIAL_PORT_TYPE_BLE_WU > 0U))
        case kSerialPort_BleWu:
            break;
#endif
        default:
            /*MISRA rule 16.4*/
            break;
    }
    return status;
}

serial_manager_status_t SerialManager_ExitLowpower(serial_handle_t serialHandle)
{
    serial_manager_handle_t *handle;
    serial_manager_status_t status = kStatus_SerialManager_Error;

    assert(NULL != serialHandle);

    handle = (serial_manager_handle_t *)serialHandle;

    switch (handle->serialPortType)
    {
#if (defined(SERIAL_PORT_TYPE_UART) && (SERIAL_PORT_TYPE_UART > 0U))
        case kSerialPort_Uart:
            status = Serial_UartExitLowpower(((serial_handle_t)&handle->lowLevelhandleBuffer[0]));
            break;
#endif

#if (defined(SERIAL_PORT_TYPE_UART_DMA) && (SERIAL_PORT_TYPE_UART_DMA > 0U))
        case kSerialPort_UartDma:
            status = Serial_UartDmaExitLowpower(((serial_handle_t)&handle->lowLevelhandleBuffer[0]));
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_USBCDC) && (SERIAL_PORT_TYPE_USBCDC > 0U))
        case kSerialPort_UsbCdc:
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_SWO) && (SERIAL_PORT_TYPE_SWO > 0U))
        case kSerialPort_Swo:
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_VIRTUAL) && (SERIAL_PORT_TYPE_VIRTUAL > 0U))
        case kSerialPort_Virtual:
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_VIRTUAL) && (SERIAL_PORT_TYPE_VIRTUAL > 0U))
        case kSerialPort_Rpmsg:
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_SPI_MASTER) && (SERIAL_PORT_TYPE_SPI_MASTER > 0U))
        case kSerialPort_SpiMaster:
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_SPI_SLAVE) && (SERIAL_PORT_TYPE_SPI_SLAVE > 0U))
        case kSerialPort_SpiSlave:
            break;
#endif
#if (defined(SERIAL_PORT_TYPE_BLE_WU) && (SERIAL_PORT_TYPE_BLE_WU > 0U))
        case kSerialPort_BleWu:
            break;
#endif
        default:
            /*MISRA rule 16.4*/
            break;
    }
    return status;
}
/*!
 * @brief This function performs initialization of the callbacks structure used to disable lowpower
 *          when serial manager is active.
 *
 *
 * @param  pfCallback Pointer to the function structure used to allow/disable lowpower.
 *
 */
void SerialManager_SetLowpowerCriticalCb(const serial_manager_lowpower_critical_CBs_t *pfCallback)
{
    s_pfserialLowpowerCriticalCallbacks = pfCallback;
    (void)s_pfserialLowpowerCriticalCallbacks;
}
