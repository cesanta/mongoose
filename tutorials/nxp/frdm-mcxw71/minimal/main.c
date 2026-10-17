// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "mongoose.h"

#define UART_DEBUG LPUART1  // MCU-Link USB serial port

static void log_fn(char ch, void *param) {
  hal_uart_write_buf(param, &ch, 1);
}

uint64_t mg_millis(void) {
  return hal_get_tick();
}

int main(void) {
  hal_clock_init();
  hal_uart_init(UART_DEBUG, 115200);
  mg_log_set_fn(log_fn, UART_DEBUG);

  MG_INFO(("Initialised. CPU clock: %lu MHz", SystemCoreClock / 1000000));
  MG_INFO(("RAM used: %zu, free: %zu", hal_ram_used(), hal_ram_free()));

  for (;;) {
    static uint64_t timer;
    if (hal_timer_expired(&timer, 1000, hal_get_tick())) {
      MG_INFO(("Hi from Mongoose, tick %llu", hal_get_tick()));
    }
  }

  return 0;
}
