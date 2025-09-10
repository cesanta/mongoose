/*
 * Copyright 2020-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef FSL_IMU_H_
#define FSL_IMU_H_

#include "fsl_common.h"

/*!
 * @addtogroup imu
 * @{
 */

/******************************************************************************
 * Definitions
 *****************************************************************************/

/*! @name Driver version */
/*! @{ */
/*! @brief IMU driver version. */
#define FSL_IMU_DRIVER_VERSION (MAKE_VERSION(2, 1, 1))
/*! @} */

#define IMU_MSG_FIFO_STATUS_MSG_FIFO_LOCKED_MASK      (1UL)
#define IMU_MSG_FIFO_STATUS_MSG_FIFO_ALMOST_FULL_MASK (1UL << 1U)
#define IMU_MSG_FIFO_STATUS_MSG_FIFO_FULL_MASK        (1UL << 2U)
#define IMU_MSG_FIFO_STATUS_MSG_FIFO_EMPTY_MASK       (1UL << 3U)
#define IMU_MSG_FIFO_STATUS_MSG_COUNT_MASK            (0x1FUL << 4U)
#define IMU_MSG_FIFO_STATUS_MSG_COUNT_SHIFT           (4U)
#define IMU_MSG_FIFO_STATUS_WR_PTR_MASK               (0xFUL << 16U)
#define IMU_MSG_FIFO_STATUS_WR_PTR_SHIFT              (16U)
#define IMU_MSG_FIFO_STATUS_RD_PTR_MASK               (0xFUL << 20U)
#define IMU_MSG_FIFO_STATUS_RD_PTR_SHIFT              (20U)

#define IMU_MSG_FIFO_CNTL_MSG_RDY_INT_CLR_MASK      (1UL << 0U)
#define IMU_MSG_FIFO_CNTL_SP_AV_INT_CLR_MASK        (1UL << 1U)
#define IMU_MSG_FIFO_CNTL_FIFO_FLUSH_MASK           (1UL << 16U)
#define IMU_MSG_FIFO_CNTL_WAIT_FOR_ACK_MASK         (1UL << 17U)
#define IMU_MSG_FIFO_CNTL_FIFO_FULL_WATERMARK_MASK  (0x0FUL << 20U)
#define IMU_MSG_FIFO_CNTL_FIFO_FULL_WATERMARK_SHIFT (20U)
#define IMU_MSG_FIFO_CNTL_FIFO_FULL_WATERMARK(x)    (((uint32_t)(x)) << 20U)

/* Write message directly. */
#define IMU_WR_MSG(link, msg) (((IMU_Type *)(uintptr_t)IMU_CUR_CPU_BASE(link))->WR_MSG = (msg))

/* Read message directly. */
#define IMU_RD_MSG(link) (((IMU_Type *)(uintptr_t)IMU_CUR_CPU_BASE(link))->RD_MSG)

/* Check whether RX FIFO is locked. */
#define IMU_RX_FIFO_LOCKED(link) \
    (0UL !=                      \
     (((IMU_Type *)(uintptr_t)IMU_PEER_CPU_BASE(link))->MSG_FIFO_STATUS & IMU_MSG_FIFO_STATUS_MSG_FIFO_LOCKED_MASK))

/* Check whether TX FIFO is locked. */
#define IMU_TX_FIFO_LOCKED(link) \
    (0UL !=                      \
     (((IMU_Type *)(uintptr_t)IMU_CUR_CPU_BASE(link))->MSG_FIFO_STATUS & IMU_MSG_FIFO_STATUS_MSG_FIFO_LOCKED_MASK))

/* Check whether TX FIFO is almost full. */
#define IMU_TX_FIFO_ALMOST_FULL(link)                                           \
    (0UL != (((IMU_Type *)(uintptr_t)IMU_CUR_CPU_BASE(link))->MSG_FIFO_STATUS & \
             IMU_MSG_FIFO_STATUS_MSG_FIFO_ALMOST_FULL_MASK))

/*! @brief Get Rx FIFO empty status. */
#define IMU_RX_FIFO_EMPTY(link) \
    (0UL !=                     \
     (((IMU_Type *)(uintptr_t)IMU_PEER_CPU_BASE(link))->MSG_FIFO_STATUS & IMU_MSG_FIFO_STATUS_MSG_FIFO_EMPTY_MASK))

