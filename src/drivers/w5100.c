#include "net_builtin.h"

#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_W5100) && MG_ENABLE_DRIVER_W5100

static void w5100_txn(struct mg_tcpip_spi *s, uint16_t addr, bool wr, void *buf,
                      size_t len) {
  size_t i;
  uint8_t *p = (uint8_t *) buf;
  uint8_t control = wr ? 0xF0 : 0x0F;
  uint8_t cmd[] = {control, (uint8_t) (addr >> 8), (uint8_t) (addr & 255)};
  s->begin(s->spi);
  for (i = 0; i < sizeof(cmd); i++) s->txn(s->spi, cmd[i]);
  for (i = 0; i < len; i++) {
    uint8_t r = s->txn(s->spi, p[i]);
    if (!wr) p[i] = r;
  }
  s->end(s->spi);
}

// clang-format off
static  void w5100_wn(struct mg_tcpip_spi *s, uint16_t addr, void *buf, size_t len) { w5100_txn(s, addr, true, buf, len); }
static  void w5100_w1(struct mg_tcpip_spi *s, uint16_t addr, uint8_t val) { w5100_wn(s, addr, &val, 1); }
static  void w5100_w2(struct mg_tcpip_spi *s, uint16_t addr, uint16_t val) { uint8_t buf[2] = {(uint8_t) (val >> 8), (uint8_t) (val & 255)}; w5100_wn(s, addr, buf, sizeof(buf)); }
static  void w5100_rn(struct mg_tcpip_spi *s, uint16_t addr, void *buf, size_t len) { w5100_txn(s, addr, false, buf, len); }
static  uint8_t w5100_r1(struct mg_tcpip_spi *s, uint16_t addr) { uint8_t r = 0; w5100_rn(s, addr, &r, 1); return r; }
static  uint16_t w5100_r2(struct mg_tcpip_spi *s, uint16_t addr) { uint8_t buf[2] = {0, 0}; w5100_rn(s, addr, buf, sizeof(buf)); return (uint16_t) ((buf[0] << 8) | buf[1]); }
// clang-format on

static size_t w5100_rx(void *buf, size_t buflen, struct mg_tcpip_if *ifp) {
  struct mg_tcpip_spi *s = (struct mg_tcpip_spi *) ifp->driver_data;
  uint16_t r = 0, n = 0, len = (uint16_t) buflen, n2;  // Read recv len
  while ((n2 = w5100_r2(s, 0x426)) > n) n = n2;        // Until it is stable
  if (n > 0) {
    uint16_t ptr = w5100_r2(s, 0x428);  // Get read pointer
    if (n <= len + 2 && n > 1) {
      r = (uint16_t) (n - 2);
    }
    uint16_t rxbuf_size = (1 << (w5100_r1(s, 0x1a) & 3)) * 1024;
    uint16_t rxbuf_addr = 0x6000;
    uint16_t ptr_ofs = (ptr + 2) & (rxbuf_size - 1);
    if (ptr_ofs + r < rxbuf_size) {
      w5100_rn(s, rxbuf_addr + ptr_ofs, buf, r);
    } else {
      uint16_t remaining_len = rxbuf_size - ptr_ofs;
      w5100_rn(s, rxbuf_addr + ptr_ofs, buf, remaining_len);
      w5100_rn(s, rxbuf_addr, buf + remaining_len, n - remaining_len);
    }
    w5100_w2(s, 0x428, (uint16_t) (ptr + n));
    w5100_w1(s, 0x401, 0x40);  // Sock0 CR -> RECV
  }
  return r;
}

static size_t w5100_tx(const void *buf, size_t buflen,
                       struct mg_tcpip_if *ifp) {
  struct mg_tcpip_spi *s = (struct mg_tcpip_spi *) ifp->driver_data;
  uint16_t i, n = 0, ptr = 0, len = (uint16_t) buflen;
  while (n < len) n = w5100_r2(s, 0x420);  // Wait for space
  ptr = w5100_r2(s, 0x424);                // Get write pointer
  uint16_t txbuf_size = (1 << (w5100_r1(s, 0x1b) & 3)) * 1024;
  uint16_t ptr_ofs = ptr & (txbuf_size - 1);
  uint16_t txbuf_addr = 0x4000;
  if (ptr_ofs + len > txbuf_size) {
    uint16_t size = txbuf_size - ptr_ofs;
    w5100_wn(s, txbuf_addr + ptr_ofs, (char *) buf, size);
    w5100_wn(s, txbuf_addr, (char *) buf + size, len - size);
  } else {
    w5100_wn(s, txbuf_addr + ptr_ofs, (char *) buf, len);
  }
  w5100_w2(s, 0x424, (uint16_t) (ptr + len));  // Advance write pointer
  w5100_w1(s, 0x401, 0x20);                    // Sock0 CR -> SEND
  for (i = 0; i < 40; i++) {
    uint8_t ir = w5100_r1(s, 0x402);  // Read S0 IR
    if (ir == 0) continue;
    // printf("IR %d, len=%d, free=%d, ptr %d\n", ir, (int) len, (int) n, ptr);
    w5100_w1(s, 0x402, ir);    // Write S0 IR: clear it!
    if (ir & 8) len = 0;       // Timeout. Report error
    if (ir & (16 | 8)) break;  // Stop on SEND_OK or timeout
  }
  return len;
}

static bool w5100_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_spi *s = (struct mg_tcpip_spi *) ifp->driver_data;
  s->end(s->spi);
  w5100_w1(s, 0, 0x80);               // Reset chip: CR -> 0x80
  w5100_w1(s, 0x72, 0x53);            // CR PHYLCKR -> unlock PHY
  w5100_w1(s, 0x46, 0);               // CR PHYCR0 -> autonegotiation
  w5100_w1(s, 0x47, 0);               // CR PHYCR1 -> reset
  w5100_w1(s, 0x72, 0x00);            // CR PHYLCKR -> lock PHY
  w5100_wn(s, 0x09, ifp->mac, 6);     // SHAR
  w5100_w1(s, 0x1a, 6);               // Sock0 RX buf size - 4KB
  w5100_w1(s, 0x1b, 6);               // Sock0 TX buf size - 4KB
  w5100_w1(s, 0x400, 0x44);           // Sock0 MR -> MACRAW, MAC filter
  w5100_w1(s, 0x401, 1);              // Sock0 CR -> OPEN
  return w5100_r1(s, 0x403) == 0x42;  // Sock0 SR == MACRAW
}

static bool w5100_poll(struct mg_tcpip_if *ifp, bool s1) {
  struct mg_tcpip_spi *spi = (struct mg_tcpip_spi *) ifp->driver_data;
  return s1 ? w5100_r1(spi, 0x3c /* PHYSR */) & 1
            : false;  // Bit 0 of PHYSR is LNK (0 - down, 1 - up)
}

struct mg_tcpip_driver mg_tcpip_driver_w5100 = {w5100_init, w5100_tx, w5100_rx,
                                                w5100_poll};
#endif
