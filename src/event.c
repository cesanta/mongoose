#include "event.h"
#include "log.h"
#include "net.h"
#include "util.h"

void mg_call(struct mg_connection *c, int ev, void *ev_data) {
  if (c->pfn != NULL) c->pfn(c, ev, ev_data, c->pfn_data);
  if (c->fn != NULL) c->fn(c, ev, ev_data, c->fn_data);
}

void mg_error(struct mg_connection *c, const char *fmt, ...) {
  char mem[256], *buf = mem;
  va_list ap;
  va_start(ap, fmt);
  mg_vasprintf(&buf, sizeof(mem), fmt, ap);
  va_end(ap);
  LOG(LL_ERROR, ("%lu %s", c->id, buf));
  mg_call(c, MG_EV_ERROR, buf);
  if (buf != mem) free(buf);
  c->is_closing = 1;
}
