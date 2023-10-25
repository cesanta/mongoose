// Copyright (c) 2022-2023 Cesanta Software Limited
// All rights reserved

#include "hal.h"
#include "mongoose.h"
#include "net.h"

#define BLINK_PERIOD_MS 1000  // LED blinking period in millis

static volatile uint64_t s_ticks;  // Milliseconds since boot
void SysTick_Handler(void) {       // SyStick IRQ handler, triggered every 1ms
  s_ticks++;
}

uint64_t mg_millis(void) {  // Let Mongoose use our uptime function
  return s_ticks;           // Return number of milliseconds since boot
}

void mg_random(void *buf, size_t len) {  // Use on-board RNG
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r = rng_read();
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
}
static void timer_fn(void *arg) {
  gpio_toggle(LED);                                      // Blink LED
  struct mg_tcpip_if *ifp = arg;                         // And show
  const char *names[] = {"down", "up", "req", "ready"};  // network stats
  MG_INFO(("Ethernet: %s, IP: %M, rx:%u, tx:%u, dr:%u, er:%u",
           names[ifp->state], mg_print_ip4, &ifp->ip, ifp->nrecv, ifp->nsent,
           ifp->ndrop, ifp->nerr));
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  struct mg_tcpip_if *ifp = (struct mg_tcpip_if *) fn_data;
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/api/hello")) {  // Request to /api/hello
      mg_http_reply(c, 200, "", "{%m:%u,%m:%u,%m:%u,%m:%u,%m:%u}\n",
                    MG_ESC("eth"), ifp->state, MG_ESC("frames_received"),
                    ifp->nrecv, MG_ESC("frames_sent"), ifp->nsent,
                    MG_ESC("frames_dropped"), ifp->ndrop,
                    MG_ESC("interface_errors"), ifp->nerr);
    } else if (mg_http_match_uri(hm, "/")) {  // Index page
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
  gpio_output(LED);               // Setup blue LED
  uart_init(UART_DEBUG, 115200);  // Initialise debug printf
  ethernet_init();                // Initialise ethernet pins
  MG_INFO(("Starting, CPU freq %g MHz", (double) SystemCoreClock / 1000000));

  struct mg_mgr mgr;        // Initialise
  mg_mgr_init(&mgr);        // Mongoose event manager
  mg_log_set(MG_LL_DEBUG);  // Set log level

  // Initialise Mongoose network stack
  struct mg_tcpip_driver_rt1020_data driver_data = {.mdc_cr = 24};
  struct mg_tcpip_if mif = {.mac = GENERATE_LOCALLY_ADMINISTERED_MAC(),
                            // Uncomment below for static configuration:
                            // .ip = mg_htonl(MG_U32(192, 168, 0, 223)),
                            // .mask = mg_htonl(MG_U32(255, 255, 255, 0)),
                            // .gw = mg_htonl(MG_U32(192, 168, 0, 1)),
                            .driver = &mg_tcpip_driver_rt1020,
                            .driver_data = &driver_data};
  mg_tcpip_init(&mgr, &mif);
  mg_timer_add(&mgr, BLINK_PERIOD_MS, MG_TIMER_REPEAT, timer_fn, &mif);

  MG_INFO(("MAC: %M. Waiting for IP...", mg_print_mac, mif.mac));
  while (mif.state != MG_TCPIP_STATE_READY) {
    mg_mgr_poll(&mgr, 0);
  }

  MG_INFO(("Initialising application..."));
  mg_http_listen(&mgr, "http://0.0.0.0:80", fn, &mif);

  MG_INFO(("Starting event loop"));
  for (;;) {
    mg_mgr_poll(&mgr, 0);
  }

  return 0;
}
