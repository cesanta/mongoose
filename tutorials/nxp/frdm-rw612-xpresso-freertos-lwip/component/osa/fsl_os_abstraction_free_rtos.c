/*! *********************************************************************************
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, 2019 NXP
 *
 *
 * This is the source file for the OS Abstraction layer for freertos.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "fsl_common.h"
#include "fsl_os_abstraction.h"
#include "fsl_os_abstraction_free_rtos.h"
#include <string.h>
#include "fsl_component_generic_list.h"

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */

/* Weak function. */
#if defined(__GNUC__)
#define __WEAK_FUNC __attribute__((weak))
#elif defined(__ICCARM__)
#define __WEAK_FUNC __weak
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
#define __WEAK_FUNC __attribute__((weak))
#endif

#define millisecToTicks(millisec) (((millisec) * (uint32_t)(configTICK_RATE_HZ) + 999U) / 1000U)
#define OSA_TICK_PERIOD_MS (1000U / (uint32_t)configTICK_RATE_HZ)

#ifdef DEBUG_ASSERT
#define OS_ASSERT(condition) \
    if (!(condition))        \
        while (1)            \
            ;
#else
#define OS_ASSERT(condition) (void)(condition);
#endif

/*! @brief Converts milliseconds to ticks*/
#define MSEC_TO_TICK(msec) \
    (((uint32_t)(msec) + 500uL / (uint32_t)configTICK_RATE_HZ) * (uint32_t)configTICK_RATE_HZ / 1000uL)
#define TICKS_TO_MSEC(tick) ((uint32_t)((uint64_t)(tick)*1000uL / (uint64_t)configTICK_RATE_HZ))

#define OSA_MEM_MAGIC_NUMBER (12345U)
#define OSA_MEM_SIZE_ALIGN(var, alignbytes) \
    ((unsigned int)((var) + ((alignbytes)-1U)) & (unsigned int)(~(unsigned int)((alignbytes)-1U)))

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef struct osa_freertos_task
{
    list_element_t link;
    TaskHandle_t taskHandle;
} osa_freertos_task_t;

typedef struct _osa_event_struct
{
    EventGroupHandle_t eventHandle; /* The event handle */
    uint8_t autoClear;              /*!< Auto clear or manual clear   */
} osa_event_struct_t;

/*! @brief State structure for bm osa manager. */
typedef struct _osa_state
{
#if (defined(FSL_OSA_TASK_ENABLE) && (FSL_OSA_TASK_ENABLE > 0U))
    list_label_t taskList;
#if (defined(FSL_OSA_MAIN_FUNC_ENABLE) && (FSL_OSA_MAIN_FUNC_ENABLE > 0U))
    OSA_TASK_HANDLE_DEFINE(mainTaskHandle);
#endif
#endif
    uint32_t basePriority;
    int32_t basePriorityNesting;
    uint32_t interruptDisableCount;
} osa_state_t;

/*! @brief Definition structure contains allocated memory information.*/
typedef struct _osa_mem_align_control_block
{
    uint16_t identifier; /*!< Identifier for the memory control block. */
    uint16_t offset;     /*!< offset from aligned address to real address */
} osa_mem_align_cb_t;

/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */
__WEAK_FUNC void main_task(void const *argument);
__WEAK_FUNC void main_task(void const *argument)
{
}

void startup_task(void *argument);

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */
const uint8_t gUseRtos_c = USE_RTOS; /* USE_RTOS = 0 for BareMetal and 1 for OS */

static osa_state_t s_osaState = {0};

/* Allocate the memory for the heap. */
#if (defined(FSL_OSA_ALLOCATED_HEAP) && (FSL_OSA_ALLOCATED_HEAP > 0U))
#if defined(configAPPLICATION_ALLOCATED_HEAP) && (configAPPLICATION_ALLOCATED_HEAP)
#if defined(DATA_SECTION_IS_CACHEABLE) && (DATA_SECTION_IS_CACHEABLE)
extern uint8_t ucHeap[configTOTAL_HEAP_SIZE];
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t ucHeap[configTOTAL_HEAP_SIZE], 4);
#else
extern uint8_t ucHeap[configTOTAL_HEAP_SIZE];
SDK_ALIGN(uint8_t ucHeap[configTOTAL_HEAP_SIZE], 4);
#endif /* DATA_SECTION_IS_CACHEABLE */
#endif /* configAPPLICATION_ALLOCATED_HEAP */
#endif /* FSL_OSA_ALLOCATED_HEAP */

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MemoryAllocate
 * Description   : Reserves the requested amount of memory in bytes.
 *
 *END**************************************************************************/
