/* clang-format off */
/*******************************************************************************
  SPI Driver Queue related local interfaces

  Company:
    Microchip Technology Inc.

  File Name:
    drv_spi_static_sys_queue.h

  Summary:
    Queue related local interface declarations

  Description:
    Queue related local interface declarations
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute Software
only when embedded on a Microchip microcontroller or digital  signal  controller
that is integrated into your product or third party  product  (pursuant  to  the
sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS  WITHOUT  WARRANTY  OF  ANY  KIND,
EITHER EXPRESS  OR  IMPLIED,  INCLUDING  WITHOUT  LIMITATION,  ANY  WARRANTY  OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A  PARTICULAR  PURPOSE.
IN NO EVENT SHALL MICROCHIP OR  ITS  LICENSORS  BE  LIABLE  OR  OBLIGATED  UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,  BREACH  OF  WARRANTY,  OR
OTHER LEGAL  EQUITABLE  THEORY  ANY  DIRECT  OR  INDIRECT  DAMAGES  OR  EXPENSES
INCLUDING BUT NOT LIMITED TO ANY  INCIDENTAL,  SPECIAL,  INDIRECT,  PUNITIVE  OR
CONSEQUENTIAL DAMAGES, LOST  PROFITS  OR  LOST  DATA,  COST  OF  PROCUREMENT  OF
SUBSTITUTE  GOODS,  TECHNOLOGY,  SERVICES,  OR  ANY  CLAIMS  BY  THIRD   PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE  THEREOF),  OR  OTHER  SIMILAR  COSTS.
*******************************************************************************/
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <string.h>
#include "system_config.h"
#include "system_definitions.h"

DRV_SPI_SYS_QUEUE_RESULT _DRV_SPI_SYS_QUEUE_LockQueue(DRV_SPI_SYS_QUEUE_HANDLE queue, bool freeList);
DRV_SPI_SYS_QUEUE_RESULT _DRV_SPI_SYS_QUEUE_UnlockQueue(DRV_SPI_SYS_QUEUE_HANDLE queue, bool freeList);
DRV_SPI_SYS_QUEUE_RESULT _DRV_SPI_SYS_QUEUE_LockQueueManager(DRV_SPI_SYS_QUEUE_MANAGER_HANDLE queueManager);
DRV_SPI_SYS_QUEUE_RESULT _DRV_SPI_SYS_QUEUE_UnlockQueueManager(DRV_SPI_SYS_QUEUE_MANAGER_HANDLE queueManager);


DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_Initialize(DRV_SPI_SYS_QUEUE_MANAGER_SETUP * initParams, DRV_SPI_SYS_QUEUE_MANAGER_HANDLE * handle)
{
    if (initParams == NULL || initParams->pBuffer == NULL || handle == NULL)
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }

    if (initParams->type != DRV_SPI_SYS_QUEUE_Fifo)
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }

    size_t sizeNeeded = (sizeof(DRV_SPI_SYS_QUEUE_QUEUE_DATA) *
                        initParams->numQueues) +
                        sizeof(DRV_SPI_SYS_QUEUE_QUEUE_MANAGER_DATA) +
                        (sizeof(DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA) +
                        initParams->elementSize);

    if (initParams->bufferLen < sizeNeeded)
    {
        return DRV_SPI_SYS_QUEUE_OUT_OF_MEMORY;
    }

    // Blank the memory area
    memset(initParams->pBuffer, 0, initParams->bufferLen);

    // Set up the Queue Manager Area
    DRV_SPI_SYS_QUEUE_QUEUE_MANAGER_DATA * pQueueManager = (DRV_SPI_SYS_QUEUE_QUEUE_MANAGER_DATA *)initParams->pBuffer;
    pQueueManager->pQueueArea  = (DRV_SPI_SYS_QUEUE_QUEUE_DATA*)((uint32_t)pQueueManager + sizeof(DRV_SPI_SYS_QUEUE_QUEUE_MANAGER_DATA));
    pQueueManager->pElementArea  = (DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA*)((uint32_t)pQueueManager->pQueueArea + (sizeof(DRV_SPI_SYS_QUEUE_QUEUE_DATA) * initParams->numQueues) );

    //Set up the Queue Handles
    DRV_SPI_SYS_QUEUE_QUEUE_DATA * pQueue = pQueueManager->pQueueArea;
    pQueueManager->pFreeQueueHead = pQueue;
    pQueueManager->pFreeQueueTail = pQueue;
    pQueue->pQueueManager = pQueueManager;
    uint8_t counter;
    for (counter = 1; counter < initParams->numQueues; counter++)
    {
        pQueueManager->pFreeQueueTail->pNext = &(pQueue[counter]);
        pQueueManager->pFreeQueueTail = &(pQueue[counter]);
        pQueue[counter].pQueueManager = pQueueManager;
    }

    // Set up the elements
    size_t spaceRemaining = initParams->bufferLen -
            sizeof(DRV_SPI_SYS_QUEUE_QUEUE_MANAGER_DATA) -
            (sizeof(DRV_SPI_SYS_QUEUE_QUEUE_DATA) * initParams->numQueues);
    size_t numberOfElements = spaceRemaining /
            (sizeof(DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA) + initParams->elementSize );

    DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA * pElement = pQueueManager->pElementArea;
    pQueueManager->pFreeElementHead = pElement;
    pQueueManager->pFreeElementTail = pElement;

    for (counter = 1; counter < numberOfElements; counter ++)
    {
        pElement = (DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA *)
              ((uint32_t)pQueueManager->pElementArea +
               (sizeof(DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA) +
                initParams->elementSize) *
               counter);
        pQueueManager->pFreeElementTail->pNext = pElement;
        pQueueManager->pFreeElementTail = pElement;
    }
    pQueueManager->numFreeElements = numberOfElements;
