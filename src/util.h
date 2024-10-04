#pragma once

#include "arch.h"
#include "config.h"
#include "net.h"
#include "str.h"

#if MG_ENABLE_ASSERT
#include <assert.h>
#elif !defined(assert)
#define assert(x)
#endif

void mg_bzero(volatile unsigned char *buf, size_t len);
bool mg_random(void *buf, size_t len);
char *mg_random_str(char *buf, size_t len);
uint16_t mg_ntohs(uint16_t net);
uint32_t mg_ntohl(uint32_t net);
uint32_t mg_crc32(uint32_t crc, const char *buf, size_t len);
uint64_t mg_millis(void);  // Return milliseconds since boot
bool mg_path_is_sane(const struct mg_str path);

#define mg_htons(x) mg_ntohs(x)
#define mg_htonl(x) mg_ntohl(x)

#define MG_U32(a, b, c, d)                                           \
  (((uint32_t) ((a) & 255) << 24) | ((uint32_t) ((b) & 255) << 16) | \
   ((uint32_t) ((c) & 255) << 8) | (uint32_t) ((d) & 255))

#define MG_IPV4(a, b, c, d) mg_htonl(MG_U32(a, b, c, d))

// For printing IPv4 addresses: printf("%d.%d.%d.%d\n", MG_IPADDR_PARTS(&ip))
#define MG_U8P(ADDR) ((uint8_t *) (ADDR))
#define MG_IPADDR_PARTS(ADDR) \
  MG_U8P(ADDR)[0], MG_U8P(ADDR)[1], MG_U8P(ADDR)[2], MG_U8P(ADDR)[3]

#define MG_LOAD_BE16(p) ((uint16_t) ((MG_U8P(p)[0] << 8U) | MG_U8P(p)[1]))
#define MG_LOAD_BE24(p) \
  ((uint32_t) ((MG_U8P(p)[0] << 16U) | (MG_U8P(p)[1] << 8U) | MG_U8P(p)[2]))
#define MG_STORE_BE16(p, n)           \
  do {                                \
    MG_U8P(p)[0] = ((n) >> 8U) & 255; \
    MG_U8P(p)[1] = (n) &255;          \
  } while (0)

#define MG_REG(x) ((volatile uint32_t *) (x))[0]
#define MG_BIT(x) (((uint32_t) 1U) << (x))
#define MG_SET_BITS(R, CLRMASK, SETMASK) (R) = ((R) & ~(CLRMASK)) | (SETMASK)

#define MG_ROUND_UP(x, a) ((a) == 0 ? (x) : ((((x) + (a) -1) / (a)) * (a)))
#define MG_ROUND_DOWN(x, a) ((a) == 0 ? (x) : (((x) / (a)) * (a)))

#if defined(__GNUC__)
#define MG_ARM_DISABLE_IRQ() asm volatile("cpsid i" : : : "memory")
#define MG_ARM_ENABLE_IRQ() asm volatile("cpsie i" : : : "memory")
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
#define MG_DSB() asm("DSB 0xf")
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
