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
