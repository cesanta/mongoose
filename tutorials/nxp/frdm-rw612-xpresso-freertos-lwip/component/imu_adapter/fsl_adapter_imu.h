/*
 * Copyright 2021 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __FSL_ADAPTER_RFIMU_H__
#define __FSL_ADAPTER_RFIMU_H__

#include "fsl_adapter_imu_common.h"

#if defined(SDK_OS_FREE_RTOS)
#include "FreeRTOS.h"
#endif
/*!
 * @addtogroup IMU_Adapter
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief IMUMC Msaster/Remote role definition (0 - Master, 1 - Remote) */
#ifndef HAL_IMUMC_SELECT_ROLE
#define HAL_IMUMC_SELECT_ROLE (1U)
#endif

/*! @brief IMUMC handle size definition */
#define HAL_IMUMC_HANDLE_SIZE (52U)

#ifndef REMOTE_CORE_BOOT_ADDRESS
#define REMOTE_CORE_BOOT_ADDRESS (0x01000000U)
#endif

#ifndef MAX_EP_COUNT
#define MAX_EP_COUNT (5U)
#endif

#define IMUMC_WAITFOREVER (0xFFFFFFFFU)

/*!
 * @brief Defines the imumc handle
 *
 * This macro is used to define a 4 byte aligned imumc handle.
 * Then use "(hal_imumc_handle_t)name" to get the imumc handle.
 *
 * The macro should be global and could be optional. You could also define imumc handle by yourself.
 *
 * This is an example,
 * @code
 * IMUMC_HANDLE_DEFINE(imumcHandle);
 * @endcode
 *
 * @param name The name string of the imumc handle.
 */
#define IMUMC_HANDLE_DEFINE(name) uint32_t name[((HAL_IMUMC_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t))]

/*! @brief The handle of IMUMC adapter */
typedef void *hal_imumc_handle_t;

/*! @brief IMUMC status */
typedef enum _hal_imumc_status
{
    kStatus_HAL_ImumcSuccess = 0U,
    kStatus_HAL_ImumcError,
    kStatus_HAL_ImumcRxBusy,
    kStatus_HAL_ImumcTxBusy,
    kStatus_HAL_ImumcTxIdle,
    kStatus_HAL_ImumcRxIdle,
    kStatus_HAL_ImumcTimeout,
} hal_imumc_status_t;

/*! @brief IMUMC return status */
typedef enum _hal_imumc_return_status
{
    kStatus_HAL_RL_RELEASE = 0U,
    kStatus_HAL_RL_HOLD,
} hal_imumc_return_status_t;

/*! @brief The callback function of IMUMC adapter.
 *
 * @note If Imumc RX callback function return kStatus_HAL_RL_RELEASE mode, no need to call HAL_ImumcFreeRxBuffer.
 * @note If Imumc RX callback function return kStatus_HAL_RL_HOLD mode,then need to call HAL_ImumcFreeRxBuffer.
 *
 */
typedef hal_imumc_return_status_t (*imumc_rx_callback_t)(void *param, uint8_t *data, uint32_t len);

/*! @brief The configure structure of IMUMC adapter. */
typedef struct _hal_imumc_config
{
    uint8_t local_addr;           /* Local address for rx */
    uint8_t remote_addr;          /* Remote address for tx */
    imumc_rx_callback_t callback; /* RPMGS Rx callback  */
    void *param;                  /* RPMGS Rx callback parameter */
    uint8_t imuLink;
} hal_imumc_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */

/*!
 * @brief Initializes the IMUMC adapter module for dual core communication.
 *
 * @note This API should be called at the beginning of the application using the IMUMC adapter driver.
 *
 * @retval kStatus_HAL_ImumcSuccess IMUMC module initialize succeed.
 */
hal_imumc_status_t HAL_ImumcMcmgrInit(void);

/*!
 * @brief Initializes the IMUMC adapter for IMUMC channel configure.
 *
 * @note This API should be called to software IMUMC communication configure, and it be
 * called whenever application need it.
 *
 * @param handle Pointer to point to a memory space of size #HAL_IMUMC_HANDLE_SIZE allocated by the caller.
 * The handle should be 4 byte aligned, because unaligned access doesn't be supported on some devices.
 * You can define the handle in the following two ways:
 * #IMUMC_HANDLE_DEFINE(handle);
 * or
 * uint32_t handle[((HAL_IMUMC_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t))];
 * @param config           Used for config local/remote endpoint addr.
 * @retval kStatus_HAL_ImumcSuccess IMUMC module initialize succeed.
 */
hal_imumc_status_t HAL_ImumcInit(hal_imumc_handle_t handle, hal_imumc_config_t *config);

/*!
 * @brief DeInitilizate the IMUMC adapter module.
 *
 * @note This API should be called when not using the IMUMC adapter driver anymore.
 *
 * @param handle           IMUMC handle pointer.
 * @retval kStatus_HAL_ImumcSuccess IMUMC module deinitialize succeed.
 */
hal_imumc_status_t HAL_ImumcDeinit(hal_imumc_handle_t handle);

/*!
 * @brief Send data to another IMUMC module with timeout.
 *
 * This function will send a specified length of data to another core by IMUMC.
 *
 * @note This API should be called to send data.
 *
 * @param handle           IMUMC handle pointer.
 * @param data             Pointer to where the send data from.
 * @param length           The send data length.
 * @param timeout          Timeout in ms, 0 if nonblocking, IMUMC_WAITFOREVER for wait for forever.
 * @retval kStatus_HAL_ImumcSuccess IMUMC send data succeed.
 */
