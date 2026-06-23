#pragma once

#include "arch.h"
#include "l2.h"
#include "net.h"
#include "queue.h"
#include "str.h"

#if MG_ENABLE_TCPIP

// Mongoose TCP/IP network interface
struct mg_tcpip_if;

// Hardware driver interface. Implement all function pointers to plug in a
// new network device. Polling drivers set rx; interrupt/DMA drivers set
// rx=NULL and push frames via mg_tcpip_qwrite() from their ISR instead.
struct mg_tcpip_driver {
  bool (*init)(struct mg_tcpip_if *);                        // One-time hardware init; return false on failure
  size_t (*tx)(const void *, size_t, struct mg_tcpip_if *);  // Transmit one raw frame; return bytes sent
  size_t (*rx)(void *buf, size_t len, struct mg_tcpip_if *); // Read one raw frame; return frame size, 0 if none. NULL for interrupt-driven drivers
  bool (*poll)(struct mg_tcpip_if *, bool);                  // Check link status; second arg is true once per second. Return true=up, false=down
};

// Event handler called by the TCP/IP stack for network-level events.
// ev is one of MG_TCPIP_EV_*; ev_data type is documented per event below.
typedef void (*mg_tcpip_event_handler_t)(struct mg_tcpip_if *ifp, int ev,
                                         void *ev_data);

// clang-format off
// Events delivered to mg_tcpip_if.fn (user) and mg_tcpip_if.pfn (driver).
// Each entry shows the ev_data type to cast inside the handler.
enum {
  MG_TCPIP_EV_STATE_CHANGE,      // IPv4 link state changed       uint8_t *(&ifp->state)
  MG_TCPIP_EV_DHCP_DNS,          // DHCP assigned a DNS server    uint32_t *ipaddr
  MG_TCPIP_EV_DHCP_SNTP,         // DHCP assigned an SNTP server  uint32_t *ipaddr
  MG_TCPIP_EV_ARP,               // Raw ARP frame received        struct mg_str *
  MG_TCPIP_EV_TIMER_1S,          // 1-second periodic tick        NULL
  MG_TCPIP_EV_WIFI_SCAN_RESULT,  // Wi-Fi BSS scan entry          struct mg_wifi_scan_bss_data *
  MG_TCPIP_EV_WIFI_SCAN_END,     // Wi-Fi scan finished           NULL
  MG_TCPIP_EV_WIFI_CONNECT_ERR,  // Wi-Fi connection failed       driver and chip specific
  MG_TCPIP_EV_DRIVER,            // Driver-defined event          driver specific
  MG_TCPIP_EV_STATE6_CHANGE,     // IPv6 link state changed       uint8_t *(&ifp->state6)
  MG_TCPIP_EV_USER               // Starting ID for user-defined events
};
// clang-format on

// Network interface. Zero-initialise, then set the fields marked "user-set"
// before calling mg_tcpip_init(). Fields in the "internal state" section
// below are managed by the stack; read them freely but do not modify them.
struct mg_tcpip_if {
  // User-set configuration
  uint8_t mac[sizeof(struct mg_l2addr)];  // MAC address; must be set to a valid address
  uint32_t ip, mask, gw;                  // Static IPv4 address, netmask, gateway; 0 = use DHCP
  struct mg_str tx;                       // TX frame buffer (managed by the stack)
  bool enable_dhcp_client;                // Enable DHCP client; auto-set if ip==0
  bool enable_dhcp_server;                // Enable DHCP server
  bool enable_get_gateway;                // DHCP server offers itself as the default gateway
  bool enable_req_dns;                    // DHCP client requests a DNS server address
  bool enable_req_sntp;                   // DHCP client requests an SNTP server address
  bool enable_fcs_check;                  // Verify and strip FCS from received frames
  bool enable_mac_check;                  // Drop frames not addressed to this MAC
  bool update_mac_hash_table;             // Signal driver to refresh MAC multicast hash table
  bool is_ip_changed;                     // Set by stack when IP changes; triggers connection restart
  struct mg_tcpip_driver *driver;         // Hardware driver; must be set before mg_tcpip_init()
  void *driver_data;                      // Passed to all driver functions as ifp->driver_data
  mg_tcpip_event_handler_t pfn;           // Driver-level event handler; called before fn
  mg_tcpip_event_handler_t fn;            // User event handler for MG_TCPIP_EV_* events
  struct mg_mgr *mgr;                     // Mongoose event manager; set by mg_tcpip_init()
  struct mg_queue recv_queue;             // Incoming frame queue; pre-size by setting recv_queue.size
  char dhcp_name[MG_TCPIP_DHCPNAME_SIZE]; // Hostname sent in DHCP requests; defaults to "mip"
  uint16_t mtu;                           // IP MTU (max payload size at the IP layer)
  uint16_t framesize;                     // Maximum L2 frame size in bytes
  uint16_t l2mtu;                 	  // L2 frame payload, default net MTU

#if MG_ENABLE_IPV6
  uint64_t ip6ll[2], ip6[2];  // IPv6 link-local and global addresses
  uint8_t prefix[8];          // Global prefix bytes
  uint8_t prefix_len;         // Global prefix length in bits
  uint64_t gw6[2];            // IPv6 default gateway
  bool enable_slaac;          // Enable SLAAC (stateless address autoconfiguration)
  bool enable_dhcp6_client;   // Enable DHCPv6 client (not yet fully implemented)
  bool is_ip6_changed;        // Set by stack when IPv6 address changes
#endif

