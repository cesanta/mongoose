#include "util.h"

#if MG_ENABLE_CUSTOM_RANDOM
#else
#define MG_RESEED_PERIOD_MS 60000
void mg_random(void *buf, size_t len) {
  bool done = false;
  unsigned char *p = (unsigned char *) buf;
#if MG_ARCH == MG_ARCH_ESP32
  while (len--) *p++ = (unsigned char) (esp_random() & 255);
  done = true;
#elif MG_ARCH == MG_ARCH_WIN32
#elif MG_ARCH == MG_ARCH_UNIX
  FILE *fp = fopen("/dev/urandom", "rb");
  if (fp != NULL) {
    if (fread(buf, 1, len, fp) == len) done = true;
    fclose(fp);
  }
#else
  static uint64_t millis_last;
  volatile uint64_t millis = mg_millis();
  // Seed/reseed on timout
  if (!millis_last || ((millis - millis_last) > MG_RESEED_PERIOD_MS)) {
    srand((unsigned int)millis);
    millis_last = millis;
  }
#endif
  // If everything above did not work, fallback to a pseudo random generator
  while (!done && len--) *p++ = (unsigned char) (rand() & 255);
}
#endif

char *mg_random_str(char *buf, size_t len) {
  size_t i;
  mg_random(buf, len);
  for (i = 0; i < len; i++) {
    uint8_t c = ((uint8_t *) buf)[i] % 62U;
    buf[i] = i == len - 1 ? (char) '\0'            // 0-terminate last byte
             : c < 26     ? (char) ('a' + c)       // lowercase
             : c < 52     ? (char) ('A' + c - 26)  // uppercase
                          : (char) ('0' + c - 52);     // numeric
  }
  return buf;
}

uint32_t mg_ntohl(uint32_t net) {
  uint8_t data[4] = {0, 0, 0, 0};
  memcpy(&data, &net, sizeof(data));
  return (((uint32_t) data[3]) << 0) | (((uint32_t) data[2]) << 8) |
         (((uint32_t) data[1]) << 16) | (((uint32_t) data[0]) << 24);
}

uint16_t mg_ntohs(uint16_t net) {
  uint8_t data[2] = {0, 0};
  memcpy(&data, &net, sizeof(data));
  return (uint16_t) ((uint16_t) data[1] | (((uint16_t) data[0]) << 8));
}

uint32_t mg_crc32(uint32_t crc, const char *buf, size_t len) {
  int i;
  crc = ~crc;
  while (len--) {
    crc ^= *(unsigned char *) buf++;
    for (i = 0; i < 8; i++) crc = crc & 1 ? (crc >> 1) ^ 0xedb88320 : crc >> 1;
  }
  return ~crc;
}

static int isbyte(int n) {
  return n >= 0 && n <= 255;
}

static int parse_net(const char *spec, uint32_t *net, uint32_t *mask) {
  int n, a, b, c, d, slash = 32, len = 0;
  if ((sscanf(spec, "%d.%d.%d.%d/%d%n", &a, &b, &c, &d, &slash, &n) == 5 ||
       sscanf(spec, "%d.%d.%d.%d%n", &a, &b, &c, &d, &n) == 4) &&
      isbyte(a) && isbyte(b) && isbyte(c) && isbyte(d) && slash >= 0 &&
      slash < 33) {
    len = n;
    *net = ((uint32_t) a << 24) | ((uint32_t) b << 16) | ((uint32_t) c << 8) |
           (uint32_t) d;
    *mask = slash ? (uint32_t) (0xffffffffU << (32 - slash)) : (uint32_t) 0;
  }
  return len;
}

int mg_check_ip_acl(struct mg_str acl, uint32_t remote_ip) {
  struct mg_str k, v;
  int allowed = acl.len == 0 ? '+' : '-';  // If any ACL is set, deny by default
  while (mg_commalist(&acl, &k, &v)) {
    uint32_t net, mask;
    if (k.ptr[0] != '+' && k.ptr[0] != '-') return -1;
    if (parse_net(&k.ptr[1], &net, &mask) == 0) return -2;
    if ((mg_ntohl(remote_ip) & mask) == net) allowed = k.ptr[0];
  }
  return allowed == '+';
}

