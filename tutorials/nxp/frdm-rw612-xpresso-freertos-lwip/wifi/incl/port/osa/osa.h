/*
 *  Copyright 2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */
/*!\file osa.h
 *\brief This file contains OSA wrapper declarations for timer, read/write lock and idle hook.
*/


#ifndef _OSA_H_
#define _OSA_H_

#include <wifi_config_default.h>

#include "fsl_os_abstraction.h"

#if defined(SDK_OS_FREE_RTOS)
#include <osa_freertos.h>
#elif defined(FSL_RTOS_THREADX)
#include "app_config.h"
#include <osa_threadx.h>
#elif (CONFIG_ZEPHYR)
#include <osa_zephyr.h>
#else
#error "Please define OS type"
#endif

#include <wmerrno.h>
#include <wm_utils.h>

/*** Timer Management ***/
/** Create timer
 *
 * This function creates a timer.
 *
 * @param[in] timerHandle Pointer to the timer handle
 * @param[in] ticks Period in ticks
 * @param[in] call_back Timer expire callback function
 * @param[in] cb_arg Timer callback data
 * @param[in] reload Reload Options, valid values include \ref KOSA_TimerOnce
 * or \ref KOSA_TimerPeriodic.
 * @param[in] activate Activate Options, valid values include \ref
 * OSA_TIMER_AUTO_ACTIVATE or \ref OSA_TIMER_NO_ACTIVATE
 *
 * @return KOSA_StatusSuccess if timer created successfully
 * @return KOSA_StatusError if timer creation fails
 */
osa_status_t OSA_TimerCreate(osa_timer_handle_t timerHandle,
                             osa_timer_tick ticks,
                             void (*call_back)(osa_timer_arg_t),
                             void *cb_arg,
                             osa_timer_t reload,
                             osa_timer_activate_t activate);

/** Activate timer
 *
 * This function activates (or starts) a timer that was previously created using
 * OSA_TimerCreate(). If the timer had already started and was already in the
 * active state, then this call is equivalent to OSA_TimerReset().
 *
 * @param[in] timerHandle Pointer to a timer handle
 *
 * @return KOSA_StatusSuccess if timer activated successfully
 * @return KOSA_StatusError if timer activation fails
 *
 */
osa_status_t OSA_TimerActivate(osa_timer_handle_t timerHandle);

/** Change timer period
 *
 * This function changes the period of a timer that was previously created using
 * OSA_TimerCreate(). This function changes the period of an active or dormant
 * state timer.
 *
 * @param[in] timerHandle Pointer to a timer handle
 * @param[in] ntime Time in ticks after which the timer will expire
 * @param[in] block_time  This option is currently not supported
 *
 * @return KOSA_StatusSuccess if timer change successfully
 * @return KOSA_StatusError if timer change fails
 */
osa_status_t OSA_TimerChange(osa_timer_handle_t timerHandle, osa_timer_tick ntime, osa_timer_tick block_time);

/** Check the timer active state
 *
 * This function checks if the timer is in the active or dormant state. A timer
 * is in the dormant state if (a) it has been created but not started, or (b) it
 * has expired and a one-shot timer.
 *
 * @param [in] timerHandle Pointer to a timer handle
 *
 * @return true if timer is active
 * @return false if time is not active
 */
bool OSA_TimerIsRunning(osa_timer_handle_t timerHandle);

/**
 * Get the timer context
 *
 * This function helps to retrieve the timer context i.e. 'cb_arg' passed
 * to OSA_TimerCreate().
 *
 * @param[in] timer_t Pointer to timer handle. The timer handle is received
 * in the timer callback.
 *
 * @return The timer context i.e. the callback argument passed to
 * OSA_TimerCreate().
 */
void *OSA_TimerGetContext(osa_timer_handle_t timerHandle);

/** Reset timer
 *
 * This function resets a timer that was previously created using using
 * OSA_TimerCreate(). If the timer had already been started and was already in
 * the active state, then this call will cause the timer to re-evaluate its
 * expiry time so that it is relative to when OSA_TimerReset() was called. If
 * the timer was in the dormant state then this call behaves in the same way as
 * OSA_TimerActivate().
 *
 * @param[in] timerHandle Pointer to a timer handle
 *
 * @return KOSA_StatusSuccess if timer reset successfully
 * @return KOSA_StatusError if timer reset fails
 */