hal_imumc_status_t HAL_ImumcSendTimeout(hal_imumc_handle_t handle, uint8_t *data, uint32_t length, uint32_t timeout);

/*!
 * @brief Send data to another IMUMC module.
 *
 * This function will send a specified length of data to another core by IMUMC.
 *
 * @note This API should be called to send data.
 *
 * @param handle           IMUMC handle pointer.
 * @param data             Pointer to where the send data from.
 * @param length           The send data length.
 * @retval kStatus_HAL_ImumcSuccess IMUMC send data succeed.
 */
hal_imumc_status_t HAL_ImumcSend(hal_imumc_handle_t handle, uint8_t *data, uint32_t length);

/*!
 * @brief Allocates the tx buffer for message payload with timeout.
 *
 * This API can only be called at process context to get the tx buffer in vring. By this way, the
 * application can directly put its message into the vring tx buffer without copy from an application buffer.
 * It is the application responsibility to correctly fill the allocated tx buffer by data and passing correct
 * parameters to the imumc_lite_send_nocopy() function to perform data no-copy-send mechanism.
 *
 *
 * @param handle           IMUMC handle pointer.
 * @param size             The send data length.
 * @param timeout          Timeout in ms, 0 if nonblocking, IMUMC_WAITFOREVER for wait for forever.
 * @retval The tx buffer address on success and RL_NULL on failure.
 */
void *HAL_ImumcAllocTxBufferTimeout(hal_imumc_handle_t handle, uint32_t size, uint32_t timeout);

/*!
 * @brief Allocates the tx buffer for message payload.
 *
 * This API can only be called at process context to get the tx buffer in vring. By this way, the
 * application can directly put its message into the vring tx buffer without copy from an application buffer.
 * It is the application responsibility to correctly fill the allocated tx buffer by data and passing correct
 * parameters to the imumc_lite_send_nocopy() function to perform data no-copy-send mechanism.
 *
 *
 * @param handle           IMUMC handle pointer.
 * @param size           The send data length.
 * @retval The tx buffer address on success and RL_NULL on failure.
 */
void *HAL_ImumcAllocTxBuffer(hal_imumc_handle_t handle, uint32_t size);

/*!
 * @brief Send data with NoCopy to another IMUMC module.
 *
 * This function will send a specified length of data to another core by IMUMC.
 * This function sends txbuf of length len to the remote dst address,
 * and uses ept->addr as the source address.
 * The application has to take the responsibility for:
 *  1. tx buffer allocation (HAL_ImumcAllocTxBuffer())
 *  2. filling the data to be sent into the pre-allocated tx buffer
 *  3. not exceeding the buffer size when filling the data
 *  4. data cache coherency
 *
 * After the HAL_ImumcNoCopySend() function is issued the tx buffer is no more owned
 * by the sending task and must not be touched anymore unless the HAL_ImumcNoCopySend()
 * function fails and returns an error.
 *
 * @note This API should be called to send data.
 *
 * @param handle           IMUMC handle pointer.
 * @param data             Pointer to where the send data from.
 * @param length           The send data length.
 * @retval kStatus_HAL_ImumcSuccess IMUMC send data succeed.
 */
hal_imumc_status_t HAL_ImumcNoCopySend(hal_imumc_handle_t handle, uint8_t *data, uint32_t length);

/*!
 * @brief Releases the rx buffer for future reuse in vring.
 * This API can be called at process context when the
 * message in rx buffer is processed.
 *
 * @note The HAL_ImumcFreeRxBuffer need be called only if Imumc RX callback function return kStatus_HAL_RL_HOLD mode.
 *
 * @param handle           IMUMC handle pointer.
 * @param data             Pointer to where the received data from perr.

 *
 * @return Status of function execution, RL_SUCCESS on success.
 */
hal_imumc_status_t HAL_ImumcFreeRxBuffer(hal_imumc_handle_t handle, uint8_t *data);

/*!
 * @brief Install IMUMC rx callback.
 *
 * @note The function must be called because imumc adapter just support asynchronous receive mode
 *        should make sure the callback function is installed before the data received from peer soc,
 *        and the rx callback function will be called when the rx process complete.
 *
 *
 * @param handle    IMUMC handle pointer.
 * @retval kStatus_HAL_ImumcSuccess IMUMC install rx callback succeed.
 */
hal_imumc_status_t HAL_ImumcInstallRxCallback(hal_imumc_handle_t handle, imumc_rx_callback_t callback, void *param);

/*!
 * @brief Prepares to enter low power consumption.
 *
 * This function is used to prepare to enter low power consumption.
 *
 * @param handle           IMUMC handle pointer.
 * @retval kStatus_HAL_ImumcSuccess Successful operation.
 * @retval kStatus_HAL_ImumcError An error occurred.
 */
hal_imumc_status_t HAL_ImumcEnterLowpower(hal_imumc_handle_t handle);

/*!
 * @brief Prepares to exit low power consumption.
 *
 * This function is used to restore from low power consumption.
 *
 * @param handle           IMUMC handle pointer.
 * @retval kStatus_HAL_ImumcSuccess Successful operation.
 * @retval kStatus_HAL_ImumcError An error occurred.
 */
hal_imumc_status_t HAL_ImumcExitLowpower(hal_imumc_handle_t handle);