static size_t print_ip4(void (*out)(char, void *), void *arg, uint8_t *p) {
    return mg_xprintf(out, arg, "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
}

static size_t print_ip6(void (*out)(char, void *), void *arg, uint16_t *p) {
    return mg_xprintf(out, arg, "[%x:%x:%x:%x:%x:%x:%x:%x]", mg_ntohs(p[0]),
                      mg_ntohs(p[1]), mg_ntohs(p[2]), mg_ntohs(p[3]),
                      mg_ntohs(p[4]), mg_ntohs(p[5]), mg_ntohs(p[6]),
                      mg_ntohs(p[7]));
}

size_t mg_print_ip4(void (*out)(char, void *), void *arg, va_list *ap) {
  uint8_t *p = va_arg(*ap, uint8_t *);
  return print_ip4(out, arg, p);
}

size_t mg_print_ip6(void (*out)(char, void *), void *arg, va_list *ap) {
  uint16_t *p = va_arg(*ap, uint16_t *);
  return print_ip6(out, arg, p);
}

size_t mg_print_ip(void (*out)(char, void *), void *arg, va_list *ap) {
  struct mg_addr *addr = va_arg(*ap, struct mg_addr *);
  if (addr->is_ip6) return print_ip6(out, arg, (uint16_t *) addr->ip6);
  return print_ip4(out, arg, (uint8_t *) &addr->ip);
}

size_t mg_print_ip_port(void (*out)(char, void *), void *arg, va_list *ap) {
  struct mg_addr *a = va_arg(*ap, struct mg_addr *);
  return mg_xprintf(out, arg, "%M:%hu", mg_print_ip, a, mg_ntohs(a->port));
}

size_t mg_print_mac(void (*out)(char, void *), void *arg, va_list *ap) {
  uint8_t *p = va_arg(*ap, uint8_t *);
  return mg_xprintf(out, arg, "%02x:%02x:%02x:%02x:%02x:%02x", p[0], p[1], p[2],
                    p[3], p[4], p[5]);
}

#if MG_ENABLE_CUSTOM_MILLIS
#else
uint64_t mg_millis(void) {
#if MG_ARCH == MG_ARCH_WIN32
  return GetTickCount();
#elif MG_ARCH == MG_ARCH_RP2040
  return time_us_64() / 1000;
#elif MG_ARCH == MG_ARCH_ESP32
  return esp_timer_get_time() / 1000;
#elif MG_ARCH == MG_ARCH_ESP8266 || MG_ARCH == MG_ARCH_FREERTOS
  return xTaskGetTickCount() * portTICK_PERIOD_MS;
#elif MG_ARCH == MG_ARCH_AZURERTOS
  return tx_time_get() * (1000 /* MS per SEC */ / TX_TIMER_TICKS_PER_SECOND);
#elif MG_ARCH == MG_ARCH_TIRTOS
  return (uint64_t) Clock_getTicks();
#elif MG_ARCH == MG_ARCH_ZEPHYR
  return (uint64_t) k_uptime_get();
#elif MG_ARCH == MG_ARCH_UNIX && defined(__APPLE__)
  // Apple CLOCK_MONOTONIC_RAW is equivalent to CLOCK_BOOTTIME on linux
  // Apple CLOCK_UPTIME_RAW is equivalent to CLOCK_MONOTONIC_RAW on linux
  return clock_gettime_nsec_np(CLOCK_UPTIME_RAW) / 1000000;
#elif MG_ARCH == MG_ARCH_UNIX
  struct timespec ts = {0, 0};
  // See #1615 - prefer monotonic clock
#if defined(CLOCK_MONOTONIC_RAW)
  // Raw hardware-based time that is not subject to NTP adjustment
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
#elif defined(CLOCK_MONOTONIC)
  // Affected by the incremental adjustments performed by adjtime and NTP
  clock_gettime(CLOCK_MONOTONIC, &ts);
#else
  // Affected by discontinuous jumps in the system time and by the incremental
  // adjustments performed by adjtime and NTP
  clock_gettime(CLOCK_REALTIME, &ts);
#endif
  return ((uint64_t) ts.tv_sec * 1000 + (uint64_t) ts.tv_nsec / 1000000);
#elif defined(ARDUINO)
  return (uint64_t) millis();
#else
  return (uint64_t) (time(NULL) * 1000);
#endif
}
#endif

