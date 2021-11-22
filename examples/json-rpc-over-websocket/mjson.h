// Copyright (c) 2018-2020 Cesanta Software Limited
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef MJSON_H
#define MJSON_H

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#ifndef MJSON_ENABLE_PRINT
#define MJSON_ENABLE_PRINT 1
#endif

#ifndef MJSON_ENABLE_RPC
#define MJSON_ENABLE_RPC 1
#endif

#ifndef MJSON_ENABLE_BASE64
#define MJSON_ENABLE_BASE64 1
#endif

#ifndef MJSON_ENABLE_MERGE
#define MJSON_ENABLE_MERGE 1
#endif

#ifndef MJSON_ENABLE_PRETTY
#define MJSON_ENABLE_PRETTY 1
#endif

#ifndef MJSON_ENABLE_NEXT
#define MJSON_ENABLE_NEXT 1
#endif

#ifndef MJSON_RPC_LIST_NAME
#define MJSON_RPC_LIST_NAME "rpc.list"
#endif

#ifndef MJSON_DYNBUF_CHUNK
#define MJSON_DYNBUF_CHUNK 256  // Allocation granularity for print_dynamic_buf
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define MJSON_ERROR_INVALID_INPUT (-1)
#define MJSON_ERROR_TOO_DEEP (-2)
#define MJSON_TOK_INVALID 0
#define MJSON_TOK_KEY 1
#define MJSON_TOK_STRING 11
#define MJSON_TOK_NUMBER 12
#define MJSON_TOK_TRUE 13
#define MJSON_TOK_FALSE 14
#define MJSON_TOK_NULL 15
#define MJSON_TOK_ARRAY 91
#define MJSON_TOK_OBJECT 123
#define MJSON_TOK_IS_VALUE(t) ((t) > 10 && (t) < 20)

typedef int (*mjson_cb_t)(int ev, const char *s, int off, int len, void *ud);

#ifndef MJSON_MAX_DEPTH
#define MJSON_MAX_DEPTH 20
#endif

int mjson(const char *s, int len, mjson_cb_t cb, void *ud);
int mjson_find(const char *s, int len, const char *jp, const char **, int *);
int mjson_get_number(const char *s, int len, const char *path, double *v);
int mjson_get_bool(const char *s, int len, const char *path, int *v);
int mjson_get_string(const char *s, int len, const char *path, char *to, int n);
int mjson_get_hex(const char *s, int len, const char *path, char *to, int n);

#if MJSON_ENABLE_NEXT
int mjson_next(const char *s, int n, int off, int *koff, int *klen, int *voff,
               int *vlen, int *vtype);
#endif

#if MJSON_ENABLE_BASE64
int mjson_get_base64(const char *s, int len, const char *path, char *to, int n);
int mjson_base64_dec(const char *src, int n, char *dst, int dlen);
#endif

#if MJSON_ENABLE_PRINT
typedef int (*mjson_print_fn_t)(const char *buf, int len, void *userdata);
typedef int (*mjson_vprint_fn_t)(mjson_print_fn_t, void *, va_list *);

struct mjson_fixedbuf {
  char *ptr;
  int size, len;
};

int mjson_printf(mjson_print_fn_t, void *, const char *fmt, ...);
int mjson_vprintf(mjson_print_fn_t, void *, const char *fmt, va_list *ap);
int mjson_print_str(mjson_print_fn_t, void *, const char *s, int len);
int mjson_print_int(mjson_print_fn_t, void *, int value, int is_signed);
int mjson_print_long(mjson_print_fn_t, void *, long value, int is_signed);
int mjson_print_buf(mjson_print_fn_t fn, void *, const char *buf, int len);
int mjson_print_dbl(mjson_print_fn_t fn, void *, double, int width);

int mjson_print_null(const char *ptr, int len, void *userdata);
int mjson_print_fixed_buf(const char *ptr, int len, void *userdata);
int mjson_print_dynamic_buf(const char *ptr, int len, void *userdata);

int mjson_snprintf(char *buf, size_t len, const char *fmt, ...);
char *mjson_aprintf(const char *fmt, ...);

#if MJSON_ENABLE_PRETTY
int mjson_pretty(const char *, int, const char *, mjson_print_fn_t, void *);
#endif

#if MJSON_ENABLE_MERGE
int mjson_merge(const char *, int, const char *, int, mjson_print_fn_t, void *);
#endif

#endif  // MJSON_ENABLE_PRINT

#if MJSON_ENABLE_RPC

void jsonrpc_init(mjson_print_fn_t, void *userdata);
int mjson_globmatch(const char *s1, int n1, const char *s2, int n2);

struct jsonrpc_request {
  struct jsonrpc_ctx *ctx;
  const char *frame;    // Points to the whole frame
  int frame_len;        // Frame length
  const char *params;   // Points to the "params" in the request frame
  int params_len;       // Length of the "params"
  const char *id;       // Points to the "id" in the request frame
  int id_len;           // Length of the "id"
  const char *method;   // Points to the "method" in the request frame
  int method_len;       // Length of the "method"
  mjson_print_fn_t fn;  // Printer function
  void *fndata;         // Printer function data
  void *userdata;       // Callback's user data as specified at export time
};

struct jsonrpc_method {
  const char *method;
  int method_sz;
  void (*cb)(struct jsonrpc_request *);
  struct jsonrpc_method *next;
};

// Main RPC context, stores current request information and a list of
// exported RPC methods.
struct jsonrpc_ctx {
  struct jsonrpc_method *methods;
  mjson_print_fn_t response_cb;
  void *response_cb_data;
};

// Registers function fn under the given name within the given RPC context
#define jsonrpc_ctx_export(ctx, name, fn)                                 \
  do {                                                                    \
    static struct jsonrpc_method m = {(name), sizeof(name) - 1, (fn), 0}; \
    m.next = (ctx)->methods;                                              \
    (ctx)->methods = &m;                                                  \
  } while (0)

void jsonrpc_ctx_init(struct jsonrpc_ctx *ctx, mjson_print_fn_t, void *);
void jsonrpc_return_error(struct jsonrpc_request *r, int code,
                          const char *message, const char *data_fmt, ...);
void jsonrpc_return_success(struct jsonrpc_request *r, const char *result_fmt,
                            ...);
void jsonrpc_ctx_process(struct jsonrpc_ctx *ctx, const char *req, int req_sz,
                         mjson_print_fn_t fn, void *fndata, void *userdata);

extern struct jsonrpc_ctx jsonrpc_default_context;
extern void jsonrpc_list(struct jsonrpc_request *r);

#define jsonrpc_export(name, fn) \
  jsonrpc_ctx_export(&jsonrpc_default_context, (name), (fn))

#define jsonrpc_process(buf, len, fn, fnd, ud) \
  jsonrpc_ctx_process(&jsonrpc_default_context, (buf), (len), (fn), (fnd), (ud))

#define JSONRPC_ERROR_INVALID -32700    /* Invalid JSON was received */
#define JSONRPC_ERROR_NOT_FOUND -32601  /* The method does not exist */
#define JSONRPC_ERROR_BAD_PARAMS -32602 /* Invalid params passed */
#define JSONRPC_ERROR_INTERNAL -32603   /* Internal JSON-RPC error */

#endif  // MJSON_ENABLE_RPC
#ifdef __cplusplus
}
#endif
#endif  // MJSON_H
