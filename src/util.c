#include "util.h"
#include "log.h"

// Not using memset for zeroing memory, cause it can be dropped by compiler
// See https://github.com/cesanta/mongoose/pull/1265
void mg_bzero(volatile unsigned char *buf, size_t len) {
  if (buf != NULL) {
    while (len--) *buf++ = 0;
  }
}

#if MG_ENABLE_CUSTOM_RANDOM
#else
bool mg_random(void *buf, size_t len) {
  bool success = false;
  unsigned char *p = (unsigned char *) buf;
#if MG_ARCH == MG_ARCH_ESP32
  while (len--) *p++ = (unsigned char) (esp_random() & 255);
  success = true;
#elif MG_ARCH == MG_ARCH_CUBE && defined(HAL_RNG_MODULE_ENABLED)
  extern RNG_HandleTypeDef hrng;
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r = HAL_RNG_ReadLastRandomNumber(&hrng);
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
  success = true;
#elif MG_ARCH == MG_ARCH_PICOSDK
  while (len--) *p++ = (unsigned char) (get_rand_32() & 255);
  success = true;
#elif MG_ARCH == MG_ARCH_WIN32
#if defined(_MSC_VER) && _MSC_VER < 1700
  static bool initialised = false;
  static HCRYPTPROV hProv;
  // CryptGenRandom() implementation earlier than 2008 is weak, see
  // https://en.wikipedia.org/wiki/CryptGenRandom
  if (!initialised) {
    initialised = CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL,
                                      CRYPT_VERIFYCONTEXT);
  }
  if (initialised) success = CryptGenRandom(hProv, len, p);
#else
  size_t i;
  for (i = 0; i < len; i++) {
    unsigned int rand_v;
    if (rand_s(&rand_v) == 0) {
      p[i] = (unsigned char) (rand_v & 255);
    } else {
      break;
    }
  }
  success = (i == len);
#endif

#elif MG_ARCH == MG_ARCH_UNIX
  FILE *fp = fopen("/dev/urandom", "rb");
  if (fp != NULL) {
    if (fread(buf, 1, len, fp) == len) success = true;
    fclose(fp);
  }
#endif
  // If everything above did not work, fallback to a pseudo random generator
  if (success == false) {
    MG_ERROR(("Weak RNG: using rand()"));
    while (len--) *p++ = (unsigned char) (rand() & 255);
  }
  return success;
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

uint32_t mg_crc32(uint32_t crc, const char *buf, size_t len) {
  static const uint32_t crclut[16] = {
      // table for polynomial 0xEDB88320 (reflected)
      0x00000000, 0x1DB71064, 0x3B6E20C8, 0x26D930AC, 0x76DC4190, 0x6B6B51F4,
      0x4DB26158, 0x5005713C, 0xEDB88320, 0xF00F9344, 0xD6D6A3E8, 0xCB61B38C,
      0x9B64C2B0, 0x86D3D2D4, 0xA00AE278, 0xBDBDF21C};
  crc = ~crc;
  while (len--) {
    uint8_t b = *(uint8_t *) buf++;
    crc = crclut[(crc ^ b) & 0x0F] ^ (crc >> 4);
    crc = crclut[(crc ^ (b >> 4)) & 0x0F] ^ (crc >> 4);
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

int mg_check_ip_acl(struct mg_str acl, struct mg_addr *remote_ip) {
  struct mg_str entry;
  int allowed = acl.len == 0 ? '+' : '-';  // If any ACL is set, deny by default
  uint32_t remote_ip4;
  if (remote_ip->is_ip6) {
    return -1;  // TODO(): handle IPv6 ACL and addresses
  } else {      // IPv4
    memcpy((void *) &remote_ip4, remote_ip->ip, sizeof(remote_ip4));
    while (mg_span(acl, &entry, &acl, ',')) {
      uint32_t net, mask;
      if (entry.buf[0] != '+' && entry.buf[0] != '-') return -1;
      if (parse_net(&entry.buf[1], &net, &mask) == 0) return -2;
      if ((mg_ntohl(remote_ip4) & mask) == net) allowed = entry.buf[0];
    }
  }
  return allowed == '+';
}

bool mg_path_is_sane(const struct mg_str path) {
  const char *s = path.buf;
  size_t n = path.len;
  if (path.buf[0] == '~') return false;  // Starts with ~
  if (path.buf[0] == '.' && path.buf[1] == '.') return false;  // Starts with ..
  for (; s[0] != '\0' && n > 0; s++, n--) {
    if ((s[0] == '/' || s[0] == '\\') && n >= 2) {   // Subdir?
      if (s[1] == '.' && s[2] == '.') return false;  // Starts with ..
    }
  }
  return true;
}

#if MG_ENABLE_CUSTOM_MILLIS
#else
uint64_t mg_millis(void) {
#if MG_ARCH == MG_ARCH_WIN32
  return GetTickCount();
#elif MG_ARCH == MG_ARCH_PICOSDK
  return time_us_64() / 1000;
#elif MG_ARCH == MG_ARCH_ESP8266 || MG_ARCH == MG_ARCH_ESP32 || \
    MG_ARCH == MG_ARCH_FREERTOS
  return xTaskGetTickCount() * portTICK_PERIOD_MS;
#elif MG_ARCH == MG_ARCH_CUBE
  return (uint64_t) HAL_GetTick();
#elif MG_ARCH == MG_ARCH_THREADX
  return tx_time_get() * (1000 /* MS per SEC */ / TX_TIMER_TICKS_PER_SECOND);
#elif MG_ARCH == MG_ARCH_TIRTOS
  return (uint64_t) Clock_getTicks();
#elif MG_ARCH == MG_ARCH_ZEPHYR
  return (uint64_t) k_uptime_get();
#elif MG_ARCH == MG_ARCH_CMSIS_RTOS1
  return (uint64_t) rt_time_get();
#elif MG_ARCH == MG_ARCH_CMSIS_RTOS2
  return (uint64_t) ((osKernelGetTickCount() * 1000) / osKernelGetTickFreq());
#elif MG_ARCH == MG_ARCH_RTTHREAD
  return (uint64_t) ((rt_tick_get() * 1000) / RT_TICK_PER_SECOND);
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

// network format equates big endian order
uint16_t mg_ntohs(uint16_t net) {
  return MG_LOAD_BE16(&net);
}

uint32_t mg_ntohl(uint32_t net) {
  return MG_LOAD_BE32(&net);
}

uint64_t mg_ntohll(uint64_t net) {
  return MG_LOAD_BE64(&net);
}

void mg_delayms(unsigned int ms) {
  uint64_t to = mg_millis() + ms + 1;
  while (mg_millis() < to) (void) 0;
}


#if MG_ENABLE_CUSTOM_CALLOC
#else
void *mg_calloc(size_t count, size_t size) {
  return calloc(count, size);
}

void mg_free(void *ptr) {
  free(ptr);
}
#endif