void *OSA_MemoryAllocate(uint32_t memLength)
{
#if defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0)

    void *p = (void *)pvPortMalloc(memLength);

    if (NULL != p)
    {
        (void)memset(p, 0, memLength);
    }

    return p;
#else
    return NULL;
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MemoryFree
 * Description   : Frees the memory previously reserved.
 *
 *END**************************************************************************/
void OSA_MemoryFree(void *p)
{
#if defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0)
    vPortFree(p);
#endif
}

void *OSA_MemoryAllocateAlign(uint32_t memLength, uint32_t alignbytes)
{
    osa_mem_align_cb_t *p_cb = NULL;
    uint32_t alignedsize;

    /* Check overflow. */
    alignedsize = (uint32_t)(unsigned int)OSA_MEM_SIZE_ALIGN(memLength, alignbytes);
    if (alignedsize < memLength)
    {
        return NULL;
    }

    if (alignedsize > 0xFFFFFFFFU - alignbytes - sizeof(osa_mem_align_cb_t))
    {
        return NULL;
    }

    alignedsize += alignbytes + (uint32_t)sizeof(osa_mem_align_cb_t);

    union
    {
        void *pointer_value;
        uintptr_t unsigned_value;
    } p_align_addr, p_addr;

    p_addr.pointer_value = OSA_MemoryAllocate(alignedsize);

    if (p_addr.pointer_value == NULL)
    {
        return NULL;
    }

    p_align_addr.unsigned_value = OSA_MEM_SIZE_ALIGN(p_addr.unsigned_value + sizeof(osa_mem_align_cb_t), alignbytes);

    p_cb             = (osa_mem_align_cb_t *)(p_align_addr.unsigned_value - 4U);
    p_cb->identifier = OSA_MEM_MAGIC_NUMBER;
    p_cb->offset     = (uint16_t)(p_align_addr.unsigned_value - p_addr.unsigned_value);

    return p_align_addr.pointer_value;
}

void OSA_MemoryFreeAlign(void *p)
{
    union
    {
        void *pointer_value;
        uintptr_t unsigned_value;
    } p_free;
    p_free.pointer_value     = p;
    osa_mem_align_cb_t *p_cb = (osa_mem_align_cb_t *)(p_free.unsigned_value - 4U);

    if (p_cb->identifier != OSA_MEM_MAGIC_NUMBER)
    {
        return;
    }

    p_free.unsigned_value = p_free.unsigned_value - p_cb->offset;

    OSA_MemoryFree(p_free.pointer_value);
}

void OSA_EnterCritical(uint32_t *sr)
{
#if defined(__GIC_PRIO_BITS)
    if ((__get_CPSR() & CPSR_M_Msk) == 0x13)
#else
    if (0U != __get_IPSR())
#endif
    {
        *sr = portSET_INTERRUPT_MASK_FROM_ISR();
    }
    else
    {
        portENTER_CRITICAL();
    }
}

void OSA_ExitCritical(uint32_t sr)
{
#if defined(__GIC_PRIO_BITS)
    if ((__get_CPSR() & CPSR_M_Msk) == 0x13)
#else
    if (0U != __get_IPSR())
#endif
    {
        portCLEAR_INTERRUPT_MASK_FROM_ISR(sr);
    }
    else
    {
        portEXIT_CRITICAL();
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : startup_task
 * Description   : Wrapper over main_task..
 *
 *END**************************************************************************/
void startup_task(void *argument)
{
    main_task(argument);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskGetCurrentHandle
 * Description   : This function is used to get current active task's handler.
 *
 *END**************************************************************************/
#if (defined(FSL_OSA_TASK_ENABLE) && (FSL_OSA_TASK_ENABLE > 0U))
osa_task_handle_t OSA_TaskGetCurrentHandle(void)
{
    list_element_handle_t list_element;
    osa_freertos_task_t *ptask;

    list_element = LIST_GetHead(&s_osaState.taskList);
    while (NULL != list_element)
    {
        ptask = (osa_freertos_task_t *)(void *)list_element;
        if (ptask->taskHandle == xTaskGetCurrentTaskHandle())
        {
            return (osa_task_handle_t)ptask;
        }
        list_element = LIST_GetNext(list_element);
    }
    return NULL;
}
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskYield
 * Description   : When a task calls this function, it will give up CPU and put
 * itself to the tail of ready list.
 *
 *END**************************************************************************/
#if (defined(FSL_OSA_TASK_ENABLE) && (FSL_OSA_TASK_ENABLE > 0U))
void OSA_TaskYield(void)
{
    taskYIELD();
}
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskGetPriority
 * Description   : This function returns task's priority by task handler.
 *
 *END**************************************************************************/
#if (defined(FSL_OSA_TASK_ENABLE) && (FSL_OSA_TASK_ENABLE > 0U))
osa_task_priority_t OSA_TaskGetPriority(osa_task_handle_t taskHandle)
{
    assert(NULL != taskHandle);
    osa_freertos_task_t *ptask = (osa_freertos_task_t *)taskHandle;
    return (osa_task_priority_t)(PRIORITY_RTOS_TO_OSA((uxTaskPriorityGet(ptask->taskHandle))));
}
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskSetPriority
 * Description   : This function sets task's priority by task handler.
 *
 *END**************************************************************************/
#if (defined(FSL_OSA_TASK_ENABLE) && (FSL_OSA_TASK_ENABLE > 0U))
osa_status_t OSA_TaskSetPriority(osa_task_handle_t taskHandle, osa_task_priority_t taskPriority)
{
    assert(NULL != taskHandle);
    osa_freertos_task_t *ptask = (osa_freertos_task_t *)taskHandle;
    vTaskPrioritySet((task_handler_t)ptask->taskHandle, PRIORITY_OSA_TO_RTOS(((uint32_t)taskPriority)));
    return KOSA_StatusSuccess;
}
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskCreate
 * Description   : This function is used to create a task and make it ready.
 * Param[in]     :  threadDef  - Definition of the thread.
 *                  task_param - Parameter to pass to the new thread.
 * Return Thread handle of the new thread, or NULL if failed.
 *
 *END**************************************************************************/
#if (defined(FSL_OSA_TASK_ENABLE) && (FSL_OSA_TASK_ENABLE > 0U))
osa_status_t OSA_TaskCreate(osa_task_handle_t taskHandle, const osa_task_def_t *thread_def, osa_task_param_t task_param)
{
    osa_status_t status = KOSA_StatusError;
#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
    assert((sizeof(osa_freertos_task_t) + sizeof(StaticTask_t)) <= OSA_TASK_HANDLE_SIZE);
#else
    assert(sizeof(osa_freertos_task_t) == OSA_TASK_HANDLE_SIZE);
#endif
    assert(NULL != taskHandle);
#if defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0)
    TaskHandle_t pxCreatedTask;
#endif
    osa_freertos_task_t *ptask = (osa_freertos_task_t *)taskHandle;
#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
    TaskHandle_t xHandle = NULL;
#endif
    OSA_InterruptDisable();
#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
    xHandle =
        xTaskCreateStatic((TaskFunction_t)thread_def->pthread, /* pointer to the task */
                          (const char *)thread_def->tname,     /* task name for kernel awareness debugging */
                          (uint32_t)((uint16_t)thread_def->stacksize / sizeof(portSTACK_TYPE)), /* task stack size */
                          (task_param_t)task_param,                      /* optional task startup argument */
                          PRIORITY_OSA_TO_RTOS((thread_def->tpriority)), /* initial priority */
                          (StackType_t *)thread_def->tstack,             /*Array to use as the task's stack*/
                          (StaticTask_t *)((uint8_t *)(taskHandle) +
                                           sizeof(osa_freertos_task_t))  /*Variable to hold the task's data structure*/
        );
    if (xHandle != NULL)
    {
        ptask->taskHandle = xHandle;
        (void)LIST_AddTail(&s_osaState.taskList, (list_element_handle_t) & (ptask->link));
        status = KOSA_StatusSuccess;
    }
#else
    if (xTaskCreate(
            (TaskFunction_t)thread_def->pthread, /* pointer to the task */
            (char const *)thread_def->tname,     /* task name for kernel awareness debugging */
            (configSTACK_DEPTH_TYPE)((uint16_t)thread_def->stacksize / sizeof(portSTACK_TYPE)), /* task stack size */
            (task_param_t)task_param,                      /* optional task startup argument */
            PRIORITY_OSA_TO_RTOS((thread_def->tpriority)), /* initial priority */
            &pxCreatedTask                                 /* optional task handle to create */
            ) == pdPASS)
    {
        ptask->taskHandle = pxCreatedTask;

        (void)LIST_AddTail(&s_osaState.taskList, (list_element_handle_t) & (ptask->link));

        status = KOSA_StatusSuccess;
    }
#endif
    OSA_InterruptEnable();
    return status;
}
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TaskDestroy
 * Description   : This function destroy a task.
 * Param[in]     :taskHandle - Thread handle.
 * Return KOSA_StatusSuccess if the task is destroied, otherwise return KOSA_StatusError.
 *
 *END**************************************************************************/
#if (defined(FSL_OSA_TASK_ENABLE) && (FSL_OSA_TASK_ENABLE > 0U))
osa_status_t OSA_TaskDestroy(osa_task_handle_t taskHandle)
{
    assert(NULL != taskHandle);
    osa_freertos_task_t *ptask = (osa_freertos_task_t *)taskHandle;
    osa_status_t status;
    UBaseType_t oldPriority;

    /*Change priority to avoid context switches*/
    oldPriority = uxTaskPriorityGet(xTaskGetCurrentTaskHandle());
    vTaskPrioritySet(xTaskGetCurrentTaskHandle(), (configMAX_PRIORITIES - 1));
#if INCLUDE_vTaskDelete /* vTaskDelete() enabled */
    vTaskDelete((task_handler_t)ptask->taskHandle);
    status = KOSA_StatusSuccess;
#else
    status = KOSA_StatusError; /* vTaskDelete() not available */
#endif
    vTaskPrioritySet(xTaskGetCurrentTaskHandle(), oldPriority);
    OSA_InterruptDisable();
    (void)LIST_RemoveElement(taskHandle);
    OSA_InterruptEnable();
    return status;
}
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TimeDelay
 * Description   : This function is used to suspend the active thread for the given number of milliseconds.
 *
 *END**************************************************************************/
void OSA_TimeDelay(uint32_t millisec)
{
    vTaskDelay(millisecToTicks(millisec));
}
/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_TimeGetMsec
 * Description   : This function gets current time in milliseconds.
 *
 *END**************************************************************************/
uint32_t OSA_TimeGetMsec(void)
{
    TickType_t ticks;

    if (0U != __get_IPSR())
    {
        ticks = xTaskGetTickCountFromISR();
    }
    else
    {
        ticks = xTaskGetTickCount();
    }

    return TICKS_TO_MSEC(ticks);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_SemaphorePrecreate
 * Description   : This function is used to pre-create a semaphore.
 * Return         : KOSA_StatusSuccess
 *
 *END**************************************************************************/

osa_status_t OSA_SemaphorePrecreate(osa_semaphore_handle_t semaphoreHandle, osa_task_ptr_t taskHandler)
{
    semaphoreHandle = semaphoreHandle;
    taskHandler     = taskHandler;
    return KOSA_StatusSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_SemaphoreCreate
 * Description   : This function is used to create a semaphore.
 * Return         : Semaphore handle of the new semaphore, or NULL if failed.
 *
 *END**************************************************************************/
osa_status_t OSA_SemaphoreCreate(osa_semaphore_handle_t semaphoreHandle, uint32_t initValue)
{
#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
    assert((sizeof(osa_semaphore_handle_t) + sizeof(StaticQueue_t)) == OSA_SEM_HANDLE_SIZE);
#else
    assert(sizeof(osa_semaphore_handle_t) == OSA_SEM_HANDLE_SIZE);
#endif
    assert(NULL != semaphoreHandle);

    union
    {
        QueueHandle_t sem;
        uint32_t semhandle;
    } xSemaHandle;
#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
    xSemaHandle.sem = xSemaphoreCreateCountingStatic(
        0xFF, initValue, (StaticQueue_t *)(void *)((uint8_t *)semaphoreHandle + sizeof(osa_semaphore_handle_t)));
#else
    xSemaHandle.sem = xSemaphoreCreateCounting(0xFF, initValue);
#endif
    if (NULL != xSemaHandle.sem)
    {
        *(uint32_t *)semaphoreHandle = xSemaHandle.semhandle;
        return KOSA_StatusSuccess;
    }
    return KOSA_StatusError;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_SemaphoreCreateBinary
 * Description   : This function is used to create a binary semaphore.
 * Return        : Semaphore handle of the new binary semaphore, or NULL if failed.
 *
 *END**************************************************************************/
osa_status_t OSA_SemaphoreCreateBinary(osa_semaphore_handle_t semaphoreHandle)
{
#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
    assert((sizeof(osa_semaphore_handle_t) + sizeof(StaticQueue_t)) == OSA_SEM_HANDLE_SIZE);
#else
    assert(sizeof(osa_semaphore_handle_t) == OSA_SEM_HANDLE_SIZE);
#endif
    assert(NULL != semaphoreHandle);

    union
    {
        QueueHandle_t sem;
        uint32_t semhandle;
    } xSemaHandle;
#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
    xSemaHandle.sem = xSemaphoreCreateBinaryStatic(
        (StaticQueue_t *)(void *)((uint8_t *)semaphoreHandle + sizeof(osa_semaphore_handle_t)));
#else
    xSemaHandle.sem = xSemaphoreCreateBinary();
#endif
    if (NULL != xSemaHandle.sem)
    {
        *(uint32_t *)semaphoreHandle = xSemaHandle.semhandle;
        return KOSA_StatusSuccess;
    }
    return KOSA_StatusError;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_SemaphoreDestroy
 * Description   : This function is used to destroy a semaphore.
 * Return        : KOSA_StatusSuccess if the semaphore is destroyed successfully, otherwise return KOSA_StatusError.
 *
 *END**************************************************************************/
osa_status_t OSA_SemaphoreDestroy(osa_semaphore_handle_t semaphoreHandle)
{
    assert(NULL != semaphoreHandle);
    QueueHandle_t sem = (QueueHandle_t)(void *)(uint32_t *)(*(uint32_t *)semaphoreHandle);

    vSemaphoreDelete(sem);
    return KOSA_StatusSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_SemaphoreWait
 * Description   : This function checks the semaphore's counting value, if it is
 * positive, decreases it and returns KOSA_StatusSuccess, otherwise, timeout
 * will be used for wait. The parameter timeout indicates how long should wait
 * in milliseconds. Pass osaWaitForever_c to wait indefinitely, pass 0 will
 * return KOSA_StatusTimeout immediately if semaphore is not positive.
 * This function returns KOSA_StatusSuccess if the semaphore is received, returns
 * KOSA_StatusTimeout if the semaphore is not received within the specified
 * 'timeout', returns KOSA_StatusError if any errors occur during waiting.
 *
 *END**************************************************************************/
osa_status_t OSA_SemaphoreWait(osa_semaphore_handle_t semaphoreHandle, uint32_t millisec)
{
    uint32_t timeoutTicks;
    assert(NULL != semaphoreHandle);
    QueueHandle_t sem = (QueueHandle_t)(void *)(uint32_t *)(*(uint32_t *)semaphoreHandle);

    /* Convert timeout from millisecond to tick. */
    if (millisec == osaWaitForever_c)
    {
        timeoutTicks = portMAX_DELAY;
    }
    else
    {
        timeoutTicks = MSEC_TO_TICK(millisec);
    }

    if (((BaseType_t)0) == (BaseType_t)xSemaphoreTake(sem, timeoutTicks))
    {
        return KOSA_StatusTimeout; /* timeout */
    }
    else
    {
        return KOSA_StatusSuccess; /* semaphore taken */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_SemaphorePost
 * Description   : This function is used to wake up one task that wating on the
 * semaphore. If no task is waiting, increase the semaphore. The function returns
 * KOSA_StatusSuccess if the semaphre is post successfully, otherwise returns
 * KOSA_StatusError.
 *
 *END**************************************************************************/
osa_status_t OSA_SemaphorePost(osa_semaphore_handle_t semaphoreHandle)
{
    assert(NULL != semaphoreHandle);
    osa_status_t status = KOSA_StatusError;
    QueueHandle_t sem   = (QueueHandle_t)(void *)(uint32_t *)(*(uint32_t *)semaphoreHandle);

    if (0U != __get_IPSR())
    {
        portBASE_TYPE taskToWake = (portBASE_TYPE)pdFALSE;

        if (((BaseType_t)1) == (BaseType_t)xSemaphoreGiveFromISR(sem, &taskToWake))
        {
            portYIELD_FROM_ISR(((bool)(taskToWake)));
            status = KOSA_StatusSuccess;
        }
        else
        {
            status = KOSA_StatusError;
        }
    }
    else
    {
        if (((BaseType_t)1) == (BaseType_t)xSemaphoreGive(sem))
        {
            status = KOSA_StatusSuccess; /* sync object given */
        }
        else
        {
            status = KOSA_StatusError;
        }
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MutexCreate
 * Description   : This function is used to create a mutex.
 * Return        : Mutex handle of the new mutex, or NULL if failed.
 *
 *END**************************************************************************/
osa_status_t OSA_MutexCreate(osa_mutex_handle_t mutexHandle)
{
#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
    assert((sizeof(osa_mutex_handle_t) + sizeof(StaticQueue_t)) == OSA_MUTEX_HANDLE_SIZE);
#else
    assert(sizeof(osa_mutex_handle_t) == OSA_MUTEX_HANDLE_SIZE);
#endif
    assert(NULL != mutexHandle);

    union
    {
        QueueHandle_t mutex;
        uint32_t pmutexHandle;
    } xMutexHandle;
#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
    xMutexHandle.mutex = xSemaphoreCreateRecursiveMutexStatic(
        (StaticQueue_t *)(void *)((uint8_t *)mutexHandle + sizeof(osa_mutex_handle_t)));
#else
    xMutexHandle.mutex = xSemaphoreCreateRecursiveMutex();
#endif
    if (NULL != xMutexHandle.mutex)
    {
        *(uint32_t *)mutexHandle = xMutexHandle.pmutexHandle;
        return KOSA_StatusSuccess;
    }
    return KOSA_StatusError;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MutexLock
 * Description   : This function checks the mutex's status, if it is unlocked,
 * lock it and returns KOSA_StatusSuccess, otherwise, wait for the mutex.
 * This function returns KOSA_StatusSuccess if the mutex is obtained, returns
 * KOSA_StatusError if any errors occur during waiting. If the mutex has been
 * locked, pass 0 as timeout will return KOSA_StatusTimeout immediately.
 *
 *END**************************************************************************/
osa_status_t OSA_MutexLock(osa_mutex_handle_t mutexHandle, uint32_t millisec)
{
    assert(NULL != mutexHandle);
    uint32_t timeoutTicks;
    QueueHandle_t mutex = (QueueHandle_t)(void *)(uint32_t *)(*(uint32_t *)mutexHandle);

    /* Convert timeout from millisecond to tick. */
    if (millisec == osaWaitForever_c)
    {
        timeoutTicks = portMAX_DELAY;
    }
    else
    {
        timeoutTicks = MSEC_TO_TICK(millisec);
    }

    if (((BaseType_t)0) == (BaseType_t)xSemaphoreTakeRecursive(mutex, timeoutTicks))
    {
        return KOSA_StatusTimeout; /* timeout */
    }
    else
    {
        return KOSA_StatusSuccess; /* semaphore taken */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MutexUnlock
 * Description   : This function is used to unlock a mutex.
 *
 *END**************************************************************************/
osa_status_t OSA_MutexUnlock(osa_mutex_handle_t mutexHandle)
{
    assert(NULL != mutexHandle);
    QueueHandle_t mutex = (QueueHandle_t)(void *)(uint32_t *)(*(uint32_t *)mutexHandle);

    if (((BaseType_t)0) == (BaseType_t)xSemaphoreGiveRecursive(mutex))
    {
        return KOSA_StatusError;
    }
    else
    {
        return KOSA_StatusSuccess;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MutexDestroy
 * Description   : This function is used to destroy a mutex.
 * Return        : KOSA_StatusSuccess if the lock object is destroyed successfully, otherwise return KOSA_StatusError.
 *
 *END**************************************************************************/
osa_status_t OSA_MutexDestroy(osa_mutex_handle_t mutexHandle)
{
    assert(NULL != mutexHandle);
    QueueHandle_t mutex = (QueueHandle_t)(void *)(uint32_t *)(*(uint32_t *)mutexHandle);

    vSemaphoreDelete(mutex);
    return KOSA_StatusSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventPrecreate
 * Description   : This function is used to pre-create a event.
 * Return         : KOSA_StatusSuccess
 *
 *END**************************************************************************/

osa_status_t OSA_EventPrecreate(osa_event_handle_t eventHandle, osa_task_ptr_t taskHandler)
{
    eventHandle = eventHandle;
    taskHandler = taskHandler;
    return KOSA_StatusSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventCreate
 * Description   : This function is used to create a event object.
 * Return        : Event handle of the new event, or NULL if failed.
 *
 *END**************************************************************************/
osa_status_t OSA_EventCreate(osa_event_handle_t eventHandle, uint8_t autoClear)
{
    assert(NULL != eventHandle);
#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
    assert((sizeof(osa_event_struct_t) + sizeof(StaticEventGroup_t)) <= OSA_EVENT_HANDLE_SIZE);
#else
    assert(sizeof(osa_event_struct_t) == OSA_EVENT_HANDLE_SIZE);
#endif
    osa_event_struct_t *pEventStruct = (osa_event_struct_t *)eventHandle;

#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
    pEventStruct->eventHandle =
        xEventGroupCreateStatic((StaticEventGroup_t *)(void *)((uint8_t *)(eventHandle) + sizeof(osa_event_struct_t)));
#else
    pEventStruct->eventHandle = xEventGroupCreate();
#endif
    if (NULL != pEventStruct->eventHandle)
    {
        pEventStruct->autoClear = autoClear;
    }
    else
    {
        return KOSA_StatusError;
    }
    return KOSA_StatusSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventSet
 * Description   : Set one or more event flags of an event object.
 * Return        : KOSA_StatusSuccess if set successfully, KOSA_StatusError if failed.
 *
 *END**************************************************************************/
osa_status_t OSA_EventSet(osa_event_handle_t eventHandle, osa_event_flags_t flagsToSet)
{
    portBASE_TYPE taskToWake = (portBASE_TYPE)pdFALSE;
    BaseType_t result;
    assert(NULL != eventHandle);
    osa_event_struct_t *pEventStruct = (osa_event_struct_t *)eventHandle;

    if (NULL == pEventStruct->eventHandle)
    {
        return KOSA_StatusError;
    }
    if (0U != __get_IPSR())
    {
#if (configUSE_TRACE_FACILITY == 1)
        result = xEventGroupSetBitsFromISR(pEventStruct->eventHandle, (event_flags_t)flagsToSet, &taskToWake);
#else
        result = xEventGroupSetBitsFromISR((void *)pEventStruct->eventHandle, (event_flags_t)flagsToSet, &taskToWake);
#endif
        assert(pdPASS == result);
        (void)result;
        portYIELD_FROM_ISR(((bool)(taskToWake)));
    }
    else
    {
        (void)xEventGroupSetBits(pEventStruct->eventHandle, (event_flags_t)flagsToSet);
    }

    (void)result;
    return KOSA_StatusSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventClear
 * Description   : Clear one or more event flags of an event object.
 * Return        :KOSA_StatusSuccess if clear successfully, KOSA_StatusError if failed.
 *
 *END**************************************************************************/
osa_status_t OSA_EventClear(osa_event_handle_t eventHandle, osa_event_flags_t flagsToClear)
{
    assert(NULL != eventHandle);
    osa_event_struct_t *pEventStruct = (osa_event_struct_t *)eventHandle;

    if (NULL == pEventStruct->eventHandle)
    {
        return KOSA_StatusError;
    }

    if (0U != __get_IPSR())
    {
#if (configUSE_TRACE_FACILITY == 1)
        (void)xEventGroupClearBitsFromISR(pEventStruct->eventHandle, (event_flags_t)flagsToClear);
#else
        (void)xEventGroupClearBitsFromISR((void *)pEventStruct->eventHandle, (event_flags_t)flagsToClear);
#endif
    }
    else
    {
        (void)xEventGroupClearBits(pEventStruct->eventHandle, (event_flags_t)flagsToClear);
    }
    return KOSA_StatusSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventGet
 * Description   : This function is used to get event's flags that specified by prameter
 * flagsMask, and the flags (user specified) are obatianed by parameter pFlagsOfEvent. So
 * you should pass the parameter 0xffffffff to specify you want to check all.
 * Return        :KOSA_StatusSuccess if event flags were successfully got, KOSA_StatusError if failed.
 *
 *END**************************************************************************/
osa_status_t OSA_EventGet(osa_event_handle_t eventHandle, osa_event_flags_t flagsMask, osa_event_flags_t *pFlagsOfEvent)
{
    assert(NULL != eventHandle);
    osa_event_struct_t *pEventStruct = (osa_event_struct_t *)eventHandle;
    EventBits_t eventFlags;

    if (NULL == pEventStruct->eventHandle)
    {
        return KOSA_StatusError;
    }

    if (NULL == pFlagsOfEvent)
    {
        return KOSA_StatusError;
    }

    if (0U != __get_IPSR())
    {
        eventFlags = xEventGroupGetBitsFromISR(pEventStruct->eventHandle);
    }
    else
    {
        eventFlags = xEventGroupGetBits(pEventStruct->eventHandle);
    }

    *pFlagsOfEvent = (osa_event_flags_t)eventFlags & flagsMask;

    return KOSA_StatusSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventWait
 * Description   : This function checks the event's status, if it meets the wait
 * condition, return KOSA_StatusSuccess, otherwise, timeout will be used for
 * wait. The parameter timeout indicates how long should wait in milliseconds.
 * Pass osaWaitForever_c to wait indefinitely, pass 0 will return the value
 * KOSA_StatusTimeout immediately if wait condition is not met. The event flags
 * will be cleared if the event is auto clear mode. Flags that wakeup waiting
 * task could be obtained from the parameter setFlags.
 * This function returns KOSA_StatusSuccess if wait condition is met, returns
 * KOSA_StatusTimeout if wait condition is not met within the specified
 * 'timeout', returns KOSA_StatusError if any errors occur during waiting.
 *
 *END**************************************************************************/
osa_status_t OSA_EventWait(osa_event_handle_t eventHandle,
                           osa_event_flags_t flagsToWait,
                           uint8_t waitAll,
                           uint32_t millisec,
                           osa_event_flags_t *pSetFlags)
{
    assert(NULL != eventHandle);
    BaseType_t clearMode;
    uint32_t timeoutTicks;
    event_flags_t flagsSave;
    osa_event_struct_t *pEventStruct = (osa_event_struct_t *)eventHandle;

    /* Clean FreeRTOS cotrol flags */
    flagsToWait = flagsToWait & 0x00FFFFFFU;
    if (NULL == pEventStruct->eventHandle)
    {
        return KOSA_StatusError;
    }

    /* Convert timeout from millisecond to tick. */
    if (millisec == osaWaitForever_c)
    {
        timeoutTicks = portMAX_DELAY;
    }
    else
    {
        timeoutTicks = millisec / (uint32_t)(OSA_TICK_PERIOD_MS);
    }

    clearMode = (pEventStruct->autoClear != 0U) ? pdTRUE : pdFALSE;

    flagsSave = xEventGroupWaitBits(pEventStruct->eventHandle, (event_flags_t)flagsToWait, clearMode,
                                    (BaseType_t)waitAll, timeoutTicks);

    flagsSave &= (event_flags_t)flagsToWait;
    if (NULL != pSetFlags)
    {
        *pSetFlags = (osa_event_flags_t)flagsSave;
    }

    if (0U != flagsSave)
    {
        return KOSA_StatusSuccess;
    }
    else
    {
        return KOSA_StatusTimeout;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EventDestroy
 * Description   : This function is used to destroy a event object. Return
 * KOSA_StatusSuccess if the event object is destroyed successfully, otherwise
 * return KOSA_StatusError.
 *
 *END**************************************************************************/
osa_status_t OSA_EventDestroy(osa_event_handle_t eventHandle)
{
    assert(NULL != eventHandle);
    osa_event_struct_t *pEventStruct = (osa_event_struct_t *)eventHandle;

    if (NULL == pEventStruct->eventHandle)
    {
        return KOSA_StatusError;
    }
    vEventGroupDelete(pEventStruct->eventHandle);
    return KOSA_StatusSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MsgQCreate
 * Description   : This function is used to create a message queue.
 * Return        : the handle to the message queue if create successfully, otherwise
 * return NULL.
 *
 *END**************************************************************************/
osa_status_t OSA_MsgQCreate(osa_msgq_handle_t msgqHandle, uint32_t msgNo, uint32_t msgSize)
{
#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
    assert((sizeof(osa_msgq_handle_t) + sizeof(StaticQueue_t)) == OSA_MSGQ_HANDLE_SIZE);
#else
    assert(sizeof(osa_msgq_handle_t) == OSA_MSGQ_HANDLE_SIZE);
#endif
    assert(NULL != msgqHandle);

    union
    {
        QueueHandle_t msgq;
        uint32_t pmsgqHandle;
    } xMsgqHandle;

    /* Create the message queue where the number and size is specified by msgNo and msgSize */
#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
    xMsgqHandle.msgq = xQueueCreateStatic(
        msgNo, msgSize, (uint8_t *)((uint8_t *)msgqHandle + sizeof(osa_msgq_handle_t) + sizeof(StaticQueue_t)),
        (StaticQueue_t *)(void *)((uint8_t *)msgqHandle + sizeof(osa_msgq_handle_t)));
#else
    xMsgqHandle.msgq = xQueueCreate(msgNo, msgSize);
#endif
    if (NULL != xMsgqHandle.msgq)
    {
        *(uint32_t *)msgqHandle = xMsgqHandle.pmsgqHandle;
        return KOSA_StatusSuccess;
    }
    return KOSA_StatusError;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MsgQPut
 * Description   : This function is used to put a message to a message queue.
 * Return         : KOSA_StatusSuccess if the message is put successfully, otherwise return KOSA_StatusError.
 *
 *END**************************************************************************/
osa_status_t OSA_MsgQPut(osa_msgq_handle_t msgqHandle, osa_msg_handle_t pMessage)
{
    osa_status_t osaStatus;
    assert(NULL != msgqHandle);
    portBASE_TYPE taskToWake = (portBASE_TYPE)pdFALSE;
    QueueHandle_t handler    = (QueueHandle_t)(void *)(uint32_t *)(*(uint32_t *)msgqHandle);

    if (0U != __get_IPSR())
    {
        if (((BaseType_t)1) == (BaseType_t)xQueueSendToBackFromISR(handler, pMessage, &taskToWake))
        {
            portYIELD_FROM_ISR(((bool)(taskToWake)));
            osaStatus = KOSA_StatusSuccess;
        }
        else
        {
            osaStatus = KOSA_StatusError;
        }
    }
    else
    {
        osaStatus = (xQueueSendToBack(handler, pMessage, 0) == pdPASS) ? (KOSA_StatusSuccess) : (KOSA_StatusError);
    }

    return osaStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MsgQGet
 * Description   : This function checks the queue's status, if it is not empty,
 * get message from it and return KOSA_StatusSuccess, otherwise, timeout will
 * be used for wait. The parameter timeout indicates how long should wait in
 * milliseconds. Pass osaWaitForever_c to wait indefinitely, pass 0 will return
 * KOSA_StatusTimeout immediately if queue is empty.
 * This function returns KOSA_StatusSuccess if message is got successfully,
 * returns KOSA_StatusTimeout if message queue is empty within the specified
 * 'timeout', returns KOSA_StatusError if any errors occur during waiting.
 *
 *END**************************************************************************/
osa_status_t OSA_MsgQGet(osa_msgq_handle_t msgqHandle, osa_msg_handle_t pMessage, uint32_t millisec)
{
    osa_status_t osaStatus;
    assert(NULL != msgqHandle);
    QueueHandle_t handler = (QueueHandle_t)(void *)(uint32_t *)(*(uint32_t *)msgqHandle);

    uint32_t timeoutTicks;

    if (millisec == osaWaitForever_c)
    {
        timeoutTicks = portMAX_DELAY;
    }
    else
    {
        timeoutTicks = MSEC_TO_TICK(millisec);
    }
    if (pdPASS != xQueueReceive(handler, pMessage, timeoutTicks))
    {
        osaStatus = KOSA_StatusTimeout; /* not able to send it to the queue? */
    }
    else
    {
        osaStatus = KOSA_StatusSuccess;
    }
    return osaStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MsgQAvailableMsgs
 * Description   : This function is used to get the available message.
 * Return        : Available message count
 *
 *END**************************************************************************/
int OSA_MsgQAvailableMsgs(osa_msgq_handle_t msgqHandle)
{
    QueueHandle_t handler;
    assert(NULL != msgqHandle);
    handler = (QueueHandle_t)(void *)(uint32_t *)(*(uint32_t *)msgqHandle);
    return (int)uxQueueMessagesWaiting((QueueHandle_t)handler);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_MsgQDestroy
 * Description   : This function is used to destroy the message queue.
 * Return        : KOSA_StatusSuccess if the message queue is destroyed successfully, otherwise return KOSA_StatusError.
 *
 *END**************************************************************************/
osa_status_t OSA_MsgQDestroy(osa_msgq_handle_t msgqHandle)
{
    assert(NULL != msgqHandle);
    QueueHandle_t handler = (QueueHandle_t)(void *)(uint32_t *)(*(uint32_t *)msgqHandle);

    vQueueDelete(handler);
    return KOSA_StatusSuccess;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_InterruptEnable
 * Description   : self explanatory.
 *
 *END**************************************************************************/
void OSA_InterruptEnable(void)
{
    if (0U != __get_IPSR())
    {
        if (1 == s_osaState.basePriorityNesting)
        {
            portCLEAR_INTERRUPT_MASK_FROM_ISR(s_osaState.basePriority);
        }

        if (s_osaState.basePriorityNesting > 0)
        {
            s_osaState.basePriorityNesting--;
        }
    }
    else
    {
        portEXIT_CRITICAL();
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_InterruptDisable
 * Description   : self explanatory.
 *
 *END**************************************************************************/
void OSA_InterruptDisable(void)
{
    if (0U != __get_IPSR())
    {
        if (0 == s_osaState.basePriorityNesting)
        {
            s_osaState.basePriority = portSET_INTERRUPT_MASK_FROM_ISR();
        }
        s_osaState.basePriorityNesting++;
    }
    else
    {
        portENTER_CRITICAL();
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EnableIRQGlobal
 * Description   : enable interrupts using PRIMASK register.
 *
 *END**************************************************************************/
void OSA_EnableIRQGlobal(void)
{
    if (s_osaState.interruptDisableCount > 0U)
    {
        s_osaState.interruptDisableCount--;

        if (0U == s_osaState.interruptDisableCount)
        {
            __enable_irq();
        }
        /* call core API to enable the global interrupt*/
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_DisableIRQGlobal
 * Description   : disable interrupts using PRIMASK register.
 *
 *END**************************************************************************/
void OSA_DisableIRQGlobal(void)
{
    /* call core API to disable the global interrupt*/
    __disable_irq();

    /* update counter*/
    s_osaState.interruptDisableCount++;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_DisableScheduler
 * Description   : Disable the scheduling of any task
 * This function will disable the scheduling of any task
 *
 *END**************************************************************************/
void OSA_DisableScheduler(void)
{
    vTaskSuspendAll();
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_EnableScheduler
 * Description   : Enable the scheduling of any task
 * This function will enable the scheduling of any task
 *
 *END**************************************************************************/
void OSA_EnableScheduler(void)
{
    (void)xTaskResumeAll();
}

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_InstallIntHandler
 * Description   : This function is used to install interrupt handler.
 *
 *END**************************************************************************/
void OSA_InstallIntHandler(uint32_t IRQNumber, void (*handler)(void))
{
#if defined(__IAR_SYSTEMS_ICC__)
    _Pragma("diag_suppress = Pm138")
#endif
#if defined(ENABLE_RAM_VECTOR_TABLE)
        (void) InstallIRQHandler((IRQn_Type)IRQNumber, (uint32_t)handler);
#endif /* ENABLE_RAM_VECTOR_TABLE. */
#if defined(__IAR_SYSTEMS_ICC__)
    _Pragma("diag_remark = PM138")
#endif
}

/*!*********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************** */
#if (defined(FSL_OSA_TASK_ENABLE) && (FSL_OSA_TASK_ENABLE > 0U))
#if (defined(FSL_OSA_MAIN_FUNC_ENABLE) && (FSL_OSA_MAIN_FUNC_ENABLE > 0U))
static OSA_TASK_DEFINE(startup_task, gMainThreadPriority_c, 1, gMainThreadStackSize_c, 0);

int main(void)
{
    extern void BOARD_InitHardware(void);
    OSA_Init();
    /* Initialize MCU clock */
    BOARD_InitHardware();

    (void)OSA_TaskCreate((osa_task_handle_t)s_osaState.mainTaskHandle, OSA_TASK(startup_task), NULL);

    OSA_Start();
    return 0;
}
#endif /*(defined(FSL_OSA_MAIN_FUNC_ENABLE) && (FSL_OSA_MAIN_FUNC_ENABLE > 0U))*/
#endif /* FSL_OSA_TASK_ENABLE */

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_Init
 * Description   : This function is used to setup the basic services, it should
 * be called first in function main.
 *
 *END**************************************************************************/
#if (defined(FSL_OSA_TASK_ENABLE) && (FSL_OSA_TASK_ENABLE > 0U))
void OSA_Init(void)
{
    LIST_Init((&s_osaState.taskList), 0);
    s_osaState.basePriorityNesting   = 0;
    s_osaState.interruptDisableCount = 0;
}
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : OSA_Start
 * Description   : This function is used to start RTOS scheduler.
 *
 *END**************************************************************************/
#if (defined(FSL_OSA_TASK_ENABLE) && (FSL_OSA_TASK_ENABLE > 0U))
void OSA_Start(void)
{
    vTaskStartScheduler();
}
#endif

/**
 * Warning: Needs to be implemented
 */
#if (defined(FSL_OSA_TASK_ENABLE) && (FSL_OSA_TASK_ENABLE > 0U))
osa_status_t OSA_TaskNotifyGet(osa_notify_time_ms_t waitTime_ms)
{
    return KOSA_StatusError;
}
#endif

/**
 * Warning: Needs to be implemented
 */
#if (defined(FSL_OSA_TASK_ENABLE) && (FSL_OSA_TASK_ENABLE > 0U))
osa_status_t OSA_TaskNotifyPost(osa_task_handle_t taskHandle)
{
    return KOSA_StatusError;
}
#endif

/**
 * Warning: Needs to be implemented
 */
osa_semaphore_count_t OSA_SemaphoreGetCount(osa_semaphore_handle_t semaphoreHandle)
{
    assert(false);

    return 0;
}
