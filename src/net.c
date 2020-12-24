#include "log.h"
#include "net.h"
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
  char tmp[100];
  const char *fmt = c->peer.is_ip6 ? "[%s]:%d" : "%s:%d";
  mg_ntoa(&c->peer, tmp, sizeof(tmp));
  snprintf(buf, len, fmt, tmp, (int) mg_ntohs(c->peer.port));
  return buf;
}

char *mg_ntoa(const struct mg_addr *addr, char *buf, size_t len) {
  if (addr->is_ip6) {
    uint16_t *p = (uint16_t *) addr->ip6;
    snprintf(buf, len, "%x:%x:%x:%x:%x:%x:%x:%x", mg_htons(p[0]),
             mg_htons(p[1]), mg_htons(p[2]), mg_htons(p[3]), mg_htons(p[4]),
             mg_htons(p[5]), mg_htons(p[6]), mg_htons(p[7]));
  } else {
    uint8_t p[4];
    memcpy(p, &addr->ip, sizeof(p));
    snprintf(buf, len, "%d.%d.%d.%d", (int) p[0], (int) p[1], (int) p[2],
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

static bool mg_aton4(struct mg_str str, struct mg_addr *addr) {
  uint8_t data[4] = {0, 0, 0, 0};
  size_t i, num_dots = 0;
  for (i = 0; i < str.len; i++) {
    if (str.ptr[i] >= '0' && str.ptr[i] <= '9') {
      int octet = data[num_dots] * 10 + (str.ptr[i] - '0');
      if (octet > 255) return false;
      data[num_dots] = octet;
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

static bool mg_aton6(struct mg_str str, struct mg_addr *addr) {
  size_t i, j = 0, n = 0, dc = 42;
  for (i = 0; i < str.len; i++) {
    if ((str.ptr[i] >= '0' && str.ptr[i] <= '9') ||
        (str.ptr[i] >= 'a' && str.ptr[i] <= 'f') ||
        (str.ptr[i] >= 'A' && str.ptr[i] <= 'F')) {
      unsigned long val;
      if (i > j + 3) return false;
      // LOG(LL_DEBUG, ("%zu %zu [%.*s]", i, j, (int) (i - j + 1),
      // &str.ptr[j]));
      val = mg_unhexn(&str.ptr[j], i - j + 1);
      addr->ip6[n] = (uint8_t)((val >> 8) & 255);
      addr->ip6[n + 1] = (uint8_t)(val & 255);
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
  // LOG(LL_INFO, ("[%.*s]", (int) str.len, str.ptr));
  return mg_atonl(str, addr) || mg_aton4(str, addr) || mg_aton6(str, addr);
}

void mg_mgr_free(struct mg_mgr *mgr) {
  struct mg_connection *c;
  for (c = mgr->conns; c != NULL; c = c->next) c->is_closing = 1;
  mg_mgr_poll(mgr, 0);
#if MG_ARCH == MG_ARCH_FREERTOS
  FreeRTOS_DeleteSocketSet(mgr->ss);
#endif
  LOG(LL_INFO, ("All connections closed"));
}

void mg_mgr_init(struct mg_mgr *mgr) {
#ifdef _WIN32
  WSADATA data;
  WSAStartup(MAKEWORD(2, 2), &data);
#elif MG_ARCH == MG_ARCH_FREERTOS
  mgr->ss = FreeRTOS_CreateSocketSet();
#elif defined(__unix) || defined(__unix__) || defined(__APPLE__)
  // Ignore SIGPIPE signal, so if client cancels the request, it
  // won't kill the whole process.
  signal(SIGPIPE, SIG_IGN);
#endif
  memset(mgr, 0, sizeof(*mgr));
  mgr->dnstimeout = 3000;
  mgr->dns4.url = "udp://8.8.8.8:53";
  mgr->dns6.url = "udp://[2001:4860:4860::8888]:53";
}
