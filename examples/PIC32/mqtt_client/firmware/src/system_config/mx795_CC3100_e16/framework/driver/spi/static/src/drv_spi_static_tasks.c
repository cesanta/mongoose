/* clang-format off */
/*******************************************************************************
  SPI Driver Functions for Static Driver Tasks Functions

  Company:
    Microchip Technology Inc.

  File Name:
    drv_spi_static_tasks.c

  Summary:
    SPI driver tasks functions

  Description:
    The SPI device driver provides a simple interface to manage the SPI
    modules on Microchip microcontrollers. This file contains implemenation
    for the SPI driver.

  Remarks:
  This file is generated from framework/driver/spi/template/drv_spi_static_tasks.c.ftl
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
//DOM-IGNORE-END
#include "system_config.h"
#include "system_definitions.h"



int32_t DRV_SPI0_PolledMasterRM8BitTasks ( struct DRV_SPI_OBJ * dObj )
{
    volatile bool continueLoop;
    uint8_t counter = 0;
    uint8_t numPolled = dObj->numTrfsSmPolled;
    uint8_t result = 0;
    do {
        
        DRV_SPI_JOB_OBJECT * currentJob = dObj->currentJob;

        /* Check for a new task */
        if (dObj->currentJob == NULL)
        {
            if (DRV_SPI_SYS_QUEUE_DequeueLock(dObj->queue, (void *)&(dObj->currentJob)) != DRV_SPI_SYS_QUEUE_SUCCESS)
            {
                SYS_ASSERT(false, "\r\nSPI Driver: Error in dequeing.");
                return 0;
            }
            if (dObj->currentJob == NULL)
            {
                return 0;
            }
            currentJob = dObj->currentJob;

            dObj->symbolsInProgress = 0;

            /* Call the operation starting function pointer.  This can be used to modify the slave select lines */
            if (dObj->operationStarting != NULL)
            {
                (*dObj->operationStarting)(DRV_SPI_BUFFER_EVENT_PROCESSING, (DRV_SPI_BUFFER_HANDLE)currentJob, currentJob->context);
            }

            /* List the new job as processing*/
            currentJob->status = DRV_SPI_BUFFER_EVENT_PROCESSING;
            /* Flush out the Receive buffer */
            PLIB_SPI_BufferClear(SPI_ID_1);
        }


        continueLoop = false;
        
        /* Execute the sub tasks */
            if
            (currentJob->dataLeftToTx +currentJob->dummyLeftToTx != 0)
        {
            DRV_SPI0_MasterRMSend8BitPolled(dObj);
        }
        
        DRV_SPI0_PolledErrorTasks(dObj);
        
        /* Figure out how many bytes are left to be received */
        volatile size_t bytesLeft = currentJob->dataLeftToRx + currentJob->dummyLeftToRx;
        
        // Check to see if we have any data left to receive and update the bytes left.
        if (bytesLeft != 0)
        {
            DRV_SPI0_MasterRMReceive8BitPolled(dObj);
            bytesLeft = currentJob->dataLeftToRx + currentJob->dummyLeftToRx;
        }
        if (bytesLeft == 0)
        {
                    /* Job is complete*/
                    currentJob->status = DRV_SPI_BUFFER_EVENT_COMPLETE;
                    /* Call the job complete call back*/
                    if (currentJob->completeCB != NULL)
                    {
                        (*currentJob->completeCB)(DRV_SPI_BUFFER_EVENT_COMPLETE, (DRV_SPI_BUFFER_HANDLE)currentJob, currentJob->context);
                    }

                    /* Call the operation complete call back.  This is different than the
                       job complete callback.  This can be used to modify the Slave Select line.*/

                    if (dObj->operationEnded != NULL)
                    {
                        (*dObj->operationEnded)(DRV_SPI_BUFFER_EVENT_COMPLETE, (DRV_SPI_BUFFER_HANDLE)currentJob, currentJob->context);
                    }

                    /* Return the job back to the free queue*/
                    if (DRV_SPI_SYS_QUEUE_FreeElementLock(dObj->queue, currentJob) != DRV_SPI_SYS_QUEUE_SUCCESS)
                    {
                        SYS_ASSERT(false, "\r\nSPI Driver: Queue free element error.");
                        return 0;
                    }
                    /* Clean up */
                    dObj->currentJob = NULL;
                }


        counter ++;
        if ((counter < numPolled) && (result != 0))
        {
            continueLoop = true;
        }

    } while(continueLoop);
    return 0;
}


