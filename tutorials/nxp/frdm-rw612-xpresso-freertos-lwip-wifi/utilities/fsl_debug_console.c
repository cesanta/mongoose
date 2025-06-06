/*
 * This is a modified version of the file printf.c, which was distributed
 * by Motorola as part of the M5407C3BOOT.zip package used to initialize
 * the M5407C3 evaluation board.
 *
 * Copyright:
 *      1999-2000 MOTOROLA, INC. All Rights Reserved.
 *  You are hereby granted a copyright license to use, modify, and
 *  distribute the SOFTWARE so long as this entire notice is
 *  retained without alteration in any modified and/or redistributed
 *  versions, and that such modified versions are clearly identified
 *  as such. No licenses are granted by implication, estoppel or
 *  otherwise under any patents or trademarks of Motorola, Inc. This
 *  software is provided on an "AS IS" basis and without warranty.
 *
 *  To the maximum extent permitted by applicable law, MOTOROLA
 *  DISCLAIMS ALL WARRANTIES WHETHER EXPRESS OR IMPLIED, INCLUDING
 *  IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR
 *  PURPOSE AND ANY WARRANTY AGAINST INFRINGEMENT WITH REGARD TO THE
 *  SOFTWARE (INCLUDING ANY MODIFIED VERSIONS THEREOF) AND ANY
 *  ACCOMPANYING WRITTEN MATERIALS.
 *
 *  To the maximum extent permitted by applicable law, IN NO EVENT
 *  SHALL MOTOROLA BE LIABLE FOR ANY DAMAGES WHATSOEVER (INCLUDING
 *  WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS
 *  INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY
 *  LOSS) ARISING OF THE USE OR INABILITY TO USE THE SOFTWARE.
 *
 *  Motorola assumes no responsibility for the maintenance and support
 *  of this software

 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020, 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdarg.h>
#include <stdlib.h>
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
#include <stdio.h>
#endif

#include "fsl_debug_console_conf.h"
#include "fsl_str.h"

#include "fsl_common.h"
#include "fsl_component_serial_manager.h"

#include "fsl_debug_console.h"

#ifdef SDK_OS_FREE_RTOS
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
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

#if (defined(SDK_DEBUGCONSOLE) && (SDK_DEBUGCONSOLE == DEBUGCONSOLE_REDIRECT_TO_SDK))
#define DEBUG_CONSOLE_FUNCTION_PREFIX
#else
#define DEBUG_CONSOLE_FUNCTION_PREFIX static
#endif

/*! @brief character backspace ASCII value */
#define DEBUG_CONSOLE_BACKSPACE 127U

/* lock definition */
#if (DEBUG_CONSOLE_SYNCHRONIZATION_MODE == DEBUG_CONSOLE_SYNCHRONIZATION_FREERTOS)

static SemaphoreHandle_t s_debugConsoleReadSemaphore;
#if configSUPPORT_STATIC_ALLOCATION
static StaticSemaphore_t s_debugConsoleReadSemaphoreStatic;
#endif
#if (defined(DEBUG_CONSOLE_RX_ENABLE) && (DEBUG_CONSOLE_RX_ENABLE > 0U))
static SemaphoreHandle_t s_debugConsoleReadWaitSemaphore;
#if configSUPPORT_STATIC_ALLOCATION
static StaticSemaphore_t s_debugConsoleReadWaitSemaphoreStatic;
#endif
#endif

#elif (DEBUG_CONSOLE_SYNCHRONIZATION_MODE == DEBUG_CONSOLE_SYNCHRONIZATION_BM)

#if (defined(DEBUG_CONSOLE_RX_ENABLE) && (DEBUG_CONSOLE_RX_ENABLE > 0U))
static volatile bool s_debugConsoleReadWaitSemaphore;
#endif

#else

#endif /* DEBUG_CONSOLE_SYNCHRONIZATION_MODE == DEBUG_CONSOLE_SYNCHRONIZATION_FREERTOS */

/*! @brief get current runing environment is ISR or not */
#ifdef __CA7_REV
#define IS_RUNNING_IN_ISR() SystemGetIRQNestingLevel()
#else
#define IS_RUNNING_IN_ISR() __get_IPSR()
#endif /* __CA7_REV */

/* semaphore definition */
#if (DEBUG_CONSOLE_SYNCHRONIZATION_MODE == DEBUG_CONSOLE_SYNCHRONIZATION_FREERTOS)

/* mutex semaphore */
/* clang-format off */
#if configSUPPORT_STATIC_ALLOCATION
#define DEBUG_CONSOLE_CREATE_MUTEX_SEMAPHORE(mutex, stack) ((mutex) = xSemaphoreCreateMutexStatic(stack))
#else
#define DEBUG_CONSOLE_CREATE_MUTEX_SEMAPHORE(mutex) ((mutex) = xSemaphoreCreateMutex())
#endif
#define DEBUG_CONSOLE_DESTROY_MUTEX_SEMAPHORE(mutex)   \
        do                                             \
        {                                              \
            if(NULL != (mutex))                        \
            {                                          \
                vSemaphoreDelete(mutex);               \
                (mutex) = NULL;                          \
            }                                          \
        } while(false)

#define DEBUG_CONSOLE_GIVE_MUTEX_SEMAPHORE(mutex) \
{                                                 \
        if (IS_RUNNING_IN_ISR() == 0U)            \
        {                                         \
            (void)xSemaphoreGive(mutex);          \
        }                                         \
}

#define DEBUG_CONSOLE_TAKE_MUTEX_SEMAPHORE_BLOCKING(mutex) \
{                                                          \
        if (IS_RUNNING_IN_ISR() == 0U)                     \
        {                                                  \
            (void)xSemaphoreTake(mutex, portMAX_DELAY);    \
        }                                                  \
}

#define DEBUG_CONSOLE_TAKE_MUTEX_SEMAPHORE_NONBLOCKING(mutex, result) \
{                                                                     \
        if (IS_RUNNING_IN_ISR() == 0U)                                \
        {                                                             \
            result = xSemaphoreTake(mutex, 0U);                       \
        }                                                             \
        else                                                          \
        {                                                             \
            result = 1U;                                              \
        }                                                             \
}

/* Binary semaphore */
#if configSUPPORT_STATIC_ALLOCATION
#define DEBUG_CONSOLE_CREATE_BINARY_SEMAPHORE(binary,stack) ((binary) = xSemaphoreCreateBinaryStatic(stack))
#else
#define DEBUG_CONSOLE_CREATE_BINARY_SEMAPHORE(binary) ((binary) = xSemaphoreCreateBinary())
#endif
#define DEBUG_CONSOLE_DESTROY_BINARY_SEMAPHORE(binary) \
        do                                             \
        {                                              \
            if(NULL != (binary))                       \
            {                                          \
                vSemaphoreDelete((binary));              \
                (binary) = NULL;                         \
            }                                          \
        } while(false)
#define DEBUG_CONSOLE_TAKE_BINARY_SEMAPHORE_BLOCKING(binary) ((void)xSemaphoreTake((binary), portMAX_DELAY))
#define DEBUG_CONSOLE_GIVE_BINARY_SEMAPHORE_FROM_ISR(binary) ((void)xSemaphoreGiveFromISR((binary), NULL))

