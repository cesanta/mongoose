#pragma once

#include "arch.h"
#include "config.h"
#include "event.h"
#include "iobuf.h"
#include "net_builtin.h"
#include "str.h"
#include "timer.h"

// DNS server configuration used by struct mg_mgr.
struct mg_dns {
  const char *url;          // DNS server URL, e.g. "udp://8.8.8.8:53"
  struct mg_connection *c;  // Active DNS connection; NULL when idle
};

// Network address: IPv4 or IPv6 address plus port. All values in network byte order.
struct mg_addr {
  union {      // IP address bytes in network byte order
    uint8_t ip[16];   // Raw 16-byte buffer; IPv4 uses the first 4 bytes
    uint32_t ip4;     // IPv4 address alias (same storage as ip[0..3])
    uint64_t ip6[2];  // IPv6 address as two 64-bit words
  } addr;
  uint16_t port;     // TCP or UDP port in network byte order
  uint8_t scope_id;  // IPv6 scope ID (from the %N suffix in the address)
  bool is_ip6;       // True when this address holds an IPv6 address
};

union mg_pipe {
  MG_SOCKET_TYPE fd;
  void *q;
};

// Central event manager. Zero-initialise with mg_mgr_init() before use.
struct mg_mgr {
  struct mg_connection *conns;  // Linked list of all open connections
  struct mg_dns dns4;           // IPv4 DNS server (default: 8.8.8.8:53)
  struct mg_dns dns6;           // IPv6 DNS server (default: [2001:4860:4860::8888]:53)
  struct mg_connection *mdns;   // mDNS listener connection, or NULL
  int dnstimeout;               // DNS resolve timeout in ms (default: 3000)
  bool use_dns6;                // If true, prefer DNS6 for hostname resolution
  bool did_sync_time;           // True after time sync has been requested
  unsigned long nextid;         // Auto-incrementing counter for connection IDs
  void *userdata;               // Arbitrary user pointer
  void *tls_ctx;                // Shared TLS context for all TLS connections
  uint16_t mqtt_id;             // Packet ID counter for MQTT pub/sub
  void *active_dns_requests;    // Pending DNS queries (internal)
  void *active_mdns_requests;   // Pending mDNS resolver queries (internal)
  struct mg_timer *timers;      // Linked list of active timers
  int epoll_fd;                 // epoll file descriptor; -1 when unused (MG_EPOLL_ENABLE=1)
  struct mg_tcpip_if *ifp;      // Builtin TCP/IP stack: network interface pointer
  size_t extraconnsize;         // Builtin TCP/IP stack: extra bytes allocated per connection
  union mg_pipe pipe;           // Socketpair write-end / queue, used by mg_wakeup()
#if MG_ENABLE_FREERTOS_TCP
  SocketSet_t ss;               // FreeRTOS-TCP socket set
#endif
};

