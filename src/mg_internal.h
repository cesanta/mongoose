/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_MONGOOSE_SRC_INTERNAL_H_
#define CS_MONGOOSE_SRC_INTERNAL_H_

#include "common/mg_mem.h"

#ifndef MBUF_REALLOC
#define MBUF_REALLOC MG_REALLOC
#endif

#ifndef MBUF_FREE
#define MBUF_FREE MG_FREE
#endif

#define MG_SET_PTRPTR(_ptr, _v) \
  do {                          \
    if (_ptr) *(_ptr) = _v;     \
  } while (0)

#ifndef MG_INTERNAL
#define MG_INTERNAL static
#endif

#ifdef PICOTCP
#define NO_LIBC
#define MG_DISABLE_PFS
#endif

#include "common/cs_dbg.h"
#include "mg_http.h"
#include "mg_net.h"

#ifndef MG_CTL_MSG_MESSAGE_SIZE
#define MG_CTL_MSG_MESSAGE_SIZE 8192
#endif

/* internals that need to be accessible in unit tests */
MG_INTERNAL struct mg_connection *mg_do_connect(struct mg_connection *nc,
                                                int proto,
                                                union socket_address *sa);

MG_INTERNAL int mg_parse_address(const char *str, union socket_address *sa,
                                 int *proto, char *host, size_t host_len);
MG_INTERNAL void mg_call(struct mg_connection *nc,
                         mg_event_handler_t ev_handler, void *user_data, int ev,
                         void *ev_data);
void mg_forward(struct mg_connection *from, struct mg_connection *to);
MG_INTERNAL void mg_add_conn(struct mg_mgr *mgr, struct mg_connection *c);
MG_INTERNAL void mg_remove_conn(struct mg_connection *c);
MG_INTERNAL struct mg_connection *mg_create_connection(
    struct mg_mgr *mgr, mg_event_handler_t callback,
    struct mg_add_sock_opts opts);
#ifdef _WIN32
/* Retur value is the same as for MultiByteToWideChar. */
int to_wchar(const char *path, wchar_t *wbuf, size_t wbuf_len);
#endif

struct ctl_msg {
  mg_event_handler_t callback;
  char message[MG_CTL_MSG_MESSAGE_SIZE];
};

#if MG_ENABLE_MQTT
struct mg_mqtt_message;

#define MG_MQTT_ERROR_INCOMPLETE_MSG -1
#define MG_MQTT_ERROR_MALFORMED_MSG -2

MG_INTERNAL int parse_mqtt(struct mbuf *io, struct mg_mqtt_message *mm);
#endif

/* Forward declarations for testing. */
extern void *(*test_malloc)(size_t size);
extern void *(*test_calloc)(size_t count, size_t size);

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#if MG_ENABLE_HTTP
struct mg_serve_http_opts;

MG_INTERNAL struct mg_http_proto_data *mg_http_create_proto_data(
    struct mg_connection *c);

/*
 * Reassemble the content of the buffer (buf, blen) which should be
 * in the HTTP chunked encoding, by collapsing data chunks to the
 * beginning of the buffer.
 *
 * If chunks get reassembled, modify hm->body to point to the reassembled
 * body and fire MG_EV_HTTP_CHUNK event. If handler sets MG_F_DELETE_CHUNK
 * in nc->flags, delete reassembled body from the mbuf.
 *
 * Return reassembled body size.
 */
MG_INTERNAL size_t mg_handle_chunked(struct mg_connection *nc,
                                     struct http_message *hm, char *buf,
                                     size_t blen);

#if MG_ENABLE_FILESYSTEM
MG_INTERNAL int mg_uri_to_local_path(struct http_message *hm,
                                     const struct mg_serve_http_opts *opts,
                                     char **local_path,
                                     struct mg_str *remainder);
MG_INTERNAL time_t mg_parse_date_string(const char *datetime);
MG_INTERNAL int mg_is_not_modified(struct http_message *hm, cs_stat_t *st);
#endif
#if MG_ENABLE_HTTP_CGI
MG_INTERNAL void mg_handle_cgi(struct mg_connection *nc, const char *prog,
                               const struct mg_str *path_info,
                               const struct http_message *hm,
                               const struct mg_serve_http_opts *opts);
struct mg_http_proto_data_cgi;
MG_INTERNAL void mg_http_free_proto_data_cgi(struct mg_http_proto_data_cgi *d);
#endif
#if MG_ENABLE_HTTP_SSI
MG_INTERNAL void mg_handle_ssi_request(struct mg_connection *nc,
                                       struct http_message *hm,
                                       const char *path,
                                       const struct mg_serve_http_opts *opts);
#endif
#if MG_ENABLE_HTTP_WEBDAV
MG_INTERNAL int mg_is_dav_request(const struct mg_str *s);
MG_INTERNAL void mg_handle_propfind(struct mg_connection *nc, const char *path,
                                    cs_stat_t *stp, struct http_message *hm,
                                    struct mg_serve_http_opts *opts);
MG_INTERNAL void mg_handle_lock(struct mg_connection *nc, const char *path);
MG_INTERNAL void mg_handle_mkcol(struct mg_connection *nc, const char *path,
                                 struct http_message *hm);
MG_INTERNAL void mg_handle_move(struct mg_connection *c,
                                const struct mg_serve_http_opts *opts,
                                const char *path, struct http_message *hm);
MG_INTERNAL void mg_handle_delete(struct mg_connection *nc,
                                  const struct mg_serve_http_opts *opts,
                                  const char *path);
MG_INTERNAL void mg_handle_put(struct mg_connection *nc, const char *path,
                               struct http_message *hm);
#endif
#if MG_ENABLE_HTTP_WEBSOCKET
MG_INTERNAL void mg_ws_handler(struct mg_connection *nc, int ev,
                               void *ev_data MG_UD_ARG(void *user_data));
MG_INTERNAL void mg_ws_handshake(struct mg_connection *nc,
                                 const struct mg_str *key,
                                 struct http_message *);
#endif
#endif /* MG_ENABLE_HTTP */

MG_INTERNAL int mg_get_errno(void);

MG_INTERNAL void mg_close_conn(struct mg_connection *conn);

#if MG_ENABLE_SNTP
MG_INTERNAL int mg_sntp_parse_reply(const char *buf, int len,
                                    struct mg_sntp_message *msg);
#endif

#endif /* CS_MONGOOSE_SRC_INTERNAL_H_ */
