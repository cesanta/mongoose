/*
 *  Copyright 2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef _OSA_FREERTOS_H_
#define _OSA_FREERTOS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include "portmacro.h"

#ifndef configMAX_PRIORITIES
#error Define configMAX_PRIORITIES in FreeRTOSConfig.h
#elif configMAX_PRIORITIES < 5
#error configMAX_PRIORITIES must be defined to be greater than or equal to 5
#endif
/*** Priority setting ***/
#define OSA_PRIORITY(x) (PRIORITY_RTOS_TO_OSA((configMAX_PRIORITIES - x)))
#define WLAN_TASK_PRI_HIGHEST (OSA_PRIORITY(1))
#define WLAN_TASK_PRI_HIGH    (OSA_PRIORITY(2))
#define WLAN_TASK_PRI_NORMAL  (OSA_PRIORITY(3))
#define WLAN_TASK_PRI_LOW     (OSA_PRIORITY(4))
#define WLAN_TASK_PRI_IDLE    (OSA_PRIORITY(5))

/**
 *  Handle for memory pools.
 *
 *  These are fixed allocation size memory areas.
 */
typedef void * MemoryPool_t;

#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION > 0U)))

#define CONFIG_MEM_POOLS 1

#include <mem_pool_config.h>

#endif

/*** Timer Management ***/
/**
 * OS Timer Activate Options
 */
typedef enum osa_timer_activation
{
    /** Start the timer on creation. */
    OSA_TIMER_AUTO_ACTIVATE,
    /** Do not start the timer on creation. */
    OSA_TIMER_NO_ACTIVATE,
} osa_timer_activate_t;

typedef TimerHandle_t osa_timer_arg_t;
typedef TickType_t osa_timer_tick;

#if CONFIG_HEAP_STAT
/** This function dumps complete statistics
 *  of the heap memory.
 */
void OSA_DumpMemStats(void);
#endif

/**
 * \def os_get_runtime_stats(__buff__)
 *
 * Get ASCII formatted run time statistics
 *
 * Please ensure that your buffer is big enough for the formatted data to
 * fit. Failing to do this may cause memory data corruption.
 */
#define OSA_GetRuntimeStats(__buff__) vTaskGetRunTimeStats(__buff__)

/**
 * \def os_get_task_list(__buff__)
 *
 * Get ASCII formatted task list
 *
 * Please ensure that your buffer is big enough for the formatted data to
 * fit. Failing to do this may cause memory data corruption.
 */

#define OSA_GetTaskList(__buff__) vTaskList(__buff__)

bool is_isr_context(void);
/** Get current OS tick counter value
 *
 * \return 32 bit value of ticks since boot-up
 */
static inline unsigned OSA_TicksGet(void)
{
    if (is_isr_context())
    {
        return xTaskGetTickCountFromISR();
    }
    else
    {
        return xTaskGetTickCount();
    }
}

static inline uint32_t OSA_MsecToTicks(uint32_t msecs)
{
    return (msecs) / (portTICK_PERIOD_MS);
}

static inline unsigned long OSA_TicksToMsec(unsigned long ticks)
{
    return (ticks) * (portTICK_PERIOD_MS);
}

/** Disable all tasks schedule */
static inline void OSA_LockSchedule(void)
{
    vTaskSuspendAll();
}

/** Enable all tasks schedule */
static inline void OSA_UnlockSchedule(void)
{
    xTaskResumeAll();
}

/**
 * Returns time in micro-secs since bootup
 *
 * @note The value returned will wrap around after sometime and caller is
 * expected to guard itself against this.
 *
 * @return Time in micro-secs since bootup
 */
unsigned int OSA_GetTimestamp(void);

/** Structure used for queue definition */
typedef struct osa_queue_pool
{
    /** Size of the queue */
    int size;
} osa_queue_pool_t;

/** Define OS Queue pool
 *
 * This macro helps define the name and size of the queue to be created
 * using the function os_queue_create().
 */
#define OSA_QueuePoolDefine(poolname, poolsize) osa_queue_pool_t poolname = {poolsize};

#endif /* ! _OSA_FREERTOS_H_ */
