/* clang-format off */
/*******************************************************************************
  SYS CONSOLE Static Functions for Console System Service

  Company:
    Microchip Technology Inc.

  File Name:
    sys_console_static.c

  Summary:
    SYS CONSOLE static function implementations for the Console System Service.

  Description:
    The Console System Service provides a simple interface to manage the oscillators
    on Microchip microcontrollers. This file defines the static implementation for the 
    Console System Service.
    
  Remarks:
    Static functions incorporate all system clock configuration settings as
    determined by the user via the Microchip Harmony Configurator GUI.  It provides 
    static version of the routines, eliminating the need for an object ID or 
    object handle.
    
    Static single-open interfaces also eliminate the need for the open handle.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2014 released Microchip Technology Inc.  All rights reserved.

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

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************

#include "system_config.h"
#include "system_definitions.h"
#include "system/console/sys_console.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Variable Definitions
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
/* Function:
    SYS_MODULE_OBJ SYS_CONSOLE_Initialize( const SYS_MODULE_INDEX index,
                                       const SYS_MODULE_INIT * const init )

  Summary:
    Initializes data for the instance of the Console module and opens the 
    specific module instance.

  Description:
    This function initializes the Console module, and selects the I/O device to
    be used. It also initializes any internal data structures.

  Precondition:
    None.

  Parameters:
    index           - Index for the instance to be initialized

    init            - Pointer to a data structure containing any data necessary
                      to initialize the sys console. This pointer may be null if no
                      data is required because static overrides have been
                      provided.

 Returns:
    If successful, returns a valid handle to an object.  Otherwise, it
    returns SYS_MODULE_OBJ_INVALID. The returned object must be passed as
    argument to SYS_CONSOLE_Reinitialize, SYS_CONSOLE_Deinitialize, 
    SYS_CONSOLE_Tasks and SYS_CONSOLE_Status routines.

  Remarks:
    This routine should only be called once during system initialization
    unless SYS_Console_Deinitialize is first called to deinitialize the device
    instance before reinitializing it. If the system was already initialized
    it safely returns without causing any disturbance.
*/

SYS_MODULE_OBJ SYS_CONSOLE_Initialize( const SYS_MODULE_INDEX index, const SYS_MODULE_INIT * const init )
{
    return 1;
}


// *****************************************************************************
/* Function:
     void SYS_CONSOLE_Reinitialize ( SYS_MODULE_OBJ object,
                                 const SYS_MODULE_INIT * const init )

   Summary:
     Reinitializes and refreshes the data structure for the instance of the
     Console module.

   Description:
     This function reinitializes and refreshes the data structure for the
     instance of the Console module.

  PreCondition:
     The SYS_CONSOLE_Initialize function should have been called before calling
     this function.

  Parameters:
    object          - Identifies the SYS CONSOLE Object returned by the Initialize
                      interface
    init            - Pointer to the data structure containing any data
                      necessary to initialize the hardware

   Returns:
    - true	- if successful
    - false	- if unsuccessful
*/

bool SYS_CONSOLE_Reinitialize( SYS_MODULE_OBJ object, const SYS_MODULE_INIT * const init )
{
    return true;
}


// *****************************************************************************
/* Function:
    void SYS_CONSOLE_Deinitialize ( SYS_MODULE_OBJ object )

  Summary:
    Deinitializes the specific module instance of the Console module.

  Description:
    Deinitializes the specific module instance disabling its operation (and
    any hardware for driver modules). Resets all the internal data
    structures and fields for the specified instance to the default
    settings.

  PreCondition:
    The SYS_CONSOLE_Initialize function should have been called before calling
    this function.

  Parameters:
    object    - SYS CONSOLE object handle, returned from SYS_CONSOLE_Initialize

   Returns:
    - true	- if successful
    - false	- if unsuccessful
*/

bool SYS_CONSOLE_Deinitialize ( SYS_MODULE_OBJ object )
{
    return true;
}


// *****************************************************************************
/* Function:
    SYS_STATUS SYS_CONSOLE_Status ( SYS_MODULE_OBJ object )

  Summary:
    Returns status of the specific module instance of the Console module.

  Description:
    This function returns the status of the specific module instance disabling its
    operation (and any hardware for driver modules).

  PreCondition:
    The SYS_CONSOLE_Initialize function should have been called before calling
    this function.

  Parameters:
    object   - SYS CONSOLE object handle, returned from SYS_CONSOLE_Initialize

  Returns:
    SYS_STATUS_READY    Indicates that any previous module operation for the
                        specified module has completed

    SYS_STATUS_BUSY     Indicates that a previous module operation for the
                        specified module has not yet completed

    SYS_STATUS_ERROR    Indicates that the specified module is in an error state
*/

