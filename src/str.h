#pragma once

#include "arch.h"

// Describes an arbitrary chunk of memory
struct mg_str {
  char *buf;   // String data
  size_t len;  // String length
};

// Using macro to avoid shadowing C++ struct constructor, see #1298
#define mg_str(s) mg_str_s(s)

struct mg_str mg_str(const char *s);
struct mg_str mg_str_n(const char *s, size_t n);
int mg_casecmp(const char *s1, const char *s2);
int mg_strcmp(const struct mg_str str1, const struct mg_str str2);
int mg_strcasecmp(const struct mg_str str1, const struct mg_str str2);  // this one is new
bool mg_match(struct mg_str str, struct mg_str pattern, struct mg_str *caps);
bool mg_span(struct mg_str s, struct mg_str *a, struct mg_str *b, char delim);

void mg_unhex(const char *buf, size_t len, unsigned char *to);
unsigned long mg_unhexn(const char *s, size_t len);

uint8_t mg_toi(char c, int base); // base: 16, 10
