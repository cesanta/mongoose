// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "hardware/clocks.h"
#include "mongoose.h"
#include "pico/stdlib.h"

#define LED1 PICO_DEFAULT_LED_PIN  // User LED, active low
//#define LED1 16  // User LED, active low

static void blink_task(void) {
  static uint64_t blink_timer = 0;
  if (mg_timer_expired(&blink_timer, 500, mg_millis())) {
    gpio_xor_mask(1u << LED1);
  }
}

// Match MG24's heap-extent measurements. The Pico SDK's newlib _sbrk(0)
// returns the current heap break without allocating. Its linker reserves
// stacks separately, so free space ends at __HeapLimit, not the current SP.
// These figures exclude globals/stacks and reusable blocks inside malloc.
extern void *_sbrk(int incr);
extern char __end__, __HeapLimit;

static size_t hal_ram_used(void) {
  return (size_t) ((uintptr_t) _sbrk(0) - (uintptr_t) &__end__);
}

static size_t hal_ram_free(void) {
  return (size_t) ((uintptr_t) &__HeapLimit - (uintptr_t) _sbrk(0));
}

int main(void) {
  gpio_init(LED1);
  gpio_put(LED1, 1);
  gpio_set_dir(LED1, GPIO_OUT);

  struct mg_mgr mgr;
  mg_mgr_init(&mgr);   // Also starts the USB driver, see mongoose_config.h
  mg_dash_init(&mgr);  // Use tutorials/device-dashboard/full/, see Makefile

  MG_INFO(("Initialised. CPU clock: %lu MHz", clock_get_hz(clk_sys) / 1000000));
  MG_INFO(("RAM used: %zu, free: %zu", hal_ram_used(), hal_ram_free()));
  MG_INFO(("USB Ethernet + serial"));

  for (;;) {
    mg_mgr_poll(&mgr, 0);
    mg_dash_poll(&mgr);
    blink_task();
  }

  return 0;
}
