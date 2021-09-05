# User Guide


## Introduction

Mongoose is a networking library for C/C++. It implements an event-driven,
non-blocking APIs for TCP, UDP, HTTP, WebSocket, MQTT.  It has been designed
for connecting devices and bringing them online. On the market since 2004, used
by a vast number of open source and commercial products - it even runs on the
International Space Station! Mongoose makes embedded network programming fast,
robust, and easy.

Mongoose has two basic data structures:

- `struct mg_mgr` - an event manager that holds all active connections
- `struct mg_connection` - a single connection descriptor

Connections could be either listening, outbound or inbound. Outbound
connections are created by the `mg_connect()` call. Listening connections are
created by the `mg_listen()` call. Inbound connections are those accepted by a
listening connection. Each connection is described by a `struct mg_connection`
structure, which has a number of fields. All fields are exposed to the
application by design, to give an application a full visibility into the
Mongoose's internals.

An application that uses mongoose should follow a standard pattern of
event-driven application:

**Step 1.** Declare and initialize an event manager:

  ```c
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  ```

**Step 2.** Create connections. For example, a server application should create listening
  connections. When any connection is created (listening or outgoing), an
  event handler function must be specified. An event handler function defines
  connection's behavior.

  ```c
  struct mg_connection *c = mg_http_listen(&mgr, "0.0.0.0:8000", fn, arg);
  ```

**Step 3.** Create an event loop by calling `mg_mgr_poll()`:

  ```c
   for (;;) {
     mg_mgr_poll(&mgr, 1000);
   }
  ```

`mg_mgr_poll()` iterates over all sockets, accepts new connections, sends and
receives data, closes connections and calls event handler functions for the
respective events. 

Since the Mongoose's core is not protected against concurrent accesses,
make sure that all `mg_*` API functions are called from the same thread
or RTOS task.

## Send and receive buffers


Each connection has a send and receive buffer:
- `struct mg_connection::send` - data to be sent to a peer
- `struct mg_connection::recv` - data received from a peer

When data arrives, Mongoose appends received data to the `recv` and triggers an
`MG_EV_READ` event. The user may send data back by calling one of the output
functions, like `mg_send()` or `mg_printf()`. Output functions append data to
the `send` buffer.  When Mongoose successfully writes data to the socket, it
discards data from struct `mg_connection::send` and sends an `MG_EV_SEND`
event.

## Event handler function

Each connection has an event handler function associated with it. That function
must be implemented by the user. Event handler is the key element of the
Mongoose application, since it defines the connection's behaviour. This is
what an event handler function looks like:

```c
// Event handler function defines connection's behavior
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_READ) {
    mg_send(c, c->recv.buf, c->recv.len);   // Implement echo server
    c->recv.len = 0;                        // Delete received data
  }
}
```

- `struct mg_connection *c` - a connection that received an event
- `int ev` - an event number, defined in mongoose.h. For example, when data
  arrives on an inbound connection, ev would be `MG_EV_READ`
- `void *ev_data` - points to the event-specific data, and it has a different
  meaning for different events. For example, for an `MG_EV_READ` event,
  `ev_data`
  is an `int *` pointing to the number of bytes received from a remote
  peer and saved into the `c->recv` IO buffer. The exact meaning of `ev_data` is
  described for each event. Protocol-specific events usually have `ev_data`
  pointing to structures that hold protocol-specific information
- `void *fn_data` - a user-defined pointer for the connection, which is a
  placeholder for application-specific data. This `fn_data` pointer is set
  during the `*_listen()` or `*_connect()` call, and it is stored in the
  `c->fn_data`. Listening connections copy the value of `c->fn_data` to the
  newly accepted connection, so all accepted connections initially share the
  same `fn_data` pointer. It is fine to update/replace that pointer for
  any connection at any time by setting `c->fn_data = new_value;`

## Events

Below is the list of events triggered by Mongoose, taken as-is from `mongoose.h`.
For each event, a comment describes a meaning of the `ev_data` pointer passed
to an event handler:

```c
enum {
  MG_EV_ERROR,      // Error                        char *error_message
  MG_EV_POLL,       // mg_mgr_poll iteration        unsigned long *millis
  MG_EV_RESOLVE,    // Host name is resolved        NULL
  MG_EV_CONNECT,    // Connection established       NULL
  MG_EV_ACCEPT,     // Connection accepted          NULL
  MG_EV_READ,       // Data received from socket    struct mg_str *
  MG_EV_WRITE,      // Data written to socket       int *num_bytes_written
  MG_EV_CLOSE,      // Connection closed            NULL
  MG_EV_HTTP_MSG,   // HTTP request/response        struct mg_http_message *
  MG_EV_HTTP_CHUNK,  // HTTP chunk (partial msg)    struct mg_http_message *
  MG_EV_WS_OPEN,    // Websocket handshake done     struct mg_http_message *
  MG_EV_WS_MSG,     // Websocket msg, text or bin   struct mg_ws_message *
  MG_EV_WS_CTL,     // Websocket control msg        struct mg_ws_message *
  MG_EV_MQTT_CMD,   // MQTT low-level command       struct mg_mqtt_message *
  MG_EV_MQTT_MSG,   // MQTT PUBLISH received        struct mg_mqtt_message *
  MG_EV_MQTT_OPEN,  // MQTT CONNACK received        int *connack_status_code
  MG_EV_SNTP_TIME,  // SNTP time received           struct timeval *
  MG_EV_USER,       // Starting ID for user events
};
```

## Connection flags

`struct mg_connection` has a bitfield with connection flags.  Flags are binary,
they can be either 0 or 1.  Some flags are set by Mongoose and must be not
changed by an application code, for example `is_udp` flag tells application if
that connection is UDP or not.  Some flags can be changed by application, for
example, `is_draining` flag, if set by an application, tells Mongoose to send
the remaining data to peer, and when everything is sent, close the connection.

User-changeable flags are: `is_hexdumping`, `is_draining`, `is_closing`.

This is taken from `mongoose.h` as-is:

```c
struct mg_connection {
  ...
  unsigned is_listening : 1;   // Listening connection
  unsigned is_client : 1;      // Outbound (client) connection
  unsigned is_accepted : 1;    // Accepted (server) connection
  unsigned is_resolving : 1;   // Non-blocking DNS resolv is in progress
  unsigned is_connecting : 1;  // Non-blocking connect is in progress
  unsigned is_tls : 1;         // TLS-enabled connection
  unsigned is_tls_hs : 1;      // TLS handshake is in progress
  unsigned is_udp : 1;         // UDP connection
  unsigned is_websocket : 1;   // WebSocket connection
  unsigned is_hexdumping : 1;  // Hexdump in/out traffic
  unsigned is_draining : 1;    // Send remaining data, then close and free
  unsigned is_closing : 1;     // Close and free the connection immediately
  unsigned is_readable : 1;    // Connection is ready to read
  unsigned is_writable : 1;    // Connection is ready to write
};
```

## Build options

