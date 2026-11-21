// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "mongoose.h"

#ifndef UART_DEBUG
#define UART_DEBUG USART2
#define UART_DEBUG_TX_PIN PIN('A', 2)
#define UART_DEBUG_RX_PIN PIN('A', 3)
#else
#define UART_DEBUG_TX_PIN PIN('A', 9)
#define UART_DEBUG_RX_PIN PIN('A', 10)
#endif

#define BLINK_PERIOD_MS 1000  // LED blinking period in millis
static struct spi spi_pins = {
    .miso = PIN('B', 4),
    .mosi = PIN('A', 0),
    .clk = PIN('A', 1),
    .cs = PIN('A', 4),
};

// SPI bus used by the W5500 driver, see MG_TCPIP_DRIVER_W5500_INIT
struct mg_tcpip_spi mg_spi = {
    .begin = (void (*)(void *)) spi_begin,
    .end = (void (*)(void *)) spi_end,
    .txn = (void (*)(void *, uint8_t *, uint8_t *, size_t)) spi_txn,
    .spi = &spi_pins,
};

static void log_fn(char ch, void *param) {
  uart_write_buf(param, &ch, 1);
}

bool mg_random(void *buf, size_t len) {  // Use on-board RNG
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r = rng_read();
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
  return true;
}

uint64_t mg_millis(void) {
  return hal_get_tick();
}

static void timer_fn(void *arg) {
  gpio_toggle(LED);               // Blink LED
  struct mg_tcpip_if *ifp = arg;  // And show
  MG_INFO(("Ethernet: %d, IP: %M, rx:%u, tx:%u, dr:%u, er:%u", ifp->state,
           mg_print_ip4, &ifp->ip, ifp->nrecv, ifp->nsent, ifp->ndrop,
           ifp->nerr));
}

static void fn(struct mg_connection *c, int ev, void *ev_data) {
  struct mg_tcpip_if *ifp = (struct mg_tcpip_if *) c->fn_data;
  if (ev == MG_EV_HTTP_HDRS) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/ota"), NULL)) {
      mg_http_stream_body(c, hm, mg_http_ota_cb, NULL);
    }
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/api/hello"),
                 NULL)) {  // Request to /api/hello
      mg_http_reply(c, 200, "", "{%m:%u,%m:%u,%m:%u,%m:%u,%m:%u}\n",
                    MG_ESC("eth"), ifp->state, MG_ESC("frames_received"),
                    ifp->nrecv, MG_ESC("frames_sent"), ifp->nsent,
                    MG_ESC("frames_dropped"), ifp->ndrop,
                    MG_ESC("interface_errors"), ifp->nerr);
    } else if (mg_match(hm->uri, mg_str("/"), NULL)) {  // Index page
      mg_http_reply(
          c, 200, "", "%s",
          "<html><head><link rel='icon' href='data:;base64,='></head><body>"
          "<h1>Welcome to Mongoose</h1>"
          "See <a href=/api/hello>/api/hello</a> for REST example"
          "</body></html>");
    } else {  // All other URIs
      mg_http_reply(c, 404, "", "Not Found\n");
    }
  }
}

int main(void) {
  gpio_output(LED);               // Setup green LED
  uart_init(UART_DEBUG, UART_DEBUG_TX_PIN, UART_DEBUG_RX_PIN, 115200);
  mg_log_set_fn(log_fn, UART_DEBUG);

  // ethernet_init();                // Initialise ethernet pins
  MG_INFO(("Starting, CPU freq %g MHz", (double) SystemCoreClock / 1000000));

  spi_init(&spi_pins);      // SPI must be ready before mg_mgr_init()
  struct mg_mgr mgr;        // Initialise
  mg_log_set(MG_LL_DEBUG);  // Set log level
  mg_mgr_init(&mgr);        // Mongoose event manager and W5500 driver

  MG_INFO(("Waiting for IP..."));
  while (mgr.ifp->state != MG_TCPIP_STATE_READY) {
    mg_mgr_poll(&mgr, 0);
  }

  MG_INFO(("Initialising application..."));
  mg_timer_add(&mgr, BLINK_PERIOD_MS, MG_TIMER_REPEAT, timer_fn, mgr.ifp);
  mg_http_listen(&mgr, "http://0.0.0.0:80", fn, mgr.ifp);

  MG_INFO(("Starting event loop"));
  for (;;) {
    mg_mgr_poll(&mgr, 0);
  }

  return 0;
}
