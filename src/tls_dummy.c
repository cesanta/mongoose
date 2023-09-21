#include "tls.h"

#if MG_TLS == MG_TLS_NONE
void mg_tls_init(struct mg_connection *c, const struct mg_tls_opts *opts) {
  (void) opts;
  mg_error(c, "TLS is not enabled");
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
void mg_tls_ctx_init(struct mg_mgr *mgr) {
  (void) mgr;
}
void mg_tls_ctx_free(struct mg_mgr *mgr) {
  (void) mgr;
}
#endif
