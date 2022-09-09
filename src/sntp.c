#include "sntp.h"
#include "arch.h"
#include "event.h"
#include "log.h"
#include "util.h"

#define SNTP_TIME_OFFSET 2208988800U  // (1970 - 1900) in seconds
#define SNTP_MAX_FRAC 4294967295.0    // 2 ** 32 - 1

static int64_t gettimestamp(const uint32_t *data) {
  uint32_t sec = mg_ntohl(data[0]), frac = mg_ntohl(data[1]);
  if (sec) sec -= SNTP_TIME_OFFSET;
  return ((int64_t) sec) * 1000 + (int64_t) (frac / SNTP_MAX_FRAC * 1000.0);
}

int64_t mg_sntp_parse(const unsigned char *buf, size_t len) {
  int64_t res = -1;
  int mode = len > 0 ? buf[0] & 7 : 0;
  int version = len > 0 ? (buf[0] >> 3) & 7 : 0;
  if (len < 48) {
    MG_ERROR(("%s", "corrupt packet"));
  } else if (mode != 4 && mode != 5) {
    MG_ERROR(("%s", "not a server reply"));
  } else if (buf[1] == 0) {
    MG_ERROR(("%s", "server sent a kiss of death"));
  } else if (version == 4 || version == 3) {
    // int64_t ref = gettimestamp((uint32_t *) &buf[16]);
    int64_t t0 = gettimestamp((uint32_t *) &buf[24]);
    int64_t t1 = gettimestamp((uint32_t *) &buf[32]);
    int64_t t2 = gettimestamp((uint32_t *) &buf[40]);
    int64_t t3 = (int64_t) mg_millis();
    int64_t delta = (t3 - t0) - (t2 - t1);
    MG_VERBOSE(("%lld %lld %lld %lld delta:%lld", t0, t1, t2, t3, delta));
    res = t2 + delta / 2;
  } else {
    MG_ERROR(("unexpected version: %d", version));
  }
  return res;
}

static void sntp_cb(struct mg_connection *c, int ev, void *evd, void *fnd) {
  if (ev == MG_EV_READ) {
    int64_t milliseconds = mg_sntp_parse(c->recv.buf, c->recv.len);
    if (milliseconds > 0) {
      MG_INFO(("%lu got time: %lld ms from epoch", c->id, milliseconds));
      mg_call(c, MG_EV_SNTP_TIME, (uint64_t *) &milliseconds);
      MG_VERBOSE(("%u.%u", (unsigned) (milliseconds / 1000),
                  (unsigned) (milliseconds % 1000)));
    }
    mg_iobuf_del(&c->recv, 0, c->recv.len);  // Free receive buffer
  } else if (ev == MG_EV_CONNECT) {
    mg_sntp_request(c);
  } else if (ev == MG_EV_CLOSE) {
  }
  (void) fnd;
  (void) evd;
}

void mg_sntp_request(struct mg_connection *c) {
  if (c->is_resolving) {
    MG_ERROR(("%lu wait until resolved", c->id));
  } else {
    int64_t now = (int64_t) mg_millis();  // Use int64_t, for vc98
    uint8_t buf[48] = {0};
    uint32_t *t = (uint32_t *) &buf[40];
    double frac = ((double) (now % 1000)) / 1000.0 * SNTP_MAX_FRAC;
    buf[0] = (0 << 6) | (4 << 3) | 3;
    t[0] = mg_htonl((uint32_t) (now / 1000) + SNTP_TIME_OFFSET);
    t[1] = mg_htonl((uint32_t) frac);
    mg_send(c, buf, sizeof(buf));
  }
}

struct mg_connection *mg_sntp_connect(struct mg_mgr *mgr, const char *url,
                                      mg_event_handler_t fn, void *fnd) {
  struct mg_connection *c = NULL;
  if (url == NULL) url = "udp://time.google.com:123";
  if ((c = mg_connect(mgr, url, fn, fnd)) != NULL) c->pfn = sntp_cb;
  return c;
}
