#pragma once

// See https://mongoose.ws/documentation/#build-options
#define MG_ARCH MG_ARCH_PICOSDK
#define MG_OTA MG_OTA_PICOSDK
#define MG_TLS MG_TLS_BUILTIN
#define MG_IO_SIZE 2048

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_DRIVER_W5500 1

// For static IP configuration, define MG_TCPIP_{IP,MASK,GW}
// By default, those are set to zero, meaning that DHCP is used
//
// #define MG_TCPIP_IP MG_IPV4(192, 168, 1, 10)
// #define MG_TCPIP_GW MG_IPV4(192, 168, 1, 1)
// #define MG_TCPIP_MASK MG_IPV4(255, 255, 255, 0)

// Set custom MAC address. By default, it is randomly generated
// Using a build-time constant:
// #define MG_SET_MAC_ADDRESS(mac) do { uint8_t buf_[6] = {2,3,4,5,6,7};
// memmove(mac, buf_, sizeof(buf_)); } while (0)
//
// Using custom function:
extern void genmac(unsigned char *mac);
#define MG_SET_MAC_ADDRESS(mac) genmac(mac)

// Hardware specific SPI support
#include <stddef.h>
#include <stdint.h>

extern void spi_begin(void *);
extern void spi_end(void *);
extern void spi_txn(void *spi, uint8_t *tx, uint8_t *rx, size_t len);

#define MG_TCPIP_DRIVER_INIT(mgr)                                          \
  do {                                                                     \
    static struct mg_tcpip_spi spi_ = {NULL, spi_begin, spi_end, spi_txn}; \
    static struct mg_tcpip_if mif_;                                        \
    mif_.ip = MG_TCPIP_IP;                                                 \
    mif_.mask = MG_TCPIP_MASK;                                             \
    mif_.gw = MG_TCPIP_GW;                                                 \
    mif_.driver = &mg_tcpip_driver_w5500;                                  \
    mif_.driver_data = &spi_;                                              \
    MG_SET_MAC_ADDRESS(mif_.mac);                                          \
    mg_tcpip_init(mgr, &mif_);                                             \
    MG_INFO(("Driver: W5500, MAC: %M", mg_print_mac, mif_.mac));           \
  } while (0)