osa_status_t OSA_TimerReset(osa_timer_handle_t timerHandle);

/** Deactivate timer
 *
 * This function deactivates (or stops) a timer that was previously started.
 *
 * @param [in] timerHandle handle populated by OSA_TimerCreate().
 *
 * @return KOSA_StatusSuccess if timer deactivate successfully
 * @return KOSA_StatusError if timer deactivate fails
 */
osa_status_t OSA_TimerDeactivate(osa_timer_handle_t timerHandle);

/** Destroy timer
 *
 * This function deletes a timer.
 *
 * @param[in] timerHandle Pointer to a timer handle
 *
 * @return KOSA_StatusSuccess if timer destroy successfully
 * @return KOSA_StatusError if timer destroy fails
 */
osa_status_t OSA_TimerDestroy(osa_timer_handle_t timerHandle);

/*
 * Reader Writer Locks
 * This is a generic implementation of reader writer locks
 * which is reader priority.
 * Not only it provides mutual exclusion but also synchronization.
 * Six APIs are exposed to user which include.
 * -# Create a reader writer lock
 * -# Delete a reader writer lock
 * -# Reader lock
 * -# Reader unlock
 * -# Writer lock
 * -# Writer unlock
 * The locking operation is timeout based.
 * Caller can give a timeout from 0 (no wait) to
 * infinite (wait forever)
 */

typedef struct _rw_lock osa_rw_lock_t;
/** This is prototype of reader callback */
typedef int (*cb_fn)(osa_rw_lock_t *plock, unsigned int wait_time);

struct _rw_lock
{
    /** Mutex for reader mutual exclusion */
    OSA_MUTEX_HANDLE_DEFINE(reader_mutex);
    /** Mutex for write mutual exclusion */
    OSA_MUTEX_HANDLE_DEFINE(write_mutex);
    /** Lock which when held by reader,
     *  writer cannot enter critical section
     */
    OSA_SEMAPHORE_HANDLE_DEFINE(rw_lock);
    /** Function being called when first reader gets
     *  the lock
     */
    cb_fn reader_cb;
    /** Counter to maintain number of readers
     *  in critical section
     */
    unsigned int reader_count;
};

int OSA_RWLockCreateWithCB(osa_rw_lock_t *plock, const char *mutex_name, const char *lock_name, cb_fn r_fn);

/** Create reader-writer lock
 *
 * This function creates a reader-writer lock.
 *
 * @param[in] lock Pointer to a reader-writer lock handle
 * @param[in] mutex_name Name of the mutex
 * @param[in] lock_name Name of the lock
 *
 * @return WM_SUCCESS on success
 * @return -WM_FAIL on error
 */
int OSA_RWLockCreate(osa_rw_lock_t *plock, const char *mutex_name, const char *lock_name);

/** Delete a reader-write lock
 *
 * This function deletes a reader-writer lock.
 *
 * @param[in] lock Pointer to the reader-writer lock handle
 *
 */
void OSA_RWLockDestroy(osa_rw_lock_t *lock);

/** Acquire writer lock
 *
 * This function acquires a writer lock. While readers can acquire the lock on a
 * sharing basis, writers acquire the lock in an exclusive manner.
 *
 * @param[in] lock Pointer to the reader-writer lock handle
 * @param[in] wait_time The maximum amount of time, in OS ticks, the task should
 * block waiting for the lock to be acquired. The special values \ref
 * osaWaitForever_c and \ref osaWaitNone_c are provided to respectively wait
 * infinitely or return immediately.
 *
 * @return  WM_SUCCESS on success
 * @return  -WM_FAIL on error
 *
 */
int OSA_RWLockWriteLock(osa_rw_lock_t *lock, unsigned int wait_time);

/** Release writer lock
 *
 * This function releases a writer lock previously acquired using
 * OSA_RWLockWriteLock().
 *
 * @param[in] lock Pointer to the reader-writer lock handle
 */
void OSA_RWLockWriteUnlock(osa_rw_lock_t *lock);

