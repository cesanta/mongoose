// Copyright (c) 2004-2013 Sergey Lyubka <valenok@gmail.com>
// Copyright (c) 2013 Cesanta Software Limited
// All rights reserved
//
// This library is dual-licensed: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation. For the terms of this
// license, see <http://www.gnu.org/licenses/>.
//
// You are free to use this library under the terms of the GNU General
// Public License, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// Alternatively, you can license this library under a commercial
// license, as set out in <http://cesanta.com/products.html>.
//
// NOTE: Detailed API documentation is at http://cesanta.com/docs.html

#ifndef MONGOOSE_HEADER_INCLUDED
#define  MONGOOSE_HEADER_INCLUDED

#include <stdio.h>      // required for FILE
#include <stddef.h>     // required for size_t

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// This structure contains information about HTTP request.
struct mg_connection {
  const char *request_method; // "GET", "POST", etc
  const char *uri;            // URL-decoded URI
  const char *http_version;   // E.g. "1.0", "1.1"
  const char *query_string;   // URL part after '?', not including '?', or NULL
  long remote_ip;             // Client's IP address
  int remote_port;            // Client's port
  int is_ssl;                 // 1 if SSL-ed, 0 if not
  int num_headers;            // Number of HTTP headers
  struct mg_header {
    const char *name;         // HTTP header name
    const char *value;        // HTTP header value
  } http_headers[64];         // Maximum 64 headers

  void *server_param;         // Parameter passed to mg_add_uri_handler()
  void *connection_param;     // Placeholder for connection-specific data
};

struct mg_server; // Opaque structure describing server instance
typedef int (*mg_uri_handler_t)(struct mg_connection *);
typedef int (*mg_error_handler_t)(struct mg_connection *, int http_error_code);

// Server management functions
struct mg_server *mg_create_server(void *server_param);
void mg_destroy_server(struct mg_server **);
const char *mg_set_option(struct mg_server *, const char *opt, const char *val);
void mg_poll_server(struct mg_server *, int milliseconds);
void mg_add_uri_handler(struct mg_server *, const char *uri, mg_uri_handler_t);
#if 0
void mg_set_error_handler(struct mg_server *, mg_error_handler_t);
void mg_set_log_handler(struct mg_server*, int (*)(struct mg_connection*, int));
#endif
const char **mg_get_valid_option_names(void);
const char *mg_get_option(const struct mg_server *server, const char *name);

// Websocket functions
void mg_websocket_handshake(struct mg_connection *);
int mg_websocket_read(struct mg_connection *, int *bits, char **data);
int mg_websocket_write(struct mg_connection* conn, int opcode,
                       const char *data, size_t data_len);

// Connection management functions
int mg_write(struct mg_connection *, const void *buf, int len);
int mg_printf(struct mg_connection *, const char *fmt, ...);

#if 0
void mg_send_file(struct mg_connection *, const char *path);
int mg_read(struct mg_connection *, void *buf, int len);
const char *mg_get_header(const struct mg_connection *, const char *name);
int mg_get_var(const char *data, size_t data_len,
               const char *var_name, char *dst, size_t dst_len);
int mg_get_cookie(const char *cookie, const char *var_name,
                  char *buf, size_t buf_len);
const char *mg_get_mime_type(const char *file_name);

// Utility functions
int mg_start_thread(void *(*func)(void *), void *param);
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MONGOOSE_HEADER_INCLUDED