#if _SPI_DRV_SYS_QUEUE_TRACKING
    pQueueManager->freeElementsLW = numberOfElements;
#endif
    *handle = (DRV_SPI_SYS_QUEUE_MANAGER_HANDLE)pQueueManager;
    return DRV_SPI_SYS_QUEUE_SUCCESS;
}

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_Deinitialize(DRV_SPI_SYS_QUEUE_MANAGER_HANDLE queueManager)
{
    if ((queueManager <= 0) && (queueManager >= DRV_SPI_SYS_QUEUE_MAX_ERROR))
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }
    return DRV_SPI_SYS_QUEUE_SUCCESS;
}

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_CreateQueue(DRV_SPI_SYS_QUEUE_MANAGER_HANDLE queueManager, DRV_SPI_SYS_QUEUE_SETUP * initParams, DRV_SPI_SYS_QUEUE_HANDLE * handle)
{
    if ((queueManager <= 0) && (queueManager >= DRV_SPI_SYS_QUEUE_MAX_ERROR))
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }
    DRV_SPI_SYS_QUEUE_QUEUE_MANAGER_DATA * pQueueManager = (DRV_SPI_SYS_QUEUE_QUEUE_MANAGER_DATA *)queueManager;
    if (initParams == NULL)
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }
    if (pQueueManager->pFreeQueueHead == NULL)
    {
        return DRV_SPI_SYS_QUEUE_OUT_OF_QUEUES;
    }

    if ((pQueueManager->numReserveElements + initParams->reserveElements) > pQueueManager->numFreeElements)
    {
        return DRV_SPI_SYS_QUEUE_OUT_OF_MEMORY;
    }

    DRV_SPI_SYS_QUEUE_QUEUE_DATA * pQueue = pQueueManager->pFreeQueueHead;
    pQueueManager->pFreeQueueHead = pQueue->pNext;
    if (pQueueManager->pFreeQueueHead == NULL)
    {
        pQueueManager->pFreeQueueTail = NULL;
    }
    memset(pQueue, 0, sizeof(DRV_SPI_SYS_QUEUE_QUEUE_DATA));
    pQueue->pQueueManager = pQueueManager;

    pQueue->fptrIntChange = initParams->fptrIntChange;
    pQueue->numReserved = initParams->reserveElements;
    pQueue->maxElements = initParams->maxElements;

    pQueueManager->numReserveElements += initParams->reserveElements;

