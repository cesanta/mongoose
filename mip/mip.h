#pragma once

#include "arch.h"
#include "net.h"

struct mip_driver {
  bool (*init)(uint8_t *mac, void *data);           // Initialise driver
  size_t (*tx)(const void *, size_t, void *data);   // Transmit frame
  size_t (*rx)(void *buf, size_t len, void *data);  // Receive frame (polling)
  bool (*up)(void *data);                           // Up/down status
  // Set receive callback for interrupt-driven drivers
  void (*setrx)(void (*fn)(void *buf, size_t len, void *rxdata), void *rxdata);
};

struct mip_cfg {
  uint8_t mac[6];         // MAC address. Must not be 0
  uint32_t ip, mask, gw;  // IP, netmask, GW. If IP is 0, DHCP is used
};

void mip_init(struct mg_mgr *, struct mip_cfg *, struct mip_driver *, void *);

extern struct mip_driver mip_driver_stm32;
extern struct mip_driver mip_driver_enc28j60;
extern struct mip_driver mip_driver_w5500;

// Drivers that require SPI, can use this SPI abstraction
struct mip_spi {
  void *spi;                        // Opaque SPI bus descriptor
  void (*begin)(void *);            // SPI begin: slave select low
  void (*end)(void *);              // SPI end: slave select high
  uint8_t (*txn)(void *, uint8_t);  // SPI transaction: write 1 byte, read reply
};

#ifdef MIP_QPROFILE
enum {QP_IRQTRIGGERED=0, QP_FRAMEPUSHED, QP_FRAMEPOPPED, QP_FRAMEDONE, QP_FRAMEDROPPED, QP_QUEUEOVF};

void qp_mark(unsigned int type, int len);
void qp_log(void);
void qp_init(void);
#endif
