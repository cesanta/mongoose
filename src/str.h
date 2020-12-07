#pragma once

#include <ctype.h>
#include <string.h>

struct mg_str {
  const char *ptr;
  size_t len;
};

#define MG_NULL_STR \
  { NULL, 0 }

struct mg_str mg_str(const char *s);
struct mg_str mg_str_n(const char *s, size_t n);
int mg_lower(const char *s);
int mg_ncasecmp(const char *s1, const char *s2, size_t len);
int mg_casecmp(const char *s1, const char *s2);
int mg_vcmp(const struct mg_str *s1, const char *s2);
int mg_vcasecmp(const struct mg_str *str1, const char *str2);
int mg_strcmp(const struct mg_str str1, const struct mg_str str2);
struct mg_str mg_strstrip(struct mg_str s);
struct mg_str mg_strdup(const struct mg_str s);
const char *mg_strstr(const struct mg_str haystack, const struct mg_str needle);
