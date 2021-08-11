#include "mongoose.h"

int usleep(useconds_t us) {
  for (useconds_t i = 0; i < us * 99; i++) asm("nop");
  return 0;
}

int clock_gettime(clockid_t clock_id, struct timespec *tp) {
  (void) clock_id;
  memset(tp, 0, sizeof(*tp));
  return 0;
}

char *realpath(const char *path, char *resolved_path) {
  if (resolved_path == NULL) resolved_path = malloc(strlen(path) + 1);
  strcpy(resolved_path, path);
  return resolved_path;
}

struct mg_connection *mg_connect(struct mg_mgr *mgr, const char *url,
                                 mg_event_handler_t fn, void *fn_data) {
  (void) mgr, (void) url, (void) fn, (void) fn_data;
  return NULL;
}

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

void mg_mgr_wakeup(struct mg_connection *c) {
  (void) c;
}

struct mg_connection *mg_mkpipe(struct mg_mgr *mgr, mg_event_handler_t fn,
                                void *fn_data) {
  (void) mgr, (void) fn, (void) fn_data;
  return NULL;
}

void _fini(void) {
}
