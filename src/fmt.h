#pragma once

#include "arch.h"

// Character output function. Called once per output character by mg_xprintf/mg_vxprintf.
// arg is an arbitrary user pointer passed through from the caller.
typedef void (*mg_pfn_t)(char, void *arg);

// Custom %M/%m printer. Reads its own arguments from ap and writes via fn.
// Returns the number of characters printed (excluding the quotes added by %m).
typedef size_t (*mg_pm_t)(mg_pfn_t fn, void *arg, va_list *);

// Formats fmt with printf-like specifiers, calling fn for each output character.
// Returns the total number of characters printed. Supported specifiers:
//  - %c - expect char
//  - %f, %g - expect double
//  - %hhd, %hd, %d, %ld, %lld - for char, short, int, long, int64_t
//  - %hhu, %hu, %u, %lu, %llu - same but for unsigned variants
//  - %hhx, %hx, %x, %lx, %llx - same, unsigned and hex output
//  - %p - expects any pointer, prints 0x..... hex value
//  - %s - expects char *
//  - %% - prints the % character itself
//  - %X.Y - optional width and precision modifiers (e.g.: %1.2d)
//  - %.* - optional precision modifier, expected as int argument (e.g.: %.*d)
//  - %M - (EXTENSION) expects a pointer to a custom print function and its arguments
//  - %m - (EXTENSION) exactly like %M but double-quotes the output
//  - %M/%m (custom mg_pm_t printer; %m additionally wraps the output in quotes).
//
// There are higher level wrappers around these functions that use
// predefined output functions, and can print to char buffer, or connection
// send iobuffer, etc etc. - for example, mg_snrptinf(). The %M/%m non-standard
// specifier allows to use custom formtter. There are several pre-defined
// formatters:
//   - mg_print_base64 - prints a buffer as a base64-encoded string
//   - mg_print_esc - prints a JSON-escaped string
//   - mg_print_hex - prints a buffer as a hex string
//   - mg_print_ip - prints an IP address in a struct mg_str
//   - mg_print_ip_port - prints IP address and port in a struct mg_str
//   - mg_print_ip4 - prints an IPv4 address
//   - mg_print_ip6 - prints an IPv6 address
//   - mg_print_mac - prints a MAC address
//
// Usage example:
//
// ```c
//   struct a { int a, b; };
//
//   size_t print_a(mg_pfn_t fn, void *arg, va_list *ap) {
//     struct a *p = va_arg(*ap, struct a *);
//     return mg_xprintf(fn, arg, "{%m:%d}", MG_ESC("a"), p->a);
//   }
//
//   struct a temp = { 42, 43 };
//   char buf[100];
//
//   mg_snprintf(buf, sizeof(buf), "%M", print_a, &temp);    // {"a":42}
// ```
size_t mg_vxprintf(mg_pfn_t fn, void *arg, const char *fmt, va_list *);
size_t mg_xprintf(mg_pfn_t fn, void *arg, const char *fmt, ...);