/** Acquire reader lock
 *
 * This function acquires a reader lock. While readers can acquire the lock on a
 * sharing basis, writers acquire the lock in an exclusive manner.
 *
 * @param[in] lock pointer to the reader-writer lock handle
 * @param[in] wait_time The maximum amount of time, in OS ticks, the task should
 * block waiting for the lock to be acquired. The special values \ref
 * osaWaitForever_c and \ref osaWaitNone_c are provided to respectively wait
 * infinitely or return immediately.
 *
 * @return  WM_SUCCESS on success
 * @return  -WM_FAIL on error
 *
 */
int OSA_RWLockReadLock(osa_rw_lock_t *lock, unsigned int wait_time);

/** Release reader lock
 *
 * This function releases a reader lock previously acquired using
 * OSA_RWLockReadLock().
 *
 * @param[in] lock pointer to the reader-writer lock handle
 *
 * @return WM_SUCCESS if unlock operation successful.
 * @return -WM_FAIL if unlock operation failed.
 */
int OSA_RWLockReadUnlock(osa_rw_lock_t *lock);

/*** Tick function */
#define MAX_CUSTOM_HOOKS 4U

extern void (*g_osa_tick_hooks[MAX_CUSTOM_HOOKS])(void);
extern void (*g_osa_idle_hooks[MAX_CUSTOM_HOOKS])(void);

/** Setup idle function
 *
 * This function sets up a callback function which will be called whenever the
 * system enters the idle thread context.
 *
 *  @param[in] func The callback function
 *
 *  @return WM_SUCCESS on success
 *  @return -WM_FAIL on error
 */
int OSA_SetupIdleFunction(void (*func)(void));

/** Setup tick function
 *
 * This function sets up a callback function which will be called on every
 * SysTick interrupt.
 *
 *  @param[in] func The callback function
 *
 *  @return WM_SUCCESS on success
 *  @return -WM_FAIL on error
 */
int OSA_SetupTickFunction(void (*func)(void));

/** Remove idle function
 *
 *  This function removes an idle callback function that was registered
 *  previously using OSA_SetupIdleFunction().
 *
 *  @param[in] func The callback function
 *
 *  @return WM_SUCCESS on success
 *  @return -WM_FAIL on error
 */
int OSA_RemoveIdleFunction(void (*func)(void));

/** Remove tick function
 *
 *  This function removes a tick callback function that was registered
 *  previously using OSA_SetupTickFunction().
 *
 *  @param[in] func Callback function
 *  @return WM_SUCCESS on success
 *  @return -WM_FAIL on error
 */
int OSA_RemoveTickFunction(void (*func)(void));

/* Init value for rand generator seed */
extern uint32_t wm_rand_seed;

/** This function initialize the seed for rand generator
 *  @return a uint32_t random numer
 */
static inline void OSA_Srand(uint32_t seed)
{
    wm_rand_seed = seed;
}

/** This function generate a random number
 *  @return a uint32_t random numer
 */
static inline uint32_t OSA_Rand()
{
    if (wm_rand_seed == 0xFFFFFFFFU)
        OSA_Srand(OSA_TimeGetMsec());
    wm_rand_seed = (uint32_t)((((uint64_t)wm_rand_seed * 279470273UL) % 4294967291UL) & 0xFFFFFFFFUL);
    return wm_rand_seed;
}

/** This function generate a random number in a range
 *  @param [in] low  range low
 *  @param [in] high range high
 *  @return a uint32_t random numer
 */
static inline uint32_t OSA_RandRange(uint32_t low, uint32_t high)
{
    uint32_t tmp;
    if (low == high)
        return low;
    if (low > high)
    {
        tmp  = low;
        low  = high;
        high = tmp;
    }
    return (low + OSA_Rand() % (high - low));
}

void OSA_DumpThreadInfo(char *name);

/** Suspend the given thread
 *
 * - The function OSA_ThreadSelfComplete() will \b permanently suspend the
 * given thread. Passing NULL will suspend the current thread. This
 * function never returns.
 * - The thread continues to consume system resources. To delete the thread
 * the function OSA_TaskDestroy() needs to be called separately.
 *
 * @param[in] taskHandle Pointer to thread handle
 */
void OSA_ThreadSelfComplete(osa_task_handle_t taskHandle);

/** Return the number of messages stored in queue.
 *
 * @param[in] msgqHandle Pointer to handle of the queue to be queried.
 *
 * @returns Number of items in the queue
 */
uint32_t OSA_MsgQWaiting(osa_msgq_handle_t msgqHandle);

#endif /* ! _OSA_H_ */
