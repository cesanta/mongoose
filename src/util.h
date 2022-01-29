#pragma once

#include "arch.h"
#include "config.h"
#include "str.h"

void mg_random(void *buf, size_t len);
uint16_t mg_ntohs(uint16_t net);
uint32_t mg_ntohl(uint32_t net);
uint32_t mg_crc32(uint32_t crc, const char *buf, size_t len);
char *mg_hexdump(const void *buf, size_t len);
char *mg_hex(const void *buf, size_t len, char *dst);
void mg_unhex(const char *buf, size_t len, unsigned char *to);
unsigned long mg_unhexn(const char *s, size_t len);
int mg_asprintf(char **buf, size_t size, const char *fmt, ...);
int mg_vasprintf(char **buf, size_t size, const char *fmt, va_list ap);
int mg_check_ip_acl(struct mg_str acl, uint32_t remote_ip);
int64_t mg_to64(struct mg_str str);
int64_t mg_millis(void);

#define mg_htons(x) mg_ntohs(x)
#define mg_htonl(x) mg_ntohl(x)

#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif
#endif

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