#if _SPI_DRV_SYS_QUEUE_TRACKING
    pQueueManager->numQueueCreateOps++;
    pQueueManager->numQueues++;
    pQueueManager->reserveElementsLW += initParams->reserveElements;
    if (pQueueManager->numQueues > pQueueManager->numQueuesHW)
    {
        pQueueManager->numQueuesHW = pQueueManager->numQueues;
    }
#endif

   *handle =  (DRV_SPI_SYS_QUEUE_HANDLE)pQueue;
   return DRV_SPI_SYS_QUEUE_SUCCESS;
}

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_DestroyQueue(DRV_SPI_SYS_QUEUE_HANDLE queue)
{
    if ((queue <= 0) && (queue >= DRV_SPI_SYS_QUEUE_MAX_ERROR))
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }

    DRV_SPI_SYS_QUEUE_QUEUE_DATA * pQueue = (DRV_SPI_SYS_QUEUE_QUEUE_DATA *)queue;
    DRV_SPI_SYS_QUEUE_QUEUE_MANAGER_DATA * pQueueManager = pQueue->pQueueManager;

    if (pQueue->pNext != NULL)
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }


    void * pElement = NULL;
    DRV_SPI_SYS_QUEUE_Dequeue(queue, &pElement);
    while (pElement != NULL)
    {
        DRV_SPI_SYS_QUEUE_FreeElement(queue, pElement);
        DRV_SPI_SYS_QUEUE_Dequeue(queue, &pElement);
    }

    pQueueManager->numReserveElements -= pQueue->numReserved;
    if (pQueueManager->pFreeQueueTail == NULL)
    {
        pQueueManager->pFreeQueueTail = pQueue;
        pQueueManager->pFreeQueueHead = pQueue;
    }
    else
    {
        pQueue->pNext = pQueueManager->pFreeQueueHead;
        pQueueManager->pFreeQueueHead = pQueue;
    }

#if _SPI_DRV_SYS_QUEUE_TRACKING
    pQueueManager->numQueueDestroyOps++;
    pQueueManager->numQueues--;
    pQueueManager->reserveElementsLW -= pQueueManager->numReserveElements;
#endif

    return DRV_SPI_SYS_QUEUE_SUCCESS;
}

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_AllocElement(DRV_SPI_SYS_QUEUE_HANDLE queue, void ** element)
{
    if (element == NULL)
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }
    if ((queue <= 0) && (queue >= DRV_SPI_SYS_QUEUE_MAX_ERROR))
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }

    DRV_SPI_SYS_QUEUE_QUEUE_DATA * pQueue = (DRV_SPI_SYS_QUEUE_QUEUE_DATA *)queue;
    DRV_SPI_SYS_QUEUE_QUEUE_MANAGER_DATA * pQueueManager = pQueue->pQueueManager;

    if (pQueue->pNext != NULL)
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }

    if (pQueueManager->pFreeElementHead== NULL)
    {
#if _SPI_DRV_SYS_QUEUE_TRACKING
        pQueueManager->outOfMemoryErrors++;
        pQueue->outOfMemoryErrors++;
#endif
        return DRV_SPI_SYS_QUEUE_OUT_OF_MEMORY;
    }

    if (pQueue->numAlloc == pQueue->maxElements)
    {
#if _SPI_DRV_SYS_QUEUE_TRACKING
        pQueueManager->outOfMemoryErrors++;
        pQueue->outOfMemoryErrors++;
#endif
        return DRV_SPI_SYS_QUEUE_OUT_OF_MEMORY;
    }

    if (pQueue->numAlloc < pQueue->numReserved)
    {
        pQueueManager->numReserveElements--;
    }
    else if (pQueueManager->numFreeElements == pQueueManager->numReserveElements)
    {
#if _SPI_DRV_SYS_QUEUE_TRACKING
        pQueueManager->outOfMemoryErrors++;
        pQueue->outOfMemoryErrors++;
#endif
        return DRV_SPI_SYS_QUEUE_OUT_OF_MEMORY;
    }

    DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA * pEntry = pQueueManager->pFreeElementHead;
    pQueueManager->pFreeElementHead = pEntry->pNext;
    if (pQueueManager->pFreeElementHead == NULL)
    {
        pQueueManager->pFreeElementTail = NULL;
    }
    pEntry->pNext = NULL;
    pQueueManager->numFreeElements --;
    pQueue->numAlloc++;

