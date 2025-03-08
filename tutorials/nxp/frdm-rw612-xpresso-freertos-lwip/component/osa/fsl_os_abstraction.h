/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020, 2024 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_OS_ABSTRACTION_H_
#define _FSL_OS_ABSTRACTION_H_

#ifndef SDK_COMPONENT_DEPENDENCY_FSL_COMMON
#define SDK_COMPONENT_DEPENDENCY_FSL_COMMON (1U)
#endif
#if (defined(SDK_COMPONENT_DEPENDENCY_FSL_COMMON) && (SDK_COMPONENT_DEPENDENCY_FSL_COMMON > 0U))
#include "fsl_common.h"
#else
#endif

#include "fsl_os_abstraction_config.h"

#ifndef __ZEPHYR__
#include "fsl_component_generic_list.h"
#endif

/*!
 * @addtogroup osa_adapter
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Type for the Task Priority*/
typedef uint16_t osa_task_priority_t;
/*! @brief Type for a task handler */
typedef void *osa_task_handle_t;
/*! @brief Type for the parameter to be passed to the task at its creation */
typedef void *osa_task_param_t;
/*! @brief Type for task pointer. Task prototype declaration */
typedef void (*osa_task_ptr_t)(osa_task_param_t task_param);
/*! @brief Type for the semaphore handler */
typedef void *osa_semaphore_handle_t;
/*! @brief Type for the mutex handler */
typedef void *osa_mutex_handle_t;
/*! @brief Type for the event handler */
typedef void *osa_event_handle_t;
/*! @brief Type for an event flags group, bit 32 is reserved. */
typedef uint32_t osa_event_flags_t;
/*! @brief Message definition. */
typedef void *osa_msg_handle_t;
/*! @brief Type for the message queue handler */
typedef void *osa_msgq_handle_t;
/*! @brief Type for the Timer handler */
typedef void *osa_timer_handle_t;
/*! @brief Type for the Timer callback function pointer. */
typedef void (*osa_timer_fct_ptr_t)(void const *argument);
/*! @brief Type for the semaphore counter. */
typedef uint32_t osa_semaphore_count_t;
/*! @brief Type for the notification wait time. */
typedef uint32_t osa_notify_time_ms_t;
/*! @brief Thread Definition structure contains startup information of a thread.*/
typedef struct osa_task_def_tag
{
    osa_task_ptr_t pthread; /*!< start address of thread function*/
    uint32_t tpriority;     /*!< initial thread priority*/
    uint32_t instances;     /*!< maximum number of instances of that thread function*/
    uint32_t stacksize;     /*!< stack size requirements in bytes; 0 is default stack size*/
    uint8_t *tstack;        /*!< stack pointer, which can be used on freertos static allocation*/
    void *tlink;            /*!< link pointer*/
    uint8_t *tname;         /*!< name pointer*/
    uint8_t useFloat;       /*!< is use float*/
} osa_task_def_t;
/*! @brief Thread Link Definition structure .*/
typedef struct osa_thread_link_tag
{
    uint8_t link[12];                  /*!< link*/
    osa_task_handle_t osThreadId;      /*!< thread id*/
    osa_task_def_t *osThreadDefHandle; /*!< pointer of thread define handle*/
    uint32_t *osThreadStackHandle;     /*!< pointer of thread stack handle*/
} osa_thread_link_t, *osa_thread_link_handle_t;

/*! @brief Definition structure contains timer parameters.*/
typedef struct osa_time_def_tag
{
    osa_timer_fct_ptr_t pfCallback; /* < start address of a timer function */
    void *argument;                 /* < argument of a timer function */
} osa_time_def_t;

/*! @brief Type for the timer definition*/
typedef enum _osa_timer
{
    KOSA_TimerOnce     = 0, /*!< one-shot timer*/
    KOSA_TimerPeriodic = 1  /*!< repeating timer*/
} osa_timer_t;

/*! @brief Defines the return status of OSA's functions */
#if (defined(SDK_COMPONENT_DEPENDENCY_FSL_COMMON) && (SDK_COMPONENT_DEPENDENCY_FSL_COMMON > 0U))
typedef enum _osa_status
{
    KOSA_StatusSuccess = kStatus_Success,                  /*!< Success */
    KOSA_StatusError   = MAKE_STATUS(kStatusGroup_OSA, 1), /*!< Failed */
    KOSA_StatusTimeout = MAKE_STATUS(kStatusGroup_OSA, 2), /*!< Timeout occurs while waiting */
    KOSA_StatusIdle    = MAKE_STATUS(kStatusGroup_OSA, 3), /*!< Used for bare metal only, the wait object is not ready
                                                                 and timeout still not occur */
} osa_status_t;
#else
typedef enum _osa_status
{
    KOSA_StatusSuccess = 0, /*!< Success */
    KOSA_StatusError   = 1, /*!< Failed */
    KOSA_StatusTimeout = 2, /*!< Timeout occurs while waiting */
    KOSA_StatusIdle    = 3, /*!< Used for bare metal only, the wait object is not ready
                                                and timeout still not occur */
} osa_status_t;

#endif

#ifdef USE_RTOS
#undef USE_RTOS
#endif

#if defined(SDK_OS_FREE_RTOS)
#include "fsl_os_abstraction_free_rtos.h"
#elif defined(FSL_RTOS_THREADX)
#include "fsl_os_abstraction_threadx.h"
#elif defined(__ZEPHYR__)
#include "fsl_os_abstraction_zephyr.h"
#else
#include "fsl_os_abstraction_bm.h"
#endif

extern const uint8_t gUseRtos_c;

