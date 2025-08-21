#include "net.h"
#include "dns.h"
#include "fmt.h"
#include "log.h"
#include "printf.h"
#include "profile.h"
#include "timer.h"
#include "tls.h"

size_t mg_vprintf(struct mg_connection *c, const char *fmt, va_list *ap) {
  size_t old = c->send.len;
  mg_vxprintf(mg_pfn_iobuf, &c->send, fmt, ap);
  return c->send.len - old;
}

size_t mg_printf(struct mg_connection *c, const char *fmt, ...) {
  size_t len = 0;
  va_list ap;
  va_start(ap, fmt);
  len = mg_vprintf(c, fmt, &ap);
  va_end(ap);
  return len;
}

static bool mg_atonl(struct mg_str str, struct mg_addr *addr) {
  uint32_t localhost = mg_htonl(0x7f000001);
  if (mg_strcasecmp(str, mg_str("localhost")) != 0) return false;
  memcpy(addr->ip, &localhost, sizeof(uint32_t));
  addr->is_ip6 = false;
  return true;
}

static bool mg_atone(struct mg_str str, struct mg_addr *addr) {
  if (str.len > 0) return false;
  memset(addr->ip, 0, sizeof(addr->ip));
  addr->is_ip6 = false;
  return true;
}

static bool mg_aton4(struct mg_str str, struct mg_addr *addr) {
  uint8_t data[4] = {0, 0, 0, 0};
  size_t i, num_dots = 0;
  for (i = 0; i < str.len; i++) {
    if (str.buf[i] >= '0' && str.buf[i] <= '9') {
      int octet = data[num_dots] * 10 + (str.buf[i] - '0');
      if (octet > 255) return false;
      data[num_dots] = (uint8_t) octet;
    } else if (str.buf[i] == '.') {
      if (num_dots >= 3 || i == 0 || str.buf[i - 1] == '.') return false;
      num_dots++;
    } else {
      return false;
    }
  }
  if (num_dots != 3 || str.buf[i - 1] == '.') return false;
  memcpy(&addr->ip, data, sizeof(data));
  addr->is_ip6 = false;
  return true;
}

static bool mg_v4mapped(struct mg_str str, struct mg_addr *addr) {
  int i;
  uint32_t ipv4;
  if (str.len < 14) return false;
  if (str.buf[0] != ':' || str.buf[1] != ':' || str.buf[6] != ':') return false;
  for (i = 2; i < 6; i++) {
    if (str.buf[i] != 'f' && str.buf[i] != 'F') return false;
  }
  // struct mg_str s = mg_str_n(&str.buf[7], str.len - 7);
  if (!mg_aton4(mg_str_n(&str.buf[7], str.len - 7), addr)) return false;
  memcpy(&ipv4, addr->ip, sizeof(ipv4));
  memset(addr->ip, 0, sizeof(addr->ip));
  addr->ip[10] = addr->ip[11] = 255;
  memcpy(&addr->ip[12], &ipv4, 4);
  addr->is_ip6 = true;
  return true;
}

static bool mg_aton6(struct mg_str str, struct mg_addr *addr) {
  size_t i, j = 0, n = 0, dc = 42;
  addr->scope_id = 0;
  if (str.len > 2 && str.buf[0] == '[') str.buf++, str.len -= 2;
  if (mg_v4mapped(str, addr)) return true;
  for (i = 0; i < str.len; i++) {
    if ((str.buf[i] >= '0' && str.buf[i] <= '9') ||
        (str.buf[i] >= 'a' && str.buf[i] <= 'f') ||
        (str.buf[i] >= 'A' && str.buf[i] <= 'F')) {
      unsigned long val = 0;  // TODO(): This loops on chars, refactor
      if (i > j + 3) return false;
      // MG_DEBUG(("%lu %lu [%.*s]", i, j, (int) (i - j + 1), &str.buf[j]));
      mg_str_to_num(mg_str_n(&str.buf[j], i - j + 1), 16, &val, sizeof(val));
      addr->ip[n] = (uint8_t) ((val >> 8) & 255);
      addr->ip[n + 1] = (uint8_t) (val & 255);
    } else if (str.buf[i] == ':') {
      j = i + 1;
      if (i > 0 && str.buf[i - 1] == ':') {
        dc = n;  // Double colon
        if (i > 1 && str.buf[i - 2] == ':') return false;
      } else if (i > 0) {
        n += 2;
      }
      if (n > 14) return false;
      addr->ip[n] = addr->ip[n + 1] = 0;  // For trailing ::
    } else if (str.buf[i] == '%') {       // Scope ID, last in string
      return mg_str_to_num(mg_str_n(&str.buf[i + 1], str.len - i - 1), 10,
                           &addr->scope_id, sizeof(uint8_t));
    } else {
      return false;
    }
  }
  if (n < 14 && dc == 42) return false;
  if (n < 14) {
    memmove(&addr->ip[dc + (14 - n)], &addr->ip[dc], n - dc + 2);
    memset(&addr->ip[dc], 0, 14 - n);
  }

  addr->is_ip6 = true;
  return true;
}

