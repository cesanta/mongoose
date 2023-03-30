// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "pico/stdlib.h"
#include "hardware/clocks.h"

#include "mongoose.h"

#include "driver_rp2040_rmii.h"

#define CLKREFPIN 20  // either 20 or 22

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    mg_http_reply(c, 200, "", "ok\n");
  }
}


int main(void) {
  // use the RMII reference clock (50MHz) as our system clock
  clock_configure_gpin(clk_sys, CLKREFPIN, 50 * MHZ, 50 * MHZ);
  sleep_ms(100);
  stdio_init_all();

  struct mg_mgr mgr;        // Initialise Mongoose event manager
  mg_mgr_init(&mgr);        // and attach it to the interface
  mg_log_set(MG_LL_DEBUG);  // Set log level

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
      .enable_crc32_check = true,
      .driver = &mg_tcpip_driver_rp2040_rmii,
      .driver_data = &driver_data,
  };
  mg_tcpip_init(&mgr, &mif);
  mg_http_listen(&mgr, "http://0.0.0.0", fn, NULL);  // HTTP listener
  MG_INFO(("Init done, starting main loop"));

  for (;;) mg_mgr_poll(&mgr, 0);  // Infinite event loop

  return 0;
}