#if defined(SDK_OS_MQX)
#define USE_RTOS (1)
#elif defined(SDK_OS_FREE_RTOS)
#define USE_RTOS (1)
#if (defined(GENERIC_LIST_LIGHT) && (GENERIC_LIST_LIGHT > 0U))
#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
#define OSA_TASK_HANDLE_SIZE (150U)
#else
#define OSA_TASK_HANDLE_SIZE (12U)
#endif
#else
#define OSA_TASK_HANDLE_SIZE (16U)
#endif
#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
#define OSA_EVENT_HANDLE_SIZE (40U)
#else
#define OSA_EVENT_HANDLE_SIZE (8U)
#endif
#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
#define OSA_SEM_HANDLE_SIZE (84U)
#else
#define OSA_SEM_HANDLE_SIZE (4U)
#endif
#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
#define OSA_MUTEX_HANDLE_SIZE (84U)
#else
#define OSA_MUTEX_HANDLE_SIZE (4U)
#endif
#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
#define OSA_MSGQ_HANDLE_SIZE (84U)
#else
#define OSA_MSGQ_HANDLE_SIZE (4U)
#endif
#define OSA_MSG_HANDLE_SIZE (0U)
#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
#define OSA_TIMER_HANDLE_SIZE (48U)
#else
#define OSA_TIMER_HANDLE_SIZE (4U)
#endif
#elif defined(SDK_OS_UCOSII)
#define USE_RTOS (1)
#elif defined(SDK_OS_UCOSIII)
#define USE_RTOS (1)
#elif defined(FSL_RTOS_THREADX)
#define USE_RTOS (1)
#elif defined(__ZEPHYR__)
#define USE_RTOS (1)
#else
#define USE_RTOS (0)
#if (defined(GENERIC_LIST_LIGHT) && (GENERIC_LIST_LIGHT > 0U))
#define OSA_TASK_HANDLE_SIZE (24U)
#else
#define OSA_TASK_HANDLE_SIZE (28U)
#endif
#if (defined(FSL_OSA_TASK_ENABLE) && (FSL_OSA_TASK_ENABLE > 0U))
#define OSA_EVENT_HANDLE_SIZE (20U)
#else
#define OSA_EVENT_HANDLE_SIZE (16U)
#endif /* FSL_OSA_TASK_ENABLE */
#if (defined(FSL_OSA_BM_TIMEOUT_ENABLE) && (FSL_OSA_BM_TIMEOUT_ENABLE > 0U))
#define OSA_SEM_HANDLE_SIZE   (16U)
#define OSA_MUTEX_HANDLE_SIZE (12U)
#else
#define OSA_SEM_HANDLE_SIZE   (8U)
#define OSA_MUTEX_HANDLE_SIZE (4U)
#endif
#if (defined(FSL_OSA_TASK_ENABLE) && (FSL_OSA_TASK_ENABLE > 0U))
#define OSA_MSGQ_HANDLE_SIZE (32U)
#else
#define OSA_MSGQ_HANDLE_SIZE (28U)
#endif /* FSL_OSA_TASK_ENABLE */
#define OSA_MSG_HANDLE_SIZE (4U)
#endif

/*! @brief Priority setting for OSA. */
#ifndef OSA_PRIORITY_IDLE
#define OSA_PRIORITY_IDLE (6U)
#endif

#ifndef OSA_PRIORITY_LOW
#define OSA_PRIORITY_LOW (5U)
#endif

#ifndef OSA_PRIORITY_BELOW_NORMAL
#define OSA_PRIORITY_BELOW_NORMAL (4U)
#endif

#ifndef OSA_PRIORITY_NORMAL
#define OSA_PRIORITY_NORMAL (3U)
#endif

#ifndef OSA_PRIORITY_ABOVE_NORMAL
#define OSA_PRIORITY_ABOVE_NORMAL (2U)
#endif

#ifndef OSA_PRIORITY_HIGH
#define OSA_PRIORITY_HIGH (1U)
#endif

#ifndef OSA_PRIORITY_REAL_TIME
#define OSA_PRIORITY_REAL_TIME (0U)
#endif

#ifndef OSA_TASK_PRIORITY_MAX
#define OSA_TASK_PRIORITY_MAX (0U)
#endif

#ifndef OSA_TASK_PRIORITY_MIN
#define OSA_TASK_PRIORITY_MIN (15U)
#endif

/*
 * Converse the percent of the priority to the priority of the OSA.
 * The the range of the parameter x is 0-100.
 */
#define OSA_TASK_PRIORITY_PERCENT(x) \
    ((((OSA_TASK_PRIORITY_MIN - OSA_TASK_PRIORITY_MAX) * (100 - (x))) / 100) + OSA_TASK_PRIORITY_MAX)

#define SIZE_IN_UINT32_UNITS(size) (((size) + sizeof(uint32_t) - 1) / sizeof(uint32_t))

/*! @brief Constant to pass as timeout value in order to wait indefinitely. */
#define osaWaitNone_c            ((uint32_t)(0))
#define osaWaitForever_c         ((uint32_t)(-1))
#define osaEventFlagsAll_c       ((osa_event_flags_t)(0x00FFFFFF))
#define osThreadStackArray(name) osThread_##name##_stack
#define osThreadStackDef(name, stacksize, instances) \
    const uint32_t osThreadStackArray(name)[SIZE_IN_UINT32_UNITS(stacksize) * (instances)];

/* ==== Thread Management ==== */

/* Create a Thread Definition with function, priority, and stack requirements.
 * \param         name         name of the thread function.
 * \param         priority     initial priority of the thread function.
 * \param         instances    number of possible thread instances.
 * \param         stackSz      stack size (in bytes) requirements for the thread function.
 * \param         useFloat
 */
#if defined(SDK_OS_MQX)
#define OSA_TASK_DEFINE(name, priority, instances, stackSz, useFloat)               \
    osa_thread_link_t osThreadLink_##name[instances] = {0};                         \
    osThreadStackDef(name, stackSz, instances)                                      \
        osa_task_def_t os_thread_def_##name = {(name),                              \
                                               (priority),                          \
                                               (instances),                         \
                                               (stackSz),                           \
                                               (uint8_t *)osThreadStackArray(name), \
                                               osThreadLink_##name,                 \
                                               (uint8_t *)#name,                    \
                                               (useFloat)}