#ifndef IMU_TASK_PRIORITY
#if defined(__ZEPHYR__)
#define IMU_TASK_PRIORITY (3U)
#elif defined(CPU2)
#define IMU_TASK_PRIORITY (2U)
#else
#ifndef configMAX_PRIORITIES
#define IMU_TASK_PRIORITY (6U)
#else
#define IMU_TASK_PRIORITY                                                                                       \
    (PRIORITY_RTOS_TO_OSA((configMAX_PRIORITIES - 3)))
    /* makesure the calculated priority of HAL_ImuMainCpu13 task is the same with tcp/ip task(priority 2), \
            otherwise the throughput of udp rx will be very low */
#endif
#endif
#endif

#ifndef IMU_TASK_STACK_SIZE
#define IMU_TASK_STACK_SIZE (2048U)
#endif

#define IMU_MAX_MSG_CNT_LONG      (16U)
#define IMU_MAX_MSG_CNT_SHORT     (2U)
#define IMU_SYNC_MAGIC_PATTERN    (0xABCDEF89U)
#define IMU_TXQ_BUFSIZE           (32U) /*! must be power of 2 */
#define IMU_TXQ_BUFSIZE_THRESHOLD (8U)
#define IMU_TXQ_ENTRY_MASK        (IMU_TXQ_BUFSIZE - 1U)
#define IMU_EVENT_TRIGGERS        ((1U << kIMU_LinkMax) - 1U)
/*! @brief IMU message payload size. */
#define IMU_PAYLOAD_SIZE (8U)

#define IMUMC_EVENT_ENDPOINT_QUERY_RSP (1U << 0U)

#ifndef IMUMC_TXQ13_BUFSIZE
#define IMUMC_TXQ13_BUFSIZE (8U)
#endif

#ifndef IMUMC_TXQ23_BUFSIZE
#define IMUMC_TXQ23_BUFSIZE (16U)
#endif

#ifndef IMUMC_TXQ13_BUFLENGTH
#define IMUMC_TXQ13_BUFLENGTH (512U)
#endif

#ifndef IMUMC_TXQ23_BUFLENGTH
#define IMUMC_TXQ23_BUFLENGTH (264U)
#endif

#define IMUMC_TXQ13_ENTRY_MASK (IMUMC_TXQ13_BUFSIZE - 1U)
#define IMUMC_TXQ23_ENTRY_MASK (IMUMC_TXQ23_BUFSIZE - 1U)

#if defined(CONFIG_IMU_GDMA) && (CONFIG_IMU_GDMA == 1)
//#ifdef CONFIG_IMU_GDMA
#define IMU_GDMA_ENABLE (1U)

#else

#define IMU_GDMA_ENABLE (0U)
#endif
      
/*! @brief IMU initialization state. */
enum imu_init_state_t
{
    IMU_UNINITIALIZED,
    IMU_INITIALIZING,
    IMU_INITIALIZED
};

/*! @brief IMUMC type. */
typedef enum _imu_msg_type_t
{
    IMU_MSG_CONTROL,
    IMU_MSG_COMMAND,
    IMU_MSG_COMMAND_RESPONSE,
    IMU_MSG_EVENT,
    IMU_MSG_TX_DATA,
    IMU_MSG_RX_DATA,
    IMU_MSG_MULTI_TX_DATA,
    IMU_MSG_MULTI_RX_DATA,
    IMU_MSG_IMUMC,
    IMU_MSG_MAX,
    /* The header is 8 bits hence max types =255 */
} imu_msg_type_t;

/*! @brief IMUMC control message subtype. */
typedef enum _imu_ctrl_msg_subtype_t
{
    IMU_MSG_CONTROL_DEFAULT,
    IMU_MSG_CONTROL_SYNC,
    IMU_MSG_CONTROL_SYNC_ACK,
    IMU_MSG_CONTROL_ACK_FOR_UNLOCK,
    IMU_MSG_CONTROL_ERROR,
    IMU_MSG_CONTROL_SHUTDOWN,
    IMU_MSG_CONTROL_CMD_BUF_ADDR,
    IMU_MSG_CONTROL_EVT_ACK,
    IMU_MSG_CONTROL_COMMAND_RSP_ACK,
    IMU_MSG_CONTROL_TX_BUF_ADDR,
    IMU_MSG_CONTROL_FREE_RX_BUF,
    IMU_MSG_CONTROL_IMUMC_EPT_QUIRY,
    IMU_MSG_CONTROL_IMUMC_EPT_QUIRY_RSP,
    IMU_MSG_CONTROL_IMUMC_BUF_FREE,
    IMU_MSG_CONTROL_EVT_DUMP,
    IMU_MSG_CONTROL_MAX,
    /* The header is 8 bits hence max types =255 */
} imu_ctrl_msg_subtype_t;

/*! @brief IMU message header structure. */
typedef struct IMU_Hdr_t_
{
    volatile uint8_t type;
    volatile uint8_t sub_type; // user of icc module needs to define own msg sub_types
    volatile uint8_t seq_num;  // may be used by user of icc module for own purpose
    volatile uint8_t length;   // length of Payload in bytes
} IMU_Hdr_t;

/*! @brief IMU message structure. */
typedef struct IMU_Msg_t_
{
    IMU_Hdr_t Hdr;
    volatile uint32_t PayloadPtr[IMU_PAYLOAD_SIZE];
} IMU_Msg_t;

/*! @brief IMU message wrapper 1. */
typedef struct IMU_Msg_Wrapper_long_t_
{
    volatile uint32_t magic[2];
    IMU_Msg_t msgQ[2][IMU_MAX_MSG_CNT_LONG];
} IMU_Msg_Wrapper_long_t;

