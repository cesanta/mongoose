#pragma once

struct mg_tcpip_driver_atcmd_data {
  void *usart;                  // Opaque UART/SPI descriptor
  void (*reset)(void *);        // Modem hardware reset
  void (*tx)(void *, uint8_t);  // USART transmit single byte
  int (*rx)(void *);            // USART receive single byte
  const char **script;          // List of AT commands and expected replies
  int script_index;             // Index of the current AT command in the list
  uint64_t deadline;            // AT command deadline in ms
  bool no_byte_stuff;           // Do not perform byte stuffing/unstuffing
  bool link;                    // internal use: done with AT processing
  bool unstuffing;              // internal use: unstuffing state
};

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_ATCMD) && MG_ENABLE_DRIVER_ATCMD

extern struct mg_tcpip_driver mg_tcpip_driver_atcmd;

// Modem callbacks and AT script, defined by the application.
// See tutorials/arduino/sim800-mqtt for modem configuration.
extern struct mg_tcpip_driver_atcmd_data mg_atcmd;

#define MG_TCPIP_DRIVER_ATCMD_INIT(mgr)                          \
  do {                                                           \
    static struct mg_tcpip_if mif_;                               \
    mif_.ip = MG_TCPIP_IP;                                        \
    mif_.mask = MG_TCPIP_MASK;                                    \
    mif_.gw = MG_TCPIP_GW;                                        \
    mif_.driver = &mg_tcpip_driver_atcmd;                          \
    mif_.driver_data = &mg_atcmd;                                 \
    mif_.l2type = MG_TCPIP_L2_PPP;                                \
    MG_SET_MAC_ADDRESS(mif_.mac);                                 \
    mg_tcpip_init((mgr), &mif_);                                 \
    MG_INFO(("Driver: atcmd, MAC: %M", mg_print_mac, mif_.mac));    \
  } while (0)

#ifndef MG_TCPIP_DRIVER_INIT
#define MG_TCPIP_DRIVER_INIT(x) MG_TCPIP_DRIVER_ATCMD_INIT(x)
#endif

#endif
