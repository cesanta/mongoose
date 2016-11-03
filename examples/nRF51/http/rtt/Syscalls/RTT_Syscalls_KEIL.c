/* clang-format off */
/*********************************************************************
*               SEGGER MICROCONTROLLER GmbH & Co KG                  *
*       Solutions for real time microcontroller applications         *
**********************************************************************
*                                                                    *
*       (c) 2014  SEGGER Microcontroller GmbH & Co KG                *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************

----------------------------------------------------------------------
File    : RTT_Syscalls_KEIL.c
Purpose : Retargeting module for KEIL MDK-CM3.
          Low-level functions for using printf() via RTT
--------- END-OF-HEADER --------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rt_sys.h>
#include <rt_misc.h>

#include "SEGGER_RTT.h"
/*********************************************************************
*
*       #pragmas
*
**********************************************************************
*/
#pragma import(__use_no_semihosting)

#ifdef _MICROLIB
  #pragma import(__use_full_stdio)
#endif

/*********************************************************************
*
*       Defines non-configurable
*
**********************************************************************
*/

/* Standard IO device handles - arbitrary, but any real file system handles must be
   less than 0x8000. */
#define STDIN             0x8001    // Standard Input Stream
#define STDOUT            0x8002    // Standard Output Stream
#define STDERR            0x8003    // Standard Error Stream

/*********************************************************************
*
*       Public const
*
**********************************************************************
*/
const char __stdin_name[]  = "STDIN";
const char __stdout_name[] = "STDOUT";
const char __stderr_name[] = "STDERR";

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

/*********************************************************************
*
*       _ttywrch
*
*  Function description:
*    Outputs a character to the console
*
*  Parameters:
*    c    - character to output
*  
*/
void _ttywrch(int c) {
  fputc(c, stdout); // stdout
  fflush(stdout);
}

/*********************************************************************
*
*       _sys_open
*
*  Function description:
*    Opens the device/file in order to do read/write operations
*
*  Parameters:
*    sName        - sName of the device/file to open
*    OpenMode    - This parameter is currently ignored
*  
*  Return value:
*    != 0     - Handle to the object to open, otherwise 
*    == 0     -"device" is not handled by this module
*
*/
FILEHANDLE _sys_open(const char * sName, int OpenMode) {
  // Register standard Input Output devices.
  if (strcmp(sName, __stdout_name) == 0) {
    return (STDOUT);
  } else if (strcmp(sName, __stderr_name) == 0) {
    return (STDERR);
  } else
  return (0);  // Not implemented
}

/*********************************************************************
*
*       _sys_close
*
*  Function description:
*    Closes the handle to the open device/file
*
*  Parameters:
*    hFile    - Handle to a file opened via _sys_open
*  
*  Return value:
*    0     - device/file closed
*
*/
int _sys_close(FILEHANDLE hFile) {
  return 0;  // Not implemented
}

/*********************************************************************
*
*       _sys_write
*
*  Function description:
*    Writes the data to an open handle.
*    Currently this function only outputs data to the console
*
*  Parameters:
*    hFile    - Handle to a file opened via _sys_open
*    pBuffer  - Pointer to the data that shall be written
*    NumBytes      - Number of bytes to write
*    Mode     - The Mode that shall be used
*  
*  Return value:
*    Number of bytes *not* written to the file/device
*
*/
int _sys_write(FILEHANDLE hFile, const unsigned char * pBuffer, unsigned NumBytes, int Mode) {
  int r = 0;

  if (hFile == STDOUT) {
    return NumBytes - SEGGER_RTT_Write(0, (const char*)pBuffer, NumBytes);
  }
  return r;
}

/*********************************************************************
*
*       _sys_read
*
*  Function description:
*    Reads data from an open handle.
*    Currently this modules does nothing.
*
*  Parameters:
*    hFile    - Handle to a file opened via _sys_open
*    pBuffer  - Pointer to buffer to store the read data
*    NumBytes      - Number of bytes to read
*    Mode     - The Mode that shall be used
*  
*  Return value:
*    Number of bytes read from the file/device
*
*/
int _sys_read(FILEHANDLE hFile, unsigned char * pBuffer, unsigned NumBytes, int Mode) {
  return (0);  // Not implemented
}

/*********************************************************************
*
*       _sys_istty
*
*  Function description:
*    This function shall return whether the opened file 
*    is a console device or not.
*
*  Parameters:
*    hFile    - Handle to a file opened via _sys_open
*  
*  Return value:
*    1       - Device is     a console
*    0       - Device is not a console
*
*/
int _sys_istty(FILEHANDLE hFile) {
  if (hFile > 0x8000) {
    return (1);
  }
  return (0);  // Not implemented
}

/*********************************************************************
*
*       _sys_seek
*
*  Function description:
*    Seeks via the file to a specific position
*
*  Parameters:
*    hFile  - Handle to a file opened via _sys_open
*    Pos    - 
*  
*  Return value:
*    int       - 
*
*/
int _sys_seek(FILEHANDLE hFile, long Pos) {
  return (0);  // Not implemented
}

/*********************************************************************
*
*       _sys_ensure
*
*  Function description:
*    
*
*  Parameters:
*    hFile    - Handle to a file opened via _sys_open
*  
*  Return value:
*    int       - 
*
*/
int _sys_ensure(FILEHANDLE hFile) {
  return (-1);  // Not implemented
}

/*********************************************************************
*
*       _sys_flen
*
*  Function description:
*    Returns the length of the opened file handle
*
*  Parameters:
*    hFile    - Handle to a file opened via _sys_open
*  
*  Return value:
*    Length of the file
*
*/
long _sys_flen(FILEHANDLE hFile) {
  return (0);  // Not implemented
}

/*********************************************************************
*
*       _sys_tmpnam
*
*  Function description:
*    This function converts the file number fileno for a temporary 
*    file to a unique filename, for example, tmp0001.
*
*  Parameters:
*    pBuffer    - Pointer to a buffer to store the name
*    FileNum    - file number to convert
*    MaxLen     - Size of the buffer
*  
*  Return value:
*     1 - Error
*     0 - Success  
*
*/
int _sys_tmpnam(char * pBuffer, int FileNum, unsigned MaxLen) {
  return (1);  // Not implemented
}

/*********************************************************************
*
*       _sys_command_string
*
*  Function description:
*    This function shall execute a system command.
*
*  Parameters:
*    cmd    - Pointer to the command string
*    len    - Length of the string
*  
*  Return value:
*    == NULL - Command was not successfully executed
*    == sCmd - Command was passed successfully
*
*/
char * _sys_command_string(char * cmd, int len) {
  return cmd;  // Not implemented
}

/*********************************************************************
*
*       _sys_exit
*
*  Function description:
*    This function is called when the application returns from main
*
*  Parameters:
*    ReturnCode    - Return code from the main function
*  
*
*/
void _sys_exit(int ReturnCode) {
  while (1);  // Not implemented
}