/*! @brief IMU message wrapper 2. */
typedef struct IMU_Msg_Wrapper_short_t_
{
    volatile uint32_t magic[2];
    IMU_Msg_t msgQ[2][IMU_MAX_MSG_CNT_SHORT];
} IMU_Msg_Wrapper_short_t;

/* imumc_std_hdr contains a reserved field,
 * this implementation of IMUMC uses this reserved
 * field to hold the idx and totlen of the buffer
 * not being returned to the vring in the receive
 * callback function. This way, the no-copy API
 * can use this field to return the buffer later.
 */
struct imumc_hdr_reserved
{
    uint16_t rfu; /* reserved for future usage */
    uint16_t idx;
};

/*!
 * Common header for all imumc messages.
 * Every message sent/received on the imumc bus begins with this header.
 */
struct imumc_std_hdr
{
    uint32_t src;                       /*!< source endpoint address */
    uint32_t dst;                       /*!< destination endpoint address */
    struct imumc_hdr_reserved reserved; /*!< reserved for future use */
    uint16_t len;                       /*!< length of payload (in bytes) */
    uint16_t flags;                     /*!< message flags */
};

/*!
 * Common message structure.
 * Contains the header and the payload.
 */
struct imumc_std_msg
{
    struct imumc_std_hdr hdr; /*!< Imumc message header */
    uint32_t data;            /*!< bytes of message payload data */
};

typedef hal_imumc_status_t (*imu_callback_t)(IMU_Msg_t *pImuMsg, uint32_t length);

/*! @brief IMU interrupt callback */
typedef void (*imu_irq_callback_t)(void);

/*! @brief Checks whether a tx buffer queue is empty */
#define IS_WLAN_TXBQ_EMPTY(q) ((q)->writeIndex == (q)->readIndex)

/*! @brief Checks whether a tx buffer queue is full */
#define IS_WLAN_TXBQ_FULL(q)                                                  \
    ((((q)->writeIndex & (1UL << 31U)) != ((q)->readIndex & (1UL << 31U))) && \
     (((q)->writeIndex & IMU_TXQ_ENTRY_MASK) == ((q)->readIndex & IMU_TXQ_ENTRY_MASK)))

/*! @brief Increments write pointer*/
#define INCR_WLAN_TXBQ_WR_INDEX(q, n)                                \
    {                                                                \
        unsigned int wrapBit = (q)->writeIndex & (1UL << 31U);       \
        unsigned int val     = (q)->writeIndex & IMU_TXQ_ENTRY_MASK; \
        unsigned int newVal  = (val + (n)) & IMU_TXQ_ENTRY_MASK;     \
        if ((n) > 0U)                                                \
        {                                                            \
            if (newVal <= val)                                       \
            {                                                        \
                (q)->writeIndex = newVal | (wrapBit ^ (1UL << 31U)); \
            }                                                        \
            else                                                     \
            {                                                        \
                (q)->writeIndex = newVal | wrapBit;                  \
            }                                                        \
        }                                                            \
    }

/*! @brief Increments read pointer */
#define INCR_WLAN_TXBQ_RD_INDEX(q, n)                               \
    {                                                               \
        unsigned int wrapBit = (q)->readIndex & (1UL << 31U);       \
        unsigned int val     = (q)->readIndex & IMU_TXQ_ENTRY_MASK; \
        unsigned int newVal  = (val + (n)) & IMU_TXQ_ENTRY_MASK;    \
        if ((n) > 0U)                                               \
        {                                                           \
            if (newVal <= val)                                      \
            {                                                       \
                (q)->readIndex = newVal | (wrapBit ^ (1UL << 31U)); \
            }                                                       \
            else                                                    \
            {                                                       \
                (q)->readIndex = newVal | wrapBit;                  \
            }                                                       \
        }                                                           \
    }

/*! @brief Checks whether a tx buffer queue is empty */
#define IS_IMUMC_TXBQ13_EMPTY(q) ((q)->writeIndex == (q)->readIndex)
#define IS_IMUMC_TXBQ23_EMPTY(q) ((q)->writeIndex == (q)->readIndex)

/*! @brief Checks whether a tx buffer queue is full */
#define IS_IMUMC_TXBQ13_FULL(q)                                               \
    ((((q)->writeIndex & (1UL << 31U)) != ((q)->readIndex & (1UL << 31U))) && \
     (((q)->writeIndex & IMUMC_TXQ13_ENTRY_MASK) == ((q)->readIndex & IMUMC_TXQ13_ENTRY_MASK)))
#define IS_IMUMC_TXBQ23_FULL(q)                                               \
    ((((q)->writeIndex & (1UL << 31U)) != ((q)->readIndex & (1UL << 31U))) && \
     (((q)->writeIndex & IMUMC_TXQ23_ENTRY_MASK) == ((q)->readIndex & IMUMC_TXQ23_ENTRY_MASK)))

/*! @brief Increments write pointer*/
#define INCR_IMUMC_TXBQ13_WR_INDEX(q, n)                                 \
    {                                                                    \
        unsigned int wrapBit = (q)->writeIndex & (1UL << 31U);           \
        unsigned int val     = (q)->writeIndex & IMUMC_TXQ13_ENTRY_MASK; \
        unsigned int newVal  = (val + (n)) & IMUMC_TXQ13_ENTRY_MASK;     \
        if ((n) > 0U)                                                    \
        {                                                                \
            if (newVal <= val)                                           \
            {                                                            \
                (q)->writeIndex = newVal | (wrapBit ^ (1UL << 31U));     \
            }                                                            \
            else                                                         \
            {                                                            \
                (q)->writeIndex = newVal | wrapBit;                      \
            }                                                            \
        }                                                                \
    }

