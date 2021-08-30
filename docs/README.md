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
  mg_mgr_init(&mgr);
  mg_listen(&mgr, "tcp://0.0.0.0:1234", cb, &mgr);
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

### mg\_mgr_init()

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



### mg\_mgr_poll()

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


### mg\_mgr\_free()

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

Create an outbound connection, append this connection to `mgr->conns`.
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

See [examples/multi-threaded](../examples/multi-threaded) for a usage example.

### mg\_mgr_wakeup()

```c
void mg_mgr_wakeup(struct mg_connection *pipe);
```

Wake up an event manager that sleeps in `mg_mgr_poll()` call. This function
must be called from a separate task/thread. Parameters:

- `pipe` - a special connection created by the `mg_mkpipe()` call

Return values: none


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


### mg\_http\_serve\_file()

```c
void mg_http_serve_file(struct mg_connection *, struct mg_http_message *hm,
                        const char *path, struct mg_http_serve_opts *opts);
```

Serve static file. Note that the `extra_headers` must end with `\r\n`. Here
is an example call:

```c
struct mg_http_serve_opts opts = {.mime_types = "png=image/png", 
                                  .extra_headers = "AA: bb\r\nCC: dd\r\n"};
mg_http_serve_file(c, hm, "a.png", &opts);
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

Example - send a simple JSON respose:
  ```c
  mg_http_reply(c, 200, "Content-Type: application/json\r\n", "{\"result\": %d}", 123);
  ```

Example - send JSON response using [mjson](https://github.com/cesanta/mjson) library:
  ```c
  char *json = NULL;
  mjson_printf(mjson_print_dynamic_buf, &json, "{%Q:%d}", "name", 123);
  mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s", json);
  free(json);
  ```

Example - send a 302 redirect:
  ```c
  mg_http_reply(c, 302, "Location: /\r\n", "");
  ```

Example - send error:
  ```c
  mg_http_reply(c, 403, "", "%s", "Not Authorised\n");
  ```

### mg\_http\_get\_header()

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

### mg\_http\_next\_multipart()

```c
// Parameter for mg_http_next_multipart
struct mg_http_part {
  struct mg_str name;      // Form field name
  struct mg_str filename;  // Filename for file uploads
  struct mg_str body;      // Part contents
};

size_t mg_http_next_multipart(struct mg_str body, size_t offset, struct mg_http_part *part);
```

Parse the multipart chunk in the `body` at a given `offset`. An initial
`offset` should be 0. Fill up parameters in the provided `part`, which could be
NULL. Return offset to the next chunk, or 0 if there are no more chunks.

See `form-upload` example for a usage example.


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
void mg_ws_upgrade(struct mg_connection *, struct mg_http_message *,
                   const char *fmt, ...);
```

Upgrade given HTTP connection to Websocket. The `fmt` is a printf-like
format string for the extra HTTP headers returned to the client in a
Websocket handshake. Set `fmt` to `NULL` if no extra headers needs to be passed.


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
                 struct mg_str *data, int qos, bool retain);
