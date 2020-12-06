# Overview

## Introduction

Mongoose is a networking library for C/C++. It implements event-driven
non-blocking APIs for TCP, UDP, HTTP, WebSocket, MQTT.  It has been designed
for connecting devices and bringing them online. On the market since 2004, used
by vast number of open source and commercial products - it even runs on the
International Space Station!  Mongoose makes embedded network programming fast,
robust, and easy.


## Features

* Cross-platform: works on Linux/UNIX, MacOS, QNX, eCos, Windows, Android,
  iPhone, FreeRTOS, etc
* Supported hardware platforms: TI CC3200, TI MSP432, NRF52, STM32, PIC32, ESP8266, ESP32 and more
* Builtin protocols:
   - plain TCP, plain UDP, SSL/TLS (over TCP, one-way or two-way)
   - HTTP client, HTTP server
   - WebSocket client, WebSocket server
   - MQTT client
   - DNS client, async DNS resolver
* Single-threaded, asynchronous, non-blocking core with simple event-based API
* Native support for [LWIP embedded TCP/IP stack](https://en.wikipedia.org/wiki/LwIP)
* Tiny static and run-time footprint
* Source code is both ISO C and ISO C++ compliant
* Very easy to integrate: just copy
  [mongoose.c](https://raw.githubusercontent.com/cesanta/mongoose/master/mongoose.c) and
  [mongoose.h](https://raw.githubusercontent.com/cesanta/mongoose/master/mongoose.h)
  files to your build tree

## Concept

Mongoose has three basic data structures:

- `struct mg_mgr` - an event manager that holds all active connections
- `struct mg_connection` - describes a connection
- `struct mg_iobuf` - describes data buffer (received or sent data)

Connections could be either listening, outbound or inbound. Outbound
connections are created by the `mg_connect()` call. Listening connections are
created by the `mg_listen()` call. Inbound connections are those accepted by a
listening connection. Each connection is described by a `struct mg_connection`
structure, which has a number of fields. All fields are exposed to the
application by design, to give an application a full visibility into the
Mongoose's internals.

An application that uses mongoose should follow a standard pattern of
event-driven application:

- Declare and initialise an event manager:

```c
struct mg_mgr mgr;
mg_mgr_init(&mgr);
```

- Create connections. For example, a server application should create listening
  connections. When any connection is created (listening or outgoing), an
  event handler function must be specified. An event handler function defines
  connection's behavior.

```c
struct mg_connection *c = mg_http_listen(&mgr, "0.0.0.0:8000", fn, arg);
```

- Create an event loop by calling `mg_mgr_poll()`:

```c
 for (;;) {
   mg_mgr_poll(&mgr, 1000);
 }
```

`mg_mgr_poll()` iterates over all sockets, accepts new connections, sends and
receives data, closes connections and calls event handler functions for the
respective events. 

Since the Mongoose's core is not protected against the concurrent access,
make sure that all `mg_*` API functions are called from the same thread
or RTOS task.

## Send and receive buffers


Each connection has a send and receive buffer:
- `struct mg_connection::send` - data to be sent to a peer
- `struct mg_connection::recv` - data received from a peer

When data arrives, Mongoose appends received data to the `recv` and triggers an
`MG_EV_RECV` event. The user may send data back by calling one of the output
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
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  switch (ev) {
    /* Event handler code that defines behavior of the connection */
    ...
  }
}
```

- `struct mg_connection *c` - a connection that received an event
- `int ev` - an event number, defined in mongoose.h. For example, when data
  arrives on an inbound connection, ev would be `MG_EV_RECV`.
- `void *ev_data` - points to the event-specific data, and it has a different
  meaning for different events. For example, for an `MG_EV_RECV` event,
  `ev_data`
  is an `int *` pointing to the number of bytes received from a remote
  peer and saved into the `c->recv` IO buffer. The exact meaning of `ev_data` is
  described for each event. Protocol-specific events usually have `ev_data`
  pointing to structures that hold protocol-specific information.
- `void *fn_data` - a user-defined pointer for the connection, which is a
  placeholder for application-specific data. Mongoose does not use that
  pointer. An event handler can store any kind of information there

## Events

Below is the list of events trigged by Mongoose, taken as-is from `mongoose.h`.
For each event, a comment describes a meaning of the `ev_data` pointer passed
to an event handler:

```c
enum {
  MG_EV_ERROR,     // Error                        char *error_message
  MG_EV_POLL,      // mg_mgr_poll iteration        unsigned long *millis
  MG_EV_RESOLVE,   // Host name is resolved        NULL
  MG_EV_CONNECT,   // Connection established       NULL
  MG_EV_ACCEPT,    // Connection accepted          NULL
  MG_EV_READ,      // Data received from socket    struct mg_str *
  MG_EV_WRITE,     // Data written to socket       int *num_bytes_written
  MG_EV_CLOSE,     // Connection closed            NULL
  MG_EV_HTTP_MSG,  // HTTP request/response        struct mg_http_message *
  MG_EV_WS_OPEN,    // Websocket handshake done     NULL
  MG_EV_WS_MSG,     // Websocket message received   struct mg_ws_message *
  MG_EV_MQTT_MSG,   // MQTT message                 struct mg_mqtt_message *
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
example, `is_drainig` flag, if set by an application, tells Mongoose to send
the remaining data to peer, and when everything is sent, close the connection.

User-changeable flags are: `is_hexdumping`, `is_drainig`, `is_closing`.

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
files to the application's source tree.

The `mongoose.c` and `mongoose.h` files are, actually, an amalgamation -
a non-amalgamated sources can be found at https://github.com/cesanta/mongoose/tree/master/src

Mongoose source code uses a bunch of build constants defined at
https://github.com/cesanta/mongoose/blob/master/src/config.h, together with
their default values.

In order to change the constant during build time, use the `-D
<PREPROCESSOR_FLAG>` compiler option. For example, to disable both MQTT,
compile the application `my_app.c` like this (assumed UNIX system):

```sh
$ cc my_app.c mongoose.c -D MG_MQTT_ENABLE=0
```

Here is a list of build constants and their default values:
| Name | Default | Description |
| ---- | ------- | ----------- |
|`MG_ENABLE_LWIP` | 0 | Use LWIP low-level API instead of BSD sockets |
|`MG_ENABLE_SOCKET` | 1 | Use BSD socket low-level API |
|`MG_ENABLE_MBEDTLS` | 0 | Enable Mbed TLS library |
|`MG_ENABLE_OPENSSL` | 0 | Enable OpenSSL library |
|`MG_ENABLE_FS` | 1 | Enable API that use filesystem, like `mg_http_send_file()` |
|`MG_ENABLE_IPV6` | 0 | Enable IPv6 |
|`MG_ENABLE_LOG` | 1 | Enable `LOG()` macro |
|`MG_ENABLE_MD5` | 0 | Use native MD5 implementation |
|`MG_ENABLE_DIRECTORY_LISTING` | 0 | Enable directory listing for HTTP server |
|`MG_ENABLE_HTTP_DEBUG_ENDPOINT` | 0 | Enable `/debug/info` debug URI |
|`MG_ENABLE_SOCKETPAIR` | 0 | Enable `mg_socketpair()` for multi-threading |
|`MG_IO_SIZE` | 512 | Granularity of the send/recv IO buffer growth |
|`MG_MAX_RECV_BUF_SIZE` | (3 * 1024 * 1024) | Maximum recv buffer size |
|`MG_MAX_HTTP_HEADERS` | 40 | Maximum number of HTTP headers |

## Minimal example

This example is a simple static HTTP server that serves current directory:

```c
#include "mongoose.h"

static const char *s_web_root_dir = ".";
static const char *s_listening_address = "http://localhost:8000";

static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) mg_http_serve_dir(c, ev_data, s_web_root_dir);
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, s_listening_address, cb, &mgr);
  for (;;) mg_mgr_poll(&mgr, 1000);
  mg_mgr_free(&mgr);
  return 0;
}
```
