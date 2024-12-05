#pragma once

#if MG_ENABLE_TCPIP && MG_ARCH == MG_ARCH_PICOSDK && \
    defined(MG_ENABLE_DRIVER_PICO_W) && MG_ENABLE_DRIVER_PICO_W

#include "cyw43.h"              // keep this include
#include "pico/cyw43_arch.h"    // keep this include
#include "pico/unique_id.h"     // keep this include

struct mg_tcpip_driver_pico_w_data {
  char *ssid;
  char *pass;
};

#endif
