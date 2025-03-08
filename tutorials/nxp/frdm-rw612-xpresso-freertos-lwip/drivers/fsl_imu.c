/*
 * Copyright 2020-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_imu.h"

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.imu"
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

/******************************************************************************
 * Code
 *****************************************************************************/

/*
 * Initializes the IMU module.
 *
 * This function sets IMU to initialized state, including:
 *
 *   - Flush the send FIFO.
 *   - Unlock the send FIFO.
 *   - Set the water mark to (IMU_MAX_MSG_FIFO_WATER_MARK)
 *
 */
status_t IMU_Init(imu_link_t link)
{
    status_t status;

    if (link >= kIMU_LinkMax)
    {
        status = kStatus_InvalidArgument;
    }
    else
    {
        IMU_FlushSendFifo(link);
        IMU_LockSendFifo(link, false);
        IMU_SetSendFifoWaterMark(link, IMU_MAX_MSG_FIFO_WATER_MARK);

        /* Flush RX FIFO. */
        while (!IMU_RX_FIFO_EMPTY(link))
        {
            (void)IMU_RD_MSG(link);
        }

        status = kStatus_Success;
    }

    return status;
}

/*
 * De-initializes the IMU module.
 *
 */
void IMU_Deinit(imu_link_t link)
{
}

/*
 * Blocking to send messages.
 *
 * This function blocks until all messages have been filled to TX FIFO.
 *
 * - If the TX FIFO is locked, this function returns IMU_ERR_TX_FIFO_LOCKED.
 * - If TX FIFO not locked, this function waits the available empty slot in TX FIFO,
 *   and fills the message to TX FIFO.
 * - To lock TX FIFO after filling all messages, set  lockSendFifo to true.
 *
 * param link IMU link.
 * param msgs The messages to send.
 * param msgCount Message count, one message is a 32-bit word.
 * param lockSendFifo If set to true, the TX FIFO is locked after all messages filled to TX FIFO.
 * return If TX FIFO is locked, this function returns IMU_ERR_TX_FIFO_LOCKED,
 * otherwise, this function returns the actual message count sent out, it equals  msgCount
 * because this function is blocking function, it returns until all messages have been
 * filled into TX FIFO.
 */
int32_t IMU_SendMsgsBlocking(imu_link_t link, const uint32_t *msgs, int32_t msgCount, bool lockSendFifo)
{
    int32_t ret;
    int32_t curSent;
    int32_t fifoEmptySpace;

    if (msgCount <= 0)
    {
        ret = 0;
    }
    else if (IMU_TX_FIFO_LOCKED(link))
    {
        ret = IMU_ERR_TX_FIFO_LOCKED;
    }
    else
    {
        ret = msgCount;

        /* Send the first (msgCount - 1), the last message should
         * be sent with the lock configuration.
         */
        msgCount--;

        while (msgCount > 0)
        {
            fifoEmptySpace = (int32_t)IMU_GetSendFifoEmptySpace(link);
            curSent        = MIN(msgCount, fifoEmptySpace);

            msgCount -= curSent;

            while (0 != (curSent--))
            {
                IMU_WR_MSG(link, *msgs);
                msgs++;
            }
        }

        /* To lock TX FIFO, set the lock bit before last message write. */
        if (lockSendFifo)
        {
            IMU_LOCK_TX_FIFO(link);
        }

        /* Send the last. */
        while (0UL == IMU_GetSendFifoEmptySpace(link))
        {
        }
        IMU_WR_MSG(link, *msgs);
    }

    return ret;
}

/*
 * Try to send messages.
 *
 * This function is similar with @ref IMU_SendMsgsBlocking, the difference is,
 * this function tries to send as many as possible, if there is not enough
 * empty slot in TX FIFO, this function fills messages to available empty slots
 * and returns how many messages have been filled.
 *
 * - If the TX FIFO is locked, this function returns IMU_ERR_TX_FIFO_LOCKED.
 * - If TX FIFO not locked, this function fills messages to TX FIFO empty slot,
 *   and returns how many messages have been filled.
 * - If  lockSendFifo is set to true, TX FIFO is locked after all messages have
 *   been filled to TX FIFO. In other word, TX FIFO is locked if the function
 *   return value equals  msgCount, when  lockSendFifo set to true.
 *
 * param link IMU link.
 * param msgs The messages to send.
 * param msgCount Message count, one message is a 32-bit word.
 * param lockSendFifo If set to true, the TX FIFO is locked after all messages filled to TX FIFO.
 * return If TX FIFO is locked, this function returns IMU_ERR_TX_FIFO_LOCKED,
 * otherwise, this function returns the actual message count sent out.
 */
