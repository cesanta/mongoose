#pragma once

struct mg_tcpip_driver_pico_w_data {
  char *ssid;
  char *pass;
};

extern struct mg_tcpip_driver mg_tcpip_driver_pico_w;

void driver_pico_w_poll(void);