bool mg_aton(struct mg_str str, struct mg_addr *addr) {
  // MG_INFO(("[%.*s]", (int) str.len, str.buf));
  return mg_atone(str, addr) || mg_atonl(str, addr) || mg_aton4(str, addr) ||
         mg_aton6(str, addr);
}

struct mg_connection *mg_alloc_conn(struct mg_mgr *mgr) {
  struct mg_connection *c =
      (struct mg_connection *) mg_calloc(1, sizeof(*c) + mgr->extraconnsize);
  if (c != NULL) {
    c->mgr = mgr;
    c->send.align = c->recv.align = c->rtls.align = MG_IO_SIZE;
    c->id = ++mgr->nextid;
    MG_PROF_INIT(c);
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
  MG_DEBUG(("%lu %ld closed", c->id, c->fd));
  MG_PROF_DUMP(c);
  MG_PROF_FREE(c);

  mg_tls_free(c);
  mg_iobuf_free(&c->recv);
  mg_iobuf_free(&c->send);
  mg_iobuf_free(&c->rtls);
  mg_bzero((unsigned char *) c, sizeof(*c));
  mg_free(c);
}

struct mg_connection *mg_connect_svc(struct mg_mgr *mgr, const char *url,
                                     mg_event_handler_t fn, void *fn_data,
                                     mg_event_handler_t pfn, void *pfn_data) {
  struct mg_connection *c = NULL;
  if (url == NULL || url[0] == '\0') {
    MG_ERROR(("null url"));
  } else if ((c = mg_alloc_conn(mgr)) == NULL) {
    MG_ERROR(("OOM"));
  } else {
    LIST_ADD_HEAD(struct mg_connection, &mgr->conns, c);
    c->is_udp = (strncmp(url, "udp:", 4) == 0);
    c->fd = (void *) (size_t) MG_INVALID_SOCKET;
    c->fn = fn;
    c->is_client = true;
    c->fn_data = fn_data;
    c->is_tls = (mg_url_is_ssl(url) != 0);
    c->pfn = pfn;
    c->pfn_data = pfn_data;
    mg_call(c, MG_EV_OPEN, (void *) url);
    MG_DEBUG(("%lu %ld %s", c->id, c->fd, url));
    mg_resolve(c, url);
  }
  return c;
}

struct mg_connection *mg_connect(struct mg_mgr *mgr, const char *url,
                                 mg_event_handler_t fn, void *fn_data) {
  return mg_connect_svc(mgr, url, fn, fn_data, NULL, NULL);
}

struct mg_connection *mg_listen(struct mg_mgr *mgr, const char *url,
                                mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = NULL;
  if ((c = mg_alloc_conn(mgr)) == NULL) {
    MG_ERROR(("OOM %s", url));
  } else if (!mg_open_listener(c, url)) {
    MG_ERROR(("Failed: %s", url));
    MG_PROF_FREE(c);
    mg_free(c);
    c = NULL;
  } else {
    c->is_listening = 1;
    c->is_udp = strncmp(url, "udp:", 4) == 0;
    LIST_ADD_HEAD(struct mg_connection, &mgr->conns, c);
    c->fn = fn;
    c->fn_data = fn_data;
    c->is_tls = (mg_url_is_ssl(url) != 0);
    mg_call(c, MG_EV_OPEN, NULL);
    MG_DEBUG(("%lu %ld %s", c->id, c->fd, url));
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
    MG_EPOLL_ADD(c);
    mg_call(c, MG_EV_OPEN, NULL);
    LIST_ADD_HEAD(struct mg_connection, &mgr->conns, c);
  }
  return c;
}

struct mg_timer *mg_timer_add(struct mg_mgr *mgr, uint64_t milliseconds,
                              unsigned flags, void (*fn)(void *), void *arg) {
  struct mg_timer *t = (struct mg_timer *) mg_calloc(1, sizeof(*t));
  if (t != NULL) {
    flags |= MG_TIMER_AUTODELETE;  // We have alloc'ed it, so autodelete
    mg_timer_init(&mgr->timers, t, milliseconds, flags, fn, arg);
  }
  return t;
}

long mg_io_recv(struct mg_connection *c, void *buf, size_t len) {
  if (c->rtls.len == 0) return MG_IO_WAIT;
  if (len > c->rtls.len) len = c->rtls.len;
  memcpy(buf, c->rtls.buf, len);
  mg_iobuf_del(&c->rtls, 0, len);
  return (long) len;
}

void mg_mgr_free(struct mg_mgr *mgr) {
  struct mg_connection *c;
  struct mg_timer *tmp, *t = mgr->timers;
  while (t != NULL) tmp = t->next, mg_free(t), t = tmp;
  mgr->timers = NULL;  // Important. Next call to poll won't touch timers
  for (c = mgr->conns; c != NULL; c = c->next) c->is_closing = 1;
  mg_mgr_poll(mgr, 0);
#if MG_ENABLE_FREERTOS_TCP
  FreeRTOS_DeleteSocketSet(mgr->ss);
#endif
  MG_DEBUG(("All connections closed"));
#if MG_ENABLE_EPOLL
  if (mgr->epoll_fd >= 0) close(mgr->epoll_fd), mgr->epoll_fd = -1;
#endif
  mg_tls_ctx_free(mgr);
#if MG_ENABLE_TCPIP
  if (mgr->ifp) mg_tcpip_free(mgr->ifp);
#endif
}

void mg_mgr_init(struct mg_mgr *mgr) {
  memset(mgr, 0, sizeof(*mgr));
#if MG_ENABLE_EPOLL
  if ((mgr->epoll_fd = epoll_create1(EPOLL_CLOEXEC)) < 0)
    MG_ERROR(("epoll_create1 errno %d", errno));
#else
  mgr->epoll_fd = -1;
#endif
#if MG_ARCH == MG_ARCH_WIN32 && MG_ENABLE_WINSOCK
  // clang-format off
  { WSADATA data; WSAStartup(MAKEWORD(2, 2), &data); }
  // clang-format on
#elif MG_ENABLE_FREERTOS_TCP
  mgr->ss = FreeRTOS_CreateSocketSet();
#elif MG_ARCH == MG_ARCH_UNIX
  // Ignore SIGPIPE signal, so if client cancels the request, it
  // won't kill the whole process.
  signal(SIGPIPE, SIG_IGN);
#elif MG_ENABLE_TCPIP_DRIVER_INIT && defined(MG_TCPIP_DRIVER_INIT)
  MG_TCPIP_DRIVER_INIT(mgr);
#endif
  mgr->pipe = MG_INVALID_SOCKET;
  mgr->dnstimeout = 3000;
  mgr->dns4.url = "udp://8.8.8.8:53";
  mgr->dns6.url = "udp://[2001:4860:4860::8888]:53";
  mg_tls_ctx_init(mgr);
  MG_DEBUG(("MG_IO_SIZE: %lu, TLS: %s", MG_IO_SIZE,
            MG_TLS == MG_TLS_NONE      ? "none"
            : MG_TLS == MG_TLS_MBED    ? "MbedTLS"
            : MG_TLS == MG_TLS_OPENSSL ? "OpenSSL"
            : MG_TLS == MG_TLS_BUILTIN ? "builtin"
            : MG_TLS == MG_TLS_WOLFSSL ? "WolfSSL"
                                       : "custom"));
}