// A single network connection. Do not allocate directly; use mg_connect() or
// mg_listen(). Each connection has an event handler fn called for every event.
struct mg_connection {
  struct mg_connection *next;     // Next connection in mgr->conns linked list
  struct mg_mgr *mgr;             // Manager that owns this connection
  struct mg_addr loc;             // Local address (IP + port)
  struct mg_addr rem;             // Remote address (IP + port)
  void *fd;                       // OS socket handle or LWIP PCB pointer
  unsigned long id;               // Unique, auto-incrementing connection ID
  struct mg_iobuf recv;           // Received data; consume in MG_EV_READ handler
  struct mg_iobuf send;           // Data to send; append with mg_send() / mg_printf()
  struct mg_iobuf prof;           // Profiling data (MG_ENABLE_PROFILE only)
  struct mg_iobuf rtls;           // TLS only: encrypted data buffered before decryption
  mg_event_handler_t fn;          // User event handler
  void *fn_data;                  // User-supplied argument passed to fn
  mg_event_handler_t pfn;         // Protocol-level handler (set by protocol modules)
  void *pfn_data;                 // Protocol-level handler argument
  char data[MG_DATA_SIZE];        // Scratch space for protocol state; freely readable
  void *tls;                      // TLS state (internal)
  unsigned is_listening : 1;      // Listening connection; accepts inbound connections
  unsigned is_client : 1;         // Outbound connection created by mg_connect()
  unsigned is_accepted : 1;       // Inbound connection accepted from a listener
  unsigned is_resolving : 1;      // DNS resolution is in progress
  unsigned is_arplooking : 1;     // ARP resolution is in progress (builtin TCP/IP)
  unsigned is_connecting : 1;     // Non-blocking TCP connect is in progress
  unsigned is_tls : 1;            // TLS connection
  unsigned is_tls_hs : 1;         // TLS handshake is in progress
  unsigned is_udp : 1;            // UDP connection (not TCP)
  unsigned is_websocket : 1;      // WebSocket connection
  unsigned is_mqtt5 : 1;          // MQTT5 connection (set by mg_mqtt_login)
  unsigned is_hexdumping : 1;     // Log hex dump of all in/out traffic
  unsigned is_draining : 1;       // Flush send buffer, then close
  unsigned is_closing : 1;        // Close and free immediately on next poll
  unsigned is_full : 1;           // Pause incoming reads until cleared
  unsigned is_tls_throttled : 1;  // TLS write was throttled; retry pending
  unsigned is_resp : 1;           // HTTP: response is still being generated
  unsigned is_readable : 1;       // Socket is ready to read (epoll/select)
  unsigned is_writable : 1;       // Socket is ready to write (epoll/select)
};

// Runs one iteration of the event loop.
//
// Example:
//   while (keep_running) mg_mgr_poll(&mgr, 50);
// Full examples:
//   tutorials/http/http-server, tutorials/mqtt/mqtt-client,
//   tutorials/websocket/websocket-server, tutorials/core/timers
// Related APIs:
//   mg_mgr_init(), mg_mgr_free(), mg_timer_add(), mg_wakeup()
// Notes:
//   Waits up to ms milliseconds for I/O events; use ms=0 to return
//   immediately. Calls event handlers for ready connections and fires expired
//   timers. Call repeatedly from the main loop or a dedicated network task.
void mg_mgr_poll(struct mg_mgr *, int ms);

// Initialises an event manager before use.
//
// Example:
//   struct mg_mgr mgr;
//   mg_mgr_init(&mgr);
// Full examples:
//   tutorials/http/http-server, tutorials/mqtt/mqtt-client,
//   tutorials/websocket/websocket-server, tutorials/core/embedded-filesystem
// Related APIs:
//   mg_mgr_poll(), mg_mgr_free(), mg_http_listen(), mg_connect()
// Notes:
//   Sets safe defaults such as DNS servers, DNS timeout, epoll/SIGPIPE setup,
//   and TLS context. Call before creating connections, listeners, or timers.
//   Set mgr.userdata after this call if your application needs it.
//   On embedded systems, if the built-in TCP/IP stack is enabled
//   (MG_ENABLE_TCPIP=1) and one built-in driver is selected, e.g.
//   MG_ENABLE_DRIVER_STM32H=1, mg_mgr_init() also calls mg_tcpip_init().
//   If no driver is selected, automatic driver init is disabled, or multiple
//   interfaces are intended, call mg_tcpip_init() separately.
void mg_mgr_init(struct mg_mgr *);

// Closes all connections, frees all timers, and releases the TLS context.
// Safe to call on a partially initialised mgr.
void mg_mgr_free(struct mg_mgr *);

// Creates a listening connection on url (e.g. "tcp://0.0.0.0:8080").
// Fires MG_EV_OPEN on the listener, then MG_EV_ACCEPT for each new client.
// Returns NULL on error.
struct mg_connection *mg_listen(struct mg_mgr *, const char *url,
                                mg_event_handler_t fn, void *fn_data);

// Opens an outbound connection to url (e.g. "tcp://example.com:80").
// DNS resolution is asynchronous. Fires MG_EV_RESOLVE, then MG_EV_CONNECT
// when the connection is established (or MG_EV_ERROR on failure).
// Returns NULL on error (OOM, network down, null url).
struct mg_connection *mg_connect(struct mg_mgr *, const char *url,
                                 mg_event_handler_t fn, void *fn_data);

