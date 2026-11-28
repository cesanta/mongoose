// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// Nucleo-U5A5ZJ-Q: Mongoose over USB. One USB-C cable on CN15 gives the board
// a CDC-NCM Ethernet interface and a CDC-ACM serial port that carries the log
// Important: set Nucleo's JP6 from st-link power to usb-c power

#include "hal.h"
#include "mongoose.h"

static void blink_task(void) {
  static uint64_t blink_timer = 0;
  if (mg_timer_expired(&blink_timer, 500, mg_millis())) hal_gpio_toggle(LED);
}

uint64_t mg_millis(void) {
  return hal_get_tick();
}

bool mg_random(void *buf, size_t len) {
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r = hal_rng_read();
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
  return true;
}

int main(void) {
  hal_rng_init();
  hal_gpio_output(LED);
  hal_usb_init();

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
