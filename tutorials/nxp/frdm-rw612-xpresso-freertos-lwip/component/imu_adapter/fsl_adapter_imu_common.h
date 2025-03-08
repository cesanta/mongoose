/*
 * Copyright 2021 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __FSL_ADAPTER_RFCOMMON_H__
#define __FSL_ADAPTER_RFCOMMON_H__

#include "fsl_os_abstraction.h"
#include "fsl_imu.h"
#include "assert.h"
#if defined(CPU2)
#include "core_cm3.h"
#endif

/*! @brief TRUE/FALSE definition. */
#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/*******************************************************************************
 * List Definitions and APIs
 ******************************************************************************/
/**
 * @brief Macro to iterate over a list using a user specified loop cursor.
 *
 * @param cur   the variable used for cursor, representing the current list node.
 * @param list  the head for your list.
 */
#define list_for_each(cur, list) for ((cur) = (list)->next; (cur) != (list); (cur) = (cur)->next)

/**
 * @brief An element in a doubly-linked circular list.
 *  A list is a doubly-linked circular list.
 */
typedef struct LIST_ELEM_st
{
    struct LIST_ELEM_st *next;
    struct LIST_ELEM_st *prev;
} LIST_ELEM_st;

/**
 *  @brief Initializes a list, as an empty list.
 *  @param list The list descriptor, i.e. the fake list element that identifies a given list.
 */
static inline void LIST_init(LIST_ELEM_st *list)
{
    list->next = list;
    list->prev = list;
}

/**
 *  @brief Initializes an element, as an element not yet bound within a list.
 *  @param elem The element to Initialize.
 */
static inline void LIST_elemInit(LIST_ELEM_st *elem)
{
    elem->next = NULL;
    elem->prev = NULL;
}

/**
 *  @brief  Inserts a new element in a list before a given existing element.
 *  @param  list    The list descriptor, i.e. the fake list element that identifies a given list.
 *  @param  elem    The new element is placed before this element.
 *  @param  newElem New element to insert in the list.
 */
static inline void LIST_insertBefore(LIST_ELEM_st *list, LIST_ELEM_st *elem, LIST_ELEM_st *newElem)
{
    assert(newElem != list);

    (void)list;

    (elem->prev)->next = newElem;
    newElem->next      = elem;
    newElem->prev      = elem->prev;
    elem->prev         = newElem;
}

/**
 *  @brief  Adds an element in a list at the tail of that list.
 *  @param  list The list descriptor, i.e. the fake list element that identifies a given list.
 *  @param  elem New element to insert in the list.
 */
static inline void LIST_addTail(LIST_ELEM_st *list, LIST_ELEM_st *elem)
{
    LIST_insertBefore(list, list, elem);
}

/**
 *  @brief  Removes an element from a list.
 *  @param  list The list descriptor, i.e. the fake list element that identifies a given list.
 *  @param  elem The element to remove.
 */
static inline void LIST_remove(LIST_ELEM_st *list, LIST_ELEM_st *elem)
{
    assert(elem != list);

    (void)list;

    if ((elem->next == NULL) || (elem->prev == NULL))
    {
        return;
    }
    (elem->prev)->next = elem->next;
    (elem->next)->prev = elem->prev;

    elem->next = NULL; // to indicate this is not linked yet
    elem->prev = NULL;
}

/*!
 * @addtogroup IMU_Adapter
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
   IRQ index should come from board level definition.
   Remove following definition after IMUMC code integration.
*/
#if defined(CPU2)
#define IRQ_IMU_CPU12 CPU1_TO_CPU2_MSG_RDY_IMU_INT_IRQn
#define IRQ_IMU_CPU32 CPU3_TO_CPU2_MSG_RDY_IMU_INT_IRQn
#else
#define IRQ_IMU_CPU13 WL_MCI_WAKEUP0_IRQn
#define IRQ_IMU_CPU23 BLE_MCI_WAKEUP0_IRQn
#endif

#if defined(CPU2)
#define IMU_MSG_SND_Q(imuLinkId) (((imuLinkId) == kIMU_LinkCpu1Cpu2) ? imuMsgQ12->msgQ[1] : imuMsgQ23->msgQ[0])
#define IMU_MSG_CUR_MAGIC_P(imuLinkId) \
    (((imuLinkId) == kIMU_LinkCpu1Cpu2) ? (&imuMsgQ12->magic[1]) : (&imuMsgQ23->magic[0]))
#define IMU_MSG_PEER_MAGIC_P(imuLinkId) \
    (((imuLinkId) == kIMU_LinkCpu1Cpu2) ? (&imuMsgQ12->magic[0]) : (&imuMsgQ23->magic[1]))
#define IRQID_TO_IMULINKID(irqId)     (((irqId) == IRQ_IMU_CPU12) ? kIMU_LinkCpu1Cpu2 : kIMU_LinkCpu2Cpu3)
#define IMULINKID_TO_IRQID(imuLinkId) (((imuLinkId) == kIMU_LinkCpu1Cpu2) ? IRQ_IMU_CPU12 : IRQ_IMU_CPU32)
#else
#define IMU_MSG_SND_Q(imuLinkId) (((imuLinkId) == kIMU_LinkCpu1Cpu3) ? imuMsgQ13->msgQ[1] : imuMsgQ23->msgQ[1])
#define IMU_MSG_CUR_MAGIC_P(imuLinkId) \
    (((imuLinkId) == kIMU_LinkCpu1Cpu3) ? (&imuMsgQ13->magic[1]) : (&imuMsgQ23->magic[1]))
#define IMU_MSG_PEER_MAGIC_P(imuLinkId) \
    (((imuLinkId) == kIMU_LinkCpu1Cpu3) ? (&imuMsgQ13->magic[0]) : (&imuMsgQ23->magic[0]))
#define IRQID_TO_IMULINKID(irqId)     (((irqId) == IRQ_IMU_CPU13) ? kIMU_LinkCpu1Cpu3 : kIMU_LinkCpu2Cpu3)
#define IMULINKID_TO_IRQID(imuLinkId) (((imuLinkId) == kIMU_LinkCpu1Cpu3) ? IRQ_IMU_CPU13 : IRQ_IMU_CPU23)
#endif
#endif /* __FSL_ADAPTER_RFCOMMON_H__ */