#define INCR_IMUMC_TXBQ23_WR_INDEX(q, n)                                 \
    {                                                                    \
        unsigned int wrapBit = (q)->writeIndex & (1UL << 31U);           \
        unsigned int val     = (q)->writeIndex & IMUMC_TXQ23_ENTRY_MASK; \
        unsigned int newVal  = (val + (n)) & IMUMC_TXQ23_ENTRY_MASK;     \
        if ((n) > 0U)                                                    \
        {                                                                \
            if (newVal <= val)                                           \
            {                                                            \
                (q)->writeIndex = newVal | (wrapBit ^ (1UL << 31U));     \
            }                                                            \
            else                                                         \
            {                                                            \
                (q)->writeIndex = newVal | wrapBit;                      \
            }                                                            \
        }                                                                \
    }

/*! @brief Increments read pointer */
#define INCR_IMUMC_TXBQ13_RD_INDEX(q, n)                                \
    {                                                                   \
        unsigned int wrapBit = (q)->readIndex & (1UL << 31U);           \
        unsigned int val     = (q)->readIndex & IMUMC_TXQ13_ENTRY_MASK; \
        unsigned int newVal  = (val + (n)) & IMUMC_TXQ13_ENTRY_MASK;    \
        if ((n) > 0U)                                                   \
        {                                                               \
            if (newVal <= val)                                          \
            {                                                           \
                (q)->readIndex = newVal | (wrapBit ^ (1UL << 31U));     \
            }                                                           \
            else                                                        \
            {                                                           \
                (q)->readIndex = newVal | wrapBit;                      \
            }                                                           \
        }                                                               \
    }

#define INCR_IMUMC_TXBQ23_RD_INDEX(q, n)                                \
    {                                                                   \
        unsigned int wrapBit = (q)->readIndex & (1UL << 31U);           \
        unsigned int val     = (q)->readIndex & IMUMC_TXQ23_ENTRY_MASK; \
        unsigned int newVal  = (val + (n)) & IMUMC_TXQ23_ENTRY_MASK;    \
        if ((n) > 0U)                                                   \
        {                                                               \
            if (newVal <= val)                                          \
            {                                                           \
                (q)->readIndex = newVal | (wrapBit ^ (1UL << 31U));     \
            }                                                           \
            else                                                        \
            {                                                           \
                (q)->readIndex = newVal | wrapBit;                      \
            }                                                           \
        }                                                               \
    }

typedef struct _hal_imumc_rx_state
{
    imumc_rx_callback_t callback;
    void *param;
} hal_imumc_rx_state_t;

/*! @brief imumc state structure. */
typedef struct _hal_imumc_state
{
    LIST_ELEM_st eptLink;
    uint32_t local_addr;
    uint32_t remote_addr;
    uint8_t imuLink;
    bool eptLinkIsReady;
    hal_imumc_rx_state_t rx;
} hal_imumc_state_t;

/*! @brief imumc ept buf structure. */
typedef struct _hal_imumc_ept_uf
{
    LIST_ELEM_st eptLink;
    uint32_t addr;
} hal_imumc_ept_buf_t;

/*! @brief IMU WLAN Tx buffer control structure */
typedef struct IMU_WLAN_TXQ_CTRL_st
{
    uint32_t writeIndex;
    uint32_t readIndex;
    uint32_t txBufQue[IMU_TXQ_BUFSIZE];
} IMU_WLAN_TXQ_CTRL_st;

/*! @brief IMUMC Tx buffer control structure */
typedef struct IMUMC_TXQ_CTRL_st
{
    uint32_t writeIndex;
    uint32_t readIndex;
    uint32_t txBufQue[IMUMC_TXQ23_BUFSIZE];
} IMUMC_TXQ_CTRL_st;

/*! @brief imu handle structure. */
typedef struct _hal_imu_handle_t
{
    /*! Word 0 */
    uint8_t imuLink;
    uint8_t imuSyncState; /*！ 0：Link Off, 1:Link Ongoing, 2: Link Up */
    uint8_t freeTxMportBufCnt;
    uint8_t imuMsgBufIdx;
    /*! Word 1 */
    bool cmd_buffer_available;
    bool cmd_response_buffer_available;
    bool rsvdbool1;
    bool rsvdbool2;
    /*! Word 2 */
    uint32_t *cmd_buffer;
    /*! Word 3 */
    IMU_WLAN_TXQ_CTRL_st wlanTxqCtl;
    /*! Word 37 */
    uint32_t imuMsgBuf[IMU_PAYLOAD_SIZE];
    /*! Word 45 */
    LIST_ELEM_st eptList;
    /*! Word 47 */
    IMUMC_TXQ_CTRL_st imumcTxqCtl;
    /*! Word 57 */
    imu_callback_t imuHandler[IMU_MSG_MAX];
} hal_imu_handle_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */

/*!
 * @brief Add wlan Tx packet for sending multiple Tx packets within one message.
 *
 * This function is used to add a wlan Tx packet for IMU_MSG_MULTI_TX_DATA.
 *
 * @param imuLink           IMU link ID.
 * @param txBuf             Tx buffer to be sent.
 * @param length            Length of Tx data.
 * @retval kStatus_HAL_ImumcSuccess or kStatus_HAL_ImumcError.
 */
