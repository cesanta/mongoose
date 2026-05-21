// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "driver_rp2040_rmii.h"
#include "mongoose.h"

#define CLKREFPIN 20  // either 20 or 22

static void driver_init(struct mg_mgr *mgr) {
  MG_INFO(("Init TCP/IP"));
  // Initialise Mongoose network stack and specific driver
  // Set consecutive GPIOs for RMII (tx and rx) and SMI function groups
  struct mg_tcpip_driver_rp2040_rmii_data driver_data = {
      // see driver_rp2040_rmii.h
      .rx0 = 6,      // 6, 7, 8     : RX0, RX1, CRS_DV
      .tx0 = 10,     // 10, 11, 12  : TX0, TX1, TX-EN
      .mdio = 14,    // 14, 15      : MDIO, MDC
      .phy_addr = 1  // check your hardware, LAN8722 is 0 or 1
  };
  // Specify MAC address, either set use_dhcp or enter a static config.
  // For static configuration, specify IP/mask/GW in network byte order
  struct mg_tcpip_if mif = {
      .mac = {2, 0, 1, 2, 3, 5},
      .ip = 0,
      .enable_mac_check = true,
      .enable_fcs_check = true,
      .driver = &mg_tcpip_driver_rp2040_rmii,
      .driver_data = &driver_data,
  };
  mg_tcpip_init(mgr, &mif);
}

static void http_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {  // New HTTP request received
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    uint64_t tick = to_ms_since_boot(get_absolute_time());
    if (mg_match(hm->uri, mg_str("/api/tick"), NULL)) {
      mg_http_reply(c, 200, "", "{%m:%llu}\n", MG_ESC("tick"), tick);
    } else {
      mg_http_reply(c, 200, "", "Hi from Mongoose, tick %llu\n", tick);
    }
  }
}

// In an RTOS environment, give this task 8 KB of stack space.
static void run_mongoose(void) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  driver_init(&mgr);
  mg_log_set(MG_LL_DEBUG);  // Set log level
  MG_INFO(("Hardware initialised, starting firmware..."));
  mg_http_listen(&mgr, "http://0.0.0.0", http_ev_handler, NULL);
  for (;;) {
    mg_mgr_poll(&mgr, 1);
  }
}

int main(void) {
  // use the RMII reference clock (50MHz) as our system clock
  clock_configure_gpin(clk_sys, CLKREFPIN, 50 * MHZ, 50 * MHZ);
  sleep_ms(100);
  stdio_init_all();
  run_mongoose();
  return 0;
}
