/*
 * Copyright (c) 2016 Cesanta Software Limited
 * All rights reserved
 */

#include <stddef.h>
#include <stdlib.h>
#include "system/common/sys_module.h"

int main(void) {
  /* Initialize all MPLAB Harmony modules, including application(s). */
  SYS_Initialize(NULL);

  while (1) {
    /* Maintain state machines of all polled MPLAB Harmony modules. */
    SYS_Tasks();
  }

  /* Execution should not come here during normal operation */

  return (EXIT_FAILURE);
}
