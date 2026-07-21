#pragma once

#include "arch.h"
#include "config.h"
#include "net.h"
#include "sntp.h"
#include "str.h"

#if MG_ENABLE_ASSERT
#include <assert.h>
#elif !defined(assert)
#define assert(x)
#endif

// Wrappers around calloc/free. Override by defining MG_ENABLE_CUSTOM_CALLOC=1
// and providing your own implementations.
void *mg_calloc(size_t count, size_t size);
void mg_free(void *ptr);

// Zeroes len bytes at buf using a volatile write loop that the compiler cannot
// elide. Safe to call with buf=NULL. Use instead of memset() for clearing
// sensitive data (keys, passwords).
void mg_bzero(volatile unsigned char *buf, size_t len);

// Fixed-length constant-time byte equality. Use for MACs, tags, signatures.
bool mg_memeq(const void *a, const void *b, size_t n);

// Fills buf with len cryptographically random bytes. Uses the best available
// hardware or OS source (hardware RNG, /dev/urandom, CryptGenRandom, etc.).
// Falls back to rand() with an error log if no strong source is available.
// Returns true when a strong source was used, false on fallback to rand().
// Override by defining MG_ENABLE_CUSTOM_RANDOM=1 and providing your own impl.
bool mg_random(void *buf, size_t len);

// Fills buf with len-1 random alphanumeric characters ([a-zA-Z0-9]) and
// NUL-terminates. Returns buf.
char *mg_random_str(char *buf, size_t len);

// Computes CRC32 (polynomial 0xEDB88320) over buf/len. Pass crc=0 to start
// a new checksum; pass the result of a prior call to extend over more data.
uint32_t mg_crc32(uint32_t crc, const char *buf, size_t len);

// Returns true if path is safe to serve from the filesystem. Rejects paths
// that start with '~' or '..', or contain a '/../' component, to prevent
// directory traversal attacks.
bool mg_path_is_sane(const struct mg_str path);

// Busy-waits for at least ms milliseconds using mg_millis(). Blocks the
// calling context; avoid in event handlers.
void mg_delayms(unsigned int ms);

uint64_t mg_timegm(unsigned int year, unsigned int month, unsigned int day,
                   unsigned int hour, unsigned int min, unsigned int sec);

// Packs four byte values into a uint32_t in big-endian order.
// MG_U32(1, 2, 3, 4) == 0x01020304
#define MG_U32(a, b, c, d)                                         \
  (((uint32_t) ((a) &255) << 24) | ((uint32_t) ((b) &255) << 16) | \
   ((uint32_t) ((c) &255) << 8) | (uint32_t) ((d) &255))

// Constructs an IPv4 address in network byte order from four decimal octets.
// Usage: uint32_t ip = MG_IPV4(192, 168, 1, 1);
#define MG_IPV4(a, b, c, d) mg_htonl(MG_U32(a, b, c, d))

// Expands to a brace-enclosed byte initialiser for a 16-byte IPv6 address.
// Arguments are the eight 16-bit groups in the address, e.g.:
//   uint8_t ip6[16] = MG_IPV6(0x2001, 0xdb8, 0, 0, 0, 0, 0, 1);
#define MG_IPV6(a, b, c, d, e, f, g ,h) \
  { (uint8_t)((a)>>8),(uint8_t)(a), \
    (uint8_t)((b)>>8),(uint8_t)(b), \
    (uint8_t)((c)>>8),(uint8_t)(c), \
    (uint8_t)((d)>>8),(uint8_t)(d), \
    (uint8_t)((e)>>8),(uint8_t)(e), \
    (uint8_t)((f)>>8),(uint8_t)(f), \
    (uint8_t)((g)>>8),(uint8_t)(g), \
    (uint8_t)((h)>>8),(uint8_t)(h) }

// For printing IPv4 addresses: printf("%d.%d.%d.%d\n", MG_IPADDR_PARTS(&ip))
#define MG_U8P(ADDR) ((uint8_t *) (ADDR))
#define MG_IPADDR_PARTS(ADDR) \
  MG_U8P(ADDR)[0], MG_U8P(ADDR)[1], MG_U8P(ADDR)[2], MG_U8P(ADDR)[3]

// Read an unaligned big-endian value from byte pointer p into a native integer.
// Safe on architectures that forbid unaligned access (e.g. Cortex-M0).
#define MG_LOAD_BE16(p) \
  ((uint16_t) (((uint16_t) MG_U8P(p)[0] << 8U) | MG_U8P(p)[1]))
#define MG_LOAD_BE24(p)                           \
  ((uint32_t) (((uint32_t) MG_U8P(p)[0] << 16U) | \
               ((uint32_t) MG_U8P(p)[1] << 8U) | MG_U8P(p)[2]))
#define MG_LOAD_BE32(p)                           \
  ((uint32_t) (((uint32_t) MG_U8P(p)[0] << 24U) | \
               ((uint32_t) MG_U8P(p)[1] << 16U) | \
               ((uint32_t) MG_U8P(p)[2] << 8U) | MG_U8P(p)[3]))
#define MG_LOAD_BE64(p)                           \
  ((uint64_t) (((uint64_t) MG_U8P(p)[0] << 56U) | \
               ((uint64_t) MG_U8P(p)[1] << 48U) | \
               ((uint64_t) MG_U8P(p)[2] << 40U) | \
               ((uint64_t) MG_U8P(p)[3] << 32U) | \
               ((uint64_t) MG_U8P(p)[4] << 24U) | \
               ((uint64_t) MG_U8P(p)[5] << 16U) | \
               ((uint64_t) MG_U8P(p)[6] << 8U) | MG_U8P(p)[7]))

