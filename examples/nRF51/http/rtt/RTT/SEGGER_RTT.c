/* clang-format off */
/*********************************************************************
*              SEGGER MICROCONTROLLER GmbH & Co. KG                  *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 2014-2014 SEGGER Microcontroller GmbH & Co. KG          *
*                                                                    *
*       Internet: www.segger.com Support: support@segger.com         *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File    : SEGGER_RTT.c
Date    : 17 Dec 2014
Purpose : Implementation of SEGGER real-time terminal (RTT) which allows
          real-time terminal communication on targets which support
          debugger memory accesses while the CPU is running.

          Type "int" is assumed to be 32-bits in size
          H->T    Host to target communication
          T->H    Target to host communication

          RTT channel 0 is always present and reserved for Terminal usage.
          Name is fixed to "Terminal"

---------------------------END-OF-HEADER------------------------------
*/

#include "SEGGER_RTT_Conf.h"
#include "SEGGER_RTT.h"

#include <string.h>                 // for memcpy

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

#ifndef   BUFFER_SIZE_UP
  #define BUFFER_SIZE_UP                                  (1024)  // Size of the buffer for terminal output of target, up to host
#endif

#ifndef   BUFFER_SIZE_DOWN
  #define BUFFER_SIZE_DOWN                                (16)    // Size of the buffer for terminal input to target from host (Usually keyboard input)
#endif

#ifndef   SEGGER_RTT_MAX_NUM_UP_BUFFERS
  #define SEGGER_RTT_MAX_NUM_UP_BUFFERS                   (1)     // Number of up-buffers (T->H) available on this target
#endif

#ifndef   SEGGER_RTT_MAX_NUM_DOWN_BUFFERS
  #define SEGGER_RTT_MAX_NUM_DOWN_BUFFERS                 (1)     // Number of down-buffers (H->T) available on this target
#endif

#ifndef   SEGGER_RTT_LOCK
  #define SEGGER_RTT_LOCK()
#endif

#ifndef   SEGGER_RTT_UNLOCK
  #define SEGGER_RTT_UNLOCK()
#endif

#ifndef   SEGGER_RTT_IN_RAM
  #define SEGGER_RTT_IN_RAM                               (0)
#endif

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/

#define MIN(a, b)        (((a) < (b)) ? (a) : (b))
#define MAX(a, b)        (((a) > (b)) ? (a) : (b))

#define MEMCPY(a, b, c)  memcpy((a),(b),(c))

//
// For some environments, NULL may not be defined until certain headers are included
//
#ifndef NULL
  #define NULL 0
#endif

/*********************************************************************
*
*       Types
*
**********************************************************************
*/

//
// Description for a circular buffer (also called "ring buffer")
// which is used as up- (T->H) or down-buffer (H->T)
//
typedef struct {
  const char* sName;                     // Optional name. Standard names so far are: "Terminal", "VCom"
  char*  pBuffer;                        // Pointer to start of buffer
  int    SizeOfBuffer;                   // Buffer size in bytes. Note that one byte is lost, as this implementation does not fill up the buffer in order to avoid the problem of being unable to distinguish between full and empty.
  volatile int WrOff;                    // Position of next item to be written by either host (down-buffer) or target (up-buffer). Must be volatile since it may be modified by host (down-buffer)
  volatile int RdOff;                    // Position of next item to be read by target (down-buffer) or host (up-buffer). Must be volatile since it may be modified by host (up-buffer)
  int    Flags;                          // Contains configuration flags
} RING_BUFFER;