SYS_STATUS SYS_CONSOLE_Status ( SYS_MODULE_OBJ object )
{
    return SYS_STATUS_READY;
}


// *****************************************************************************
/* Function:
    void SYS_CONSOLE_Tasks ( SYS_MODULE_OBJ object )

  Summary:
    Maintains the system console's state machine and implements its ISR.

  Description:
    This routine is used to maintain the system console's internal state machine 
    and implement its ISR for interrupt-driven implementations.

  Precondition:
    The SYS_CONSOLE_Initialize function must have been called for the specified 
    console instance.

  Parameters:
    object   - SYS CONSOLE object handle, returned from SYS_CONSOLE_Initialize

  Returns:
    None.
*/

void SYS_CONSOLE_Tasks ( SYS_MODULE_OBJ object )
{
}

// *****************************************************************************
/* Function:
    ssize_t SYS_CONSOLE_Read( const SYS_MODULE_INDEX index, int fd, void *buf, size_t count )

  Summary:
    Reads data from the console device.

  Description:
    This function reads the data from the console device.

  Preconditions:
    None.

  Parameters:
    index           - Console instance index
    fd              - This field is not used
                      Maintained for backward compatibility
                      NULL value can be passed as a parameter
    buf             - Buffer to hold the read data.
    count           - Number of bytes to read.

  Returns:
    Number of bytes actually read.

  Remarks:
    None.
*/

ssize_t SYS_CONSOLE_Read(const SYS_MODULE_INDEX index, int fd, void *buf, size_t count )
{
    size_t  numBytes = 0;
    char* pReadByte = (char*)buf;
	if( !DRV_USART0_ReceiverBufferIsEmpty() )
	{
		*pReadByte = DRV_USART0_ReadByte();

		numBytes++;
		pReadByte++;
    }

    return numBytes;
}


// *****************************************************************************
/* Function:
    ssize_t SYS_CONSOLE_Write( const SYS_MODULE_INDEX index, int fd, const void *buffer, size_t count )

  Summary:
    Writes data to the console device.

  Description:
    This function writes data to the console device.

  Preconditions:
    None.

  Parameters:
    index           - Console instance index
    fd              - This field is not used
                      Maintained for backward compatibility
                      NULL value can be passed as a parameter
    buf             - Buffer holding the data to be written.
    count           - Number of bytes to write.

  Returns:
    Number of bytes written or -1 if error.

  Remarks:
    None.
*/

ssize_t SYS_CONSOLE_Write(const SYS_MODULE_INDEX index, int fd, const char *buf, size_t count )
{
    size_t numBytes = 0;
    while( numBytes < count )
    {
        DRV_USART0_WriteByte(buf[numBytes++]);
    }
    return count;
}


// *****************************************************************************
/* Function:
    void SYS_CONSOLE_RegisterCallback(const SYS_MODULE_INDEX index, consoleCallbackFunction cbFunc, SYS_CONSOLE_EVENT event)

  Summary:
    Registers a callback function with the console service that will be
    executed when the read or write queue is emptied.

  Description:
    This function is used by an application to register a callback function
    with the console service. The callback function is called in response to
    an event. Separate callback functions are required for each event.

  Preconditions:
    None.

  Parameters:
    index               - Console instance index
    consCallbackFunc    - The name of the callback function
    event               - Enumerated list of events that can trigger a callback

  Returns:
    None.

  Remarks:
    None.
*/

void SYS_CONSOLE_RegisterCallback(const SYS_MODULE_INDEX index, consoleCallbackFunction cbFunc, SYS_CONSOLE_EVENT event)
{
}


// *****************************************************************************
/* Function:
    void SYS_CONSOLE_Flush(const SYS_MODULE_INDEX index)

  Summary:
    Flushes the read and write queues and resets an overflow error for the
    console.

  Description:
    This function flushes the read and write queues and resets an overflow
    error for the console.

  Preconditions:
    None.

  Parameters:
    index               - Console instance index

  Returns:
    None.

  Remarks:
    None.
*/

void SYS_CONSOLE_Flush(const SYS_MODULE_INDEX index)
{
}

                      
/*******************************************************************************
 End of File
*/
