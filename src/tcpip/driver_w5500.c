#include "tcpip.h"

#if MG_ENABLE_TCPIP

enum { W5500_CR = 0, W5500_S0 = 1, W5500_TX0 = 2, W5500_RX0 = 3 };

static void w5500_txn(struct mg_tcpip_spi *s, uint8_t block, uint16_t addr, bool wr,
                      void *buf, size_t len) {
  uint8_t *p = (uint8_t *) buf;
  uint8_t cmd[] = {(uint8_t) (addr >> 8), (uint8_t) (addr & 255),
                   (uint8_t) ((block << 3) | (wr ? 4 : 0))};
  s->begin(s->spi);
  for (size_t i = 0; i < sizeof(cmd); i++) s->txn(s->spi, cmd[i]);
  for (size_t i = 0; i < len; i++) {
    uint8_t r = s->txn(s->spi, p[i]);
    if (!wr) p[i] = r;
  }
  s->end(s->spi);
}

// clang-format off
static  void w5500_wn(struct mg_tcpip_spi *s, uint8_t block, uint16_t addr, void *buf, size_t len) { w5500_txn(s, block, addr, true, buf, len); }
static  void w5500_w1(struct mg_tcpip_spi *s, uint8_t block, uint16_t addr, uint8_t val) { w5500_wn(s, block, addr, &val, 1); }
static  void w5500_w2(struct mg_tcpip_spi *s, uint8_t block, uint16_t addr, uint16_t val) { uint8_t buf[2] = {(uint8_t) (val >> 8), (uint8_t) (val & 255)}; w5500_wn(s, block, addr, buf, sizeof(buf)); }
static  void w5500_rn(struct mg_tcpip_spi *s, uint8_t block, uint16_t addr, void *buf, size_t len) { w5500_txn(s, block, addr, false, buf, len); }
static  uint8_t w5500_r1(struct mg_tcpip_spi *s, uint8_t block, uint16_t addr) { uint8_t r = 0; w5500_rn(s, block, addr, &r, 1); return r; }
static  uint16_t w5500_r2(struct mg_tcpip_spi *s, uint8_t block, uint16_t addr) { uint8_t buf[2] = {0, 0}; w5500_rn(s, block, addr, buf, sizeof(buf)); return (uint16_t) ((buf[0] << 8) | buf[1]); }
// clang-format on

static size_t w5500_rx(void *buf, size_t buflen, struct mg_tcpip_if *ifp) {
  struct mg_tcpip_spi *s = (struct mg_tcpip_spi *) ifp->driver_data;
  uint16_t r = 0, n = 0, len = (uint16_t) buflen, n2;     // Read recv len
  while ((n2 = w5500_r2(s, W5500_S0, 0x26)) > n) n = n2;  // Until it is stable
  // printf("RSR: %d\n", (int) n);
  if (n > 0) {
    uint16_t ptr = w5500_r2(s, W5500_S0, 0x28);  // Get read pointer
    n = w5500_r2(s, W5500_RX0, ptr);             // Read frame length
    if (n <= len + 2 && n > 1) {
      r = (uint16_t) (n - 2);
      w5500_rn(s, W5500_RX0, (uint16_t) (ptr + 2), buf, r);
    }
    w5500_w2(s, W5500_S0, 0x28, (uint16_t) (ptr + n));  // Advance read pointer
    w5500_w1(s, W5500_S0, 1, 0x40);                     // Sock0 CR -> RECV
    // printf("  RX_RD: tot=%u n=%u r=%u\n", n2, n, r);
  }
  return r;
}

static size_t w5500_tx(const void *buf, size_t buflen, struct mg_tcpip_if *ifp) {
  struct mg_tcpip_spi *s = (struct mg_tcpip_spi *) ifp->driver_data;
  uint16_t n = 0, len = (uint16_t) buflen;
  while (n < len) n = w5500_r2(s, W5500_S0, 0x20);      // Wait for space
  uint16_t ptr = w5500_r2(s, W5500_S0, 0x24);           // Get write pointer
  w5500_wn(s, W5500_TX0, ptr, (void *) buf, len);       // Write data
  w5500_w2(s, W5500_S0, 0x24, (uint16_t) (ptr + len));  // Advance write pointer
  w5500_w1(s, W5500_S0, 1, 0x20);                       // Sock0 CR -> SEND
  for (int i = 0; i < 40; i++) {
    uint8_t ir = w5500_r1(s, W5500_S0, 2);  // Read S0 IR
    if (ir == 0) continue;
    // printf("IR %d, len=%d, free=%d, ptr %d\n", ir, (int) len, (int) n, ptr);
    w5500_w1(s, W5500_S0, 2, ir);  // Write S0 IR: clear it!
    if (ir & 8) len = 0;           // Timeout. Report error
    if (ir & (16 | 8)) break;      // Stop on SEND_OK or timeout
  }
  return len;
}

static bool w5500_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_spi *s = (struct mg_tcpip_spi *) ifp->driver_data;
  s->end(s->spi);
  w5500_w1(s, W5500_CR, 0, 0x80);     // Reset chip: CR -> 0x80
  w5500_w1(s, W5500_CR, 0x2e, 0);     // CR PHYCFGR -> reset
  w5500_w1(s, W5500_CR, 0x2e, 0xf8);  // CR PHYCFGR -> set
  // w5500_wn(s, W5500_CR, 9, s->mac, 6);      // Set source MAC
  w5500_w1(s, W5500_S0, 0x1e, 16);          // Sock0 RX buf size
  w5500_w1(s, W5500_S0, 0x1f, 16);          // Sock0 TX buf size
  w5500_w1(s, W5500_S0, 0, 4);              // Sock0 MR -> MACRAW
  w5500_w1(s, W5500_S0, 1, 1);              // Sock0 CR -> OPEN
  return w5500_r1(s, W5500_S0, 3) == 0x42;  // Sock0 SR == MACRAW
}

static bool w5500_up(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_spi *spi = (struct mg_tcpip_spi *) ifp->driver_data;
  uint8_t phycfgr = w5500_r1(spi, W5500_CR, 0x2e);
  return phycfgr & 1;  // Bit 0 of PHYCFGR is LNK (0 - down, 1 - up)
}

struct mg_tcpip_driver mg_tcpip_driver_w5500 = {w5500_init, w5500_tx, w5500_rx, w5500_up};
#endif
