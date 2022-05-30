#include "net.h"
#include "dns.h"
#include "log.h"
#include "timer.h"
#include "tls.h"
#include "util.h"

size_t mg_vprintf(struct mg_connection *c, const char *fmt, va_list ap) {
  char mem[256], *buf = mem;
  size_t len = mg_vasprintf(&buf, sizeof(mem), fmt, ap);
  len = mg_send(c, buf, len);
  if (buf != mem) free(buf);
  return len;
}

size_t mg_printf(struct mg_connection *c, const char *fmt, ...) {
  size_t len = 0;
  va_list ap;
  va_start(ap, fmt);
  len = mg_vprintf(c, fmt, ap);
  va_end(ap);
  return len;
}

char *mg_straddr(struct mg_addr *a, char *buf, size_t len) {
  char tmp[30];
  const char *fmt = a->is_ip6 ? "[%s]:%d" : "%s:%d";
  mg_ntoa(a, tmp, sizeof(tmp));
  mg_snprintf(buf, len, fmt, tmp, (int) mg_ntohs(a->port));
  return buf;
}

char *mg_ntoa(const struct mg_addr *addr, char *buf, size_t len) {
  if (addr->is_ip6) {
    uint16_t *p = (uint16_t *) addr->ip6;
    mg_snprintf(buf, len, "%x:%x:%x:%x:%x:%x:%x:%x", mg_htons(p[0]),
                mg_htons(p[1]), mg_htons(p[2]), mg_htons(p[3]), mg_htons(p[4]),
                mg_htons(p[5]), mg_htons(p[6]), mg_htons(p[7]));
  } else {
    uint8_t p[4];
    memcpy(p, &addr->ip, sizeof(p));
    mg_snprintf(buf, len, "%d.%d.%d.%d", (int) p[0], (int) p[1], (int) p[2],
                (int) p[3]);
  }
  return buf;
}

static bool mg_atonl(struct mg_str str, struct mg_addr *addr) {
  if (mg_vcasecmp(&str, "localhost") != 0) return false;
  addr->ip = mg_htonl(0x7f000001);
  addr->is_ip6 = false;
  return true;
}

static bool mg_atone(struct mg_str str, struct mg_addr *addr) {
  if (str.len > 0) return false;
  addr->ip = 0;
  addr->is_ip6 = false;
  return true;
}

static bool mg_aton4(struct mg_str str, struct mg_addr *addr) {
  uint8_t data[4] = {0, 0, 0, 0};
  size_t i, num_dots = 0;
  for (i = 0; i < str.len; i++) {
    if (str.ptr[i] >= '0' && str.ptr[i] <= '9') {
      int octet = data[num_dots] * 10 + (str.ptr[i] - '0');
      if (octet > 255) return false;
      data[num_dots] = (uint8_t) octet;
    } else if (str.ptr[i] == '.') {
      if (num_dots >= 3 || i == 0 || str.ptr[i - 1] == '.') return false;
      num_dots++;
    } else {
      return false;
    }
  }
  if (num_dots != 3 || str.ptr[i - 1] == '.') return false;
  memcpy(&addr->ip, data, sizeof(data));
  addr->is_ip6 = false;
  return true;
}

static bool mg_v4mapped(struct mg_str str, struct mg_addr *addr) {
  int i;
  if (str.len < 14) return false;
  if (str.ptr[0] != ':' || str.ptr[1] != ':' || str.ptr[6] != ':') return false;
  for (i = 2; i < 6; i++) {
    if (str.ptr[i] != 'f' && str.ptr[i] != 'F') return false;
  }
  if (!mg_aton4(mg_str_n(&str.ptr[7], str.len - 7), addr)) return false;
  memset(addr->ip6, 0, sizeof(addr->ip6));
  addr->ip6[10] = addr->ip6[11] = 255;
  memcpy(&addr->ip6[12], &addr->ip, 4);
  addr->is_ip6 = true;
  return true;
}

static bool mg_aton6(struct mg_str str, struct mg_addr *addr) {
  size_t i, j = 0, n = 0, dc = 42;
  if (str.len > 2 && str.ptr[0] == '[') str.ptr++, str.len -= 2;
  if (mg_v4mapped(str, addr)) return true;
  for (i = 0; i < str.len; i++) {
    if ((str.ptr[i] >= '0' && str.ptr[i] <= '9') ||
        (str.ptr[i] >= 'a' && str.ptr[i] <= 'f') ||
        (str.ptr[i] >= 'A' && str.ptr[i] <= 'F')) {
      unsigned long val;
      if (i > j + 3) return false;
      // MG_DEBUG(("%zu %zu [%.*s]", i, j, (int) (i - j + 1), &str.ptr[j]));
      val = mg_unhexn(&str.ptr[j], i - j + 1);
      addr->ip6[n] = (uint8_t) ((val >> 8) & 255);
      addr->ip6[n + 1] = (uint8_t) (val & 255);
    } else if (str.ptr[i] == ':') {
      j = i + 1;
      if (i > 0 && str.ptr[i - 1] == ':') {
        dc = n;  // Double colon
        if (i > 1 && str.ptr[i - 2] == ':') return false;
      } else if (i > 0) {
        n += 2;
      }
      if (n > 14) return false;
      addr->ip6[n] = addr->ip6[n + 1] = 0;  // For trailing ::
    } else {
      return false;
    }
  }
  if (n < 14 && dc == 42) return false;
  if (n < 14) {
    memmove(&addr->ip6[dc + (14 - n)], &addr->ip6[dc], n - dc + 2);
    memset(&addr->ip6[dc], 0, 14 - n);
  }
  addr->is_ip6 = true;
  return true;
}

