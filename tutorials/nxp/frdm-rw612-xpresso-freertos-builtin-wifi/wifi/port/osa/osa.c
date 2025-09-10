/** @file osa.c
 *
 *  @brief OS Abstraction API
 *
 *  Copyright 2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <inttypes.h>
#include <stdio.h>
#include <osa.h>
#include <wmlog.h>

/** Check if cpu is in isr context
 *
 * \return bool value - true if cpu is in isr context
 */
bool is_isr_context(void)
{
#ifdef __CA7_REV
    return (0U != if (SystemGetIRQNestingLevel()))
#else /* __CA7_REV */
    return (0U != __get_IPSR());
#endif
}

/*** OS Reader Writer Locks ***/
int OSA_RWLockCreate(osa_rw_lock_t *plock, const char *mutex_name, const char *lock_name)
{
    return OSA_RWLockCreateWithCB(plock, mutex_name, lock_name, NULL);
}

int OSA_RWLockCreateWithCB(osa_rw_lock_t *plock, const char *mutex_name, const char *lock_name, cb_fn r_fn)
{
    osa_status_t status;

    status = OSA_MutexCreate((osa_mutex_handle_t)plock->reader_mutex);
    if (status != KOSA_StatusSuccess)
    {
        return -WM_FAIL;
    }
    status = OSA_MutexCreate((osa_mutex_handle_t)plock->write_mutex);
    if (status != KOSA_StatusSuccess)
    {
        return -WM_FAIL;
    }
    status = OSA_SemaphoreCreateBinary((osa_semaphore_handle_t)plock->rw_lock);
    if (status != KOSA_StatusSuccess)
    {
        return -WM_FAIL;
    }
    OSA_SemaphorePost((osa_semaphore_handle_t)plock->rw_lock);
    plock->reader_count = 0;
    plock->reader_cb    = r_fn;
    return WM_SUCCESS;
}

int OSA_RWLockReadLock(osa_rw_lock_t *lock, unsigned int wait_time)
{
    int ret;
    osa_status_t status = OSA_MutexLock((osa_mutex_handle_t)lock->reader_mutex, osaWaitForever_c);
    if (status != KOSA_StatusSuccess)
    {
        return -WM_FAIL;
    }
    lock->reader_count++;
    if (lock->reader_count == 1U)
    {
        if (lock->reader_cb != NULL)
        {
            ret = lock->reader_cb(lock, wait_time);
            if (ret != WM_SUCCESS)
            {
                lock->reader_count--;
                (void)OSA_MutexUnlock((osa_mutex_handle_t)lock->reader_mutex);
                return ret;
            }
        }
        else
        {
            /* If  1 it is the first reader and
             * if writer is not active, reader will get access
             * else reader will block.
             */
            status = OSA_SemaphoreWait((osa_semaphore_handle_t)lock->rw_lock, wait_time);
            if (status != KOSA_StatusSuccess)
            {
                lock->reader_count--;
                (void)OSA_MutexUnlock((osa_mutex_handle_t)lock->reader_mutex);
                return -WM_FAIL;
            }
        }
    }
    (void)OSA_MutexUnlock((osa_mutex_handle_t)lock->reader_mutex);
    return WM_SUCCESS;
}

int OSA_RWLockReadUnlock(osa_rw_lock_t *lock)
{
    osa_status_t status = OSA_MutexLock((osa_mutex_handle_t)lock->reader_mutex, osaWaitForever_c);
    if (status != KOSA_StatusSuccess)
    {
        return -WM_FAIL;
    }
    lock->reader_count--;
    if (lock->reader_count == 0U)
    {
        /* This is last reader so
         * give a chance to writer now
         */
        (void)OSA_SemaphorePost((osa_semaphore_handle_t)lock->rw_lock);
    }
    (void)OSA_MutexUnlock((osa_mutex_handle_t)lock->reader_mutex);
    return WM_SUCCESS;
}

int OSA_RWLockWriteLock(osa_rw_lock_t *lock, unsigned int wait_time)
{
    osa_status_t status = OSA_SemaphoreWait((osa_semaphore_handle_t)lock->rw_lock, wait_time);
    if (status != KOSA_StatusSuccess)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

void OSA_RWLockWriteUnlock(osa_rw_lock_t *lock)
{
    (void)OSA_SemaphorePost((osa_semaphore_handle_t)lock->rw_lock);
}

void OSA_RWLockDestroy(osa_rw_lock_t *lock)
{
    lock->reader_cb = NULL;

    (void)OSA_SemaphoreDestroy((osa_semaphore_handle_t)lock->rw_lock);

    (void)OSA_MutexDestroy((osa_mutex_handle_t)lock->reader_mutex);

    (void)OSA_MutexDestroy((osa_mutex_handle_t)lock->write_mutex);

    lock->reader_count = 0;
}