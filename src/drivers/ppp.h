#pragma once

struct mg_tcpip_driver_ppp_data {
  void *uart;                   // Opaque UART bus descriptor
  void (*reset)(void *);        // Modem hardware reset
  void (*tx)(void *, uint8_t);  // UART transmit single byte
  int (*rx)(void *);            // UART receive single byte
  const char **script;          // List of AT commands and expected replies
  int script_index;             // Index of the current AT command in the list
  uint64_t deadline;            // AT command deadline in ms
};