#elif (DEBUG_CONSOLE_SYNCHRONIZATION_BM == DEBUG_CONSOLE_SYNCHRONIZATION_MODE)

#define DEBUG_CONSOLE_CREATE_MUTEX_SEMAPHORE(mutex)         (void)(mutex)
#define DEBUG_CONSOLE_DESTROY_MUTEX_SEMAPHORE(mutex)        (void)(mutex)
#define DEBUG_CONSOLE_TAKE_MUTEX_SEMAPHORE_BLOCKING(mutex)  (void)(mutex)
#define DEBUG_CONSOLE_GIVE_MUTEX_SEMAPHORE(mutex)           (void)(mutex)
#define DEBUG_CONSOLE_TAKE_MUTEX_SEMAPHORE_NONBLOCKING(mutex, result) (result = 1U)

#define DEBUG_CONSOLE_CREATE_BINARY_SEMAPHORE(binary)       (void)(binary)
#define DEBUG_CONSOLE_DESTROY_BINARY_SEMAPHORE(binary)      (void)(binary)
#ifdef DEBUG_CONSOLE_TRANSFER_NON_BLOCKING
#define DEBUG_CONSOLE_TAKE_BINARY_SEMAPHORE_BLOCKING(binary) \
    {                                                        \
        while (!(binary))                                    \
        {                                                    \
        }                                                    \
        (binary) = false;                                      \
    }
#define DEBUG_CONSOLE_GIVE_BINARY_SEMAPHORE_FROM_ISR(binary) \
    do                                                       \
    {                                                        \
        (binary) = true;                                       \
    } while(false)
#else
#define DEBUG_CONSOLE_TAKE_BINARY_SEMAPHORE_BLOCKING(binary)  (void)(binary)
#define DEBUG_CONSOLE_GIVE_BINARY_SEMAPHORE_FROM_ISR(binary)  (void)(binary)
#endif /* DEBUG_CONSOLE_TRANSFER_NON_BLOCKING */
/* clang-format on */

/* add other implementation here
 *such as :
 * #elif(DEBUG_CONSOLE_SYNCHRONIZATION_MODE == DDEBUG_CONSOLE_SYNCHRONIZATION_xxx)
 */

#else

#error RTOS type is not defined by DEBUG_CONSOLE_SYNCHRONIZATION_MODE.

#endif /* DEBUG_CONSOLE_SYNCHRONIZATION_MODE == DEBUG_CONSOLE_SYNCHRONIZATION_FREERTOS */

#ifdef DEBUG_CONSOLE_TRANSFER_NON_BLOCKING
/* receive state structure */
typedef struct _debug_console_write_ring_buffer
{
    uint32_t ringBufferSize;
    volatile uint32_t ringHead;
    volatile uint32_t ringTail;
    uint8_t ringBuffer[DEBUG_CONSOLE_TRANSMIT_BUFFER_LEN];
} debug_console_write_ring_buffer_t;
#endif

typedef struct _debug_console_state_struct
{
    serial_handle_t serialHandle; /*!< serial manager handle */
#ifdef DEBUG_CONSOLE_TRANSFER_NON_BLOCKING
    SERIAL_MANAGER_HANDLE_DEFINE(serialHandleBuffer);
    debug_console_write_ring_buffer_t writeRingBuffer;
    uint8_t readRingBuffer[DEBUG_CONSOLE_RECEIVE_BUFFER_LEN];
    SERIAL_MANAGER_WRITE_HANDLE_DEFINE(serialWriteHandleBuffer);
    SERIAL_MANAGER_WRITE_HANDLE_DEFINE(serialWriteHandleBuffer2);
    SERIAL_MANAGER_READ_HANDLE_DEFINE(serialReadHandleBuffer);
#else
    SERIAL_MANAGER_BLOCK_HANDLE_DEFINE(serialHandleBuffer);
    SERIAL_MANAGER_WRITE_BLOCK_HANDLE_DEFINE(serialWriteHandleBuffer);
    SERIAL_MANAGER_READ_BLOCK_HANDLE_DEFINE(serialReadHandleBuffer);
#endif
} debug_console_state_struct_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Debug console state information. */
#if (defined(DATA_SECTION_IS_CACHEABLE) && (DATA_SECTION_IS_CACHEABLE > 0))
AT_NONCACHEABLE_SECTION(static debug_console_state_struct_t s_debugConsoleState);
#else
static debug_console_state_struct_t s_debugConsoleState;
#endif
serial_handle_t g_serialHandle; /*!< serial manager handle */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief This is a printf call back function which is used to relocate the log to buffer
 * or print the log immediately when the local buffer is full.
 *
 * @param[in] buf   Buffer to store log.
 * @param[in] indicator Buffer index.
 * @param[in] val Target character to store.
 * @param[in] len length of the character
 *
 */
#if (defined(SDK_DEBUGCONSOLE) && (SDK_DEBUGCONSOLE == DEBUGCONSOLE_REDIRECT_TO_SDK))
static void DbgConsole_PrintCallback(char *buf, int32_t *indicator, char dbgVal, int len);
#endif

status_t DbgConsole_ReadOneCharacter(uint8_t *ch);
int DbgConsole_SendData(uint8_t *ch, size_t size);
int DbgConsole_SendDataReliable(uint8_t *ch, size_t size);
int DbgConsole_ReadLine(uint8_t *buf, size_t size);
int DbgConsole_ReadCharacter(uint8_t *ch);

#if ((SDK_DEBUGCONSOLE != DEBUGCONSOLE_REDIRECT_TO_SDK) && defined(DEBUG_CONSOLE_TRANSFER_NON_BLOCKING) && \
     (defined(DEBUG_CONSOLE_TX_RELIABLE_ENABLE) && (DEBUG_CONSOLE_TX_RELIABLE_ENABLE > 0U)))
DEBUG_CONSOLE_FUNCTION_PREFIX status_t DbgConsole_Flush(void);
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/

#if defined(DEBUG_CONSOLE_TRANSFER_NON_BLOCKING)

static status_t DbgConsole_SerialManagerPerformTransfer(debug_console_state_struct_t *ioState)
{
    serial_manager_status_t ret = kStatus_SerialManager_Error;
    uint32_t sendDataLength;
    uint32_t startIndex;
    uint32_t regPrimask;

    regPrimask = DisableGlobalIRQ();
    if (ioState->writeRingBuffer.ringTail != ioState->writeRingBuffer.ringHead)
    {
        if (ioState->writeRingBuffer.ringHead > ioState->writeRingBuffer.ringTail)
        {
            sendDataLength = ioState->writeRingBuffer.ringHead - ioState->writeRingBuffer.ringTail;
            startIndex     = ioState->writeRingBuffer.ringTail;
        }
        else
        {
            sendDataLength = ioState->writeRingBuffer.ringBufferSize - ioState->writeRingBuffer.ringTail;
            startIndex     = ioState->writeRingBuffer.ringTail;
            if (0U != ioState->writeRingBuffer.ringHead)
            {
                ret = SerialManager_WriteNonBlocking(((serial_write_handle_t)&ioState->serialWriteHandleBuffer2[0]),
                                                     &ioState->writeRingBuffer.ringBuffer[startIndex], sendDataLength);
                sendDataLength = ioState->writeRingBuffer.ringHead - 0U;
                startIndex     = 0U;
            }
        }
        ret = SerialManager_WriteNonBlocking(((serial_write_handle_t)&ioState->serialWriteHandleBuffer[0]),
                                             &ioState->writeRingBuffer.ringBuffer[startIndex], sendDataLength);
    }
    EnableGlobalIRQ(regPrimask);
    return (status_t)ret;
}