hal_imumc_status_t HAL_ImuAddWlanTxPacket(uint8_t imuLink, uint8_t *txBuf, uint32_t length);

/*!
 * @brief Add wlan Tx packet for sending multiple Tx packets within one message.
 *
 * This function is used to add a wlan Tx packet with callback function for IMU_MSG_MULTI_TX_DATA.
 *
 * @param imuLink           IMU link ID.
 * @param txBuf             Tx buffer to be sent.
 * @param length            Length of Tx data.
 * @param cb                Callback function to add packet.
 * @retval kStatus_HAL_ImumcSuccess or kStatus_HAL_ImumcError.
 */
hal_imumc_status_t HAL_ImuAddWlanTxPacketExt(uint8_t imuLink,
                                             uint8_t *txBuf,
                                             uint32_t length,
                                             void (*cb)(void *destAddr, void *srcAddr, uint32_t len));

/*!
 * @brief Install wlan callback.
 *
 * This function is used to imstall wlan callback
 *
 * @param imuLink        IMU link ID.
 * @param callback       Callback to be installed.
 * @param type           Callback type.
 * @retval kStatus_HAL_ImumcSuccess or kStatus_HAL_ImumcError.
 */
hal_imumc_status_t HAL_ImuInstallCallback(uint8_t imuLink, imu_callback_t callback, uint8_t type);

/*!
 * @brief Install imu interrupt callback.
 *
 * This function is used to install imu interrupt callback
 *
 * @param imuLink        IMU link ID.
 * @param callback       Callback to be installed.
 * @retval kStatus_HAL_ImumcSuccess or kStatus_HAL_ImumcError.
 */
hal_imumc_status_t HAL_ImuInstallIrqCallback(uint8_t imuLink, imu_irq_callback_t callback);

/*!
 * @brief Send wlan command packet.
 *
 * This function is used to send wlan command packet
 *
 * @param imuLink        IMU link ID.
 * @param cmdBuf         Command buffer to be sent.
 * @param length         Length of command data.
 * @retval kStatus_HAL_ImumcSuccess or kStatus_HAL_ImumcError.
 */
hal_imumc_status_t HAL_ImuSendCommand(uint8_t imuLink, uint8_t *cmdBuf, uint32_t length);

/*!
 * @brief Add wlan Tx packet for sending one Tx packet within one message.
 *
 * This function is used to send a wlan Tx packet.
 *
 * @param imuLink           IMU link ID.
 * @param txBuf             Tx buffer to be sent.
 * @param length            Length of Tx data.
 * @retval kStatus_HAL_ImumcSuccess or kStatus_HAL_ImumcError.
 */
hal_imumc_status_t HAL_ImuSendTxData(uint8_t imuLink, uint8_t *txBuf, uint32_t length);

/*!
 * @brief Send multiple Tx packets within one message.
 *
 * This function is used to send multiple wlan Tx packets within one IMU message,
 * which have been added by HAL_ImuAddWlanTxPacket already.
 *
 * @param imuLink           IMU link ID.
 * @retval kStatus_HAL_ImumcSuccess or kStatus_HAL_ImumcError.
 */
hal_imumc_status_t HAL_ImuSendMultiTxData(uint8_t imuLink);

/*!
 * @brief Check if IMU link is up.
 *
 * This function is used to check if IMU link is up.
 *
 * @param imuLink        IMU link ID.
 * @retval kStatus_HAL_ImumcSuccess for IMU link up or kStatus_HAL_ImumcError for IMU link not up.
 */
hal_imumc_status_t HAL_ImuLinkIsUp(uint8_t imuLink);

/*!
 * @brief Start IMU link
 *
 * This function is used to start specified IMU link.
 *
 * @param imuLink        IMU link ID.
 * @retval kStatus_HAL_ImumcSuccess or kStatus_HAL_ImumcError.
 */
hal_imumc_status_t HAL_ImuInit(imu_link_t link);

/*!
 * @brief Stop IMU link
 *
 * This function is used to stop specified IMU link.
 *
 * @param imuLink        IMU link ID.
 * @param flag           flag with bits to control.
 *        flag bit0: control if need send imu msg to fw (imu tx return /imu shutdown)
 *        flag bit1: control if need destroy ImuTask and ImuQFlagsRef
 * @retval kStatus_HAL_ImumcSuccess or kStatus_HAL_ImumcError.
 */
hal_imumc_status_t HAL_ImuDeinit(imu_link_t link, uint32_t flag);

/*!
 * @brief Return all tx buffer.
 *
 * This function is used to return all tx buffer to CPU1.
 *
 * @param imuLink              IMU link ID.
 * @retval kStatus_HAL_ImumcSuccess or kStatus_HAL_ImumcError.
 */
hal_imumc_status_t HAL_ImuReturnAllTxBuf(imu_link_t link);

/*!
 * @brief Receive IMU message.
 *
 * This function is used to receive and process IMU message
 *
 * @param imuLink              IMU link ID.
 * @retval kStatus_HAL_ImumcSuccess or kStatus_HAL_ImumcError.
 */
hal_imumc_status_t HAL_ImuReceive(uint8_t imuLink);

/*!
 * @brief Check if tx buffer queue empty.
 *
 * This function is used to check if tx buffer queue empty
 *
 * @param imuLink              IMU link ID.
 * @retval TRUE or FALSE.
 */
bool HAL_ImuIsTxBufQueueEmpty(uint8_t imuLink);

