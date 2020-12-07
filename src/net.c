#include "net.h"
#include "log.h"
#include "util.h"

int mg_vprintf(struct mg_connection *c, const char *fmt, va_list ap) {
  char mem[256], *buf = mem;
  int len = mg_vasprintf(&buf, sizeof(mem), fmt, ap);
  len = mg_send(c, buf, len);
  if (buf != mem) free(buf);
  return len;
}

int mg_printf(struct mg_connection *c, const char *fmt, ...) {
  int len = 0;
  va_list ap;
  va_start(ap, fmt);
  len = mg_vprintf(c, fmt, ap);
  va_end(ap);
  return len;
}

char *mg_straddr(struct mg_connection *c, char *buf, size_t len) {
  unsigned char *p = (unsigned char *) &c->peer.ip;
  snprintf(buf, len, "%d.%d.%d.%d:%hu", p[0], p[1], p[2], p[3],
           mg_ntohs(c->peer.port));
  return buf;
}
