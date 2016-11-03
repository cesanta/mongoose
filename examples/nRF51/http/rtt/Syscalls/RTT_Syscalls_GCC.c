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
File    : RTT_Syscalls.c
Purpose : Low-level functions for using printf() via RTT in GCC
--------  END-OF-HEADER  ---------------------------------------------
*/
#include <stdlib.h>
#include "SEGGER_RTT.h"

/*********************************************************************
 *
 *       Function prototypes
 *
 **********************************************************************
 */
int _write(int file, char *ptr, int len);
int _write_r(struct _reent *r, int file, char *ptr, int len);

/*********************************************************************
 *
 *       Global functions
 *
 **********************************************************************
 */

/*********************************************************************
 *
 *       _write()
 *
 * Function description
 *   Low-level write function.
 *   libc subroutines will use this system routine for output to all files,
 *   including stdout.
 *   Write data via RTT.
 */
int _write(int file, char *ptr, int len) {
  (void) file;  /* Not used, avoid warning */
  SEGGER_RTT_Write(0, ptr, len);
  return len;
}

/*********************************************************************
 *
 *       _write_r()
 *
 * Function description
 *   Low-level reentrant write function.
 *   libc subroutines will use this system routine for output to all files,
 *   including stdout.
 *   Write data via RTT.
 */
int _write_r(struct _reent *r, int file, char *ptr, int len) {
  (void) file;  /* Not used, avoid warning */
  (void) r;  /* Not used, avoid warning */
  SEGGER_RTT_Write(0, ptr, len);
  return len;
}
