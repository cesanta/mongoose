/*
 * FreeRTOS Kernel V11.0.1
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/** @file osa_freertos.c
 *
 *  @brief OS FreeRTOS interaction APIs
 *
 */
#include <inttypes.h>
#include <stdio.h>
#include <osa.h>
#include <wmlog.h>

#if defined(SDK_OS_FREE_RTOS)

#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))

/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    /* If the buffers to be provided to the Idle task are declared inside this
    function then they must be declared static - otherwise they will be allocated on
    the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    /* If the buffers to be provided to the Timer task are declared inside this
    function then they must be declared static - otherwise they will be allocated on
    the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
#endif

osa_status_t OSA_TimerCreate(osa_timer_handle_t timerHandle,
                             osa_timer_tick ticks,
                             void (*call_back)(osa_timer_arg_t),
                             void *cb_arg,
                             osa_timer_t reload,
                             osa_timer_activate_t activate)
{
    int auto_reload = (reload == KOSA_TimerOnce) ? pdFALSE : pdTRUE;

#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
    assert((sizeof(osa_timer_handle_t) + sizeof(StaticTimer_t)) == OSA_TIMER_HANDLE_SIZE);
#else
    assert(sizeof(osa_timer_handle_t) == OSA_TIMER_HANDLE_SIZE);
#endif
    assert(NULL != timerHandle);

    union
    {
        TimerHandle_t tm;
        uint32_t timerhandle;
    } xTimerHandle;

#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))
    xTimerHandle.tm =
        xTimerCreateStatic(NULL, ticks, (UBaseType_t)auto_reload, cb_arg, call_back,
                           (StaticTimer_t *)(void *)((uint8_t *)timerHandle + sizeof(osa_timer_handle_t)));
#else
    xTimerHandle.tm = xTimerCreate(NULL, ticks, (UBaseType_t)auto_reload, cb_arg, call_back);
#endif
    if (NULL != xTimerHandle.tm)
    {
        *(uint32_t *)timerHandle = xTimerHandle.timerhandle;

        if (activate == OSA_TIMER_AUTO_ACTIVATE)
        {
            return OSA_TimerActivate(timerHandle);
        }

        return KOSA_StatusSuccess;
    }

    return KOSA_StatusError;
}

osa_status_t OSA_TimerActivate(osa_timer_handle_t timerHandle)
{
    assert(NULL != timerHandle);
    osa_status_t status = KOSA_StatusError;
    TimerHandle_t tm    = (TimerHandle_t)(void *)(uint32_t *)(*(uint32_t *)timerHandle);

    if (0U != __get_IPSR())
    {
        portBASE_TYPE taskToWake = (portBASE_TYPE)pdFALSE;

        if (((BaseType_t)1) == (BaseType_t)xTimerStartFromISR(tm, &taskToWake))
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
        if (((BaseType_t)1) == (BaseType_t)xTimerStart(tm, 0))
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

osa_status_t OSA_TimerChange(osa_timer_handle_t timerHandle, osa_timer_tick ntime, osa_timer_tick block_time)
{
    assert(NULL != timerHandle);
    osa_status_t status = KOSA_StatusError;
    TimerHandle_t tm    = (TimerHandle_t)(void *)(uint32_t *)(*(uint32_t *)timerHandle);

    if (0U != __get_IPSR())
    {
        portBASE_TYPE taskToWake = (portBASE_TYPE)pdFALSE;

        if (((BaseType_t)1) == (BaseType_t)xTimerChangePeriodFromISR(tm, ntime, &taskToWake))
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
        if (((BaseType_t)1) == (BaseType_t)xTimerChangePeriod(tm, ntime, 100))
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

bool OSA_TimerIsRunning(osa_timer_handle_t timerHandle)
{
    assert(NULL != timerHandle);
    TimerHandle_t tm = (TimerHandle_t)(void *)(uint32_t *)(*(uint32_t *)timerHandle);

    if (((BaseType_t)0) == (BaseType_t)xTimerIsTimerActive(tm))
    {
        return false; /* Timer not active */
    }
    else
    {
        return true; /* Timer active */
    }
}

void *OSA_TimerGetContext(osa_timer_handle_t timerHandle)
{
    assert(NULL != timerHandle);
    TimerHandle_t tm = (TimerHandle_t)(void *)(uint32_t *)(*(uint32_t *)timerHandle);

    return pvTimerGetTimerID(tm);
}