```

Publish message `data` to the topic `topic` with given QoS and retain flag.

### mg\_mqtt\_sub()

```c
void mg_mqtt_sub(struct mg_connection *, struct mg_str *topic, int qos);
```

Subscribe to topic `topic` with given QoS.

### mg\_mqtt\_next\_sub()

```c
size_t mg_mqtt_next_sub(struct mg_mqtt_message *msg, struct mg_str *topic, uint8_t *qos, size_t pos);
```

Traverse list of subscribed topics. 
Used to implement MQTT server when `MQTT_CMD_SUBSCRIBE` is received.
Return next position, or 0 when done. Initial position `pos` should be 4. Example:

```c
if (ev == MG_EV_MQTT_CMD) {
  struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
  if (mm->cmd == MQTT_CMD_SUBSCRIBE) {
    size_t pos = 4;
    uint8_t qos;
    struct mg_str topic;
    while ((pos = mg_mqtt_next_sub(mm, &topic, &qos, pos)) > 0) {
      LOG(LL_INFO, ("SUB [%.*s]", (int) topic.len, topic.ptr));
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
int mg_tls_init(struct mg_connection *c, struct mg_tls_opts *opts);
```

Initialise TLS on a given connection.

<span class="badge bg-danger">NOTE:</span> mbedTLS implementation uses `mg_random` as RNG. The `mg_random`
can be overridden by setting `MG_ENABLE_CUSTOM_RANDOM` and defining
your own `mg_random()` implementation.


## Time


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
void mg_timer_init(struct mg_timer *, int ms, int flags, void (*fn)(void *), void *fn_data);
```

Setup a timer.
- `ms` - an interval in milliseconds
- `flags` - timer flags bitmask, `MG_TIMER_REPEAT` and `MG_TIMER_RUN_NOW`
- `fn` - function to invoke
- `fn_data` - function argument

A timer gets initialized and linked into the `g_timers` list:

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



## String

### mg\_globmatch()

```c
bool mg_globmatch(const char *pattern, int plen, const char *s, int n);
```

Return true if string `s`, `n` matches glob pattern `pattern`, `plen`.
The glob pattern matching rules are as follows:

- `?` matches any single character 
- `*` matches zero or more characters except `/`
- `#` matches zero or more characters
- any other character matches itself


### mg\_commalist()

```c
bool mg_commalist(struct mg_str *s, struct mg_str *k, struct mg_str *v);
```

Parse string `s`, which is a comma-separated list of entries. An entry could be
either an arbitrary string, which gets stored in `v`, or a `KEY=VALUE` which
gets stored in `k` and `v` respectively.

IMPORTANT: this function modifies `s` by pointing to the next entry. Usage
example:

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

### mg\_hex()

```c
char *mg_hex(const void *buf, int len, char *dst);
```

Hex-encode binary data `buf`, `len` into a buffer `dst` and nul-terminate it.
The output buffer must be at least 2 x `len` + 1 big.
Return value: `dst` pointer. The encoded characters are lowercase,
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

```c
int64_t mg_to64(const char *s);
```

Parse 64-bit integer value held by string `s`.

### mg\_aton()

```c
bool mg_aton(struct mg_str str, uint32_t *ipaddr);
```

Parse IP address held by `str` and store it in `ipaddr`. Return true on success.


### mg\_ntoa()

```c
char *mg_ntoa(const struct mg_addr *, char *buf, size_t len);
```

Stringify IP address `ipaddr` into a buffer `buf`, `len`. Return `buf`.



## Utility


### mg\_file\_read()

```c
char *mg_file_read(const char *path, size_t *sizep);
```

Read file contents into a nul-terminated malloc-ed string. It is a caller's
responsibility to free() a returned pointer. If `sizep` is not NULL, it will
return a file size in bytes.

### mg\_file\_write()

```c
bool mg_file_write(const char *path, const void *buf, size_t len);
```

Write data to a file, return `true` if written, `false` otherwise.
The write is atomic, i.e. data gets written to a temporary file first,
then `rename()-ed` to a destination file name.


### mg\_file\_printf()

```c
int mg_file_printf(const char *path, const char *fmt, ...);
```

Write into a file `path` using `printf()` semantics.
Return `true` on success, `false` otherwise. This function prints data to
a temporary in-memory buffer first, then calls `mg_file_write()`.


### mg\_random()

```c
void mg_random(void *buf, size_t len);
```

Fill in buffer `buf`, `len` with random data.


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


### mg\_crc32()

```c
uint32_t mg_crc32(uint32_t crc, const uint8_t *buf, size_t len);
```

Calculate CRC32 checksum for a given buffer. An initial `crc` value should
be `0`.

### mg\_check\_ip\_acl()

```c
int mg_check_ip_acl(struct mg_str acl, uint32_t remote_ip);
```

Check IPv4 address `remote_ip` against the IP ACL `acl`. Parameters:

- `acl` - an ACL string, e.g. `-0.0.0.0/0,+1.2.3.4`
- `remote_ip` - IPv4 address in network byte order

Return value: 1 if `remote_ip` is allowed, 0 if not, and <0 if `acl` is
invalid.

Usage example:

```c
  if (mg_check_ip_acl(mg_str("-0.0.0.0/0,+1.2.3.4"), c->peer.ip) != 1) {
    LOG(LL_INFO, ("NOT ALLOWED!"));
  }
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

### mg\_iobuf\_init()

```c
int mg_iobuf_init(struct mg_iobuf *io, size_t size);
```

Initialize IO buffer, allocate `size` bytes. Return 1 on success,
0 on allocation failure.


### mg\_iobuf\_resize()

```c
int mg_iobuf_resize(struct mg_iobuf *io, size_t size);
```

Resize IO buffer, set the new size to `size`. The `io->buf` pointer could
change after this, for example if the buffer grows. If `size` is 0, then the
`io->buf` is freed and set to NULL, and both `size` and `len` are set to 0.
Return 1 on success, 0 on allocation failure.


### mg\_iobuf\_free()

```c
void mg_iobuf_free(struct mg_iobuf *io);
```

Free memory pointed by `io->buf` and set to NULL. Both `size` and `len` are set
to 0.


### mg\_iobuf\_add()

```c
size_t mg_iobuf_add(struct mg_iobuf *io, size_t offset, const void *buf, size_t len, size_t align);
```

Insert data buffer `buf`, `len` at offset `offset`. The iobuf gets
is expanded if required. The resulting `io->size` is always
aligned to the `align` byte boundary - therefore, to avoid memory fragmentation
and frequent reallocations, set `align` to a higher value. Example:

```c
struct mg_iobuf io;
mg_iobuf_init(&io, 0);               // Empty buffer
mg_iobuf_add(&io, 0, "hi", 2, 512);  // io->len is 2, io->size is 512
```

### mg\_iobuf\_del()

```c
size_t mg_iobuf_del(struct mg_iobuf *io, size_t offset, size_t len);
```

Delete `len` bytes starting from `offset`, and shift the remaining
bytes. If `len` is greater than `io->len`, nothing happens,
so such call is silently ignored.

