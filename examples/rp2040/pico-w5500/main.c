// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#include <stdio.h>
#include <string.h>
#include "hardware/spi.h"
#include "pico/rand.h"
#include "pico/stdlib.h"
#include "pico/unique_id.h"

#include "mongoose.h"
#include "net.h"

enum { BLINK_PERIOD_MS = 1000 };
enum { LED = 25, SPI_CS = 17, SPI_CLK = 18, SPI_TX = 19, SPI_RX = 16 };  // Pins

static void spi_begin(void *spi) {
  gpio_put(SPI_CS, 0);
}
static void spi_end(void *spi) {
  gpio_put(SPI_CS, 1);
}
static uint8_t spi_txn(void *spi, uint8_t byte) {
  uint8_t result = 0;
  spi_write_read_blocking(spi0, &byte, &result, 1);
  return result;
}

void mg_random(void *buf, size_t len) {
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r = get_rand_32();
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
}

static void timer_fn(void *arg) {
  gpio_put(PICO_DEFAULT_LED_PIN,
           !gpio_get_out_level(PICO_DEFAULT_LED_PIN));   // Blink LED
  struct mg_tcpip_if *ifp = arg;                         // And show
  const char *names[] = {"down", "up", "req", "ready"};  // network stats
  MG_INFO(("Ethernet: %s, IP: %M, rx:%u, tx:%u, dr:%u, er:%u",
           names[ifp->state], mg_print_ip4, &ifp->ip, ifp->nrecv, ifp->nsent,
           ifp->ndrop, ifp->nerr));
}

static void eth_spi_init(void) {
  // Init SPI pins
  spi_init(spi0, 500 * 1000);
  gpio_set_function(SPI_RX, GPIO_FUNC_SPI);   // MISO
  gpio_set_function(SPI_TX, GPIO_FUNC_SPI);   // MOSI
  gpio_set_function(SPI_CLK, GPIO_FUNC_SPI);  // CLK
  gpio_init(SPI_CS);                          // CS
  gpio_set_dir(SPI_CS, GPIO_OUT);             // Set CS to output
  gpio_put(SPI_CS, 1);                        // And drive CS high (inactive)
}

// Helper macro for MAC generation
#define GENERATE_LOCALLY_ADMINISTERED_MAC(id) \
  { 2, id[3], id[4], id[5], id[6], id[7] }

int main(void) {
  stdio_init_all();
  gpio_init(LED);  // Setup board LED
  gpio_set_dir(LED, GPIO_OUT);
  eth_spi_init();  // Initialise SPI pins
  MG_INFO(("Starting ..."));

  pico_unique_board_id_t board_id;
  pico_get_unique_board_id(&board_id);
  uint8_t *id = board_id.id;

  struct mg_mgr mgr;        // Initialise
  mg_mgr_init(&mgr);        // Mongoose event manager
  mg_log_set(MG_LL_DEBUG);  // Set log level

  // Initialise Mongoose network stack
  struct mg_tcpip_spi spi = {NULL, spi_begin, spi_end, spi_txn};
  struct mg_tcpip_if mif = {.mac = GENERATE_LOCALLY_ADMINISTERED_MAC(id),
                            // Uncomment below for static configuration:
                            // .ip = mg_htonl(MG_U32(192, 168, 0, 223)),
                            // .mask = mg_htonl(MG_U32(255, 255, 255, 0)),
                            // .gw = mg_htonl(MG_U32(192, 168, 0, 1)),
                          .driver = &mg_tcpip_driver_w5500,
                            .driver_data = &spi};
  mg_tcpip_init(&mgr, &mif);
  mg_timer_add(&mgr, BLINK_PERIOD_MS, MG_TIMER_REPEAT, timer_fn, &mif);

  MG_INFO(("MAC: %M. Waiting for IP...", mg_print_mac, mif.mac));
  while (mif.state != MG_TCPIP_STATE_READY) {
    mg_mgr_poll(&mgr, 0);
  }

  MG_INFO(("Initialising application..."));
  web_init(&mgr);

  MG_INFO(("Starting event loop"));
  for (;;) {
    mg_mgr_poll(&mgr, 1);
  }

  return 0;
}
