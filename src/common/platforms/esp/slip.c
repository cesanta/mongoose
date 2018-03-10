#include "rom_functions.h"

void SLIP_send(uint8_t *pkt, uint32_t size) {
  send_packet(pkt, size);
}

uint32_t SLIP_recv(void *pkt, uint32_t max_len) {
  uint8_t c;
  uint32_t len = 0;
  uint8_t *p = (uint8_t *) pkt;
  do {
    c = uart_rx_one_char_block();
  } while (c != '\xc0');
  while (len < max_len) {
    c = uart_rx_one_char_block();
    if (c == '\xc0') return len;
    if (c == '\xdb') {
      c = uart_rx_one_char_block();
      if (c == '\xdc') {
        c = '\xc0';
      } else if (c == '\xdd') {
        c = '\xdb';
      } else {
        len = 0;
        break; /* Bad esc sequence. */
      }
    }
    *p++ = c;
    len++;
  }
  do {
    c = uart_rx_one_char_block();
  } while (c != '\xc0');
  return len;
}
