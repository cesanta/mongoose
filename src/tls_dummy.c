#include "tls.h"

#if !MG_ENABLE_MBEDTLS && !MG_ENABLE_OPENSSL && !MG_ENABLE_CUSTOM_TLS
void* mg_tls_ctx_init(const struct mg_tls_opts *opts) {
  (void) opts;
  return (void *)mg_tls_ctx_init;
}
void mg_tls_ctx_free(void *ctx) {
  (void) ctx;
}
bool mg_tls_init(struct mg_connection *c, struct mg_str *opts) {
  (void) opts;
  mg_error(c, "TLS is not enabled");
  return false;
}
void mg_tls_handshake(struct mg_connection *c) {
  (void) c;
}
void mg_tls_free(struct mg_connection *c) {
  (void) c;
}
long mg_tls_recv(struct mg_connection *c, void *buf, size_t len) {
  return c == NULL || buf == NULL || len == 0 ? 0 : -1;
}
long mg_tls_send(struct mg_connection *c, const void *buf, size_t len) {
  return c == NULL || buf == NULL || len == 0 ? 0 : -1;
}
size_t mg_tls_pending(struct mg_connection *c) {
  (void) c;
  return 0;
}
#endif
