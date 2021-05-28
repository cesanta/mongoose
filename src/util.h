#pragma once

#include "arch.h"
#include "str.h"

// WEAK symbol makes it possible to define a "default" function implementation,
// which could be overridden by the user who can define a function with the
// same name without linking conflict
#if !defined(WEAK)
#if (defined(__GNUC__) || defined(__clang__) || \
     defined(__TI_COMPILER_VERSION__)) &&       \
    !defined(_WIN32) && !defined(__CYGWIN__)
#define WEAK __attribute__((weak))
#else
#define WEAK
#endif
#endif

char *mg_file_read(const char *path, size_t *size);
int64_t mg_file_size(const char *path);
bool mg_file_write(const char *path, const void *buf, size_t len);
bool mg_file_printf(const char *path, const char *fmt, ...);
void mg_random(void *buf, size_t len) WEAK;
bool mg_globmatch(const char *pattern, size_t plen, const char *s, size_t n);
bool mg_next_comma_entry(struct mg_str *s, struct mg_str *k, struct mg_str *v);
uint16_t mg_ntohs(uint16_t net);
uint32_t mg_ntohl(uint32_t net);
uint32_t mg_crc32(uint32_t crc, const char *buf, size_t len);
char *mg_hexdump(const void *buf, size_t len);
char *mg_hex(const void *buf, size_t len, char *dst);
void mg_unhex(const char *buf, size_t len, unsigned char *to);
unsigned long mg_unhexn(const char *s, size_t len);
int mg_asprintf(char **buf, size_t size, const char *fmt, ...);
int mg_vasprintf(char **buf, size_t size, const char *fmt, va_list ap);
int64_t mg_to64(struct mg_str str);
double mg_time(void);
unsigned long mg_millis(void);
void mg_usleep(unsigned long usecs);

#if MG_ENABLE_FS
#ifdef _WIN32
typedef struct _stati64 mg_stat_t;
#else
typedef struct stat mg_stat_t;
#endif
int mg_stat(const char *path, mg_stat_t *);
FILE *mg_fopen(const char *fp, const char *mode);
#endif

#define mg_htons(x) mg_ntohs(x)
#define mg_htonl(x) mg_ntohl(x)

// Expands to a string representation of its argument: e.g.
// MG_STRINGIFY_LITERAL(5) expands to "5"
#if !defined(_MSC_VER) || _MSC_VER >= 1900
#define MG_STRINGIFY_LITERAL(...) #__VA_ARGS__
#else
#define MG_STRINGIFY_LITERAL(x) #x
#endif
// Expands to a string representation of its argument, which can be a macro:
// #define FOO 123
// MG_STRINGIFY_MACRO(FOO)  // Expands to 123
#define MG_STRINGIFY_MACRO(x) MG_STRINGIFY_LITERAL(x)

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
