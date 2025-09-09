#pragma once

#if MG_ENABLE_TCPIP && MG_ARCH == MG_ARCH_PICOSDK && \
    defined(MG_ENABLE_DRIVER_PICO_W) && MG_ENABLE_DRIVER_PICO_W

#include "cyw43.h"              // keep this include
#include "pico/cyw43_arch.h"    // keep this include
#include "pico/unique_id.h"     // keep this include

struct mg_tcpip_driver_pico_w_data {
  struct mg_wifi_data wifi;
};

#define MG_TCPIP_DRIVER_INIT(mgr)                                 \
  do {                                                            \
    static struct mg_tcpip_driver_pico_w_data driver_data_;       \
    static struct mg_tcpip_if mif_;                               \
    MG_SET_WIFI_CONFIG(&driver_data_);                            \
    mif_.ip = MG_TCPIP_IP;                                        \
    mif_.mask = MG_TCPIP_MASK;                                    \
    mif_.gw = MG_TCPIP_GW;                                        \
    mif_.driver = &mg_tcpip_driver_pico_w;                        \
    mif_.driver_data = &driver_data_;                             \
    mif_.recv_queue.size = 8192;                                  \
    mif_.mac[0] = 2; /* MAC read from OTP at driver init */       \
    mg_tcpip_init(mgr, &mif_);                                    \
    MG_INFO(("Driver: pico-w, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif
