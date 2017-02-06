/* clang-format off */
/*******************************************************************************
  SPI Driver Local Data Structures for static implementation

  Company:
    Microchip Technology Inc.

  File Name:
    drv_spi_static_local.h

  Summary:
    SPI Driver Local Data Structures for static implementation

  Description:
    Driver Local Data Structures for static implementation
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

#ifndef _DRV_SPI_STATIC_LOCAL_H
#define _DRV_SPI_STATIC_LOCAL_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "system_config.h"
#include "driver/spi/drv_spi.h"
#include "system/int/sys_int.h"
#include "system/debug/sys_debug.h"
#include "driver/spi/static/src/drv_spi_static_sys_queue.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data Type Definitions
// *****************************************************************************
// *****************************************************************************
#define MAX(a,b) ((a<b) ? b : a)
#define MIN(a,b) ((b<a) ? b : a)

#define PLIB_SPI_RX_8BIT_FIFO_SIZE(index)    16
#define PLIB_SPI_RX_8BIT_HW_MARK(index)      8
#define PLIB_SPI_RX_16BIT_FIFO_SIZE(index)   8
#define PLIB_SPI_RX_16BIT_HW_MARK(index)     4
#define PLIB_SPI_RX_32BIT_FIFO_SIZE(index)   4
#define PLIB_SPI_RX_32BIT_HW_MARK(index)     2


#define PLIB_SPI_TX_8BIT_FIFO_SIZE(index)   16
#define PLIB_SPI_TX_8BIT_LW_MARK(index)     8
#define PLIB_SPI_TX_16BIT_FIFO_SIZE(index)  8
#define PLIB_SPI_TX_16BIT_LW_MARK(index)    4
#define PLIB_SPI_TX_32BIT_FIFO_SIZE(index)  4
#define PLIB_SPI_TX_32BIT_LW_MARK(index)    2

struct DRV_SPI_OBJ;

/* SPI Job object. */
typedef struct _DRV_SPI_JOB_OBJECT
{
    uint8_t *txBuffer;
    uint8_t *rxBuffer;
    size_t dataTxed;
    size_t dataRxed;
    size_t dataLeftToTx;
    size_t dummyLeftToTx;
    size_t dataLeftToRx;
    size_t dummyLeftToRx;
    DRV_SPI_BUFFER_EVENT status;
    DRV_SPI_BUFFER_EVENT_HANDLER completeCB;
    void * context;
}DRV_SPI_JOB_OBJECT;

// *****************************************************************************
/* SPI Static Driver Instance Object

  Summary:
    Object used to keep any data required for the static SPI driver.

  Description:
    This object is used to keep track of any data that must be maintained to
    manage the SPI static driver.

  Remarks:
    None.
*/

typedef struct DRV_SPI_OBJ
{
    uint8_t                                     numTrfsSmPolled;
    bool                                        errEnabled;
    DRV_SPI_BUFFER_EVENT_HANDLER                operationStarting;
    DRV_SPI_BUFFER_EVENT_HANDLER                operationEnded;
    DRV_SPI_SYS_QUEUE_HANDLE                    queue;
    DRV_SPI_JOB_OBJECT *                        currentJob;
    bool                                        txEnabled;
    bool                                        rxEnabled;
    uint8_t                                     symbolsInProgress;
} DRV_SPI_OBJ;

// *****************************************************************************
// *****************************************************************************
// Section: Queue related Data Type Definitions
// *****************************************************************************
// *****************************************************************************
typedef struct _DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA
{
    struct _DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA * pNext;
}DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA;

typedef struct _DRV_SPI_SYS_QUEUE_QUEUE_DATA
{
    void * pQueueManager;
    DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA * pHead;
    DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA * pTail;
    DRV_SPI_SYS_QUEUE_INTERUPT_CHANGE fptrIntChange;
    size_t numReserved;
    size_t maxElements;
    size_t numAlloc;
    struct _DRV_SPI_SYS_QUEUE_QUEUE_DATA * pNext;
    size_t numEnqueued;
    size_t numAllocOps;
    size_t numFreeOps;
    size_t numDequeueOps;
    size_t numEnqueueOps;
    size_t numReserveLW;
    size_t numAllocHW;
    size_t numEnqueuedHW;
    size_t outOfMemoryErrors;
}DRV_SPI_SYS_QUEUE_QUEUE_DATA;

typedef struct _DRV_SPI_SYS_QUEUE_QUEUE_MANAGER_DATA
{
    DRV_SPI_SYS_QUEUE_QUEUE_DATA * pQueueArea;
    DRV_SPI_SYS_QUEUE_QUEUE_DATA * pFreeQueueHead;
    DRV_SPI_SYS_QUEUE_QUEUE_DATA * pFreeQueueTail;
    DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA * pElementArea;
    DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA * pFreeElementHead;
    DRV_SPI_SYS_QUEUE_FIFO_ELEMENT_DATA * pFreeElementTail;
    size_t numReserveElements;
    size_t numFreeElements;
    size_t numAllocOps;
    size_t numFreeOps;
    size_t numQueueCreateOps;
    size_t numQueueDestroyOps;
    size_t freeElementsLW;
    size_t reserveElementsLW;
    size_t outOfMemoryErrors;
    uint8_t numQueues;
    uint8_t numQueuesHW;
}DRV_SPI_SYS_QUEUE_QUEUE_MANAGER_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Local functions.
// *****************************************************************************
// *****************************************************************************

int32_t DRV_SPI0_PolledMasterRM8BitTasks ( struct DRV_SPI_OBJ * dObj );
int32_t DRV_SPI0_PolledErrorTasks(struct DRV_SPI_OBJ * dObj);
int32_t DRV_SPI0_MasterRMSend8BitPolled( struct DRV_SPI_OBJ * dObj );
int32_t DRV_SPI0_MasterRMReceive8BitPolled( struct DRV_SPI_OBJ * dObj );
// DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
// DOM-IGNORE-END

#endif //#ifndef _DRV_SPI_STATIC_LOCAL_H

/*******************************************************************************
 End of File
*/