// Wraps an existing OS file descriptor in a Mongoose connection.
// The connection is added to mgr and fires MG_EV_OPEN immediately.
// Useful for integrating pre-opened sockets (e.g. stdin, accept() fd).
struct mg_connection *mg_wrapfd(struct mg_mgr *mgr, int fd,
                                mg_event_handler_t fn, void *fn_data);

// Called internally after DNS resolution completes to create the OS socket
// and initiate the TCP/IP connect. Normally not called by user code.
void mg_connect_resolved(struct mg_connection *);

// Appends buf/len to c->send. Returns true on success, false on OOM
// (which also fires MG_EV_ERROR and schedules the connection for close).
// Data is sent asynchronously by the next mg_mgr_poll() call.
bool mg_send(struct mg_connection *, const void *, size_t);

// Formats data and appends it to a connection send buffer.
//
// Returns:
//   Number of bytes appended, or 0 on OOM.
// Example:
//   mg_printf(c, "GET / HTTP/1.0\r\nHost: %.*s\r\n\r\n",
//             (int) host.len, host.buf);
// Full examples:
//   tutorials/http/http-client, tutorials/http/http-proxy-client,
//   tutorials/http/http-restful-server
// Related APIs:
//   mg_send(), mg_snprintf(), mg_http_reply(), mg_ws_printf()
// Notes:
//   Data is sent asynchronously by a later mg_mgr_poll() call. Supports
//   mg_xprintf specifiers, including custom %M/%m printers. See src/fmt.h for
//   the full specifier list.
size_t mg_printf(struct mg_connection *, const char *fmt, ...);
size_t mg_vprintf(struct mg_connection *, const char *fmt, va_list *ap);

// Parses an IP address string str into addr. Handles "localhost", plain IPv4
// (e.g. "1.2.3.4"), IPv6 with optional brackets and scope ID, and
// IPv4-mapped IPv6 (::ffff:1.2.3.4). Does NOT parse the port.
// Returns true on success; addr->is_ip6 and addr->addr are set.
bool mg_aton(struct mg_str str, struct mg_addr *addr);

// Low-level functions for integrating with custom network stacks.
// Allocates a new connection struct with mgr->extraconnsize extra bytes.
// Returns NULL on OOM.
struct mg_connection *mg_alloc_conn(struct mg_mgr *);

// Fires MG_EV_CLOSE, frees all buffers (recv/send/rtls), removes c from
// mgr->conns, and calls mg_free(c). Do not use c after this call.
void mg_close_conn(struct mg_connection *c);

// Creates and binds the OS socket for a listening connection c at url.
// Called internally by mg_listen(); expose for custom network stack use.
// Returns false on error.
bool mg_open_listener(struct mg_connection *c, const char *url);

// Thread-safe wakeup: fires MG_EV_WAKEUP on the connection whose id matches,
// with ev_data pointing to an mg_str containing buf/len.
// Requires mg_wakeup_init() to have been called first.
// Returns false if the pipe is not initialised or conn_id is 0.
// Safe to call from any thread or interrupt context.
bool mg_wakeup(struct mg_mgr *, unsigned long id, const void *buf, size_t len);

// Initialises the internal socketpair used by mg_wakeup(). Call once after
// mg_mgr_init() if you intend to wake the event loop from another thread.
// Returns false on failure (e.g. socketpair creation failed).
bool mg_wakeup_init(struct mg_mgr *);

// Deprecated API, do not expose. Use mg_timer_expired instead
struct mg_timer *mg_timer_add(struct mg_mgr *mgr, uint64_t milliseconds,
                              unsigned flags, void (*fn)(void *), void *arg);

// Like mg_connect() but also sets a protocol-level handler pfn/pfn_data.
// Used internally by protocol modules (MQTT, WebSocket, etc.).
struct mg_connection *mg_connect_svc(struct mg_mgr *mgr, const char *url,
                                     mg_event_handler_t fn, void *fn_data,
                                     mg_event_handler_t pfn, void *pfn_data);

// Restores c->rem from a saved address buffer written by the caller, then
// maps the IP back through the builtin TCP/IP stack. Used after a UDP
// multicast send to restore the connection's original remote address.
void mg_multicast_restore(struct mg_connection *c, uint8_t *from);
