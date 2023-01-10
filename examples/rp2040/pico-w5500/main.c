// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#include <stdio.h>
#include <string.h>
#include "hardware/spi.h"
#include "pico/stdlib.h"

#include "mongoose.h"

enum { LED = 25, SPI_CS = 17, SPI_CLK = 18, SPI_TX = 19, SPI_RX = 16 };  // Pins
enum { STATUS_TIMER_MS = 3000, BLINK_TIMER_MS = 250 };  // Timeouts

static void spi_begin(void *spi) {
  gpio_put(SPI_CS, 0);
}

static void spi_end(void *spi) {
  gpio_put(SPI_CS, 1);
}

static uint8_t spi_txn(void *spi, uint8_t byte) {
  uint8_t result = 0;
  spi_write_read_blocking(spi0, &byte, &result, 1);
  // MG_INFO(("%x -> %x", byte, result));
  return result;
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    mg_http_reply(c, 200, "", "ok\n");
  }
}

int main(void) {
  stdio_init_all();
  MG_INFO(("Starting ..."));

  // Init LED
  gpio_init(LED);
  gpio_set_dir(LED, GPIO_OUT);

  // Init SPI pins
  uint r = spi_init(spi0, 500 * 1000);
  MG_INFO(("r = %x\n", r));
  gpio_set_function(SPI_RX, GPIO_FUNC_SPI);   // MISO
  gpio_set_function(SPI_TX, GPIO_FUNC_SPI);   // MOSI
  gpio_set_function(SPI_CLK, GPIO_FUNC_SPI);  // CLK
  gpio_init(SPI_CS);                          // CS
  gpio_set_dir(SPI_CS, GPIO_OUT);             // Set CS it to output
  gpio_put(SPI_CS, 1);                        // And drive CS high (inactive)

  // Init Mongoose
  struct mip_spi spi = {NULL, spi_begin, spi_end, spi_txn};
  struct mip_if mif = {.mac = {2, 0, 1, 2, 3, 5},
                       .driver = &mip_driver_w5500,
                       .driver_data = &spi};
  struct mg_mgr mgr;                                 // Declare event manager
  mg_mgr_init(&mgr);                                 // Init event manager
  mg_log_set(MG_LL_DEBUG);                           // Set DEBUG log level
  mip_init(&mgr, &mif);                              // Init TCP/IP stack
  mg_http_listen(&mgr, "http://0.0.0.0", fn, NULL);  // HTTP listener

  bool led_on = false;                         // Initial LED state
  uint64_t status_timer = 0, blink_timer = 0;  // Initial timer expirations

  // Infinite event manager loop
  for (;;) {
    if (mg_timer_expired(&blink_timer, BLINK_TIMER_MS, mg_millis())) {
      led_on = !led_on;                              // Flip LED state
      if (mip_driver_w5500.up(&mif)) led_on = true;  // Always on if Eth up
      gpio_put(LED, led_on);                         // Set LED
    }
    if (mg_timer_expired(&status_timer, STATUS_TIMER_MS, mg_millis())) {
      MG_INFO(("Ethernet: %s", mip_driver_w5500.up(&mif) ? "up" : "down"));
    }
    mg_mgr_poll(&mgr, 1);
  }

  return 0;
}