//
// RTT control block which describes the number of buffers available
// as well as the configuration for each buffer
//
//
typedef struct {
  char        acID[16];                                 // Initialized to "SEGGER RTT"
  int         MaxNumUpBuffers;                          // Initialized to SEGGER_RTT_MAX_NUM_UP_BUFFERS (type. 2)
  int         MaxNumDownBuffers;                        // Initialized to SEGGER_RTT_MAX_NUM_DOWN_BUFFERS (type. 2)
  RING_BUFFER aUp[SEGGER_RTT_MAX_NUM_UP_BUFFERS];       // Up buffers, transferring information up from target via debug probe to host
  RING_BUFFER aDown[SEGGER_RTT_MAX_NUM_DOWN_BUFFERS];   // Down buffers, transferring information down from host via debug probe to target
} SEGGER_RTT_CB;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
//
// Allocate buffers for channel 0
//
static char _acUpBuffer  [BUFFER_SIZE_UP];
static char _acDownBuffer[BUFFER_SIZE_DOWN];
//
// Initialize SEGGER Real-time-Terminal control block (CB)
//
static SEGGER_RTT_CB _SEGGER_RTT = {
#if SEGGER_RTT_IN_RAM
  "SEGGER RTTI",
#else
  "SEGGER RTT",
#endif
  SEGGER_RTT_MAX_NUM_UP_BUFFERS,
  SEGGER_RTT_MAX_NUM_DOWN_BUFFERS,
  {{ "Terminal", &_acUpBuffer[0],   sizeof(_acUpBuffer),   0, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP }},
  {{ "Terminal", &_acDownBuffer[0], sizeof(_acDownBuffer), 0, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP }},
};

static char _ActiveTerminal;

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       _strlen
*
*  Function description
*    ANSI compatible function to determine the length of a string
*
*  Return value
*    Length of string in bytes.
*
*  Parameters
*    s         Pointer to \0 terminated string.
*
*  Notes
*    (1) s needs to point to an \0 terminated string. Otherwise proper functionality of this function is not guaranteed.
*/
static int _strlen(const char* s) {
  int Len;

  Len = 0;
  if (s == NULL) {
    return 0;
  }
  do {
    if (*s == 0) {
      break;
    }
    Len++;
    s++;
  } while (1);
  return Len;
}

