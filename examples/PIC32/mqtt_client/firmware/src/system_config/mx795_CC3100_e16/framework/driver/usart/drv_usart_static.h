/* clang-format off */
/*******************************************************************************
  USART Driver Interface Declarations for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usart_static.h

  Summary:
    USART driver interface declarations for the static single instance driver.

  Description:
    The USART device driver provides a simple interface to manage the USART
    modules on Microchip microcontrollers. This file defines the interface
    Declarations for the USART static driver.

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

#ifndef _DRV_USART_STATIC_H
#define _DRV_USART_STATIC_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "driver/usart/src/drv_usart_static_local.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *********************************************************************************************
// *********************************************************************************************
// Section: System Interface Headers for the Instance 0 of USART static driver
// *********************************************************************************************
// *********************************************************************************************

SYS_MODULE_OBJ DRV_USART0_Initialize(void);
void  DRV_USART0_Deinitialize(void);
SYS_STATUS DRV_USART0_Status(void);
void DRV_USART0_TasksTransmit(void);
void DRV_USART0_TasksReceive(void);
void DRV_USART0_TasksError(void);

// *********************************************************************************************
// *********************************************************************************************
// Section: General Client Interface Headers for the Instance 0 of USART static driver
// *********************************************************************************************
// *********************************************************************************************

DRV_HANDLE DRV_USART0_Open(const SYS_MODULE_INDEX index, const DRV_IO_INTENT ioIntent);
void DRV_USART0_Close(void);
DRV_USART_CLIENT_STATUS DRV_USART0_ClientStatus(void);
DRV_USART_TRANSFER_STATUS DRV_USART0_TransferStatus(void);

// *********************************************************************************************
// *********************************************************************************************
// Section: Byte Model Client Interface Headers for the Instance 0 of USART static driver
// *********************************************************************************************
// *********************************************************************************************

uint8_t DRV_USART0_ReadByte( void);
void DRV_USART0_WriteByte( const uint8_t byte);
unsigned int DRV_USART0_ReceiverBufferSizeGet(void);
unsigned int DRV_USART0_TransmitBufferSizeGet(void);
bool DRV_USART0_ReceiverBufferIsEmpty( void );
bool DRV_USART0_TransmitBufferIsFull(void);

// *********************************************************************************************
// *********************************************************************************************
// Section: Set up Client Interface Headers for the Instance 0 of USART static driver
// *********************************************************************************************
// *********************************************************************************************
DRV_USART_BAUD_SET_RESULT DRV_USART0_BaudSet(uint32_t baud);
DRV_USART_LINE_CONTROL_SET_RESULT DRV_USART0_LineControlSet(DRV_USART_LINE_CONTROL lineControlMode);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
// DOM-IGNORE-END

#endif // #ifndef _DRV_USART_STATIC_H
/*******************************************************************************
 End of File
*/
