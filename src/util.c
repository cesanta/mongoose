#include "util.h"

#if MG_ARCH == MG_ARCH_UNIX && defined(__APPLE__)
#include <mach/mach_time.h>
#endif

#if MG_ENABLE_CUSTOM_RANDOM
#else
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
#endif
  // If everything above did not work, fallback to a pseudo random generator
  while (!done && len--) *p++ = (unsigned char) (rand() & 255);
}
#endif

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

#if MG_ENABLE_CUSTOM_MILLIS
#else
uint64_t mg_millis(void) {
#if MG_ARCH == MG_ARCH_WIN32
  return GetTickCount();
#elif MG_ARCH == MG_ARCH_ESP32
  return esp_timer_get_time() / 1000;
#elif MG_ARCH == MG_ARCH_ESP8266
  return xTaskGetTickCount() * portTICK_PERIOD_MS;
#elif MG_ARCH == MG_ARCH_FREERTOS_TCP || MG_ARCH == MG_ARCH_FREERTOS_LWIP
  return xTaskGetTickCount() * portTICK_PERIOD_MS;
#elif MG_ARCH == MG_ARCH_AZURERTOS
  return tx_time_get() * (1000 /* MS per SEC */ / TX_TIMER_TICKS_PER_SECOND);
#elif MG_ARCH == MG_ARCH_UNIX
  struct timespec ts = {0, 0};
  clock_gettime(CLOCK_REALTIME, &ts);
  return ((uint64_t) ts.tv_sec * 1000 + (uint64_t) ts.tv_nsec / 1000000);
#else
  return (uint64_t) (time(NULL) * 1000);
#endif
}
#endif