#if defined(IMU_GDMA_ENABLE) && (IMU_GDMA_ENABLE == 1)
/*!
 * @brief Copy data by GDMA.
 *
 * This function is used to copy data by GDMA between shared memory and local memory.
 *
 * @param destAddr          Source address of data.
 * @param srcAddr           Destination address of data.
 * @param len               Data Length.
 */
void HAL_ImuGdmaCopyData(void *destAddr, void *srcAddr, uint32_t len);

/*! @}*/
#endif

#ifdef CONFIG_FW_DUMP_EVENT
/*!
 * @brief Send fw dump event ack.
 *
 * This function is used to Send fw dump event ack to fw, When the event data process is completed.
 *
 * @param imuLink              IMU link ID.
 * @param rxBuf                Rx buffer to be sent.
 * @param length               Length of Rx buffer
 * @retval kStatus_HAL_ImumcSuccess or kStatus_HAL_ImumcError.
 */
hal_imumc_status_t HAL_ImuSendEventDumpAck(imu_link_t link, uint8_t *rxBuf, uint8_t length);
#endif

/*!
 * @brief Create imu task mutex.
 *
 * This function creates a imu task mutex.
 *
 * @retval kStatus_HAL_ImumcSuccess or kStatus_HAL_ImumcError.
 */
hal_imumc_status_t HAL_ImuCreateTaskLock(void);

/*!
 * @brief Delete imu task mutex.
 *
 * This function deletes the imu task mutex.
 */
void HAL_ImuDeleteTaskLock(void);

/*!
 * @brief Acquire imu task mutex.
 *
 * This function acquires the imu task mutex. Only one thread can acquire the mutex at any
 * given time.
 *
 * @retval kStatus_HAL_ImumcSuccess or kStatus_HAL_ImumcError.
 */
hal_imumc_status_t HAL_ImuGetTaskLock(void);

/*!
 * @brief Release imu task mutex.
 *
 * This function releases the imu task mutex.
 *
 * @retval kStatus_HAL_ImumcSuccess or kStatus_HAL_ImumcError.
 */
hal_imumc_status_t HAL_ImuPutTaskLock(void);

/*!
 * @brief Reset imu readIndex and writeIndex to 0.
 *
 * This function reset the imu txq.
 */
void HAL_ImuResetWlanTxq(uint8_t imuLink);



/*!
 * @addtogroup IMUMC_Adapter
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief IMUMC Msaster/Remote role definition (0 - Master, 1 - Remote) */
#ifndef HAL_IMUMC_SELECT_ROLE
#define HAL_IMUMC_SELECT_ROLE (1U)
#endif

/*! @brief IMUMC handle size definition */
#define HAL_IMUMC_HANDLE_SIZE (52U)

#ifndef REMOTE_CORE_BOOT_ADDRESS
#define REMOTE_CORE_BOOT_ADDRESS (0x01000000U)
#endif

#ifndef MAX_EP_COUNT
#define MAX_EP_COUNT (5U)
#endif

#define IMUMC_WAITFOREVER (0xFFFFFFFFU)

/*!
 * @brief Defines the imumc handle
 *
 * This macro is used to define a 4 byte aligned imumc handle.
 * Then use "(hal_imumc_handle_t)name" to get the imumc handle.
 *
 * The macro should be global and could be optional. You could also define imumc handle by yourself.
 *
 * This is an example,
 * @code
 * IMUMC_HANDLE_DEFINE(imumcHandle);
 * @endcode
 *
 * @param name The name string of the imumc handle.
 */
#define IMUMC_HANDLE_DEFINE(name) uint32_t name[((HAL_IMUMC_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t))]

/*!
 * @brief Initializes the IMUMC adapter module for dual core communication.
 *
 * @note This API should be called at the beginning of the application using the IMUMC adapter driver.
 *
 * @retval kStatus_HAL_ImumcSuccess IMUMC module initialize succeed.
 */
hal_imumc_status_t HAL_ImumcMcmgrInit(void);

/*!
 * @brief Initializes the IMUMC adapter for IMUMC channel configure.
 *
 * @note This API should be called to software IMUMC communication configure, and it be
 * called whenever application need it.
 *
 * @param handle Pointer to point to a memory space of size #HAL_IMUMC_HANDLE_SIZE allocated by the caller.
 * The handle should be 4 byte aligned, because unaligned access doesn't be supported on some devices.
 * You can define the handle in the following two ways:
 * #IMUMC_HANDLE_DEFINE(handle);
 * or
 * uint32_t handle[((HAL_IMUMC_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t))];
 * @param config           Used for config local/remote endpoint addr.
 * @retval kStatus_HAL_ImumcSuccess IMUMC module initialize succeed.
 */
hal_imumc_status_t HAL_ImumcInit(hal_imumc_handle_t handle, hal_imumc_config_t *config);

/*!
 * @brief DeInitilizate the IMUMC adapter module.
 *
 * @note This API should be called when not using the IMUMC adapter driver anymore.
 *
 * @param handle           IMUMC handle pointer.
 * @retval kStatus_HAL_ImumcSuccess IMUMC module deinitialize succeed.
 */
hal_imumc_status_t HAL_ImumcDeinit(hal_imumc_handle_t handle);

/*!
 * @brief Send data to another IMUMC module with timeout.
 *
 * This function will send a specified length of data to another core by IMUMC.
 *
 * @note This API should be called to send data.
 *
 * @param handle           IMUMC handle pointer.
 * @param data             Pointer to where the send data from.
 * @param length           The send data length.
 * @param timeout          Timeout in ms, 0 if nonblocking, IMUMC_WAITFOREVER for wait for forever.
 * @retval kStatus_HAL_ImumcSuccess IMUMC send data succeed.
 */