static void DbgConsole_SerialManagerTxCallback(void *callbackParam,
                                               serial_manager_callback_message_t *message,
                                               serial_manager_status_t serialManagerStatus)
{
    debug_console_state_struct_t *ioState;

    if ((NULL == callbackParam) || (NULL == message))
    {
        return;
    }

    ioState = (debug_console_state_struct_t *)callbackParam;

    ioState->writeRingBuffer.ringTail += message->length;
    if (ioState->writeRingBuffer.ringTail >= ioState->writeRingBuffer.ringBufferSize)
    {
        ioState->writeRingBuffer.ringTail = 0U;
    }

    if (kStatus_SerialManager_Success == serialManagerStatus)
    {
        (void)DbgConsole_SerialManagerPerformTransfer(ioState);
    }
    else if (kStatus_SerialManager_Canceled == serialManagerStatus)
    {
        ioState->writeRingBuffer.ringTail = 0U;
        ioState->writeRingBuffer.ringHead = 0U;
    }
    else
    {
        /*MISRA rule 16.4*/
    }
}

static void DbgConsole_SerialManagerTx2Callback(void *callbackParam,
                                                serial_manager_callback_message_t *message,
                                                serial_manager_status_t serialManagerStatus)
{
    debug_console_state_struct_t *ioState;

    if ((NULL == callbackParam) || (NULL == message))
    {
        return;
    }

    ioState = (debug_console_state_struct_t *)callbackParam;

    ioState->writeRingBuffer.ringTail += message->length;
    if (ioState->writeRingBuffer.ringTail >= ioState->writeRingBuffer.ringBufferSize)
    {
        ioState->writeRingBuffer.ringTail = 0U;
    }

    if (kStatus_SerialManager_Success == serialManagerStatus)
    {
        /* Empty block*/
    }
    else if (kStatus_SerialManager_Canceled == serialManagerStatus)
    {
        /* Empty block*/
    }
    else
    {
        /*MISRA rule 16.4*/
    }
}

#if (defined(DEBUG_CONSOLE_RX_ENABLE) && (DEBUG_CONSOLE_RX_ENABLE > 0U))

static void DbgConsole_SerialManagerRxCallback(void *callbackParam,
                                               serial_manager_callback_message_t *message,
                                               serial_manager_status_t serialManagerStatus)
{
    if ((NULL == callbackParam) || (NULL == message))
    {
        return;
    }

    if (kStatus_SerialManager_Notify == serialManagerStatus)
    {
    }
    else if (kStatus_SerialManager_Success == serialManagerStatus)
    {
        /* release s_debugConsoleReadWaitSemaphore from RX callback */
        DEBUG_CONSOLE_GIVE_BINARY_SEMAPHORE_FROM_ISR(s_debugConsoleReadWaitSemaphore);
    }
    else
    {
        /*MISRA rule 16.4*/
    }
}
#endif

#endif

status_t DbgConsole_ReadOneCharacter(uint8_t *ch)
{
#if (defined(DEBUG_CONSOLE_RX_ENABLE) && (DEBUG_CONSOLE_RX_ENABLE > 0U))

#if defined(DEBUG_CONSOLE_TRANSFER_NON_BLOCKING) && \
    (DEBUG_CONSOLE_SYNCHRONIZATION_MODE == DEBUG_CONSOLE_SYNCHRONIZATION_BM) && defined(OSA_USED)
    return (status_t)kStatus_Fail;
#else  /*defined(DEBUG_CONSOLE_TRANSFER_NON_BLOCKING) && (DEBUG_CONSOLE_SYNCHRONIZATION_MODE == \
          DEBUG_CONSOLE_SYNCHRONIZATION_BM) && defined(OSA_USED)*/
    serial_manager_status_t serialManagerStatus = kStatus_SerialManager_Error;

/* recieve one char every time */
#if defined(DEBUG_CONSOLE_TRANSFER_NON_BLOCKING)
    serialManagerStatus =
        SerialManager_ReadNonBlocking(((serial_read_handle_t)&s_debugConsoleState.serialReadHandleBuffer[0]), ch, 1);
#else  /*defined(DEBUG_CONSOLE_TRANSFER_NON_BLOCKING)*/
    serialManagerStatus =
        SerialManager_ReadBlocking(((serial_read_handle_t)&s_debugConsoleState.serialReadHandleBuffer[0]), ch, 1);
#endif /*defined(DEBUG_CONSOLE_TRANSFER_NON_BLOCKING)*/
    if (kStatus_SerialManager_Success != serialManagerStatus)
    {
        serialManagerStatus = (serial_manager_status_t)kStatus_Fail;
    }
    else
    {
        /* wait s_debugConsoleReadWaitSemaphore from RX callback */
        DEBUG_CONSOLE_TAKE_BINARY_SEMAPHORE_BLOCKING(s_debugConsoleReadWaitSemaphore);
        serialManagerStatus = (serial_manager_status_t)kStatus_Success;
    }
    return (status_t)serialManagerStatus;
#endif /*defined(DEBUG_CONSOLE_TRANSFER_NON_BLOCKING) && (DEBUG_CONSOLE_SYNCHRONIZATION_MODE == \
          DEBUG_CONSOLE_SYNCHRONIZATION_BM) && defined(OSA_USED)*/

#else  /*(defined(DEBUG_CONSOLE_RX_ENABLE) && (DEBUG_CONSOLE_RX_ENABLE > 0U))*/

    return (status_t)kStatus_Fail;

#endif /*(defined(DEBUG_CONSOLE_RX_ENABLE) && (DEBUG_CONSOLE_RX_ENABLE > 0U))*/
}

#if DEBUG_CONSOLE_ENABLE_ECHO_FUNCTION
static status_t DbgConsole_EchoCharacter(uint8_t *ch, bool isGetChar, int *index)
{
    /* Due to scanf take \n and \r as end of string,should not echo */
    if (((*ch != (uint8_t)'\r') && (*ch != (uint8_t)'\n')) || (isGetChar))
    {
        /* recieve one char every time */
        if (1 != DbgConsole_SendDataReliable(ch, 1U))
        {
            return (status_t)kStatus_Fail;
        }
    }

    if ((!isGetChar) && (index != NULL))
    {
        if (DEBUG_CONSOLE_BACKSPACE == *ch)
        {
            if ((*index >= 2))
            {
                *index -= 2;
            }
            else
            {
                *index = 0;
            }
        }
    }

    return (status_t)kStatus_Success;
}
#endif

