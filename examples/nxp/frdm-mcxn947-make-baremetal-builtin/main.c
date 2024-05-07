// Copyright (c) 2024 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "mongoose.h"
#include "net.h"

#define BLINK_PERIOD_MS 1000

static void timer_fn(void *arg) {
  gpio_toggle(LED1);  // Blink LED
  (void) arg;         // Unused
}

int main(void) {
  struct mg_mgr mgr;  // Mongoose event manager

  hal_init();  // Cross-platform hardware init
  
  mg_mgr_init(&mgr);        // Initialise it
  mg_log_set(MG_LL_DEBUG);  // Set log level to debug
  mg_timer_add(&mgr, BLINK_PERIOD_MS, MG_TIMER_REPEAT, timer_fn, &mgr);

  MG_INFO(("Initialising application..."));
  web_init(&mgr);

  for (;;) {
    mg_mgr_poll(&mgr, 0);
  }

  return 0;
}