/* Lock the TX FIFO. */
#define IMU_LOCK_TX_FIFO(link) \
    (((IMU_Type *)(uintptr_t)IMU_CUR_CPU_BASE(link))->MSG_FIFO_CNTL |= IMU_MSG_FIFO_CNTL_WAIT_FOR_ACK_MASK)

/* Unlock the TX FIFO. */
#define IMU_UNLOCK_TX_FIFO(link) \
    (((IMU_Type *)(uintptr_t)IMU_CUR_CPU_BASE(link))->MSG_FIFO_CNTL &= ~IMU_MSG_FIFO_CNTL_WAIT_FOR_ACK_MASK)

/* Get message count in RX FIFO. */
#define IMU_RX_FIFO_MSG_COUNT(link)                                                                              \
    ((((IMU_Type *)(uintptr_t)IMU_PEER_CPU_BASE(link))->MSG_FIFO_STATUS & IMU_MSG_FIFO_STATUS_MSG_COUNT_MASK) >> \
     IMU_MSG_FIFO_STATUS_MSG_COUNT_SHIFT)

/* Get message count in TX FIFO. */
#define IMU_TX_FIFO_MSG_COUNT(link)                                                                             \
    ((((IMU_Type *)(uintptr_t)IMU_CUR_CPU_BASE(link))->MSG_FIFO_STATUS & IMU_MSG_FIFO_STATUS_MSG_COUNT_MASK) >> \
     IMU_MSG_FIFO_STATUS_MSG_COUNT_SHIFT)

/* Get message count from RX FIFO status register value. */
#define IMU_RX_FIFO_MSG_COUNT_FROM_STATUS(rxFifoStatus) \
    (((rxFifoStatus)&IMU_MSG_FIFO_STATUS_MSG_COUNT_MASK) >> IMU_MSG_FIFO_STATUS_MSG_COUNT_SHIFT)

/* Get RX FIFO lock status from RX FIFO status register value. */
#define IMU_RX_FIFO_LOCKED_FROM_STATUS(rxFifoStatus) (0UL != ((rxFifoStatus)&IMU_MSG_FIFO_STATUS_MSG_FIFO_LOCKED_MASK))

/* TX FIFO status register. */
#define IMU_TX_FIFO_STATUS(link) (((IMU_Type *)(uintptr_t)IMU_CUR_CPU_BASE(link))->MSG_FIFO_STATUS)

/* RX FIFO status register. */
#define IMU_RX_FIFO_STATUS(link) (((IMU_Type *)(uintptr_t)IMU_PEER_CPU_BASE(link))->MSG_FIFO_STATUS)

/* RX FIFO control register. */
#define IMU_TX_FIFO_CNTL(link) (((IMU_Type *)(uintptr_t)IMU_CUR_CPU_BASE(link))->MSG_FIFO_CNTL)

/*! @brief IMU driver returned error value. */
#define IMU_ERR_TX_FIFO_LOCKED (-1L)

/*! @brief Maximum message numbers in FIFO. */
#define IMU_MSG_FIFO_MAX_COUNT 16U

/*! @brief Maximum message FIFO warter mark. */
#define IMU_MAX_MSG_FIFO_WATER_MARK (IMU_MSG_FIFO_MAX_COUNT - 1U)

/* IMP: ICC_MAX_MSG_CNT should be power of 2 */
#define IMU_FIFO_SW_WRAPAROUND(ptr) ((ptr) & (IMU_MSG_FIFO_MAX_COUNT - 1U))

#define IMU_WR_PTR(link)                                                                   \
    IMU_FIFO_SW_WRAPAROUND((IMU_TX_FIFO_STATUS(link) & IMU_MSG_FIFO_STATUS_WR_PTR_MASK) >> \
                           IMU_MSG_FIFO_STATUS_WR_PTR_SHIFT)

#define IMU_RD_PTR(link)                                                                   \
    IMU_FIFO_SW_WRAPAROUND((IMU_RX_FIFO_STATUS(link) & IMU_MSG_FIFO_STATUS_RD_PTR_MASK) >> \
                           IMU_MSG_FIFO_STATUS_RD_PTR_SHIFT)