int DbgConsole_SendData(uint8_t *ch, size_t size)
{
    status_t dbgConsoleStatus;
#if defined(DEBUG_CONSOLE_TRANSFER_NON_BLOCKING)
    uint32_t sendDataLength;
    int txBusy = 0;
#endif
    assert(NULL != ch);
    assert(0U != size);

#if defined(DEBUG_CONSOLE_TRANSFER_NON_BLOCKING)
    uint32_t regPrimask = DisableGlobalIRQ();
    if (s_debugConsoleState.writeRingBuffer.ringHead != s_debugConsoleState.writeRingBuffer.ringTail)
    {
        txBusy = 1;
        sendDataLength =
            (s_debugConsoleState.writeRingBuffer.ringHead + s_debugConsoleState.writeRingBuffer.ringBufferSize -
             s_debugConsoleState.writeRingBuffer.ringTail) %
            s_debugConsoleState.writeRingBuffer.ringBufferSize;
    }
    else
    {
        sendDataLength = 0U;
    }
    sendDataLength = s_debugConsoleState.writeRingBuffer.ringBufferSize - sendDataLength - 1U;
    if (sendDataLength < size)
    {
        EnableGlobalIRQ(regPrimask);
        return -1;
    }
    for (int i = 0; i < (int)size; i++)
    {
        s_debugConsoleState.writeRingBuffer.ringBuffer[s_debugConsoleState.writeRingBuffer.ringHead++] = ch[i];
        if (s_debugConsoleState.writeRingBuffer.ringHead >= s_debugConsoleState.writeRingBuffer.ringBufferSize)
        {
            s_debugConsoleState.writeRingBuffer.ringHead = 0U;
        }
    }

    dbgConsoleStatus = (status_t)kStatus_SerialManager_Success;

    if (txBusy == 0)
    {
        dbgConsoleStatus = DbgConsole_SerialManagerPerformTransfer(&s_debugConsoleState);
    }
    EnableGlobalIRQ(regPrimask);
#else
    dbgConsoleStatus = (status_t)SerialManager_WriteBlocking(
        ((serial_write_handle_t)&s_debugConsoleState.serialWriteHandleBuffer[0]), ch, size);
#endif
    return (((status_t)kStatus_Success == dbgConsoleStatus) ? (int)size : -1);
}

int DbgConsole_SendDataReliable(uint8_t *ch, size_t size)
{
#if defined(DEBUG_CONSOLE_TRANSFER_NON_BLOCKING)
#if (defined(DEBUG_CONSOLE_TX_RELIABLE_ENABLE) && (DEBUG_CONSOLE_TX_RELIABLE_ENABLE > 0U))
    serial_manager_status_t serialManagerStatus = kStatus_SerialManager_Error;
    uint32_t sendDataLength;
    uint32_t totalLength = size;
    int sentLength;
#endif /* DEBUG_CONSOLE_TX_RELIABLE_ENABLE */
#else  /* DEBUG_CONSOLE_TRANSFER_NON_BLOCKING */
    serial_manager_status_t serialManagerStatus;
#endif /* DEBUG_CONSOLE_TRANSFER_NON_BLOCKING */

    assert(NULL != ch);

    if (0U == size)
    {
        return 0;
    }

    if (NULL == g_serialHandle)
    {
        return 0;
    }

#if defined(DEBUG_CONSOLE_TRANSFER_NON_BLOCKING)

#if (defined(DEBUG_CONSOLE_TX_RELIABLE_ENABLE) && (DEBUG_CONSOLE_TX_RELIABLE_ENABLE > 0U))
    do
    {
        uint32_t regPrimask = DisableGlobalIRQ();
        if (s_debugConsoleState.writeRingBuffer.ringHead != s_debugConsoleState.writeRingBuffer.ringTail)
        {
            sendDataLength =
                (s_debugConsoleState.writeRingBuffer.ringHead + s_debugConsoleState.writeRingBuffer.ringBufferSize -
                 s_debugConsoleState.writeRingBuffer.ringTail) %
                s_debugConsoleState.writeRingBuffer.ringBufferSize;
        }
        else
        {
            sendDataLength = 0U;
        }
        sendDataLength = s_debugConsoleState.writeRingBuffer.ringBufferSize - sendDataLength - 1U;

        if ((sendDataLength > 0U) && ((sendDataLength >= totalLength) ||
                                      (totalLength >= (s_debugConsoleState.writeRingBuffer.ringBufferSize - 1U))))
        {
            if (sendDataLength > totalLength)
            {
                sendDataLength = totalLength;
            }

            sentLength = DbgConsole_SendData(&ch[size - totalLength], (size_t)sendDataLength);
            if (sentLength > 0)
            {
                totalLength = totalLength - (uint32_t)sentLength;
            }
        }
        EnableGlobalIRQ(regPrimask);

        if (totalLength != 0U)
        {
            serialManagerStatus = (serial_manager_status_t)DbgConsole_Flush();
            if (kStatus_SerialManager_Success != serialManagerStatus)
            {
                break;
            }
        }
    } while (totalLength != 0U);
    return ((int)size - (int)totalLength);
#else  /* DEBUG_CONSOLE_TX_RELIABLE_ENABLE */
    return DbgConsole_SendData(ch, size);
#endif /* DEBUG_CONSOLE_TX_RELIABLE_ENABLE */

#else  /* DEBUG_CONSOLE_TRANSFER_NON_BLOCKING */
    serialManagerStatus =
        SerialManager_WriteBlocking(((serial_write_handle_t)&s_debugConsoleState.serialWriteHandleBuffer[0]), ch, size);
    return ((kStatus_SerialManager_Success == serialManagerStatus) ? (int)size : -1);
#endif /* DEBUG_CONSOLE_TRANSFER_NON_BLOCKING */
}

int DbgConsole_ReadLine(uint8_t *buf, size_t size)
{
    int i = 0;

    assert(buf != NULL);

    if (NULL == g_serialHandle)
    {
        return -1;
    }

    /* take mutex lock function */
#if (DEBUG_CONSOLE_SYNCHRONIZATION_MODE == DEBUG_CONSOLE_SYNCHRONIZATION_FREERTOS)
    DEBUG_CONSOLE_TAKE_MUTEX_SEMAPHORE_BLOCKING(s_debugConsoleReadSemaphore);
#endif

    do
    {
        /* recieve one char every time */
        if ((status_t)kStatus_Success != DbgConsole_ReadOneCharacter(&buf[i]))
        {
            /* release mutex lock function */
#if (DEBUG_CONSOLE_SYNCHRONIZATION_MODE == DEBUG_CONSOLE_SYNCHRONIZATION_FREERTOS)
            DEBUG_CONSOLE_GIVE_MUTEX_SEMAPHORE(s_debugConsoleReadSemaphore);
#endif
            i = -1;
            break;
        }
#if DEBUG_CONSOLE_ENABLE_ECHO_FUNCTION
        (void)DbgConsole_EchoCharacter(&buf[i], false, &i);
#endif
        /* analysis data */
        if (((uint8_t)'\r' == buf[i]) || ((uint8_t)'\n' == buf[i]))
        {
            /* End of Line. */
            if (0 == i)
            {
                buf[i] = (uint8_t)'\0';
                continue;
            }
            else
            {
                break;
            }
        }
        i++;
    } while (i < (int)size);

    /* get char should not add '\0'*/
    if (i == (int)size)
    {
        buf[i] = (uint8_t)'\0';
    }
    else
    {
        buf[i + 1] = (uint8_t)'\0';
    }

    /* release mutex lock function */
#if (DEBUG_CONSOLE_SYNCHRONIZATION_MODE == DEBUG_CONSOLE_SYNCHRONIZATION_FREERTOS)
    DEBUG_CONSOLE_GIVE_MUTEX_SEMAPHORE(s_debugConsoleReadSemaphore);
#endif

    return i;
}

