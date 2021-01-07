#pragma once

#include "config.h"
#include "net.h"
#include "str.h"

struct mg_http_header {
  struct mg_str name;
  struct mg_str value;
};

struct mg_http_message {
  //        GET /foo/bar/baz?aa=b&cc=ddd HTTP/1.1
  // method |-| |----uri---| |--query--| |proto-|

  struct mg_str method, uri, query, proto;             // Request/response line
  struct mg_http_header headers[MG_MAX_HTTP_HEADERS];  // Headers
  struct mg_str body;                                  // Body
  struct mg_str message;  // Request line + headers + body
};

// Parameter for mg_http_serve_dir()
struct mg_http_serve_opts {
  const char *root_dir;     // Web root directory, must be non-NULL
  const char *ssi_pattern;  // SSI filename pattern, e.g. #.shtml
};

int mg_http_parse(const char *s, size_t len, struct mg_http_message *);
int mg_http_get_request_len(const unsigned char *buf, size_t buf_len);
void mg_http_printf_chunk(struct mg_connection *cnn, const char *fmt, ...);
void mg_http_write_chunk(struct mg_connection *c, const char *buf, size_t len);
struct mg_connection *mg_http_listen(struct mg_mgr *, const char *url,
                                     mg_event_handler_t fn, void *fn_data);
struct mg_connection *mg_http_connect(struct mg_mgr *, const char *url,
                                      mg_event_handler_t fn, void *fn_data);
void mg_http_serve_dir(struct mg_connection *, struct mg_http_message *hm,
                       struct mg_http_serve_opts *);
void mg_http_serve_file(struct mg_connection *, struct mg_http_message *,
                        const char *, const char *mime, const char *headers);
void mg_http_reply(struct mg_connection *, int status_code, const char *headers,
                   const char *body_fmt, ...);
struct mg_str *mg_http_get_header(struct mg_http_message *, const char *name);
void mg_http_event_handler(struct mg_connection *c, int ev);
int mg_http_get_var(const struct mg_str *, const char *name, char *, int);
int mg_url_decode(const char *s, size_t n, char *to, size_t to_len, int form);
void mg_http_creds(struct mg_http_message *, char *user, int, char *pass, int);
bool mg_http_match_uri(const struct mg_http_message *, const char *glob);
int mg_http_upload(struct mg_connection *, struct mg_http_message *hm,
                   const char *dir);
void mg_http_bauth(struct mg_connection *, const char *user, const char *pass);
