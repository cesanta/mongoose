#pragma once

#include "arch.h"
#include "config.h"
#include "fs.h"
#include "net.h"
#include "str.h"

// A single HTTP header name/value pair.
struct mg_http_header {
  struct mg_str name;   // Header name, e.g. "Content-Type"
  struct mg_str value;  // Header value, e.g. "text/html"
};

// Parsed HTTP request or response.
// Passed as ev_data in MG_EV_HTTP_MSG and MG_EV_HTTP_HDRS.
// For requests:  method="GET", uri="/path", proto="a=1&b=2", proto="HTTP/1.1"
// For responses: method="HTTP/1.0", uri="200", query="",  proto="OK"
struct mg_http_message {
  struct mg_str method, uri, query, proto;             // Request/response line
  struct mg_http_header headers[MG_MAX_HTTP_HEADERS];  // Parsed headers array
  struct mg_str body;     // Request or response body
  struct mg_str head;     // Raw bytes: request/status line + headers, no body
  struct mg_str message;  // Raw bytes: head + body
};

// Options for mg_http_serve_dir() and mg_http_serve_file().
struct mg_http_serve_opts {
  const char *root_dir;       // Root directory for serving files, e.g. "/web"
  const char *ssi_pattern;    // Pattern for Server-Side Include files, e.g. "#.shtml". NULL disables SSI
  const char *extra_headers;  // Extra response headers, e.g. "A: b\r\nC: d\r\n". NULL for none
  const char *mime_types;     // Additional MIME types: "ext1=type1,ext2=type2". NULL for defaults only
  const char *page404;        // Path to a custom 404 page, e.g. "/404.html". NULL for built-in
  struct mg_fs *fs;           // Filesystem to use. NULL defaults to POSIX
};

// A single part of a multipart/form-data body, filled by mg_http_next_multipart().
struct mg_http_part {
  struct mg_str name;      // Form field name, from Content-Disposition: name="..."
  struct mg_str filename;  // Original filename for file uploads, empty for plain fields
  struct mg_str body;      // Part contents, points into the original message buffer
};

// Parses HTTP request or response in buffer s/len into hm.
// Returns number of bytes consumed, 0 if incomplete, -1 on error.
int mg_http_parse(const char *s, size_t len, struct mg_http_message *);

// Returns the byte length of HTTP headers in buf (i.e. offset of the body).
// Returns 0 if headers are incomplete, -1 on error.
int mg_http_get_request_len(const unsigned char *buf, size_t buf_len);

// Sends one chunk in HTTP chunked transfer encoding using printf-style fmt.
// Call with empty fmt ("") to send the terminating zero-length chunk.
void mg_http_printf_chunk(struct mg_connection *cnn, const char *fmt, ...);

// Sends one chunk in HTTP chunked transfer encoding from a raw buffer.
// Call with len=0 to send the terminating zero-length chunk.
void mg_http_write_chunk(struct mg_connection *c, const char *buf, size_t len);

// Creates an HTTP server on url (e.g. "http://0.0.0.0:8000"). Returns the
// listening connection, or NULL on error. Fires MG_EV_HTTP_MSG (full request
// received) and MG_EV_HTTP_HDRS (headers only).
// ev_data for those events is struct mg_http_message *
struct mg_connection *mg_http_listen(struct mg_mgr *, const char *url,
                                     mg_event_handler_t fn, void *fn_data);

// Opens an HTTP client connection to url. Returns the connection, or NULL on
// error. Send the request in the MG_EV_CONNECT handler; read reply in
// MG_EV_HTTP_MSG.
struct mg_connection *mg_http_connect(struct mg_mgr *, const char *url,
                                      mg_event_handler_t fn, void *fn_data);

// Serves files from a directory. Call from an MG_EV_HTTP_MSG handler.
void mg_http_serve_dir(struct mg_connection *, struct mg_http_message *hm,
                       const struct mg_http_serve_opts *);

// Serves a single file at path. Call from an MG_EV_HTTP_MSG handler.
void mg_http_serve_file(struct mg_connection *, struct mg_http_message *hm,
                        const char *path, const struct mg_http_serve_opts *);

// Sends an HTTP response. headers must end with "\r\n", or be "" for none.
// body_fmt is printf-style. Example:
//   mg_http_reply(c, 200, "Content-Type: application/json\r\n",
//                 "{%m:%d}", MG_ESC("temperature"), 123);
void mg_http_reply(struct mg_connection *, int status_code, const char *headers,
                   const char *body_fmt, ...);

// Looks up a request header by name (case-insensitive).
// Returns a pointer to the mg_str value, or NULL if not found.
struct mg_str *mg_http_get_header(struct mg_http_message *, const char *name);

// Extracts a named variable from a query string or form-encoded body (buf).
// Returns the raw (URL-encoded) value as mg_str, or an empty mg_str if not found.
// Not NUL-terminated. Use mg_url_decode() to get a decoded string.
struct mg_str mg_http_var(struct mg_str buf, struct mg_str name);

// URL-decodes variable name from query string or form body into a NUL-terminated buffer.
// Returns the length of the decoded value, or negative if not found / buffer too small.
int mg_http_get_var(const struct mg_str *, const char *name, char *, size_t);

// URL-decodes s/n into to/to_len. Set form=1 to also decode '+' as space.
// Returns number of bytes written, or -1 if to buffer is too small.
int mg_url_decode(const char *s, size_t n, char *to, size_t to_len, int form);

// URL-encodes s/n into buf/len. Returns number of bytes written.
size_t mg_url_encode(const char *s, size_t n, char *buf, size_t len);

// Extracts HTTP credentials into caller-supplied buffers.
// For Basic auth: fills user (buf1) and password (buf2).
// For Bearer auth: fills buf1 with "" and buf2 with the token.
void mg_http_creds(struct mg_http_message *, char *, size_t, char *, size_t);

// Deprecated API, do not expose
long mg_http_upload(struct mg_connection *c, struct mg_http_message *hm,
                    struct mg_fs *fs, const char *dir, size_t max_size);

// Streams the raw request body into dir/name via fs. Non-blocking: installs an
// internal handler and returns immediately. fn is called with NULL on success
// or an error string on failure.
void mg_http_start_upload(struct mg_connection *c, struct mg_http_message *hm,
                          struct mg_str name, struct mg_str dir,
                          struct mg_fs *fs,
                          void (*fn)(struct mg_connection *, const char *));

// Starts an OTA firmware update from an HTTP POST upload.
// Calls mg_ota_begin/write/end internally; fn is called with NULL on success,
// or an error string on failure.
void mg_http_start_ota(struct mg_connection *c, struct mg_http_message *hm,
                       void (*fn)(struct mg_connection *, const char *));

// Sends a 401 Unauthorized response with a Basic Auth WWW-Authenticate challenge.
void mg_http_bauth(struct mg_connection *, const char *user, const char *pass);

// Extracts a named parameter from a header value string s.
// E.g. for s="multipart/form-data; boundary=abc", v="boundary" returns "abc".
struct mg_str mg_http_get_header_var(struct mg_str s, struct mg_str v);

// Iterates over parts of a multipart/form-data body. Start with ofs=0.
// Returns the offset for the next call, or 0 when there are no more parts. Example:
//   size_t ofs = 0;
//   while ((ofs = mg_http_next_multipart(hm->body, ofs, &part)) > 0) { ... }
size_t mg_http_next_multipart(struct mg_str, size_t, struct mg_http_part *);

// Returns the HTTP status code from a parsed response message (e.g. 200, 404).
int mg_http_status(const struct mg_http_message *hm);
