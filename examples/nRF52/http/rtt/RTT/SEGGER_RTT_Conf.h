/* clang-format off */
/*********************************************************************
*              SEGGER MICROCONTROLLER SYSTEME GmbH                   *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996-2014 SEGGER Microcontroller Systeme GmbH           *
*                                                                    *
* Internet: www.segger.com Support: support@segger.com               *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File    : SEGGER_RTT_Conf.h
Date    : 17 Dec 2014
Purpose : Implementation of SEGGER real-time terminal which allows
          real-time terminal communication on targets which support
          debugger memory accesses while the CPU is running.
---------------------------END-OF-HEADER------------------------------
*/

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

#define SEGGER_RTT_MAX_NUM_UP_BUFFERS             (2)     // Max. number of up-buffers (T->H) available on this target    (Default: 2)
#define SEGGER_RTT_MAX_NUM_DOWN_BUFFERS           (2)     // Max. number of down-buffers (H->T) available on this target  (Default: 2)

#define BUFFER_SIZE_UP                            (2048)  // Size of the buffer for terminal output of target, up to host (Default: 1k)
#define BUFFER_SIZE_DOWN                          (16)    // Size of the buffer for terminal input to target from host (Usually keyboard input) (Default: 16)

#define SEGGER_RTT_PRINTF_BUFFER_SIZE             (64)    // Size of buffer for RTT printf to bulk-send chars via RTT     (Default: 64)

//
// Target is not allowed to perform other RTT operations while string still has not been stored completely.
// Otherwise we would probably end up with a mixed string in the buffer.
// If using  RTT from within interrupts, multiple tasks or multi processors, define the SEGGER_RTT_LOCK() and SEGGER_RTT_UNLOCK() function here.
//
#define SEGGER_RTT_LOCK()
#define SEGGER_RTT_UNLOCK()

//
// Define SEGGER_RTT_IN_RAM as 1
// when using RTT in RAM targets (init and data section both in RAM).
// This prevents the host to falsly identify the RTT Callback Structure
// in the init segment as the used Callback Structure.
//
// When defined as 1,
// the first call to an RTT function will modify the ID of the RTT Callback Structure.
// To speed up identifying on the host,
// especially when RTT functions are not called at the beginning of execution,
// SEGGER_RTT_Init() should be called at the start of the application.
//
#define SEGGER_RTT_IN_RAM                         (0)

/*************************** End of file ****************************/