Mongoose source code ships in two files:
- [mongoose.h](https://github.com/cesanta/mongoose/blob/master/mongoose.h) - API definitions
- [mongoose.c](https://github.com/cesanta/mongoose/blob/master/mongoose.c) - implementation

Therefore to integrate Mongoose into an application, simply copy these two
files to the application's source tree. The `mongoose.c` and `mongoose.h` files
are, actually, an amalgamation - a non-amalgamated sources can be found at
https://github.com/cesanta/mongoose/tree/master/src

Mongoose have two types of build constants (preprocessor definitions) that
affect the build: a target architecture, and tunables. In order to set the
option during build time, use the `-D OPTION` compiler flag:

```sh
$ cc app0.c mongoose.c                                        # Use defaults!
$ cc app1.c mongoose.c -D MG_ENABLE_IPV6=1                    # Build with IPv6 enabled
$ cc app2.c mongoose.c -D MG_ARCH=MG_ARCH_FREERTOS_LWIP       # Set architecture
$ cc app3.c mongoose.c -D MG_ENABLE_SSI=0 -D MG_ENABLE_LOG=0  # Multiple options
```

The list of supported
architectures is defined in the
[arch.h](https://github.com/cesanta/mongoose/blob/master/src/arch.h) header
file. Normally, there is no need to explicitly specify the architecture.  The
architecture is guessed during the build, so setting it is not usually required.

| Name | Description |
| ---- | ----------- |
|MG_ARCH_UNIX | All UNIX-like systems like Linux, MacOS, FreeBSD, etc |
|MG_ARCH_WIN32 | Windows systems |
|MG_ARCH_ESP32 | Espressif's ESP32 |
|MG_ARCH_ESP8266 | Espressif's ESP8266 |
|MG_ARCH_FREERTOS_LWIP | All systems with FreeRTOS kernel and LwIP IP stack |
|MG_ARCH_FREERTOS_TCP | All systems with FreeRTOS kernel and FreeRTOS-Plus-TCP IP stack |
|MG_ARCH_CUSTOM | A custom architecture, discussed in the next section |


The other class of build constants is defined in
[src/config.h](https://github.com/cesanta/mongoose/blob/master/src/config.h)
together with their default values. They are tunables that include/exclude
a certain functionality or change relevant parameters.


Here is a list of build constants and their default values:

| Name | Default | Description |
| ---- | ------- | ----------- |
|MG_ENABLE_SOCKET | 1 | Use BSD socket low-level API |
|MG_ENABLE_MBEDTLS | 0 | Enable Mbed TLS library |
|MG_ENABLE_OPENSSL | 0 | Enable OpenSSL library |
|MG_ENABLE_IPV6 | 0 | Enable IPv6 |
|MG_ENABLE_LOG | 1 | Enable `LOG()` macro |
|MG_ENABLE_MD5 | 0 | Use native MD5 implementation |
|MG_ENABLE_SSI | 1 | Enable serving SSI files by `mg_http_serve_dir()` |
|MG_ENABLE_DIRLIST | 0 | Enable directory listing |
|MG_ENABLE_CUSTOM_RANDOM | 0 | Provide custom RNG function `mg_random()` |
|MG_ENABLE_PACKED_FS | 0 | Enable embedded FS support |
|MG_IO_SIZE | 2048 | Granularity of the send/recv IO buffer growth |
|MG_MAX_RECV_BUF_SIZE | (3 * 1024 * 1024) | Maximum recv buffer size |
|MG_MAX_HTTP_HEADERS | 40 | Maximum number of HTTP headers |
|MG_ENABLE_LINES | undefined | If defined, show source file names in logs |


<span class="badge bg-danger">NOTE:</span> the `MG_IO_SIZE` constant also sets
maximum UDP message size, see
[issues/907](https://github.com/cesanta/mongoose/issues/907) for details. If
application uses large UDP messages, increase the `MG_IO_SIZE` limit
accordingly.

## Custom build

A custom build should be used for cases which is not covered by the
existing architecture options. For example, an embedded architecture that
uses some proprietary RTOS and network stack. In order to build on such
systems, follow the guideline outlined below:

1. Add `-DMG_ARCH=MG_ARCH_CUSTOM` to your build flags.

2. Create a file called `mongoose_custom.h`, with defines and includes that
are relevant to your platform. Mongoose uses `bool` type, `MG_DIRSEP` define,
and optionally other structures like `DIR *` depending on the functionality
you have enabled - see previous section. Below is an example:

```c
#include <stdbool.h>            // For bool
#include <stdarg.h>

#define MG_DIRSEP '/'
#define MG_INT64_FMT "%lld"
#define MG_ENABLE_SOCKET 0      // Disable BSD socket API, implement your own
```

3. This step is optional. If you have disabled BSD socket API, your build is
going to fail due to several undefined symbols. Create `mongoose_custom.c`
and implement the following functions (take a look at `src/sock.c` for the
reference implementation):

```c
struct mg_connection *mg_connect(struct mg_mgr *mgr, const char *url,
                                 mg_event_handler_t fn, void *fn_data) {
  // implement this!
}

void mg_connect_resolved(struct mg_connection *c) {
  // implement this!
}


struct mg_connection *mg_listen(struct mg_mgr *mgr, const char *url,
                                mg_event_handler_t fn, void *fn_data) {
  // implement this!
}

void mg_mgr_poll(struct mg_mgr *mgr, int ms) {
  // implement this!
}

int mg_send(struct mg_connection *c, const void *buf, size_t len) {
  // implement this!
}
```


## Minimal HTTP server

This example is a simple static HTTP server that serves current directory:

```c
#include "mongoose.h"

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  struct mg_http_serve_opts opts = {.root_dir = "."};   // Serve local dir
  if (ev == MG_EV_HTTP_MSG) mg_http_serve_dir(c, ev_data, &opts);
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);                                        // Init manager
  mg_http_listen(&mgr, "http://localhost:8000", fn, &mgr);  // Setup listener
  for (;;) mg_mgr_poll(&mgr, 1000);                         // Event loop
  mg_mgr_free(&mgr);                                        // Cleanup
  return 0;
}
```

## Minimal TCP echo server

This example is a simple TCP echo server that listens on port 1234:

```c
#include "mongoose.h"

static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_READ) {
    mg_send(c, c->recv.buf, c->recv.len);     // Echo received data back
    mg_iobuf_del(&c->recv, 0, c->recv.len);   // And discard it
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);                                // Init manager
  mg_listen(&mgr, "tcp://0.0.0.0:1234", cb, &mgr);  // Setup listener
  for (;;) mg_mgr_poll(&mgr, 1000);                 // Event loop
  mg_mgr_free(&mgr);                                // Cleanup    
  return 0;
}
```


# API Reference

## Core

### struct mg\_addr

```c
struct mg_addr {
  uint16_t port;    // TCP or UDP port in network byte order
  uint32_t ip;      // IP address in network byte order
  uint8_t ip6[16];  // IPv6 address
  bool is_ip6;      // True when address is IPv6 address
};
```

This structure contains network address, it can be considered as a Mongoose equivalent for sockets `sockaddr` structure.
 
### struct mg\_mgr

```c
struct mg_mgr {
  struct mg_connection *conns;  // List of active connections
  struct mg_dns dns4;           // DNS for IPv4
  struct mg_dns dns6;           // DNS for IPv6
  int dnstimeout;               // DNS resolve timeout in milliseconds
  unsigned long nextid;         // Next connection ID
  void *userdata;               // Arbitrary user data pointer
};
```
Event management structure that holds a list of active connections, together
with some housekeeping information.


### struct mg\_connection

```c
struct mg_connection {
  struct mg_connection *next;  // Linkage in struct mg_mgr :: connections
  struct mg_mgr *mgr;          // Our container
  struct mg_addr peer;         // Remote address. For listeners, local address
  void *fd;                    // Connected socket, or LWIP data
  unsigned long id;            // Auto-incrementing unique connection ID
  struct mg_iobuf recv;        // Incoming data
  struct mg_iobuf send;        // Outgoing data
  mg_event_handler_t fn;       // User-specified event handler function
  void *fn_data;               // User-specified function parameter
  mg_event_handler_t pfn;      // Protocol-specific handler function
  void *pfn_data;              // Protocol-specific function parameter
  char label[50];              // Arbitrary label
  void *tls;                   // TLS specific data
  unsigned is_listening : 1;   // Listening connection
  unsigned is_client : 1;      // Outbound (client) connection
  unsigned is_accepted : 1;    // Accepted (server) connection
  unsigned is_resolving : 1;   // Non-blocking DNS resolve is in progress
  unsigned is_connecting : 1;  // Non-blocking connect is in progress
  unsigned is_tls : 1;         // TLS-enabled connection
  unsigned is_tls_hs : 1;      // TLS handshake is in progress
  unsigned is_udp : 1;         // UDP connection
  unsigned is_websocket : 1;   // WebSocket connection
  unsigned is_hexdumping : 1;  // Hexdump in/out traffic
  unsigned is_draining : 1;    // Send remaining data, then close and free
  unsigned is_closing : 1;     // Close and free the connection immediately
  unsigned is_readable : 1;    // Connection is ready to read
  unsigned is_writable : 1;    // Connection is ready to write
};
```

A connection - either a listening connection, or an accepted connection,
or an outbound connection.

### mg\_mgr\_init()

```c
void mg_mgr_init(struct mg_mgr *mgr);
```

Initialize event manager structure:
- Set a list of active connections to NULL
- Set default DNS servers for IPv4 and IPv6
- Set default DNS lookup timeout

Parameters:
- `mgr` - a pointer to `struct mg_mgr` that needs to be initialised

Return value: none

Usage example:

```c
struct mg_mgr mgr;
mg_mgr_init(&mgr);
```



### mg\_mgr\_poll()

```c
void mg_mgr_poll(struct mg_mgr *mgr, int ms);
```

Perform a single poll iteration. For each connection in the `mgr->conns` list,
- See if there is incoming data. If it is, read it into the `c->recv` buffer, send `MG_EV_READ` event
- See if there is data in the `c->send` buffer, and write it, send `MG_EV_WRITE` event
- If a connection is listening, accept an incoming connection if any, and send `MG_EV_ACCEPT` event to it
- Send `MG_EV_POLL` event

Each connection has two event handler functions: `c->fn` and `c->pfn`.  The
`c->fn` is a user-specified event handler function. The `c->pfn` is a
protocol-specific handler function that is set implicitly. For example, a
`mg_http_listen()` sets `c->pfn` to a Mongoose's HTTP event handler.  A
protocol-specific handler is called before user-specific handler.  It parses
incoming data and may invoke protocol-specific events like `MG_EV_HTTP_MSG`.

Usage example:

```c
struct mg_mgr mgr;
bool stopped;
...
while(!stopped) {
  mg_mgr_poll(&mgr, 1000 /* 1 sec */);
  // Do something else
}
```

### mg\_mgr\_free()

```c
void mg_mgr_free(struct mg_mgr *mgr);
```

Close all connections, and free all resources.

Usage example:

```c
struct mg_mgr mgr;
// App code here
// ...

// Time to cleanup
mg_mgr_free(&mgr);
```

### mg\_listen()

```c
struct mg_connection *mg_listen(struct mg_mgr *mgr, const char *url,
                                mg_event_handler_t fn, void *fn_data);
```

Create a listening connection, append this connection to `mgr->conns`.
- `url` - specifies local IP address and port to listen on, e.g.
  `tcp://127.0.0.1:1234` or `udp://0.0.0.0:9000`
- `fn` - an event handler function
- `fn_data` - an arbitrary pointer, which will be passed as `fn_data` when an
  event handler is called. This pointer is also stored in a connection
  structure as `c->fn_data`

Return value: created connection, or `NULL` on error.

Usage example:

```c
// Mongoose events handler
void handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
// Processing Mongoose events
}

struct mg_mgr mgr;
// Initialization
// ...

struct mg_connection *lc = mg_listen(&mgr, "tcp://127.0.0.1:8080", handler, NULL); 
```

### mg\_connect()

```c
struct mg_connection *mg_connect(struct mg_mgr *mgr, const char *url,
                                 mg_event_handler_t fn, void *fn_data);
```

Create an outbound connection, append this connection to `mgr->conns`.
- `url` - specifies remote IP address/port to connect to, e.g. `http://a.com`
- `fn` - an event handler function
- `fn_data` - an arbitrary pointer, which will be passed as `fn_data` when an
  event handler is called. This pointer is also stored in a connection
  structure as `c->fn_data`

Return value: created connection, or `NULL` on error.

Usage example:

```c
// Mongoose events handler
void handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
// Processing Mongoose events
}

struct mg_mgr mgr;
// Initialization
// ...

struct mg_connection *c = mg_connect(&mgr, "http://example.org", handler, NULL); 
```

### mg\_send()

```c
int mg_send(struct mg_connection *c, const void *data, size_t size);
```

Append `data` of size `size` to the `c->send` buffer. Return number of bytes
appended.

Note: this function does not push data to the network! It only appends data to
the output buffer.  The data is being sent when `mg_mgr_poll()` is called. If
`mg_send()` is called multiple times, the output buffer grows.

Usage example:

```c
struct mg_connection *c;
// Connection initialization
// ...

char data="Hello, world!";
mg_send(c, data, sizeof(data) - 1); // Add "Hello, world!" to output buffer
```

### mg\_printf()

```c
int mg_printf(struct mg_connection *, const char *fmt, ...);
```

Same as `mg_send()`, but formats data using `printf()` semantics. Return
number of bytes appended to the output buffer.

Usage example:

```c
struct mg_connection *c;
// Connection initialization
// ...

mg_printf(c, "Hello, %s!", "world"); // Add "Hello, world!" to output buffer
```

### mg\_vprintf()

```c
int mg_vprintf(struct mg_connection *, const char *fmt, va_list ap);
```

Same as `mg_printf()`, but takes `va_list` argument as a parameter.

Usage example:

```c
void foo(struct mg_connection *c, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  mg_vprintf(c, fmt, ap);
  va_end(ap);
}

// ...
struct mg_connection *c;
// Connection initialization
// ...

foo(c, "Hello, %s!", "world"); // Add "Hello, world!" to output buffer
```

### mg\_straddr

```c
char *mg_straddr(struct mg_connection *c, char *buf, size_t len)
```

Write stringified IP address, associated with given connection to `buf`.

Usage example:

```c
struct mg_connection *c;
// Connection initialization
// ...

char buf[1024];
mg_straddr(c, buf, sizeof(buf)); // `buf` is now IP address string, like "127.0.0.1:8080"
```

### mg\_mkpipe()

```c
struct mg_connection *mg_mkpipe(struct mg_mgr *, mg_event_handler_t, void *);
```

Create a "pipe" connection which is safe to pass to a different task/thread,
and which is used to wake up event manager from a different task. These 
functions are designed to implement multi-threaded support, to handle two
common use cases:

- There are multiple consumer connections, e.g. connected websocket clients.
  A server constantly pushes some data to all of them. In this case, a data
  producer task should call `mg_mgr_wakeup()` as soon as more data is produced.
  A pipe's event handler should push data to all client connection.
  Use `c->label` to mark client connections.
- In order to serve a request, a long blocking operation should be performed.
  In this case, request handler assignes some marker to `c->label` and then
  spawns a handler task and gives a pipe to a
  handler task. A handler does its job, and when data is ready, wakes up a
  manager. A pipe's event handler pushes data to a marked connection.

Another task can wake up a sleeping event manager (in `mg_mgr_poll()` call)
using `mg_mgr_wakeup()`. When an event manager is woken up, a pipe
connection event handler function receives `MG_EV_READ` event.

Usage example:

```c
struct mg_mgr mgr;
// Mgr initialization
// ...

// Mongoose events handler
static void handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
// Processing Mongoose events
}

// ...

struct mg_connection *c = mg_mkpipe(&mgr, handler, NULL);

```

See [examples/multi-threaded](../examples/multi-threaded) for full usage example.

### mg\_mgr_wakeup()

```c
void mg_mgr_wakeup(struct mg_connection *pipe);
```

Wake up an event manager that sleeps in `mg_mgr_poll()` call. This function
must be called from a separate task/thread. Parameters:

- `pipe` - a special connection created by the `mg_mkpipe()` call

Usage example:

```c
struct mg_connection *c;
...
mg_mgr_wakeup(c);
```

## HTTP

### struct mg\_http\_header

```c
struct mg_http_header {
  struct mg_str name;   // Header name
  struct mg_str value;  // Header value
};
```

### struct mg\_http\_message

```c
struct mg_http_message {
  //        GET /foo/bar/baz?aa=b&cc=ddd HTTP/1.1
  // method |-| |----uri---| |--query--| |proto-|

  struct mg_str method, uri, query, proto;             // Request/response line
  struct mg_http_header headers[MG_MAX_HTTP_HEADERS];  // Headers
  struct mg_str body;                                  // Body
  struct mg_str message;                               // Request line + headers + body
};
```

<kbd>
<img src=".\pics\mg_http_message.png">
</kbd>

### mg\_http\_listen()

```c
struct mg_connection *mg_http_listen(struct mg_mgr *, const char *url, mg_event_handler_t fn, void *fn_data);
```

Create HTTP listener.

- `url` - specifies local IP address and port to listen on, e.g. `http://0.0.0.0:8000`
- `fn` - an event handler function
- `fn_data` - an arbitrary pointer, which will be passed as `fn_data` when an
  event handler is called. This pointer is also stored in a connection
  structure as `c->fn_data`

Usage example:

```c
struct mg_mgr mgr;
// Mgr initialization
// ...

struct mg_connection *c = mg_http_listen(&mgr, "0.0.0.0:8000", fn, arg); 
if (c != NULL) {
// Successfully started listening on port 8000
}
```

### mg\_http\_connect()

```c
struct mg_connection *mg_http_connect(struct mg_mgr *, const char *url,
                                      mg_event_handler_t fn, void *fn_data);
```

Create HTTP client connection.
- `url` - specifies remote URL, e.g. `http://google.com`
- `fn` - an event handler function
- `fn_data` - an arbitrary pointer, which will be passed as `fn_data` when an
  event handler is called. This pointer is also stored in a connection
  structure as `c->fn_data`

Usage example: 

```c
struct mg_mgr mgr;
// Mgr initialization
// ...

// Mongoose events handler
void handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
// Processing Mongoose events
}

// ...

struct mg_connection *c = mg_http_connect(&mgr, "http://google.com", handler, NULL);
if (c != NULL) {
// Connection created successfully 
}
```


### mg\_http\_get\_request\_len()

```c
int mg_http_get_request_len(const unsigned char *buf, size_t buf_len);
```

Return length of request in `buf` (with maximum len `buf_len`).

The length of request is a number of bytes till the end of HTTP headers. It does not include length of HTTP body. 
Return value: -1 on error, 0 if a message is incomplete, or the length of request. 

Usage example:

```c
char buf[1024]; // Input buffer, for ex: "GET /test \n\nGET /test \n\nGET /test \n\n"
size_t buf_len; // Input buffer size
// ... 
int req_len = mg_http_get_request_len(buf, buf_len); // req_len is now 12
```

### mg\_http\_parse()

```c
int mg_http_parse(const char *s, size_t len, struct mg_http_message *hm);
```

Parse string `s` (with maximum size `len`) into a structure `hm`. Return request length - see `mg_http_get_request_len()`.

Usage example:

```c
char req[1024]; // Received request, for ex: "GET /test HTTP/1.0\r\nFoo:  bar  \r\n\r\n"
size_t req_len; // Received request len
// ...
struct mg_http_message hm;
if(mg_http_parse(req, req_len, &hm) > 0) {
  // Parsed successfully 
}
```

### mg\_http\_printf\_chunk()

```
void mg_http_printf_chunk(struct mg_connection *cnn, const char *fmt, ...);
```

Write a chunk of data in chunked encoding format, using `printf()` semantic.

Usage example:

```c
struct mg_connection *c;
// Connection initialization
// ...

mg_http_printf_chunk(c, "Hello, %s!", "world"); // Encoded "Hello, world!" is added to output buffer
```

### mg\_http\_write\_chunk()

```c
void mg_http_write_chunk(struct mg_connection *c, const char *buf, size_t len);
```

Write a chunk of data in chunked encoding format.

Usage example:

```c
struct mg_connection *c;
// Connection initialization
// ...

char data[] = "Hello, world!";
mg_http_write_chunk(c, data, sizeof(data) - 1); // Encoded "Hello, world!" is added to output buffer
```

### mg\_http\_delete\_chunk()

```c
void mg_http_delete_chunk(struct mg_connection *c, struct mg_http_message *hm);
```

Remove chunk specified by `hm` from input buffer.

Usage example:

```c
// Mongoose events handler
void handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_CHUNK) { // Chunk received
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    mg_http_delete_chunk(c, hm); // Remove received chunk
  }
}
```

### mg\_http\_serve\_dir()

```c
struct mg_http_serve_opts {
  const char *root_dir;       // Web root directory, must be non-NULL
  const char *ssi_pattern;    // SSI file name pattern, e.g. #.shtml
  const char *extra_headers;  // Extra HTTP headers to add in responses
};
void mg_http_serve_dir(struct mg_connection *, struct mg_http_message *hm,
                       const struct mg_http_serve_opts *opts);
```

Serve static files according to the given options. Note that in order to
enable SSI, set a `-DMG_ENABLE_SSI=1` build flag.

Usage example:

```c
// Mongoose events handler
void handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) { // HTTP message received
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct mg_http_serve_opts sopts;
    memset(&sopts, 0, sizeof(sopts));
    sopts.root_dir = "/my_root";
    mg_http_serve_dir(c, hm, &sopts); // Send directory information 
  }
}
```

### mg\_http\_serve\_file()

```c
void mg_http_serve_file(struct mg_connection *, struct mg_http_message *hm,
                        const char *path, struct mg_http_serve_opts *opts);
```

Serve static file. Note that the `extra_headers` must end with `\r\n`.

Usage example:

```c
// Mongoose events handler
void handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) { // HTTP message received
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct mg_http_serve_opts opts = {.mime_types = "png=image/png", 
                                  .extra_headers = "AA: bb\r\nCC: dd\r\n"};
    mg_http_serve_file(c, hm, "a.png", &opts);  // Send file information
  }
}
```

### mg\_http\_reply()

```c
void mg_http_reply(struct mg_connection *c, int status_code, 
                   const char *headers, const char *body_fmt, ...);
```

Send simple HTTP response using `printf()` semantic. This function formats
response body according to a `body_fmt`, and automatically appends a correct
`Content-Length` header. Extra headers could be passed via `headers`
parameter.

- `status_code` - an HTTP response code
- `headers` - extra headers, default NULL. If not NULL, must end with `\r\n`
- `fmt` - a format string for the HTTP body, in a printf semantics

<kbd>
<img src=".\pics\mg_http_reply.png">
</kbd>

Usage examples:


Send a simple JSON respose:
```c
mg_http_reply(c, 200, "Content-Type: application/json\r\n", "{\"result\": %d}", 123);
```

Send JSON response using [mjson](https://github.com/cesanta/mjson) library:
```c
char *json = NULL;
mjson_printf(mjson_print_dynamic_buf, &json, "{%Q:%d}", "name", 123);
mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s", json);
free(json);
```

Send a 302 redirect:
```c
mg_http_reply(c, 302, "Location: /\r\n", "");
```

Send error:
```c
mg_http_reply(c, 403, "", "%s", "Not Authorized\n");
```

### mg\_http\_get\_header()

```c
struct mg_str *mg_http_get_header(struct mg_http_message *, const char *name);
```

Return value of `name` HTTP header, or NULL if not found.

Usage example:

```c
// Mongoose events handler
void handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) { // HTTP message received
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    // Get "X-Extra-Header" header value 
    struct mg_str *header = mg_http_get_header(hm, "X-Extra-Header");
    if(header != NULL) {
      // Use header
    }
  }
}
```

### mg\_http\_get\_var()

```c
int mg_http_get_var(const struct mg_str *, const char *name, char *buf, int len);
```

Decode HTTP variable `name` into a given buffer. Return length of decoded
variable. Zero or negative value means error.

Usage example:

```c
struct mg_str body; // request body
// ...

char buf[256];
if(mg_http_get_var(&body, "key1", buf, sizeof(buf)) {
  // "key1" value successfully retrieved 
}
```

### mg\_http\_creds()

```c
void mg_http_creds(struct mg_http_message *, char *user, size_t userlen, char *pass, size_t passlen);
```

Fetch authentication credential from the request, and store into the
`user`, `userlen` and `pass`, `passlen` buffers. The credentials are looked
up in the following order:
- from the `Authorization` HTTP header,
   - Basic auth fills both user and pass
   - Bearer auth fills only pass
- from the `access_token` cookie, fills pass
- from the `?access_token=...` query string parameter, fills pass

If none is found, then both user and pass are set to empty nul-terminated strings.

Usage example:

```c
// Mongoose events handler
void handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) { // HTTP message received
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    char user[100], pass[100];
    mg_http_creds(hm, user, sizeof(user), pass, sizeof(pass)); // "user" is now user name and "pass" is now password from request
  }
}
```

### mg\_http\_match\_uri()

```c
bool mg_http_match_uri(const struct mg_http_message *, const char *glob);
```

Return true if HTTP request matches a given glob pattern; false otherwise.

Usage example:

```c
// Mongoose events handler
void handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {  // HTTP message received
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/test_uri")) { 
      // HTTP request to /test_uri
      // ...
    }
  }
}
```

### mg\_http\_upload()

```c
int mg_http_upload(struct mg_connection *, struct mg_http_message *hm,
                   const char *dir);
```

Handle file upload. See [file upload example](https://github.com/cesanta/mongoose/tree/master/examples/file-upload).

This function  expects a series of POST requests with file data. POST requests
should have `name` and `offset` query string parameters set:

```text
POST /whatever_uri?name=myfile.txt&offset=1234 HTTP/1.0
Content-Length: 5

hello
```

- `name` - a mandatory query string parameter, specifies a file name. It it
  created in the `dir` directory
- `offset` - an optional parameter, default `0`. If it set to `0`, or omitted,
  then a file gets truncated before write. Otherwise, the body of
  the POST request gets appended to the file
- Server must call `mg_http_upload()` when `/whatever_uri` is hit

So, the expected usage of this API function is this:
- A client splits a file into small enough chunks, to ensure that a chunk
  fits into the server's RAM
- Then, each chunk is POST-ed to the server with using URI like
  `/some_uri?name=FILENAME&offset=OFFSET`
- Initial OFFSET is `0`, and subsequent offsets are non-zero
- Each chunk gets appended to the file
- When the last chunk is POSTed, upload finishes
- POST data must not be encoded in any way, it it saved as-is

Usage example:

```c
// Mongoose events handler
void handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) { // HTTP message received
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    mg_http_upload(c, hm, "."); // Upload to current folder 
  }
```

### mg\_http\_bauth()

```c
void mg_http_bauth(struct mg_connection *, const char *user, const char *pass);
```

Write a Basic `Authorization` header to the output buffer.

Usage example:

```c
struct mg_connection *c;
// Connection initialization
// ...

mg_http_bauth(c, "user_name", "password") // "user_name:password" is now in output buffer
```

### mg\_http\_next\_multipart()

```c
// Parameter for mg_http_next_multipart
struct mg_http_part {
  struct mg_str name;      // Form field name
  struct mg_str filename;  // Filename for file uploads
  struct mg_str body;      // Part contents
};
```
<kbd>
<img src=".\pics\mg_http_part.png">
</kbd>

```c
size_t mg_http_next_multipart(struct mg_str body, size_t offset, struct mg_http_part *part);
```

Parse the multipart chunk in the `body` at a given `offset`. An initial
`offset` should be 0. Fill up parameters in the provided `part`, which could be NULL. Return offset to the next chunk, or 0 if there are no more chunks.

See [examples/form-upload](../examples/form-upload) for full usage example.

<kbd>
<img src=".\pics\mg_http_next_multipart.png">
</kbd>

Usage example:

```c
struct mg_str *body; // received body
// ...

struct mg_http_part part;
if(mg_http_next_multipart(body, 0 /* begin */, &part)) {
  // Use part
}
```

## Websocket

### struct mg\_ws\_message

```c
struct mg_ws_message {
  struct mg_str data; // Websocket message data
  uint8_t flags;      // Websocket message flags
};
```

### mg\_ws\_connect()

```c
struct mg_connection *mg_ws_connect(struct mg_mgr *, const char *url,
                                    mg_event_handler_t fn, void *fn_data,
                                    const char *fmt, ...);
```

Create client Websocket connection.
- `url` - specifies remote URL, e.g. `http://google.com`
- `opts` - MQTT options, with client ID, qos, etc
- `fn` - an event handler function
- `fn_data` - an arbitrary pointer, which will be passed as `fn_data` when an
  event handler is called. This pointer is also stored in a connection
  structure as `c->fn_data`
- `fmt` - printf-like format string for additional HTTP headers, or NULL


Usage example: 

```c
// Mongoose events handler
void handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
// Processing Mongoose events
}
// ...
struct mg_mgr mgr;
// Mgr initialization
// ...

struct mg_connection *c = mg_ws_connect(&mgr, "ws://test_ws_server.com:1000", 
                                        handler, NULL, "%s", "Sec-WebSocket-Protocol: echo\r\n");
if(c != NULL) {
// Connection created successfully 
}
```

### mg\_ws\_upgrade()

```c
void mg_ws_upgrade(struct mg_connection *, struct mg_http_message *,
                   const char *fmt, ...);
```

Upgrade given HTTP connection to Websocket. The `fmt` is a printf-like
format string for the extra HTTP headers returned to the client in a
Websocket handshake. Set `fmt` to `NULL` if no extra headers needs to be passed.

Usage example:

```c
// Mongoose events handler
void handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {  // HTTP message received
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    mg_ws_upgrade(c, hm, NULL);  // Upgrade HTTP to WS
  }
}
```

### mg\_ws\_send()

```c
size_t mg_ws_send(struct mg_connection *, const char *buf, size_t len, int op);
```

Send `buf` (`len` size) to the websocket peer. `op` is the Websocket message type:

```c
#define WEBSOCKET_OP_CONTINUE 0
#define WEBSOCKET_OP_TEXT 1
#define WEBSOCKET_OP_BINARY 2
#define WEBSOCKET_OP_CLOSE 8
#define WEBSOCKET_OP_PING 9
#define WEBSOCKET_OP_PONG 10
```

Usage example:

```c
// Mongoose events handler
void handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_WS_OPEN) {  // WS connection opened
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    mg_ws_send(c, "opened", 6, WEBSOCKET_OP_BINARY);  // Send "opened" to web socket connection
  }
}
```

### mg\_ws\_wrap()

```c
size_t mg_ws_wrap(struct mg_connection *c, size_t len, int op)
```

Convert data in output buffer to WebSocket format. Useful then implementing protocol over WebSocket
See [examples/mqtt-over-ws-client](../examples/mqtt-over-ws-client) for full example.

Usage example:

```c
struct mg_connection *c;
// Connection initialization
// ...

size_t len = c->send.len;         // Store output buffer len
struct mg_mqtt_opts opts
mg_mqtt_login(c, s_url, &opts);  // Prepare output buffer data

mg_ws_wrap(c, c->send.len - len, WEBSOCKET_OP_BINARY); // Convert output buffer to binary websocket format
```

## SNTP

### mg_sntp_connect()

```c
struct mg_connection *mg_sntp_connect(struct mg_mgr *mgr, const char *url,
                                      mg_event_handler_t fn, void *fn_data)
```

Connect SNTP server specified by `url` or `time.google.com` if NULL.
Return pointer to created connection or `NULL` on error.

Usage example:

```c
struct mg_mgr mgr;
// Mgr initialization
// ...

// Mongoose events handler
static void sntp_cb(struct mg_connection *c, int ev, void *evd, void *fnd) {
  if (ev == MG_EV_SNTP_TIME) { SNTP message received
    // Time received
    struct timeval *tv = (struct timeval *tv)evd;
  }
}

struct mg_connection *c = mg_sntp_connect(mgr&, NULL /* connect to time.google.com */,
                                          sntp_cb, NULL);
if (c != NULL) {
  // Connection successfully created
}                                          
```

### mg_sntp_send()

```c
void mg_sntp_send(struct mg_connection *c, unsigned long utc)
```

Send time request to SNTP server. Note, that app can't send SNTP request more often than every 1 hour.
`utc` is a current time, used to indicate if new request is possible.

Usage example:

```c
struct mg_connection *c;
// Connection initialization
// ...

mg_sntp_send(c, (unsigned long) time(NULL));
```

## MQTT

### struct mg\_mqtt\_opts

```c
struct mg_mqtt_opts {
  struct mg_str client_id;    // Client id
  struct mg_str will_topic;   // Will Topic
  struct mg_str will_message; // Will Message
  uint8_t qos;                // Quality of service
  bool will_retain;           // Retain last will
  bool clean;                 // Use clean session, 0 or 1
  uint16_t keepalive;         // Keep-alive timer in seconds
};
```

### struct mg\_mqtt\_message

```c
struct mg_mqtt_message {
  struct mg_str topic;  // Topic
  struct mg_str data;   // Message data
};
```


### mg\_mqtt\_connect()

```c
struct mg_connection *mg_mqtt_connect(struct mg_mgr *, const char *url,
                                      struct mg_mqtt_opts *opts,
                                      mg_event_handler_t fn, void *fn_data);
```

Create client MQTT connection.
- `url` - specifies remote URL, e.g. `http://google.com`
- `opts` - MQTT options, with client ID, qos, etc
- `fn` - an event handler function
- `fn_data` - an arbitrary pointer, which will be passed as `fn_data` when an
  event handler is called. This pointer is also stored in a connection
  structure as `c->fn_data`

Usage example:

```c
struct mg_mgr mgr;
// Mgr initialization
// ...

// Mongoose events handler
void handler(struct mg_connection *c, int ev, void *evd, void *fnd) {
  char *buf = (char *) fnd;
  if (ev == MG_EV_MQTT_OPEN) {
    // Connection ready
  } 
}

if(mg_mqtt_connect(&mgr, "mqtt://test.org:1883", NULL, handler, NULL) != NULL) {
  // Connection successfully created 
}
```

### mg\_mqtt\_listen()

```c
struct mg_connection *mg_mqtt_listen(struct mg_mgr *mgr, const char *url,
                                     mg_event_handler_t fn, void *fn_data);
```

Create MQTT listener.

- `url` - specifies local IP address and port to listen on, e.g. `mqtt://0.0.0.0:1883`
- `fn` - an event handler function
- `fn_data` - an arbitrary pointer, which will be passed as `fn_data` when an
  event handler is called. This pointer is also stored in a connection
  structure as `c->fn_data`

Usage example:

```c
struct mg_mgr mgr;
// Mgr initialization
// ...

struct mg_connection *c = mg_mqtt_listen(&mgr, "0.0.0.0:1883", fn, arg); 
if (c != NULL) {
  // Successfully started listening on port 1833
}
```

### mg\_mqtt\_login

```c
void mg_mqtt_login(struct mg_connection *c, const char *url,
                   struct mg_mqtt_opts *opts);
```

Send MQTT login request.

Usage example:

```c
// Mongoose events handler
void handler(struct mg_connection *c, int ev, void *evd, void *fnd) {
  char *buf = (char *) fnd;
  if (ev == MG_EV_MQTT_OPEN) {  // MQTT connection open
    struct mg_mqtt_opts opts = {.qos = 1,
                                .will_topic = mg_str("my topic"),
                                .will_message = mg_str("goodbye")};
    mg_mqtt_login(c, s_url, &opts);
  } 
}
```

### mg\_mqtt\_pub()

```c
void mg_mqtt_pub(struct mg_connection *, struct mg_str *topic,
                 struct mg_str *data, int qos, bool retain);
```

Publish message `data` to the topic `topic` with given QoS and retain flag.

Usage example:

```c
struct mg_connection *c;
// Connection initialization
// ...

struct mg_str topic = mg_str("topic");
struct mg_str data = mg_str("data");

mg_mqtt_pub(c, &topic, &data, 1, false);
```

### mg\_mqtt\_sub()

```c
void mg_mqtt_sub(struct mg_connection *, struct mg_str *topic, int qos);
```

Subscribe to topic `topic` with given QoS.

```c
struct mg_connection *c;
// Connection initialization
// ...

struct mg_str topic = mg_str("topic");
mg_mqtt_sub(c, &topic, 1);
```

### mg\_mqtt\_next\_sub()

```c
size_t mg_mqtt_next_sub(struct mg_mqtt_message *msg, struct mg_str *topic, uint8_t *qos, size_t pos);
```

Traverse list of subscribed topics. 
Used to implement MQTT server when `MQTT_CMD_SUBSCRIBE` is received.
Return next position, or 0 when done. Initial position `pos` should be 4. Example:

Usage example:

```c
// Mongoose events handler
void handler(struct mg_connection *c, int ev, void *evd, void *fnd) {
  if (ev == MG_EV_MQTT_CMD) { // MQTT command received
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    if (mm->cmd == MQTT_CMD_SUBSCRIBE) { // "Subscribe" command
      size_t pos = 4;
      uint8_t qos;
      struct mg_str topic;
      while ((pos = mg_mqtt_next_sub(mm, &topic, &qos, pos)) > 0) { // Traverse subscribed topics
        LOG(LL_INFO, ("SUB [%.*s]", (int) topic.len, topic.ptr)); 
      }
    }
  }
}
```

### mg\_mqtt\_next\_unsub()

```c
size_t mg_mqtt_next_unsub(struct mg_mqtt_message *msg, struct mg_str *topic, size_t pos);
```

Same as `mg_mqtt_next_sub()`, but for unsubscribed topics. The difference
is that there is no QoS in unsubscribe request.

Usage example:

```c
// Mongoose events handler
void handler(struct mg_connection *c, int ev, void *evd, void *fnd) {
  if (ev == MG_EV_MQTT_CMD) { // MQTT command received
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    if (mm->cmd == MQTT_CMD_UNSUBSCRIBE) {  // "Unsubscribe" command
      size_t pos = 4;
      struct mg_str topic;
      if (mm->cmd == MQTT_CMD_UNSUBSCRIBE) {  // "Unsubscribe" command
        while ((pos = mg_mqtt_next_unsub(mm, &topic, pos)) > 0) { // Traverse unsubscribed topics
          LOG(LL_INFO, ("SUB [%.*s]", (int) topic.len, topic.ptr));
        }
      }
   }
  }
}
```

### mg\_mqtt\_send_header()

```c
void mg_mqtt_send_header(struct mg_connection *, uint8_t cmd, uint8_t flags, uint32_t len);
```
Send MQTT command header. Useful in MQTT server implementation. Command can be one of the following value:

```c
#define MQTT_CMD_CONNECT 1
#define MQTT_CMD_CONNACK 2
#define MQTT_CMD_PUBLISH 3
#define MQTT_CMD_PUBACK 4
#define MQTT_CMD_PUBREC 5
#define MQTT_CMD_PUBREL 6
#define MQTT_CMD_PUBCOMP 7
#define MQTT_CMD_SUBSCRIBE 8
#define MQTT_CMD_SUBACK 9
#define MQTT_CMD_UNSUBSCRIBE 10
#define MQTT_CMD_UNSUBACK 11
#define MQTT_CMD_PINGREQ 12
#define MQTT_CMD_PINGRESP 13
#define MQTT_CMD_DISCONNECT 14
```

Usage example:

```c
// Mongoose events handler
void handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_MQTT_CMD) { // MQTT command received
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    if (mm->cmd == MQTT_CMD_CONNECT) {  Connect request
        uint8_t response[] = {0, 0};
        mg_mqtt_send_header(c, MQTT_CMD_CONNACK, 0, sizeof(response));  // Send acknowledgement
        mg_send(c, response, sizeof(response));
    }
  }
}
```

### mg\_mqtt\_ping()

```c
void mg_mqtt_ping(struct mg_connection *);
```

Send `MQTT_CMD_PINGREQ` command via `mg_mqtt_send_header`

Usage example:

```c
struct mg_connection *c;
// Connection initialization
// ...

mg_mqtt_ping(c);
```

### mg\_mqtt\_pong

```c
void mg_mqtt_pong(struct mg_connection *);
```

Send `MQTT_CMD_PINGRESP` command via `mg_mqtt_send_header`

Usage example:

```c
struct mg_connection *c;
// Connection initialization
// ...

mg_mqtt_pong(c);
```

### mg_mqtt_disconnect

```c
void mg_mqtt_disconnect(struct mg_connection *nc);
```

Disconnect from broker by sending `MQTT_CMD_DISCONNECT` command via `mg_mqtt_send_header`

Usage example:

```c
struct mg_connection *c;
// Connection initialization
// ...

mg_mqtt_disconnect(c);
```

### mg_mqtt_parse

```c
int mg_mqtt_parse(const uint8_t *buf, size_t len, struct mg_mqtt_message *m);
```

Parse buffer and fill `m` struct if buffer contain MQTT message.
Return `MQTT_OK` if message succesfully parsed, `MQTT_INCOMPLETE` if message isn't fully receives and `MQTT_MALFORMED` is message has wrong format.

Usage example:

```c
uint8_t buf[1024]; // Received buffer
size_t buf_len; // Received data len
// ...
struct mg_mqtt_message m;
if(mg_mqtt_parse(buf, buf_len, &m) == MQTT_OK) {
  // Message received
}
```

## TLS

### struct mg\_tls\_opts

```c
struct mg_tls_opts {
  const char *ca;        // CA certificate file. For both listeners and clients
  const char *cert;      // Certificate
  const char *certkey;   // Certificate key
  const char *ciphers;   // Cipher list
  struct mg_str srvname; // If not empty, enables server name verification
};
```

TLS initialisation structure:
- `ca` - Certificate Authority. Can be a filename, or a string. Used to verify
  a certificate that other end sends to us. If NULL, then certificate checking
  is disabled
- `cert` - our own certificate. Can be a filename, or a string. If NULL, then
  we don't authenticate with the other peer
- `certkey` - a key for a `cert`. Sometimes, a certificate and its key are
  bundled in a single .pem file, in which case the values for `cert` and
  `certkey` could be the same
- `ciphers` - a list of allowed ciphers
- `srvname` - enable server name verification


NOTE: if both `ca` and `cert` are set, then so-called two-way TLS is enabled,
when both sides authenticate with each other. Usually, server-side connections
set both `ca` and `cert`, whilst client-side - only `ca`.

### mg\_tls\_init()

```c
void mg_tls_init(struct mg_connection *c, struct mg_tls_opts *opts);
```

Initialise TLS on a given connection.

<span class="badge bg-danger">NOTE:</span> mbedTLS implementation uses `mg_random` as RNG. The `mg_random`
can be overridden by setting `MG_ENABLE_CUSTOM_RANDOM` and defining
your own `mg_random()` implementation.

Usage example:

```c
struct mg_connection *c;
// Connection initialization
// ...

struct mg_tls_opts opts = {0};
opts.cert = get_my_cert();  // Read cert (app specific)
mg_tls_init(c, &opts);
```

## Timer

### struct mg\_timer

```c
struct mg_timer {
  int period_ms;            // Timer period in milliseconds
  int flags;                // Possible flags values below
  void (*fn)(void *);       // Function to call
  void *arg;                // Function argument
  unsigned long expire;     // Expiration timestamp in milliseconds
  struct mg_timer *next;    // Linkage in g_timers list
};

#define MG_TIMER_REPEAT 1   // Call function periodically, otherwise run once
#define MG_TIMER_RUN_NOW 2  // Call immediately when timer is set
```

Timer structure. Describes a software timer. Timer granularity is the same
as the `mg_mgr_poll()` timeout argument in the main event loop.

### mg\_timer\_init()

```c
void mg_timer_init(struct mg_timer *, unsigned long ms, unsigned flags, void (*fn)(void *), void *fn_data);
```

Setup a timer.
- `ms` - an interval in milliseconds
- `flags` - timer flags bitmask, `MG_TIMER_REPEAT` and `MG_TIMER_RUN_NOW`
- `fn` - function to invoke
- `fn_data` - function argument

A timer gets initialized and linked into the internal timers list:

Usage example:
```c
  void timer_func(void *data) {
    (void) data;
  }

  struct mg_timer timer;
  mg_timer_init(&timer, 1000 /* 1sec */, MG_TIMER_REPEAT, timer_func, NULL);
```

### mg\_timer\_free()

```c
void mg_timer_free(struct mg_timer *);
```

Free timer, remove it from the internal timers list.

Usage example:
```c
  struct mg_timer timer;
  // ...

  mg_timer_free(&timer);
```

### mg\_timer\_poll()

```c
void mg_timer_poll(unsigned long uptime_ms);
```

Traverse list of timers, and call them if current timestamp `uptime_ms` is
past the timer's expiration time.

Note, that `mg_mgr_poll` function internally calls `mg_timer_poll`, therefore, in most cases it is unnecessary to call it explicitly.

Usage example:

```c
unsigned long now = mg_millis();
mg_timer_poll(now);
```

## Time

### mg\_time()

```
double mg_time(void);
```

Return current time as UNIX epoch, using `double` value for sub-second accuracy.

Usage example:

```c
double now = mg_time()
```

### mg\_millis()

```c
unsigned long mg_millis(void);
```

Return current uptime in milliseconds.

Usage example:

```c
unsigned long uptime = mg_millis();
```

### mg\_usleep()

```c
void mg_usleep(unsigned long usecs);
```

Block for a given number of microseconds.

Usage example:

```c
mg_usleep(1000000 /* 1 sec */)
```

## String

### mg\_str

In most cases, Mongoose uses `mg_str` struct for string representation rather than NULL-terminated C-strings.

```
struct mg_str {
  const char *ptr;  // pointer to string data
  size_t len;       // string length
};
```

Note, that in general, `ptr` points to non-NULL terminated string, so, do not use functions from C standard library on it.

### mg\_str()

```c
struct mg_str mg_str(const char *s)
```

Create Mongoose string from NULL-terminated C-string. This function doesn't duplicate provided string, but stores pointer within created `mg_str` structure.  

Note, that is you have problems in C++ (constructor shadowing) C++, there is `mg_str_s` synonym for this function.

Usage example:

```c
struct mg_str str = mg_str("Hello, world!);
```

### mg\_str\_n()

```c
struct mg_str mg\_str\_n(const char *s, size_t n);
```

Create Mongoose string from C-string (can be non-NULL terminated). This function doesn't duplicate provided string, but stores pointer within created `mg_str` structure.  

Usage example:

```c
char raw_str[] = "Hello, world!";
struct mg_str str = mg_str_n(raw_str, sizeof(raw_str));
```

### mg\_casecmp()

```c
int mg\_casecmp(const char *s1, const char *s2);
```

Case insensitive compare two NULL-terminated strings.
Return value is 0 if strings are equal, more than zero if first argument is greater then second and less than zero otherwise.

Usage example:

```c
if(mg_casecmp("hello", "HELLO") == 0) {
  // Strings are equal
}
```

### mg\_ncasecmp()

```c
int mg\_ncasecmp(const char *s1, const char *s2, size_t len);
```

Case insensitive compare two C-strings, not more than `len` symbols or until meet `\0` symbol.

Return value is 0 if strings are equal, more than zero if first argument is greater then second and less than zero otherwise.

Usage example:

```c
if(mg_ncasecmp("hello1", "HELLO2", 5) == 0) {
  // Strings are equal
}
```

### mg\_vcmp()

```c
int mg\_vcmp(const struct mg_str *s1, const char *s2);
```

Сompare mongoose string and C-string.

Return value is 0 if strings are equal, more than zero if first argument is greater then second and less than zero otherwise.

Usage example:

```c
struct mg_str str = mg_str("hello");
if(mg_vcmp(str, "hello") == 0) {
  // Strings are equal
}
```

### mg\_vcasecmp()

```c
int mg\_vcasecmp(const struct mg_str *str1, const char *str2);
```

Case insensitive compare mongoose string and C-string.

Return value is 0 if strings are equal, more than zero if first argument is greater then second and less than zero otherwise.

Usage example:

```c
struct mg_str str = mg_str("hello");
if(mg_vcasecmp(str, "HELLO") == 0) {
  // Strings are equal
}
```

### mg\_strcmp()

```c
int mg\_strcmp(const struct mg_str str1, const struct mg_str str2);
```

Сompare two mongoose strings.

Return value is 0 if strings are equal, more than zero if first argument is greater then second and less than zero otherwise.

Usage example:

```c
struct mg_str str1 = mg_str("hello");
struct mg_str str2 = mg_str("hello");

if(mg_strcmp(str1, str2) == 0) {
  // Strings are equal
}
```

### mg\_strdup()

```c
struct mg_str mg\_strdup(const struct mg_str s);
```

Duplicate provided string. Return new string or `MG_NULL_STR` on error.
Note: this function allocates memory for returned string. You may need to free it using `free` function.

Usage example:

```c
struct mg_str str1 = mg_str("hello");
struct mg_str str2 = mg_strdup(str1);
//...
free(str1.ptr);
```


### mg\_strstr()

```c
const char *mg\_strstr(const struct mg_str haystack, const struct mg_str needle)
```

Search for `needle` substring in `haystack` string. Return pointer to `needle` occurrence within `haystack` or `NULL` if not found. 

Usage example:

```c
struct mg_str str = mg_str("Hello, world");
struct mg_str sub_str = mg_str("world");

if(mg_strstr(str, sub_str) != NULL) {
  // Found
}
```
 
### mg\_strstrip()

```c
struct mg_str mg\_strstrip(struct mg_str s)
```

Remove heading and trailing whitespace from mongoose string `s`.

Usage example:

```c
struct mg_str str = mg_strstrip(mg_str("   Hello, world   "));
if(mg_vcmp(str, "Hello, world") == 0) {
  // Strings are equal
}
```

### mg\_globmatch()

```c
bool mg_globmatch(const char *pattern, size_t p_len, const char *s, size_t s_len);
```

Return true if string `s` (limited to `s_len` symbols) matches glob pattern `pattern`, (limited to `p_len` symbols).
The glob pattern matching rules are as follows:

- `?` matches any single character 
- `*` matches zero or more characters except `/`
- `#` matches zero or more characters
- any other character matches itself

Usage example:

```c
char pattern[] = "Hello, ?????";
char s[] = "Hello, world";

if(mg_globmatch(pattern, sizeof(pattern) - 1, s, sizeof(s) - 1)) {
  // Match found
}
```

### mg\_commalist()

```c
bool mg_commalist(struct mg_str *s, struct mg_str *k, struct mg_str *v);
```

Parse string `s`, which is a comma-separated list of entries. An entry could be
either an arbitrary string, which gets stored in `v`, or a `KEY=VALUE` which
gets stored in `k` and `v` respectively.

IMPORTANT: this function modifies `s` by pointing to the next entry.

<kbd>
<img src=".\pics\mg_commalist.png">
</kbd>

Usage example:

```c
struct mg_str k, v, s = mg_str("a=333,b=777");
while (mg_commalist(&s, &k, &v))               // This loop output:
  printf("[%.*s] set to [%.*s]\n",                    // [a] set to [333]
         (int) k.len, k.ptr, (int) v.len, v.ptr);     // [b] set to [777]
```

### mg\_hexdump()

```c
char *mg_hexdump(const void *buf, int len);
```

Hexdump binary data `buf`, `len` into malloc-ed buffer and return it.
It is a caller's responsibility to free() returned pointer.

Usage example:

```c
char arr[] = "\0x1\0x2\0x3";
char *hex = mg_hexdump(arr, sizeof(arr));
LOG(LL_INFO, ("%s", hex)); // Output "0000  01 02 03 00";
free(hex);
```

### mg\_hex()

```c
char *mg_hex(const void *buf, size_t len, char *dst);
```

Hex-encode binary data `buf`, `len` into a buffer `dst` and nul-terminate it.
The output buffer must be at least 2 x `len` + 1 big.
Return value: `dst` pointer. The encoded characters are lowercase.

Usage example:

```c
char data[] = "\x1\x2\x3";
char buf[sizeof(data)*2];
char *hex = mg_hex(data, sizeof(data) - 1, buf);
LOG(LL_INFO, ("%s", hex)); // Output "010203";
free(hex);
```

### mg\_unhex()

```c
void mg_unhex(const char *buf, size_t len, unsigned char *to);
```

Hex-decode string `buf`, `len` into a buffer `to`. The `to` buffer should be
at least `lsn` / 2 big.

Usage example:

```c
char data[] = "010203";
char *buf[sizeof(data)/2];
char *hex = mg_unhex(data, sizeof(data) - 1, buf); // buf is now [1,2,3]
free(hex);
```

### mg\_unhexn()

```c
unsigned long mg_unhexn(const char *s, size_t len);
```

Parse `len` characters of the hex-encoded string `s`, return parsed value.
The maximum value of `len` is the width of the `long` x 2, for example
on 32-bit platforms it is 8.

Usage example:

```c
char data[] = "010203";
char *buf[sizeof(data)/2];
unsigned long val = mg_unhex(data, sizeof(data) - 1); // val is now 123
```


### mg\_asprintf()

```c
int mg_asprintf(char **buf, size_t size, const char *fmt, ...);
```

Print message specified by printf-like format string `fmt` into a buffer
pointed by `buf` of size `size`. If `size` is large enough to hold the whole
message, then a message is stored in `*buf`. If it does not fit, then
a large enough buffer is allocated to hold a message, and `buf` is changed to
point to that buffer. Return value: number of bytes printed.

Usage example:

```c
char buf[1024], *pbuf = &buf;
mg_asprintf(&pbuf, sizeof(buf), "Hello, %s!", "world"); // buf is now "Hello, world!" 
```

### mg\_vasprintf()

```c
int mg_vasprintf(char **buf, size_t size, const char *fmt, va_list ap);
```

Same as `mg_asprintf()` but uses `va_list` argument.

Usage example:
```c
void foo(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  char buf[1024], *pbuf = buf;
  mg_vasprintf(&pbuf, sizeof(buf), fmt, ap);
  va_end(ap);

  printf("%s\n", buf);
}

// ...

foo("Hello, %s!", "world"); // Print "Hello, world!

```

### mg\_to64()

```c
int64_t mg_to64(struct mg_str str);
```

Parse 64-bit integer value held by string `s`.

Usage example:

```c
int64_t val = mg_to64(mg_str("123")); // Val is now 123
```

### mg\_aton()

```c
bool mg_aton(struct mg_str str, struct mg_addr *addr);
```

Parse IP address held by `str` and store it in `addr`. Return true on success.

Usage example:

```c
struct mg_addr addr;
if(mg_aton(mg_str("127.0.0.1"), &addr)) {
  // addr is now binary representation of 127.0.0.1 IP address
}
```

### mg\_ntoa()

```c
char *mg_ntoa(const struct mg_addr *addr, char *buf, size_t len);
```

Stringify IP address `ipaddr` into a buffer `buf`, `len`. Return `buf`.

Usage example:

```c
struct mg_addr addr;
char buf[1024];
// ...
mg_ntoa(&addr, buf, sizeof(buf)); // buf is now contains string representation of addr
```

## Utility

### mg\_call()

```c
void mg_call(struct mg_connection *c, int ev, void *ev_data);
```

Send `ev` event to `c` event handler. This function is useful then implementing your own protocol.

Usage example:

```c
// Mongoose events handler
void handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_WS_MSG) { Websocket Message received
    // ...
    mg_call(c, MG_MY_PROTO_MSG); // Send some custom event (MG_MY_PROTO_MSG) to connection event handler
  }
}
```

### mg\_error()

```c
void mg_error(struct mg_connection *c, const char *fmt, ...);
```

Send `MG_EV_ERROR` to connection event handler with error message formatted using printf semantics.

Usage example:

```c
struct mg_connection *c;
// Connection initialization
// ...

mg_error(c, "Operation failed, error code: %d", errno);
```

### mg\_md5\_init()

```c
void mg_md5_init(mg_md5_ctx *c);
```

Initialize context for MD5 hashing

Usage example:

```c
mg_md5_ctx ctx;
mg_md5_init(&ctx)
```

### mg\_md5\_update()

```c
void mg_md5_update(mg_md5_ctx *c, const unsigned char *data, size_t len);
```
Hash `len` bytes of data pointed by `data` using MD5 algorithm.

Usage example:

```c
mg_md5_ctx ctx;
// Context initialization
// ...

mg_md5_update(&ctx, "data", 4);
mg_md5_update(&ctx, "more data", 9);
```

### mg\_md5\_final() 

```c
void mg_md5_final(mg_md5_ctx *c, unsigned char[16]);
```

Get current MD5 hash for context.

Usage example:

```c
mg_md5_ctx ctx;
// Context initialization
// ...

unsigned char buf[16];
mg_md5_final(&ctx, buf);  // `buf` is now MD5 hash
```

### mg\_sha1\_init()

```c
void mg_sha1_init(mg_sha1_ctx *);
```

Initialize context for calculating SHA1 hash

Usage example:

```c
mg_sha1_ctx ctx;
mg_sha1_init(&ctx);
```

### mg\_sha1\_update()

```c
void mg_sha1_update(mg_sha1_ctx *, const unsigned char *data, size_t len);
```

Hash `len` bytes of `data` using SHA1 algorithm.

Usage example:

```c
mg_sha1_ctx ctx;
// Context initialization
// ...

mg_sha1_update(&ctx, "data", 4);      // hash "data" string
mg_sha1_update(&ctx, "more data", 9); // hash "more data" string
```

### mg\_sha1\_final()

```c
void mg_sha1_final(unsigned char digest[20], mg_sha1_ctx *);
```

Get current SHA1 hash for context.

Usage example:

```c
mg_sha1_ctx ctx;
// Context initialization
// ...

unsigned char buf[20];
mg_sha1_final(buf, &ctx); // `buf` is now SHA1 hash
```

### mg\_base64\_update()

```c
int mg_base64_update(unsigned char p, char *out, int pos);
```

Encode `p` byte to base64 and write result into `out` buffer starting with `pos` position.
Return new position for futher operations.

Usage example:

```c
char buf[10];
mg_base64_update((unsigned char)"a", buf, 0); // Encode "a" into base64 and write it to the beginning of buf 
```

### mg\_base64\_final()

```c
int mg_base64_final(char *buf, int pos);
```

Add base64 finish mark and `\0` symbol to `buf` at `pos` position.

```c
char buf[10];
int pos;
// ...

mg_base64_final(buf, pos);
```

### mg\_base64\_encode()

```c
int mg_base64_encode(const unsigned char *p, int n, char *to);
```

Encode `n` bytes data pointed by `p` using base64 and write result into `to`. Return written symbols number.

Usage example:

```c
char buf[128];
mg_base64_encode((uint8_t *) "abcde", 5, buf); // buf is now YWJjZGU=
```

### mg\_base64\_decode()

```c
int mg_base64_decode(const char *src, int n, char *dst);
```

Decode `n` bytes of base64-ed `src` and write it to `dst`. Return number of written symbols.

Usage example:

```c
char buf[128];
mg_base64_decode("Q2VzYW50YQ==", 12, buf); // buf is now "Cesanta"
```

### mg\_url\_port()

```c
unsigned short mg_url_port(const char *url);
```

Return port for given `url` or `0` if url doesn't contain port and there isn't default port for url protocol.

Usage example:

```c
unsigned short port1 = mg_url_port("htts://myhost.com") // port1 is now 443 (default https port)
unsigned short port2 = mg_url_port("127.0.0.1:567") // port2 is now 567
```

### mg\_url\_is_ssl()

```c
int mg_url_is_ssl(const char *url);
```

Return `0` is given URL uses encrypted scheme and non-zero otherwise.

Usage example:

```c
if(mg_url_is_ssl("https://example.org") == 0) {
  // scheme is encrypted
}
```

### mg\_url\_host()

```c
struct mg_str mg_url_host(const char *url)
```

Extract host name from given URL.

Usage example:

```c
struct mg_str host = mg_url_host("https://my.example.org:1234"); // host is now "my.example.org"
```

### mg\_url\_user()

```c
struct mg_str mg_url_user(const char *url);
```

Extract user name from given URL.

Usage example:

```c
struct mg_str user_name = mg_url_user("https://user@password@my.example.org"); // user_name is now "user" 
```

### mg\_url\_pass()

```c
struct mg_str mg_url_pass(const char *url);
```

Extract user name from given URL.

Usage example:

```c
struct mg_str pwd = mg_url_user("https://user@password@my.example.org"); // pwd is now "password" 
```

### mg\_url\_uri()

```c
const char *mg_url_uri(const char *url);
```

Extract URI from given URL. Return `/` if no URI found.
Note, that function returns pointer within `url`, no need to free() it explicitly.

Usage example:

```c
const char *uri = mg_url_uri("https://example.org/subdir/subsubdir"); // `uri` is now pointer to "subdir/subsubdir" 
```

### mg\_file\_read()

```c
char *mg_file_read(const char *path, size_t *sizep);
```

Read file contents into a nul-terminated malloc-ed string. It is a caller's responsibility to free() a returned pointer. If `sizep` is not NULL, it will
return a file size in bytes. Return `NULL` on error.

Usage example:

```c
size_t file_size;
char* data = mg_file_read("myfile.txt", &file_size);
if (data != NULL) {
  // `data` is now pointer to information readen from file and `file_size` is it size.
}
free(data);
```

### mg\_file\_write()

```c
bool mg_file_write(const char *path, const void *buf, size_t len);
```

Write data to a file, return `true` if written, `false` otherwise.
The write is atomic, i.e. data gets written to a temporary file first, then `rename()-ed` to a destination file name.

Usage example:

```c
char data[] = "Hello, world!";
if(mg_file_write("my_file.txt", data, sizeof(data) - 1)) {
  // File contains "Hello, world!" string 
}
```

### mg\_file\_printf()

```c
int mg_file_printf(const char *path, const char *fmt, ...);
```

Write into a file `path` using `printf()` semantics.
Return `true` on success, `false` otherwise. This function prints data to a temporary in-memory buffer first, then calls `mg_file_write()`.

```c
if(mg_file_printf("my_file.txt", "Hello, %s!", "world") {
  // File contains "Hello, world!" string 
}
```

### mg\_random()

```c
void mg_random(void *buf, size_t len);
```

Fill in buffer `buf`, `len` with random data.

Usage example:
```c
char buf[10];
mg_random(buf, sizeof(buf)); // `buf` is now random bytes
```

### mg\_ntohs()

```c
uint16_t mg_ntohs(uint16_t net);
```

Convert `uint16_t` value to host order.

Usage example:

```c
uint16_t val = mg_ntohs(0x1234);
```

### mg\_ntohl()

```c
uint32_t mg_ntohl(uint32_t net);
```

Convert `uint32_t` value to host order.

Usage example:

```c
uint32_t val = mg_ntohl(0x12345678);
```

### mg\_ntohs()

```c
uint16_t mg_htons(uint16_t net);
```

Convert `uint16_t` value to network order.

Usage example:

```c
uint16_t val = mg_htons(0x1234);
```

### mg\_htonl()

```c
uint32_t mg_ntohl(uint32_t net);
```

Convert `uint32_t` value to network order.

Usage example:

```c
uint32_t val = mg_htonl(0x12345678);
```

### mg\_crc32()

```c
uint32_t mg_crc32(uint32_t crc, const char *buf, size_t len);
```

Calculate CRC32 checksum for a given buffer. An initial `crc` value should be `0`.

Usage example:

```c
char data[] = "hello";
uint32_t crc = mg_crc32(0, data, sizeof(data));
```

### mg\_check\_ip\_acl()

```c
int mg_check_ip_acl(struct mg_str acl, uint32_t remote_ip);
```

Check IPv4 address `remote_ip` against the IP ACL `acl`. Parameters:

- `acl` - an ACL string, e.g. `-0.0.0.0/0,+1.2.3.4`
- `remote_ip` - IPv4 address in network byte order

Return value: 1 if `remote_ip` is allowed, 0 if not, and <0 if `acl` is invalid.

Usage example:

```c
if (mg_check_ip_acl(mg_str("-0.0.0.0/0,+1.2.3.4"), c->peer.ip) != 1) {
  LOG(LL_INFO, ("NOT ALLOWED!"));
}
```

### mg\_url\_decode()

```c
int mg_url_decode(const char *s, size_t n, char *to, size_t to_len, int form);
```

Decode URL-encoded string `s` and write it into `to` buffer. 
If `form` is non-zero, then `+` is decoded as whitespace.

Usage example:

```c
char url[] = "eexample.org%2Ftest"; 
char buf[1024];
mg_url_encode(url, sizeof(url) - 1, buf, sizeof(buf), 0); // buf is now "example.org/test"
```

### mg\_url\_encode

```c
size_t mg_url_encode(const char *s, size_t n, char *buf, size_t len);
```

Encode `s` string to URL-encoding and write encoded string into `buf`.
Return number of characters written to `buf`

Usage example:

```c
char url[] = "example.org/test";
char buf[1024];
mg_url_encode(url, sizeof(url) - 1, buf, sizeof(buf)); // buf is now "example.org%2Ftest"
```

## IO Buffers

IO buffer, described by the `struct mg_iobuf`, is a simple data structure
that insert or delete chunks of data at arbitrary offsets and grow/shrink
automatically.

### struct mg\_iobuf

```c
struct mg_iobuf {
  unsigned char *buf;  // Pointer to stored data
  size_t size;         // Total size available
  size_t len;          // Current number of bytes
};
```

Generic IO buffer. The `size` specifies an allocation size of the data pointed
by `buf`, and `len` specifies number of bytes currently stored.

<kbd>
<img src=".\pics\mg_iobuf.png">
</kbd>

### mg\_iobuf\_init()

```c
int mg_iobuf_init(struct mg_iobuf *io, size_t size);
```

Initialize IO buffer, allocate `size` bytes. Return 1 on success, 0 on allocation failure.

Usage example:

```c
struct mg_iobuf io;
if(mg_iobuf_init(&io)) {
  // io successfully initialized 
}
```

### mg\_iobuf\_resize()

```c
int mg_iobuf_resize(struct mg_iobuf *io, size_t size);
```

Resize IO buffer, set the new size to `size`. The `io->buf` pointer could
change after this, for example if the buffer grows. If `size` is 0, then the
`io->buf` is freed and set to NULL, and both `size` and `len` are set to 0.
Return 1 on success, 0 on allocation failure.

Usage example:

```c
struct mg_iobuf io;
// IO buffer initialization
// ...

if(mg_iobuf_resize(&io, 1024)) {
  // New io size is 1024 bytes
}
```

### mg\_iobuf\_free()

```c
void mg_iobuf_free(struct mg_iobuf *io);
```

Free memory pointed by `io->buf` and set to NULL. Both `size` and `len` are set to 0.

Usage example:

```c
struct mg_iobuf io;
// IO buffer initialization
// ...

// Time to cleanup
mg_iobuf_free(&io);
```

### mg\_iobuf\_add()

```c
size_t mg_iobuf_add(struct mg_iobuf *io, size_t offset, const void *buf, size_t len, size_t align);
```

Insert data buffer `buf`, `len` at offset `offset`. The iobuf gets is expanded if required. The resulting `io->size` is always aligned to the `align` byte boundary - therefore, to avoid memory fragmentation and frequent reallocations, set `align` to a higher value.

Usage example:

```c
struct mg_iobuf io;
mg_iobuf_init(&io, 0);               // Empty buffer
mg_iobuf_add(&io, 0, "hi", 2, 512);  // io->len is 2, io->size is 512
```

<kbd>
<img src=".\pics\mg_iobuf_add.png">
</kbd>

### mg\_iobuf\_del()

```c
size_t mg_iobuf_del(struct mg_iobuf *io, size_t offset, size_t len);
```

Delete `len` bytes starting from `offset`, and shift the remaining bytes. If `len` is greater than `io->len`, nothing happens, so such call is silently ignored.

Usage example:

```c
struct mg_iobuf io;
mg_iobuf_init(&io, 0);               // Empty buffer
mg_iobuf_add(&io, 0, "hi", 2, 512);  // io->len is 2, io->size is 512
// ...
mg_iobuf_del(&io, 0, "hi", 2, 512);  // io->len is 0, io->size is still 512

```

<kbd>
<img src=".\pics\mg_iobug_del.png">
</kbd>

## Logging

Mongoose provides a set of functions and macroses intented for logging. Application can use these functions for its own purposes as well as the rest of Mongoose API.

### LOG() 

```c
#define LOG(level, args) 
```

General way to log is using `LOG` macro. 
`LOG` prints to log only is `MG_ENABLE_LOG` macro defined, otherwise is does nothing.

This macro has two arguments: log level and information to log. Second argument is a printf-alike format string. 

Log levels defined as:
```c
enum { LL_NONE, LL_ERROR, LL_INFO, LL_DEBUG, LL_VERBOSE_DEBUG };
```

Usage example:
```c
LOG(LL_ERROR, ("Hello %s!", "world"));  // Output "Hello, world"
```

### mg\_log\_set()

```c
void mg\_log\_set(const char *spec);
```

Set mongoose logging level. `spec` is a string, containing log level, can be one of the following value:

```
"0" - none
"1" - error
"2" - info
"3" - debug
"4" - verbose debug
```

Usage example:
```c
mg_log_set("3"); // Set log level to debug
```

### mg\_log\_set\_callback() 

```c
void mg_log_set_callback(void (*fn)(const void *, size_t, void *), void *fnd);
```

By default, `LOG` writes to standard output stream (aka `stdout`), but this behaviour can be changes via `mg_log_set_callback`. This function allows to set callback, which called once mongoose (or host application) calls `LOG`

Usage example:

```c
void log_via_printf(const void *buf, size_t len, void *userdata) {
  (void) userdata;
  printf("*.%s", buf, len);
}

// ...
mg_log_set_callback(&log_via_printf, NULL);
```

## Filesystem

Mongoose allows to override file i/o operations in order to support different storages, like programmable flash, no-filesystem devices etc.

In order to accomplish this, Mongoose uses wrappers, instead of using File API directly, and allows to provide application specific wrappers or to use one of standard.

Wrapper is an istance of the following structure with members assigned to actual files functions.

```c
struct mg_fs {
  // Return real path (i.e. with resolved symlinks etc)
  char *(*realpath)(const char *path, char *resolved_path);

  // Return file size and modification time   
  int (*stat)(const char *path, size_t *size, time_t *mtime);
  
  // Enumerates objects in directory
  void (*list)(const char *path, void (*fn)(const char *, void *), void *);

  // Open file
  struct mg_fd *(*open)(const char *path, int flags);

  // Close file
  void (*close)(struct mg_fd *fd);

  // Read file
  size_t (*read)(void *fd, void *buf, size_t len);

  // Write file
  size_t (*write)(void *fd, const void *buf, size_t len);

  // Seek file
  size_t (*seek)(void *fd, size_t offset);
};
```

At this moment Mongoose provides support for two built-in file system
1. Standard posix filesystem (default)
2. Packed filesystem (see below)

Filesystem can be choosen via `fs` member of `mg_http_serve_opts`

Example:
```c
struct mg_http_serve_opts opts;
opts.fs = &mg_fs_posix;
mg_http_serve_dir(c, hm, &opts);
```

## Packed filesystem

Packed filesystem allow to "pack" filesystem into single file, for example, into flashed binary itself. This is useful, for example, for implementation of HTTP-server on devices without filesystem.

In order to use packed filesystem do the following:

1. Compile file test\pack.c:<br>
   cc -o pack pack.c

2. Convert list of files into single .c:<br>
   ./pack file1.data file2.data > fs.c

3. In your application code, you can access files using this function:<br>
   `const char *mg_unpack(const char *file_name, size_t *size)`
   
4. Build your app with fs.c:
   cc -o my_app my_app.c fs.c

Note, that app can also force `mg_http_serve_dir` func to use packed file system:

```c
struct mg_http_serve_opts opts;
opts.fs = &mg_fs_packed; // Set packed ds as a file system
mg_http_serve_dir(c, hm, &opts);
```