  // Internal state - read-only for users, managed by the stack
  uint8_t gwmac[sizeof(struct mg_l2addr)];  // Gateway hardware address (resolved via ARP)
  enum mg_l2type l2type;                    // Layer-2 type: Ethernet, PPP, etc. (see l2.h)
  union mg_l2data l2data;                   // Layer-2 config and state (see l2.h)
  char *dns4_url;                           // Active DNS server URL (set from DHCP or config)
  uint64_t now;                             // Current time in ms (updated each mg_mgr_poll)
  uint64_t timer_1000ms;                    // Timestamp of last 1-second tick
  uint64_t lease_expire;                    // DHCP lease expiry time in ms
  uint16_t eport;                           // Next ephemeral port to assign
  volatile uint32_t ndrop;                  // Frames received but dropped (queue full)
  volatile uint32_t nrecv;                  // Total frames received
  volatile uint32_t nsent;                  // Total frames transmitted
  volatile uint32_t nerr;                   // Driver-reported transmit errors
  uint8_t state;                            // Current IPv4 link/IP state (MG_TCPIP_STATE_*)
#define MG_TCPIP_STATE_DOWN 0       // Physical link is down
#define MG_TCPIP_STATE_LINK_UP 1    // Driver reports physical link is up
#define MG_TCPIP_STATE_UP 2         // L2 is ready (e.g. PPP negotiated)
#define MG_TCPIP_STATE_REQ 3        // DHCP REQUESTING: waiting for DHCP offer
#define MG_TCPIP_STATE_IP 4         // IP address assigned; resolving gateway MAC
#define MG_TCPIP_STATE_READY 5      // Fully operational; gateway MAC resolved
  bool gw_ready;                    // Gateway MAC address has been resolved via ARP

#if MG_ENABLE_IPV6
  uint8_t gw6mac[sizeof(struct mg_l2addr)];  // IPv6 gateway hardware address
  uint8_t state6;                            // Current IPv6 link/IP state (MG_TCPIP_STATE_*)
  bool gw6_ready;                            // IPv6 gateway MAC address has been resolved
#endif
};

// Initialises the TCP/IP stack. Calls driver->init(), allocates the TX frame
// buffer (ifp->framesize bytes) and the RX queue. If ifp->ip == 0 and the
// L2 type is not PPP, DHCP client is enabled automatically.
// Must be called once after populating ifp->driver, ifp->mac, and any
// static IP/mask/gw values.
void mg_tcpip_init(struct mg_mgr *, struct mg_tcpip_if *);

// Frees the TX buffer and RX queue allocated by mg_tcpip_init().
void mg_tcpip_free(struct mg_tcpip_if *);

// Pushes a received raw frame into ifp->recv_queue. Used by interrupt-driven
// or DMA-based drivers (those with rx=NULL) to hand frames to the stack from
// an ISR or DMA completion callback. Drops the frame and increments ndrop if
// the queue is full.
void mg_tcpip_qwrite(void *buf, size_t len, struct mg_tcpip_if *ifp);

// Sends a broadcast ARP who-has request for ip. mac may be NULL for a plain
// broadcast, or a non-NULL hint to set the target MAC field in the request.
void mg_tcpip_arp_request(struct mg_tcpip_if *ifp, uint32_t ip, uint8_t *mac);

// Built-in hardware drivers. Assign one to mg_tcpip_if.driver before calling
// mg_tcpip_init(). Each driver also requires a corresponding driver_data
// struct; see the matching driver header for details.
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
extern struct mg_tcpip_driver mg_tcpip_driver_pico_w;
extern struct mg_tcpip_driver mg_tcpip_driver_rw612;
extern struct mg_tcpip_driver mg_tcpip_driver_cyw;
extern struct mg_tcpip_driver mg_tcpip_driver_nxp_wifi;
extern struct mg_tcpip_driver mg_tcpip_driver_st67w6;
extern struct mg_tcpip_driver mg_tcpip_driver_atcmd;

// SPI bus abstraction for SPI-attached network chips (e.g. W5500).
// Populate and assign to the driver's driver_data field.
struct mg_tcpip_spi {
  void *spi;                                                   // Opaque SPI bus handle passed to all callbacks
  void (*begin)(void *);                                       // Assert chip-select (start of transaction)
  void (*end)(void *);                                         // Deassert chip-select (end of transaction)
  void (*txn)(void *, uint8_t *write, uint8_t *read, size_t); // Full-duplex transfer: write and read len bytes simultaneously
};

// Alignment attributes for DMA-capable Ethernet descriptor buffers.
// Use these on driver RX/TX descriptor arrays where the hardware requires
// specific alignment.
#ifndef MG_8BYTE_ALIGNED
#if defined(__GNUC__)
#define MG_8BYTE_ALIGNED __attribute__((aligned((8U))))
#else
#define MG_8BYTE_ALIGNED
#endif  // compiler
#endif  // 8BYTE_ALIGNED

#ifndef MG_16BYTE_ALIGNED
#if defined(__GNUC__)
#define MG_16BYTE_ALIGNED __attribute__((aligned((16U))))
#else
#define MG_16BYTE_ALIGNED
#endif  // compiler
#endif  // 16BYTE_ALIGNED

#ifndef MG_32BYTE_ALIGNED
#if defined(__GNUC__)
#define MG_32BYTE_ALIGNED __attribute__((aligned((32U))))
#else
#define MG_32BYTE_ALIGNED
#endif  // compiler
#endif  // 32BYTE_ALIGNED

#ifndef MG_64BYTE_ALIGNED
#if defined(__GNUC__)
#define MG_64BYTE_ALIGNED __attribute__((aligned((64U))))
#else
#define MG_64BYTE_ALIGNED
#endif  // compiler
#endif  // 64BYTE_ALIGNED

// Memory section attribute for Ethernet frame buffers. Define to
// __attribute__((section(".eth_ram"))) or similar if the Ethernet DMA
// requires buffers in a specific RAM region.
#ifndef MG_ETH_RAM
#define MG_ETH_RAM
#endif

#endif
