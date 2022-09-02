// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved
//
// This software is dual-licensed: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License version 3 as
// published by the Free Software Foundation. For the terms of this
// license, see http://www.fsf.org/licensing/licenses/agpl-3.0.html
//
// You are free to use this software under the terms of the GNU General
// Public License, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// Alternatively, you can license this software under a commercial
// license, please contact us at https://cesanta.com/contact.html

#pragma once

struct w5500 {
  uint8_t mac[6];                   // MAC address
  void *spi;                        // Opaque SPI bus descriptor
  uint8_t (*txn)(void *, uint8_t);  // SPI transaction
  void (*begin)(void *);            // SPI begin
  void (*end)(void *);              // SPI end
};

enum { W5500_CR = 0, W5500_S0 = 1, W5500_TX0 = 2, W5500_RX0 = 3 };

static inline void w5500_txn(struct w5500 *w, uint8_t block, uint16_t addr,
                             bool wr, void *buf, size_t len) {
  uint8_t *p = buf, cmd[] = {(uint8_t) (addr >> 8), (uint8_t) (addr & 255),
                             (uint8_t) ((block << 3) | (wr ? 4 : 0))};
  w->begin(w->spi);
  for (size_t i = 0; i < sizeof(cmd); i++) w->txn(w->spi, cmd[i]);
  for (size_t i = 0; i < len; i++) {
    uint8_t r = w->txn(w->spi, p[i]);
    if (!wr) p[i] = r;
  }
  w->end(w->spi);
}

// clang-format off
static inline void w5500_wn(struct w5500 *w, uint8_t block, uint16_t addr, void *buf, size_t len) { w5500_txn(w, block, addr, true, buf, len); }
static inline void w5500_w1(struct w5500 *w, uint8_t block, uint16_t addr, uint8_t val) { w5500_wn(w, block, addr, &val, 1); }
static inline void w5500_w2(struct w5500 *w, uint8_t block, uint16_t addr, uint16_t val) { uint8_t buf[2] = {(uint8_t) (val >> 8), (uint8_t) (val & 255)}; w5500_wn(w, block, addr, buf, sizeof(buf)); }
static inline void w5500_rn(struct w5500 *w, uint8_t block, uint16_t addr, void *buf, size_t len) { w5500_txn(w, block, addr, false, buf, len); }
static inline uint8_t w5500_r1(struct w5500 *w, uint8_t block, uint16_t addr) { uint8_t r = 0; w5500_rn(w, block, addr, &r, 1); return r; }
static inline uint16_t w5500_r2(struct w5500 *w, uint8_t block, uint16_t addr) { uint8_t buf[2] = {0, 0}; w5500_rn(w, block, addr, buf, sizeof(buf)); return (uint16_t) ((buf[0] << 8) | buf[1]); }
// clang-format on

static inline uint8_t w5500_status(struct w5500 *w) {
  return w5500_r1(w, W5500_CR, 0x2e);
}

static inline uint16_t w5500_rx(struct w5500 *w, uint8_t *buf, uint16_t len) {
  uint16_t r = 0, n = 0, n2;                              // Read recv len
  while ((n2 = w5500_r2(w, W5500_S0, 0x26)) > n) n = n2;  // Until it is stable
  // printf("RSR: %d\n", (int) n);
  if (n > 0) {
    uint16_t ptr = w5500_r2(w, W5500_S0, 0x28);  // Get read pointer
    n = w5500_r2(w, W5500_RX0, ptr);             // Read frame length
    if (n <= len + 2) r = n - 2, w5500_rn(w, W5500_RX0, ptr + 2, buf, r);
    w5500_w2(w, W5500_S0, 0x28, ptr + n);  // Advance read pointer
    w5500_w1(w, W5500_S0, 1, 0x40);        // Sock0 CR -> RECV
    // printf("  RX_RD: tot=%u n=%u r=%u\n", n2, n, r);
  }
  return r;
}

static inline uint16_t w5500_tx(struct w5500 *w, void *buf, uint16_t len) {
  uint16_t n = 0;
  while (n < len) n = w5500_r2(w, W5500_S0, 0x20);  // Wait for space
  uint16_t ptr = w5500_r2(w, W5500_S0, 0x24);       // Get write pointer
  w5500_wn(w, W5500_TX0, ptr, buf, len);            // Write data
  w5500_w2(w, W5500_S0, 0x24, ptr + len);           // Advance write pointer
  w5500_w1(w, W5500_S0, 1, 0x20);                   // Sock0 CR -> SEND
  for (int i = 0; i < 40; i++) {
    uint8_t ir = w5500_r1(w, W5500_S0, 2);  // Read S0 IR
    if (ir == 0) continue;
    // printf("IR %d, len=%d, free=%d, ptr %d\n", ir, (int) len, (int) n, ptr);
    w5500_w1(w, W5500_S0, 2, ir);  // Write S0 IR: clear it!
    if (ir & 8) len = 0;           // Timeout. Report error
    if (ir & (16 | 8)) break;      // Stop on SEND_OK or timeout
  }
  return len;
}

static inline bool w5500_init(struct w5500 *w) {
  w->end(w->spi);
  w5500_w1(w, W5500_CR, 0, 0x80);     // Reset chip: CR -> 0x80
  w5500_w1(w, W5500_CR, 0x2e, 0);     // CR PHYCFGR -> reset
  w5500_w1(w, W5500_CR, 0x2e, 0xf8);  // CR PHYCFGR -> set
  // w5500_wn(w, W5500_CR, 9, w->mac, 6);      // Set source MAC
  w5500_w1(w, W5500_S0, 0x1e, 16);          // Sock0 RX buf size
  w5500_w1(w, W5500_S0, 0x1f, 16);          // Sock0 TX buf size
  w5500_w1(w, W5500_S0, 0, 4);              // Sock0 MR -> MACRAW
  w5500_w1(w, W5500_S0, 1, 1);              // Sock0 CR -> OPEN
  return w5500_r1(w, W5500_S0, 3) == 0x42;  // Sock0 SR == MACRAW
}
