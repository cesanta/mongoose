#include "mongoose.h"

int mkdir(const char *path, mode_t mode) {
  (void) path, (void) mode;
  return -1;
}

struct mg_connection *mg_connect(struct mg_mgr *mgr, const char *url,
                                 mg_event_handler_t fn, void *fn_data) {
  (void) mgr, (void) url, (void) fn, (void) fn_data;
  return NULL;
}

void mg_connect_resolved(struct mg_connection *c);
void mg_connect_resolved(struct mg_connection *c) {
  (void) c;
}

struct mg_connection *mg_listen(struct mg_mgr *mgr, const char *url,
                                mg_event_handler_t fn, void *fn_data) {
  (void) mgr, (void) url, (void) fn, (void) fn_data;
  return NULL;
}

void mg_mgr_poll(struct mg_mgr *mgr, int ms) {
  (void) mgr, (void) ms;
}

bool mg_send(struct mg_connection *c, const void *buf, size_t len) {
  (void) c, (void) buf, (void) len;
  return 0;
}

void mg_mgr_wakeup(struct mg_connection *c, const void *buf, size_t len) {
  (void) c, (void) buf, (void) len;
}

struct mg_connection *mg_mkpipe(struct mg_mgr *mgr, mg_event_handler_t fn,
                                void *fn_data) {
  (void) mgr, (void) fn, (void) fn_data;
  return NULL;
}

void _fini(void) {
}
