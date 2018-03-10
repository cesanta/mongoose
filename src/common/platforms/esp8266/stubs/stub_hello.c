/*
 * Copyright (c) 2015 Cesanta Software Limited
 * All rights reserved
 *
 *
 * Stub template.
 */

#include <inttypes.h>
#include "rom_functions.h"

/* Define the args vector and put it into the ".params" section. */
uint32_t params[3] __attribute__((section(".params")));

/* Define a function called stub_main. Do not return or reboot.
 * Use send_packet to communicate to the host. */

const char *hello = "Hello";

void stub_main(void) {
  send_packet(hello, 5);
  _ResetVector();
}
