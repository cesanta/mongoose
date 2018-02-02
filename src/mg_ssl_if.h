/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_MONGOOSE_SRC_SSL_IF_H_
#define CS_MONGOOSE_SRC_SSL_IF_H_

#if MG_ENABLE_SSL

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct mg_ssl_if_ctx;
struct mg_connection;

void mg_ssl_if_init();

enum mg_ssl_if_result {
  MG_SSL_OK = 0,
  MG_SSL_WANT_READ = -1,
  MG_SSL_WANT_WRITE = -2,
  MG_SSL_ERROR = -3,
};

struct mg_ssl_if_conn_params {
  const char *cert;
  const char *key;
  const char *ca_cert;
  const char *server_name;
  const char *cipher_suites;
  const char *psk_identity;
  const char *psk_key;
};

enum mg_ssl_if_result mg_ssl_if_conn_init(
    struct mg_connection *nc, const struct mg_ssl_if_conn_params *params,
    const char **err_msg);
enum mg_ssl_if_result mg_ssl_if_conn_accept(struct mg_connection *nc,
                                            struct mg_connection *lc);
void mg_ssl_if_conn_close_notify(struct mg_connection *nc);
void mg_ssl_if_conn_free(struct mg_connection *nc);

enum mg_ssl_if_result mg_ssl_if_handshake(struct mg_connection *nc);
int mg_ssl_if_read(struct mg_connection *nc, void *buf, size_t buf_size);
int mg_ssl_if_write(struct mg_connection *nc, const void *data, size_t len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MG_ENABLE_SSL */

#endif /* CS_MONGOOSE_SRC_SSL_IF_H_ */
