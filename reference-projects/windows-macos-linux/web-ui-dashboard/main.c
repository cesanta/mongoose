// Copyright (c) 2024 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "mongoose.h"
#include "net.h"

#define BLINK_PERIOD_MS 1000  // LED blinking period in millis

static void timer_fn(void *arg) {
  gpio_toggle(LED1);  // Blink LED
  (void) arg;         // Unused
}

// In RTOS environment, you can run this function in a separate task
static void run_mongoose(void) {
  struct mg_mgr mgr;        // Mongoose event manager
  mg_mgr_init(&mgr);        // Initialise event manager
  mg_log_set(MG_LL_DEBUG);  // Set log level to debug
  mg_timer_add(&mgr, BLINK_PERIOD_MS, MG_TIMER_REPEAT, timer_fn, NULL);

  net_init(&mgr);          // Initialise application defined in net.c
  for (;;) {               // Infinite event loop
    mg_mgr_poll(&mgr, 0);  // Process network events
  }
}

int main(void) {
  hal_init();      // Cross-platform hardware init
  run_mongoose();  // Initialise and run network application
  return 0;
}
