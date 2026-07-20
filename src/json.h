#pragma once

#include "arch.h"
#include "str.h"

// Maximum JSON nesting depth. mg_json_get() returns MG_JSON_TOO_DEEP if
// the path traverses more than this many nested objects/arrays.
#ifndef MG_JSON_MAX_DEPTH
#define MG_JSON_MAX_DEPTH 30
#endif

// Error codes returned by mg_json_get(). Successful returns are >= 0.
enum { MG_JSON_TOO_DEEP = -1, MG_JSON_INVALID = -2, MG_JSON_NOT_FOUND = -3 };

// Low-level token locator. Finds the JSON token at path inside json and
// returns its byte offset in json.buf. Sets *toklen to the token length.
// toklen may be NULL if only a presence check is needed.
// path must start with "$" (root). Use ".key" for object fields and "[N]"
// for array indices. Example: "$.items[0].name"
// Returns MG_JSON_NOT_FOUND, MG_JSON_INVALID, or MG_JSON_TOO_DEEP on error.
int mg_json_get(struct mg_str json, const char *path, int *toklen);

// Returns the raw JSON token at path.
//
// Returns:
//   Zero-copy mg_str slice into json.buf, or {NULL, 0} if not found or invalid.
// Example:
//   struct mg_str tok = mg_json_get_tok(body, "$.device.name");
// Full examples:
//   src/dash.c
// Related APIs:
//   mg_json_get(), mg_json_get_str(), mg_json_unescape()
// Notes:
//   The returned token is not NUL-terminated. String tokens include the
//   surrounding double quotes and escape sequences; use mg_json_unescape() or
//   mg_json_get_str() when you need decoded string content.
struct mg_str mg_json_get_tok(struct mg_str json, const char *path);

// Parses a numeric JSON value at path into *v.
// Returns true and sets *v on success. Returns false if not found or not a
// number. v may be NULL to check existence only.
bool mg_json_get_num(struct mg_str json, const char *path, double *v);

// Parses a boolean JSON value at path into *v.
// Returns true and sets *v on success. Returns false if not found or not a
// boolean literal. v may be NULL to check existence only.
bool mg_json_get_bool(struct mg_str json, const char *path, bool *v);

// Returns a JSON number at path as a long, or dflt if not found or not a
// number. Use mg_json_get_num() when you need to distinguish missing from 0.
long mg_json_get_long(struct mg_str json, const char *path, long dflt);

// Returns a heap-allocated, NUL-terminated, JSON-unescaped copy of the string
// at path. Caller must mg_free() the result. Returns NULL if not found or the
// token is not a JSON string.
char *mg_json_get_str(struct mg_str json, const char *path);

// Decodes a hex-encoded JSON string at path into a heap-allocated byte array.
// Sets *len to the decoded byte count. Caller must mg_free() the result.
// Returns NULL if not found, not a string, or not valid hexadecimal.
char *mg_json_get_hex(struct mg_str json, const char *path, int *len);

// Decodes a base64-encoded JSON string at path into a heap-allocated byte
// array. Sets *len to the decoded byte count. Caller must mg_free() the result.
// Returns NULL if not found or not a string.
char *mg_json_get_b64(struct mg_str json, const char *path, int *len);

// Writes a JSON-unescaped string value at path into a caller-supplied buffer.
//
// Returns:
//   Number of bytes written excluding the NUL, or 0 on error, not found,
//   non-string token, too-small buffer, or an empty string.
// Example:
//   char name[32];
//   mg_json_unescape(body, "$.device.name", name, sizeof(name));
// Full examples:
//   src/dash.c, src/ota.c
// Related APIs:
//   mg_json_get_tok(), mg_json_get_str(), mg_json_get_num()
// Notes:
//   On success, NUL-terminates when n > 0. The destination buffer is owned by
//   the caller. Only string tokens are unescaped; other token types return 0.
size_t mg_json_unescape(struct mg_str json, const char *path, char *, size_t);

// Sequential iterator over a JSON object or array. Start with ofs=0.
// Each call fills *key and *val with the next entry and returns the new ofs
// to pass on the next call. Returns 0 when there are no more entries.
// For arrays, *key is set to {NULL, 0}. For objects, *key is the raw
// quoted key (e.g. "\"name\""). *val is the raw unparsed token.
size_t mg_json_next(struct mg_str obj, size_t ofs, struct mg_str *key,
                    struct mg_str *val);
