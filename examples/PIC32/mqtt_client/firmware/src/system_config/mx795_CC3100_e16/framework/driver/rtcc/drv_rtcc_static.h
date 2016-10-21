/* clang-format off */
/*******************************************************************************
  RTCC Driver Interface Declarations for Static Single Instance Driver

  Company:
    Microchip Technology Inc.

  File Name:
    drv_rtcc_static.h

  Summary:
    RTCC driver interface declarations for the static single instance driver.

  Description:
    The RTCC device driver provides a simple interface to manage the RTCC
    modules on Microchip microcontrollers. This file defines the interface
    Declarations for the OC driver.
    
  Remarks:
    Static interfaces incorporate the driver instance number within the names
    of the routines, eliminating the need for an object ID or object handle.
    Static single-open interfaces also eliminate the need for the open handle.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTOCULAR PURPOSE.
IN NO EVENT SHALL MOCROCHIP OR ITS LOCENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STROCT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVOCES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
//DOM-IGNORE-END

#ifndef _DRV_RTCC_STATIC_H
#define _DRV_RTCC_STATIC_H

// *****************************************************************************
// *****************************************************************************
// Section: Interface Headers for RTCC Instance
// *****************************************************************************
// *****************************************************************************
void DRV_RTCC_Initialize(void);

void DRV_RTCC_Start(void);

void DRV_RTCC_Stop(void);

void DRV_RTCC_ClockOutput(void);

uint32_t DRV_RTCC_TimeGet(void);

uint32_t DRV_RTCC_DateGet(void);

uint32_t DRV_RTCC_AlarmTimeGet(void);

uint32_t DRV_RTCC_AlarmDateGet(void);

#endif // #ifndef _DRV_RTCC_STATIC_H
/*******************************************************************************
 End of File
*/