osa_status_t OSA_TimerReset(osa_timer_handle_t timerHandle)
{
    assert(NULL != timerHandle);
    osa_status_t status = KOSA_StatusError;
    TimerHandle_t tm    = (TimerHandle_t)(void *)(uint32_t *)(*(uint32_t *)timerHandle);

    if (0U != __get_IPSR())
    {
        portBASE_TYPE taskToWake = (portBASE_TYPE)pdFALSE;

        if (((BaseType_t)1) == (BaseType_t)xTimerResetFromISR(tm, &taskToWake))
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
        if (((BaseType_t)1) == (BaseType_t)xTimerReset(tm, 0))
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

osa_status_t OSA_TimerDeactivate(osa_timer_handle_t timerHandle)
{
    assert(NULL != timerHandle);
    osa_status_t status = KOSA_StatusError;
    TimerHandle_t tm    = (TimerHandle_t)(void *)(uint32_t *)(*(uint32_t *)timerHandle);

    if (0U != __get_IPSR())
    {
        portBASE_TYPE taskToWake = (portBASE_TYPE)pdFALSE;

        if (((BaseType_t)1) == (BaseType_t)xTimerStopFromISR(tm, &taskToWake))
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
        if (((BaseType_t)1) == (BaseType_t)xTimerStop(tm, 0))
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

osa_status_t OSA_TimerDestroy(osa_timer_handle_t timerHandle)
{
    assert(NULL != timerHandle);
    TimerHandle_t tm = (TimerHandle_t)(void *)(uint32_t *)(*(uint32_t *)timerHandle);
    int ret;

    /* Below timer handle invalidation needs to be protected as a context
     * switch may create issues if same handle is used before
     * invalidation.
     */
    OSA_SR_ALLOC();

    OSA_ENTER_CRITICAL();
    /* Note: Block time is set as 0, thus signifying non-blocking
       API. Can be changed later if required. */
    ret = xTimerDelete(tm, 0);

    OSA_EXIT_CRITICAL();

    return ret == pdPASS ? KOSA_StatusSuccess : KOSA_StatusError;
}

void (*g_os_tick_hooks[MAX_CUSTOM_HOOKS])(void) = {NULL};
void (*g_os_idle_hooks[MAX_CUSTOM_HOOKS])(void) = {NULL};

/** The FreeRTOS Tick hook function. */
void vApplicationTickHook(void)
{
    int i;

    for (i = 0; i < MAX_CUSTOM_HOOKS; i++)
    {
        if (g_os_tick_hooks[i] != NULL)
        {
            g_os_tick_hooks[i]();
        }
    }
}

void vApplicationIdleHook(void)
{
    int i;
    for (i = 0; i < MAX_CUSTOM_HOOKS; i++)
    {
        if (g_os_idle_hooks[i] != NULL)
        {
            g_os_idle_hooks[i]();
        }
    }
}

int OSA_SetupIdleFunction(void (*func)(void))
{
    unsigned int i;

    for (i = 0; i < MAX_CUSTOM_HOOKS; i++)
    {
        if (g_os_idle_hooks[i] != NULL && g_os_idle_hooks[i] == func)
        {
            return WM_SUCCESS;
        }
    }

    for (i = 0; i < MAX_CUSTOM_HOOKS; i++)
    {
        if (g_os_idle_hooks[i] == NULL)
        {
            g_os_idle_hooks[i] = func;
            break;
        }
    }

    if (i == MAX_CUSTOM_HOOKS)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

int OSA_SetupTickFunction(void (*func)(void))
{
    unsigned int i;

    for (i = 0; i < MAX_CUSTOM_HOOKS; i++)
    {
        if (g_os_tick_hooks[i] != NULL && g_os_tick_hooks[i] == func)
        {
            return WM_SUCCESS;
        }
    }

    for (i = 0; i < MAX_CUSTOM_HOOKS; i++)
    {
        if (g_os_tick_hooks[i] == NULL)
        {
            g_os_tick_hooks[i] = func;
            break;
        }
    }

    if (i == MAX_CUSTOM_HOOKS)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

int OSA_RemoveIdleFunction(void (*func)(void))
{
    unsigned int i;

    for (i = 0; i < MAX_CUSTOM_HOOKS; i++)
    {
        if (g_os_idle_hooks[i] == func)
        {
            g_os_idle_hooks[i] = NULL;
            break;
        }
    }

    if (i == MAX_CUSTOM_HOOKS)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

int OSA_RemoveTickFunction(void (*func)(void))
{
    unsigned int i;

    for (i = 0; i < MAX_CUSTOM_HOOKS; i++)
    {
        if (g_os_tick_hooks[i] == func)
        {
            g_os_tick_hooks[i] = NULL;
            break;
        }
    }

    if (i == MAX_CUSTOM_HOOKS)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

#if CONFIG_HEAP_STAT
/** This function dumps complete statistics
 *  of the heap memory.
 */
void OSA_DumpMemStats(void)
{
    OSA_SR_ALLOC()
    OSA_ENTER_CRITICAL();

    HeapStats_t HS;

    HS.xAvailableHeapSpaceInBytes      = 0;
    HS.xSizeOfLargestFreeBlockInBytes  = 0;
    HS.xSizeOfSmallestFreeBlockInBytes = 0;
    HS.xNumberOfFreeBlocks             = 0;
    HS.xNumberOfSuccessfulAllocations  = 0;
    HS.xNumberOfSuccessfulFrees        = 0;
    HS.xMinimumEverFreeBytesRemaining  = 0;

    vPortGetHeapStats(&HS);

    (void)PRINTF("\n\r");
    (void)PRINTF("Heap size ---------------------- : %d\n\r", HS.xAvailableHeapSpaceInBytes);
    (void)PRINTF("Largest Free Block size -------- : %d\n\r", HS.xSizeOfLargestFreeBlockInBytes);
    (void)PRINTF("Smallest Free Block size ------- : %d\n\r", HS.xSizeOfSmallestFreeBlockInBytes);
    (void)PRINTF("Number of Free Blocks ---------- : %d\n\r", HS.xNumberOfFreeBlocks);
    (void)PRINTF("Total successful allocations --- : %d\n\r", HS.xNumberOfSuccessfulAllocations);
    (void)PRINTF("Total successful frees --------- : %d\n\r", HS.xNumberOfSuccessfulFrees);
    (void)PRINTF("Min Free since system boot ----- : %d\n\r", HS.xMinimumEverFreeBytesRemaining);

    OSA_EXIT_CRITICAL()
}
#endif
void OSA_ThreadSelfComplete(osa_task_handle_t taskHandle)
{
    /* Suspend self until someone calls delete. This is required because in
     * freeRTOS, main functions of a thread cannot return.
     */
    if (taskHandle != NULL)
    {
        PRINTF("OSA: Thread Complete: %p\r\n", taskHandle);
        vTaskSuspend(taskHandle);
    }
    else
    {
        PRINTF("OSA: Thread Complete: SELF\r\n");
        vTaskSuspend(NULL);
    }

    /*
     * We do not want this function to return ever.
     */
    while (true)
    {
        OSA_TimeDelay(60000);
    }
}

/* the OS timer register is loaded with CNTMAX */
#define CNTMAX                 ((SystemCoreClock / configTICK_RATE_HZ) - 1UL)
#define CPU_CLOCK_TICKSPERUSEC (SystemCoreClock / 1000000U)
#define USECSPERTICK           (1000000U / configTICK_RATE_HZ)

/* returns time in micro-secs since time began */
unsigned int OSA_GetTimestamp(void)
{
    uint32_t nticks;
    uint32_t counter;

    vPortEnterCritical();
    nticks  = xTaskGetTickCount();
    counter = SysTick->VAL;

    /*
     * If this is called after SysTick counter
     * expired but before SysTick Handler got a
     * chance to run, then set the return value
     * to the start of next tick.
     */
    if ((SCB->ICSR & SCB_ICSR_PENDSTSET_Msk) != 0U)
    {
        nticks++;
        counter = CNTMAX;
    }

    vPortExitCritical();
    return ((CNTMAX - counter) / CPU_CLOCK_TICKSPERUSEC) + (nticks * USECSPERTICK);
}

uint32_t OSA_MsgQWaiting(osa_msgq_handle_t msgqHandle)
{
    uint32_t nmsg = 0;
    assert(NULL != msgqHandle);
    QueueHandle_t handler = (QueueHandle_t)(void *)(uint32_t *)(*(uint32_t *)msgqHandle);

    nmsg = (uint32_t)uxQueueMessagesWaiting(handler);

    return nmsg;
}
#endif
