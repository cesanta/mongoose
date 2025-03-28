#pragma once

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_CYW) && MG_ENABLE_DRIVER_CYW

struct mg_tcpip_spi_ {
  void *spi;              // Opaque SPI bus descriptor
  void (*begin)(void *);  // SPI begin: slave select low
  void (*end)(void *);    // SPI end: slave select high
  void (*txn)(void *, uint8_t *, uint8_t *,
              size_t len);  // SPI transaction: write-read len bytes
};

struct mg_tcpip_driver_cyw_firmware {
  const uint8_t *code_addr;
  size_t code_len;
  const uint8_t *nvram_addr;
  size_t nvram_len;
  const uint8_t *clm_addr;
  size_t clm_len;
};

struct mg_tcpip_driver_cyw_data {
  struct mg_tcpip_spi_ *spi;
  struct mg_tcpip_driver_cyw_firmware *fw;
  char *ssid;
  char *pass;
  char *apssid;
  char *appass;
  uint8_t security; // TBD
  uint8_t apsecurity; // TBD
  uint8_t apchannel;
  bool apmode;      // start in AP mode; 'false' starts connection to 'ssid' if not NULL
  bool hs;          // use chip "high-speed" mode; otherwise SPI CPOL0 CPHA0 (DS 4.2.3 Table 6)
};

//#define MG_TCPIP_DRIVER_INIT(mgr)                                 \
//  do {                                                            \
//    static struct mg_tcpip_driver_cyw_data driver_data_;          \
//    static struct mg_tcpip_if mif_;                               \
//    MG_SET_WIFI_CONFIG(&driver_data_);                            \
//    mif_.ip = MG_TCPIP_IP;                                        \
//    mif_.mask = MG_TCPIP_MASK;                                    \
//    mif_.gw = MG_TCPIP_GW;                                        \
//    mif_.driver = &mg_tcpip_driver_pico_w;                        \
//    mif_.driver_data = &driver_data_;                             \
//    mif_.recv_queue.size = 8192;                                  \
//    mif_.mac[0] = 2; /* MAC read from OTP at driver init */       \
//    mg_tcpip_init(mgr, &mif_);                                    \
//    MG_INFO(("Driver: cyw, MAC: %M", mg_print_mac, mif_.mac)); \
//  } while (0)

#endif
