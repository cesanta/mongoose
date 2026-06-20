#pragma once

#include "arch.h"

// A non-owning view of an arbitrary byte buffer. Not NUL-terminated in general.
// Use mg_str() or mg_str_n() to construct; use mg_strdup() for an owned copy.
struct mg_str {
  char *buf;   // Pointer to the data (not necessarily NUL-terminated)
  size_t len;  // Number of bytes
};

// Using macro to avoid shadowing the C++ struct constructor, see #1298
#define mg_str(s) mg_str_s(s)

// Creates an mg_str from a NUL-terminated C string. Computes length via
// strlen. Returns {NULL, 0} when s is NULL.
struct mg_str mg_str(const char *s);

// Creates an mg_str from a pointer and explicit byte count. s need not be
// NUL-terminated. Does not copy the data.
struct mg_str mg_str_n(const char *s, size_t n);

// Case-insensitive strcmp for NUL-terminated C strings.
// Returns 0 if equal, <0 if s1 < s2, >0 if s1 > s2.
int mg_casecmp(const char *s1, const char *s2);

// Compares two mg_str by content and length. Returns 0 if identical,
// -1 if str1 < str2, 1 if str1 > str2. Length is used as a tiebreaker
// when one string is a prefix of the other.
int mg_strcmp(const struct mg_str str1, const struct mg_str str2);

// Case-insensitive mg_strcmp.
int mg_strcasecmp(const struct mg_str str1, const struct mg_str str2);

// Returns a heap-allocated, NUL-terminated copy of s.
// Returns {NULL, 0} on OOM or if s.buf is NULL or s.len is 0.
// Caller must mg_free(result.buf) when done.
struct mg_str mg_strdup(const struct mg_str s);

// Matches a string against a glob pattern.
//
// Returns:
//   True if the whole str matches pattern.
// Example:
//   if (mg_match(hm->uri, mg_str("/api/#"), NULL)) { ... }
// Full examples:
//   tutorials/http/http-server, tutorials/http/link-checker,
//   tutorials/websocket/websocket-server
// Related APIs:
//   mg_str(), mg_str_n(), mg_span(), mg_strcmp(), mg_strcasecmp()
// Notes:
//   Pattern wildcards: ? matches one character, * matches any sequence except
//   '/', and # matches any sequence including '/'. If caps is not NULL, each
//   wildcard capture is stored as a zero-copy mg_str slice into str. Pass
//   caps=NULL when captures are not needed. The caps array size should be
//   at least the number of wildcards in a pattern plus one. The last cap
//   is initialised to an empty string.
//   Attention: the pattern must consume the whole str in order to match.
bool mg_match(struct mg_str str, struct mg_str pattern, struct mg_str *caps);

// Splits s at the first occurrence of sep. Sets *a to the part before sep
// and *b to the part after it (sep is not included in either). Both slices
// are zero-copy views into s. a or b may be NULL if that half is not needed.
// If sep is not found, *a is the whole string and *b is empty.
// Returns false if s is empty or s.buf is NULL.
// Example - scan comma-separated key=value pairs:
// ```c
//   struct mg_str entry, key, val;
//   struct mg_str s = mg_str("a=333,b=777,hello");
//
//   while (mg_span(s, &entry, &s, ',')) {
//     mg_span(entry, &key, &val, '=');
//     printf("key: %.*s, val: %.*s\n", (int) key.len, key.buf, (int) val.len, val.buf);
//   }
// ```
bool mg_span(struct mg_str s, struct mg_str *a, struct mg_str *b, char sep);

// Parses str as an unsigned integer into the val_len-byte value at *val.
// val_len must be 1, 2, 4, or 8 (for uint8_t, uint16_t, uint32_t, uint64_t).
// base must be 2, 10, or 16; pass 0 to auto-detect from a "0b" or "0x" prefix.
// The entire string must consist of valid digits with no trailing characters.
// Returns false on overflow, invalid input, unsupported base, or unsupported val_len.
bool mg_str_to_num(struct mg_str str, int base, void *val, size_t val_len);
