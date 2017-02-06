/* clang-format off */
/*******************************************************************************
  MPLAB Harmony Exceptions Source File

  File Name:
    system_exceptions.c

  Summary:
    This file contains a function which overrides the deafult _weak_ exception 
    handler provided by the XC32 compiler.

  Description:
    This file redefines the default _weak_  exception handler with a more debug
    friendly one. If an unexpected exception occurs the code will stop in a
    while(1) loop.  The debugger can be halted and two variables _excep_code and
    _except_addr can be examined to determine the cause and address where the
    exception occured.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2015 released Microchip Technology Inc.  All rights reserved.

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
// DOM-IGNORE-END


#include <xc.h>                 /* Defines special funciton registers, CP0 regs  */
#include "system_config.h"
#include "system_definitions.h"
#include "system/debug/sys_debug.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Exception Reason Data
  
  <editor-fold defaultstate="expanded" desc="Exception Reason Data">
  
  Remarks:
    These global static items are used instead of local variables in the 
    _general_exception_handler function because the stack may not be available
    if an exception has occured.
*/

/* Code identifying the cause of the exception (CP0 Cause register). */
static unsigned int _excep_code;

/* Address of instruction that caused the exception. */
static unsigned int _excep_addr;

/* Pointer to the string describing the cause of the exception. */
static char *_cause_str;

/* Array identifying the cause (indexed by _exception_code). */
static char *cause[] = 
{
    "Interrupt",
    "Undefined",
    "Undefined",
    "Undefined",
    "Load/fetch address error",
    "Store address error",
    "Instruction bus error",
    "Data bus error",
    "Syscall",
    "Breakpoint",
    "Reserved instruction",
    "Coprocessor unusable",
    "Arithmetic overflow",
    "Trap",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

// </editor-fold>


// *****************************************************************************
// *****************************************************************************
// Section: Exception Handling
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void _general_exception_handler ( void )

  Summary:
    Overrides the XC32 _weak_ _generic_exception_handler.
    
  Description:
    This function overrides the XC32 default _weak_ _generic_exception_handler.

  Remarks:
    Refer to the XC32 User's Guide for additional information.
 */

void _general_exception_handler ( void )
{
    /* Mask off Mask of the ExcCode Field from the Cause Register
    Refer to the MIPs Software User's manual */
    _excep_code = (_CP0_GET_CAUSE() & 0x0000007C) >> 2;
    _excep_addr = _CP0_GET_EPC();
    _cause_str  = cause[_excep_code];

    SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "\nGeneral Exception %s (cause=%d, addr=%x).\n", 
                    _cause_str, _excep_code, _excep_addr);

    while (1)
    {
        SYS_DEBUG_BreakPoint();
    }
}

/*******************************************************************************
 End of File
*/