hal_imumc_status_t HAL_ImumcSendTimeout(hal_imumc_handle_t handle, uint8_t *data, uint32_t length, uint32_t timeout);

/*!
 * @brief Send data to another IMUMC module.
 *
 * This function will send a specified length of data to another core by IMUMC.
 *
 * @note This API should be called to send data.
 *
 * @param handle           IMUMC handle pointer.
 * @param data             Pointer to where the send data from.
 * @param length           The send data length.
 * @retval kStatus_HAL_ImumcSuccess IMUMC send data succeed.
 */
hal_imumc_status_t HAL_ImumcSend(hal_imumc_handle_t handle, uint8_t *data, uint32_t length);

/*!
 * @brief Allocates the tx buffer for message payload with timeout.
 *
 * This API can only be called at process context to get the tx buffer in vring. By this way, the
 * application can directly put its message into the vring tx buffer without copy from an application buffer.
 * It is the application responsibility to correctly fill the allocated tx buffer by data and passing correct
 * parameters to the imumc_lite_send_nocopy() function to perform data no-copy-send mechanism.
 *
 *
 * @param handle           IMUMC handle pointer.
 * @param size             The send data length.
 * @param timeout          Timeout in ms, 0 if nonblocking, IMUMC_WAITFOREVER for wait for forever.
 * @retval The tx buffer address on success and RL_NULL on failure.
 */
void *HAL_ImumcAllocTxBufferTimeout(hal_imumc_handle_t handle, uint32_t size, uint32_t timeout);

/*!
 * @brief Allocates the tx buffer for message payload.
 *
 * This API can only be called at process context to get the tx buffer in vring. By this way, the
 * application can directly put its message into the vring tx buffer without copy from an application buffer.
 * It is the application responsibility to correctly fill the allocated tx buffer by data and passing correct
 * parameters to the imumc_lite_send_nocopy() function to perform data no-copy-send mechanism.
 *
 *
 * @param handle           IMUMC handle pointer.
 * @param size           The send data length.
 * @retval The tx buffer address on success and RL_NULL on failure.
 */
void *HAL_ImumcAllocTxBuffer(hal_imumc_handle_t handle, uint32_t size);

/*!
 * @brief Send data with NoCopy to another IMUMC module.
 *
 * This function will send a specified length of data to another core by IMUMC.
 * This function sends txbuf of length len to the remote dst address,
 * and uses ept->addr as the source address.
 * The application has to take the responsibility for:
 *  1. tx buffer allocation (HAL_ImumcAllocTxBuffer())
 *  2. filling the data to be sent into the pre-allocated tx buffer
 *  3. not exceeding the buffer size when filling the data
 *  4. data cache coherency
 *
 * After the HAL_ImumcNoCopySend() function is issued the tx buffer is no more owned
 * by the sending task and must not be touched anymore unless the HAL_ImumcNoCopySend()
 * function fails and returns an error.
 *
 * @note This API should be called to send data.
 *
 * @param handle           IMUMC handle pointer.
 * @param data             Pointer to where the send data from.
 * @param length           The send data length.
 * @retval kStatus_HAL_ImumcSuccess IMUMC send data succeed.
 */
hal_imumc_status_t HAL_ImumcNoCopySend(hal_imumc_handle_t handle, uint8_t *data, uint32_t length);

/*!
 * @brief Releases the rx buffer for future reuse in vring.
 * This API can be called at process context when the
 * message in rx buffer is processed.
 *
 * @note The HAL_ImumcFreeRxBuffer need be called only if Imumc RX callback function return kStatus_HAL_RL_HOLD mode.
 *
 * @param handle           IMUMC handle pointer.
 * @param data             Pointer to where the received data from perr.

 *
 * @return Status of function execution, RL_SUCCESS on success.
 */
hal_imumc_status_t HAL_ImumcFreeRxBuffer(hal_imumc_handle_t handle, uint8_t *data);

/*!
 * @brief Install IMUMC rx callback.
 *
 * @note The function must be called because imumc adapter just support asynchronous receive mode
 *        should make sure the callback function is installed before the data received from peer soc,
 *        and the rx callback function will be called when the rx process complete.
 *
 *
 * @param handle    IMUMC handle pointer.
 * @retval kStatus_HAL_ImumcSuccess IMUMC install rx callback succeed.
 */
hal_imumc_status_t HAL_ImumcInstallRxCallback(hal_imumc_handle_t handle, imumc_rx_callback_t callback, void *param);

/*!
 * @brief Prepares to enter low power consumption.
 *
 * This function is used to prepare to enter low power consumption.
 *
 * @param handle           IMUMC handle pointer.
 * @retval kStatus_HAL_ImumcSuccess Successful operation.
 * @retval kStatus_HAL_ImumcError An error occurred.
 */
hal_imumc_status_t HAL_ImumcEnterLowpower(hal_imumc_handle_t handle);

/*!
 * @brief Prepares to exit low power consumption.
 *
 * This function is used to restore from low power consumption.
 *
 * @param handle           IMUMC handle pointer.
 * @retval kStatus_HAL_ImumcSuccess Successful operation.
 * @retval kStatus_HAL_ImumcError An error occurred.
 */
hal_imumc_status_t HAL_ImumcExitLowpower(hal_imumc_handle_t handle);

/*! @}*/

#if defined(__cplusplus)
}
#endif
/*! @}*/
#endif /* __FSL_ADAPTER_RFIMU_H__ */