#if _SPI_DRV_SYS_QUEUE_TRACKING
    pQueue->numAllocOps ++;
    pQueueManager->numAllocOps ++;
    if (pQueueManager->numFreeElements < pQueueManager->freeElementsLW)
    {
        pQueueManager->freeElementsLW = pQueueManager->numFreeElements;
    }
    if (pQueueManager->numReserveElements < pQueueManager->reserveElementsLW)
    {
        pQueueManager->reserveElementsLW = pQueueManager->numReserveElements;
    }
    if (pQueue->numAlloc > pQueue->numAllocHW)
    {
        pQueue->numAllocHW = pQueue->numAlloc;
    }
#endif
    *element = (void *)((uint32_t)pEntry + sizeof(DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA));
    return DRV_SPI_SYS_QUEUE_SUCCESS;
}

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_FreeElement(DRV_SPI_SYS_QUEUE_HANDLE queue, void * element)
{
    if ((queue <= 0) && (queue >= DRV_SPI_SYS_QUEUE_MAX_ERROR))
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }

    DRV_SPI_SYS_QUEUE_QUEUE_DATA * pQueue = (DRV_SPI_SYS_QUEUE_QUEUE_DATA *)queue;
    DRV_SPI_SYS_QUEUE_QUEUE_MANAGER_DATA * pQueueManager = pQueue->pQueueManager;

    if (pQueue->pNext != NULL)
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }

    DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA * pEntry = (DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA *)((uint32_t)element - sizeof(DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA));

    if (pQueueManager->pFreeElementHead == NULL)
    {
        pQueueManager->pFreeElementHead = pEntry;
    }
    else
    {
        pQueueManager->pFreeElementTail->pNext = pEntry;
    }
    pQueueManager->pFreeElementTail = pEntry;

    pQueueManager->numFreeElements++;
    pQueue->numAlloc--;
    if (pQueue->numAlloc < pQueue->numReserved)
    {
        pQueueManager->numReserveElements++;
    }

#if _SPI_DRV_SYS_QUEUE_TRACKING
    pQueue->numFreeOps ++;
    pQueueManager->numFreeOps ++;
#endif
    return DRV_SPI_SYS_QUEUE_SUCCESS;
}

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_Enqueue(DRV_SPI_SYS_QUEUE_HANDLE queue, void * element)
{
    if ((queue <= 0) && (queue >= DRV_SPI_SYS_QUEUE_MAX_ERROR))
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }

    DRV_SPI_SYS_QUEUE_QUEUE_DATA * pQueue = (DRV_SPI_SYS_QUEUE_QUEUE_DATA *)queue;

    if (pQueue->pNext != NULL)
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }

    DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA * pEntry = (DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA *)((uint32_t)element - sizeof(DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA));

    if (pQueue->pHead == NULL)
    {
        pQueue->pHead = pEntry;
        pQueue->pTail = pEntry;
    }
    else
    {
        pQueue->pTail->pNext = pEntry;
        pQueue->pTail = pEntry;
    }

#if _SPI_DRV_SYS_QUEUE_TRACKING
    pQueue->numEnqueued++;
    if (pQueue->numEnqueued > pQueue->numEnqueuedHW)
    {
        pQueue->numEnqueuedHW = pQueue->numEnqueued;
    }
    pQueue->numEnqueueOps++;
#endif
    return DRV_SPI_SYS_QUEUE_SUCCESS;
}

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_Dequeue(DRV_SPI_SYS_QUEUE_HANDLE queue, void ** element)
{
    if (element == NULL)
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }
    if ((queue <= 0) && (queue >= DRV_SPI_SYS_QUEUE_MAX_ERROR))
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }

    DRV_SPI_SYS_QUEUE_QUEUE_DATA * pQueue = (DRV_SPI_SYS_QUEUE_QUEUE_DATA *)queue;

    if (pQueue->pNext != NULL)
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }
    if (pQueue->pHead == NULL)
    {
        *element = NULL;
        return DRV_SPI_SYS_QUEUE_SUCCESS;
    }
    DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA * pEntry = pQueue->pHead;
    pQueue->pHead = pEntry->pNext;
    if (pQueue->pHead == NULL)
    {
        pQueue->pTail = NULL;
    }

    pEntry->pNext = NULL;

