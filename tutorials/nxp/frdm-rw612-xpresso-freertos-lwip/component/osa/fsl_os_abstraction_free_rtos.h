/*! *********************************************************************************
 * Copyright (c) 2013-2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * ile
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */
#if !defined(__FSL_OS_ABSTRACTION_FREERTOS_H__)
#define __FSL_OS_ABSTRACTION_FREERTOS_H__

#if defined(__IAR_SYSTEMS_ICC__)
/**
 * Workaround to disable MISRA C message suppress warnings for IAR compiler.
 */
/* http://supp.iar.com/Support/?note=24725 */

#define MISRAC_DISABLE \
    _Pragma(           \
        "diag_suppress=                       \
    Pm001,Pm002,Pm003,Pm004,Pm005,Pm006,Pm007,Pm008,Pm009,Pm010,Pm011,\
    Pm012,Pm013,Pm014,Pm015,Pm016,Pm017,Pm018,Pm019,Pm020,Pm021,Pm022,\
    Pm023,Pm024,Pm025,Pm026,Pm027,Pm028,Pm029,Pm030,Pm031,Pm032,Pm033,\
    Pm034,Pm035,Pm036,Pm037,Pm038,Pm039,Pm040,Pm041,Pm042,Pm043,Pm044,\
    Pm045,Pm046,Pm047,Pm048,Pm049,Pm050,Pm051,Pm052,Pm053,Pm054,Pm055,\
    Pm056,Pm057,Pm058,Pm059,Pm060,Pm061,Pm062,Pm063,Pm064,Pm065,Pm066,\
    Pm067,Pm068,Pm069,Pm070,Pm071,Pm072,Pm073,Pm074,Pm075,Pm076,Pm077,\
    Pm078,Pm079,Pm080,Pm081,Pm082,Pm083,Pm084,Pm085,Pm086,Pm087,Pm088,\
    Pm089,Pm090,Pm091,Pm092,Pm093,Pm094,Pm095,Pm096,Pm097,Pm098,Pm099,\
    Pm100,Pm101,Pm102,Pm103,Pm104,Pm105,Pm106,Pm107,Pm108,Pm109,Pm110,\
    Pm111,Pm112,Pm113,Pm114,Pm115,Pm116,Pm117,Pm118,Pm119,Pm120,Pm121,\
    Pm122,Pm123,Pm124,Pm125,Pm126,Pm127,Pm128,Pm129,Pm130,Pm131,Pm132,\
    Pm133,Pm134,Pm135,Pm136,Pm137,Pm138,Pm139,Pm140,Pm141,Pm142,Pm143,\
    Pm144,Pm145,Pm146,Pm147,Pm148,Pm149,Pm150,Pm151,Pm152,Pm153,Pm154,\
    Pm155")

#define MISRAC_ENABLE \
    _Pragma(          \
        "diag_default=                         \
    Pm001,Pm002,Pm003,Pm004,Pm005,Pm006,Pm007,Pm008,Pm009,Pm010,Pm011,\
    Pm012,Pm013,Pm014,Pm015,Pm016,Pm017,Pm018,Pm019,Pm020,Pm021,Pm022,\
    Pm023,Pm024,Pm025,Pm026,Pm027,Pm028,Pm029,Pm030,Pm031,Pm032,Pm033,\
    Pm034,Pm035,Pm036,Pm037,Pm038,Pm039,Pm040,Pm041,Pm042,Pm043,Pm044,\
    Pm045,Pm046,Pm047,Pm048,Pm049,Pm050,Pm051,Pm052,Pm053,Pm054,Pm055,\
    Pm056,Pm057,Pm058,Pm059,Pm060,Pm061,Pm062,Pm063,Pm064,Pm065,Pm066,\
    Pm067,Pm068,Pm069,Pm070,Pm071,Pm072,Pm073,Pm074,Pm075,Pm076,Pm077,\
    Pm078,Pm079,Pm080,Pm081,Pm082,Pm083,Pm084,Pm085,Pm086,Pm087,Pm088,\
    Pm089,Pm090,Pm091,Pm092,Pm093,Pm094,Pm095,Pm096,Pm097,Pm098,Pm099,\
    Pm100,Pm101,Pm102,Pm103,Pm104,Pm105,Pm106,Pm107,Pm108,Pm109,Pm110,\
    Pm111,Pm112,Pm113,Pm114,Pm115,Pm116,Pm117,Pm118,Pm119,Pm120,Pm121,\
    Pm122,Pm123,Pm124,Pm125,Pm126,Pm127,Pm128,Pm129,Pm130,Pm131,Pm132,\
    Pm133,Pm134,Pm135,Pm136,Pm137,Pm138,Pm139,Pm140,Pm141,Pm142,Pm143,\
    Pm144,Pm145,Pm146,Pm147,Pm148,Pm149,Pm150,Pm151,Pm152,Pm153,Pm154,\
    Pm155")
#else
/* Empty MISRA C macros for other toolchains. */
#define MISRAC_DISABLE
#define MISRAC_ENABLE
#endif

MISRAC_DISABLE
#include "FreeRTOS.h"
#include "semphr.h"
#include "event_groups.h"
MISRAC_ENABLE

/*!
 * @addtogroup os_abstraction_free_rtos
 * @{
 */

/*******************************************************************************
 * Declarations
 ******************************************************************************/
/*! @brief Type for a task handler, returned by the OSA_TaskCreate function. */
typedef TaskHandle_t task_handler_t;

/*! @brief Type for a task stack.*/
typedef portSTACK_TYPE task_stack_t;

/*! @brief Type for task parameter */
typedef void *task_param_t;

/*! @brief Type for an event flags object.*/
typedef EventBits_t event_flags_t;

/*! @brief Constant to pass as timeout value in order to wait indefinitely. */
#define OSA_WAIT_FOREVER 0xFFFFFFFFU

/*! @brief OSA's time range in millisecond, OSA time wraps if exceeds this value. */
#define FSL_OSA_TIME_RANGE 0xFFFFFFFFU

/*! @brief The default interrupt handler installed in vector table. */
#define OSA_DEFAULT_INT_HANDLER ((osa_int_handler_t)(&DefaultISR))

extern void DefaultISR(void);

/*!
 * @name Thread management
 * @{
 */

/*!
 * @brief To provide unified task piority for upper layer, OSA layer makes conversion.
 */
#define PRIORITY_OSA_TO_RTOS(osa_prio) \
    (((UBaseType_t)configMAX_PRIORITIES - 1U) * (OSA_TASK_PRIORITY_MIN - osa_prio) / OSA_TASK_PRIORITY_MIN)
#define PRIORITY_RTOS_TO_OSA(rtos_prio)                                               \
    (OSA_TASK_PRIORITY_MIN * (((UBaseType_t)configMAX_PRIORITIES - 1U) - rtos_prio) / \
     ((UBaseType_t)configMAX_PRIORITIES - 1U))

/*! @} */

/*!
 * @name Message queues
 * @{
 */

/*!
 * @brief This macro statically reserves the memory required for the queue.
 *
 * @param name Identifier for the memory region.
 * @param number Number of elements in the queue.
 * @param size Size of every elements in words.
 */
#define MSG_QUEUE_DECLARE(name, number, size) msg_queue_t *name = NULL

/*! @} */

/*! @}*/
/*! @}*/
/*! @}*/

#endif /* __FSL_OS_ABSTRACTION_FREERTOS_H__ */
