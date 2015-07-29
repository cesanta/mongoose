// Copyright (c) 2004-2013 Sergey Lyubka <valenok@gmail.com>
// Copyright (c) 2013-2014 Cesanta Software Limited
// All rights reserved
//
// This software is dual-licensed: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation. For the terms of this
// license, see <http://www.gnu.org/licenses/>.
//
// You are free to use this software under the terms of the GNU General
// Public License, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// Alternatively, you can license this software under a commercial
// license, as set out in <http://cesanta.com/>.

#ifndef MONGOOSE_HEADER_INCLUDED
#define  MONGOOSE_HEADER_INCLUDED

#define MONGOOSE_VERSION "5.6"

#include <stdarg.h>     // required for va_list
#include <stdio.h>      // required for FILE
#include <stddef.h>     // required for size_t
#include <sys/types.h>  // required for time_t

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// This structure contains information about HTTP request.
struct mg_connection {
  const char *request_method; // "GET", "POST", etc
  const char *uri;            // URL-decoded URI
  const char *http_version;   // E.g. "1.0", "1.1"
  const char *query_string;   // URL part after '?', not including '?', or NULL

  char remote_ip[48];         // Max IPv6 string length is 45 characters
  char local_ip[48];          // Local IP address
  unsigned short remote_port; // Client's port
  unsigned short local_port;  // Local port number

  int num_headers;            // Number of HTTP headers
  struct mg_header {
    const char *name;         // HTTP header name
    const char *value;        // HTTP header value
  } http_headers[30];

  char *content;              // POST (or websocket message) data, or NULL
  size_t content_len;         // Data length

  int is_websocket;           // Connection is a websocket connection
  int status_code;            // HTTP status code for HTTP error handler
  int wsbits;                 // First byte of the websocket frame
  void *server_param;         // Parameter passed to mg_create_server()
  void *connection_param;     // Placeholder for connection-specific data
  void *callback_param;
};

struct mg_server; // Opaque structure describing server instance
enum mg_result { MG_FALSE, MG_TRUE, MG_MORE };
enum mg_event {
  MG_POLL = 100,  // If callback returns MG_TRUE connection closes
                  // after all of data is sent
  MG_CONNECT,     // If callback returns MG_FALSE, connect fails
  MG_AUTH,        // If callback returns MG_FALSE, authentication fails
  MG_REQUEST,     // If callback returns MG_FALSE, Mongoose continues with req
  MG_REPLY,       // If callback returns MG_FALSE, Mongoose closes connection
  MG_RECV,        // Mongoose has received POST data chunk.
                  // Callback should return a number of bytes to discard from
                  // the receive buffer, or -1 to close the connection.
  MG_CLOSE,       // Connection is closed, callback return value is ignored
  MG_WS_HANDSHAKE,  // New websocket connection, handshake request
  MG_WS_CONNECT,  // New websocket connection established
  MG_HTTP_ERROR   // If callback returns MG_FALSE, Mongoose continues with err
};
typedef int (*mg_handler_t)(struct mg_connection *, enum mg_event);

// Websocket opcodes, from http://tools.ietf.org/html/rfc6455
enum {
  WEBSOCKET_OPCODE_CONTINUATION = 0x0,
  WEBSOCKET_OPCODE_TEXT = 0x1,
  WEBSOCKET_OPCODE_BINARY = 0x2,
  WEBSOCKET_OPCODE_CONNECTION_CLOSE = 0x8,
  WEBSOCKET_OPCODE_PING = 0x9,
  WEBSOCKET_OPCODE_PONG = 0xa
};

// Server management functions
struct mg_server *mg_create_server(void *server_param, mg_handler_t handler);
void mg_destroy_server(struct mg_server **);
const char *mg_set_option(struct mg_server *, const char *opt, const char *val);
time_t mg_poll_server(struct mg_server *, int milliseconds);
const char **mg_get_valid_option_names(void);
const char *mg_get_option(const struct mg_server *server, const char *name);
void mg_copy_listeners(struct mg_server *from, struct mg_server *to);
struct mg_connection *mg_next(struct mg_server *, struct mg_connection *);
void mg_wakeup_server(struct mg_server *);
void mg_wakeup_server_ex(struct mg_server *, mg_handler_t, const char *, ...);
struct mg_connection *mg_connect(struct mg_server *, const char *);

// Connection management functions
void mg_send_status(struct mg_connection *, int status_code);
void mg_send_header(struct mg_connection *, const char *name, const char *val);
size_t mg_send_data(struct mg_connection *, const void *data, int data_len);
size_t mg_printf_data(struct mg_connection *, const char *format, ...);
size_t mg_vprintf_data(struct mg_connection *, const char *format, va_list ap);
size_t mg_write(struct mg_connection *, const void *buf, size_t len);
size_t mg_printf(struct mg_connection *conn, const char *fmt, ...);
size_t mg_vprintf(struct mg_connection *conn, const char *fmt, va_list ap);

size_t mg_websocket_write(struct mg_connection *, int opcode,
                          const char *data, size_t data_len);
size_t mg_websocket_printf(struct mg_connection* conn, int opcode,
                           const char *fmt, ...);

void mg_send_file(struct mg_connection *, const char *path, const char *);
void mg_send_file_data(struct mg_connection *, int fd);

const char *mg_get_header(const struct mg_connection *, const char *name);
const char *mg_get_mime_type(const char *name, const char *default_mime_type);
int mg_get_var(const struct mg_connection *conn, const char *var_name,
               char *buf, size_t buf_len);
int mg_get_var_n(const struct mg_connection *conn, const char *var_name,
                 char *buf, size_t buf_len, int n);
int mg_parse_header(const char *hdr, const char *var_name, char *buf, size_t);
int mg_parse_multipart(const char *buf, int buf_len,
                       char *var_name, int var_name_len,
                       char *file_name, int file_name_len,
                       const char **data, int *data_len);


// Utility functions
void *mg_start_thread(void *(*func)(void *), void *param);
char *mg_md5(char buf[33], ...);
int mg_authorize_digest(struct mg_connection *c, FILE *fp);
size_t mg_url_encode(const char *src, size_t s_len, char *dst, size_t dst_len);
int mg_url_decode(const char *src, size_t src_len, char *dst, size_t dst_len, int);
int mg_terminate_ssl(struct mg_connection *c, const char *cert);
int mg_forward(struct mg_connection *c, const char *addr);
void *mg_mmap(FILE *fp, size_t size);
void mg_munmap(void *p, size_t size);


// Templates support
struct mg_expansion {
  const char *keyword;
  void (*handler)(struct mg_connection *);
};
void mg_template(struct mg_connection *, const char *text,
                 struct mg_expansion *expansions);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MONGOOSE_HEADER_INCLUDED