#if _SPI_DRV_SYS_QUEUE_TRACKING
    pQueue->numEnqueued--;
    pQueue->numDequeueOps++;
#endif
    *element = (void *)((uint32_t)pEntry + sizeof(DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA));
    return DRV_SPI_SYS_QUEUE_SUCCESS;
}

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_Peek(DRV_SPI_SYS_QUEUE_HANDLE queue, void ** element)
{
    if (element == NULL)
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }
    if ((queue <= 0) && (queue >= DRV_SPI_SYS_QUEUE_MAX_ERROR))
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }

    DRV_SPI_SYS_QUEUE_QUEUE_DATA * pQueue = (DRV_SPI_SYS_QUEUE_QUEUE_DATA *)queue;

    if (pQueue->pNext != NULL)
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }
    if (pQueue->pHead == NULL)
    {
        *element = NULL;
    }
    DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA * pEntry = pQueue->pHead;
    *element = (void *)((uint32_t)pEntry + sizeof(DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA));
    return DRV_SPI_SYS_QUEUE_SUCCESS;
}

bool DRV_SPI_SYS_QUEUE_IsEmpty(DRV_SPI_SYS_QUEUE_HANDLE queue)
{
    DRV_SPI_SYS_QUEUE_QUEUE_DATA * pQueue = (DRV_SPI_SYS_QUEUE_QUEUE_DATA *)queue;
    return pQueue->pHead == NULL;
}

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_Lock(DRV_SPI_SYS_QUEUE_HANDLE queue)
{

    return _DRV_SPI_SYS_QUEUE_LockQueue(queue, false);
}

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_Unlock(DRV_SPI_SYS_QUEUE_HANDLE queue)
{
    return _DRV_SPI_SYS_QUEUE_UnlockQueue(queue, false);
}

DRV_SPI_SYS_QUEUE_RESULT _DRV_SPI_SYS_QUEUE_LockQueue(DRV_SPI_SYS_QUEUE_HANDLE queue, bool freeList)
{
    if ((queue <= 0) && (queue >= DRV_SPI_SYS_QUEUE_MAX_ERROR))
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }

    DRV_SPI_SYS_QUEUE_QUEUE_DATA * pQueue = (DRV_SPI_SYS_QUEUE_QUEUE_DATA *)queue;
    DRV_SPI_SYS_QUEUE_QUEUE_MANAGER_DATA * pQueueManager = pQueue->pQueueManager;

    if (pQueue->pNext != NULL)
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }

    if (freeList)
    {
        return _DRV_SPI_SYS_QUEUE_LockQueueManager((DRV_SPI_SYS_QUEUE_MANAGER_HANDLE)pQueueManager);
    }
    else
    {
        if (pQueue->fptrIntChange != NULL)
        {
            (*pQueue->fptrIntChange)(queue, true);
        }
    }

    return DRV_SPI_SYS_QUEUE_SUCCESS;
}

DRV_SPI_SYS_QUEUE_RESULT _DRV_SPI_SYS_QUEUE_UnlockQueue(DRV_SPI_SYS_QUEUE_HANDLE queue, bool freeList)
{
    if ((queue <= 0) && (queue >= DRV_SPI_SYS_QUEUE_MAX_ERROR))
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }

    DRV_SPI_SYS_QUEUE_QUEUE_DATA * pQueue = (DRV_SPI_SYS_QUEUE_QUEUE_DATA *)queue;
    DRV_SPI_SYS_QUEUE_QUEUE_MANAGER_DATA * pQueueManager = pQueue->pQueueManager;

    if (pQueue->pNext != NULL)
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }

    if (freeList)
    {
        return _DRV_SPI_SYS_QUEUE_UnlockQueueManager((DRV_SPI_SYS_QUEUE_MANAGER_HANDLE)pQueueManager);
    }
    else
    {
        if (pQueue->fptrIntChange != NULL)
        {
            (*pQueue->fptrIntChange)(queue, false);
        }
    }

    return DRV_SPI_SYS_QUEUE_SUCCESS;
}