#elif defined(SDK_OS_UCOSII)
#if gTaskMultipleInstancesManagement_c
#define OSA_TASK_DEFINE(name, priority, instances, stackSz, useFloat)               \
    osa_thread_link_t osThreadLink_##name[instances] = {0};                         \
    osThreadStackDef(name, stackSz, instances)                                      \
        osa_task_def_t os_thread_def_##name = {(name),                              \
                                               (priority),                          \
                                               (instances),                         \
                                               (stackSz),                           \
                                               (uint8_t *)osThreadStackArray(name), \
                                               osThreadLink_##name,                 \
                                               (uint8_t *)#name,                    \
                                               (useFloat)}
#else
#define OSA_TASK_DEFINE(name, priority, instances, stackSz, useFloat)                          \
    osThreadStackDef(name, stackSz, instances) osa_task_def_t os_thread_def_##name = {         \
        (name), (priority),       (instances), (stackSz), (uint8_t *)osThreadStackArray(name), \
        NULL,   (uint8_t *)#name, (useFloat)}
#endif
#elif defined(FSL_RTOS_THREADX)
#define OSA_TASK_DEFINE(name, priority, instances, stackSz, useFloat)                                             \
    uint32_t s_stackBuffer##name[(stackSz + sizeof(uint32_t) - 1U) / sizeof(uint32_t)];                           \
    static const osa_task_def_t                                                                                   \
        os_thread_def_##name = {(name), (priority),       (instances), (stackSz), (uint8_t *)s_stackBuffer##name, \
                                NULL,   (uint8_t *)#name, (useFloat)}
