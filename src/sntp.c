#include "arch.h"
#include "event.h"
#include "log.h"
#include "sntp.h"
#include "util.h"

#define SNTP_INTERVAL_SEC (3600)
#define SNTP_TIME_OFFSET 2208988800

static unsigned long s_sntmp_next;

int mg_sntp_parse(const unsigned char *buf, size_t len, struct timeval *tv) {
  int mode = len > 0 ? buf[0] & 7 : 0, res = -1;
  if (len < 48) {
    LOG(LL_ERROR, ("%s", "corrupt packet"));
  } else if ((buf[0] & 0x38) >> 3 != 4) {
    LOG(LL_ERROR, ("%s", "wrong version"));
  } else if (mode != 4 && mode != 5) {
    LOG(LL_ERROR, ("%s", "not a server reply"));
  } else if (buf[1] == 0) {
    LOG(LL_ERROR, ("%s", "server sent a kiss of death"));
  } else {
    uint32_t *data = (uint32_t *) &buf[40];
    tv->tv_sec = mg_ntohl(data[0]) - SNTP_TIME_OFFSET;
    tv->tv_usec = mg_ntohl(data[1]);
    s_sntmp_next = tv->tv_sec + SNTP_INTERVAL_SEC;
    res = 0;
  }
  return res;
}

static void sntp_cb(struct mg_connection *c, int ev, void *evd, void *fnd) {
  if (ev == MG_EV_READ) {
    struct timeval tv = {0, 0};
    if (mg_sntp_parse(c->recv.buf, c->recv.len, &tv) == 0) {
      mg_call(c, MG_EV_SNTP_TIME, &tv);
      LOG(LL_DEBUG, ("%u.%u, next at %lu", (unsigned) tv.tv_sec,
                     (unsigned) tv.tv_usec, s_sntmp_next));
    }
    c->recv.len = 0;  // Clear receive buffer
  } else if (ev == MG_EV_RESOLVE) {
    mg_sntp_send(c, time(NULL));
  } else if (ev == MG_EV_CLOSE) {
    // mg_fn_del(c, sntp_cb);
  }
  (void) fnd;
  (void) evd;
}

void mg_sntp_send(struct mg_connection *c, unsigned long utc) {
  if (c->is_resolving) {
    LOG(LL_ERROR, ("%lu wait until resolved", c->id));
  } else if (utc > s_sntmp_next) {
    uint8_t buf[48] = {0};
    s_sntmp_next = utc + SNTP_INTERVAL_SEC;
    buf[0] = (3 << 6) | (4 << 3) | 3;
    mg_send(c, buf, sizeof(buf));
    LOG(LL_DEBUG,
        ("%p request sent, ct %lu, next at %lu", c->fd, utc, s_sntmp_next));
  }
}

struct mg_connection *mg_sntp_connect(struct mg_mgr *mgr, const char *url,
                                      mg_event_handler_t fn, void *fnd) {
  struct mg_connection *c = NULL;
  if (url == NULL) url = "udp://time.google.com:123";
  if ((c = mg_connect(mgr, url, fn, fnd)) != NULL) c->pfn = sntp_cb;
  return c;
}
