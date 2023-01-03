#pragma once

#include "arch.h"
#include "net.h"

struct mip_if;  // MIP network interface

struct mip_driver {
  bool (*init)(struct mip_if *);                         // Initialise driver
  size_t (*tx)(const void *, size_t, struct mip_if *);   // Transmit frame
  size_t (*rx)(void *buf, size_t len, struct mip_if *);  // Receive frame (poll)
  bool (*up)(struct mip_if *);                           // Up/down status
};

// Receive queue - single producer, single consumer queue.  Interrupt-based
// drivers copy received frames to the queue in interrupt context. mip_poll()
// function runs in event loop context, reads from the queue
struct queue {
  uint8_t *buf;
  size_t len;
  volatile size_t tail, head;
};

#define MIP_ARP_ENTRIES 5  // Number of ARP cache entries. Maximum 21
#define MIP_ARP_CS (2 + 12 * MIP_ARP_ENTRIES)  // ARP cache size

// Network interface
struct mip_if {
  uint8_t mac[6];             // MAC address. Must be set to a valid MAC
  uint32_t ip, mask, gw;      // IP address, mask, default gateway
  struct mg_str rx;           // Output (TX) buffer
  struct mg_str tx;           // Input (RX) buffer
  bool enable_dhcp_client;    // Enable DCHP client
  bool enable_dhcp_server;    // Enable DCHP server
  struct mip_driver *driver;  // Low level driver
  void *driver_data;          // Driver-specific data
  struct mg_mgr *mgr;         // Mongoose event manager
  struct queue queue;         // Set queue.len for interrupt based drivers

  // Internal state, user can use it but should not change it
  uint64_t now;                   // Current time
  uint64_t timer_1000ms;          // 1000 ms timer: for DHCP and link state
  uint64_t lease_expire;          // Lease expiration time
  uint8_t arp_cache[MIP_ARP_CS];  // Each entry is 12 bytes
  uint16_t eport;                 // Next ephemeral port
  uint16_t dropped;               // Number of dropped frames
  uint8_t state;                  // Current state
#define MIP_STATE_DOWN 0          // Interface is down
#define MIP_STATE_UP 1            // Interface is up
#define MIP_STATE_READY 2         // Interface is up and has IP
};

void mip_init(struct mg_mgr *, struct mip_if *);
void mip_free(struct mip_if *);
void mip_qwrite(void *buf, size_t len, struct mip_if *ifp);
size_t mip_qread(void *buf, struct mip_if *ifp);
// conveniency rx function for IRQ-driven drivers
size_t mip_driver_rx(void *buf, size_t len, struct mip_if *ifp);

extern struct mip_driver mip_driver_stm32;
extern struct mip_driver mip_driver_w5500;
extern struct mip_driver mip_driver_tm4c;

// Drivers that require SPI, can use this SPI abstraction
struct mip_spi {
  void *spi;                        // Opaque SPI bus descriptor
  void (*begin)(void *);            // SPI begin: slave select low
  void (*end)(void *);              // SPI end: slave select high
  uint8_t (*txn)(void *, uint8_t);  // SPI transaction: write 1 byte, read reply
};

#ifdef MIP_QPROFILE
enum {
  QP_IRQTRIGGERED = 0,  // payload is number of interrupts so far
  QP_FRAMEPUSHED,       // available space in the frame queue
  QP_FRAMEPOPPED,       // available space in the frame queue
  QP_FRAMEDONE,         // available space in the frame queue
  QP_FRAMEDROPPED,      // number of dropped frames
  QP_QUEUEOVF  // profiling queue is full, payload is number of frame drops
};

void qp_mark(unsigned int type, int len);
void qp_log(void);  // timestamp, type, payload
void qp_init(void);
#else
#define qp_mark(a, b)
#endif
