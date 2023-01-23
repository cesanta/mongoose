// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#include <stdio.h>
#include <string.h>
#include "hardware/spi.h"
#include "pico/stdlib.h"

#include "mongoose.h"

enum { BLINK_PERIOD_MS = 1000 };
enum { LED = 25, SPI_CS = 17, SPI_CLK = 18, SPI_TX = 19, SPI_RX = 16 };  // Pins

static void spi_begin(void *spi) { gpio_put(SPI_CS, 0); }
static void spi_end(void *spi) { gpio_put(SPI_CS, 1); }
static uint8_t spi_txn(void *spi, uint8_t byte) {
  uint8_t result = 0;
  spi_write_read_blocking(spi0, &byte, &result, 1);
  return result;
}

static void timer_cb(void *arg) {
  gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get_out_level(PICO_DEFAULT_LED_PIN));
  bool up = ((struct mip_if *) arg)->state == MIP_STATE_READY;
  MG_INFO(("Ethernet: %s", up ? "up" : "down"));  // Show network status
}

int main(void) {
  stdio_init_all();
  MG_INFO(("Starting ..."));

  // Init LED
  gpio_init(LED);
  gpio_set_dir(LED, GPIO_OUT);

  // Init SPI pins
  spi_init(spi0, 500 * 1000);
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
  struct mg_mgr mgr;        // Declare event manager
  mg_mgr_init(&mgr);        // Init event manager
  mg_log_set(MG_LL_DEBUG);  // Set DEBUG log level
  mip_init(&mgr, &mif);     // Init TCP/IP stack
  mg_timer_add(&mgr, BLINK_PERIOD_MS, MG_TIMER_REPEAT, timer_cb, &mif);

  MG_INFO(("Waiting until network is up..."));
  while (mif.state != MIP_STATE_READY) {
    mg_mgr_poll(&mgr, 0);
  }

  MG_INFO(("Initialising application..."));
  extern void device_dashboard_fn(struct mg_connection *, int, void *, void *);
  mg_http_listen(&mgr, "http://0.0.0.0", device_dashboard_fn, &mgr);

  MG_INFO(("Starting event loop"));
  for (;;) {
    mg_mgr_poll(&mgr, 1);
  }

  return 0;
}