int32_t IMU_TrySendMsgs(imu_link_t link, const uint32_t *msgs, int32_t msgCount, bool lockSendFifo)
{
    int32_t ret;
    int32_t curSent;
    int32_t fifoEmptySpace;

    if (msgCount <= 0)
    {
        ret = 0;
    }
    else if (IMU_TX_FIFO_LOCKED(link))
    {
        ret = IMU_ERR_TX_FIFO_LOCKED;
    }
    else
    {
        ret = 0;

        /* Send the first (msgCount - 1), the last message should
         * be sent with the lock configuration.
         */
        msgCount--;

        while (true)
        {
            fifoEmptySpace = (int32_t)IMU_GetSendFifoEmptySpace(link);

            if (fifoEmptySpace == 0)
            {
                break;
            }
            /* If this is the last and have FIFO space to send. */
            else if (0 == msgCount)
            {
                if (lockSendFifo)
                {
                    IMU_LOCK_TX_FIFO(link);
                }

                /* Send the last. */
                IMU_WR_MSG(link, *msgs);
                ret++;
                break;
            }
            else
            {
                curSent = MIN(msgCount, fifoEmptySpace);

                /* Send the data. */
                msgCount -= curSent;
                ret += curSent;
                while (0 != (curSent--))
                {
                    IMU_WR_MSG(link, *msgs);
                    msgs++;
                }
            }
        }
    }

    return ret;
}

/*
 * Try to receive messages.
 *
 * This function tries to read messages from RX FIFO. It reads the messages already
 * exists in RX FIFO and returns the actual read count.
 *
 * - If the RX FIFO has enough messages, this function reads the messages and returns.
 * - If the RX FIFO does not have enough messages, this function the messages in RX FIFO
 *   and returns the actual read count.
 * - During message reading, if RX FIFO is empty and locked, in this case peer CPU will not
 *   send message until current CPU send lock ack message. Then this function
 *   returns the message count actually received, and sets  needAckLock to true
 *   to inform upper layer.
 *
 * param link IMU link.
 * param msgs The buffer to read messages.
 * param desiredMsgCount Desired read count, one message is a 32-bit word.
 * param needAckLock Upper layer should always check this value. When this is
 * set to true by this function, upper layer should send lock ack message to peer CPU.
 * return Count of messages actually received.
 */
int32_t IMU_TryReceiveMsgs(imu_link_t link, uint32_t *msgs, int32_t desiredMsgCount, bool *needAckLock)
{
    int32_t receivedCount = 0;
    int32_t rxFifoMsgCount;
    int32_t countToRead;
    uint32_t rxFifoStatus;
    bool localNeedAckLock = false;

    while (true)
    {
        rxFifoStatus = IMU_RX_FIFO_STATUS(link);

        rxFifoMsgCount = (int32_t)(uint32_t)IMU_RX_FIFO_MSG_COUNT_FROM_STATUS(rxFifoStatus);

        if (0 == rxFifoMsgCount)
        {
            if (IMU_RX_FIFO_LOCKED_FROM_STATUS(rxFifoStatus))
            {
                localNeedAckLock = true;
            }
            break;
        }

        /*
         * Dont need to check RX FIFO lock status, only notify upper layer
         * when last message read out.
         */
        if (0 == desiredMsgCount)
        {
            break;
        }

        countToRead = MIN(desiredMsgCount, rxFifoMsgCount);
        receivedCount += countToRead;
        desiredMsgCount -= countToRead;

        while (0 != (countToRead--))
        {
            *msgs++ = IMU_RD_MSG(link);
        }
    }

    if (needAckLock != NULL)
    {
        *needAckLock = localNeedAckLock;
    }

    return receivedCount;
}