#elif defined(__ZEPHYR__)
#define OSA_TASK_DEFINE(name, priority, instances, stackSz, useFloat) \
    K_THREAD_STACK_DEFINE(name##_stack, (stackSz));                   \
    const osa_task_def_t os_thread_def_##name = {                     \
        (name), (priority), (instances), (stackSz), (uint8_t *)name##_stack, NULL, (uint8_t *)#name, (useFloat)}
#else
#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
#define OSA_TASK_DEFINE(name, priority, instances, stackSz, useFloat)                                             \
    uint32_t s_stackBuffer##name[(stackSz + sizeof(uint32_t) - 1U) / sizeof(uint32_t)];                           \
    static const osa_task_def_t                                                                                   \
        os_thread_def_##name = {(name), (priority),       (instances), (stackSz), (uint8_t *)s_stackBuffer##name, \
                                NULL,   (uint8_t *)#name, (useFloat)}
#else
#define OSA_TASK_DEFINE(name, priority, instances, stackSz, useFloat)                             \
    const osa_task_def_t os_thread_def_##name = {(name), (priority), (instances),      (stackSz), \
                                                 NULL,   NULL,       (uint8_t *)#name, (useFloat)}
#endif
#endif
/* Access a Thread defintion.
 * \param         name          name of the thread definition object.
 */
#define OSA_TASK(name) (const osa_task_def_t *)&os_thread_def_##name

#define OSA_TASK_PROTO(name) extern osa_task_def_t os_thread_def_##name
/*  ==== Timer Management  ====
 * Define a Timer object.
 * \param         name          name of the timer object.
 * \param         function      name of the timer call back function.
 */

#define OSA_TIMER_DEF(name, function) osa_time_def_t os_timer_def_##name = {(function), NULL}

/* Access a Timer definition.
 * \param         name          name of the timer object.
 */
#define OSA_TIMER(name) &os_timer_def_##name

/* ==== Buffer Definition ==== */

/*!
 * @brief Defines the semaphore handle
 *
 * This macro is used to define a 4 byte aligned semaphore handle.
 * Then use "(osa_semaphore_handle_t)name" to get the semaphore handle.
 *
 * The macro should be global and could be optional. You could also define semaphore handle by yourself.
 *
 * This is an example,
 * @code
 *   OSA_SEMAPHORE_HANDLE_DEFINE(semaphoreHandle);
 * @endcode
 *
 * @param name The name string of the semaphore handle.
 */
#define OSA_SEMAPHORE_HANDLE_DEFINE(name) \
    uint32_t name[(OSA_SEM_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t)]

/*!
 * @brief Defines the mutex handle
 *
 * This macro is used to define a 4 byte aligned mutex handle.
 * Then use "(osa_mutex_handle_t)name" to get the mutex handle.
 *
 * The macro should be global and could be optional. You could also define mutex handle by yourself.
 *
 * This is an example,
 * @code
 *   OSA_MUTEX_HANDLE_DEFINE(mutexHandle);
 * @endcode
 *
 * @param name The name string of the mutex handle.
 */
#define OSA_MUTEX_HANDLE_DEFINE(name) uint32_t name[(OSA_MUTEX_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t)]

/*!
 * @brief Defines the event handle
 *
 * This macro is used to define a 4 byte aligned event handle.
 * Then use "(osa_event_handle_t)name" to get the event handle.
 *
 * The macro should be global and could be optional. You could also define event handle by yourself.
 *
 * This is an example,
 * @code
 *   OSA_EVENT_HANDLE_DEFINE(eventHandle);
 * @endcode
 *
 * @param name The name string of the event handle.
 */
#define OSA_EVENT_HANDLE_DEFINE(name) uint32_t name[(OSA_EVENT_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t)]

/*!
 * @brief Defines the message queue handle
 *
 * This macro is used to define a 4 byte aligned message queue handle.
 * Then use "(osa_msgq_handle_t)name" to get the message queue handle.
 *
 * The macro should be global and could be optional. You could also define message queue handle by yourself.
 *
 * This is an example,
 * @code
 *   OSA_MSGQ_HANDLE_DEFINE(msgqHandle, 3, sizeof(msgStruct));
 * @endcode
 *
 * @param name The name string of the message queue handle.
 * @param numberOfMsgs Number of messages.
 * @param msgSize Message size.
 *
 */
#if defined(SDK_OS_FREE_RTOS) && (defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0))
/*< Macro For FREE_RTOS dynamic allocation*/
#define OSA_MSGQ_HANDLE_DEFINE(name, numberOfMsgs, msgSize) \
    uint32_t name[(OSA_MSGQ_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t)]
#elif defined(__ZEPHYR__)
#define OSA_MSGQ_HANDLE_DEFINE(name, numberOfMsgs, msgSize) \
    uint32_t name[(OSA_MSGQ_HANDLE_SIZE + (numberOfMsgs * msgSize) + sizeof(uint32_t) - 1U) / sizeof(uint32_t)]
#else
/*< Macro For BARE_MATEL and FREE_RTOS static allocation*/
#define OSA_MSGQ_HANDLE_DEFINE(name, numberOfMsgs, msgSize) \
    uint32_t name[((OSA_MSGQ_HANDLE_SIZE + numberOfMsgs * msgSize) + sizeof(uint32_t) - 1U) / sizeof(uint32_t)]
#endif

/*!
 * @brief Defines the timer handle
 *
 * This macro is used to define a 4 byte aligned timer handle.
 * Then use "(osa_timer_handle_t)name" to get the timer handle.
 *
 * The macro should be global and could be optional. You could also define timer handle by yourself.
 *
 * This is an example,
 * @code
 *   OSA_TIMER_HANDLE_DEFINE(timerHandle);
 * @endcode
 *
 * @param name The name string of the timer handle.
 */
#define OSA_TIMER_HANDLE_DEFINE(name) uint32_t name[(OSA_TIMER_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t)]

/*!
 * @brief Defines the TASK handle
 *
 * This macro is used to define a 4 byte aligned TASK handle.
 * Then use "(osa_task_handle_t)name" to get the TASK handle.
 *
 * The macro should be global and could be optional. You could also define TASK handle by yourself.
 *
 * This is an example,
 * @code
 *   OSA_TASK_HANDLE_DEFINE(taskHandle);
 * @endcode
 *
 * @param name The name string of the TASK handle.
 */
#define OSA_TASK_HANDLE_DEFINE(name) uint32_t name[(OSA_TASK_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t)]

/*
 * alloc the temporary memory to store the status
 */
#define OSA_SR_ALLOC() uint32_t osaCurrentSr = 0U;
/*
 * Enter critical mode
 */
#define OSA_ENTER_CRITICAL() OSA_EnterCritical(&osaCurrentSr)
/*
 * Exit critical mode and retore the previous mode
 */
#define OSA_EXIT_CRITICAL() \
    __DSB();                \
    OSA_ExitCritical(osaCurrentSr);

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Reserves the requested amount of memory in bytes.
 *
 * The function is used to reserve the requested amount of memory in bytes and initializes it to 0.
 *
 * @param memLength Amount of bytes to reserve.
 *
 * @return Pointer to the reserved memory. NULL if memory can't be allocated.
 */
void *OSA_MemoryAllocate(uint32_t memLength);

/*!
 * @brief Frees the memory previously reserved.
 *
 * The function is used to free the memory block previously reserved.
 *
 * @param p Pointer to the start of the memory block previously reserved.
 *
 */
void OSA_MemoryFree(void *p);

/*!
 * @brief Reserves the requested amount of memory in bytes.
 *
 * The function is used to reserve the requested amount of memory in bytes and initializes it to 0.
 * The function allocates some extra memory to ensure that the return address is aligned on a alignbytes boundary
 * and that the memory size is a multiple of alignbytes.
 *
 * @param memLength Amount of bytes to reserve.
 * @param alignbytes Bytes boundary.
 *
 * @return Pointer to the reserved memory. NULL if memory can't be allocated.
 */
void *OSA_MemoryAllocateAlign(uint32_t memLength, uint32_t alignbytes);

/*!
 * @brief Frees the memory previously reserved.
 *
 * The function is used to free the memory block previously reserved.
 *
 * @param p Pointer to the start of the memory block previously reserved.
 *
 */
void OSA_MemoryFreeAlign(void *p);

/*!
 * @brief Enter critical with nesting mode.
 *
 * @param sr Store current status and return to caller.
 */
void OSA_EnterCritical(uint32_t *sr);

/*!
 * @brief Exit critical with nesting mode.
 *
 * @param sr Previous status to restore.
 */
void OSA_ExitCritical(uint32_t sr);

/*!
 * @name Task management
 * @{
 */

/*!
 * @brief Initialize OSA.
 *
 * This function is used to setup the basic services.
 *
 * Example below shows how to use this API to create the task handle.
 * @code
 *   OSA_Init();
 * @endcode
 */
#if (defined(FSL_OSA_TASK_ENABLE) && (FSL_OSA_TASK_ENABLE > 0U))
void OSA_Init(void);
#endif

/*!
 * @brief Start OSA schedule.
 *
 * This function is used to start OSA scheduler.
 *
 * Example below shows how to use this API to start osa schedule.
 * @code
 *   OSA_Start();
 * @endcode
 */
#if (defined(FSL_OSA_TASK_ENABLE) && (FSL_OSA_TASK_ENABLE > 0U))
void OSA_Start(void);
#endif

#if ((defined(FSL_OSA_TASK_ENABLE)) && (FSL_OSA_TASK_ENABLE > 0U))
/*!
 * @brief Creates a task.
 *
 * This function is used to create task based on the resources defined
 * by the macro OSA_TASK_DEFINE.
 *
 * Example below shows how to use this API to create the task handle.
 * @code
 *   OSA_TASK_HANDLE_DEFINE(taskHandle);
 *   OSA_TASK_DEFINE( Job1, OSA_PRIORITY_HIGH, 1, 800, 0);
 *   OSA_TaskCreate((osa_task_handle_t)taskHandle, OSA_TASK(Job1), (osa_task_param_t)NULL);
 * @endcode
 *
 * @param taskHandle Pointer to a memory space of size OSA_TASK_HANDLE_SIZE allocated by the caller, task handle.
 * The handle should be 4 byte aligned, because unaligned access doesn't be supported on some devices.
 * You can define the handle in the following two ways:
 * #OSA_TASK_HANDLE_DEFINE(taskHandle);
 * or
 * uint32_t taskHandle[((OSA_TASK_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t))];
 * @param thread_def pointer to theosa_task_def_t structure which defines the task.
 * @param task_param Pointer to be passed to the task when it is created.
 * @retval KOSA_StatusSuccess The task is successfully created.
 * @retval KOSA_StatusError   The task can not be created.
 */
osa_status_t OSA_TaskCreate(osa_task_handle_t taskHandle,
                            const osa_task_def_t *thread_def,
                            osa_task_param_t task_param);
#endif /* FSL_OSA_TASK_ENABLE */

#if ((defined(FSL_OSA_TASK_ENABLE)) && (FSL_OSA_TASK_ENABLE > 0U))
/*!
 * @brief Gets the handler of active task.
 *
 * @return Handler to current active task.
 */
osa_task_handle_t OSA_TaskGetCurrentHandle(void);
#endif /* FSL_OSA_TASK_ENABLE */

#if ((defined(FSL_OSA_TASK_ENABLE)) && (FSL_OSA_TASK_ENABLE > 0U))
/*!
 * @brief Puts the active task to the end of scheduler's queue.
 *
 * When a task calls this function, it gives up the CPU and puts itself to the
 * end of a task ready list.
 *
 * @retval NULL
 */
void OSA_TaskYield(void);
#endif /* FSL_OSA_TASK_ENABLE */

#if ((defined(FSL_OSA_TASK_ENABLE)) && (FSL_OSA_TASK_ENABLE > 0U))
/*!
 * @brief Gets the priority of a task.
 *
 * @param taskHandle The handler of the task whose priority is received.
 *
 * @return Task's priority.
 */
osa_task_priority_t OSA_TaskGetPriority(osa_task_handle_t taskHandle);
#endif /* FSL_OSA_TASK_ENABLE */

#if ((defined(FSL_OSA_TASK_ENABLE)) && (FSL_OSA_TASK_ENABLE > 0U))
/*!
 * @brief Sets the priority of a task.
 *
 * @param taskHandle  The handler of the task whose priority is set.
 * @param taskPriority The priority to set.
 *
 * @retval KOSA_StatusSuccess Task's priority is set successfully.
 * @retval KOSA_StatusError   Task's priority can not be set.
 */
osa_status_t OSA_TaskSetPriority(osa_task_handle_t taskHandle, osa_task_priority_t taskPriority);
#endif /* FSL_OSA_TASK_ENABLE */

#if ((defined(FSL_OSA_TASK_ENABLE)) && (FSL_OSA_TASK_ENABLE > 0U))
/*!
 * @brief Destroys a previously created task.
 *
 * @param taskHandle The handler of the task to destroy.
 *
 * @retval KOSA_StatusSuccess The task was successfully destroyed.
 * @retval KOSA_StatusError   Task destruction failed or invalid parameter.
 */
osa_status_t OSA_TaskDestroy(osa_task_handle_t taskHandle);
#endif /* FSL_OSA_TASK_ENABLE */

#if (defined(FSL_OSA_TASK_ENABLE) && (FSL_OSA_TASK_ENABLE > 0U))
/**
 * @brief Waits for task notification.
 *
 * This function waits for task notification from other task or interrupt
 * context.
 *
 * @param[in] waitTime_ms Timeout specified in milliseconds.
 *
 * @return KOSA_StatusSuccess When notification is successful
 * @return KOSA_StatusTimeout When timeout occurs
 * @return KOSA_StatusError   On failure
 */
osa_status_t OSA_TaskNotifyGet(osa_notify_time_ms_t waitTime_ms);

/**
 * @brief Sends task notification
 *
 * This function gives task notification so that waiting task can be
 * unblocked.
 *
 * @param[in] task Task handle to be notified
 *
 * @return KOSA_StatusSuccess When notification is successful
 * @return KOSA_StatusError   On failure
 */
osa_status_t OSA_TaskNotifyPost(osa_task_handle_t taskHandle);
#endif /* (defined(FSL_OSA_TASK_ENABLE) && (FSL_OSA_TASK_ENABLE > 0U)) */

/*!
 * @brief Pre-creates a semaphore.
 *
 * This function pre-creates a semaphore with the task handler.
 *
 * Example below shows how to use this API to create the semaphore handle.
 * @code
 *   OSA_SEMAPHORE_HANDLE_DEFINE(semaphoreHandle);
 *   OSA_SemaphoreCreate((osa_semaphore_handle_t)semaphoreHandle, (osa_task_ptr_t)taskHandler);
 * @endcode
 *
 * @param semaphoreHandle Pointer to a memory space of size OSA_SEM_HANDLE_SIZE allocated by the caller.
 * The handle should be 4 byte aligned, because unaligned access doesn't be supported on some devices.
 * You can define the handle in the following two ways:
 * #OSA_SEMAPHORE_HANDLE_DEFINE(semaphoreHandle);
 * or
 * uint32_t semaphoreHandle[((OSA_SEM_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t))];
 * @param taskHandler The task handler this semaphore is used by.
 *
 * @retval KOSA_StatusSuccess  the new semaphore if the semaphore is created successfully.
 */
osa_status_t OSA_SemaphorePrecreate(osa_semaphore_handle_t semaphoreHandle, osa_task_ptr_t taskHandler);

/*!
 * @brief Creates a semaphore with a given value.
 *
 * This function creates a semaphore and sets the value to the parameter
 * initValue.
 *
 * Example below shows how to use this API to create the semaphore handle.
 * @code
 *   OSA_SEMAPHORE_HANDLE_DEFINE(semaphoreHandle);
 *   OSA_SemaphoreCreate((osa_semaphore_handle_t)semaphoreHandle, 0xff);
 * @endcode
 *
 * @param semaphoreHandle Pointer to a memory space of size OSA_SEM_HANDLE_SIZE allocated by the caller.
 * The handle should be 4 byte aligned, because unaligned access doesn't be supported on some devices.
 * You can define the handle in the following two ways:
 * #OSA_SEMAPHORE_HANDLE_DEFINE(semaphoreHandle);
 * or
 * uint32_t semaphoreHandle[((OSA_SEM_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t))];
 * @param initValue Initial value the semaphore will be set to.
 *
 * @retval KOSA_StatusSuccess  the new semaphore if the semaphore is created successfully.
 * @retval KOSA_StatusError   if the semaphore can not be created.
 */
osa_status_t OSA_SemaphoreCreate(osa_semaphore_handle_t semaphoreHandle, uint32_t initValue);

/*!
 * @brief Creates a binary semaphore.
 *
 * This function creates a binary semaphore
 *
 * Example below shows how to use this API to create the semaphore handle.
 * @code
 *   OSA_SEMAPHORE_HANDLE_DEFINE(semaphoreHandle);
 *   OSA_SemaphoreCreateBinary((osa_semaphore_handle_t)semaphoreHandle);
 * @endcode
 *
 * @param semaphoreHandle Pointer to a memory space of size OSA_SEM_HANDLE_SIZE allocated by the caller.
 * The handle should be 4 byte aligned, because unaligned access doesn't be supported on some devices.
 * You can define the handle in the following two ways:
 * #OSA_SEMAPHORE_HANDLE_DEFINE(semaphoreHandle);
 * or
 * uint32_t semaphoreHandle[((OSA_SEM_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t))];
 *
 * @retval KOSA_StatusSuccess  the new binary semaphore if the binary semaphore is created successfully.
 * @retval KOSA_StatusError   if the binary semaphore can not be created.
 */
osa_status_t OSA_SemaphoreCreateBinary(osa_semaphore_handle_t semaphoreHandle);

/*!
 * @brief Destroys a previously created semaphore.
 *
 * @param semaphoreHandle The semaphore handle.
 * The macro SEMAPHORE_HANDLE_BUFFER_GET is used to get the semaphore buffer pointer,
 * and should not be used before the macro SEMAPHORE_HANDLE_BUFFER_DEFINE is used.
 *
 * @retval KOSA_StatusSuccess The semaphore is successfully destroyed.
 * @retval KOSA_StatusError   The semaphore can not be destroyed.
 */
osa_status_t OSA_SemaphoreDestroy(osa_semaphore_handle_t semaphoreHandle);

/*!
 * @brief Pending a semaphore with timeout.
 *
 * This function checks the semaphore's counting value. If it is positive,
 * decreases it and returns KOSA_StatusSuccess. Otherwise, a timeout is used
 * to wait.
 *
 * @param semaphoreHandle    The semaphore handle.
 * @param millisec The maximum number of milliseconds to wait if semaphore is not
 *                 positive. Pass osaWaitForever_c to wait indefinitely, pass 0
 *                 will return KOSA_StatusTimeout immediately.
 *
 * @retval KOSA_StatusSuccess  The semaphore is received.
 * @retval KOSA_StatusTimeout  The semaphore is not received within the specified 'timeout'.
 * @retval KOSA_StatusError    An incorrect parameter was passed.
 */
osa_status_t OSA_SemaphoreWait(osa_semaphore_handle_t semaphoreHandle, uint32_t millisec);

/*!
 * @brief Signals for someone waiting on the semaphore to wake up.
 *
 * Wakes up one task that is waiting on the semaphore. If no task is waiting, increases
 * the semaphore's counting value.
 *
 * @param semaphoreHandle The semaphore handle to signal.
 *
 * @retval KOSA_StatusSuccess The semaphore is successfully signaled.
 * @retval KOSA_StatusError   The object can not be signaled or invalid parameter.
 *
 */
osa_status_t OSA_SemaphorePost(osa_semaphore_handle_t semaphoreHandle);

/*!
 * @brief Gets the semaphore counter.
 *
 * @param semaphoreHandle The semaphore handle counter should be obtained from.
 *
 * @retval count The semaphore count.
 *
 */
osa_semaphore_count_t OSA_SemaphoreGetCount(osa_semaphore_handle_t semaphoreHandle);

/*!
 * @brief Create an unlocked mutex.
 *
 * This function creates a non-recursive mutex and sets it to unlocked status.
 *
 * Example below shows how to use this API to create the mutex handle.
 * @code
 *   OSA_MUTEX_HANDLE_DEFINE(mutexHandle);
 *   OSA_MutexCreate((osa_mutex_handle_t)mutexHandle);
 * @endcode
 *
 * @param mutexHandle       Pointer to a memory space of size OSA_MUTEX_HANDLE_SIZE allocated by the caller.
 * The handle should be 4 byte aligned, because unaligned access doesn't be supported on some devices.
 * You can define the handle in the following two ways:
 * #OSA_MUTEX_HANDLE_DEFINE(mutexHandle);
 * or
 * uint32_t mutexHandle[((OSA_MUTEX_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t))];
 * @retval KOSA_StatusSuccess  the new mutex if the mutex is created successfully.
 * @retval KOSA_StatusError   if the mutex can not be created.
 */
osa_status_t OSA_MutexCreate(osa_mutex_handle_t mutexHandle);

/*!
 * @brief Waits for a mutex and locks it.
 *
 * This function checks the mutex's status. If it is unlocked, locks it and returns the
 * KOSA_StatusSuccess. Otherwise, waits for a timeout in milliseconds to lock.
 *
 * @param mutexHandle The mutex handle.
 * @param millisec The maximum number of milliseconds to wait for the mutex.
 *                 If the mutex is locked, Pass the value osaWaitForever_c will
 *                 wait indefinitely, pass 0 will return KOSA_StatusTimeout
 *                 immediately.
 *
 * @retval KOSA_StatusSuccess The mutex is locked successfully.
 * @retval KOSA_StatusTimeout Timeout occurred.
 * @retval KOSA_StatusError   Incorrect parameter was passed.
 *
 * @note This is non-recursive mutex, a task can not try to lock the mutex it has locked.
 */
osa_status_t OSA_MutexLock(osa_mutex_handle_t mutexHandle, uint32_t millisec);

/*!
 * @brief Unlocks a previously locked mutex.
 *
 * @param mutexHandle The mutex handle.
 *
 * @retval KOSA_StatusSuccess The mutex is successfully unlocked.
 * @retval KOSA_StatusError   The mutex can not be unlocked or invalid parameter.
 */
osa_status_t OSA_MutexUnlock(osa_mutex_handle_t mutexHandle);

/*!
 * @brief Destroys a previously created mutex.
 *
 * @param mutexHandle The mutex handle.
 *
 * @retval KOSA_StatusSuccess The mutex is successfully destroyed.
 * @retval KOSA_StatusError   The mutex can not be destroyed.
 *
 */
osa_status_t OSA_MutexDestroy(osa_mutex_handle_t mutexHandle);

/*!
 * @brief Pre-initializes an event object.
 *
 * This function pre-creates an event object and indicates which task this event is used by.
 *
 * Example below shows how to use this API to create the event handle.
 * @code
 *   OSA_EVENT_HANDLE_DEFINE(eventHandle);
 *   OSA_EventPrecreate((osa_event_handle_t)eventHandle, (osa_task_ptr_t)taskHandler);
 * @endcode
 *
 * @param eventHandle Pointer to a memory space of size OSA_EVENT_HANDLE_SIZE allocated by the caller.
 * The handle should be 4 byte aligned, because unaligned access doesn't be supported on some devices.
 * You can define the handle in the following two ways:
 * #OSA_EVENT_HANDLE_DEFINE(eventHandle);
 * or
 * uint32 eventHandle[((OSA_EVENT_HANDLE_SIZE + sizeof(uint32) - 1U) / sizeof(uint32))];
 * @param taskHandler The task handler this event is used by.
 * @retval KOSA_StatusSuccess  the new event if the event is pre-created successfully.
 */
osa_status_t OSA_EventPrecreate(osa_event_handle_t eventHandle, osa_task_ptr_t taskHandler);

/*!
 * @brief Initializes an event object with all flags cleared.
 *
 * This function creates an event object and set its clear mode. If autoClear
 * is 1, when a task gets the event flags, these flags will be
 * cleared automatically. Otherwise these flags must
 * be cleared manually.
 *
 * Example below shows how to use this API to create the event handle.
 * @code
 *   OSA_EVENT_HANDLE_DEFINE(eventHandle);
 *   OSA_EventCreate((osa_event_handle_t)eventHandle, 0);
 * @endcode
 *
 * @param eventHandle Pointer to a memory space of size OSA_EVENT_HANDLE_SIZE allocated by the caller.
 * The handle should be 4 byte aligned, because unaligned access doesn't be supported on some devices.
 * You can define the handle in the following two ways:
 * #OSA_EVENT_HANDLE_DEFINE(eventHandle);
 * or
 * uint32_t eventHandle[((OSA_EVENT_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t))];
 * @param autoClear 1 The event is auto-clear.
 *                  0 The event manual-clear
 * @retval KOSA_StatusSuccess  the new event if the event is created successfully.
 * @retval KOSA_StatusError   if the event can not be created.
 */
osa_status_t OSA_EventCreate(osa_event_handle_t eventHandle, uint8_t autoClear);

/*!
 * @brief Sets one or more event flags.
 *
 * Sets specified flags of an event object.
 *
 * @param eventHandle     The event handle.
 * @param flagsToSet  Flags to be set.
 *
 * @retval KOSA_StatusSuccess The flags were successfully set.
 * @retval KOSA_StatusError   An incorrect parameter was passed.
 */
osa_status_t OSA_EventSet(osa_event_handle_t eventHandle, osa_event_flags_t flagsToSet);

/*!
 * @brief Clears one or more flags.
 *
 * Clears specified flags of an event object.
 *
 * @param eventHandle       The event handle.
 * @param flagsToClear  Flags to be clear.
 *
 * @retval KOSA_StatusSuccess The flags were successfully cleared.
 * @retval KOSA_StatusError   An incorrect parameter was passed.
 */
osa_status_t OSA_EventClear(osa_event_handle_t eventHandle, osa_event_flags_t flagsToClear);

/*!
 * @brief Get event's flags.
 *
 * Get specified flags of an event object.
 *
 * @param eventHandle       The event handle.
 * The macro EVENT_HANDLE_BUFFER_GET is used to get the event buffer pointer,
 * and should not be used before the macro EVENT_HANDLE_BUFFER_DEFINE is used.
 * @param flagsMask         The flags user want to get are specified by this parameter.
 * @param pFlagsOfEvent     The event flags are obtained by this parameter.
 *
 * @retval KOSA_StatusSuccess The event flags were successfully got.
 * @retval KOSA_StatusError   An incorrect parameter was passed.
 */
osa_status_t OSA_EventGet(osa_event_handle_t eventHandle,
                          osa_event_flags_t flagsMask,
                          osa_event_flags_t *pFlagsOfEvent);

/*!
 * @brief Waits for specified event flags to be set.
 *
 * This function waits for a combination of flags to be set in an event object.
 * Applications can wait for any/all bits to be set. Also this function could
 * obtain the flags who wakeup the waiting task.
 *
 * @param eventHandle     The event handle.
 * @param flagsToWait Flags that to wait.
 * @param waitAll     Wait all flags or any flag to be set.
 * @param millisec    The maximum number of milliseconds to wait for the event.
 *                    If the wait condition is not met, pass osaWaitForever_c will
 *                    wait indefinitely, pass 0 will return KOSA_StatusTimeout
 *                    immediately.
 * @param pSetFlags    Flags that wakeup the waiting task are obtained by this parameter.
 *
 * @retval KOSA_StatusSuccess The wait condition met and function returns successfully.
 * @retval KOSA_StatusTimeout Has not met wait condition within timeout.
 * @retval KOSA_StatusError   An incorrect parameter was passed.

 *
 * @note    Please pay attention to the flags bit width, FreeRTOS uses the most
 *          significant 8 bis as control bits, so do not wait these bits while using
 *          FreeRTOS.
 *
 */
osa_status_t OSA_EventWait(osa_event_handle_t eventHandle,
                           osa_event_flags_t flagsToWait,
                           uint8_t waitAll,
                           uint32_t millisec,
                           osa_event_flags_t *pSetFlags);

/*!
 * @brief Destroys a previously created event object.
 *
 * @param eventHandle The event handle.
 *
 * @retval KOSA_StatusSuccess The event is successfully destroyed.
 * @retval KOSA_StatusError   Event destruction failed.
 */
osa_status_t OSA_EventDestroy(osa_event_handle_t eventHandle);

/*!
 * @brief Initializes a message queue.
 *
 * This function  allocates memory for and initializes a message queue. Message queue elements are hardcoded as void*.
 *
 * Example below shows how to use this API to create the massage queue handle.
 * @code
 *   OSA_MSGQ_HANDLE_DEFINE(msgqHandle);
 *   OSA_MsgQCreate((osa_msgq_handle_t)msgqHandle, 5U, sizeof(msg));
 * @endcode
 *
 * @param msgqHandle    Pointer to a memory space of size #(OSA_MSGQ_HANDLE_SIZE + msgNo*msgSize) on bare-matel,
 * FreeRTOS static allocation allocated by the caller and #(OSA_MSGQ_HANDLE_SIZE) on FreeRTOS dynamic allocation,
 * message queue handle. The handle should be 4 byte aligned, because unaligned access doesn't be supported on some
 * devices. You can define the handle in the following two ways: #OSA_MSGQ_HANDLE_DEFINE(msgqHandle); or For bm and
 * freertos static: uint32_t msgqHandle[((OSA_MSGQ_HANDLE_SIZE + msgNo*msgSize + sizeof(uint32_t) - 1U) /
 * sizeof(uint32_t))]; For freertos dynamic: uint32_t msgqHandle[((OSA_MSGQ_HANDLE_SIZE + sizeof(uint32_t) - 1U) /
 * sizeof(uint32_t))];
 * @param msgNo :number of messages the message queue should accommodate.
 * @param msgSize :size of a single message structure.
 *
 * @retval KOSA_StatusSuccess Message queue successfully Create.
 * @retval KOSA_StatusError     Message queue create failure.
 */
osa_status_t OSA_MsgQCreate(osa_msgq_handle_t msgqHandle, uint32_t msgNo, uint32_t msgSize);

/*!
 * @brief Puts a message at the end of the queue.
 *
 * This function puts a message to the end of the message queue. If the queue
 * is full, this function returns the KOSA_StatusError;
 *
 * @param msgqHandle  Message Queue handler.
 * @param pMessage Pointer to the message to be put into the queue.
 *
 * @retval KOSA_StatusSuccess Message successfully put into the queue.
 * @retval KOSA_StatusError   The queue was full or an invalid parameter was passed.
 */
osa_status_t OSA_MsgQPut(osa_msgq_handle_t msgqHandle, osa_msg_handle_t pMessage);

/*!
 * @brief Reads and remove a message at the head of the queue.
 *
 * This function gets a message from the head of the message queue. If the
 * queue is empty, timeout is used to wait.
 *
 * @param msgqHandle   Message Queue handler.
 * @param pMessage Pointer to a memory to save the message.
 * @param millisec The number of milliseconds to wait for a message. If the
 *                 queue is empty, pass osaWaitForever_c will wait indefinitely,
 *                 pass 0 will return KOSA_StatusTimeout immediately.
 *
 * @retval KOSA_StatusSuccess   Message successfully obtained from the queue.
 * @retval KOSA_StatusTimeout   The queue remains empty after timeout.
 * @retval KOSA_StatusError     Invalid parameter.
 */
osa_status_t OSA_MsgQGet(osa_msgq_handle_t msgqHandle, osa_msg_handle_t pMessage, uint32_t millisec);

/*!
 * @brief Get the available message
 *
 * This function is used to get the available message.
 *
 * @param msgqHandle Message Queue handler.
 *
 * @return Available message count
 */
int OSA_MsgQAvailableMsgs(osa_msgq_handle_t msgqHandle);

/*!
 * @brief Destroys a previously created queue.
 *
 * @param msgqHandle Message Queue handler.
 *
 * @retval KOSA_StatusSuccess The queue was successfully destroyed.
 * @retval KOSA_StatusError   Message queue destruction failed.
 */
osa_status_t OSA_MsgQDestroy(osa_msgq_handle_t msgqHandle);

/*!
 * @brief Enable all interrupts.
 */
void OSA_InterruptEnable(void);

/*!
 * @brief Disable all interrupts.
 */
void OSA_InterruptDisable(void);

/*!
 * @brief Enable all interrupts using PRIMASK.
 */
void OSA_EnableIRQGlobal(void);

/*!
 * @brief Disable all interrupts using PRIMASK.
 */
void OSA_DisableIRQGlobal(void);

/*!
 * @brief Disable the scheduling of any task.
 */
void OSA_DisableScheduler(void);

/*!
 * @brief Enable the scheduling of any task.
 */
void OSA_EnableScheduler(void);

/*!
 * @brief Delays execution for a number of milliseconds.
 *
 * @param millisec The time in milliseconds to wait.
 */
void OSA_TimeDelay(uint32_t millisec);

/*!
 * @brief This function gets current time in milliseconds.
 *
 * @retval current time in milliseconds
 */
uint32_t OSA_TimeGetMsec(void);

/*!
 * @brief Installs the interrupt handler.
 *
 * @param IRQNumber IRQ number of the interrupt.
 * @param handler The interrupt handler to install.
 */
void OSA_InstallIntHandler(uint32_t IRQNumber, void (*handler)(void));

/*! @}*/
#ifdef __cplusplus
}
#endif
/*! @}*/
#endif // _FSL_OS_ABSTRACTION_H_
