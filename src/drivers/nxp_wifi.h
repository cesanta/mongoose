#pragma once

#if MG_ENABLE_TCPIP  && \
    defined(MG_ENABLE_DRIVER_NXP_WIFI) && MG_ENABLE_DRIVER_NXP_WIFI


struct mg_tcpip_driver_nxp_wifi_data {
  struct mg_wifi_data wifi;
};


#define MG_TCPIP_DRIVER_INIT(mgr)                                   \
  do {                                                              \
    static struct mg_tcpip_driver_nxp_wifi_data driver_data_;       \
    static struct mg_tcpip_if mif_;                                 \
    MG_SET_WIFI_CONFIG(&driver_data_);                              \
    mif_.ip = MG_TCPIP_IP;                                          \
    mif_.mask = MG_TCPIP_MASK;                                      \
    mif_.gw = MG_TCPIP_GW;                                          \
    mif_.driver = &mg_tcpip_driver_nxp_wifi;                        \
    mif_.driver_data = &driver_data_;                               \
    mif_.recv_queue.size = 8192;                                    \
    mif_.mac[0] = 2; /* MAC read from OTP at driver init */         \
    mg_tcpip_init(mgr, &mif_);                                      \
    MG_INFO(("Driver: nxp wifi, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif
