#pragma once

#include "arch.h"
#include "str.h"

char *mg_file_read(const char *path);
size_t mg_file_size(const char *path);
int mg_file_write(const char *path, const char *fmt, ...);
void mg_random(void *buf, size_t len);
bool mg_globmatch(const char *pattern, int plen, const char *s, int n);
bool mg_next_comma_entry(struct mg_str *s, struct mg_str *k, struct mg_str *v);
uint16_t mg_ntohs(uint16_t net);
uint32_t mg_ntohl(uint32_t net);
char *mg_hexdump(const void *buf, size_t len);
char *mg_hex(const void *buf, int len, char *dst);
void mg_unhex(const char *buf, int len, unsigned char *to);
unsigned long mg_unhexn(const char *s, int len);
int mg_asprintf(char **buf, size_t size, const char *fmt, ...);
int mg_vasprintf(char **buf, size_t size, const char *fmt, va_list ap);
int64_t mg_to64(struct mg_str str);
double mg_time(void);
unsigned long mg_millis(void);
void mg_usleep(unsigned long usecs);

#define mg_htons(x) mg_ntohs(x)
#define mg_htonl(x) mg_ntohl(x)

#define MG_SWAP16(x) ((((x) >> 8) & 255) | ((x & 255) << 8))
#define MG_SWAP32(x) \
  (((x) >> 24) | (((x) &0xff0000) >> 8) | (((x) &0xff00) << 8) | ((x) << 24))

#if !defined(WEAK)
#if (defined(__GNUC__) || defined(__clang__) || \
     defined(__TI_COMPILER_VERSION__)) &&       \
    !defined(_WIN32)
#define WEAK __attribute__((weak))
#else
#define WEAK
#endif
#endif

// Expands to a string representation of its argument: e.g.
// MG_STRINGIFY_LITERAL(5) expands to "5"
#if !defined(_MSC_VER) || _MSC_VER >= 1900
#define MG_STRINGIFY_LITERAL(...) #__VA_ARGS__
#else
#define MG_STRINGIFY_LITERAL(x) #x
#endif

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

// Expands to a string representation of its argument, which can be a macro:
// #define FOO 123
// MG_STRINGIFY_MACRO(FOO)  // Expands to 123
#define MG_STRINGIFY_MACRO(x) MG_STRINGIFY_LITERAL(x)
