// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "mongoose.h"

static void blink_task(void) {
  static uint64_t blink_timer = 0;
  if (mg_timer_expired(&blink_timer, 500, mg_millis())) {
    hal_gpio_toggle(LED);
  }
}

int main(void) {
  hal_system_init();
  hal_rng_init();
  hal_gpio_write(LED, true);  // Off
  hal_gpio_output(LED);

  struct mg_mgr mgr;
  mg_mgr_init(&mgr);   // Also starts the USB driver, see mongoose_config.h
  mg_dash_init(&mgr);  // Use tutorials/device-dashboard/full/, see Makefile

  MG_INFO(("Initialised. CPU clock: %lu MHz", SystemCoreClock / 1000000));
  MG_INFO(("RAM used: %zu, free: %zu", hal_ram_used(), hal_ram_free()));
  MG_INFO(("USB Ethernet + serial"));

  for (;;) {
    mg_mgr_poll(&mgr, 0);
    mg_dash_poll(&mgr);
    blink_task();
  }

  return 0;
}
