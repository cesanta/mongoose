# User Guide


## Introduction

Mongoose is a networking library for C/C++. It implements an event-driven,
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

1. Declare and initialise an event manager:

  ```c
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  ```

2. Create connections. For example, a server application should create listening
  connections. When any connection is created (listening or outgoing), an
  event handler function must be specified. An event handler function defines
  connection's behavior.

  ```c
  struct mg_connection *c = mg_http_listen(&mgr, "0.0.0.0:8000", fn, arg);
  ```

3. Create an event loop by calling `mg_mgr_poll()`:

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
  arrives on an inbound connection, ev would be `MG_EV_RECV`
- `void *ev_data` - points to the event-specific data, and it has a different
  meaning for different events. For example, for an `MG_EV_RECV` event,
  `ev_data`
  is an `int *` pointing to the number of bytes received from a remote
  peer and saved into the `c->recv` IO buffer. The exact meaning of `ev_data` is
  described for each event. Protocol-specific events usually have `ev_data`
  pointing to structures that hold protocol-specific information
- `void *fn_data` - a user-defined pointer for the connection, which is a
  placeholder for application-specific data

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
|`MG_ENABLE_SSI` | 0 | Enable serving SSI files by `mg_http_serve_dir()` |
|`MG_IO_SIZE` | 512 | Granularity of the send/recv IO buffer growth |
|`MG_MAX_RECV_BUF_SIZE` | (3 * 1024 * 1024) | Maximum recv buffer size |
|`MG_MAX_HTTP_HEADERS` | 40 | Maximum number of HTTP headers |

NOTE: `MG_IO_SIZE` controls the maximum UDP message size, see
https://github.com/cesanta/mongoose/issues/907 for details. If application
uses large UDP messages, increase the `MG_IO_SIZE` limit accordingly.

## Minimal HTTP server

This example is a simple static HTTP server that serves current directory:

```c
#include "mongoose.h"

static const char *s_web_root_dir = ".";
static const char *s_listening_address = "http://localhost:8000";

static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  struct mg_http_serve_opts opts = {.root_dir = s_web_root_dir};
  if (ev == MG_EV_HTTP_MSG) mg_http_serve_dir(c, ev_data, &opts);
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

## Minimal TCP echo server

This example is a simple TCP echo server that listens on port 1234:

```c
#include "mongoose.h"

static const char *s_listening_address = "tcp://0.0.0.0:1234";