// Write a native integer to byte pointer p in big-endian byte order.
// Safe on architectures that forbid unaligned access.
#define MG_STORE_BE16(p, n)               \
  do {                                    \
    MG_U8P(p)[0] = (uint8_t) ((n) >> 8U); \
    MG_U8P(p)[1] = (uint8_t) (n);         \
  } while (0)
#define MG_STORE_BE24(p, n)                \
  do {                                     \
    MG_U8P(p)[0] = (uint8_t) ((n) >> 16U); \
    MG_U8P(p)[1] = (uint8_t) ((n) >> 8U);  \
    MG_U8P(p)[2] = (uint8_t) (n);          \
  } while (0)
#define MG_STORE_BE32(p, n)                \
  do {                                     \
    MG_U8P(p)[0] = (uint8_t) ((n) >> 24U); \
    MG_U8P(p)[1] = (uint8_t) ((n) >> 16U); \
    MG_U8P(p)[2] = (uint8_t) ((n) >> 8U);  \
    MG_U8P(p)[3] = (uint8_t) (n);          \
  } while (0)
#define MG_STORE_BE64(p, n)                \
  do {                                     \
    MG_U8P(p)[0] = (uint8_t) ((n) >> 56U); \
    MG_U8P(p)[1] = (uint8_t) ((n) >> 48U); \
    MG_U8P(p)[2] = (uint8_t) ((n) >> 40U); \
    MG_U8P(p)[3] = (uint8_t) ((n) >> 32U); \
    MG_U8P(p)[4] = (uint8_t) ((n) >> 24U); \
    MG_U8P(p)[5] = (uint8_t) ((n) >> 16U); \
    MG_U8P(p)[6] = (uint8_t) ((n) >> 8U);  \
    MG_U8P(p)[7] = (uint8_t) (n);          \
  } while (0)

// Network / host byte-order conversion (big-endian <-> native).
// mg_htons/mg_htonl/mg_htonll are aliases for the same operation (symmetric).
uint16_t mg_ntohs(uint16_t net);
uint32_t mg_ntohl(uint32_t net);
uint64_t mg_ntohll(uint64_t net);
#define mg_htons(x) mg_ntohs(x)
#define mg_htonl(x) mg_ntohl(x)
#define mg_htonll(x) mg_ntohll(x)

// Memory-mapped register access: reads/writes a volatile uint32_t at address x.
#define MG_REG(x) ((volatile uint32_t *) (x))[0]

// Produces a uint32_t with bit x set. x must be 0-31.
#define MG_BIT(x) (((uint32_t) 1U) << (x))

// Clears bits in CLRMASK and sets bits in SETMASK in register R atomically
// (read-modify-write). Example: MG_SET_BITS(MG_REG(addr), 0xF, 0x3);
#define MG_SET_BITS(R, CLRMASK, SETMASK) (R) = ((R) & ~(CLRMASK)) | (SETMASK)

// Round x up / down to the nearest multiple of a. Returns x when a is 0.
#define MG_ROUND_UP(x, a) ((a) == 0 ? (x) : ((((x) + (a) -1) / (a)) * (a)))
#define MG_ROUND_DOWN(x, a) ((a) == 0 ? (x) : (((x) / (a)) * (a)))

#if defined(__GNUC__) && defined(__arm__)
#ifdef __ZEPHYR__
#define MG_ARM_DISABLE_IRQ() __asm__ __volatile__("cpsid i" : : : "memory")
#define MG_ARM_ENABLE_IRQ() __asm__ __volatile__("cpsie i" : : : "memory")
#else
#define MG_ARM_DISABLE_IRQ() asm volatile("cpsid i" : : : "memory")
#define MG_ARM_ENABLE_IRQ() asm volatile("cpsie i" : : : "memory")
#endif // !ZEPHYR
#elif defined(__CCRH__)
#define MG_RH850_DISABLE_IRQ() __DI()
#define MG_RH850_ENABLE_IRQ() __EI()
#else
#define MG_ARM_DISABLE_IRQ()
#define MG_ARM_ENABLE_IRQ()
#endif

#if defined(__CC_ARM)
#define MG_DSB() __dsb(0xf)
#elif defined(__ARMCC_VERSION)
#define MG_DSB() __builtin_arm_dsb(0xf)
#elif defined(__GNUC__) && defined(__arm__) && defined(__thumb__)
#ifdef __ZEPHYR__
#define MG_DSB() __asm__("DSB 0xf")
#else
#define MG_DSB() asm("DSB 0xf")
#endif // !ZEPHYR
#elif defined(__ICCARM__)
#define MG_DSB() __iar_builtin_DSB()
#else
#define MG_DSB()
#endif

struct mg_addr;
int mg_check_ip_acl(struct mg_str acl, struct mg_addr *remote_ip);

// Linked list management macros
#define LIST_ADD_HEAD(type_, head_, elem_) \
  do {                                     \
    (elem_)->next = (*head_);              \
    *(head_) = (elem_);                    \
  } while (0)

#define LIST_ADD_TAIL(type_, head_, elem_) \
  do {                                     \
    type_ **h = head_;                     \
    while (*h != NULL) h = &(*h)->next;    \
    *h = (elem_);                          \
  } while (0)

#define LIST_DELETE(type_, head_, elem_)   \
  do {                                     \
    type_ **h = head_;                     \
    while (*h != (elem_)) h = &(*h)->next; \
    *h = (elem_)->next;                    \
  } while (0)
