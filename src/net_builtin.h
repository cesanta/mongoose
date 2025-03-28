#pragma once

#include "arch.h"
#include "net.h"
#include "queue.h"
#include "str.h"

struct mg_tcpip_if;  // Mongoose TCP/IP network interface

struct mg_tcpip_driver {
  bool (*init)(struct mg_tcpip_if *);                         // Init driver
  size_t (*tx)(const void *, size_t, struct mg_tcpip_if *);   // Transmit frame
  size_t (*rx)(void *buf, size_t len, struct mg_tcpip_if *);  // Receive frame
  bool (*poll)(struct mg_tcpip_if *, bool);  // Poll, return Up/down status
};

typedef void (*mg_tcpip_event_handler_t)(struct mg_tcpip_if *ifp, int ev,
                                         void *ev_data);

enum {
  MG_TCPIP_EV_ST_CHG,           // state change                   uint8_t * (&ifp->state)
  MG_TCPIP_EV_DHCP_DNS,         // DHCP DNS assignment            uint32_t *ipaddr
  MG_TCPIP_EV_DHCP_SNTP,        // DHCP SNTP assignment           uint32_t *ipaddr
  MG_TCPIP_EV_ARP,              // Got ARP packet                 struct mg_str *
  MG_TCPIP_EV_TIMER_1S,         // 1 second timer                 NULL
  MG_TCPIP_EV_WIFI_SCAN_RESULT, // Wi-Fi scan results             struct mg_wifi_scan_bss_data *
  MG_TCPIP_EV_WIFI_SCAN_END,    // Wi-Fi scan has finished        NULL
  MG_TCPIP_EV_WIFI_CONNECT_ERR, // Wi-Fi connect has failed       driver and chip specific
  MG_TCPIP_EV_DRIVER,           // Driver event                   driver specific
  MG_TCPIP_EV_USER              // Starting ID for user events
};


// Network interface
struct mg_tcpip_if {
  uint8_t mac[6];                  // MAC address. Must be set to a valid MAC
  uint32_t ip, mask, gw;           // IP address, mask, default gateway
  struct mg_str tx;                // Output (TX) buffer
  bool enable_dhcp_client;         // Enable DCHP client
  bool enable_dhcp_server;         // Enable DCHP server
  bool enable_get_gateway;         // DCHP server sets client as gateway
  bool enable_req_dns;             // DCHP client requests DNS server
  bool enable_req_sntp;            // DCHP client requests SNTP server
  bool enable_crc32_check;         // Do a CRC check on RX frames and strip it
  bool enable_mac_check;           // Do a MAC check on RX frames
  struct mg_tcpip_driver *driver;  // Low level driver
  void *driver_data;               // Driver-specific data
  mg_tcpip_event_handler_t fn;     // User-specified event handler function
  struct mg_mgr *mgr;              // Mongoose event manager
  struct mg_queue recv_queue;      // Receive queue
  char dhcp_name[12];              // Name reported to DHCP, "mip" if unset
  uint16_t mtu;                    // Interface MTU
#define MG_TCPIP_MTU_DEFAULT 1500

  // Internal state, user can use it but should not change it
  uint8_t gwmac[6];             // Router's MAC
  uint64_t now;                 // Current time
  uint64_t timer_1000ms;        // 1000 ms timer: for DHCP and link state
  uint64_t lease_expire;        // Lease expiration time, in ms
  uint16_t eport;               // Next ephemeral port
  volatile uint32_t ndrop;      // Number of received, but dropped frames
  volatile uint32_t nrecv;      // Number of received frames
  volatile uint32_t nsent;      // Number of transmitted frames
  volatile uint32_t nerr;       // Number of driver errors
  uint8_t state;                // Current state
#define MG_TCPIP_STATE_DOWN 0   // Interface is down
#define MG_TCPIP_STATE_UP 1     // Interface is up
#define MG_TCPIP_STATE_REQ 2    // Interface is up, DHCP REQUESTING state
#define MG_TCPIP_STATE_IP 3     // Interface is up and has an IP assigned
#define MG_TCPIP_STATE_READY 4  // Interface has fully come up, ready to work
};

void mg_tcpip_init(struct mg_mgr *, struct mg_tcpip_if *);
void mg_tcpip_free(struct mg_tcpip_if *);
void mg_tcpip_qwrite(void *buf, size_t len, struct mg_tcpip_if *ifp);
void mg_tcpip_arp_request(struct mg_tcpip_if *ifp, uint32_t ip, uint8_t *mac);

extern struct mg_tcpip_driver mg_tcpip_driver_stm32f;
extern struct mg_tcpip_driver mg_tcpip_driver_w5500;
extern struct mg_tcpip_driver mg_tcpip_driver_w5100;
extern struct mg_tcpip_driver mg_tcpip_driver_tm4c;
extern struct mg_tcpip_driver mg_tcpip_driver_tms570;
extern struct mg_tcpip_driver mg_tcpip_driver_stm32h;
extern struct mg_tcpip_driver mg_tcpip_driver_imxrt;
extern struct mg_tcpip_driver mg_tcpip_driver_same54;
extern struct mg_tcpip_driver mg_tcpip_driver_cmsis;
extern struct mg_tcpip_driver mg_tcpip_driver_ra;
extern struct mg_tcpip_driver mg_tcpip_driver_xmc;
extern struct mg_tcpip_driver mg_tcpip_driver_xmc7;
extern struct mg_tcpip_driver mg_tcpip_driver_ppp;
extern struct mg_tcpip_driver mg_tcpip_driver_pico_w;
extern struct mg_tcpip_driver mg_tcpip_driver_rw612;
extern struct mg_tcpip_driver mg_tcpip_driver_cyw;

// Drivers that require SPI, can use this SPI abstraction
struct mg_tcpip_spi {
  void *spi;                        // Opaque SPI bus descriptor
  void (*begin)(void *);            // SPI begin: slave select low
  void (*end)(void *);              // SPI end: slave select high
  uint8_t (*txn)(void *, uint8_t);  // SPI transaction: write 1 byte, read reply
};