static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_RECV) {
    mg_send(c, c->recv.buf, c->recv.len);     // Echo received data back
    mg_iobuf_delete(&c->recv, c->recv.len);   // And discard it
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_listen(&mgr, s_listening_address, cb, &mgr);
  for (;;) mg_mgr_poll(&mgr, 1000);
  mg_mgr_free(&mgr);
  return 0;
}
```


# API Reference

## Core

### struct mg_mgr

```c
struct mg_mgr {
  struct mg_connection *conns;  // List of active connections
  struct mg_connection *dnsc;   // DNS resolver connection
  const char *dnsserver;        // DNS server URL
  int dnstimeout;               // DNS resolve timeout in milliseconds
};
```
Event management structure that holds a list of active connections, together
with some housekeeping information.


### struct mg\_connection

```c
struct mg_connection {
  struct mg_connection *next;  // Linkage in struct mg_mgr :: connections
  struct mg_mgr *mgr;          // Our container
  struct mg_addr peer;         // Remote peer address
  void *fd;                    // Connected socket, or LWIP data
  struct mg_iobuf recv;        // Incoming data
  struct mg_iobuf send;        // Outgoing data
  mg_event_handler_t fn;       // User-specified event handler function
  void *fn_data;               // User-speficied function parameter
  mg_event_handler_t pfn;      // Protocol-specific handler function
  void *pfn_data;              // Protocol-specific function parameter
  char label[32];              // Arbitrary label
  void *tls;                   // TLS specific data
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

A connection - either a listening connection, or an accepted connection,
or an outbout connection.

### mg\_mgr_init()

```c
void mg_mgr_init(struct mg_mgr *);
```

Initialise event manager structure: set a list of active connections to NULL,
set DNS server and timeout to their default values, etc.


### mg\_mgr_poll()

```c
void mg_mgr_poll(struct mg_mgr *mgr, int ms);
```

Perform a single poll iteration. For each connection in the `mgr->conns` list,
- See if there is incoming data. If it is, read it into the `c->recv` buffer, send `MG_EV_RECV` event
- See if there is data in the `c->send` buffer, and write it, send `MG_EV_WRITE` event
- If a connection is listening, accept an incoming connection if any, and send `MG_EV_ACCEPT` event to it
- Send `MG_EV_POLL` event

Each connection has two event handler functions: `c->fn` and `c->pfn`.  The
`c->fn` is a user-specified event handler function. The `c->pfn` is a
protocol-specific handler function that is set implicitly. For example, a
`mg_http_listen()` sets `c->pfn` to a Mongoose's HTTP event handler.  A
protocol-specific handler is called before user-specific handler.  It parses
incoming data and may invoke protocol-specific events like `MG_EV_HTTP_MSG`.


### mh\_mgr\_free()

```c
void mg_mgr_free(struct mg_mgr *mgr);
```

Close all connections, and free all resources.


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

### mg\_connect()

```c
struct mg_connection *mg_connect(struct mg_mgr *mgr, const char *url,
                                 mg_event_handler_t fn, void *fn_data);
```

Create an outbout connection, append this connection to `mgr->conns`.
- `url` - specifies remote IP address/port to connect to, e.g. `http://a.com`
- `fn` - an event handler function
- `fn_data` - an arbitrary pointer, which will be passed as `fn_data` when an
  event handler is called. This pointer is also stored in a connection
  structure as `c->fn_data`

Return value: created connection, or `NULL` on error.


### mg\_send()

```c
int mg_send(struct mg_connection *c, const void *data, size_t size);
```

Append `data` of size `size` to the `c->send` buffer. Return number of bytes
appended.

Note: this function does not push data to the network! It only appends data to
the output buffer.  The data is being sent when `mg_mgr_poll()` is called. If
`mg_send()` is called multiple times, the output buffer grows.


### mg\_printf()

```c
int mg_printf(struct mg_connection *, const char *fmt, ...);

```

Same as `mg_send()`, but formats data using `printf()` semantics. Return
number of bytes appended to the output buffer.


### mg\_vprintf()

```c
int mg_vprintf(struct mg_connection *, const char *fmt, va_list ap);
```

Same as `mg_printf()`, but takes `va_list` argument as a parameter.


### mg\_socketpair()

```c
bool mg_socketpair(int *blocking, int *non_blocking);
```

Create a socket pair for exchanging data in multi-threaded environment. The
`blocking` socket is blocking - it should be passed to the processing task.
The `non_blocking` socket is non blocking, it should be used by an event
handler function. Return value: true on success, false on error.


## IO buffers

### struct mg\_iobuf

```c
struct mg_iobuf {
  unsigned char *buf;
  size_t size, len;
};
```

Generic IO buffer. The `size` specifies an allocation size of the data pointed
by `buf`, and `len` specifies number of bytes currently stored.

### mg\_iobuf\_init()

```c
void mg_iobuf_init(struct mg_iobuf *io, size_t size);
```

Initialise IO buffer, allocate `size` bytes.


### mg\_iobuf\_resize()

```c
void mg_iobuf_resize(struct mg_iobuf *io, size_t size);
```

Resize IO buffer, set the new size to `size`. The `io->buf` pointer could
change after this, for example if the buffer grows. If `size` is 0, then the
`io->buf` is freed and set to NULL, and both `size` and `len` are set to 0.


### mg\_iobuf\_free()

```c
void mg_iobuf_free(struct mg_iobuf *io);
```

Free memory pointed by `io->buf` and set to NULL. Both `size` and `len` are set
to 0.


### mg\_iobuf\_append()

```c
size_t mg_iobuf_append(struct mg_iobuf *io, const void *data, size_t data_size, size_t granularity);
```

Append `data` bytes of size `data_size` to the end of the buffer. The buffer
is expanded if `data_size` is greater than `io->size - io->len`. If that
happens, the `io->buf` can change. The resulting `io->size` is always
set to the `granularity` byte boundary. Example:

```c
struct mg_iobuf io;
mg_iobuf_init(&io, 0);                // Empty buffer
mg_iobuf_append(&io, "hi", 2, 1024);  // io->len is 2, io->size is 1024
```

### mg\_iobuf\_delete()

```c
size_t mg_iobuf_delete(struct mg_iobuf *io, size_t len);
```

Discard `len` bytes from the beginning of the buffer, and shift the remaining
bytes to the beginning. If `len` is greater than `io->len`, nothing happens,
so such call is silently ignored.


## HTTP

### struct mg\_http\_header

```c
struct mg_http_header {
  struct mg_str name;
  struct mg_str value;
};
```

### struct mg\_http\_message

```c
struct mg_http_message {
  //        GET /foo/bar/baz?aa=b&cc=ddd HTTP/1.1
  // method |-| |----uri---| |--query--| |proto-|

  struct mg_str method, uri, query, proto;  // Request/response line
  struct mg_http_header headers[MG_MAX_HTTP_HEADERS];  // Headers
  struct mg_str body;                       // Body
  struct mg_str message;                    // Request line + headers + body
};
```

### mg\_http\_listen()

```c
struct mg_connection *mg_http_listen(struct mg_mgr *, const char *url,
                                     mg_event_handler_t fn, void *fn_data);
```

Create HTTP listener.

- `url` - specifies local IP address and port to listen on, e.g. `http://0.0.0.0:8000`
- `fn` - an event handler function
- `fn_data` - an arbitrary pointer, which will be passed as `fn_data` when an
  event handler is called. This pointer is also stored in a connection
  structure as `c->fn_data`


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




### mg\_http\_get\_request\_len()

```c
int mg_http_get_request_len(const unsigned char *buf, size_t buf_len);
```

Return value: -1 on error,
0 if a message is incomplete, or the length of request. The length of
request is a number of bytes till the end of HTTP headers. It does not include
length of HTTP body.


### mg\_http\_parse()

```c
int mg_http_parse(const char *s, size_t len, struct mg_http_message *hm);
```

Parse string `s`, `len` into a structure `hm`. Return request length - see
`mg_http_get_request_len()`.


### mg\_http\_printf\_chunk()

```
void mg_http_printf_chunk(struct mg_connection *cnn, const char *fmt, ...);
```

Write a chunk of data in chunked encoding format, using `printf()` semantic.


### mg\_http\_write\_chunk()

```c
void mg_http_write_chunk(struct mg_connection *c, const char *buf, size_t len);
```

Write a chunk of data in chunked encoding format.


### mg\_serve\_dir()

```c
struct mg_http_serve_opts {
  const char *root_dir;     // Web root directory, must be non-NULL
  const char *ssi_pattern;  // SSI filename pattern, e.g. #.shtml
};
void mg_http_serve_dir(struct mg_connection *, struct mg_http_message *hm,
                       const struct mg_http_serve_opts *opts);
```

Serve static files according to the given options. Note that in order to
enable SSI, set a `-DMG_ENABLE_SSI=1` build flag.


### mg\_serve\_file()

```c
void mg_http_serve_file(struct mg_connection *, struct mg_http_message *hm,
                        const char *path, const char *mimetype,
                        const char *extra_headers);
```

Serve static file. Note that the `extra_headers` must end with `\r\n`. Here
is an example call:

```c
mg_http_serve_file(c, hm, "a.png", "image/png", "AA: bb\r\nCC: dd\r\n");
```


### mg\_http\_reply()

```c
void mg_http_reply(struct mg_connection *c, int status_code, const char *headers,
                   const char *body_fmt, ...);
```

Send simple HTTP response using `printf()` semantic. This function formats
response body according to a `body_fmt`, and automatically appends a correct
`Content-Length` header. Extra headers could be passed via `headers`
parameter.

- `status_code` - an HTTP response code
- `headers` - extra headers, default NULL. If not NULL, must end with `\r\n`
- `fmt` - a format string for the HTTP body, in a printf semantics


### mg\_http\_header()

```c
struct mg_str *mg_http_get_header(struct mg_http_message *, const char *name);
```

Return value of HTTP header, or NULL if not found.

### mg\_http\_get\_var()

```c
int mg_http_get_var(const struct mg_str *, const char *name, char *buf, int len);
```

Decode HTTP variable `name` into a given buffer. Return length of decoded
variable. Zero or negative value means error.

### mg\_url\_decode()

```c
int mg_url_decode(const char *s, size_t n, char *to, size_t to_len, int form);
```


URL-decode string `s`, `n` unto a buffer `buf`, `len`. Return decoded length.
If `form` is non-zero, then `+` is decoded as whitespace.


### mg\_http\_creds()

```c
void mg_http_creds(struct mg_http_message *, char *user, int userlen, char *pass, int passlen);
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


### mg\_http\_match\_uri()

```c
bool mg_http_match_uri(const struct mg_http_message *, const char *glob);
```

Return true if HTTP request matches a given glob pattern; false otherwise.

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

### mg\_http\_bauth()

```c
void mg_http_bauth(struct mg_connection *, const char *user, const char *pass);
```

Write a Basic `Authorization` header to the output buffer.


## Websocket

### struct mg\_ws\_message

```c
struct mg_ws_message {
  struct mg_str data;
  uint8_t flags;  // Websocket message flags
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


### mg\_ws\_upgrade()

```c
void mg_ws_upgrade(struct mg_connection *, struct mg_http_message *);
```

Upgrade given HTTP connection to Websocket.


### mg\_ws\_send()

```c
size_t mg_ws_send(struct mg_connection *, const char *buf, size_t len, int op);
```

Send `buf`, `len` to the websocket peer. `op` is the Websocket message type:

```c
#define WEBSOCKET_OP_CONTINUE 0
#define WEBSOCKET_OP_TEXT 1
#define WEBSOCKET_OP_BINARY 2
#define WEBSOCKET_OP_CLOSE 8
#define WEBSOCKET_OP_PING 9
#define WEBSOCKET_OP_PONG 10
```

## MQTT

### struct mg\_mqtt\_opts

```c
struct mg_mqtt_opts {
  struct mg_str client_id;
  struct mg_str will_topic;
  struct mg_str will_message;
  uint8_t qos;         // Quality of service
  bool will_retain;    // Retain last will
  bool clean;          // Use clean session, 0 or 1
  uint16_t keepalive;  // Keep-alive timer in seconds
};
```

### struct mg\_mqtt\_message

```c
struct mg_mqtt_message {
  struct mg_str topic;
  struct mg_str data;
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


### mg\_mqtt\_pub()

```c
void mg_mqtt_pub(struct mg_connection *, struct mg_str *topic,
                 struct mg_str *data);
```

Publish message `data` to the topic `topic`.


### mg\_mqtt\_sub()

```c
void mg_mqtt_sub(struct mg_connection *, struct mg_str *topic);
```

Subscribe to topic `topic`.


## TLS

### struct mg\_tls\_opts

```c
struct mg_tls_opts {
  const char *ca;        // CA certificate file. For both listeners and clients
  const char *cert;      // Certificate
  const char *certkey;   // Certificate key
  const char *ciphers;   // Cipher list
  const char *srvname;   // If not NULL, enables server name verification
};
```

Describes TLS parameters


### mg\_tls\_init()

```c
int mg_tls_init(struct mg_connection *c, struct mg_tls_opts *opts);
```

Turns on TLS on a given connection.


## Timers


### struct mg\_timer

```c
struct mg_timer {
  int period_ms;            // Timer period in milliseconds
  int flags;                // Possible flags values below
#define MG_TIMER_REPEAT 1   // Call function periodically, otherwise run once
#define MG_TIMER_RUN_NOW 2  // Call immediately when timer is set
  void (*fn)(void *);       // Function to call
  void *arg;                // Function agrument
  unsigned long expire;     // Expiration timestamp in milliseconds
  struct mg_timer *next;    // Linkage in g_timers list
};
```

Timer structure.

### mg\_timer\_init()

```c
void mg_timer_init(struct mg_timer *, int ms, int flags, void (*fn)(void *), void *fn_data);
```

Setup a timer.
- `ms` - an interval in milliseconds
- `flags` - timer flags bitmask, `MG_TIMER_REPEAT` and `MG_TIMER_RUN_NOW`
- `fn` - function to invoke
- `fn_data` - function argument

A timer gets initialised and linked into the `g_timers` list:

```c
struct mg_timer *g_timers;
```

### mg\_timer\_free()

```c
void mg_timer_free(struct mg_timer *);
```

Free timer, remove it from the `g_timers` list.

### mg\_timer\_poll()

```c
void mg_timer_poll(unsigned long uptime_ms);
```

Traverse list of timers, and call them if current timestamp `uptime_ms` is
past the timer's expiration time.

## Utility functions


### mg\_file\_read()

```c
char *mg_file_read(const char *path);
```

Read file contents into a nul-terminated malloc-ed string. It is a caller's
responsibility to free() a returned pointer.

### mg\_file\_size()

```c
size_t mg_file_size(const char *path);
```

Return file size, or 0 on failure. Empty files also report 0 length.


### mg\_file\_write()

```c
int mg_file_write(const char *path, const char *fmt, ...);
```

Write into a file `path` using `printf()` semantics. Return number of bytes written.


### mg\_random()

```c
void mg_random(void *buf, size_t len);
```

Fill in buffer `buf`, `len` with random data.


### mg\_globmatch()

```c
bool mg_globmatch(const char *pattern, int plen, const char *s, int n);
```

Return true if string `s`, `n` matches glob pattern `pattern`, `plen`.
The glob pattern matching rules are as follows:

- `?` matches any single character 
- `*` matches zero or more characters except `/`
- `#` matches zero or more characters
- any other caracter matches itself


### mg\_next\_comma\_entry()

```c
bool mg_next_comma_entry(struct mg_str *s, struct mg_str *k, struct mg_str *v);
```

Parse string `s`, which is a comma-separated list of entries. An entry could be
either an arbitrary string, which gets stored in `v`, or a `KEY=VALUE` which
gets stored in `k` and `v` respectively.

IMPORTANT: this function modifies `s` by pointing to the next entry. Usage
example:

```c
struct mg_str k, v, s = mg_str("a=333,b=777");
while (mg_next_comma_entry(&s, &k, &v))               // This loop output:
  printf("[%.*s] set to [%.*s]\n",                    // [a] set to [333]
         (int) k.len, k.ptr, (int) v.len, v.ptr);     // [b] set to [777]
```

### mg\_ntohs()

```c
uint16_t mg_ntohs(uint16_t net);
```

Convert `uint16_t` value to host order.


### mg\_ntohl()

```c
uint32_t mg_ntohl(uint32_t net);
```

Convert `uint32_t` value to host order.


### mg\_hexdump()

```c
char *mg_hexdump(const void *buf, int len);
```

Hexdump binary data `buf`, `len` into malloc-ed buffer and return it.
It is a caller's responsibility to free() returned pointer.

### mg\_hex()

```c
char *mg_hex(const void *buf, int len, char *dst);
```

Hex-encode binary data `buf`, `len` into a buffer `dst` and nul-terminate it.
The output buffer must be at least 2 x `len` + 1 big.
Return value: `dst` pointer. The encoded characters are lowecase,
for example `mg_hex("hi", 2, buf)` outputs `6869` and 0 byte, 5 bytes overall.


### mg\_unhex()

```c
void mg_unhex(const char *buf, int len, unsigned char *to);
```

Hex-decode string `buf`, `len` into a buffer `to`. The `to` buffer should be
at least `lsn` / 2 big.


### mg\_unhexn()

```c
unsigned long mg_unhexn(const char *s, int len);
```

Parse `len` characters of the hex-encoded string `s`, return parsed value.
The maximum value of `len` is the width of the `long` x 2, for example
on 32-bit platforms it is 8.


### mg\_asprintf()

```c
int mg_asprintf(char **buf, size_t size, const char *fmt, ...);
```

Print message specified by printf-like format string `fmt` into a buffer
pointed by `buf` of size `size`. If `size` is large enough to hold the whole
message, then a message is stored in `*buf`. If it does not fit, then
a large enough buffer is allocated to hold a message, and `buf` is changed to
point to that buffer. Return value: number of bytes printed.


### mg\_vasprintf()

```c
int mg_vasprintf(char **buf, size_t size, const char *fmt, va_list ap);
```

Same as `mg_asprintf()` but uses `va_list` argument.


### mg\_to64()

```
int64_t mg_to64(const char *s);
```

Parse 64-bit integer value held by string `s`.

### mg\_aton()

```
bool mg_aton(struct mg_str str, uint32_t *ipaddr);
```

Parse IP address held by `str` and store it in `ipaddr`. Return true on success.


### mg\_ntoa()

```c
char *mg_ntoa(const struct mg_addr *, char *buf, size_t len);
```

Stringify IP address `ipaddr` into a buffer `buf`, `len`. Return `buf`.

### mg\_time()

```
double mg_time(void);
```

Return current time as UNIX epoch, using `double` value for sub-second accuracy.


### mg\_millis()

```c
unsigned long mg_millis(void);
```

Return current uptime in milliseconds.


### mg\_usleep()

```c
void mg_usleep(unsigned long usecs);
```

Block for a given number of microseconds.
