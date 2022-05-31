#include "mongoose.h"

int mkdir(const char *path, mode_t mode) {
  (void) path, (void) mode;
  return -1;
}

void mg_connect_resolved(struct mg_connection *c) {
  (void) c;
}

bool mg_open_listener(struct mg_connection *c, const char *url) {
  (void) c, (void) url;
  return false;
}

void mg_mgr_poll(struct mg_mgr *mgr, int ms) {
  (void) mgr, (void) ms;
}

bool mg_send(struct mg_connection *c, const void *buf, size_t len) {
  (void) c, (void) buf, (void) len;
  return false;
}

int mg_mkpipe(struct mg_mgr *m, mg_event_handler_t fn, void *d, bool udp) {
  (void) m, (void) fn, (void) d, (void) udp;
  return -1;
}

void _fini(void);
void _fini(void) {
}
