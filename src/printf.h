#pragma once

#include "arch.h"
#include "iobuf.h"
#include "queue.h"

// Formats into buf/len. Returns the number of bytes that would be written if
// buf were large enough (like snprintf). Call with buf=NULL, len=0 to measure.
// NUL-terminates if the result fits (n < len).
size_t mg_vsnprintf(char *buf, size_t len, const char *fmt, va_list *ap);
size_t mg_snprintf(char *buf, size_t len, const char *fmt, ...);

// Formats into a heap-allocated NUL-terminated string. Caller must mg_free() it.
char *mg_vmprintf(const char *fmt, va_list *ap);
char *mg_mprintf(const char *fmt, ...);

// Formats into a mg_queue slot. Returns bytes written, or 0 if no space.
size_t mg_queue_printf(struct mg_queue *, const char *fmt, ...);

// Built-in %M/%m printer functions. Each reads its argument(s) from ap.
size_t mg_print_base64(mg_pfn_t, void *arg, va_list *ap);   // expects: const void *buf, size_t len
size_t mg_print_esc(mg_pfn_t, void *arg, va_list *ap);      // expects: int quote, const char *str -- use MG_ESC()
size_t mg_print_hex(mg_pfn_t, void *arg, va_list *ap);      // expects: const void *buf, size_t len
size_t mg_print_ip(mg_pfn_t, void *arg, va_list *ap);       // expects: const struct mg_addr *
size_t mg_print_ip_port(mg_pfn_t, void *arg, va_list *ap);  // expects: const struct mg_addr *
size_t mg_print_ip4(mg_pfn_t, void *arg, va_list *ap);      // expects: uint32_t *ipv4
size_t mg_print_ip6(mg_pfn_t, void *arg, va_list *ap);      // expects: uint8_t[16] ipv6
size_t mg_print_mac(mg_pfn_t, void *arg, va_list *ap);      // expects: uint8_t[6] mac
size_t mg_print_ieee64(mg_pfn_t, void *arg, va_list *ap);   // expects: uint64_t
size_t mg_print_l2addr(mg_pfn_t, void *arg, va_list *ap);   // expects: const struct mg_addr *

// Output functions for use as the fn argument to mg_xprintf/mg_vxprintf.
void mg_pfn_iobuf(char ch, void *param);           // param: struct mg_iobuf * (resizes as needed)
void mg_pfn_iobuf_noresize(char ch, void *param);  // param: struct mg_iobuf * (fixed buffer, no resize)
void mg_pfn_stdout(char c, void *param);           // param: ignored, writes to stdout

// Expands to the three arguments for %m to JSON-escape a string literal.
// Example: mg_snprintf(buf, len, "%m", MG_ESC("hello")) -> "\"hello\""
#define MG_ESC(str) mg_print_esc, 0, (str)
