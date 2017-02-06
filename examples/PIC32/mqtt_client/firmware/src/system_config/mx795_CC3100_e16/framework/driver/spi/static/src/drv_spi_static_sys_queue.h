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

#ifndef _DRV_SPI_STATIC_SYS_QUEUE_H
#define _DRV_SPI_STATIC_SYS_QUEUE_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "system_config.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

#define _DRV_SPI_QM_SIZE 72
#define _DRV_SPI_Q_SIZE 80
#define _DRV_SPI_QE_SIZE 4

#define DRV_SPI_SYS_QUEUE_BUFFER_SIZE(queues, elementSize, desiredElements) \
    ( _DRV_SPI_QM_SIZE + \
     (_DRV_SPI_Q_SIZE * queues) + \
    ((elementSize + _DRV_SPI_QE_SIZE) * \
     desiredElements) )

// *****************************************************************************
/* SPI Driver Queue Manager Handle

  Summary:
    Handle identifying the queue manager

  Description:
    A queue manager handle value is returned by a call to the DRV_SPI_SYS_QUEUE_Initialize() function.
    This handle is associated with the buffer passed into the function and it allows the
    application to track the queue.
  Remarks:
    None
*/

typedef uintptr_t DRV_SPI_SYS_QUEUE_MANAGER_HANDLE;

// *****************************************************************************
/* SPI Driver Queue Handle

  Summary:
    Handle identifying the queue

  Description:
    A queue handle value is returned by a call to the DRV_SPI_SYS_QUEUE_CreateQueue () function.
    This handle is associated with the buffer passed into the the DRV_SPI_SYS_QUEUE_Initialize() function.
  Remarks:
    None
*/

typedef uintptr_t DRV_SPI_SYS_QUEUE_HANDLE;



typedef enum {
    DRV_SPI_SYS_QUEUE_SUCCESS = 0,
    DRV_SPI_SYS_QUEUE_OUT_OF_MEMORY = -1,
    DRV_SPI_SYS_QUEUE_OUT_OF_QUEUES = -2,
    DRV_SPI_SYS_QUEUE_INVALID_PARAMETER = -3,
    DRV_SPI_SYS_QUEUE_MAX_ERROR = DRV_SPI_SYS_QUEUE_INVALID_PARAMETER,
}DRV_SPI_SYS_QUEUE_RESULT;

typedef enum {

    DRV_SPI_SYS_QUEUE_Unknown = 0,
    DRV_SPI_SYS_QUEUE_Fifo = 1,
}DRV_SPI_SYS_QUEUE_Type;

typedef void ( *DRV_SPI_SYS_QUEUE_INTERUPT_CHANGE )  (DRV_SPI_SYS_QUEUE_HANDLE queue, bool mask);


typedef struct _DRV_SPI_SYS_QUEUE_MANAGER_SETUP
{
    void * pBuffer;
    size_t bufferLen;
    uint8_t numQueues;
    size_t elementSize;
    DRV_SPI_SYS_QUEUE_Type  type;
}DRV_SPI_SYS_QUEUE_MANAGER_SETUP;

typedef struct _DRV_SPI_SYS_QUEUE_SETUP
{
    size_t reserveElements;
    size_t maxElements;
    DRV_SPI_SYS_QUEUE_INTERUPT_CHANGE fptrIntChange;
}DRV_SPI_SYS_QUEUE_SETUP;

typedef struct _DRV_SPI_SYS_QUEUE_MANAGER_STATUS
{
    size_t numAllocOps;
    size_t numFreeOps;
    size_t numQueueCreateOps;
    size_t numQueueDestroyOps;
    size_t numReserveElements;
    size_t numFreeElements;
    size_t freeElementsLW;
    size_t reserveElementsLW;
    size_t outOfMemoryErrors;
    uint8_t numQueues;
    uint8_t numQueuesHW;
} DRV_SPI_SYS_QUEUE_MANAGER_STATUS;

typedef struct _DRV_SPI_SYS_QUEUE_STATUS
{
    size_t numAllocOps;
    size_t numFreeOps;
    size_t numDequeueOps;
    size_t numEnqueueOps;
    size_t numReserved;
    size_t numAlloc;
    size_t numEnqueued;
    size_t numReserveLW;
    size_t numAllocHW;
    size_t numEnqueuedHW;
}DRV_SPI_SYS_QUEUE_STATUS;

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_Initialize(DRV_SPI_SYS_QUEUE_MANAGER_SETUP * initParams, DRV_SPI_SYS_QUEUE_MANAGER_HANDLE * handle);
DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_Deinitialize(DRV_SPI_SYS_QUEUE_MANAGER_HANDLE queueManager);

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_CreateQueue(DRV_SPI_SYS_QUEUE_MANAGER_HANDLE queueManager, DRV_SPI_SYS_QUEUE_SETUP * initParams, DRV_SPI_SYS_QUEUE_HANDLE * handle);
DRV_SPI_SYS_QUEUE_HANDLE DRV_SPI_SYS_QUEUE_CreateQueueLock(DRV_SPI_SYS_QUEUE_MANAGER_HANDLE queueManager, DRV_SPI_SYS_QUEUE_SETUP * initParams, DRV_SPI_SYS_QUEUE_HANDLE * queue);
DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_DestroyQueue(DRV_SPI_SYS_QUEUE_HANDLE queue);
DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_DestroyQueueLock(DRV_SPI_SYS_QUEUE_HANDLE queue);

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_Lock(DRV_SPI_SYS_QUEUE_HANDLE queue);
DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_Unlock(DRV_SPI_SYS_QUEUE_HANDLE queue);

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_AllocElement(DRV_SPI_SYS_QUEUE_HANDLE queue, void ** element);
DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_AllocElementLock(DRV_SPI_SYS_QUEUE_HANDLE queue, void ** element);

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_FreeElement(DRV_SPI_SYS_QUEUE_HANDLE queue, void * element);
DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_FreeElementLock(DRV_SPI_SYS_QUEUE_HANDLE queue, void * element);

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_Enqueue(DRV_SPI_SYS_QUEUE_HANDLE queue, void * element);
DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_EnqueueLock(DRV_SPI_SYS_QUEUE_HANDLE queue, void * element);

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_Dequeue(DRV_SPI_SYS_QUEUE_HANDLE queue, void ** element);
DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_DequeueLock(DRV_SPI_SYS_QUEUE_HANDLE queue, void ** element);
DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_Peek(DRV_SPI_SYS_QUEUE_HANDLE queue, void ** element);

bool DRV_SPI_SYS_QUEUE_IsEmpty(DRV_SPI_SYS_QUEUE_HANDLE queue);

DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_QueueManagerStatus(DRV_SPI_SYS_QUEUE_MANAGER_HANDLE queueManager, DRV_SPI_SYS_QUEUE_MANAGER_STATUS * status);
DRV_SPI_SYS_QUEUE_RESULT DRV_SPI_SYS_QUEUE_QueueStatus(DRV_SPI_SYS_QUEUE_HANDLE queue, DRV_SPI_SYS_QUEUE_STATUS * status);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
// DOM-IGNORE-END

#endif //_DRV_SPI_STATIC_SYS_QUEUE_H_