int DbgConsole_ReadCharacter(uint8_t *ch)
{
    int ret;

    assert(ch);

    if (NULL == g_serialHandle)
    {
        return -1;
    }

    /* take mutex lock function */
#if (DEBUG_CONSOLE_SYNCHRONIZATION_MODE == DEBUG_CONSOLE_SYNCHRONIZATION_FREERTOS)
    DEBUG_CONSOLE_TAKE_MUTEX_SEMAPHORE_BLOCKING(s_debugConsoleReadSemaphore);
#endif
    /* read one character */
    if ((status_t)kStatus_Success == DbgConsole_ReadOneCharacter(ch))
    {
        ret = 1;
#if DEBUG_CONSOLE_ENABLE_ECHO_FUNCTION
        (void)DbgConsole_EchoCharacter(ch, true, NULL);
#endif
    }
    else
    {
        ret = -1;
    }

    /* release mutex lock function */
#if (DEBUG_CONSOLE_SYNCHRONIZATION_MODE == DEBUG_CONSOLE_SYNCHRONIZATION_FREERTOS)
    DEBUG_CONSOLE_GIVE_MUTEX_SEMAPHORE(s_debugConsoleReadSemaphore);
#endif

    return ret;
}

#if (defined(SDK_DEBUGCONSOLE) && (SDK_DEBUGCONSOLE == DEBUGCONSOLE_REDIRECT_TO_SDK))
static void DbgConsole_PrintCallback(char *buf, int32_t *indicator, char dbgVal, int len)
{
    int i = 0;

    for (i = 0; i < len; i++)
    {
        if (((uint32_t)*indicator + 1UL) >= (uint32_t)DEBUG_CONSOLE_PRINTF_MAX_LOG_LEN)
        {
            (void)DbgConsole_SendDataReliable((uint8_t *)buf, (size_t)(*indicator));
            *indicator = 0;
        }

        buf[*indicator] = dbgVal;
        (*indicator)++;
    }
}
#endif