/*!
 * @brief IMU status flags.
 * @anchor _imu_status_flags
 */
enum _imu_status_flags
{
    kIMU_TxFifoEmpty      = IMU_MSG_FIFO_STATUS_MSG_FIFO_EMPTY_MASK,
    kIMU_TxFifoFull       = IMU_MSG_FIFO_STATUS_MSG_FIFO_FULL_MASK,
    kIMU_TxFifoAlmostFull = IMU_MSG_FIFO_STATUS_MSG_FIFO_ALMOST_FULL_MASK,
    kIMU_TxFifoLocked     = IMU_MSG_FIFO_STATUS_MSG_FIFO_LOCKED_MASK,
    kIMU_RxFifoEmpty      = IMU_MSG_FIFO_STATUS_MSG_FIFO_EMPTY_MASK << 8U,
    kIMU_RxFifoFull       = IMU_MSG_FIFO_STATUS_MSG_FIFO_FULL_MASK << 8U,
    kIMU_RxFifoAlmostFull = IMU_MSG_FIFO_STATUS_MSG_FIFO_ALMOST_FULL_MASK << 8U,
    kIMU_RxFifoLocked     = IMU_MSG_FIFO_STATUS_MSG_FIFO_LOCKED_MASK << 8U,
};

/*!
 * @brief IMU interrupt.
 * @anchor _imu_interrupts
 */
enum _imu_interrupts
{
    kIMU_RxMsgReadyInterrupt           = IMU_MSG_FIFO_CNTL_MSG_RDY_INT_CLR_MASK,
    kIMU_TxFifoSpaceAvailableInterrupt = IMU_MSG_FIFO_CNTL_SP_AV_INT_CLR_MASK,
};