/*
 * Blocking to receive messages.
 *
 * This function blocks until all desired messages have been received or the RX FIFO
 * is locked.
 *
 * - If the RX FIFO has enough messages, this function reads the messages and returns.
 * - If the RX FIFO does not have enough messages, this function waits for the new
 *   messages.
 * - During message reading, if RX FIFO is empty and locked, in this case peer CPU will not
 *   send message until current CPU send lock ack message. Then this function
 *   returns the message count actually received, and sets  needAckLock to true
 *   to inform upper layer.
 *
 * param link IMU link.
 * param msgs The buffer to read messages.
 * param desiredMsgCount Desired read count, one message is a 32-bit word.
 * param needAckLock Upper layer should always check this value. When this is
 * set to true by this function, upper layer should send lock ack message to peer CPU.
 * return Count of messages actually received.
 */
int32_t IMU_ReceiveMsgsBlocking(imu_link_t link, uint32_t *msgs, int32_t desiredMsgCount, bool *needAckLock)
{
    int32_t receivedCount = 0;
    int32_t rxFifoMsgCount;
    int32_t countToRead;
    uint32_t rxFifoStatus;
    bool localNeedAckLock;

    while (true)
    {
        rxFifoStatus = IMU_RX_FIFO_STATUS(link);

        rxFifoMsgCount = (int32_t)(uint32_t)IMU_RX_FIFO_MSG_COUNT_FROM_STATUS(rxFifoStatus);

        if ((0 == rxFifoMsgCount) && (IMU_RX_FIFO_LOCKED_FROM_STATUS(rxFifoStatus)))
        {
            localNeedAckLock = true;
            break;
        }

        if (0 == desiredMsgCount)
        {
            localNeedAckLock = false;
            break;
        }

        countToRead = MIN(desiredMsgCount, rxFifoMsgCount);
        receivedCount += countToRead;
        desiredMsgCount -= countToRead;

        while (0 != (countToRead--))
        {
            *msgs++ = IMU_RD_MSG(link);
        }
    }

    if (NULL != needAckLock)
    {
        *needAckLock = localNeedAckLock;
    }

    return receivedCount;
}

/*
 * brief Blocking to send messages pointer.
 *
 * Compared with @ref IMU_SendMsgsBlocking, this function fills message pointer
 * to TX FIFO, but not the message content.
 *
 * This function blocks until the message pointer is filled to TX FIFO.
 *
 * - If the TX FIFO is locked, this function returns IMU_ERR_TX_FIFO_LOCKED.
 * - If TX FIFO not locked, this function waits the available empty slot in TX FIFO,
 *   and fills the message pointer to TX FIFO.
 * - To lock TX FIFO after filling the message pointer, set lockSendFifo to true.
 *
 * param link IMU link.
 * param msgPtr The buffer pointer to message to send.
 * param needAckLock Upper layer should always check this value. When this is
 * set to true by this function, upper layer should send lock ack message to peer CPU.
 * retval 0 The message pointer set successfully.
 * retval IMU_ERR_TX_FIFO_LOCKED The TX FIFO is locked, send failed.
 */
int32_t IMU_SendMsgPtrBlocking(imu_link_t link, uint32_t msgPtr, bool lockSendFifo)
{
    int32_t ret = 0;

    if (IMU_TX_FIFO_LOCKED(link))
    {
        ret = IMU_ERR_TX_FIFO_LOCKED;
    }
    else
    {
        while (IMU_TX_FIFO_ALMOST_FULL(link))
        {
        }

        if (lockSendFifo)
        {
            IMU_LOCK_TX_FIFO(link);
        }

        IMU_WR_MSG(link, msgPtr);
    }

    return ret;
}

/*
 * brief Flush the send FIFO.
 *
 * Flush all messages in send FIFO.
 *
 * param link IMU link.
 */
void IMU_FlushSendFifo(imu_link_t link)
{
    IMU_TX_FIFO_CNTL(link) |= IMU_MSG_FIFO_CNTL_FIFO_FLUSH_MASK;
    IMU_TX_FIFO_CNTL(link) &= ~IMU_MSG_FIFO_CNTL_FIFO_FLUSH_MASK;
}

/*
 * Gets the IMU status flags.
 *
 * param link IMU link.
 * return Bit mask of the IMU status flags, see _imu_status_flags.
 */
uint32_t IMU_GetStatusFlags(imu_link_t link)
{
    uint32_t txFifoStatus = IMU_TX_FIFO_STATUS(link);
    uint32_t rxFifoStatus = IMU_RX_FIFO_STATUS(link);

    return txFifoStatus | (rxFifoStatus << 8U);
}
