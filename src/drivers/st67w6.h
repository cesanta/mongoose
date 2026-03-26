#pragma once

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_ST67W6) && \
    MG_ENABLE_DRIVER_ST67W6

struct mg_tcpip_driver_st67w6_data {
  struct mg_wifi_data wifi;
  void *spi;
  bool (*is_ready)(void);     // return state of module RDY pin
  struct mg_queue send_queue; // decouple tx calls from module polls
};

#define MG_TCPIP_DRIVER_INIT(mgr)                                 \
  do {                                                            \
    static struct mg_tcpip_driver_st67w6_data driver_data_;       \
    static struct mg_tcpip_if mif_;                               \
    MG_SET_WIFI_CONFIG(&driver_data_);                            \
    mif_.ip = MG_TCPIP_IP;                                        \
    mif_.mask = MG_TCPIP_MASK;                                    \
    mif_.gw = MG_TCPIP_GW;                                        \
    mif_.driver = &mg_tcpip_driver_st67w6;                        \
    mif_.driver_data = &driver_data_;                             \
    mif_.recv_queue.size = 8192;                                  \
    mif_.mac[0] = 2; /* MAC read from OTP at driver init */       \
    mg_tcpip_init(mgr, &mif_);                                    \
    MG_INFO(("Driver: st67w6, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif
