/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*
 * Copyright (c) 2013-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020,2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

//*****************************************************************************
//
// Include OS functionality.
//
//*****************************************************************************

/* ------------------------ System architecture includes ----------------------------- */
#include "arch/sys_arch.h"

/* ------------------------ lwIP includes --------------------------------- */
#include "lwip/opt.h"

#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"
#include "lwip/stats.h"
#include "lwip/tcpip.h"
#if NO_SYS
#include "lwip/init.h"
#endif

/* ------------------------ SDK includes --------------------------------- */
#include "fsl_common.h"

#ifndef errno
int errno = 0;
#endif

/*
 * Prints an assertion messages and aborts execution.
 */
void sys_assert(const char *pcMessage)
{
// FSL:only needed for debugging
#ifdef LWIP_DEBUG
    LWIP_PLATFORM_DIAG((pcMessage));
#endif
#if !NO_SYS
    (void)sys_arch_protect();
#endif
    for (;;)
    {
    }
}

/************************************************************************
 * Generates a pseudo-random number.
 * NOTE: Contributed by the FNET project.
 *************************************************************************/
static u32_t _rand_value;
u32_t lwip_rand(void)
{
    _rand_value = _rand_value * 1103515245u + 12345u;
    return ((u32_t)(_rand_value >> 16u) % (32767u + 1u));
}

#if !NO_SYS
/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Creates a new mailbox
 * Inputs:
 *      int size                -- Size of elements in the mailbox
 * Outputs:
 *      sys_mbox_t              -- Handle to new mailbox
 *---------------------------------------------------------------------------*/
err_t sys_mbox_new(sys_mbox_t *pxMailBox, int iSize)
{
    err_t xReturn = ERR_MEM;
    *pxMailBox    = xQueueCreate(iSize, sizeof(void *));
    if (*pxMailBox != NULL)
    {
        xReturn = ERR_OK;
        SYS_STATS_INC_USED(mbox);
    }
    return xReturn;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_free
 *---------------------------------------------------------------------------*
 * Description:
 *      Deallocates a mailbox. If there are messages still present in the
 *      mailbox when the mailbox is deallocated, it is an indication of a
 *      programming error in lwIP and the developer should be notified.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 * Outputs:
 *      sys_mbox_t              -- Handle to new mailbox
 *---------------------------------------------------------------------------*/
void sys_mbox_free(sys_mbox_t *pxMailBox)
{
    unsigned long ulMessagesWaiting;

    ulMessagesWaiting = uxQueueMessagesWaiting(*pxMailBox);
    configASSERT((ulMessagesWaiting == 0));

#if SYS_STATS
    {
        if (ulMessagesWaiting != 0UL)
        {
            SYS_STATS_INC(mbox.err);
        }

        SYS_STATS_DEC(mbox.used);
    }
#endif /* SYS_STATS */

    vQueueDelete(*pxMailBox);
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_post
 *---------------------------------------------------------------------------*
 * Description:
 *      Post the "msg" to the mailbox.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void *data              -- Pointer to data to post
 *---------------------------------------------------------------------------*/
void sys_mbox_post(sys_mbox_t *pxMailBox, void *pxMessageToPost)
{
    while (xQueueSendToBack(*pxMailBox, &pxMessageToPost, portMAX_DELAY) != pdTRUE)
        ;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_trypost
 *---------------------------------------------------------------------------*
 * Description:
 *      Try to post the "msg" to the mailbox.  Returns immediately with
 *      error if cannot.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void *msg               -- Pointer to data to post
 * Outputs:
 *      err_t                   -- ERR_OK if message posted, else ERR_MEM
 *                                  if not.
 *---------------------------------------------------------------------------*/
err_t sys_mbox_trypost(sys_mbox_t *pxMailBox, void *pxMessageToPost)
{
    portBASE_TYPE taskToWake = pdFALSE;
#ifdef __CA7_REV
    if (SystemGetIRQNestingLevel())
#else
    if (__get_IPSR())
#endif
    {
        if (pdTRUE == xQueueSendToBackFromISR(*pxMailBox, &pxMessageToPost, &taskToWake))
        {
            if (taskToWake == pdTRUE)
            {
                portYIELD_FROM_ISR(taskToWake);
            }
            return ERR_OK;
        }
        else
        {
            /* The queue was already full. */
            SYS_STATS_INC(mbox.err);
            return ERR_MEM;
        }
    }
    else
    {
        if (pdTRUE == xQueueSendToBack(*pxMailBox, &pxMessageToPost, 0))
        {
            return ERR_OK;
        }
        else
        {
            /* The queue was already full. */
            SYS_STATS_INC(mbox.err);
            return ERR_MEM;
        }
    }
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_mbox_trypost_fromisr
 *---------------------------------------------------------------------------*
 * Description:
 *      Try to post the "msg" to the mailbox.  Returns immediately with
 *      error if cannot. To be be used from ISR.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void *msg               -- Pointer to data to post
 * Outputs:
 *      err_t                   -- ERR_OK if message posted, else ERR_MEM
 *                                  if not.
 *---------------------------------------------------------------------------*/
err_t sys_mbox_trypost_fromisr(sys_mbox_t *mbox, void *msg)
{
    return sys_mbox_trypost(mbox, msg);
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_mbox_fetch
 *---------------------------------------------------------------------------*
 * Description:
 *      Blocks the thread until a message arrives in the mailbox, but does
 *      not block the thread longer than "timeout" milliseconds (similar to
 *      the sys_arch_sem_wait() function). The "msg" argument is a result
 *      parameter that is set by the function (i.e., by doing "*msg =
 *      ptr"). The "msg" parameter maybe NULL to indicate that the message
 *      should be dropped.
 *
 *      The return values are the same as for the sys_arch_sem_wait() function:
 *      Number of milliseconds spent waiting or SYS_ARCH_TIMEOUT if there was a
 *      timeout.
 *
 *      Note that a function with a similar name, sys_mbox_fetch(), is
 *      implemented by lwIP.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void **msg              -- Pointer to pointer to msg received
 *      u32_t timeout           -- Number of milliseconds until timeout
 * Outputs:
 *      u32_t                   -- SYS_ARCH_TIMEOUT if timeout, else number
 *                                  of milliseconds until received.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_mbox_fetch(sys_mbox_t *pxMailBox, void **ppvBuffer, u32_t ulTimeOut)
{
    void *pvDummy;
    TickType_t xStartTime, xEndTime, xElapsed;
    unsigned long ulReturn;

    xStartTime = xTaskGetTickCount();

    if (NULL == ppvBuffer)
    {
        ppvBuffer = &pvDummy;
    }

    if (ulTimeOut != 0UL)
    {
        if (pdTRUE == xQueueReceive(*pxMailBox, &(*ppvBuffer), ulTimeOut / portTICK_PERIOD_MS))
        {
            xEndTime = xTaskGetTickCount();
            xElapsed = (xEndTime - xStartTime) * portTICK_PERIOD_MS;

            ulReturn = xElapsed;
        }
        else
        {
            /* Timed out. */
            *ppvBuffer = NULL;
            ulReturn   = SYS_ARCH_TIMEOUT;
        }
    }
    else
    {
        while (pdTRUE != xQueueReceive(*pxMailBox, &(*ppvBuffer), portMAX_DELAY))
            ;
        xEndTime = xTaskGetTickCount();
        xElapsed = (xEndTime - xStartTime) * portTICK_PERIOD_MS;

        if (xElapsed == 0UL)
        {
            xElapsed = 1UL;
        }

        ulReturn = xElapsed;
    }

    return ulReturn;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_mbox_tryfetch
 *---------------------------------------------------------------------------*
 * Description:
 *      Similar to sys_arch_mbox_fetch, but if message is not ready
 *      immediately, we'll return with SYS_MBOX_EMPTY.  On success, 0 is
 *      returned.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      void **msg              -- Pointer to pointer to msg received
 * Outputs:
 *      u32_t                   -- SYS_MBOX_EMPTY if no messages.  Otherwise,
 *                                  return ERR_OK.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_mbox_tryfetch(sys_mbox_t *pxMailBox, void **ppvBuffer)
{
    void *pvDummy;
    unsigned long ulReturn;

    if (ppvBuffer == NULL)
    {
        ppvBuffer = &pvDummy;
    }

    if (pdTRUE == xQueueReceive(*pxMailBox, &(*ppvBuffer), 0UL))
    {
        ulReturn = ERR_OK;
    }
    else
    {
        ulReturn = SYS_MBOX_EMPTY;
    }

    return ulReturn;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Creates and returns a new semaphore. If the value of "ucCount" argument
 *      is 0, maximum count of semaphore is 1 and the initial state is 0.
 *      Otherwise the "ucCount" argument specifies both initial state and
 *      maximum count of the semaphore.
 * Inputs:
 *      sys_mbox_t mbox         -- Handle of mailbox
 *      u8_t ucCount            -- Initial ucCount of semaphore
 * Outputs:
 *      sys_sem_t               -- Created semaphore or 0 if could not create.
 *---------------------------------------------------------------------------*/
err_t sys_sem_new(sys_sem_t *pxSemaphore, u8_t ucCount)
{
    err_t xReturn = ERR_MEM;

    if (ucCount > 1U)
    {
        *pxSemaphore = xSemaphoreCreateCounting(ucCount, ucCount);
    }
    else
    {
        *pxSemaphore = xSemaphoreCreateBinary();
    }

    if (*pxSemaphore != NULL)
    {
        if (ucCount == 0U)
        {
            xSemaphoreTake(*pxSemaphore, 1UL);
        }

        xReturn = ERR_OK;
        SYS_STATS_INC_USED(sem);
    }
    else
    {
        SYS_STATS_INC(sem.err);
    }

    return xReturn;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_sem_wait
 *---------------------------------------------------------------------------*
 * Description:
 *      Blocks the thread while waiting for the semaphore to be
 *      signaled. If the "timeout" argument is non-zero, the thread should
 *      only be blocked for the specified time (measured in
 *      milliseconds).
 *
 *      If the timeout argument is non-zero, the return value is the number of
 *      milliseconds spent waiting for the semaphore to be signaled. If the
 *      semaphore wasn't signaled within the specified time, the return value is
 *      SYS_ARCH_TIMEOUT. If the thread didn't have to wait for the semaphore
 *      (i.e., it was already signaled), the function may return zero.
 *
 *      Notice that lwIP implements a function with a similar name,
 *      sys_sem_wait(), that uses the sys_arch_sem_wait() function.
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to wait on
 *      u32_t timeout           -- Number of milliseconds until timeout
 * Outputs:
 *      u32_t                   -- Time elapsed or SYS_ARCH_TIMEOUT.
 *---------------------------------------------------------------------------*/
u32_t sys_arch_sem_wait(sys_sem_t *pxSemaphore, u32_t ulTimeout)
{
    TickType_t xStartTime, xEndTime, xElapsed;
    unsigned long ulReturn;

    xStartTime = xTaskGetTickCount();

    if (ulTimeout != 0UL)
    {
        if (xSemaphoreTake(*pxSemaphore, ulTimeout / portTICK_PERIOD_MS) == pdTRUE)
        {
            xEndTime = xTaskGetTickCount();
            xElapsed = (xEndTime - xStartTime) * portTICK_PERIOD_MS;
            ulReturn = xElapsed;
        }
        else
        {
            ulReturn = SYS_ARCH_TIMEOUT;
        }
    }
    else
    {
        while (xSemaphoreTake(*pxSemaphore, portMAX_DELAY) != pdTRUE)
            ;
        xEndTime = xTaskGetTickCount();
        xElapsed = (xEndTime - xStartTime) * portTICK_PERIOD_MS;

        if (xElapsed == 0UL)
        {
            xElapsed = 1UL;
        }

        ulReturn = xElapsed;
    }

    return ulReturn;
}

/** Create a new mutex
 * @param mutex pointer to the mutex to create
 * @return a new mutex */
err_t sys_mutex_new(sys_mutex_t *pxMutex)
{
    err_t xReturn = ERR_MEM;

    *pxMutex = xSemaphoreCreateMutex();

    if (*pxMutex != NULL)
    {
        xReturn = ERR_OK;
        SYS_STATS_INC_USED(mutex);
    }
    else
    {
        SYS_STATS_INC(mutex.err);
    }

    return xReturn;
}

/** Lock a mutex
 * @param mutex the mutex to lock */
void sys_mutex_lock(sys_mutex_t *pxMutex)
{
    while (xSemaphoreTake(*pxMutex, portMAX_DELAY) != pdPASS)
        ;
}

/** Unlock a mutex
 * @param mutex the mutex to unlock */
void sys_mutex_unlock(sys_mutex_t *pxMutex)
{
    xSemaphoreGive(*pxMutex);
}

/** Delete a semaphore
 * @param mutex the mutex to delete */
void sys_mutex_free(sys_mutex_t *pxMutex)
{
    SYS_STATS_DEC(mutex.used);
    vQueueDelete(*pxMutex);
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_signal
 *---------------------------------------------------------------------------*
 * Description:
 *      Signals (releases) a semaphore
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to signal
 *---------------------------------------------------------------------------*/
void sys_sem_signal(sys_sem_t *pxSemaphore)
{
    xSemaphoreGive(*pxSemaphore);
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_sem_free
 *---------------------------------------------------------------------------*
 * Description:
 *      Deallocates a semaphore
 * Inputs:
 *      sys_sem_t sem           -- Semaphore to free
 *---------------------------------------------------------------------------*/
void sys_sem_free(sys_sem_t *pxSemaphore)
{
    SYS_STATS_DEC(sem.used);
    vQueueDelete(*pxSemaphore);
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_init
 *---------------------------------------------------------------------------*
 * Description:
 *      Initialize sys arch
 *---------------------------------------------------------------------------*/
void sys_init(void)
{
}

u32_t sys_now(void)
{
    return xTaskGetTickCount() * portTICK_PERIOD_MS;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_thread_new
 *---------------------------------------------------------------------------*
 * Description:
 *      Starts a new thread with priority "iPriority" that will begin its
 *      execution in the function "pxThread()". The "pvArg" argument will be
 *      passed as an argument to the pxThread() function. The id of the new
 *      thread is returned. Both the id and the priority are system
 *      dependent.
 * Inputs:
 *      char *pcName            -- Name of thread
 *      void (* pxThread)(void *pvParameters)  -- Pointer to function to run.
 *      void *pvArg             -- Argument passed into function
 *      int iStackSize          -- Required stack amount in words (not bytes!)
 *      int iPriority           -- Thread priority
 * Outputs:
 *      sys_thread_t            -- Pointer to the new thread's structure.
 *---------------------------------------------------------------------------*/
sys_thread_t sys_thread_new(
    const char *pcName, void (*pxThread)(void *pvParameters), void *pvArg, int iStackSize, int iPriority)
{
    TaskHandle_t xCreatedTask;
    portBASE_TYPE xResult;
    sys_thread_t xReturn;

    LWIP_ASSERT("invalid stacksize", iStackSize > 0);

    xResult = xTaskCreate(pxThread, pcName, (configSTACK_DEPTH_TYPE)iStackSize, pvArg, iPriority, &xCreatedTask);
    LWIP_ASSERT("task creation failed", xResult == pdPASS);

    if (xResult == pdPASS)
    {
        xReturn = xCreatedTask;
    }
    else
    {
        xReturn = NULL;
    }

    return xReturn;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_protect
 *---------------------------------------------------------------------------*
 * Description:
 *      This optional function does a "fast" critical region protection and
 *      returns the previous protection level. This function is only called
 *      during very short critical regions. An embedded system which supports
 *      ISR-based drivers might want to implement this function by disabling
 *      interrupts. Task-based systems might want to implement this by using
 *      a mutex or disabling tasking. This function should support recursive
 *      calls from the same task or interrupt. In other words,
 *      sys_arch_protect() could be called while already protected. In
 *      that case the return value indicates that it is already protected.
 *
 *      sys_arch_protect() is only required if your port is supporting an
 *      operating system.
 * Outputs:
 *      sys_prot_t              -- Previous protection level (not used here)
 *---------------------------------------------------------------------------*/
sys_prot_t sys_arch_protect(void)
{
    sys_prot_t result = 0;

#ifdef __CA7_REV
    if (SystemGetIRQNestingLevel())
#else
    if (__get_IPSR())
#endif
    {
        result = portSET_INTERRUPT_MASK_FROM_ISR();
    }
    else
    {
        portENTER_CRITICAL();
    }
    return result;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_unprotect
 *---------------------------------------------------------------------------*
 * Description:
 *      This optional function does a "fast" set of critical region
 *      protection to the value specified by pval. See the documentation for
 *      sys_arch_protect() for more information. This function is only
 *      required if your port is supporting an operating system.
 * Inputs:
 *      sys_prot_t              -- Previous protection level (not used here)
 *---------------------------------------------------------------------------*/
void sys_arch_unprotect(sys_prot_t xValue)
{
#ifdef __CA7_REV
    if (SystemGetIRQNestingLevel())
#else
    if (__get_IPSR())
#endif
    {
        portCLEAR_INTERRUPT_MASK_FROM_ISR(xValue);
    }
    else
    {
        portEXIT_CRITICAL();
    }
}

#if LWIP_TCPIP_CORE_LOCKING

static u8_t lwip_core_lock_count;
static TaskHandle_t lwip_core_lock_holder_thread;

void sys_lock_tcpip_core(void)
{
    sys_mutex_lock(&lock_tcpip_core);
    if (lwip_core_lock_count == 0U)
    {
        lwip_core_lock_holder_thread = xTaskGetCurrentTaskHandle();
    }
    lwip_core_lock_count++;
}

void sys_unlock_tcpip_core(void)
{
    lwip_core_lock_count--;
    if (lwip_core_lock_count == 0)
    {
        lwip_core_lock_holder_thread = 0;
    }
    sys_mutex_unlock(&lock_tcpip_core);
}

#endif /* LWIP_TCPIP_CORE_LOCKING */

static TaskHandle_t lwip_tcpip_thread;

void sys_mark_tcpip_thread(void)
{
    lwip_tcpip_thread = xTaskGetCurrentTaskHandle();
}

void sys_check_core_locking(void)
{
    LWIP_ASSERT("Function called from interrupt context",
#ifdef __CA7_REV
                (SystemGetIRQNestingLevel() == 0)
#else
                (__get_IPSR() == 0)
#endif
    );

    if (lwip_tcpip_thread != 0)
    {
        TaskHandle_t current_thread = xTaskGetCurrentTaskHandle();
        LWIP_UNUSED_ARG(current_thread); /* for LWIP_NOASSERT */

#if LWIP_TCPIP_CORE_LOCKING
        LWIP_UNUSED_ARG(lwip_core_lock_holder_thread); /* for LWIP_NOASSERT */
        LWIP_ASSERT("Function called without core lock",
                    current_thread == lwip_core_lock_holder_thread && lwip_core_lock_count > 0);
#else  /* LWIP_TCPIP_CORE_LOCKING */
        LWIP_ASSERT("Function called from wrong thread", current_thread == lwip_tcpip_thread);
#endif /* LWIP_TCPIP_CORE_LOCKING */
    }
}

#else /* Bare-metal */

static volatile uint32_t time_now = 0;

void time_isr(void)
{
#ifdef __CA7_REV
    SystemClearSystickFlag();
#endif
    time_now++;
}

void time_init(void)
{
#ifdef __CA7_REV
    /* special for i.mx6ul */
    SystemSetupSystick(1000U, (void *)time_isr, 0U);
    SystemClearSystickFlag();
#else
    /* Set SysTick period to 1 ms and enable its interrupts */
    extern uint32_t SystemCoreClock;
    SysTick_Config(USEC_TO_COUNT(1000U, SystemCoreClock));
#endif
}

/*
This optional function returns the current time in milliseconds (don't care
  for wraparound, this is only used for time diffs).
  Not implementing this function means you cannot use some modules (e.g. TCP
  timestamps, internal timeouts for NO_SYS==1).
  */

u32_t sys_now(void)
{
    return (u32_t)time_now;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_protect
 *---------------------------------------------------------------------------*
 * Description:
 *      This optional function does a "fast" critical region protection and
 *      returns the previous protection level. This function is only called
 *      during very short critical regions. An embedded system which supports
 *      ISR-based drivers might want to implement this function by disabling
 *      interrupts. Task-based systems might want to implement this by using
 *      a mutex or disabling tasking. This function should support recursive
 *      calls from the same task or interrupt. In other words,
 *      sys_arch_protect() could be called while already protected. In
 *      that case the return value indicates that it is already protected.
 *
 *      sys_arch_protect() is only required if your port is supporting an
 *      operating system.
 * Outputs:
 *      sys_prot_t              -- Previous protection level (not used here)
 *---------------------------------------------------------------------------*/
sys_prot_t sys_arch_protect(void)
{
    sys_prot_t result;

    result = (sys_prot_t)DisableGlobalIRQ();

    return result;
}

/*---------------------------------------------------------------------------*
 * Routine:  sys_arch_unprotect
 *---------------------------------------------------------------------------*
 * Description:
 *      This optional function does a "fast" set of critical region
 *      protection to the value specified by pval. See the documentation for
 *      sys_arch_protect() for more information. This function is only
 *      required if your port is supporting an operating system.
 * Inputs:
 *      sys_prot_t              -- Previous protection level (not used here)
 *---------------------------------------------------------------------------*/
void sys_arch_unprotect(sys_prot_t xValue)
{
    EnableGlobalIRQ((uint32_t)xValue);
}

void sys_check_core_locking(void)
{
    LWIP_ASSERT("Function called from interrupt context",
#ifdef __CA7_REV
                (SystemGetIRQNestingLevel() == 0)
#else
                (__get_IPSR() == 0)
#endif
    );
}

#endif /* !NO_SYS */

/*-------------------------------------------------------------------------*
 * End of File:  sys_arch.c
 *-------------------------------------------------------------------------*/