DRV_SPI_SYS_QUEUE_HANDLE DRV_SPI_SYS_QUEUE_CreateQueueLock(DRV_SPI_SYS_QUEUE_MANAGER_HANDLE queueManager, DRV_SPI_SYS_QUEUE_SETUP * initParams, DRV_SPI_SYS_QUEUE_HANDLE * queue)
{
    DRV_SPI_SYS_QUEUE_RESULT ret;
    DRV_SPI_SYS_QUEUE_HANDLE  ret2;
    ret = _DRV_SPI_SYS_QUEUE_LockQueueManager(queueManager);
    if (ret != DRV_SPI_SYS_QUEUE_SUCCESS)
    {
        return ret;
    }

    ret2 = DRV_SPI_SYS_QUEUE_CreateQueue(queueManager, initParams, queue);

    ret = _DRV_SPI_SYS_QUEUE_UnlockQueueManager(queueManager);
    if (ret != DRV_SPI_SYS_QUEUE_SUCCESS)
    {
        return ret;
    }
    return ret2;
}

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_DestroyQueueLock(DRV_SPI_SYS_QUEUE_HANDLE queue)
{
    DRV_SPI_SYS_QUEUE_RESULT ret;
    DRV_SPI_SYS_QUEUE_RESULT  ret2;
    ret = _DRV_SPI_SYS_QUEUE_LockQueue(queue, true);// TODO!!! Fix this
    if (ret != DRV_SPI_SYS_QUEUE_SUCCESS)
    {
        return ret;
    }

    ret2 = DRV_SPI_SYS_QUEUE_DestroyQueue(queue);

    ret = _DRV_SPI_SYS_QUEUE_UnlockQueue(queue, true);
    if (ret != DRV_SPI_SYS_QUEUE_SUCCESS)
    {
        return ret;
    }
    return ret2;
}

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_AllocElementLock(DRV_SPI_SYS_QUEUE_HANDLE queue, void  ** element)
{
    DRV_SPI_SYS_QUEUE_RESULT ret;
    DRV_SPI_SYS_QUEUE_RESULT ret2;
    ret = _DRV_SPI_SYS_QUEUE_LockQueue(queue, true);
    if (ret != DRV_SPI_SYS_QUEUE_SUCCESS)
    {
        return ret;
    }

    ret2 = DRV_SPI_SYS_QUEUE_AllocElement(queue, element);

    ret = _DRV_SPI_SYS_QUEUE_UnlockQueue(queue, true);
    if (ret != DRV_SPI_SYS_QUEUE_SUCCESS)
    {
        return ret;
    }
    return ret2;
}


DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_FreeElementLock(DRV_SPI_SYS_QUEUE_HANDLE queue, void * element)
{
    DRV_SPI_SYS_QUEUE_RESULT ret;
    DRV_SPI_SYS_QUEUE_RESULT  ret2;
    ret = _DRV_SPI_SYS_QUEUE_LockQueue(queue, true);
    if (ret != DRV_SPI_SYS_QUEUE_SUCCESS)
    {
        return ret;
    }

    ret2 = DRV_SPI_SYS_QUEUE_FreeElement(queue, element);

    ret = _DRV_SPI_SYS_QUEUE_UnlockQueue(queue, true);
    if (ret != DRV_SPI_SYS_QUEUE_SUCCESS)
    {
        return ret;
    }
    return ret2;
}

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_EnqueueLock(DRV_SPI_SYS_QUEUE_HANDLE queue, void * element)
{
    DRV_SPI_SYS_QUEUE_RESULT ret;
    DRV_SPI_SYS_QUEUE_RESULT  ret2;
    ret = _DRV_SPI_SYS_QUEUE_LockQueue(queue, false);
    if (ret != DRV_SPI_SYS_QUEUE_SUCCESS)
    {
        return ret;
    }

    ret2 = DRV_SPI_SYS_QUEUE_Enqueue(queue, element);

    ret = _DRV_SPI_SYS_QUEUE_UnlockQueue(queue, false);
    if (ret != DRV_SPI_SYS_QUEUE_SUCCESS)
    {
        return ret;
    }
    return ret2;
}

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_DequeueLock(DRV_SPI_SYS_QUEUE_HANDLE queue, void ** element)
{
    DRV_SPI_SYS_QUEUE_RESULT ret;
    DRV_SPI_SYS_QUEUE_RESULT  ret2;
    ret = _DRV_SPI_SYS_QUEUE_LockQueue(queue, false);
    if (ret != DRV_SPI_SYS_QUEUE_SUCCESS)
    {
        return ret;
    }

    ret2 = DRV_SPI_SYS_QUEUE_Dequeue(queue, element);

    ret = _DRV_SPI_SYS_QUEUE_UnlockQueue(queue, false);
    if (ret != DRV_SPI_SYS_QUEUE_SUCCESS)
    {
        return ret;
    }
    return ret2;
}

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_QueueManagerStatus(DRV_SPI_SYS_QUEUE_MANAGER_HANDLE queueManager, DRV_SPI_SYS_QUEUE_MANAGER_STATUS * status)
{
#if _SPI_DRV_SYS_QUEUE_TRACKING
    if ((queueManager <= 0) && (queueManager >= DRV_SPI_SYS_QUEUE_MAX_ERROR))
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }
    DRV_SPI_SYS_QUEUE_QUEUE_MANAGER_DATA * pQueueManager = (DRV_SPI_SYS_QUEUE_QUEUE_MANAGER_DATA *)queueManager;

    status->numAllocOps = pQueueManager->numAllocOps;
    status->numFreeOps = pQueueManager->numFreeOps;
    status->numQueueCreateOps = pQueueManager->numQueueCreateOps;
    status->numQueueDestroyOps = pQueueManager->numQueueDestroyOps;
    status->numReserveElements = pQueueManager->numReserveElements;
    status->numFreeElements = pQueueManager->numFreeElements;
    status->freeElementsLW = pQueueManager->freeElementsLW;
    status->reserveElementsLW = pQueueManager->reserveElementsLW;
    status->outOfMemoryErrors = pQueueManager->outOfMemoryErrors;
    status->numQueues = pQueueManager->numQueues;
    status->numQueuesHW = pQueueManager->numQueuesHW;
    return DRV_SPI_SYS_QUEUE_SUCCESS;
#else
    return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
#endif
}

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_QueueStatus(DRV_SPI_SYS_QUEUE_HANDLE queue, DRV_SPI_SYS_QUEUE_STATUS * status)
{
#if _SPI_DRV_SYS_QUEUE_TRACKING
    if ((queue <= 0) && (queue >= DRV_SPI_SYS_QUEUE_MAX_ERROR))
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }

    DRV_SPI_SYS_QUEUE_QUEUE_DATA * pQueue = (DRV_SPI_SYS_QUEUE_QUEUE_DATA *)queue;

    if (pQueue->pNext != NULL)
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }
    if (status == NULL)
    {
        return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
    }

    status->numAllocOps = pQueue->numAllocOps;
    status->numFreeOps = pQueue->numFreeOps;
    status->numDequeueOps = pQueue->numDequeueOps;
    status->numEnqueueOps = pQueue->numEnqueueOps;
    status->numReserved = pQueue->numReserved;
    status->numAlloc = pQueue->numAlloc;
    status->numEnqueued = pQueue->numEnqueued;
    status->numReserveLW = pQueue->numReserveLW;
    status->numAllocHW = pQueue->numAllocHW;
    status->numEnqueuedHW = pQueue->numEnqueuedHW;
    return DRV_SPI_SYS_QUEUE_SUCCESS;
#else
    return DRV_SPI_SYS_QUEUE_INVALID_PARAMETER;
#endif

}

DRV_SPI_SYS_QUEUE_RESULT _DRV_SPI_SYS_QUEUE_LockQueueManager(DRV_SPI_SYS_QUEUE_MANAGER_HANDLE queueManager)
{
    return DRV_SPI_SYS_QUEUE_SUCCESS;
}
DRV_SPI_SYS_QUEUE_RESULT _DRV_SPI_SYS_QUEUE_UnlockQueueManager(DRV_SPI_SYS_QUEUE_MANAGER_HANDLE queueManager)
{
    return DRV_SPI_SYS_QUEUE_SUCCESS;
}