/*************Code for DbgConsole Init, Deinit, Printf, Scanf *******************************/
#if ((SDK_DEBUGCONSOLE == DEBUGCONSOLE_REDIRECT_TO_SDK) || defined(SDK_DEBUGCONSOLE_UART))
#if (defined(SERIAL_USE_CONFIGURE_STRUCTURE) && (SERIAL_USE_CONFIGURE_STRUCTURE > 0U))
#include "board.h"
#if (defined(SERIAL_PORT_TYPE_UART) && (SERIAL_PORT_TYPE_UART > 0U))
static const serial_port_uart_config_t uartConfig = {.instance     = BOARD_DEBUG_UART_INSTANCE,
                                                     .clockRate    = BOARD_DEBUG_UART_CLK_FREQ,
                                                     .baudRate     = BOARD_DEBUG_UART_BAUDRATE,
                                                     .parityMode   = kSerialManager_UartParityDisabled,
                                                     .stopBitCount = kSerialManager_UartOneStopBit,
                                                     .enableRx     = 1U,
                                                     .enableTx     = 1U,
                                                     .enableRxRTS  = 0U,
                                                     .enableTxCTS  = 0U,
#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
                                                     .txFifoWatermark = 0U,
                                                     .rxFifoWatermark = 0U
#endif
};
#endif
#endif
/* See fsl_debug_console.h for documentation of this function. */
status_t DbgConsole_Init(uint8_t instance, uint32_t baudRate, serial_port_type_t device, uint32_t clkSrcFreq)
{
    serial_manager_config_t serialConfig;
    serial_manager_status_t serialManagerStatus = kStatus_SerialManager_Success;

#if (defined(SERIAL_USE_CONFIGURE_STRUCTURE) && (SERIAL_USE_CONFIGURE_STRUCTURE == 0U))
#if (defined(SERIAL_PORT_TYPE_UART) && (SERIAL_PORT_TYPE_UART > 0U))
    serial_port_uart_config_t uartConfig = {
        .instance     = instance,
        .clockRate    = clkSrcFreq,
        .baudRate     = baudRate,
        .parityMode   = kSerialManager_UartParityDisabled,
        .stopBitCount = kSerialManager_UartOneStopBit,
        .enableRx     = 1,
        .enableTx     = 1,
        .enableRxRTS  = 0U,
        .enableTxCTS  = 0U,
#if (defined(HAL_UART_ADAPTER_FIFO) && (HAL_UART_ADAPTER_FIFO > 0u))
        .txFifoWatermark = 0U,
        .rxFifoWatermark = 0U
#endif
    };
#endif
#endif
    (void)memset(&serialConfig, 0x0, sizeof(serial_manager_config_t));
#if (defined(SERIAL_PORT_TYPE_USBCDC) && (SERIAL_PORT_TYPE_USBCDC > 0U))
    serial_port_usb_cdc_config_t usbCdcConfig = {
        .controllerIndex = (serial_port_usb_cdc_controller_index_t)instance,
    };
#endif

#if (defined(SERIAL_PORT_TYPE_SWO) && (SERIAL_PORT_TYPE_SWO > 0U))
    serial_port_swo_config_t swoConfig = {
        .clockRate = clkSrcFreq,
        .baudRate  = baudRate,
        .port      = instance,
        .protocol  = kSerialManager_SwoProtocolNrz,
    };
#endif

#if (defined(SERIAL_PORT_TYPE_VIRTUAL) && (SERIAL_PORT_TYPE_VIRTUAL > 0U))
    serial_port_virtual_config_t serialPortVirtualConfig = {
        .controllerIndex = (serial_port_virtual_controller_index_t)instance,
    };
#endif

    serialConfig.type = device;
#if defined(DEBUG_CONSOLE_TRANSFER_NON_BLOCKING)
    serialConfig.ringBuffer     = &s_debugConsoleState.readRingBuffer[0];
    serialConfig.ringBufferSize = DEBUG_CONSOLE_RECEIVE_BUFFER_LEN;
    serialConfig.blockType      = kSerialManager_NonBlocking;
#else
    serialConfig.blockType = kSerialManager_Blocking;
#endif

    if (kSerialPort_Uart == device)
    {
#if (defined(SERIAL_PORT_TYPE_UART) && (SERIAL_PORT_TYPE_UART > 0U))
#if (defined(SERIAL_USE_CONFIGURE_STRUCTURE) && (SERIAL_USE_CONFIGURE_STRUCTURE > 0U))
        serialConfig.portConfig = (void *)&uartConfig;
#else
        serialConfig.portConfig = &uartConfig;
#endif
#else
        serialManagerStatus = kStatus_SerialManager_Error;
#endif
    }
    else if (kSerialPort_UsbCdc == device)
    {
#if (defined(SERIAL_PORT_TYPE_USBCDC) && (SERIAL_PORT_TYPE_USBCDC > 0U))
        serialConfig.portConfig = &usbCdcConfig;
#else
        serialManagerStatus = kStatus_SerialManager_Error;
#endif
    }
    else if (kSerialPort_Swo == device)
    {
#if (defined(SERIAL_PORT_TYPE_SWO) && (SERIAL_PORT_TYPE_SWO > 0U))
        serialConfig.portConfig = &swoConfig;
#else
        serialManagerStatus = kStatus_SerialManager_Error;
#endif
    }
    else if (kSerialPort_Virtual == device)
    {
#if (defined(SERIAL_PORT_TYPE_VIRTUAL) && (SERIAL_PORT_TYPE_VIRTUAL > 0U))
        serialConfig.portConfig = &serialPortVirtualConfig;
#else
        serialManagerStatus = kStatus_SerialManager_Error;
#endif
    }
    else if (kSerialPort_BleWu == device)
    {
#if (defined(SERIAL_PORT_TYPE_BLE_WU) && (SERIAL_PORT_TYPE_BLE_WU > 0U))
        serialConfig.portConfig = NULL;
#else
        serialManagerStatus = kStatus_SerialManager_Error;
#endif
    }
    else
    {
        serialManagerStatus = kStatus_SerialManager_Error;
    }

    if (kStatus_SerialManager_Error != serialManagerStatus)
    {
        (void)memset(&s_debugConsoleState, 0, sizeof(s_debugConsoleState));

#if defined(DEBUG_CONSOLE_TRANSFER_NON_BLOCKING)
        s_debugConsoleState.writeRingBuffer.ringBufferSize = DEBUG_CONSOLE_TRANSMIT_BUFFER_LEN;
#endif

        s_debugConsoleState.serialHandle = (serial_handle_t)&s_debugConsoleState.serialHandleBuffer[0];
        serialManagerStatus              = SerialManager_Init(s_debugConsoleState.serialHandle, &serialConfig);

        assert(kStatus_SerialManager_Success == serialManagerStatus);

#if (DEBUG_CONSOLE_SYNCHRONIZATION_MODE == DEBUG_CONSOLE_SYNCHRONIZATION_FREERTOS)
#if configSUPPORT_STATIC_ALLOCATION
        DEBUG_CONSOLE_CREATE_MUTEX_SEMAPHORE(s_debugConsoleReadSemaphore, &s_debugConsoleReadSemaphoreStatic);
#else
        DEBUG_CONSOLE_CREATE_MUTEX_SEMAPHORE(s_debugConsoleReadSemaphore);
#endif
#endif
#if (defined(DEBUG_CONSOLE_RX_ENABLE) && (DEBUG_CONSOLE_RX_ENABLE > 0U))
#if (DEBUG_CONSOLE_SYNCHRONIZATION_MODE == DEBUG_CONSOLE_SYNCHRONIZATION_FREERTOS) && configSUPPORT_STATIC_ALLOCATION
        DEBUG_CONSOLE_CREATE_BINARY_SEMAPHORE(s_debugConsoleReadWaitSemaphore, &s_debugConsoleReadWaitSemaphoreStatic);
#else
        DEBUG_CONSOLE_CREATE_BINARY_SEMAPHORE(s_debugConsoleReadWaitSemaphore);
#endif
#endif

        {
            serialManagerStatus =
                SerialManager_OpenWriteHandle(s_debugConsoleState.serialHandle,
                                              ((serial_write_handle_t)&s_debugConsoleState.serialWriteHandleBuffer[0]));
            assert(kStatus_SerialManager_Success == serialManagerStatus);
#if defined(DEBUG_CONSOLE_TRANSFER_NON_BLOCKING)
            (void)SerialManager_InstallTxCallback(
                ((serial_write_handle_t)&s_debugConsoleState.serialWriteHandleBuffer[0]),
                DbgConsole_SerialManagerTxCallback, &s_debugConsoleState);
            serialManagerStatus = SerialManager_OpenWriteHandle(
                s_debugConsoleState.serialHandle,
                ((serial_write_handle_t)&s_debugConsoleState.serialWriteHandleBuffer2[0]));
            assert(kStatus_SerialManager_Success == serialManagerStatus);
            (void)SerialManager_InstallTxCallback(
                ((serial_write_handle_t)&s_debugConsoleState.serialWriteHandleBuffer2[0]),
                DbgConsole_SerialManagerTx2Callback, &s_debugConsoleState);
#endif
        }

#if (defined(DEBUG_CONSOLE_RX_ENABLE) && (DEBUG_CONSOLE_RX_ENABLE > 0U))
        {
            serialManagerStatus =
                SerialManager_OpenReadHandle(s_debugConsoleState.serialHandle,
                                             ((serial_read_handle_t)&s_debugConsoleState.serialReadHandleBuffer[0]));
            assert(kStatus_SerialManager_Success == serialManagerStatus);
#if defined(DEBUG_CONSOLE_TRANSFER_NON_BLOCKING)
            (void)SerialManager_InstallRxCallback(
                ((serial_read_handle_t)&s_debugConsoleState.serialReadHandleBuffer[0]),
                DbgConsole_SerialManagerRxCallback, &s_debugConsoleState);
#endif
        }
#endif

        g_serialHandle = s_debugConsoleState.serialHandle;
    }
    return (status_t)serialManagerStatus;
}

/* See fsl_debug_console.h for documentation of this function. */
status_t DbgConsole_EnterLowpower(void)
{
    serial_manager_status_t serialManagerStatus = kStatus_SerialManager_Error;
    if (s_debugConsoleState.serialHandle != NULL)
    {
        serialManagerStatus = SerialManager_EnterLowpower(s_debugConsoleState.serialHandle);
    }
    return (status_t)serialManagerStatus;
}

/* See fsl_debug_console.h for documentation of this function. */
status_t DbgConsole_ExitLowpower(void)
{
    serial_manager_status_t serialManagerStatus = kStatus_SerialManager_Error;

    if (s_debugConsoleState.serialHandle != NULL)
    {
        serialManagerStatus = SerialManager_ExitLowpower(s_debugConsoleState.serialHandle);
    }
    return (status_t)serialManagerStatus;
}
/* See fsl_debug_console.h for documentation of this function. */
status_t DbgConsole_Deinit(void)
{
    {
        if (s_debugConsoleState.serialHandle != NULL)
        {
#if defined(DEBUG_CONSOLE_TRANSFER_NON_BLOCKING)
            (void)SerialManager_CloseWriteHandle(
                ((serial_write_handle_t)&s_debugConsoleState.serialWriteHandleBuffer2[0]));
#endif
            (void)SerialManager_CloseWriteHandle(
                ((serial_write_handle_t)&s_debugConsoleState.serialWriteHandleBuffer[0]));
        }
    }
#if (defined(DEBUG_CONSOLE_RX_ENABLE) && (DEBUG_CONSOLE_RX_ENABLE > 0U))
    {
        if (s_debugConsoleState.serialHandle != NULL)
        {
            (void)SerialManager_CloseReadHandle(((serial_read_handle_t)&s_debugConsoleState.serialReadHandleBuffer[0]));
        }
    }
#endif
    if (NULL != s_debugConsoleState.serialHandle)
    {
        if (kStatus_SerialManager_Success == SerialManager_Deinit(s_debugConsoleState.serialHandle))
        {
            s_debugConsoleState.serialHandle = NULL;
            g_serialHandle                   = NULL;
        }
    }
#if (defined(DEBUG_CONSOLE_RX_ENABLE) && (DEBUG_CONSOLE_RX_ENABLE > 0U))
    DEBUG_CONSOLE_DESTROY_BINARY_SEMAPHORE(s_debugConsoleReadWaitSemaphore);
#endif
#if (DEBUG_CONSOLE_SYNCHRONIZATION_MODE == DEBUG_CONSOLE_SYNCHRONIZATION_FREERTOS)
    DEBUG_CONSOLE_DESTROY_MUTEX_SEMAPHORE(s_debugConsoleReadSemaphore);
#endif

    return (status_t)kStatus_Success;
}
#endif /* ((SDK_DEBUGCONSOLE == DEBUGCONSOLE_REDIRECT_TO_SDK) || defined(SDK_DEBUGCONSOLE_UART)) */