/*********************************************************************
*
*       _Init
*
*  Function description
*    In case SEGGER_RTT_IN_RAM is defined,
*    _Init() modifies the ID of the RTT CB to allow identifying the
*    RTT Control Block Structure in the data segment.
*/
static void _Init(void) {
#if SEGGER_RTT_IN_RAM
  if (_SEGGER_RTT.acID[10] == 'I') {
    _SEGGER_RTT.acID[10] = '\0';
  }
#endif
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       SEGGER_RTT_Read
*
*  Function description
*    Reads characters from SEGGER real-time-terminal control block
*    which have been previously stored by the host.
*
*  Parameters
*    BufferIndex  Index of Down-buffer to be used. (e.g. 0 for "Terminal")
*    pBuffer      Pointer to buffer provided by target application, to copy characters from RTT-down-buffer to.
*    BufferSize   Size of the target application buffer
*
*  Return values
*    Number of bytes that have been read
*/
int SEGGER_RTT_Read(unsigned BufferIndex, char* pBuffer, unsigned BufferSize) {
  int NumBytesRem;
  unsigned NumBytesRead;
  int RdOff;
  int WrOff;

  SEGGER_RTT_LOCK();
  _Init();
  RdOff = _SEGGER_RTT.aDown[BufferIndex].RdOff;
  WrOff = _SEGGER_RTT.aDown[BufferIndex].WrOff;
  NumBytesRead = 0;
  //
  // Read from current read position to wrap-around of buffer, first
  //
  if (RdOff > WrOff) {
    NumBytesRem = _SEGGER_RTT.aDown[BufferIndex].SizeOfBuffer - RdOff;
    NumBytesRem = MIN(NumBytesRem, (int)BufferSize);
    MEMCPY(pBuffer, _SEGGER_RTT.aDown[BufferIndex].pBuffer + RdOff, NumBytesRem);
    NumBytesRead += NumBytesRem;
    pBuffer      += NumBytesRem;
    BufferSize   -= NumBytesRem;
    RdOff        += NumBytesRem;
    //
    // Handle wrap-around of buffer
    //
    if (RdOff == _SEGGER_RTT.aDown[BufferIndex].SizeOfBuffer) {
      RdOff = 0;
    }
  }
  //
  // Read remaining items of buffer
  //
  NumBytesRem = WrOff - RdOff;
  NumBytesRem = MIN(NumBytesRem, (int)BufferSize);
  if (NumBytesRem > 0) {
    MEMCPY(pBuffer, _SEGGER_RTT.aDown[BufferIndex].pBuffer + RdOff, NumBytesRem);
    NumBytesRead += NumBytesRem;
    pBuffer      += NumBytesRem;
    BufferSize   -= NumBytesRem;
    RdOff        += NumBytesRem;
  }
  if (NumBytesRead) {
    _SEGGER_RTT.aDown[BufferIndex].RdOff = RdOff;
  }
  SEGGER_RTT_UNLOCK();
  return NumBytesRead;
}

/*********************************************************************
*
*       SEGGER_RTT_Write
*
*  Function description
*    Stores a specified number of characters in SEGGER RTT
*    control block which is then read by the host.
*
*  Parameters
*    BufferIndex  Index of "Up"-buffer to be used. (e.g. 0 for "Terminal")
*    pBuffer      Pointer to character array. Does not need to point to a \0 terminated string.
*    NumBytes     Number of bytes to be stored in the SEGGER RTT control block.
*
*  Return values
*    Number of bytes which have been stored in the "Up"-buffer.
*
*  Notes
*    (1) If there is not enough space in the "Up"-buffer, remaining characters of pBuffer are dropped.
*/
int SEGGER_RTT_Write(unsigned BufferIndex, const char* pBuffer, unsigned NumBytes) {
  int NumBytesToWrite;
  unsigned NumBytesWritten;
  int RdOff;
  //
  // Target is not allowed to perform other RTT operations while string still has not been stored completely.
  // Otherwise we would probably end up with a mixed string in the buffer.
  //
  SEGGER_RTT_LOCK();
  _Init();
  //
  // In case we are not in blocking mode,
  // we need to calculate, how many bytes we can put into the buffer at all.
  //
  if ((_SEGGER_RTT.aUp[BufferIndex].Flags & SEGGER_RTT_MODE_MASK) != SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL) {
    RdOff = _SEGGER_RTT.aUp[BufferIndex].RdOff;
    NumBytesToWrite =  RdOff - _SEGGER_RTT.aUp[BufferIndex].WrOff - 1;
    if (NumBytesToWrite < 0) {
      NumBytesToWrite += _SEGGER_RTT.aUp[BufferIndex].SizeOfBuffer;
    }
    //
    // If the complete data does not fit in the buffer, check if we have to skip it completely or trim the data
    //
    if ((int)NumBytes > NumBytesToWrite) {
      if ((_SEGGER_RTT.aUp[BufferIndex].Flags & SEGGER_RTT_MODE_MASK) == SEGGER_RTT_MODE_NO_BLOCK_SKIP) {
        SEGGER_RTT_UNLOCK();
        return 0;
      } else {
        NumBytes = NumBytesToWrite;
      }
    }
  }
  //
  // Early out if nothing is to do
  //
  if (NumBytes == 0) {
    SEGGER_RTT_UNLOCK();
    return 0;
  }
  //
  // Write data to buffer and handle wrap-around if necessary
  //
  NumBytesWritten = 0;
  do {
    RdOff = _SEGGER_RTT.aUp[BufferIndex].RdOff;                          // May be changed by host (debug probe) in the meantime
    NumBytesToWrite = RdOff - _SEGGER_RTT.aUp[BufferIndex].WrOff - 1;
    if (NumBytesToWrite < 0) {
      NumBytesToWrite += _SEGGER_RTT.aUp[BufferIndex].SizeOfBuffer;
    }
    NumBytesToWrite = MIN(NumBytesToWrite, (_SEGGER_RTT.aUp[BufferIndex].SizeOfBuffer - _SEGGER_RTT.aUp[BufferIndex].WrOff));    // Number of bytes that can be written until buffer wrap-around
    NumBytesToWrite = MIN(NumBytesToWrite, (int)NumBytes);
    MEMCPY(_SEGGER_RTT.aUp[BufferIndex].pBuffer + _SEGGER_RTT.aUp[BufferIndex].WrOff, pBuffer, NumBytesToWrite);
    NumBytesWritten     += NumBytesToWrite;
    pBuffer             += NumBytesToWrite;
    NumBytes            -= NumBytesToWrite;
    _SEGGER_RTT.aUp[BufferIndex].WrOff += NumBytesToWrite;
    if (_SEGGER_RTT.aUp[BufferIndex].WrOff == _SEGGER_RTT.aUp[BufferIndex].SizeOfBuffer) {
      _SEGGER_RTT.aUp[BufferIndex].WrOff = 0;
    }
  } while (NumBytes);
  SEGGER_RTT_UNLOCK();
  return NumBytesWritten;
}

/*********************************************************************
*
*       SEGGER_RTT_WriteString
*
*  Function description
*    Stores string in SEGGER RTT control block.
*    This data is read by the host.
*
*  Parameters
*    BufferIndex  Index of "Up"-buffer to be used. (e.g. 0 for "Terminal")
*    s            Pointer to string.
*
*  Return values
*    Number of bytes which have been stored in the "Up"-buffer.
*
*  Notes
*    (1) If there is not enough space in the "Up"-buffer, depending on configuration,
*        remaining characters may be dropped or RTT module waits until there is more space in the buffer.
*    (2) String passed to this function has to be \0 terminated
*    (3) \0 termination character is *not* stored in RTT buffer
*/
int SEGGER_RTT_WriteString(unsigned BufferIndex, const char* s) {
  int Len;

  Len = _strlen(s);
  return SEGGER_RTT_Write(BufferIndex, s, Len);
}

/*********************************************************************
*
*       SEGGER_RTT_GetKey
*
*  Function description
*    Reads one character from the SEGGER RTT buffer.
*    Host has previously stored data there.
*
*  Return values
*    <  0    No character available (buffer empty).
*    >= 0    Character which has been read. (Possible values: 0 - 255)
*
*  Notes
*    (1) This function is only specified for accesses to RTT buffer 0.
*/
int SEGGER_RTT_GetKey(void) {
  char c;
  int r;

  r = SEGGER_RTT_Read(0, &c, 1);
  if (r == 1) {
    return (int)(unsigned char)c;
  }
  return -1;
}

/*********************************************************************
*
*       SEGGER_RTT_WaitKey
*
*  Function description
*    Waits until at least one character is avaible in the SEGGER RTT buffer.
*    Once a character is available, it is read and this function returns.
*
*  Return values
*    >=0    Character which has been read.
*
*  Notes
*    (1) This function is only specified for accesses to RTT buffer 0
*    (2) This function is blocking if no character is present in RTT buffer
*/
int SEGGER_RTT_WaitKey(void) {
  int r;

  do {
    r = SEGGER_RTT_GetKey();
  } while (r < 0);
  return r;
}

/*********************************************************************
*
*       SEGGER_RTT_HasKey
*
*  Function description
*    Checks if at least one character for reading is available in the SEGGER RTT buffer.
*
*  Return values
*    0      No characters are available to read.
*    1      At least one character is available.
*
*  Notes
*    (1) This function is only specified for accesses to RTT buffer 0
*/
int SEGGER_RTT_HasKey(void) {
  int RdOff;

  _Init();
  RdOff = _SEGGER_RTT.aDown[0].RdOff;
  if (RdOff != _SEGGER_RTT.aDown[0].WrOff) {
    return 1;
  }
  return 0;
}

/*********************************************************************
*
*       SEGGER_RTT_ConfigUpBuffer
*
*  Function description
*    Run-time configuration of a specific up-buffer (T->H).
*    Buffer to be configured is specified by index.
*    This includes: Buffer address, size, name, flags, ...
*
*  Return value
*    >= 0  O.K.
*     < 0  Error
*/
int SEGGER_RTT_ConfigUpBuffer(unsigned BufferIndex, const char* sName, char* pBuffer, int BufferSize, int Flags) {
  _Init();
  if (BufferIndex < (unsigned)_SEGGER_RTT.MaxNumUpBuffers) {
    SEGGER_RTT_LOCK();
    if (BufferIndex > 0) {
      _SEGGER_RTT.aUp[BufferIndex].sName        = sName;
      _SEGGER_RTT.aUp[BufferIndex].pBuffer      = pBuffer;
      _SEGGER_RTT.aUp[BufferIndex].SizeOfBuffer = BufferSize;
      _SEGGER_RTT.aUp[BufferIndex].RdOff        = 0;
      _SEGGER_RTT.aUp[BufferIndex].WrOff        = 0;
    }
    _SEGGER_RTT.aUp[BufferIndex].Flags          = Flags;
    SEGGER_RTT_UNLOCK();
    return 0;
  }
  return -1;
}

/*********************************************************************
*
*       SEGGER_RTT_ConfigDownBuffer
*
*  Function description
*    Run-time configuration of a specific down-buffer (H->T).
*    Buffer to be configured is specified by index.
*    This includes: Buffer address, size, name, flags, ...
*
*  Return value
*    >= 0  O.K.
*     < 0  Error
*/
int SEGGER_RTT_ConfigDownBuffer(unsigned BufferIndex, const char* sName, char* pBuffer, int BufferSize, int Flags) {
  _Init();
  if (BufferIndex < (unsigned)_SEGGER_RTT.MaxNumDownBuffers) {
    SEGGER_RTT_LOCK();
    if (BufferIndex > 0) {
      _SEGGER_RTT.aDown[BufferIndex].sName        = sName;
      _SEGGER_RTT.aDown[BufferIndex].pBuffer      = pBuffer;
      _SEGGER_RTT.aDown[BufferIndex].SizeOfBuffer = BufferSize;
      _SEGGER_RTT.aDown[BufferIndex].RdOff        = 0;
      _SEGGER_RTT.aDown[BufferIndex].WrOff        = 0;
    }
    _SEGGER_RTT.aDown[BufferIndex].Flags          = Flags;
    SEGGER_RTT_UNLOCK();
    return 0;
  }
  return -1;
}

/*********************************************************************
*
*       SEGGER_RTT_Init
*
*  Function description
*    Initializes the RTT Control Block.
*    Should be used in RAM targets, at start of the application.
*
*/
void SEGGER_RTT_Init (void) {
  _Init();
}

/*********************************************************************
*
*       SEGGER_RTT_SetTerminal
*
*  Function description
*    Sets the terminal to be used for output on channel 0.
*
*/
void SEGGER_RTT_SetTerminal (char TerminalId) {
  char ac[2];

  ac[0] = 0xFF;
  if (TerminalId < 10) {
    ac[1] = '0' + TerminalId;
  } else if (TerminalId < 16) {
    ac[1] = 'A' + (TerminalId - 0x0A);
  } else {
    return; // RTT only supports up to 16 virtual terminals.
  }
  _ActiveTerminal = TerminalId;
  SEGGER_RTT_Write(0, ac, 2);
}

/*********************************************************************
*
*       SEGGER_RTT_TerminalOut
*
*  Function description
*    Writes a string to the given terminal
*     without changing the terminal for channel 0.
*
*/
int SEGGER_RTT_TerminalOut (char TerminalId, const char* s) {
  char ac[2];
  int  r;

  ac[0] = 0xFF;
  if (TerminalId < 10) {
    ac[1] = '0' + TerminalId;
  } else if (TerminalId < 16) {
    ac[1] = 'A' + (TerminalId - 0x0A);
  } else {
    return -1; // RTT only supports up to 16 virtual terminals.
  }
  SEGGER_RTT_Write(0, ac, 2);
  r = SEGGER_RTT_WriteString(0, s);
  if (TerminalId < 10) {
    ac[1] = '0' + _ActiveTerminal;
  } else if (TerminalId < 16) {
    ac[1] = 'A' + (_ActiveTerminal - 0x0A);
  }
  SEGGER_RTT_Write(0, ac, 2);
  return r;
}

/*************************** End of file ****************************/