/*! @brief IMU register structure. */
typedef struct
{
    volatile uint32_t WR_MSG;
    const volatile uint32_t RD_MSG;
    const volatile uint32_t MSG_FIFO_STATUS;
    volatile uint32_t MSG_FIFO_CNTL;
    const volatile uint32_t RD_MSG_DBG;
} IMU_Type;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name IMU initialization.
 * @{
 */

/*!
 * @brief Initializes the IMU module.
 *
 * This function sets IMU to initialized state, including:
 *
 *   - Flush the send FIFO.
 *   - Unlock the send FIFO.
 *   - Set the water mark to (IMU_MAX_MSG_FIFO_WATER_MARK)
 *   - Flush the read FIFO.
 *
 * @param link IMU link.
 * @retval kStatus_InvalidArgument The link is invalid.
 * @retval kStatus_Success Initialized successfully.
 */
status_t IMU_Init(imu_link_t link);

/*!
 * @brief De-initializes the IMU module.
 *
 * @param link IMU link.
 */
void IMU_Deinit(imu_link_t link);

/*! @} */

/*!
 * @name IMU Message
 * @{
 */

/*!
 * @brief Write one message to TX FIFO.
 *
 * This function writes message to the TX FIFO, user need to make sure
 * there is empty space in the TX FIFO, and TX FIFO not locked before
 * calling this function.
 *
 * @param link IMU link.
 * @param msg The message to send.
 */
static inline void IMU_WriteMsg(imu_link_t link, uint32_t msg)
{
    IMU_WR_MSG(link, msg);
}

/*!
 * @brief Read one message from RX FIFO.
 *
 * User need to make sure there is available message in the RX FIFO.
 *
 * @param link IMU link.
 * @return The message.
 */
static inline uint32_t IMU_ReadMsg(imu_link_t link)
{
    return IMU_RD_MSG(link);
}

/*!
 * @brief Blocking to send messages.
 *
 * This function blocks until all messages have been filled to TX FIFO.
 *
 * - If the TX FIFO is locked, this function returns IMU_ERR_TX_FIFO_LOCKED.
 * - If TX FIFO not locked, this function waits the available empty slot in TX FIFO,
 *   and fills the message to TX FIFO.
 * - To lock TX FIFO after filling all messages, set @p lockSendFifo to true.
 *
 * @param link IMU link.
 * @param msgs The messages to send.
 * @param msgCount Message count, one message is a 32-bit word.
 * @param lockSendFifo If set to true, the TX FIFO is locked after all messages filled to TX FIFO.
 * @return If TX FIFO is locked, this function returns IMU_ERR_TX_FIFO_LOCKED,
 * otherwise, this function returns the actual message count sent out, it equals @p msgCount
 * because this function is blocking function, it returns until all messages have been
 * filled into TX FIFO.
 */
int32_t IMU_SendMsgsBlocking(imu_link_t link, const uint32_t *msgs, int32_t msgCount, bool lockSendFifo);

/*!
 * @brief Try to send messages.
 *
 * This function is similar with @ref IMU_SendMsgsBlocking, the difference is,
 * this function tries to send as many as possible, if there is not enough
 * empty slot in TX FIFO, this function fills messages to available empty slots
 * and returns how many messages have been filled.
 *
 * - If the TX FIFO is locked, this function returns IMU_ERR_TX_FIFO_LOCKED.
 * - If TX FIFO not locked, this function fills messages to TX FIFO empty slot,
 *   and returns how many messages have been filled.
 * - If @p lockSendFifo is set to true, TX FIFO is locked after all messages have
 *   been filled to TX FIFO. In other word, TX FIFO is locked if the function
 *   return value equals @p msgCount, when @p lockSendFifo set to true.
 *
 * @param link IMU link.
 * @param msgs The messages to send.
 * @param msgCount Message count, one message is a 32-bit word.
 * @param lockSendFifo If set to true, the TX FIFO is locked after all messages filled to TX FIFO.
 * @return If TX FIFO is locked, this function returns IMU_ERR_TX_FIFO_LOCKED,
 * otherwise, this function returns the actual message count sent out.
 */
int32_t IMU_TrySendMsgs(imu_link_t link, const uint32_t *msgs, int32_t msgCount, bool lockSendFifo);

/*!
 * @brief Try to receive messages.
 *
 * This function tries to read messages from RX FIFO. It reads the messages already
 * exists in RX FIFO and returns the actual read count.
 *
 * - If the RX FIFO has enough messages, this function reads the messages and returns.
 * - If the RX FIFO does not have enough messages, this function the messages in RX FIFO
 *   and returns the actual read count.
 * - During message reading, if RX FIFO is empty and locked, in this case peer CPU will not
 *   send message until current CPU send lock ack message. Then this function
 *   returns the message count actually received, and sets @p needAckLock to true
 *   to inform upper layer.
 *
 * @param link IMU link.
 * @param msgs The buffer to read messages.
 * @param desiredMsgCount Desired read count, one message is a 32-bit word.
 * @param needAckLock Upper layer should always check this value. When this is
 * set to true by this function, upper layer should send lock ack message to peer CPU.
 * @return Count of messages actually received.
 */
int32_t IMU_TryReceiveMsgs(imu_link_t link, uint32_t *msgs, int32_t desiredMsgCount, bool *needAckLock);

/*!
 * @brief Blocking to receive messages.
 *
 * This function blocks until all desired messages have been received or the RX FIFO
 * is locked.
 *
 * - If the RX FIFO has enough messages, this function reads the messages and returns.
 * - If the RX FIFO does not have enough messages, this function waits for the new
 *   messages.
 * - During message reading, if RX FIFO is empty and locked, in this case peer CPU will not
 *   send message until current CPU send lock ack message. Then this function
 *   returns the message count actually received, and sets @p needAckLock to true
 *   to inform upper layer.
 *
 * @param link IMU link.
 * @param msgs The buffer to read messages.
 * @param desiredMsgCount Desired read count, one message is a 32-bit word.
 * @param needAckLock Upper layer should always check this value. When this is
 * set to true by this function, upper layer should send lock ack message to peer CPU.
 * @return Count of messages actually received.
 */
int32_t IMU_ReceiveMsgsBlocking(imu_link_t link, uint32_t *msgs, int32_t desiredMsgCount, bool *needAckLock);

/*!
 * @brief Blocking to send messages pointer.
 *
 * Compared with @ref IMU_SendMsgsBlocking, this function fills message pointer
 * to TX FIFO, but not the message content.
 *
 * This function blocks until the message pointer is filled to TX FIFO.
 *
 * - If the TX FIFO is locked, this function returns IMU_ERR_TX_FIFO_LOCKED.
 * - If TX FIFO not locked, this function waits the available empty slot in TX FIFO,
 *   and fills the message pointer to TX FIFO.
 * - To lock TX FIFO after filling the message pointer, set @p lockSendFifo to true.
 *
 * @param link IMU link.
 * @param msgPtr The buffer pointer to message to send.
 * @param needAckLock Upper layer should always check this value. When this is
 * set to true by this function, upper layer should send lock ack message to peer CPU.
 * @retval 0 The message pointer set successfully.
 * @retval IMU_ERR_TX_FIFO_LOCKED The TX FIFO is locked, send failed.
 */
int32_t IMU_SendMsgPtrBlocking(imu_link_t link, uint32_t msgPtr, bool lockSendFifo);

/*!
 * @brief Lock or unlock the TX FIFO.
 *
 * @param link IMU link.
 * @param lock Use true to lock the FIFO, use false to unlock.
 */
static inline void IMU_LockSendFifo(imu_link_t link, bool lock)
{
    if (lock)
    {
        ((IMU_Type *)(uintptr_t)IMU_CUR_CPU_BASE(link))->MSG_FIFO_CNTL |= IMU_MSG_FIFO_CNTL_WAIT_FOR_ACK_MASK;
    }
    else
    {
        ((IMU_Type *)(uintptr_t)IMU_CUR_CPU_BASE(link))->MSG_FIFO_CNTL &= ~IMU_MSG_FIFO_CNTL_WAIT_FOR_ACK_MASK;
    }
}

/*!
 * @brief Flush the send FIFO.
 *
 * Flush all messages in send FIFO.
 *
 * @param link IMU link.
 */
void IMU_FlushSendFifo(imu_link_t link);

/*!
 * @brief Set send FIFO warter mark.
 *
 * The warter mark must be less than IMU_MAX_MSG_FIFO_WATER_MARK,
 * i.e. 0 < waterMark <= IMU_MAX_MSG_FIFO_WATER_MARK.
 *
 * @param link IMU link.
 * @param waterMark Send FIFO warter mark.
 */
static inline void IMU_SetSendFifoWaterMark(imu_link_t link, uint8_t waterMark)
{
    uint32_t reg = ((IMU_Type *)(uintptr_t)IMU_CUR_CPU_BASE(link))->MSG_FIFO_CNTL;

    reg &= ~IMU_MSG_FIFO_CNTL_FIFO_FULL_WATERMARK_MASK;
    reg |= IMU_MSG_FIFO_CNTL_FIFO_FULL_WATERMARK(waterMark);

    ((IMU_Type *)(uintptr_t)IMU_CUR_CPU_BASE(link))->MSG_FIFO_CNTL = reg;
}

/*!
 * @brief Get the message count in receive FIFO.
 *
 * @param link IMU link.
 * @return The message count in receive FIFO.
 */
static inline uint32_t IMU_GetReceivedMsgCount(imu_link_t link)
{
    return IMU_RX_FIFO_MSG_COUNT(link);
}

/*!
 * @brief Get the empty slot in send FIFO.
 *
 * @param link IMU link.
 * @return The empty slot count in send FIFO.
 */
static inline uint32_t IMU_GetSendFifoEmptySpace(imu_link_t link)
{
    return IMU_MSG_FIFO_MAX_COUNT - IMU_TX_FIFO_MSG_COUNT(link) - 1U;
}

/*! @} */

/*!
 * @name Status and Interrupt.
 * @{
 */

/*!
 * @brief Gets the IMU status flags.
 *
 * @param link IMU link.
 * @return Bit mask of the IMU status flags, see @ref _imu_status_flags.
 */
uint32_t IMU_GetStatusFlags(imu_link_t link);

/*!
 * @brief Clear the IMU IRQ
 *
 * @param link IMU link.
 * @param mask Bit mask of the interrupts to clear, see @ref _imu_interrupts.
 */
static inline void IMU_ClearPendingInterrupts(imu_link_t link, uint32_t mask)
{
    ((IMU_Type *)(uintptr_t)IMU_CUR_CPU_BASE(link))->MSG_FIFO_CNTL |= mask;
}

/*! @} */

#if defined(__cplusplus)
}
#endif /*_cplusplus*/
/*! @} */

#endif /* FSL_IMU_H_*/
