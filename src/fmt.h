#pragma once

#include "arch.h"
#include "iobuf.h"

typedef void (*mg_pfn_t)(char, void *);                  // Custom putchar
typedef size_t (*mg_pm_t)(mg_pfn_t, void *, va_list *);  // %M printer
void mg_pfn_iobuf(char ch, void *param);                 // iobuf printer

size_t mg_lld(char *buf, int64_t val, bool is_signed, bool is_hex);
double mg_atod(const char *buf, int len, int *numlen);
size_t mg_dtoa(char *buf, size_t len, double d, int width);

size_t mg_vrprintf(void (*)(char, void *), void *, const char *fmt, va_list *);
size_t mg_rprintf(void (*fn)(char, void *), void *, const char *fmt, ...);
size_t mg_vsnprintf(char *buf, size_t len, const char *fmt, va_list *ap);
size_t mg_snprintf(char *, size_t, const char *fmt, ...);
size_t mg_asprintf(char **, size_t, const char *fmt, ...);
size_t mg_vasprintf(char **buf, size_t size, const char *fmt, va_list ap);
char *mg_mprintf(const char *fmt, ...);
char *mg_vmprintf(const char *fmt, va_list ap);
