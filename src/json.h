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

// Returns the raw JSON token at path as an mg_str slice into json.buf.
// Returns {NULL, 0} if the path is not found or the JSON is invalid.
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
// Returns NULL if not found or not a string.
char *mg_json_get_hex(struct mg_str json, const char *path, int *len);

// Decodes a base64-encoded JSON string at path into a heap-allocated byte
// array. Sets *len to the decoded byte count. Caller must mg_free() the result.
// Returns NULL if not found or not a string.
char *mg_json_get_b64(struct mg_str json, const char *path, int *len);

// Writes the JSON-unescaped string at path into the caller-supplied buffer
// to/n. NUL-terminates on success. Returns the number of bytes written
// (excluding the NUL), or 0 on error or if the path is not found.
size_t mg_json_unescape(struct mg_str json, const char *path, char *, size_t);

// Sequential iterator over a JSON object or array. Start with ofs=0.
// Each call fills *key and *val with the next entry and returns the new ofs
// to pass on the next call. Returns 0 when there are no more entries.
// For arrays, *key is set to {NULL, 0}. For objects, *key is the raw
// quoted key (e.g. "\"name\""). *val is the raw unparsed token.
size_t mg_json_next(struct mg_str obj, size_t ofs, struct mg_str *key,
                    struct mg_str *val);