bool mg_aton(struct mg_str str, struct mg_addr *addr) {
  // MG_INFO(("[%.*s]", (int) str.len, str.ptr));
  return mg_atone(str, addr) || mg_atonl(str, addr) || mg_aton4(str, addr) ||
         mg_aton6(str, addr);
}

struct mg_connection *mg_alloc_conn(struct mg_mgr *mgr) {
  struct mg_connection *c =
      (struct mg_connection *) calloc(1, sizeof(*c) + mgr->extraconnsize);
  if (c != NULL) {
    c->mgr = mgr;
    c->id = ++mgr->nextid;
  }
  return c;
}

void mg_close_conn(struct mg_connection *c) {
  mg_resolve_cancel(c);  // Close any pending DNS query
  LIST_DELETE(struct mg_connection, &c->mgr->conns, c);
  if (c == c->mgr->dns4.c) c->mgr->dns4.c = NULL;
  if (c == c->mgr->dns6.c) c->mgr->dns6.c = NULL;
  // Order of operations is important. `MG_EV_CLOSE` event must be fired
  // before we deallocate received data, see #1331
  mg_call(c, MG_EV_CLOSE, NULL);
  MG_DEBUG(("%lu closed", c->id));

  mg_tls_free(c);
  mg_iobuf_free(&c->recv);
  mg_iobuf_free(&c->send);
  memset(c, 0, sizeof(*c));
  free(c);
}

struct mg_connection *mg_connect(struct mg_mgr *mgr, const char *url,
                                 mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = NULL;
  if (url == NULL || url[0] == '\0') {
    MG_ERROR(("null url"));
  } else if ((c = mg_alloc_conn(mgr)) == NULL) {
    MG_ERROR(("OOM"));
  } else {
    LIST_ADD_HEAD(struct mg_connection, &mgr->conns, c);
    c->is_udp = (strncmp(url, "udp:", 4) == 0);
    c->fn = fn;
    c->is_client = true;
    c->fd = (void *) (size_t) -1;  // Set to invalid socket
    c->fn_data = fn_data;
    MG_DEBUG(("%lu -1 %s", c->id, url));
    mg_call(c, MG_EV_OPEN, NULL);
    mg_resolve(c, url);
  }
  return c;
}

struct mg_connection *mg_listen(struct mg_mgr *mgr, const char *url,
                                mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = NULL;
  if ((c = mg_alloc_conn(mgr)) == NULL) {
    MG_ERROR(("OOM %s", url));
  } else if (!mg_open_listener(c, url)) {
    MG_ERROR(("Failed: %s, errno %d", url, errno));
    free(c);
    c = NULL;
  } else {
    c->is_listening = 1;
    c->is_udp = strncmp(url, "udp:", 4) == 0;
    LIST_ADD_HEAD(struct mg_connection, &mgr->conns, c);
    c->fn = fn;
    c->fn_data = fn_data;
    mg_call(c, MG_EV_OPEN, NULL);
    MG_DEBUG(("%lu %p %s", c->id, c->fd, url));
  }
  return c;
}

struct mg_connection *mg_wrapfd(struct mg_mgr *mgr, int fd,
                                mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = mg_alloc_conn(mgr);
  if (c != NULL) {
    c->fd = (void *) (size_t) fd;
    c->fn = fn;
    c->fn_data = fn_data;
    mg_call(c, MG_EV_OPEN, NULL);
    LIST_ADD_HEAD(struct mg_connection, &mgr->conns, c);
  }
  return c;
}

struct mg_timer *mg_timer_add(struct mg_mgr *mgr, uint64_t milliseconds,
                              unsigned flags, void (*fn)(void *), void *arg) {
  struct mg_timer *t = (struct mg_timer *) calloc(1, sizeof(*t));
  mg_timer_init(&mgr->timers, t, milliseconds, flags, fn, arg);
  return t;
}

void mg_mgr_free(struct mg_mgr *mgr) {
  struct mg_connection *c;
  struct mg_timer *tmp, *t = mgr->timers;
  while (t != NULL) tmp = t->next, free(t), t = tmp;
  mgr->timers = NULL;  // Important. Next call to poll won't touch timers
  for (c = mgr->conns; c != NULL; c = c->next) c->is_closing = 1;
  mg_mgr_poll(mgr, 0);
#if MG_ARCH == MG_ARCH_FREERTOS_TCP
  FreeRTOS_DeleteSocketSet(mgr->ss);
#endif
  MG_DEBUG(("All connections closed"));
}

void mg_mgr_init(struct mg_mgr *mgr) {
  memset(mgr, 0, sizeof(*mgr));
#if MG_ARCH == MG_ARCH_WIN32 && MG_ENABLE_WINSOCK
  // clang-format off
  { WSADATA data; WSAStartup(MAKEWORD(2, 2), &data); }
  // clang-format on
#elif MG_ARCH == MG_ARCH_FREERTOS_TCP
  mgr->ss = FreeRTOS_CreateSocketSet();
#elif defined(__unix) || defined(__unix__) || defined(__APPLE__)
  // Ignore SIGPIPE signal, so if client cancels the request, it
  // won't kill the whole process.
  signal(SIGPIPE, SIG_IGN);
#endif
  mgr->dnstimeout = 3000;
  mgr->dns4.url = "udp://8.8.8.8:53";
  mgr->dns6.url = "udp://[2001:4860:4860::8888]:53";
}
