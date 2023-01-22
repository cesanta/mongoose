// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mcu.h"
#include "mongoose.h"

#define LED1 PIN('B', 0)      // On-board LED pin (green)
#define LED2 PIN('E', 1)      // On-board LED pin (yellow)
#define LED3 PIN('B', 14)     // On-board LED pin (red)
#define BLINK_PERIOD_MS 1000  // LED blinking period in millis

static uint64_t s_ticks;
void SysTick_Handler(void) { s_ticks++; }
uint64_t mg_millis(void) { return s_ticks; }

static void timer_cb(void *arg) {
  gpio_toggle(LED2);                      // Blink LED
  MG_INFO(("ticks: %lld", mg_millis()));  // Log something
  (void) arg;
}

int main(void) {
  clock_init();
  systick_init(SYS_FREQUENCY / 1000);  // Increment s_ticks every ms
  gpio_output(LED2);                   // Setup LED
  uart_init(UART_DEBUG, 115200);            // Initialise debug printf

  MG_INFO(("Initialising Mongoose..."));
  struct mg_mgr mgr;        // Initialise Mongoose event manager
  mg_mgr_init(&mgr);        // and attach it to the MIP interface
  mg_log_set(MG_LL_DEBUG);  // Set log level
  mg_timer_add(&mgr, BLINK_PERIOD_MS, MG_TIMER_REPEAT, timer_cb, NULL);

  MG_INFO(("Starting event loop"));
  for (;;) {
    mg_mgr_poll(&mgr, 0);
  }

  return 0;
}