#if (((defined(SDK_DEBUGCONSOLE) && (SDK_DEBUGCONSOLE == DEBUGCONSOLE_REDIRECT_TO_SDK))) ||                 \
     ((SDK_DEBUGCONSOLE != DEBUGCONSOLE_REDIRECT_TO_SDK) && defined(DEBUG_CONSOLE_TRANSFER_NON_BLOCKING) && \
      (defined(DEBUG_CONSOLE_TX_RELIABLE_ENABLE) && (DEBUG_CONSOLE_TX_RELIABLE_ENABLE > 0U))))
DEBUG_CONSOLE_FUNCTION_PREFIX status_t DbgConsole_Flush(void)
{
#if defined(DEBUG_CONSOLE_TRANSFER_NON_BLOCKING)

#if (DEBUG_CONSOLE_SYNCHRONIZATION_MODE == DEBUG_CONSOLE_SYNCHRONIZATION_BM) && defined(OSA_USED)

    if (s_debugConsoleState.writeRingBuffer.ringHead != s_debugConsoleState.writeRingBuffer.ringTail)
    {
        return (status_t)kStatus_Fail;
    }

#else

    while (s_debugConsoleState.writeRingBuffer.ringHead != s_debugConsoleState.writeRingBuffer.ringTail)
    {
#if (DEBUG_CONSOLE_SYNCHRONIZATION_MODE == DEBUG_CONSOLE_SYNCHRONIZATION_FREERTOS)
        if (0U == IS_RUNNING_IN_ISR())
        {
            if (taskSCHEDULER_RUNNING == xTaskGetSchedulerState())
            {
                vTaskDelay(1);
            }
        }
        else
        {
            return (status_t)kStatus_Fail;
        }
#endif
    }

#endif

#endif
    return (status_t)kStatus_Success;
}
#endif

#if (defined(SDK_DEBUGCONSOLE) && (SDK_DEBUGCONSOLE == DEBUGCONSOLE_REDIRECT_TO_SDK))
/* See fsl_debug_console.h for documentation of this function. */
int DbgConsole_Printf(const char *fmt_s, ...)
{
    va_list ap;
    int result = 0;

    va_start(ap, fmt_s);
    result = DbgConsole_Vprintf(fmt_s, ap);
    va_end(ap);

    return result;
}

/* See fsl_debug_console.h for documentation of this function. */
int DbgConsole_Vprintf(const char *fmt_s, va_list formatStringArg)
{
    int logLength = 0, result = 0;
    char printBuf[DEBUG_CONSOLE_PRINTF_MAX_LOG_LEN] = {'\0'};

    if (NULL != g_serialHandle)
    {
        /* format print log first */
        logLength = StrFormatPrintf(fmt_s, formatStringArg, printBuf, DbgConsole_PrintCallback);
        /* print log */
        result = DbgConsole_SendDataReliable((uint8_t *)printBuf, (size_t)logLength);
    }
    return result;
}

/* See fsl_debug_console.h for documentation of this function. */
int DbgConsole_Putchar(int ch)
{
    /* print char */
    return DbgConsole_SendDataReliable((uint8_t *)&ch, 1U);
}

/* See fsl_debug_console.h for documentation of this function. */
int DbgConsole_Scanf(char *fmt_s, ...)
{
    va_list ap;
    int formatResult;
    char scanfBuf[DEBUG_CONSOLE_SCANF_MAX_LOG_LEN + 1U] = {'\0'};

    /* scanf log */
    (void)DbgConsole_ReadLine((uint8_t *)scanfBuf, DEBUG_CONSOLE_SCANF_MAX_LOG_LEN);
    /* get va_list */
    va_start(ap, fmt_s);
    /* format scanf log */
    formatResult = StrFormatScanf(scanfBuf, fmt_s, ap);

    va_end(ap);

    return formatResult;
}

/* See fsl_debug_console.h for documentation of this function. */
int DbgConsole_BlockingPrintf(const char *fmt_s, ...)
{
    va_list ap;
    int result = 0;

    va_start(ap, fmt_s);
    result = DbgConsole_BlockingVprintf(fmt_s, ap);
    va_end(ap);

    return result;
}

/* See fsl_debug_console.h for documentation of this function. */
int DbgConsole_BlockingVprintf(const char *fmt_s, va_list formatStringArg)
{
    status_t dbgConsoleStatus;
    int logLength = 0, result = 0;
    char printBuf[DEBUG_CONSOLE_PRINTF_MAX_LOG_LEN] = {'\0'};

    if (NULL == g_serialHandle)
    {
        return 0;
    }

    /* format print log first */
    logLength = StrFormatPrintf(fmt_s, formatStringArg, printBuf, DbgConsole_PrintCallback);

#if defined(DEBUG_CONSOLE_TRANSFER_NON_BLOCKING)
    (void)SerialManager_CancelWriting(((serial_write_handle_t)&s_debugConsoleState.serialWriteHandleBuffer[0]));
#endif
    /* print log */
    dbgConsoleStatus =
        (status_t)SerialManager_WriteBlocking(((serial_write_handle_t)&s_debugConsoleState.serialWriteHandleBuffer[0]),
                                              (uint8_t *)printBuf, (size_t)logLength);
    result = (((status_t)kStatus_Success == dbgConsoleStatus) ? (int)logLength : -1);

    return result;
}

