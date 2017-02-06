/* clang-format off */
/*******************************************************************************
  SPI Driver Interface Declarations for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_spi_static.h

  Summary:
    SPI driver interface declarations for the static single instance driver.

  Description:
    The SPI device driver provides a simple interface to manage the SPI
    modules on Microchip microcontrollers. This file defines the interface
    Declarations for the SPI static driver.

  Remarks:
    Static interfaces incorporate the driver instance number within the names
    of the routines, eliminating the need for an object ID or object handle.

    Static single-open interfaces also eliminate the need for the open handle.
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

#ifndef _DRV_SPI_STATIC_H
#define _DRV_SPI_STATIC_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "driver/spi/static/src/drv_spi_static_local.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *********************************************************************************************
// *********************************************************************************************
// Section: System Interface Headers for the Instance 0 of SPI static driver
// *********************************************************************************************
// *********************************************************************************************

SYS_MODULE_OBJ DRV_SPI0_Initialize( void );
void DRV_SPI0_Deinitialize ( void );
SYS_STATUS DRV_SPI0_Status ( void );
void DRV_SPI0_Tasks ( void );

// *********************************************************************************************
// *********************************************************************************************
// Section: General Client Interface Headers for the Instance 0 of SPI static driver
// *********************************************************************************************
// *********************************************************************************************

DRV_HANDLE DRV_SPI0_Open ( const SYS_MODULE_INDEX drvIndex, const DRV_IO_INTENT ioIntent );
void DRV_SPI0_Close ( void );
int32_t DRV_SPI0_ClientConfigure ( const DRV_SPI_CLIENT_DATA * cfgData );

// *********************************************************************************************
// *********************************************************************************************
// Section: Read & Write Client Interface Headers for the Instance 0 of SPI static driver
// *********************************************************************************************
// *********************************************************************************************

DRV_SPI_BUFFER_HANDLE DRV_SPI0_BufferAddRead ( void *rxBuffer, size_t size, DRV_SPI_BUFFER_EVENT_HANDLER completeCB, void * context);

DRV_SPI_BUFFER_HANDLE DRV_SPI0_BufferAddWrite ( void *txBuffer, size_t size, DRV_SPI_BUFFER_EVENT_HANDLER completeCB, void * context );

DRV_SPI_BUFFER_HANDLE DRV_SPI0_BufferAddRead2 ( void *rxBuffer, size_t size, DRV_SPI_BUFFER_EVENT_HANDLER completeCB, void * context, DRV_SPI_BUFFER_HANDLE * jobHandle);

DRV_SPI_BUFFER_HANDLE DRV_SPI0_BufferAddWrite2 ( void *txBuffer, size_t size, DRV_SPI_BUFFER_EVENT_HANDLER completeCB, void * context, DRV_SPI_BUFFER_HANDLE * jobHandle );

DRV_SPI_BUFFER_HANDLE DRV_SPI0_BufferAddWriteRead2 ( void *txBuffer, size_t txSize,void *rxBuffer, size_t rxSize, DRV_SPI_BUFFER_EVENT_HANDLER completeCB, void * context,  DRV_SPI_BUFFER_HANDLE * jobHandle);

DRV_SPI_BUFFER_EVENT DRV_SPI0_BufferStatus ( DRV_SPI_BUFFER_HANDLE bufferHandle );

/* This API name was present in the old static driver which is maintained for compatibility.
   So, giving a new name for the new static driver which is taken care of while mapping */
#define  DRV_SPI0_BufferAddWriteReadNew(txbuff, txsize, rxbuff, rxsize, cb, ct ) \
    DRV_SPI0_BufferAddWriteRead2((txbuff), (txsize), (rxbuff), (rxsize), (cb), (ct), NULL)



// *********************************************************************************************
// *********************************************************************************************
// Section: Old static driver compatibility APIs, these will be deprecated.
// *********************************************************************************************
// *********************************************************************************************
bool DRV_SPI0_ReceiverBufferIsFull( void );
bool DRV_SPI0_TransmitterBufferIsFull( void );
int32_t DRV_SPI0_BufferAddWriteRead(const void * txBuffer, void * rxBuffer, uint32_t size);
// *********************************************************************************************



// DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
// DOM-IGNORE-END

#endif // #ifndef _DRV_SPI_STATIC_H
/*******************************************************************************
 End of File
*/
