// Copyright (c) 2025 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"
#include "hal.h"


static const struct mg_tcpip_spi spi = {NULL, hwspecific_spi_begin, hwspecific_spi_end, hwspecific_spi_txn};


int main(void) {
  // initialize stdio
  stdio_init_all();

  hwspecific_spi_init();

  struct mg_mgr mgr;        // Initialise Mongoose event manager
  mg_mgr_init(&mgr);        // and attach it to the interface
  mg_log_set(MG_LL_DEBUG);  // Set log level

  // Initialise Mongoose network stack
  // Either set use_dhcp or enter a static config.
  // For static configuration, specify IP/mask/GW in network byte order
  struct mg_tcpip_if mif = {
      .ip = 0,
      .driver = &mg_tcpip_driver_w5500,
      .driver_data = (void *) &spi
  };

  hal_ethernet_genmac(mif.mac);
  mg_tcpip_init(&mgr, &mif);
  MG_INFO(("Init done, starting main loop"));

  MG_INFO(("Initialising application..."));

  MG_INFO(("Starting event loop"));
  for (;;) {
    mg_mgr_poll(&mgr, 0);
  }

  return 0;
}