#ifdef DEBUG_CONSOLE_TRANSFER_NON_BLOCKING
status_t DbgConsole_TryGetchar(char *ch)
{
#if (defined(DEBUG_CONSOLE_RX_ENABLE) && (DEBUG_CONSOLE_RX_ENABLE > 0U))
    uint32_t length           = 0;
    status_t dbgConsoleStatus = (status_t)kStatus_Fail;

    assert(ch);

    if (NULL == g_serialHandle)
    {
        return kStatus_Fail;
    }

    /* take mutex lock function */
#if (DEBUG_CONSOLE_SYNCHRONIZATION_MODE == DEBUG_CONSOLE_SYNCHRONIZATION_FREERTOS)
    DEBUG_CONSOLE_TAKE_MUTEX_SEMAPHORE_BLOCKING(s_debugConsoleReadSemaphore);
#endif

    if (kStatus_SerialManager_Success ==
        SerialManager_TryRead(((serial_read_handle_t)&s_debugConsoleState.serialReadHandleBuffer[0]), (uint8_t *)ch, 1,
                              &length))
    {
        if (length != 0U)
        {
#if DEBUG_CONSOLE_ENABLE_ECHO_FUNCTION
            (void)DbgConsole_EchoCharacter((uint8_t *)ch, true, NULL);
#endif
            dbgConsoleStatus = (status_t)kStatus_Success;
        }
    }
    /* release mutex lock function */
#if (DEBUG_CONSOLE_SYNCHRONIZATION_MODE == DEBUG_CONSOLE_SYNCHRONIZATION_FREERTOS)
    DEBUG_CONSOLE_GIVE_MUTEX_SEMAPHORE(s_debugConsoleReadSemaphore);
#endif
    return dbgConsoleStatus;
#else
    return (status_t)kStatus_Fail;
#endif
}
#endif

/* See fsl_debug_console.h for documentation of this function. */
int DbgConsole_Getchar(void)
{
    int ret    = -1;
    uint8_t ch = 0U;

    /* Get char */
    if (DbgConsole_ReadCharacter(&ch) > 0)
    {
        ret = (int)ch;
    }

    return ret;
}

#endif /* SDK_DEBUGCONSOLE */

/*************Code to support toolchain's printf, scanf *******************************/
/* These function __write and __read is used to support IAR toolchain to printf and scanf*/
#if (defined(__ICCARM__))
#if defined(SDK_DEBUGCONSOLE_UART)
#pragma weak __write
size_t __write(int handle, const unsigned char *buffer, size_t size);
size_t __write(int handle, const unsigned char *buffer, size_t size)
{
    size_t ret;
    if (NULL == buffer)
    {
        /*
         * This means that we should flush internal buffers.  Since we don't we just return.
         * (Remember, "handle" == -1 means that all handles should be flushed.)
         */
        ret = 0U;
    }
    else if ((handle != 1) && (handle != 2))
    {
        /* This function only writes to "standard out" and "standard err" for all other file handles it returns failure.
         */
        ret = (size_t)-1;
    }
    else
    {
        /* Send data. */
        uint8_t buff[512];
        (void)memcpy(buff, buffer, size);
        (void)DbgConsole_SendDataReliable((uint8_t *)buff, size);

        ret = size;
    }
    return ret;
}

#pragma weak __read
size_t __read(int handle, unsigned char *buffer, size_t size);
size_t __read(int handle, unsigned char *buffer, size_t size)
{
    uint8_t ch     = 0U;
    int actualSize = 0;

    /* This function only reads from "standard in", for all other file  handles it returns failure. */
    if (0 != handle)
    {
        actualSize = -1;
    }
    else
    {
        /* Receive data.*/
        for (; size > 0U; size--)
        {
            (void)DbgConsole_ReadCharacter(&ch);
            if (0U == ch)
            {
                break;
            }

            *buffer++ = ch;
            actualSize++;
        }
    }
    return (size_t)actualSize;
}
#endif /* SDK_DEBUGCONSOLE_UART */

/* support LPC Xpresso with RedLib */
#elif (defined(__REDLIB__))

#if (defined(SDK_DEBUGCONSOLE_UART))
int __attribute__((weak)) __sys_write(int handle, char *buffer, int size)
{
    if (NULL == buffer)
    {
        /* return -1 if error. */
        return -1;
    }

    /* This function only writes to "standard out" and "standard err" for all other file handles it returns failure. */
    if ((handle != 1) && (handle != 2))
    {
        return -1;
    }

    /* Send data. */
    DbgConsole_SendDataReliable((uint8_t *)buffer, size);

    return 0;
}

int __attribute__((weak)) __sys_readc(void)
{
    char tmp;

    /* Receive data. */
    DbgConsole_ReadCharacter((uint8_t *)&tmp);

    return tmp;
}
#endif /* SDK_DEBUGCONSOLE_UART */

/* These function fputc and fgetc is used to support KEIL toolchain to printf and scanf*/
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
#if defined(SDK_DEBUGCONSOLE_UART)
#if defined(__CC_ARM)
struct __FILE
{
    int handle;
    /*
     * Whatever you require here. If the only file you are using is standard output using printf() for debugging,
     * no file handling is required.
     */
};
#endif

/* FILE is typedef in stdio.h. */
#pragma weak __stdout
#pragma weak __stdin
FILE __stdout;
FILE __stdin;

#pragma weak fputc
int fputc(int ch, FILE *f)
{
    /* Send data. */
    return DbgConsole_SendDataReliable((uint8_t *)(&ch), 1);
}

#pragma weak fgetc
int fgetc(FILE *f)
{
    char ch;

    /* Receive data. */
    DbgConsole_ReadCharacter((uint8_t *)&ch);

    return ch;
}

/*
 * Terminate the program, passing a return code back to the user.
 * This function may not return.
 */
void _sys_exit(int returncode)
{
    while (1)
    {
    }
}

/*
 * Writes a character to the output channel. This function is used
 * for last-resort error message output.
 */
void _ttywrch(int ch)
{
    char ench = ch;
    DbgConsole_SendDataReliable((uint8_t *)(&ench), 1);
}

char *_sys_command_string(char *cmd, int len)
{
    return (cmd);
}
#endif /* SDK_DEBUGCONSOLE_UART */

/* These function __write and __read is used to support ARM_GCC, KDS, Atollic toolchains to printf and scanf*/
#elif (defined(__GNUC__))

#if ((defined(__GNUC__) && (!defined(__MCUXPRESSO)) && (defined(SDK_DEBUGCONSOLE_UART))) || \
     (defined(__MCUXPRESSO) && (defined(SDK_DEBUGCONSOLE_UART))))
int __attribute__((weak)) _write(int handle, char *buffer, int size);
int __attribute__((weak)) _write(int handle, char *buffer, int size)
{
    if (NULL == buffer)
    {
        /* return -1 if error. */
        return -1;
    }

    /* This function only writes to "standard out" and "standard err" for all other file handles it returns failure. */
    if ((handle != 1) && (handle != 2))
    {
        return -1;
    }

    /* Send data. */
    (void)DbgConsole_SendDataReliable((uint8_t *)buffer, (size_t)size);

    return size;
}

int __attribute__((weak)) _read(int handle, char *buffer, int size);
int __attribute__((weak)) _read(int handle, char *buffer, int size)
{
    uint8_t ch     = 0U;
    int actualSize = 0;

    /* This function only reads from "standard in", for all other file handles it returns failure. */
    if (handle != 0)
    {
        return -1;
    }

    /* Receive data. */
    for (; size > 0; size--)
    {
        if (DbgConsole_ReadCharacter(&ch) < 0)
        {
            break;
        }

        *buffer++ = (char)ch;
        actualSize++;

        if ((ch == 0U) || (ch == (uint8_t)'\n') || (ch == (uint8_t)'\r'))
        {
            break;
        }
    }

    return (actualSize > 0) ? actualSize : -1;
}
#endif

#endif /* __ICCARM__ */
