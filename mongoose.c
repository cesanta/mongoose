#include "mongoose.h"
#ifdef MG_MODULE_LINES
#line 1 "./src/internal.h"
#endif
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_MONGOOSE_SRC_INTERNAL_H_
#define CS_MONGOOSE_SRC_INTERNAL_H_

#ifndef MG_MALLOC
#define MG_MALLOC malloc
#endif

#ifndef MG_CALLOC
#define MG_CALLOC calloc
#endif

#ifndef MG_REALLOC
#define MG_REALLOC realloc
#endif

#ifndef MG_FREE
#define MG_FREE free
#endif

#ifndef MBUF_REALLOC
#define MBUF_REALLOC MG_REALLOC
#endif

#ifndef MBUF_FREE
#define MBUF_FREE MG_FREE
#endif

#define MG_SET_PTRPTR(_ptr, _v) \
  do {                          \
    if (_ptr) *(_ptr) = _v;     \
  } while (0)

#ifndef MG_INTERNAL
#define MG_INTERNAL static
#endif

#ifdef PICOTCP
#define NO_LIBC
#define MG_DISABLE_FILESYSTEM
#define MG_DISABLE_POPEN
#define MG_DISABLE_CGI
#define MG_DISABLE_DIRECTORY_LISTING
#define MG_DISABLE_SOCKETPAIR
#define MG_DISABLE_PFS
#endif

/* Amalgamated: #include "mongoose/src/net.h" */
/* Amalgamated: #include "mongoose/src/http.h" */

#define MG_CTL_MSG_MESSAGE_SIZE 8192

/* internals that need to be accessible in unit tests */
MG_INTERNAL struct mg_connection *mg_do_connect(struct mg_connection *nc,
                                                int proto,
                                                union socket_address *sa);

MG_INTERNAL int mg_parse_address(const char *str, union socket_address *sa,
                                 int *proto, char *host, size_t host_len);
MG_INTERNAL void mg_call(struct mg_connection *nc,
                         mg_event_handler_t ev_handler, int ev, void *ev_data);
void mg_forward(struct mg_connection *from, struct mg_connection *to);
MG_INTERNAL void mg_add_conn(struct mg_mgr *mgr, struct mg_connection *c);
MG_INTERNAL void mg_remove_conn(struct mg_connection *c);
MG_INTERNAL struct mg_connection *mg_create_connection(
    struct mg_mgr *mgr, mg_event_handler_t callback,
    struct mg_add_sock_opts opts);
#ifndef MG_DISABLE_FILESYSTEM
MG_INTERNAL int mg_uri_to_local_path(struct http_message *hm,
                                     const struct mg_serve_http_opts *opts,
                                     char **local_path,
                                     struct mg_str *remainder);
#endif
#ifdef _WIN32
/* Retur value is the same as for MultiByteToWideChar. */
int to_wchar(const char *path, wchar_t *wbuf, size_t wbuf_len);
#endif

/*
 * Reassemble the content of the buffer (buf, blen) which should be
 * in the HTTP chunked encoding, by collapsing data chunks to the
 * beginning of the buffer.
 *
 * If chunks get reassembled, modify hm->body to point to the reassembled
 * body and fire MG_EV_HTTP_CHUNK event. If handler sets MG_F_DELETE_CHUNK
 * in nc->flags, delete reassembled body from the mbuf.
 *
 * Return reassembled body size.
 */
MG_INTERNAL size_t mg_handle_chunked(struct mg_connection *nc,
                                     struct http_message *hm, char *buf,
                                     size_t blen);

#ifndef MG_DISABLE_FILESYSTEM
MG_INTERNAL time_t mg_parse_date_string(const char *datetime);
MG_INTERNAL int mg_is_not_modified(struct http_message *hm, cs_stat_t *st);
#endif

struct ctl_msg {
  mg_event_handler_t callback;
  char message[MG_CTL_MSG_MESSAGE_SIZE];
};

#ifndef MG_DISABLE_MQTT
struct mg_mqtt_message;
MG_INTERNAL int parse_mqtt(struct mbuf *io, struct mg_mqtt_message *mm);
#endif

/* Forward declarations for testing. */
extern void *(*test_malloc)(size_t size);
extern void *(*test_calloc)(size_t count, size_t size);

#endif /* CS_MONGOOSE_SRC_INTERNAL_H_ */
#ifdef MG_MODULE_LINES
#line 1 "./src/../../common/base64.c"
#endif
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#ifndef EXCLUDE_COMMON

/* Amalgamated: #include "common/base64.h" */
#include <string.h>

/* ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/ */

#define NUM_UPPERCASES ('Z' - 'A' + 1)
#define NUM_LETTERS (NUM_UPPERCASES * 2)
#define NUM_DIGITS ('9' - '0' + 1)

/*
 * Emit a base64 code char.
 *
 * Doesn't use memory, thus it's safe to use to safely dump memory in crashdumps
 */
static void cs_base64_emit_code(struct cs_base64_ctx *ctx, int v) {
  if (v < NUM_UPPERCASES) {
    ctx->b64_putc(v + 'A', ctx->user_data);
  } else if (v < (NUM_LETTERS)) {
    ctx->b64_putc(v - NUM_UPPERCASES + 'a', ctx->user_data);
  } else if (v < (NUM_LETTERS + NUM_DIGITS)) {
    ctx->b64_putc(v - NUM_LETTERS + '0', ctx->user_data);
  } else {
    ctx->b64_putc(v - NUM_LETTERS - NUM_DIGITS == 0 ? '+' : '/',
                  ctx->user_data);
  }
}

static void cs_base64_emit_chunk(struct cs_base64_ctx *ctx) {
  int a, b, c;

  a = ctx->chunk[0];
  b = ctx->chunk[1];
  c = ctx->chunk[2];

  cs_base64_emit_code(ctx, a >> 2);
  cs_base64_emit_code(ctx, ((a & 3) << 4) | (b >> 4));
  if (ctx->chunk_size > 1) {
    cs_base64_emit_code(ctx, (b & 15) << 2 | (c >> 6));
  }
  if (ctx->chunk_size > 2) {
    cs_base64_emit_code(ctx, c & 63);
  }
}

void cs_base64_init(struct cs_base64_ctx *ctx, cs_base64_putc_t b64_putc,
                    void *user_data) {
  ctx->chunk_size = 0;
  ctx->b64_putc = b64_putc;
  ctx->user_data = user_data;
}

void cs_base64_update(struct cs_base64_ctx *ctx, const char *str, size_t len) {
  const unsigned char *src = (const unsigned char *) str;
  size_t i;
  for (i = 0; i < len; i++) {
    ctx->chunk[ctx->chunk_size++] = src[i];
    if (ctx->chunk_size == 3) {
      cs_base64_emit_chunk(ctx);
      ctx->chunk_size = 0;
    }
  }
}

void cs_base64_finish(struct cs_base64_ctx *ctx) {
  if (ctx->chunk_size > 0) {
    int i;
    memset(&ctx->chunk[ctx->chunk_size], 0, 3 - ctx->chunk_size);
    cs_base64_emit_chunk(ctx);
    for (i = 0; i < (3 - ctx->chunk_size); i++) {
      ctx->b64_putc('=', ctx->user_data);
    }
  }
}

#define BASE64_ENCODE_BODY                                                \
  static const char *b64 =                                                \
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; \
  int i, j, a, b, c;                                                      \
                                                                          \
  for (i = j = 0; i < src_len; i += 3) {                                  \
    a = src[i];                                                           \
    b = i + 1 >= src_len ? 0 : src[i + 1];                                \
    c = i + 2 >= src_len ? 0 : src[i + 2];                                \
                                                                          \
    BASE64_OUT(b64[a >> 2]);                                              \
    BASE64_OUT(b64[((a & 3) << 4) | (b >> 4)]);                           \
    if (i + 1 < src_len) {                                                \
      BASE64_OUT(b64[(b & 15) << 2 | (c >> 6)]);                          \
    }                                                                     \
    if (i + 2 < src_len) {                                                \
      BASE64_OUT(b64[c & 63]);                                            \
    }                                                                     \
  }                                                                       \
                                                                          \
  while (j % 4 != 0) {                                                    \
    BASE64_OUT('=');                                                      \
  }                                                                       \
  BASE64_FLUSH()

#define BASE64_OUT(ch) \
  do {                 \
    dst[j++] = (ch);   \
  } while (0)

#define BASE64_FLUSH() \
  do {                 \
    dst[j++] = '\0';   \
  } while (0)

void cs_base64_encode(const unsigned char *src, int src_len, char *dst) {
  BASE64_ENCODE_BODY;
}

#undef BASE64_OUT
#undef BASE64_FLUSH

#ifndef CS_DISABLE_STDIO
#define BASE64_OUT(ch)      \
  do {                      \
    fprintf(f, "%c", (ch)); \
    j++;                    \
  } while (0)

#define BASE64_FLUSH()

void cs_fprint_base64(FILE *f, const unsigned char *src, int src_len) {
  BASE64_ENCODE_BODY;
}

#undef BASE64_OUT
#undef BASE64_FLUSH
#endif /* !CS_DISABLE_STDIO */

/* Convert one byte of encoded base64 input stream to 6-bit chunk */
static unsigned char from_b64(unsigned char ch) {
  /* Inverse lookup map */
  static const unsigned char tab[128] = {
      255, 255, 255, 255,
      255, 255, 255, 255, /*  0 */
      255, 255, 255, 255,
      255, 255, 255, 255, /*  8 */
      255, 255, 255, 255,
      255, 255, 255, 255, /*  16 */
      255, 255, 255, 255,
      255, 255, 255, 255, /*  24 */
      255, 255, 255, 255,
      255, 255, 255, 255, /*  32 */
      255, 255, 255, 62,
      255, 255, 255, 63, /*  40 */
      52,  53,  54,  55,
      56,  57,  58,  59, /*  48 */
      60,  61,  255, 255,
      255, 200, 255, 255, /*  56   '=' is 200, on index 61 */
      255, 0,   1,   2,
      3,   4,   5,   6, /*  64 */
      7,   8,   9,   10,
      11,  12,  13,  14, /*  72 */
      15,  16,  17,  18,
      19,  20,  21,  22, /*  80 */
      23,  24,  25,  255,
      255, 255, 255, 255, /*  88 */
      255, 26,  27,  28,
      29,  30,  31,  32, /*  96 */
      33,  34,  35,  36,
      37,  38,  39,  40, /*  104 */
      41,  42,  43,  44,
      45,  46,  47,  48, /*  112 */
      49,  50,  51,  255,
      255, 255, 255, 255, /*  120 */
  };
  return tab[ch & 127];
}

int cs_base64_decode(const unsigned char *s, int len, char *dst) {
  unsigned char a, b, c, d;
  int orig_len = len;
  while (len >= 4 && (a = from_b64(s[0])) != 255 &&
         (b = from_b64(s[1])) != 255 && (c = from_b64(s[2])) != 255 &&
         (d = from_b64(s[3])) != 255) {
    s += 4;
    len -= 4;
    if (a == 200 || b == 200) break; /* '=' can't be there */
    *dst++ = a << 2 | b >> 4;
    if (c == 200) break;
    *dst++ = b << 4 | c >> 2;
    if (d == 200) break;
    *dst++ = c << 6 | d;
  }
  *dst = 0;
  return orig_len - len;
}

#endif /* EXCLUDE_COMMON */
#ifdef MG_MODULE_LINES
#line 1 "./src/../../common/cs_dbg.c"
#endif
/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

/* Amalgamated: #include "common/cs_dbg.h" */

#include <stdarg.h>
#include <stdio.h>

/* Amalgamated: #include "common/cs_time.h" */

enum cs_log_level cs_log_level =
#ifdef CS_ENABLE_DEBUG
    LL_VERBOSE_DEBUG;
#else
    LL_ERROR;
#endif

#ifndef CS_DISABLE_STDIO

FILE *cs_log_file = NULL;

#ifdef CS_LOG_TS_DIFF
double cs_log_ts;
#endif

void cs_log_print_prefix(const char *func) {
  if (cs_log_file == NULL) cs_log_file = stderr;
  fprintf(cs_log_file, "%-20s ", func);
#ifdef CS_LOG_TS_DIFF
  {
    double now = cs_time();
    fprintf(cs_log_file, "%7u ", (unsigned int) ((now - cs_log_ts) * 1000000));
    cs_log_ts = now;
  }
#endif
}

void cs_log_printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(cs_log_file, fmt, ap);
  va_end(ap);
  fputc('\n', cs_log_file);
  fflush(cs_log_file);
}

void cs_log_set_file(FILE *file) {
  cs_log_file = file;
}

#endif /* !CS_DISABLE_STDIO */

void cs_log_set_level(enum cs_log_level level) {
  cs_log_level = level;
#if defined(CS_LOG_TS_DIFF) && !defined(CS_DISABLE_STDIO)
  cs_log_ts = cs_time();
#endif
}
#ifdef MG_MODULE_LINES
#line 1 "./src/../../common/cs_dirent.h"
#endif
/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_CS_DIRENT_H_
#define CS_COMMON_CS_DIRENT_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef CS_ENABLE_SPIFFS

#include <spiffs.h>

typedef struct {
  spiffs_DIR dh;
  struct spiffs_dirent de;
} DIR;

#define d_name name
#define dirent spiffs_dirent

int rmdir(const char *path);
int mkdir(const char *path, mode_t mode);

#endif

#if defined(_WIN32)
struct dirent {
  char d_name[MAX_PATH];
};

typedef struct DIR {
  HANDLE handle;
  WIN32_FIND_DATAW info;
  struct dirent result;
} DIR;
#endif

#if defined(_WIN32) || defined(CS_ENABLE_SPIFFS)
DIR *opendir(const char *dir_name);
int closedir(DIR *dir);
struct dirent *readdir(DIR *dir);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CS_COMMON_CS_DIRENT_H_ */
#ifdef MG_MODULE_LINES
#line 1 "./src/../../common/cs_dirent.c"
#endif
/*
 * Copyright (c) 2015 Cesanta Software Limited
 * All rights reserved
 */

#ifndef EXCLUDE_COMMON

/* Amalgamated: #include "common/cs_dirent.h" */

/*
 * This file contains POSIX opendir/closedir/readdir API implementation
 * for systems which do not natively support it (e.g. Windows).
 */

#ifndef MG_FREE
#define MG_FREE free
#endif

#ifndef MG_MALLOC
#define MG_MALLOC malloc
#endif

#ifdef _WIN32
DIR *opendir(const char *name) {
  DIR *dir = NULL;
  wchar_t wpath[MAX_PATH];
  DWORD attrs;

  if (name == NULL) {
    SetLastError(ERROR_BAD_ARGUMENTS);
  } else if ((dir = (DIR *) MG_MALLOC(sizeof(*dir))) == NULL) {
    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
  } else {
    to_wchar(name, wpath, ARRAY_SIZE(wpath));
    attrs = GetFileAttributesW(wpath);
    if (attrs != 0xFFFFFFFF && (attrs & FILE_ATTRIBUTE_DIRECTORY)) {
      (void) wcscat(wpath, L"\\*");
      dir->handle = FindFirstFileW(wpath, &dir->info);
      dir->result.d_name[0] = '\0';
    } else {
      MG_FREE(dir);
      dir = NULL;
    }
  }

  return dir;
}

int closedir(DIR *dir) {
  int result = 0;

  if (dir != NULL) {
    if (dir->handle != INVALID_HANDLE_VALUE)
      result = FindClose(dir->handle) ? 0 : -1;
    MG_FREE(dir);
  } else {
    result = -1;
    SetLastError(ERROR_BAD_ARGUMENTS);
  }

  return result;
}

struct dirent *readdir(DIR *dir) {
  struct dirent *result = NULL;

  if (dir) {
    if (dir->handle != INVALID_HANDLE_VALUE) {
      result = &dir->result;
      (void) WideCharToMultiByte(CP_UTF8, 0, dir->info.cFileName, -1,
                                 result->d_name, sizeof(result->d_name), NULL,
                                 NULL);

      if (!FindNextFileW(dir->handle, &dir->info)) {
        (void) FindClose(dir->handle);
        dir->handle = INVALID_HANDLE_VALUE;
      }

    } else {
      SetLastError(ERROR_FILE_NOT_FOUND);
    }
  } else {
    SetLastError(ERROR_BAD_ARGUMENTS);
  }

  return result;
}
#endif

#ifdef CS_ENABLE_SPIFFS

DIR *opendir(const char *dir_name) {
  DIR *dir = NULL;
  extern spiffs fs;

  if (dir_name != NULL && (dir = (DIR *) malloc(sizeof(*dir))) != NULL &&
      SPIFFS_opendir(&fs, (char *) dir_name, &dir->dh) == NULL) {
    free(dir);
    dir = NULL;
  }

  return dir;
}

int closedir(DIR *dir) {
  if (dir != NULL) {
    SPIFFS_closedir(&dir->dh);
    free(dir);
  }
  return 0;
}

struct dirent *readdir(DIR *dir) {
  return SPIFFS_readdir(&dir->dh, &dir->de);
}

/* SPIFFs doesn't support directory operations */
int rmdir(const char *path) {
  (void) path;
  return ENOTDIR;
}

int mkdir(const char *path, mode_t mode) {
  (void) path;
  (void) mode;
  /* for spiffs supports only root dir, which comes from mongoose as '.' */
  return (strlen(path) == 1 && *path == '.') ? 0 : ENOTDIR;
}

#endif /* CS_ENABLE_SPIFFS */

#endif /* EXCLUDE_COMMON */

/* ISO C requires a translation unit to contain at least one declaration */
typedef int cs_dirent_dummy;
#ifdef MG_MODULE_LINES
#line 1 "./src/../../common/cs_time.c"
#endif
/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

/* Amalgamated: #include "common/cs_time.h" */

#ifndef _WIN32
#include <stddef.h>
#if !defined(CS_PLATFORM) || \
    (CS_PLATFORM != CS_P_CC3200 && CS_PLATFORM != CS_P_MSP432)
#include <sys/time.h>
#endif
#else
#include <windows.h>
#endif

double cs_time() {
  double now;
#ifndef _WIN32
  struct timeval tv;
  if (gettimeofday(&tv, NULL /* tz */) != 0) return 0;
  now = (double) tv.tv_sec + (((double) tv.tv_usec) / 1000000.0);
#else
  now = GetTickCount() / 1000.0;
#endif
  return now;
}
#ifdef MG_MODULE_LINES
#line 1 "./src/../../common/md5.c"
#endif
/*
 * This code implements the MD5 message-digest algorithm.
 * The algorithm is due to Ron Rivest.  This code was
 * written by Colin Plumb in 1993, no copyright is claimed.
 * This code is in the public domain; do with it what you wish.
 *
 * Equivalent code is available from RSA Data Security, Inc.
 * This code has been tested against that, and is equivalent,
 * except that you don't need to include two pages of legalese
 * with every copy.
 *
 * To compute the message digest of a chunk of bytes, declare an
 * MD5Context structure, pass it to MD5Init, call MD5Update as
 * needed on buffers full of bytes, and then call MD5Final, which
 * will fill a supplied 16-byte array with the digest.
 */

#if !defined(DISABLE_MD5) && !defined(EXCLUDE_COMMON)

/* Amalgamated: #include "common/md5.h" */

#ifndef CS_ENABLE_NATIVE_MD5
static void byteReverse(unsigned char *buf, unsigned longs) {
/* Forrest: MD5 expect LITTLE_ENDIAN, swap if BIG_ENDIAN */
#if BYTE_ORDER == BIG_ENDIAN
  do {
    uint32_t t = (uint32_t)((unsigned) buf[3] << 8 | buf[2]) << 16 |
                 ((unsigned) buf[1] << 8 | buf[0]);
    *(uint32_t *) buf = t;
    buf += 4;
  } while (--longs);
#else
  (void) buf;
  (void) longs;
#endif
}

#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

#define MD5STEP(f, w, x, y, z, data, s) \
  (w += f(x, y, z) + data, w = w << s | w >> (32 - s), w += x)

/*
 * Start MD5 accumulation.  Set bit count to 0 and buffer to mysterious
 * initialization constants.
 */
void MD5_Init(MD5_CTX *ctx) {
  ctx->buf[0] = 0x67452301;
  ctx->buf[1] = 0xefcdab89;
  ctx->buf[2] = 0x98badcfe;
  ctx->buf[3] = 0x10325476;

  ctx->bits[0] = 0;
  ctx->bits[1] = 0;
}

static void MD5Transform(uint32_t buf[4], uint32_t const in[16]) {
  register uint32_t a, b, c, d;

  a = buf[0];
  b = buf[1];
  c = buf[2];
  d = buf[3];

  MD5STEP(F1, a, b, c, d, in[0] + 0xd76aa478, 7);
  MD5STEP(F1, d, a, b, c, in[1] + 0xe8c7b756, 12);
  MD5STEP(F1, c, d, a, b, in[2] + 0x242070db, 17);
  MD5STEP(F1, b, c, d, a, in[3] + 0xc1bdceee, 22);
  MD5STEP(F1, a, b, c, d, in[4] + 0xf57c0faf, 7);
  MD5STEP(F1, d, a, b, c, in[5] + 0x4787c62a, 12);
  MD5STEP(F1, c, d, a, b, in[6] + 0xa8304613, 17);
  MD5STEP(F1, b, c, d, a, in[7] + 0xfd469501, 22);
  MD5STEP(F1, a, b, c, d, in[8] + 0x698098d8, 7);
  MD5STEP(F1, d, a, b, c, in[9] + 0x8b44f7af, 12);
  MD5STEP(F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
  MD5STEP(F1, b, c, d, a, in[11] + 0x895cd7be, 22);
  MD5STEP(F1, a, b, c, d, in[12] + 0x6b901122, 7);
  MD5STEP(F1, d, a, b, c, in[13] + 0xfd987193, 12);
  MD5STEP(F1, c, d, a, b, in[14] + 0xa679438e, 17);
  MD5STEP(F1, b, c, d, a, in[15] + 0x49b40821, 22);

  MD5STEP(F2, a, b, c, d, in[1] + 0xf61e2562, 5);
  MD5STEP(F2, d, a, b, c, in[6] + 0xc040b340, 9);
  MD5STEP(F2, c, d, a, b, in[11] + 0x265e5a51, 14);
  MD5STEP(F2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
  MD5STEP(F2, a, b, c, d, in[5] + 0xd62f105d, 5);
  MD5STEP(F2, d, a, b, c, in[10] + 0x02441453, 9);
  MD5STEP(F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
  MD5STEP(F2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
  MD5STEP(F2, a, b, c, d, in[9] + 0x21e1cde6, 5);
  MD5STEP(F2, d, a, b, c, in[14] + 0xc33707d6, 9);
  MD5STEP(F2, c, d, a, b, in[3] + 0xf4d50d87, 14);
  MD5STEP(F2, b, c, d, a, in[8] + 0x455a14ed, 20);
  MD5STEP(F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
  MD5STEP(F2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
  MD5STEP(F2, c, d, a, b, in[7] + 0x676f02d9, 14);
  MD5STEP(F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

  MD5STEP(F3, a, b, c, d, in[5] + 0xfffa3942, 4);
  MD5STEP(F3, d, a, b, c, in[8] + 0x8771f681, 11);
  MD5STEP(F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
  MD5STEP(F3, b, c, d, a, in[14] + 0xfde5380c, 23);
  MD5STEP(F3, a, b, c, d, in[1] + 0xa4beea44, 4);
  MD5STEP(F3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
  MD5STEP(F3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
  MD5STEP(F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
  MD5STEP(F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
  MD5STEP(F3, d, a, b, c, in[0] + 0xeaa127fa, 11);
  MD5STEP(F3, c, d, a, b, in[3] + 0xd4ef3085, 16);
  MD5STEP(F3, b, c, d, a, in[6] + 0x04881d05, 23);
  MD5STEP(F3, a, b, c, d, in[9] + 0xd9d4d039, 4);
  MD5STEP(F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
  MD5STEP(F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
  MD5STEP(F3, b, c, d, a, in[2] + 0xc4ac5665, 23);

  MD5STEP(F4, a, b, c, d, in[0] + 0xf4292244, 6);
  MD5STEP(F4, d, a, b, c, in[7] + 0x432aff97, 10);
  MD5STEP(F4, c, d, a, b, in[14] + 0xab9423a7, 15);
  MD5STEP(F4, b, c, d, a, in[5] + 0xfc93a039, 21);
  MD5STEP(F4, a, b, c, d, in[12] + 0x655b59c3, 6);
  MD5STEP(F4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
  MD5STEP(F4, c, d, a, b, in[10] + 0xffeff47d, 15);
  MD5STEP(F4, b, c, d, a, in[1] + 0x85845dd1, 21);
  MD5STEP(F4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
  MD5STEP(F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
  MD5STEP(F4, c, d, a, b, in[6] + 0xa3014314, 15);
  MD5STEP(F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
  MD5STEP(F4, a, b, c, d, in[4] + 0xf7537e82, 6);
  MD5STEP(F4, d, a, b, c, in[11] + 0xbd3af235, 10);
  MD5STEP(F4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
  MD5STEP(F4, b, c, d, a, in[9] + 0xeb86d391, 21);

  buf[0] += a;
  buf[1] += b;
  buf[2] += c;
  buf[3] += d;
}

void MD5_Update(MD5_CTX *ctx, const unsigned char *buf, size_t len) {
  uint32_t t;

  t = ctx->bits[0];
  if ((ctx->bits[0] = t + ((uint32_t) len << 3)) < t) ctx->bits[1]++;
  ctx->bits[1] += (uint32_t) len >> 29;

  t = (t >> 3) & 0x3f;

  if (t) {
    unsigned char *p = (unsigned char *) ctx->in + t;

    t = 64 - t;
    if (len < t) {
      memcpy(p, buf, len);
      return;
    }
    memcpy(p, buf, t);
    byteReverse(ctx->in, 16);
    MD5Transform(ctx->buf, (uint32_t *) ctx->in);
    buf += t;
    len -= t;
  }

  while (len >= 64) {
    memcpy(ctx->in, buf, 64);
    byteReverse(ctx->in, 16);
    MD5Transform(ctx->buf, (uint32_t *) ctx->in);
    buf += 64;
    len -= 64;
  }

  memcpy(ctx->in, buf, len);
}

void MD5_Final(unsigned char digest[16], MD5_CTX *ctx) {
  unsigned count;
  unsigned char *p;
  uint32_t *a;

  count = (ctx->bits[0] >> 3) & 0x3F;

  p = ctx->in + count;
  *p++ = 0x80;
  count = 64 - 1 - count;
  if (count < 8) {
    memset(p, 0, count);
    byteReverse(ctx->in, 16);
    MD5Transform(ctx->buf, (uint32_t *) ctx->in);
    memset(ctx->in, 0, 56);
  } else {
    memset(p, 0, count - 8);
  }
  byteReverse(ctx->in, 14);

  a = (uint32_t *) ctx->in;
  a[14] = ctx->bits[0];
  a[15] = ctx->bits[1];

  MD5Transform(ctx->buf, (uint32_t *) ctx->in);
  byteReverse((unsigned char *) ctx->buf, 4);
  memcpy(digest, ctx->buf, 16);
  memset((char *) ctx, 0, sizeof(*ctx));
}
#endif /* CS_ENABLE_NATIVE_MD5 */

/*
 * Stringify binary data. Output buffer size must be 2 * size_of_input + 1
 * because each byte of input takes 2 bytes in string representation
 * plus 1 byte for the terminating \0 character.
 */
void cs_to_hex(char *to, const unsigned char *p, size_t len) {
  static const char *hex = "0123456789abcdef";

  for (; len--; p++) {
    *to++ = hex[p[0] >> 4];
    *to++ = hex[p[0] & 0x0f];
  }
  *to = '\0';
}

char *cs_md5(char buf[33], ...) {
  unsigned char hash[16];
  const unsigned char *p;
  va_list ap;
  MD5_CTX ctx;

  MD5_Init(&ctx);

  va_start(ap, buf);
  while ((p = va_arg(ap, const unsigned char *) ) != NULL) {
    size_t len = va_arg(ap, size_t);
    MD5_Update(&ctx, p, len);
  }
  va_end(ap);

  MD5_Final(hash, &ctx);
  cs_to_hex(buf, hash, sizeof(hash));

  return buf;
}

#endif /* EXCLUDE_COMMON */
#ifdef MG_MODULE_LINES
#line 1 "./src/../../common/mbuf.c"
#endif
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#ifndef EXCLUDE_COMMON

#include <assert.h>
#include <string.h>
/* Amalgamated: #include "common/mbuf.h" */

#ifndef MBUF_REALLOC
#define MBUF_REALLOC realloc
#endif

#ifndef MBUF_FREE
#define MBUF_FREE free
#endif

void mbuf_init(struct mbuf *mbuf, size_t initial_size) {
  mbuf->len = mbuf->size = 0;
  mbuf->buf = NULL;
  mbuf_resize(mbuf, initial_size);
}

void mbuf_free(struct mbuf *mbuf) {
  if (mbuf->buf != NULL) {
    MBUF_FREE(mbuf->buf);
    mbuf_init(mbuf, 0);
  }
}

void mbuf_resize(struct mbuf *a, size_t new_size) {
  if (new_size > a->size || (new_size < a->size && new_size >= a->len)) {
    char *buf = (char *) MBUF_REALLOC(a->buf, new_size);
    /*
     * In case realloc fails, there's not much we can do, except keep things as
     * they are. Note that NULL is a valid return value from realloc when
     * size == 0, but that is covered too.
     */
    if (buf == NULL && new_size != 0) return;
    a->buf = buf;
    a->size = new_size;
  }
}

void mbuf_trim(struct mbuf *mbuf) {
  mbuf_resize(mbuf, mbuf->len);
}

size_t mbuf_insert(struct mbuf *a, size_t off, const void *buf, size_t len) {
  char *p = NULL;

  assert(a != NULL);
  assert(a->len <= a->size);
  assert(off <= a->len);

  /* check overflow */
  if (~(size_t) 0 - (size_t) a->buf < len) return 0;

  if (a->len + len <= a->size) {
    memmove(a->buf + off + len, a->buf + off, a->len - off);
    if (buf != NULL) {
      memcpy(a->buf + off, buf, len);
    }
    a->len += len;
  } else {
    size_t new_size = (a->len + len) * MBUF_SIZE_MULTIPLIER;
    if ((p = (char *) MBUF_REALLOC(a->buf, new_size)) != NULL) {
      a->buf = p;
      memmove(a->buf + off + len, a->buf + off, a->len - off);
      if (buf != NULL) memcpy(a->buf + off, buf, len);
      a->len += len;
      a->size = new_size;
    } else {
      len = 0;
    }
  }

  return len;
}

size_t mbuf_append(struct mbuf *a, const void *buf, size_t len) {
  return mbuf_insert(a, a->len, buf, len);
}

void mbuf_remove(struct mbuf *mb, size_t n) {
  if (n > 0 && n <= mb->len) {
    memmove(mb->buf, mb->buf + n, mb->len - n);
    mb->len -= n;
  }
}

#endif /* EXCLUDE_COMMON */
#ifdef MG_MODULE_LINES
#line 1 "./src/../../common/mg_str.c"
#endif
/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

/* Amalgamated: #include "common/mg_str.h" */

#include <string.h>

struct mg_str mg_mk_str(const char *s) {
  struct mg_str ret = {s, 0};
  if (s != NULL) ret.len = strlen(s);
  return ret;
}

struct mg_str mg_mk_str_n(const char *s, size_t len) {
  struct mg_str ret = {s, len};
  return ret;
}

int mg_vcmp(const struct mg_str *str1, const char *str2) {
  size_t n2 = strlen(str2), n1 = str1->len;
  int r = memcmp(str1->p, str2, (n1 < n2) ? n1 : n2);
  if (r == 0) {
    return n1 - n2;
  }
  return r;
}

int mg_vcasecmp(const struct mg_str *str1, const char *str2) {
  size_t n2 = strlen(str2), n1 = str1->len;
  int r = mg_ncasecmp(str1->p, str2, (n1 < n2) ? n1 : n2);
  if (r == 0) {
    return n1 - n2;
  }
  return r;
}
#ifdef MG_MODULE_LINES
#line 1 "./src/../../common/sha1.c"
#endif
/* Copyright(c) By Steve Reid <steve@edmweb.com> */
/* 100% Public Domain */

#if !defined(DISABLE_SHA1) && !defined(EXCLUDE_COMMON)

/* Amalgamated: #include "common/sha1.h" */

#define SHA1HANDSOFF
#if defined(__sun)
/* Amalgamated: #include "common/solarisfixes.h" */
#endif

union char64long16 {
  unsigned char c[64];
  uint32_t l[16];
};

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

static uint32_t blk0(union char64long16 *block, int i) {
/* Forrest: SHA expect BIG_ENDIAN, swap if LITTLE_ENDIAN */
#if BYTE_ORDER == LITTLE_ENDIAN
  block->l[i] =
      (rol(block->l[i], 24) & 0xFF00FF00) | (rol(block->l[i], 8) & 0x00FF00FF);
#endif
  return block->l[i];
}

/* Avoid redefine warning (ARM /usr/include/sys/ucontext.h define R0~R4) */
#undef blk
#undef R0
#undef R1
#undef R2
#undef R3
#undef R4

#define blk(i)                                                               \
  (block->l[i & 15] = rol(block->l[(i + 13) & 15] ^ block->l[(i + 8) & 15] ^ \
                              block->l[(i + 2) & 15] ^ block->l[i & 15],     \
                          1))
#define R0(v, w, x, y, z, i)                                          \
  z += ((w & (x ^ y)) ^ y) + blk0(block, i) + 0x5A827999 + rol(v, 5); \
  w = rol(w, 30);
#define R1(v, w, x, y, z, i)                                  \
  z += ((w & (x ^ y)) ^ y) + blk(i) + 0x5A827999 + rol(v, 5); \
  w = rol(w, 30);
#define R2(v, w, x, y, z, i)                          \
  z += (w ^ x ^ y) + blk(i) + 0x6ED9EBA1 + rol(v, 5); \
  w = rol(w, 30);
#define R3(v, w, x, y, z, i)                                        \
  z += (((w | x) & y) | (w & x)) + blk(i) + 0x8F1BBCDC + rol(v, 5); \
  w = rol(w, 30);
#define R4(v, w, x, y, z, i)                          \
  z += (w ^ x ^ y) + blk(i) + 0xCA62C1D6 + rol(v, 5); \
  w = rol(w, 30);

void cs_sha1_transform(uint32_t state[5], const unsigned char buffer[64]) {
  uint32_t a, b, c, d, e;
  union char64long16 block[1];

  memcpy(block, buffer, 64);
  a = state[0];
  b = state[1];
  c = state[2];
  d = state[3];
  e = state[4];
  R0(a, b, c, d, e, 0);
  R0(e, a, b, c, d, 1);
  R0(d, e, a, b, c, 2);
  R0(c, d, e, a, b, 3);
  R0(b, c, d, e, a, 4);
  R0(a, b, c, d, e, 5);
  R0(e, a, b, c, d, 6);
  R0(d, e, a, b, c, 7);
  R0(c, d, e, a, b, 8);
  R0(b, c, d, e, a, 9);
  R0(a, b, c, d, e, 10);
  R0(e, a, b, c, d, 11);
  R0(d, e, a, b, c, 12);
  R0(c, d, e, a, b, 13);
  R0(b, c, d, e, a, 14);
  R0(a, b, c, d, e, 15);
  R1(e, a, b, c, d, 16);
  R1(d, e, a, b, c, 17);
  R1(c, d, e, a, b, 18);
  R1(b, c, d, e, a, 19);
  R2(a, b, c, d, e, 20);
  R2(e, a, b, c, d, 21);
  R2(d, e, a, b, c, 22);
  R2(c, d, e, a, b, 23);
  R2(b, c, d, e, a, 24);
  R2(a, b, c, d, e, 25);
  R2(e, a, b, c, d, 26);
  R2(d, e, a, b, c, 27);
  R2(c, d, e, a, b, 28);
  R2(b, c, d, e, a, 29);
  R2(a, b, c, d, e, 30);
  R2(e, a, b, c, d, 31);
  R2(d, e, a, b, c, 32);
  R2(c, d, e, a, b, 33);
  R2(b, c, d, e, a, 34);
  R2(a, b, c, d, e, 35);
  R2(e, a, b, c, d, 36);
  R2(d, e, a, b, c, 37);
  R2(c, d, e, a, b, 38);
  R2(b, c, d, e, a, 39);
  R3(a, b, c, d, e, 40);
  R3(e, a, b, c, d, 41);
  R3(d, e, a, b, c, 42);
  R3(c, d, e, a, b, 43);
  R3(b, c, d, e, a, 44);
  R3(a, b, c, d, e, 45);
  R3(e, a, b, c, d, 46);
  R3(d, e, a, b, c, 47);
  R3(c, d, e, a, b, 48);
  R3(b, c, d, e, a, 49);
  R3(a, b, c, d, e, 50);
  R3(e, a, b, c, d, 51);
  R3(d, e, a, b, c, 52);
  R3(c, d, e, a, b, 53);
  R3(b, c, d, e, a, 54);
  R3(a, b, c, d, e, 55);
  R3(e, a, b, c, d, 56);
  R3(d, e, a, b, c, 57);
  R3(c, d, e, a, b, 58);
  R3(b, c, d, e, a, 59);
  R4(a, b, c, d, e, 60);
  R4(e, a, b, c, d, 61);
  R4(d, e, a, b, c, 62);
  R4(c, d, e, a, b, 63);
  R4(b, c, d, e, a, 64);
  R4(a, b, c, d, e, 65);
  R4(e, a, b, c, d, 66);
  R4(d, e, a, b, c, 67);
  R4(c, d, e, a, b, 68);
  R4(b, c, d, e, a, 69);
  R4(a, b, c, d, e, 70);
  R4(e, a, b, c, d, 71);
  R4(d, e, a, b, c, 72);
  R4(c, d, e, a, b, 73);
  R4(b, c, d, e, a, 74);
  R4(a, b, c, d, e, 75);
  R4(e, a, b, c, d, 76);
  R4(d, e, a, b, c, 77);
  R4(c, d, e, a, b, 78);
  R4(b, c, d, e, a, 79);
  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;
  state[4] += e;
  /* Erase working structures. The order of operations is important,
   * used to ensure that compiler doesn't optimize those out. */
  memset(block, 0, sizeof(block));
  a = b = c = d = e = 0;
  (void) a;
  (void) b;
  (void) c;
  (void) d;
  (void) e;
}

void cs_sha1_init(cs_sha1_ctx *context) {
  context->state[0] = 0x67452301;
  context->state[1] = 0xEFCDAB89;
  context->state[2] = 0x98BADCFE;
  context->state[3] = 0x10325476;
  context->state[4] = 0xC3D2E1F0;
  context->count[0] = context->count[1] = 0;
}

void cs_sha1_update(cs_sha1_ctx *context, const unsigned char *data,
                    uint32_t len) {
  uint32_t i, j;

  j = context->count[0];
  if ((context->count[0] += len << 3) < j) context->count[1]++;
  context->count[1] += (len >> 29);
  j = (j >> 3) & 63;
  if ((j + len) > 63) {
    memcpy(&context->buffer[j], data, (i = 64 - j));
    cs_sha1_transform(context->state, context->buffer);
    for (; i + 63 < len; i += 64) {
      cs_sha1_transform(context->state, &data[i]);
    }
    j = 0;
  } else
    i = 0;
  memcpy(&context->buffer[j], &data[i], len - i);
}

void cs_sha1_final(unsigned char digest[20], cs_sha1_ctx *context) {
  unsigned i;
  unsigned char finalcount[8], c;

  for (i = 0; i < 8; i++) {
    finalcount[i] = (unsigned char) ((context->count[(i >= 4 ? 0 : 1)] >>
                                      ((3 - (i & 3)) * 8)) &
                                     255);
  }
  c = 0200;
  cs_sha1_update(context, &c, 1);
  while ((context->count[0] & 504) != 448) {
    c = 0000;
    cs_sha1_update(context, &c, 1);
  }
  cs_sha1_update(context, finalcount, 8);
  for (i = 0; i < 20; i++) {
    digest[i] =
        (unsigned char) ((context->state[i >> 2] >> ((3 - (i & 3)) * 8)) & 255);
  }
  memset(context, '\0', sizeof(*context));
  memset(&finalcount, '\0', sizeof(finalcount));
}

void cs_hmac_sha1(const unsigned char *key, size_t keylen,
                  const unsigned char *data, size_t datalen,
                  unsigned char out[20]) {
  cs_sha1_ctx ctx;
  unsigned char buf1[64], buf2[64], tmp_key[20], i;

  if (keylen > sizeof(buf1)) {
    cs_sha1_init(&ctx);
    cs_sha1_update(&ctx, key, keylen);
    cs_sha1_final(tmp_key, &ctx);
    key = tmp_key;
    keylen = sizeof(tmp_key);
  }

  memset(buf1, 0, sizeof(buf1));
  memset(buf2, 0, sizeof(buf2));
  memcpy(buf1, key, keylen);
  memcpy(buf2, key, keylen);

  for (i = 0; i < sizeof(buf1); i++) {
    buf1[i] ^= 0x36;
    buf2[i] ^= 0x5c;
  }

  cs_sha1_init(&ctx);
  cs_sha1_update(&ctx, buf1, sizeof(buf1));
  cs_sha1_update(&ctx, data, datalen);
  cs_sha1_final(out, &ctx);

  cs_sha1_init(&ctx);
  cs_sha1_update(&ctx, buf2, sizeof(buf2));
  cs_sha1_update(&ctx, out, 20);
  cs_sha1_final(out, &ctx);
}

#endif /* EXCLUDE_COMMON */
#ifdef MG_MODULE_LINES
#line 1 "./src/../../common/str_util.c"
#endif
/*
 * Copyright (c) 2015 Cesanta Software Limited
 * All rights reserved
 */

#ifndef EXCLUDE_COMMON

/* Amalgamated: #include "common/platform.h" */
/* Amalgamated: #include "common/str_util.h" */

size_t c_strnlen(const char *s, size_t maxlen) {
  size_t l = 0;
  for (; l < maxlen && s[l] != '\0'; l++) {
  }
  return l;
}

#define C_SNPRINTF_APPEND_CHAR(ch)       \
  do {                                   \
    if (i < (int) buf_size) buf[i] = ch; \
    i++;                                 \
  } while (0)

#define C_SNPRINTF_FLAG_ZERO 1

#ifdef C_DISABLE_BUILTIN_SNPRINTF
int c_vsnprintf(char *buf, size_t buf_size, const char *fmt, va_list ap) {
  return vsnprintf(buf, buf_size, fmt, ap);
}
#else
static int c_itoa(char *buf, size_t buf_size, int64_t num, int base, int flags,
                  int field_width) {
  char tmp[40];
  int i = 0, k = 0, neg = 0;

  if (num < 0) {
    neg++;
    num = -num;
  }

  /* Print into temporary buffer - in reverse order */
  do {
    int rem = num % base;
    if (rem < 10) {
      tmp[k++] = '0' + rem;
    } else {
      tmp[k++] = 'a' + (rem - 10);
    }
    num /= base;
  } while (num > 0);

  /* Zero padding */
  if (flags && C_SNPRINTF_FLAG_ZERO) {
    while (k < field_width && k < (int) sizeof(tmp) - 1) {
      tmp[k++] = '0';
    }
  }

  /* And sign */
  if (neg) {
    tmp[k++] = '-';
  }

  /* Now output */
  while (--k >= 0) {
    C_SNPRINTF_APPEND_CHAR(tmp[k]);
  }

  return i;
}

int c_vsnprintf(char *buf, size_t buf_size, const char *fmt, va_list ap) {
  int ch, i = 0, len_mod, flags, precision, field_width;

  while ((ch = *fmt++) != '\0') {
    if (ch != '%') {
      C_SNPRINTF_APPEND_CHAR(ch);
    } else {
      /*
       * Conversion specification:
       *   zero or more flags (one of: # 0 - <space> + ')
       *   an optional minimum  field  width (digits)
       *   an  optional precision (. followed by digits, or *)
       *   an optional length modifier (one of: hh h l ll L q j z t)
       *   conversion specifier (one of: d i o u x X e E f F g G a A c s p n)
       */
      flags = field_width = precision = len_mod = 0;

      /* Flags. only zero-pad flag is supported. */
      if (*fmt == '0') {
        flags |= C_SNPRINTF_FLAG_ZERO;
      }

      /* Field width */
      while (*fmt >= '0' && *fmt <= '9') {
        field_width *= 10;
        field_width += *fmt++ - '0';
      }
      /* Dynamic field width */
      if (*fmt == '*') {
        field_width = va_arg(ap, int);
        fmt++;
      }

      /* Precision */
      if (*fmt == '.') {
        fmt++;
        if (*fmt == '*') {
          precision = va_arg(ap, int);
          fmt++;
        } else {
          while (*fmt >= '0' && *fmt <= '9') {
            precision *= 10;
            precision += *fmt++ - '0';
          }
        }
      }

      /* Length modifier */
      switch (*fmt) {
        case 'h':
        case 'l':
        case 'L':
        case 'I':
        case 'q':
        case 'j':
        case 'z':
        case 't':
          len_mod = *fmt++;
          if (*fmt == 'h') {
            len_mod = 'H';
            fmt++;
          }
          if (*fmt == 'l') {
            len_mod = 'q';
            fmt++;
          }
          break;
      }

      ch = *fmt++;
      if (ch == 's') {
        const char *s = va_arg(ap, const char *); /* Always fetch parameter */
        int j;
        int pad = field_width - (precision >= 0 ? c_strnlen(s, precision) : 0);
        for (j = 0; j < pad; j++) {
          C_SNPRINTF_APPEND_CHAR(' ');
        }

        /* `s` may be NULL in case of %.*s */
        if (s != NULL) {
          /* Ignore negative and 0 precisions */
          for (j = 0; (precision <= 0 || j < precision) && s[j] != '\0'; j++) {
            C_SNPRINTF_APPEND_CHAR(s[j]);
          }
        }
      } else if (ch == 'c') {
        ch = va_arg(ap, int); /* Always fetch parameter */
        C_SNPRINTF_APPEND_CHAR(ch);
      } else if (ch == 'd' && len_mod == 0) {
        i += c_itoa(buf + i, buf_size - i, va_arg(ap, int), 10, flags,
                    field_width);
      } else if (ch == 'd' && len_mod == 'l') {
        i += c_itoa(buf + i, buf_size - i, va_arg(ap, long), 10, flags,
                    field_width);
#ifdef SSIZE_MAX
      } else if (ch == 'd' && len_mod == 'z') {
        i += c_itoa(buf + i, buf_size - i, va_arg(ap, ssize_t), 10, flags,
                    field_width);
#endif
      } else if (ch == 'd' && len_mod == 'q') {
        i += c_itoa(buf + i, buf_size - i, va_arg(ap, int64_t), 10, flags,
                    field_width);
      } else if ((ch == 'x' || ch == 'u') && len_mod == 0) {
        i += c_itoa(buf + i, buf_size - i, va_arg(ap, unsigned),
                    ch == 'x' ? 16 : 10, flags, field_width);
      } else if ((ch == 'x' || ch == 'u') && len_mod == 'l') {
        i += c_itoa(buf + i, buf_size - i, va_arg(ap, unsigned long),
                    ch == 'x' ? 16 : 10, flags, field_width);
      } else if ((ch == 'x' || ch == 'u') && len_mod == 'z') {
        i += c_itoa(buf + i, buf_size - i, va_arg(ap, size_t),
                    ch == 'x' ? 16 : 10, flags, field_width);
      } else if (ch == 'p') {
        unsigned long num = (unsigned long) va_arg(ap, void *);
        C_SNPRINTF_APPEND_CHAR('0');
        C_SNPRINTF_APPEND_CHAR('x');
        i += c_itoa(buf + i, buf_size - i, num, 16, flags, 0);
      } else {
#ifndef NO_LIBC
        /*
         * TODO(lsm): abort is not nice in a library, remove it
         * Also, ESP8266 SDK doesn't have it
         */
        abort();
#endif
      }
    }
  }

  /* Zero-terminate the result */
  if (buf_size > 0) {
    buf[i < (int) buf_size ? i : (int) buf_size - 1] = '\0';
  }

  return i;
}
#endif

int c_snprintf(char *buf, size_t buf_size, const char *fmt, ...) {
  int result;
  va_list ap;
  va_start(ap, fmt);
  result = c_vsnprintf(buf, buf_size, fmt, ap);
  va_end(ap);
  return result;
}

#ifdef _WIN32
int to_wchar(const char *path, wchar_t *wbuf, size_t wbuf_len) {
  int ret;
  char buf[MAX_PATH * 2], buf2[MAX_PATH * 2], *p;

  strncpy(buf, path, sizeof(buf));
  buf[sizeof(buf) - 1] = '\0';

  /* Trim trailing slashes. Leave backslash for paths like "X:\" */
  p = buf + strlen(buf) - 1;
  while (p > buf && p[-1] != ':' && (p[0] == '\\' || p[0] == '/')) *p-- = '\0';

  memset(wbuf, 0, wbuf_len * sizeof(wchar_t));
  ret = MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, (int) wbuf_len);

  /*
   * Convert back to Unicode. If doubly-converted string does not match the
   * original, something is fishy, reject.
   */
  WideCharToMultiByte(CP_UTF8, 0, wbuf, (int) wbuf_len, buf2, sizeof(buf2),
                      NULL, NULL);
  if (strcmp(buf, buf2) != 0) {
    wbuf[0] = L'\0';
    ret = 0;
  }

  return ret;
}
#endif /* _WIN32 */

/* The simplest O(mn) algorithm. Better implementation are GPLed */
const char *c_strnstr(const char *s, const char *find, size_t slen) {
  size_t find_length = strlen(find);
  size_t i;

  for (i = 0; i < slen; i++) {
    if (i + find_length > slen) {
      return NULL;
    }

    if (strncmp(&s[i], find, find_length) == 0) {
      return &s[i];
    }
  }

  return NULL;
}

#endif /* EXCLUDE_COMMON */
#ifdef MG_MODULE_LINES
#line 1 "./src/net.c"
#endif
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 *
 * This software is dual-licensed: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. For the terms of this
 * license, see <http://www.gnu.org/licenses/>.
 *
 * You are free to use this software under the terms of the GNU General
 * Public License, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * Alternatively, you can license this software under a commercial
 * license, as set out in <https://www.cesanta.com/license>.
 */

/* Amalgamated: #include "mongoose/src/internal.h" */
/* Amalgamated: #include "mongoose/src/util.h" */
/* Amalgamated: #include "mongoose/src/dns.h" */
/* Amalgamated: #include "mongoose/src/resolv.h" */
/* Amalgamated: #include "common/cs_time.h" */

#define MG_MAX_HOST_LEN 200

#define MG_COPY_COMMON_CONNECTION_OPTIONS(dst, src) \
  memcpy(dst, src, sizeof(*dst));

/* Which flags can be pre-set by the user at connection creation time. */
#define _MG_ALLOWED_CONNECT_FLAGS_MASK                                   \
  (MG_F_USER_1 | MG_F_USER_2 | MG_F_USER_3 | MG_F_USER_4 | MG_F_USER_5 | \
   MG_F_USER_6 | MG_F_WEBSOCKET_NO_DEFRAG)
/* Which flags should be modifiable by user's callbacks. */
#define _MG_CALLBACK_MODIFIABLE_FLAGS_MASK                               \
  (MG_F_USER_1 | MG_F_USER_2 | MG_F_USER_3 | MG_F_USER_4 | MG_F_USER_5 | \
   MG_F_USER_6 | MG_F_WEBSOCKET_NO_DEFRAG | MG_F_SEND_AND_CLOSE |        \
   MG_F_CLOSE_IMMEDIATELY | MG_F_IS_WEBSOCKET | MG_F_DELETE_CHUNK)

#ifndef intptr_t
#define intptr_t long
#endif

extern void mg_ev_mgr_init(struct mg_mgr *mgr);
extern void mg_ev_mgr_free(struct mg_mgr *mgr);
extern void mg_ev_mgr_add_conn(struct mg_connection *nc);
extern void mg_ev_mgr_remove_conn(struct mg_connection *nc);

MG_INTERNAL void mg_add_conn(struct mg_mgr *mgr, struct mg_connection *c) {
  DBG(("%p %p", mgr, c));
  c->mgr = mgr;
  c->next = mgr->active_connections;
  mgr->active_connections = c;
  c->prev = NULL;
  if (c->next != NULL) c->next->prev = c;
  mg_ev_mgr_add_conn(c);
}

MG_INTERNAL void mg_remove_conn(struct mg_connection *conn) {
  if (conn->prev == NULL) conn->mgr->active_connections = conn->next;
  if (conn->prev) conn->prev->next = conn->next;
  if (conn->next) conn->next->prev = conn->prev;
  mg_ev_mgr_remove_conn(conn);
}

MG_INTERNAL void mg_call(struct mg_connection *nc,
                         mg_event_handler_t ev_handler, int ev, void *ev_data) {
  if (ev_handler == NULL) {
    /*
     * If protocol handler is specified, call it. Otherwise, call user-specified
     * event handler.
     */
    ev_handler = nc->proto_handler ? nc->proto_handler : nc->handler;
  }
  DBG(("%p %s ev=%d ev_data=%p flags=%lu rmbl=%d smbl=%d", nc,
       ev_handler == nc->handler ? "user" : "proto", ev, ev_data, nc->flags,
       (int) nc->recv_mbuf.len, (int) nc->send_mbuf.len));

#if !defined(NO_LIBC) && !defined(MG_DISABLE_HEXDUMP)
  /* LCOV_EXCL_START */
  if (nc->mgr->hexdump_file != NULL && ev != MG_EV_POLL &&
      ev != MG_EV_SEND /* handled separately */) {
    if (ev == MG_EV_RECV) {
      mg_hexdump_connection(nc, nc->mgr->hexdump_file, nc->recv_mbuf.buf,
                            *(int *) ev_data, ev);
    } else {
      mg_hexdump_connection(nc, nc->mgr->hexdump_file, NULL, 0, ev);
    }
  }
/* LCOV_EXCL_STOP */
#endif
  if (ev_handler != NULL) {
    unsigned long flags_before = nc->flags;
    size_t recv_mbuf_before = nc->recv_mbuf.len, recved;
    ev_handler(nc, ev, ev_data);
    recved = (recv_mbuf_before - nc->recv_mbuf.len);
    /* Prevent user handler from fiddling with system flags. */
    if (ev_handler == nc->handler && nc->flags != flags_before) {
      nc->flags = (flags_before & ~_MG_CALLBACK_MODIFIABLE_FLAGS_MASK) |
                  (nc->flags & _MG_CALLBACK_MODIFIABLE_FLAGS_MASK);
    }
    if (recved > 0 && !(nc->flags & MG_F_UDP)) {
      mg_if_recved(nc, recved);
    }
  }
  DBG(("%p after %s flags=%lu rmbl=%d smbl=%d", nc,
       ev_handler == nc->handler ? "user" : "proto", nc->flags,
       (int) nc->recv_mbuf.len, (int) nc->send_mbuf.len));
}

void mg_if_timer(struct mg_connection *c, double now) {
  if (c->ev_timer_time > 0 && now >= c->ev_timer_time) {
    double old_value = c->ev_timer_time;
    mg_call(c, NULL, MG_EV_TIMER, &now);
    /*
     * To prevent timer firing all the time, reset the timer after delivery.
     * However, in case user sets it to new value, do not reset.
     */
    if (c->ev_timer_time == old_value) {
      c->ev_timer_time = 0;
    }
  }
}

void mg_if_poll(struct mg_connection *nc, time_t now) {
  if (!(nc->flags & MG_F_SSL) || (nc->flags & MG_F_SSL_HANDSHAKE_DONE)) {
    mg_call(nc, NULL, MG_EV_POLL, &now);
  }
}

static void mg_destroy_conn(struct mg_connection *conn) {
  if (conn->proto_data != NULL && conn->proto_data_destructor != NULL) {
    conn->proto_data_destructor(conn->proto_data);
  }
  mg_if_destroy_conn(conn);
#if defined(MG_ENABLE_SSL) && !defined(MG_SOCKET_SIMPLELINK)
  if (conn->ssl != NULL) SSL_free(conn->ssl);
  if (conn->ssl_ctx != NULL) SSL_CTX_free(conn->ssl_ctx);
#endif
  mbuf_free(&conn->recv_mbuf);
  mbuf_free(&conn->send_mbuf);

  memset(conn, 0, sizeof(*conn));
  MG_FREE(conn);
}

void mg_close_conn(struct mg_connection *conn) {
  DBG(("%p %lu", conn, conn->flags));
  mg_call(conn, NULL, MG_EV_CLOSE, NULL);
  mg_remove_conn(conn);
  mg_destroy_conn(conn);
}

void mg_mgr_init(struct mg_mgr *m, void *user_data) {
  memset(m, 0, sizeof(*m));
#ifndef MG_DISABLE_SOCKETPAIR
  m->ctl[0] = m->ctl[1] = INVALID_SOCKET;
#endif
  m->user_data = user_data;

#ifdef _WIN32
  {
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);
  }
#elif defined(__unix__)
  /* Ignore SIGPIPE signal, so if client cancels the request, it
   * won't kill the whole process. */
  signal(SIGPIPE, SIG_IGN);
#endif

#if defined(MG_ENABLE_SSL) && !defined(MG_SOCKET_SIMPLELINK)
  {
    static int init_done;
    if (!init_done) {
      SSL_library_init();
      init_done++;
    }
  }
#endif

  mg_ev_mgr_init(m);
  DBG(("=================================="));
  DBG(("init mgr=%p", m));
}

#ifdef MG_ENABLE_JAVASCRIPT
static enum v7_err mg_send_js(struct v7 *v7, v7_val_t *res) {
  v7_val_t arg0 = v7_arg(v7, 0);
  v7_val_t arg1 = v7_arg(v7, 1);
  struct mg_connection *c = (struct mg_connection *) v7_get_ptr(v7, arg0);
  size_t len = 0;

  if (v7_is_string(arg1)) {
    const char *data = v7_get_string(v7, &arg1, &len);
    mg_send(c, data, len);
  }

  *res = v7_mk_number(v7, len);

  return V7_OK;
}

enum v7_err mg_enable_javascript(struct mg_mgr *m, struct v7 *v7,
                                 const char *init_file_name) {
  v7_val_t v;
  m->v7 = v7;
  v7_set_method(v7, v7_get_global(v7), "mg_send", mg_send_js);
  return v7_exec_file(v7, init_file_name, &v);
}
#endif

void mg_mgr_free(struct mg_mgr *m) {
  struct mg_connection *conn, *tmp_conn;

  DBG(("%p", m));
  if (m == NULL) return;
  /* Do one last poll, see https://github.com/cesanta/mongoose/issues/286 */
  mg_mgr_poll(m, 0);

#ifndef MG_DISABLE_SOCKETPAIR
  if (m->ctl[0] != INVALID_SOCKET) closesocket(m->ctl[0]);
  if (m->ctl[1] != INVALID_SOCKET) closesocket(m->ctl[1]);
  m->ctl[0] = m->ctl[1] = INVALID_SOCKET;
#endif

  for (conn = m->active_connections; conn != NULL; conn = tmp_conn) {
    tmp_conn = conn->next;
    mg_close_conn(conn);
  }

  mg_ev_mgr_free(m);
}

int mg_vprintf(struct mg_connection *nc, const char *fmt, va_list ap) {
  char mem[MG_VPRINTF_BUFFER_SIZE], *buf = mem;
  int len;

  if ((len = mg_avprintf(&buf, sizeof(mem), fmt, ap)) > 0) {
    mg_send(nc, buf, len);
  }
  if (buf != mem && buf != NULL) {
    MG_FREE(buf); /* LCOV_EXCL_LINE */
  }               /* LCOV_EXCL_LINE */

  return len;
}

int mg_printf(struct mg_connection *conn, const char *fmt, ...) {
  int len;
  va_list ap;
  va_start(ap, fmt);
  len = mg_vprintf(conn, fmt, ap);
  va_end(ap);
  return len;
}

#ifndef MG_DISABLE_SYNC_RESOLVER
/* TODO(lsm): use non-blocking resolver */
static int mg_resolve2(const char *host, struct in_addr *ina) {
#ifdef MG_ENABLE_GETADDRINFO
  int rv = 0;
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_in *h = NULL;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  if ((rv = getaddrinfo(host, NULL, NULL, &servinfo)) != 0) {
    DBG(("getaddrinfo(%s) failed: %s", host, strerror(errno)));
    return 0;
  }
  for (p = servinfo; p != NULL; p = p->ai_next) {
    memcpy(&h, &p->ai_addr, sizeof(struct sockaddr_in *));
    memcpy(ina, &h->sin_addr, sizeof(ina));
  }
  freeaddrinfo(servinfo);
  return 1;
#else
  struct hostent *he;
  if ((he = gethostbyname(host)) == NULL) {
    DBG(("gethostbyname(%s) failed: %s", host, strerror(errno)));
  } else {
    memcpy(ina, he->h_addr_list[0], sizeof(*ina));
    return 1;
  }
  return 0;
#endif /* MG_ENABLE_GETADDRINFO */
}

int mg_resolve(const char *host, char *buf, size_t n) {
  struct in_addr ad;
  return mg_resolve2(host, &ad) ? snprintf(buf, n, "%s", inet_ntoa(ad)) : 0;
}
#endif /* MG_DISABLE_SYNC_RESOLVER */

MG_INTERNAL struct mg_connection *mg_create_connection_base(
    struct mg_mgr *mgr, mg_event_handler_t callback,
    struct mg_add_sock_opts opts) {
  struct mg_connection *conn;

  if ((conn = (struct mg_connection *) MG_CALLOC(1, sizeof(*conn))) != NULL) {
    conn->sock = INVALID_SOCKET;
    conn->handler = callback;
    conn->mgr = mgr;
    conn->last_io_time = mg_time();
    conn->flags = opts.flags & _MG_ALLOWED_CONNECT_FLAGS_MASK;
    conn->user_data = opts.user_data;
    /*
     * SIZE_MAX is defined as a long long constant in
     * system headers on some platforms and so it
     * doesn't compile with pedantic ansi flags.
     */
    conn->recv_mbuf_limit = ~0;
  } else {
    MG_SET_PTRPTR(opts.error_string, "failed to create connection");
  }

  return conn;
}

MG_INTERNAL struct mg_connection *mg_create_connection(
    struct mg_mgr *mgr, mg_event_handler_t callback,
    struct mg_add_sock_opts opts) {
  struct mg_connection *conn = mg_create_connection_base(mgr, callback, opts);

  if (!mg_if_create_conn(conn)) {
    MG_FREE(conn);
    conn = NULL;
    MG_SET_PTRPTR(opts.error_string, "failed to init connection");
  }

  return conn;
}

/*
 * Address format: [PROTO://][HOST]:PORT
 *
 * HOST could be IPv4/IPv6 address or a host name.
 * `host` is a destination buffer to hold parsed HOST part. Shoud be at least
 * MG_MAX_HOST_LEN bytes long.
 * `proto` is a returned socket type, either SOCK_STREAM or SOCK_DGRAM
 *
 * Return:
 *   -1   on parse error
 *    0   if HOST needs DNS lookup
 *   >0   length of the address string
 */
MG_INTERNAL int mg_parse_address(const char *str, union socket_address *sa,
                                 int *proto, char *host, size_t host_len) {
  unsigned int a, b, c, d, port = 0;
  int ch, len = 0;
#ifdef MG_ENABLE_IPV6
  char buf[100];
#endif

  /*
   * MacOS needs that. If we do not zero it, subsequent bind() will fail.
   * Also, all-zeroes in the socket address means binding to all addresses
   * for both IPv4 and IPv6 (INADDR_ANY and IN6ADDR_ANY_INIT).
   */
  memset(sa, 0, sizeof(*sa));
  sa->sin.sin_family = AF_INET;

  *proto = SOCK_STREAM;

  if (strncmp(str, "udp://", 6) == 0) {
    str += 6;
    *proto = SOCK_DGRAM;
  } else if (strncmp(str, "tcp://", 6) == 0) {
    str += 6;
  }

  if (sscanf(str, "%u.%u.%u.%u:%u%n", &a, &b, &c, &d, &port, &len) == 5) {
    /* Bind to a specific IPv4 address, e.g. 192.168.1.5:8080 */
    sa->sin.sin_addr.s_addr =
        htonl(((uint32_t) a << 24) | ((uint32_t) b << 16) | c << 8 | d);
    sa->sin.sin_port = htons((uint16_t) port);
#ifdef MG_ENABLE_IPV6
  } else if (sscanf(str, "[%99[^]]]:%u%n", buf, &port, &len) == 2 &&
             inet_pton(AF_INET6, buf, &sa->sin6.sin6_addr)) {
    /* IPv6 address, e.g. [3ffe:2a00:100:7031::1]:8080 */
    sa->sin6.sin6_family = AF_INET6;
    sa->sin.sin_port = htons((uint16_t) port);
#endif
#ifndef MG_DISABLE_RESOLVER
  } else if (strlen(str) < host_len &&
             sscanf(str, "%[^ :]:%u%n", host, &port, &len) == 2) {
    sa->sin.sin_port = htons((uint16_t) port);
    if (mg_resolve_from_hosts_file(host, sa) != 0) {
      return 0;
    }
#endif
  } else if (sscanf(str, ":%u%n", &port, &len) == 1 ||
             sscanf(str, "%u%n", &port, &len) == 1) {
    /* If only port is specified, bind to IPv4, INADDR_ANY */
    sa->sin.sin_port = htons((uint16_t) port);
  } else {
    return -1;
  }

  ch = str[len]; /* Character that follows the address */
  return port < 0xffffUL && (ch == '\0' || ch == ',' || isspace(ch)) ? len : -1;
}

#ifdef MG_ENABLE_SSL

#ifndef MG_SOCKET_SIMPLELINK
/*
 * Certificate generation script is at
 * https://github.com/cesanta/mongoose/blob/master/scripts/generate_ssl_certificates.sh
 */

#ifndef MG_DISABLE_PFS
/*
 * Cipher suite options used for TLS negotiation.
 * https://wiki.mozilla.org/Security/Server_Side_TLS#Recommended_configurations
 */
static const char mg_s_cipher_list[] =
#if defined(MG_SSL_CRYPTO_MODERN)
    "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:"
    "ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:"
    "DHE-RSA-AES128-GCM-SHA256:DHE-DSS-AES128-GCM-SHA256:kEDH+AESGCM:"
    "ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:"
    "ECDHE-ECDSA-AES128-SHA:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:"
    "ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:DHE-RSA-AES128-SHA256:"
    "DHE-RSA-AES128-SHA:DHE-DSS-AES128-SHA256:DHE-RSA-AES256-SHA256:"
    "DHE-DSS-AES256-SHA:DHE-RSA-AES256-SHA:"
    "!aNULL:!eNULL:!EXPORT:!DES:!RC4:!3DES:!MD5:!PSK"
#elif defined(MG_SSL_CRYPTO_OLD)
    "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:"
    "ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:"
    "DHE-RSA-AES128-GCM-SHA256:DHE-DSS-AES128-GCM-SHA256:kEDH+AESGCM:"
    "ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:"
    "ECDHE-ECDSA-AES128-SHA:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:"
    "ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:DHE-RSA-AES128-SHA256:"
    "DHE-RSA-AES128-SHA:DHE-DSS-AES128-SHA256:DHE-RSA-AES256-SHA256:"
    "DHE-DSS-AES256-SHA:DHE-RSA-AES256-SHA:ECDHE-RSA-DES-CBC3-SHA:"
    "ECDHE-ECDSA-DES-CBC3-SHA:AES128-GCM-SHA256:AES256-GCM-SHA384:"
    "AES128-SHA256:AES256-SHA256:AES128-SHA:AES256-SHA:AES:DES-CBC3-SHA:"
    "HIGH:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!MD5:!PSK:!aECDH:"
    "!EDH-DSS-DES-CBC3-SHA:!EDH-RSA-DES-CBC3-SHA:!KRB5-DES-CBC3-SHA"
#else /* Default - intermediate. */
    "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:"
    "ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:"
    "DHE-RSA-AES128-GCM-SHA256:DHE-DSS-AES128-GCM-SHA256:kEDH+AESGCM:"
    "ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:"
    "ECDHE-ECDSA-AES128-SHA:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:"
    "ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:DHE-RSA-AES128-SHA256:"
    "DHE-RSA-AES128-SHA:DHE-DSS-AES128-SHA256:DHE-RSA-AES256-SHA256:"
    "DHE-DSS-AES256-SHA:DHE-RSA-AES256-SHA:AES128-GCM-SHA256:AES256-GCM-SHA384:"
    "AES128-SHA256:AES256-SHA256:AES128-SHA:AES256-SHA:AES:CAMELLIA:"
    "DES-CBC3-SHA:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!MD5:!PSK:!aECDH:"
    "!EDH-DSS-DES-CBC3-SHA:!EDH-RSA-DES-CBC3-SHA:!KRB5-DES-CBC3-SHA"
#endif
    ;

/*
 * Default DH params for PFS cipher negotiation. This is a 2048-bit group.
 * Will be used if none are provided by the user in the certificate file.
 */
static const char mg_s_default_dh_params[] =
    "\
-----BEGIN DH PARAMETERS-----\n\
MIIBCAKCAQEAlvbgD/qh9znWIlGFcV0zdltD7rq8FeShIqIhkQ0C7hYFThrBvF2E\n\
Z9bmgaP+sfQwGpVlv9mtaWjvERbu6mEG7JTkgmVUJrUt/wiRzwTaCXBqZkdUO8Tq\n\
+E6VOEQAilstG90ikN1Tfo+K6+X68XkRUIlgawBTKuvKVwBhuvlqTGerOtnXWnrt\n\
ym//hd3cd5PBYGBix0i7oR4xdghvfR2WLVu0LgdThTBb6XP7gLd19cQ1JuBtAajZ\n\
wMuPn7qlUkEFDIkAZy59/Hue/H2Q2vU/JsvVhHWCQBL4F1ofEAt50il6ZxR1QfFK\n\
9VGKDC4oOgm9DlxwwBoC2FjqmvQlqVV3kwIBAg==\n\
-----END DH PARAMETERS-----\n";
#endif

static int mg_use_ca_cert(SSL_CTX *ctx, const char *cert) {
  if (ctx == NULL) {
    return -1;
  } else if (cert == NULL || cert[0] == '\0') {
    return 0;
  }
  SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, 0);
  return SSL_CTX_load_verify_locations(ctx, cert, NULL) == 1 ? 0 : -2;
}

static int mg_use_cert(SSL_CTX *ctx, const char *cert, const char *key) {
  if (ctx == NULL) {
    return -1;
  } else if (cert == NULL || cert[0] == '\0' || key == NULL || key[0] == '\0') {
    return 0;
  } else if (SSL_CTX_use_certificate_file(ctx, cert, 1) == 0 ||
             SSL_CTX_use_PrivateKey_file(ctx, key, 1) == 0) {
    return -2;
  } else {
#ifndef MG_DISABLE_PFS
    BIO *bio = NULL;
    DH *dh = NULL;

    /* Try to read DH parameters from the cert/key file. */
    bio = BIO_new_file(cert, "r");
    if (bio != NULL) {
      dh = PEM_read_bio_DHparams(bio, NULL, NULL, NULL);
      BIO_free(bio);
    }
    /*
     * If there are no DH params in the file, fall back to hard-coded ones.
     * Not ideal, but better than nothing.
     */
    if (dh == NULL) {
      bio = BIO_new_mem_buf((void *) mg_s_default_dh_params, -1);
      dh = PEM_read_bio_DHparams(bio, NULL, NULL, NULL);
      BIO_free(bio);
    }
    if (dh != NULL) {
      SSL_CTX_set_tmp_dh(ctx, dh);
      SSL_CTX_set_options(ctx, SSL_OP_SINGLE_DH_USE);
      DH_free(dh);
    }
#endif
    SSL_CTX_set_mode(ctx, SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);
    SSL_CTX_use_certificate_chain_file(ctx, cert);
    return 0;
  }
}

/*
 * Turn the connection into SSL mode.
 * `cert` is the certificate file in PEM format. For listening connections,
 * certificate file must contain private key and server certificate,
 * concatenated. It may also contain DH params - these will be used for more
 * secure key exchange. `ca_cert` is a certificate authority (CA) PEM file, and
 * it is optional (can be set to NULL). If `ca_cert` is non-NULL, then
 * the connection is so-called two-way-SSL: other peer's certificate is
 * checked against the `ca_cert`.
 *
 * Handy OpenSSL command to generate test self-signed certificate:
 *
 *    openssl req -x509 -newkey rsa:2048 -keyout key.pem -out cert.pem -days 999
 *
 * Return NULL on success, or error message on failure.
 */
static const char *mg_set_ssl2(struct mg_connection *nc, const char *cert,
                               const char *key, const char *ca_cert) {
  const char *result = NULL;
  DBG(("%p %s,%s,%s", nc, (cert ? cert : ""), (key ? key : ""),
       (ca_cert ? ca_cert : "")));

  if (nc->flags & MG_F_UDP) {
    return "SSL for UDP is not supported";
  }

  if (key == NULL && cert != NULL) key = cert;

  if (nc->ssl != NULL) {
    SSL_free(nc->ssl);
    nc->ssl = NULL;
  }
  if (nc->ssl_ctx != NULL) {
    SSL_CTX_free(nc->ssl_ctx);
    nc->ssl_ctx = NULL;
  }

  if ((nc->flags & MG_F_LISTENING) &&
      (nc->ssl_ctx = SSL_CTX_new(SSLv23_server_method())) == NULL) {
    result = "SSL_CTX_new() failed";
  } else if (!(nc->flags & MG_F_LISTENING) &&
             (nc->ssl_ctx = SSL_CTX_new(SSLv23_client_method())) == NULL) {
    result = "SSL_CTX_new() failed";
  } else if (mg_use_cert(nc->ssl_ctx, cert, key) != 0) {
    result = "Invalid ssl cert";
  } else if (mg_use_ca_cert(nc->ssl_ctx, ca_cert) != 0) {
    result = "Invalid CA cert";
  } else if (!(nc->flags & MG_F_LISTENING) &&
             (nc->ssl = SSL_new(nc->ssl_ctx)) == NULL) {
    result = "SSL_new() failed";
  }

#ifndef MG_DISABLE_PFS
  SSL_CTX_set_cipher_list(nc->ssl_ctx, mg_s_cipher_list);
#endif

  if (result == NULL) nc->flags |= MG_F_SSL;

  return result;
}

const char *mg_set_ssl(struct mg_connection *nc, const char *cert,
                       const char *ca_cert) {
  return mg_set_ssl2(nc, cert, NULL, ca_cert);
}

#else
const char *mg_set_ssl2(struct mg_connection *nc, const char *cert,
                        const char *key, const char *ca_cert);
#endif /* MG_SOCKET_SIMPLELINK */

#endif /* MG_ENABLE_SSL */

struct mg_connection *mg_if_accept_new_conn(struct mg_connection *lc) {
  struct mg_add_sock_opts opts;
  struct mg_connection *nc;
  memset(&opts, 0, sizeof(opts));
  nc = mg_create_connection(lc->mgr, lc->handler, opts);
  if (nc == NULL) return NULL;
  nc->listener = lc;
  nc->proto_handler = lc->proto_handler;
  nc->user_data = lc->user_data;
  nc->recv_mbuf_limit = lc->recv_mbuf_limit;
  if (lc->flags & MG_F_SSL) nc->flags |= MG_F_SSL;
  mg_add_conn(nc->mgr, nc);
  DBG(("%p %p %d %d", lc, nc, nc->sock, (int) nc->flags));
  return nc;
}

void mg_if_accept_tcp_cb(struct mg_connection *nc, union socket_address *sa,
                         size_t sa_len) {
  (void) sa_len;
  nc->sa = *sa;
  mg_call(nc, NULL, MG_EV_ACCEPT, &nc->sa);
}

void mg_send(struct mg_connection *nc, const void *buf, int len) {
  nc->last_io_time = mg_time();
  if (nc->flags & MG_F_UDP) {
    mg_if_udp_send(nc, buf, len);
  } else {
    mg_if_tcp_send(nc, buf, len);
  }
#if !defined(NO_LIBC) && !defined(MG_DISABLE_HEXDUMP)
  if (nc->mgr && nc->mgr->hexdump_file != NULL) {
    mg_hexdump_connection(nc, nc->mgr->hexdump_file, buf, len, MG_EV_SEND);
  }
#endif
}

void mg_if_sent_cb(struct mg_connection *nc, int num_sent) {
  if (num_sent < 0) {
    nc->flags |= MG_F_CLOSE_IMMEDIATELY;
  }
  mg_call(nc, NULL, MG_EV_SEND, &num_sent);
}

static void mg_recv_common(struct mg_connection *nc, void *buf, int len) {
  DBG(("%p %d %u", nc, len, (unsigned int) nc->recv_mbuf.len));
  if (nc->flags & MG_F_CLOSE_IMMEDIATELY) {
    DBG(("%p discarded %d bytes", nc, len));
    /*
     * This connection will not survive next poll. Do not deliver events,
     * send data to /dev/null without acking.
     */
    MG_FREE(buf);
    return;
  }
  nc->last_io_time = mg_time();
  if (nc->recv_mbuf.len == 0) {
    /* Adopt buf as recv_mbuf's backing store. */
    mbuf_free(&nc->recv_mbuf);
    nc->recv_mbuf.buf = (char *) buf;
    nc->recv_mbuf.size = nc->recv_mbuf.len = len;
  } else {
    mbuf_append(&nc->recv_mbuf, buf, len);
    MG_FREE(buf);
  }
  mg_call(nc, NULL, MG_EV_RECV, &len);
}

void mg_if_recv_tcp_cb(struct mg_connection *nc, void *buf, int len) {
  mg_recv_common(nc, buf, len);
}

void mg_if_recv_udp_cb(struct mg_connection *nc, void *buf, int len,
                       union socket_address *sa, size_t sa_len) {
  assert(nc->flags & MG_F_UDP);
  DBG(("%p %u", nc, (unsigned int) len));
  if (nc->flags & MG_F_LISTENING) {
    struct mg_connection *lc = nc;
    /*
     * Do we have an existing connection for this source?
     * This is very inefficient for long connection lists.
     */
    for (nc = mg_next(lc->mgr, NULL); nc != NULL; nc = mg_next(lc->mgr, nc)) {
      if (memcmp(&nc->sa.sa, &sa->sa, sa_len) == 0 && nc->listener == lc) {
        break;
      }
    }
    if (nc == NULL) {
      struct mg_add_sock_opts opts;
      memset(&opts, 0, sizeof(opts));
      /* Create fake connection w/out sock initialization */
      nc = mg_create_connection_base(lc->mgr, lc->handler, opts);
      if (nc != NULL) {
        nc->sock = lc->sock;
        nc->listener = lc;
        nc->sa = *sa;
        nc->proto_handler = lc->proto_handler;
        nc->user_data = lc->user_data;
        nc->recv_mbuf_limit = lc->recv_mbuf_limit;
        nc->flags = MG_F_UDP;
        mg_add_conn(lc->mgr, nc);
        mg_call(nc, NULL, MG_EV_ACCEPT, &nc->sa);
      } else {
        DBG(("OOM"));
        /* No return here, we still need to drop on the floor */
      }
    }
  }
  if (nc != NULL) {
    mg_recv_common(nc, buf, len);
  } else {
    /* Drop on the floor. */
    MG_FREE(buf);
    mg_if_recved(nc, len);
  }
}

/*
 * Schedules an async connect for a resolved address and proto.
 * Called from two places: `mg_connect_opt()` and from async resolver.
 * When called from the async resolver, it must trigger `MG_EV_CONNECT` event
 * with a failure flag to indicate connection failure.
 */
MG_INTERNAL struct mg_connection *mg_do_connect(struct mg_connection *nc,
                                                int proto,
                                                union socket_address *sa) {
  DBG(("%p %s://%s:%hu", nc, proto == SOCK_DGRAM ? "udp" : "tcp",
       inet_ntoa(sa->sin.sin_addr), ntohs(sa->sin.sin_port)));

  nc->flags |= MG_F_CONNECTING;
  if (proto == SOCK_DGRAM) {
    mg_if_connect_udp(nc);
  } else {
    mg_if_connect_tcp(nc, sa);
  }
  mg_add_conn(nc->mgr, nc);
  return nc;
}

void mg_if_connect_cb(struct mg_connection *nc, int err) {
  DBG(("%p connect, err=%d", nc, err));
  nc->flags &= ~MG_F_CONNECTING;
  if (err != 0) {
    nc->flags |= MG_F_CLOSE_IMMEDIATELY;
  }
  mg_call(nc, NULL, MG_EV_CONNECT, &err);
}

#ifndef MG_DISABLE_RESOLVER
/*
 * Callback for the async resolver on mg_connect_opt() call.
 * Main task of this function is to trigger MG_EV_CONNECT event with
 *    either failure (and dealloc the connection)
 *    or success (and proceed with connect()
 */
static void resolve_cb(struct mg_dns_message *msg, void *data,
                       enum mg_resolve_err e) {
  struct mg_connection *nc = (struct mg_connection *) data;
  int i;
  int failure = -1;

  nc->flags &= ~MG_F_RESOLVING;
  if (msg != NULL) {
    /*
     * Take the first DNS A answer and run...
     */
    for (i = 0; i < msg->num_answers; i++) {
      if (msg->answers[i].rtype == MG_DNS_A_RECORD) {
        /*
         * Async resolver guarantees that there is at least one answer.
         * TODO(lsm): handle IPv6 answers too
         */
        mg_dns_parse_record_data(msg, &msg->answers[i], &nc->sa.sin.sin_addr,
                                 4);
        mg_do_connect(nc, nc->flags & MG_F_UDP ? SOCK_DGRAM : SOCK_STREAM,
                      &nc->sa);
        return;
      }
    }
  }

  if (e == MG_RESOLVE_TIMEOUT) {
    double now = mg_time();
    mg_call(nc, NULL, MG_EV_TIMER, &now);
  }

  /*
   * If we get there was no MG_DNS_A_RECORD in the answer
   */
  mg_call(nc, NULL, MG_EV_CONNECT, &failure);
  mg_call(nc, NULL, MG_EV_CLOSE, NULL);
  mg_destroy_conn(nc);
}
#endif

struct mg_connection *mg_connect(struct mg_mgr *mgr, const char *address,
                                 mg_event_handler_t callback) {
  struct mg_connect_opts opts;
  memset(&opts, 0, sizeof(opts));
  return mg_connect_opt(mgr, address, callback, opts);
}

struct mg_connection *mg_connect_opt(struct mg_mgr *mgr, const char *address,
                                     mg_event_handler_t callback,
                                     struct mg_connect_opts opts) {
  struct mg_connection *nc = NULL;
  int proto, rc;
  struct mg_add_sock_opts add_sock_opts;
  char host[MG_MAX_HOST_LEN];

  MG_COPY_COMMON_CONNECTION_OPTIONS(&add_sock_opts, &opts);

  if ((nc = mg_create_connection(mgr, callback, add_sock_opts)) == NULL) {
    return NULL;
  }

  if ((rc = mg_parse_address(address, &nc->sa, &proto, host, sizeof(host))) <
      0) {
    /* Address is malformed */
    MG_SET_PTRPTR(opts.error_string, "cannot parse address");
    mg_destroy_conn(nc);
    return NULL;
  }

  nc->flags |= opts.flags & _MG_ALLOWED_CONNECT_FLAGS_MASK;
  nc->flags |= (proto == SOCK_DGRAM) ? MG_F_UDP : 0;
  nc->user_data = opts.user_data;

#ifdef MG_ENABLE_SSL
  DBG(("%p %s %s %s %s", nc, address, (opts.ssl_cert ? opts.ssl_cert : ""),
       (opts.ssl_key ? opts.ssl_key : ""),
       (opts.ssl_ca_cert ? opts.ssl_ca_cert : "")));

  if (opts.ssl_cert != NULL || opts.ssl_ca_cert != NULL) {
    const char *err =
        mg_set_ssl2(nc, opts.ssl_cert, opts.ssl_key, opts.ssl_ca_cert);
    if (err != NULL) {
      MG_SET_PTRPTR(opts.error_string, err);
      mg_destroy_conn(nc);
      return NULL;
    }
    if (opts.ssl_ca_cert != NULL && (opts.ssl_server_name == NULL ||
                                     strcmp(opts.ssl_server_name, "*") != 0)) {
      if (opts.ssl_server_name == NULL) opts.ssl_server_name = host;
#ifdef SSL_KRYPTON
      SSL_CTX_kr_set_verify_name(nc->ssl_ctx, opts.ssl_server_name);
#elif defined(MG_SOCKET_SIMPLELINK)
      nc->ssl_server_name = strdup(opts.ssl_server_name);
#else
      /* TODO(rojer): Implement server name verification on OpenSSL. */
      MG_SET_PTRPTR(opts.error_string,
                    "Server name verification requested but is not supported");
      mg_destroy_conn(nc);
      return NULL;
#endif /* SSL_KRYPTON */
    }
  }
#endif /* MG_ENABLE_SSL */

  if (rc == 0) {
#ifndef MG_DISABLE_RESOLVER
    /*
     * DNS resolution is required for host.
     * mg_parse_address() fills port in nc->sa, which we pass to resolve_cb()
     */
    struct mg_connection *dns_conn = NULL;
    struct mg_resolve_async_opts o;
    memset(&o, 0, sizeof(o));
    o.dns_conn = &dns_conn;
    if (mg_resolve_async_opt(nc->mgr, host, MG_DNS_A_RECORD, resolve_cb, nc,
                             o) != 0) {
      MG_SET_PTRPTR(opts.error_string, "cannot schedule DNS lookup");
      mg_destroy_conn(nc);
      return NULL;
    }
    nc->priv_2 = dns_conn;
    nc->flags |= MG_F_RESOLVING;
    return nc;
#else
    MG_SET_PTRPTR(opts.error_string, "Resolver is disabled");
    mg_destroy_conn(nc);
    return NULL;
#endif
  } else {
    /* Address is parsed and resolved to IP. proceed with connect() */
    return mg_do_connect(nc, proto, &nc->sa);
  }
}

struct mg_connection *mg_bind(struct mg_mgr *srv, const char *address,
                              mg_event_handler_t event_handler) {
  struct mg_bind_opts opts;
  memset(&opts, 0, sizeof(opts));
  return mg_bind_opt(srv, address, event_handler, opts);
}

struct mg_connection *mg_bind_opt(struct mg_mgr *mgr, const char *address,
                                  mg_event_handler_t callback,
                                  struct mg_bind_opts opts) {
  union socket_address sa;
  struct mg_connection *nc = NULL;
  int proto, rc;
  struct mg_add_sock_opts add_sock_opts;
  char host[MG_MAX_HOST_LEN];

  MG_COPY_COMMON_CONNECTION_OPTIONS(&add_sock_opts, &opts);

  if (mg_parse_address(address, &sa, &proto, host, sizeof(host)) <= 0) {
    MG_SET_PTRPTR(opts.error_string, "cannot parse address");
    return NULL;
  }

  nc = mg_create_connection(mgr, callback, add_sock_opts);
  if (nc == NULL) {
    return NULL;
  }

  nc->sa = sa;
  nc->flags |= MG_F_LISTENING;
  if (proto == SOCK_DGRAM) nc->flags |= MG_F_UDP;

#ifdef MG_ENABLE_SSL
  DBG(("%p %s %s %s %s", nc, address, (opts.ssl_cert ? opts.ssl_cert : ""),
       (opts.ssl_key ? opts.ssl_key : ""),
       (opts.ssl_ca_cert ? opts.ssl_ca_cert : "")));

  if (opts.ssl_cert != NULL || opts.ssl_ca_cert != NULL) {
    const char *err =
        mg_set_ssl2(nc, opts.ssl_cert, opts.ssl_key, opts.ssl_ca_cert);
    if (err != NULL) {
      MG_SET_PTRPTR(opts.error_string, err);
      mg_destroy_conn(nc);
      return NULL;
    }
  }
#endif /* MG_ENABLE_SSL */

  if (nc->flags & MG_F_UDP) {
    rc = mg_if_listen_udp(nc, &nc->sa);
  } else {
    rc = mg_if_listen_tcp(nc, &nc->sa);
  }
  if (rc != 0) {
    DBG(("Failed to open listener: %d", rc));
    MG_SET_PTRPTR(opts.error_string, "failed to open listener");
    mg_destroy_conn(nc);
    return NULL;
  }
  mg_add_conn(nc->mgr, nc);

  return nc;
}

struct mg_connection *mg_next(struct mg_mgr *s, struct mg_connection *conn) {
  return conn == NULL ? s->active_connections : conn->next;
}

#ifndef MG_DISABLE_SOCKETPAIR
void mg_broadcast(struct mg_mgr *mgr, mg_event_handler_t cb, void *data,
                  size_t len) {
  struct ctl_msg ctl_msg;

  /*
   * Mongoose manager has a socketpair, `struct mg_mgr::ctl`,
   * where `mg_broadcast()` pushes the message.
   * `mg_mgr_poll()` wakes up, reads a message from the socket pair, and calls
   * specified callback for each connection. Thus the callback function executes
   * in event manager thread.
   */
  if (mgr->ctl[0] != INVALID_SOCKET && data != NULL &&
      len < sizeof(ctl_msg.message)) {
    size_t dummy;

    ctl_msg.callback = cb;
    memcpy(ctl_msg.message, data, len);
    dummy = MG_SEND_FUNC(mgr->ctl[0], (char *) &ctl_msg,
                         offsetof(struct ctl_msg, message) + len, 0);
    dummy = MG_RECV_FUNC(mgr->ctl[0], (char *) &len, 1, 0);
    (void) dummy; /* https://gcc.gnu.org/bugzilla/show_bug.cgi?id=25509 */
  }
}
#endif /* MG_DISABLE_SOCKETPAIR */

static int isbyte(int n) {
  return n >= 0 && n <= 255;
}

static int parse_net(const char *spec, uint32_t *net, uint32_t *mask) {
  int n, a, b, c, d, slash = 32, len = 0;

  if ((sscanf(spec, "%d.%d.%d.%d/%d%n", &a, &b, &c, &d, &slash, &n) == 5 ||
       sscanf(spec, "%d.%d.%d.%d%n", &a, &b, &c, &d, &n) == 4) &&
      isbyte(a) && isbyte(b) && isbyte(c) && isbyte(d) && slash >= 0 &&
      slash < 33) {
    len = n;
    *net =
        ((uint32_t) a << 24) | ((uint32_t) b << 16) | ((uint32_t) c << 8) | d;
    *mask = slash ? 0xffffffffU << (32 - slash) : 0;
  }

  return len;
}

int mg_check_ip_acl(const char *acl, uint32_t remote_ip) {
  int allowed, flag;
  uint32_t net, mask;
  struct mg_str vec;

  /* If any ACL is set, deny by default */
  allowed = (acl == NULL || *acl == '\0') ? '+' : '-';

  while ((acl = mg_next_comma_list_entry(acl, &vec, NULL)) != NULL) {
    flag = vec.p[0];
    if ((flag != '+' && flag != '-') ||
        parse_net(&vec.p[1], &net, &mask) == 0) {
      return -1;
    }

    if (net == (remote_ip & mask)) {
      allowed = flag;
    }
  }

  DBG(("%08x %c", remote_ip, allowed));
  return allowed == '+';
}

/* Move data from one connection to another */
void mg_forward(struct mg_connection *from, struct mg_connection *to) {
  mg_send(to, from->recv_mbuf.buf, from->recv_mbuf.len);
  mbuf_remove(&from->recv_mbuf, from->recv_mbuf.len);
}

double mg_set_timer(struct mg_connection *c, double timestamp) {
  double result = c->ev_timer_time;
  c->ev_timer_time = timestamp;
  /*
   * If this connection is resolving, it's not in the list of active
   * connections, so not processed yet. It has a DNS resolver connection
   * linked to it. Set up a timer for the DNS connection.
   */
  DBG(("%p %p %d -> %lu", c, c->priv_2, c->flags & MG_F_RESOLVING,
       (unsigned long) timestamp));
  if ((c->flags & MG_F_RESOLVING) && c->priv_2 != NULL) {
    ((struct mg_connection *) c->priv_2)->ev_timer_time = timestamp;
  }
  return result;
}

struct mg_connection *mg_add_sock_opt(struct mg_mgr *s, sock_t sock,
                                      mg_event_handler_t callback,
                                      struct mg_add_sock_opts opts) {
  struct mg_connection *nc = mg_create_connection_base(s, callback, opts);
  if (nc != NULL) {
    mg_sock_set(nc, sock);
    mg_add_conn(nc->mgr, nc);
  }
  return nc;
}

struct mg_connection *mg_add_sock(struct mg_mgr *s, sock_t sock,
                                  mg_event_handler_t callback) {
  struct mg_add_sock_opts opts;
  memset(&opts, 0, sizeof(opts));
  return mg_add_sock_opt(s, sock, callback, opts);
}

double mg_time() {
  return cs_time();
}
#ifdef MG_MODULE_LINES
#line 1 "./src/net_if_socket.c"
#endif
/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#if !defined(MG_DISABLE_SOCKET_IF) && !defined(MG_SOCKET_SIMPLELINK)

/* Amalgamated: #include "mongoose/src/internal.h" */
/* Amalgamated: #include "mongoose/src/util.h" */

#define MG_TCP_RECV_BUFFER_SIZE 1024
#define MG_UDP_RECV_BUFFER_SIZE 1500

static sock_t mg_open_listening_socket(union socket_address *sa, int type,
                                       int proto);
#if defined(MG_ENABLE_SSL)
static void mg_ssl_begin(struct mg_connection *nc);
static int mg_ssl_err(struct mg_connection *conn, int res);
#endif

void mg_set_non_blocking_mode(sock_t sock) {
#ifdef _WIN32
  unsigned long on = 1;
  ioctlsocket(sock, FIONBIO, &on);
#else
  int flags = fcntl(sock, F_GETFL, 0);
  fcntl(sock, F_SETFL, flags | O_NONBLOCK);
#endif
}

static int mg_is_error(int n) {
  return n == 0 || (n < 0 && errno != EINTR && errno != EINPROGRESS &&
                    errno != EAGAIN && errno != EWOULDBLOCK
#ifdef _WIN32
                    && WSAGetLastError() != WSAEINTR &&
                    WSAGetLastError() != WSAEWOULDBLOCK
#endif
                    );
}

void mg_if_connect_tcp(struct mg_connection *nc,
                       const union socket_address *sa) {
  int rc, proto = 0;
  nc->sock = socket(AF_INET, SOCK_STREAM, proto);
  if (nc->sock == INVALID_SOCKET) {
    nc->err = errno ? errno : 1;
    return;
  }
#if !defined(MG_ESP8266)
  mg_set_non_blocking_mode(nc->sock);
#endif
  rc = connect(nc->sock, &sa->sa, sizeof(sa->sin));
  nc->err = mg_is_error(rc) ? errno : 0;
  LOG(LL_INFO, ("%p sock %d err %d", nc, nc->sock, nc->err));
}

void mg_if_connect_udp(struct mg_connection *nc) {
  nc->sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (nc->sock == INVALID_SOCKET) {
    nc->err = errno ? errno : 1;
    return;
  }
  nc->err = 0;
}

int mg_if_listen_tcp(struct mg_connection *nc, union socket_address *sa) {
  int proto = 0;
  sock_t sock = mg_open_listening_socket(sa, SOCK_STREAM, proto);
  if (sock == INVALID_SOCKET) {
    return (errno ? errno : 1);
  }
  mg_sock_set(nc, sock);
  return 0;
}

int mg_if_listen_udp(struct mg_connection *nc, union socket_address *sa) {
  sock_t sock = mg_open_listening_socket(sa, SOCK_DGRAM, 0);
  if (sock == INVALID_SOCKET) return (errno ? errno : 1);
  mg_sock_set(nc, sock);
  return 0;
}

void mg_if_tcp_send(struct mg_connection *nc, const void *buf, size_t len) {
  mbuf_append(&nc->send_mbuf, buf, len);
}

void mg_if_udp_send(struct mg_connection *nc, const void *buf, size_t len) {
  mbuf_append(&nc->send_mbuf, buf, len);
}

void mg_if_recved(struct mg_connection *nc, size_t len) {
  (void) nc;
  (void) len;
}

int mg_if_create_conn(struct mg_connection *nc) {
  (void) nc;
  return 1;
}

void mg_if_destroy_conn(struct mg_connection *nc) {
  if (nc->sock == INVALID_SOCKET) return;
  if (!(nc->flags & MG_F_UDP)) {
    closesocket(nc->sock);
  } else {
    /* Only close outgoing UDP sockets or listeners. */
    if (nc->listener == NULL) closesocket(nc->sock);
  }
  nc->sock = INVALID_SOCKET;
}

static int mg_accept_conn(struct mg_connection *lc) {
  struct mg_connection *nc;
  union socket_address sa;
  socklen_t sa_len = sizeof(sa);
  /* NOTE(lsm): on Windows, sock is always > FD_SETSIZE */
  sock_t sock = accept(lc->sock, &sa.sa, &sa_len);
  if (sock == INVALID_SOCKET) {
    if (mg_is_error(-1)) DBG(("%p: failed to accept: %d", lc, errno));
    return 0;
  }
  nc = mg_if_accept_new_conn(lc);
  if (nc == NULL) {
    closesocket(sock);
    return 0;
  }
  DBG(("%p conn from %s:%d", nc, inet_ntoa(sa.sin.sin_addr),
       ntohs(sa.sin.sin_port)));
  mg_sock_set(nc, sock);
#if defined(MG_ENABLE_SSL)
  if (lc->ssl_ctx != NULL) {
    nc->ssl = SSL_new(lc->ssl_ctx);
    if (nc->ssl == NULL || SSL_set_fd(nc->ssl, sock) != 1) {
      DBG(("SSL error"));
      mg_close_conn(nc);
    }
  } else
#endif
  {
    mg_if_accept_tcp_cb(nc, &sa, sa_len);
  }
  return 1;
}

/* 'sa' must be an initialized address to bind to */
static sock_t mg_open_listening_socket(union socket_address *sa, int type,
                                       int proto) {
  socklen_t sa_len =
      (sa->sa.sa_family == AF_INET) ? sizeof(sa->sin) : sizeof(sa->sin6);
  sock_t sock = INVALID_SOCKET;
#if !defined(MG_LWIP)
  int on = 1;
#endif

  if ((sock = socket(sa->sa.sa_family, type, proto)) != INVALID_SOCKET &&
#if !defined(MG_LWIP) /* LWIP doesn't support either */
#if defined(_WIN32) && defined(SO_EXCLUSIVEADDRUSE)
      /* "Using SO_REUSEADDR and SO_EXCLUSIVEADDRUSE" http://goo.gl/RmrFTm */
      !setsockopt(sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (void *) &on,
                  sizeof(on)) &&
#endif

#if !defined(_WIN32) || !defined(SO_EXCLUSIVEADDRUSE)
      /*
       * SO_RESUSEADDR is not enabled on Windows because the semantics of
       * SO_REUSEADDR on UNIX and Windows is different. On Windows,
       * SO_REUSEADDR allows to bind a socket to a port without error even if
       * the port is already open by another program. This is not the behavior
       * SO_REUSEADDR was designed for, and leads to hard-to-track failure
       * scenarios. Therefore, SO_REUSEADDR was disabled on Windows unless
       * SO_EXCLUSIVEADDRUSE is supported and set on a socket.
       */
      !setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *) &on, sizeof(on)) &&
#endif
#endif /* !MG_LWIP */

      !bind(sock, &sa->sa, sa_len) &&
      (type == SOCK_DGRAM || listen(sock, SOMAXCONN) == 0)) {
#if !defined(MG_LWIP)
    mg_set_non_blocking_mode(sock);
    /* In case port was set to 0, get the real port number */
    (void) getsockname(sock, &sa->sa, &sa_len);
#endif
  } else if (sock != INVALID_SOCKET) {
    closesocket(sock);
    sock = INVALID_SOCKET;
  }

  return sock;
}

static void mg_write_to_socket(struct mg_connection *nc) {
  struct mbuf *io = &nc->send_mbuf;
  int n = 0;

#ifdef MG_LWIP
  /* With LWIP we don't know if the socket is ready */
  if (io->len == 0) return;
#endif

  assert(io->len > 0);

  if (nc->flags & MG_F_UDP) {
    int n =
        sendto(nc->sock, io->buf, io->len, 0, &nc->sa.sa, sizeof(nc->sa.sin));
    DBG(("%p %d %d %d %s:%hu", nc, nc->sock, n, errno,
         inet_ntoa(nc->sa.sin.sin_addr), ntohs(nc->sa.sin.sin_port)));
    if (n > 0) {
      mbuf_remove(io, n);
      mg_if_sent_cb(nc, n);
    }
    return;
  }

#if defined(MG_ENABLE_SSL)
  if (nc->ssl != NULL) {
    if (nc->flags & MG_F_SSL_HANDSHAKE_DONE) {
      n = SSL_write(nc->ssl, io->buf, io->len);
      DBG(("%p %d bytes -> %d (SSL)", nc, n, nc->sock));
      if (n <= 0) {
        int ssl_err = mg_ssl_err(nc, n);
        if (ssl_err != SSL_ERROR_WANT_READ && ssl_err != SSL_ERROR_WANT_WRITE) {
          nc->flags |= MG_F_CLOSE_IMMEDIATELY;
        }
        return;
      } else {
        /* Successful SSL operation, clear off SSL wait flags */
        nc->flags &= ~(MG_F_WANT_READ | MG_F_WANT_WRITE);
      }
    } else {
      mg_ssl_begin(nc);
      return;
    }
  } else
#endif
  {
    n = (int) MG_SEND_FUNC(nc->sock, io->buf, io->len, 0);
    DBG(("%p %d bytes -> %d", nc, n, nc->sock));
    if (n < 0 && mg_is_error(n)) {
      /* Something went wrong, drop the connection. */
      nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      return;
    }
  }

  if (n > 0) {
    mbuf_remove(io, n);
    mg_if_sent_cb(nc, n);
  }
}

MG_INTERNAL size_t recv_avail_size(struct mg_connection *conn, size_t max) {
  size_t avail;
  if (conn->recv_mbuf_limit < conn->recv_mbuf.len) return 0;
  avail = conn->recv_mbuf_limit - conn->recv_mbuf.len;
  return avail > max ? max : avail;
}

static void mg_handle_tcp_read(struct mg_connection *conn) {
  int n = 0;
  char *buf = (char *) MG_MALLOC(MG_TCP_RECV_BUFFER_SIZE);

  if (buf == NULL) {
    DBG(("OOM"));
    return;
  }

#if defined(MG_ENABLE_SSL)
  if (conn->ssl != NULL) {
    if (conn->flags & MG_F_SSL_HANDSHAKE_DONE) {
      /* SSL library may have more bytes ready to read then we ask to read.
       * Therefore, read in a loop until we read everything. Without the loop,
       * we skip to the next select() cycle which can just timeout. */
      while ((n = SSL_read(conn->ssl, buf, MG_TCP_RECV_BUFFER_SIZE)) > 0) {
        DBG(("%p %d bytes <- %d (SSL)", conn, n, conn->sock));
        mg_if_recv_tcp_cb(conn, buf, n);
        buf = NULL;
        if (conn->flags & MG_F_CLOSE_IMMEDIATELY) break;
        /* buf has been freed, we need a new one. */
        buf = (char *) MG_MALLOC(MG_TCP_RECV_BUFFER_SIZE);
        if (buf == NULL) break;
      }
      MG_FREE(buf);
      mg_ssl_err(conn, n);
    } else {
      MG_FREE(buf);
      mg_ssl_begin(conn);
      return;
    }
  } else
#endif
  {
    n = (int) MG_RECV_FUNC(conn->sock, buf,
                           recv_avail_size(conn, MG_TCP_RECV_BUFFER_SIZE), 0);
    DBG(("%p %d bytes (PLAIN) <- %d", conn, n, conn->sock));
    if (n > 0) {
      mg_if_recv_tcp_cb(conn, buf, n);
    } else {
      MG_FREE(buf);
    }
    if (n == 0) {
      /* Orderly shutdown of the socket, try flushing output. */
      conn->flags |= MG_F_SEND_AND_CLOSE;
    } else if (mg_is_error(n)) {
      conn->flags |= MG_F_CLOSE_IMMEDIATELY;
    }
  }
}

static int mg_recvfrom(struct mg_connection *nc, union socket_address *sa,
                       socklen_t *sa_len, char **buf) {
  int n;
  *buf = (char *) MG_MALLOC(MG_UDP_RECV_BUFFER_SIZE);
  if (*buf == NULL) {
    DBG(("Out of memory"));
    return -ENOMEM;
  }
  n = recvfrom(nc->sock, *buf, MG_UDP_RECV_BUFFER_SIZE, 0, &sa->sa, sa_len);
  if (n <= 0) {
    DBG(("%p recvfrom: %s", nc, strerror(errno)));
    MG_FREE(*buf);
  }
  return n;
}

static void mg_handle_udp_read(struct mg_connection *nc) {
  char *buf = NULL;
  union socket_address sa;
  socklen_t sa_len = sizeof(sa);
  int n = mg_recvfrom(nc, &sa, &sa_len, &buf);
  DBG(("%p %d bytes from %s:%d", nc, n, inet_ntoa(nc->sa.sin.sin_addr),
       ntohs(nc->sa.sin.sin_port)));
  mg_if_recv_udp_cb(nc, buf, n, &sa, sa_len);
}

#if defined(MG_ENABLE_SSL)
static int mg_ssl_err(struct mg_connection *conn, int res) {
  int ssl_err = SSL_get_error(conn->ssl, res);
  DBG(("%p %d -> %d", conn, res, ssl_err));
  if (ssl_err == SSL_ERROR_WANT_READ) {
    conn->flags |= MG_F_WANT_READ;
  } else if (ssl_err == SSL_ERROR_WANT_WRITE) {
    conn->flags |= MG_F_WANT_WRITE;
  } else {
    /* There could be an alert to deliver. Try our best. */
    SSL_write(conn->ssl, "", 0);
    conn->flags |= MG_F_CLOSE_IMMEDIATELY;
  }
  return ssl_err;
}

static void mg_ssl_begin(struct mg_connection *nc) {
  int server_side = (nc->listener != NULL);
  int res = server_side ? SSL_accept(nc->ssl) : SSL_connect(nc->ssl);
  DBG(("%p %d res %d %d", nc, server_side, res, errno));

  if (res == 1) {
    nc->flags |= MG_F_SSL_HANDSHAKE_DONE;
    nc->flags &= ~(MG_F_WANT_READ | MG_F_WANT_WRITE);

    if (server_side) {
      union socket_address sa;
      socklen_t sa_len = sizeof(sa);
      (void) getpeername(nc->sock, &sa.sa, &sa_len);
      mg_if_accept_tcp_cb(nc, &sa, sa_len);
    } else {
      mg_if_connect_cb(nc, 0);
    }
  } else {
    int ssl_err = mg_ssl_err(nc, res);
    if (ssl_err != SSL_ERROR_WANT_READ && ssl_err != SSL_ERROR_WANT_WRITE) {
      if (!server_side) {
        mg_if_connect_cb(nc, ssl_err);
      }
      nc->flags |= MG_F_CLOSE_IMMEDIATELY;
    }
  }
}
#endif /* defined(MG_ENABLE_SSL) */

#define _MG_F_FD_CAN_READ 1
#define _MG_F_FD_CAN_WRITE 1 << 1
#define _MG_F_FD_ERROR 1 << 2

void mg_mgr_handle_conn(struct mg_connection *nc, int fd_flags, double now) {
  DBG(("%p fd=%d fd_flags=%d nc_flags=%lu rmbl=%d smbl=%d", nc, nc->sock,
       fd_flags, nc->flags, (int) nc->recv_mbuf.len, (int) nc->send_mbuf.len));

  if (nc->flags & MG_F_CONNECTING) {
    if (fd_flags != 0) {
      int err = 0;
#if !defined(MG_ESP8266)
      if (!(nc->flags & MG_F_UDP)) {
        socklen_t len = sizeof(err);
        int ret =
            getsockopt(nc->sock, SOL_SOCKET, SO_ERROR, (char *) &err, &len);
        if (ret != 0) err = 1;
      }
#else
      /*
       * On ESP8266 we use blocking connect.
       */
      err = nc->err;
#endif
#if defined(MG_ENABLE_SSL)
      if (nc->ssl != NULL && err == 0) {
        SSL_set_fd(nc->ssl, nc->sock);
        mg_ssl_begin(nc);
      } else {
        mg_if_connect_cb(nc, err);
      }
#else
      mg_if_connect_cb(nc, err);
#endif
    } else if (nc->err != 0) {
      mg_if_connect_cb(nc, nc->err);
    }
  }

  if (fd_flags & _MG_F_FD_CAN_READ) {
    if (nc->flags & MG_F_UDP) {
      mg_handle_udp_read(nc);
    } else {
      if (nc->flags & MG_F_LISTENING) {
        /*
         * We're not looping here, and accepting just one connection at
         * a time. The reason is that eCos does not respect non-blocking
         * flag on a listening socket and hangs in a loop.
         */
        mg_accept_conn(nc);
      } else {
        mg_handle_tcp_read(nc);
      }
    }
  }

  if (!(nc->flags & MG_F_CLOSE_IMMEDIATELY)) {
    if ((fd_flags & _MG_F_FD_CAN_WRITE) && nc->send_mbuf.len > 0) {
      mg_write_to_socket(nc);
    }

    if (!(fd_flags & (_MG_F_FD_CAN_READ | _MG_F_FD_CAN_WRITE))) {
      mg_if_poll(nc, now);
    }
    mg_if_timer(nc, now);
  }

  DBG(("%p after fd=%d nc_flags=%lu rmbl=%d smbl=%d", nc, nc->sock, nc->flags,
       (int) nc->recv_mbuf.len, (int) nc->send_mbuf.len));
}

#ifndef MG_DISABLE_SOCKETPAIR
static void mg_mgr_handle_ctl_sock(struct mg_mgr *mgr) {
  struct ctl_msg ctl_msg;
  int len =
      (int) MG_RECV_FUNC(mgr->ctl[1], (char *) &ctl_msg, sizeof(ctl_msg), 0);
  size_t dummy = MG_SEND_FUNC(mgr->ctl[1], ctl_msg.message, 1, 0);
  DBG(("read %d from ctl socket", len));
  (void) dummy; /* https://gcc.gnu.org/bugzilla/show_bug.cgi?id=25509 */
  if (len >= (int) sizeof(ctl_msg.callback) && ctl_msg.callback != NULL) {
    struct mg_connection *nc;
    for (nc = mg_next(mgr, NULL); nc != NULL; nc = mg_next(mgr, nc)) {
      ctl_msg.callback(nc, MG_EV_POLL, ctl_msg.message);
    }
  }
}
#endif

/* Associate a socket to a connection. */
void mg_sock_set(struct mg_connection *nc, sock_t sock) {
  mg_set_non_blocking_mode(sock);
  mg_set_close_on_exec(sock);
  nc->sock = sock;
  DBG(("%p %d", nc, sock));
}

void mg_ev_mgr_init(struct mg_mgr *mgr) {
  (void) mgr;
  DBG(("%p using select()", mgr));
#ifndef MG_DISABLE_SOCKETPAIR
  do {
    mg_socketpair(mgr->ctl, SOCK_DGRAM);
  } while (mgr->ctl[0] == INVALID_SOCKET);
#endif
}

void mg_ev_mgr_free(struct mg_mgr *mgr) {
  (void) mgr;
}

void mg_ev_mgr_add_conn(struct mg_connection *nc) {
  (void) nc;
}

void mg_ev_mgr_remove_conn(struct mg_connection *nc) {
  (void) nc;
}

void mg_add_to_set(sock_t sock, fd_set *set, sock_t *max_fd) {
  if (sock != INVALID_SOCKET
#ifdef __unix__
      && sock < FD_SETSIZE
#endif
      ) {
    FD_SET(sock, set);
    if (*max_fd == INVALID_SOCKET || sock > *max_fd) {
      *max_fd = sock;
    }
  }
}

time_t mg_mgr_poll(struct mg_mgr *mgr, int timeout_ms) {
  double now = mg_time();
  double min_timer;
  struct mg_connection *nc, *tmp;
  struct timeval tv;
  fd_set read_set, write_set, err_set;
  sock_t max_fd = INVALID_SOCKET;
  int num_fds, num_ev, num_timers = 0;
#ifdef __unix__
  int try_dup = 1;
#endif

  FD_ZERO(&read_set);
  FD_ZERO(&write_set);
  FD_ZERO(&err_set);
#ifndef MG_DISABLE_SOCKETPAIR
  mg_add_to_set(mgr->ctl[1], &read_set, &max_fd);
#endif

  /*
   * Note: it is ok to have connections with sock == INVALID_SOCKET in the list,
   * e.g. timer-only "connections".
   */
  min_timer = 0;
  for (nc = mgr->active_connections, num_fds = 0; nc != NULL; nc = tmp) {
    tmp = nc->next;

    if (nc->sock != INVALID_SOCKET) {
      num_fds++;

#ifdef __unix__
      /* A hack to make sure all our file descriptos fit into FD_SETSIZE. */
      if (nc->sock >= FD_SETSIZE && try_dup) {
        int new_sock = dup(nc->sock);
        if (new_sock >= 0 && new_sock < FD_SETSIZE) {
          closesocket(nc->sock);
          DBG(("new sock %d -> %d", nc->sock, new_sock));
          nc->sock = new_sock;
        } else {
          try_dup = 0;
        }
      }
#endif

      if (!(nc->flags & MG_F_WANT_WRITE) &&
          nc->recv_mbuf.len < nc->recv_mbuf_limit &&
          (!(nc->flags & MG_F_UDP) || nc->listener == NULL)) {
        mg_add_to_set(nc->sock, &read_set, &max_fd);
      }

      if (((nc->flags & MG_F_CONNECTING) && !(nc->flags & MG_F_WANT_READ)) ||
          (nc->send_mbuf.len > 0 && !(nc->flags & MG_F_CONNECTING))) {
        mg_add_to_set(nc->sock, &write_set, &max_fd);
        mg_add_to_set(nc->sock, &err_set, &max_fd);
      }
    }

    if (nc->ev_timer_time > 0) {
      if (num_timers == 0 || nc->ev_timer_time < min_timer) {
        min_timer = nc->ev_timer_time;
      }
      num_timers++;
    }
  }

  /*
   * If there is a timer to be fired earlier than the requested timeout,
   * adjust the timeout.
   */
  if (num_timers > 0) {
    double timer_timeout_ms = (min_timer - mg_time()) * 1000 + 1 /* rounding */;
    if (timer_timeout_ms < timeout_ms) {
      timeout_ms = timer_timeout_ms;
    }
  }
  if (timeout_ms < 0) timeout_ms = 0;

  tv.tv_sec = timeout_ms / 1000;
  tv.tv_usec = (timeout_ms % 1000) * 1000;

  num_ev = select((int) max_fd + 1, &read_set, &write_set, &err_set, &tv);
  now = mg_time();
  DBG(("select @ %ld num_ev=%d of %d, timeout=%d", (long) now, num_ev, num_fds,
       timeout_ms));

#ifndef MG_DISABLE_SOCKETPAIR
  if (num_ev > 0 && mgr->ctl[1] != INVALID_SOCKET &&
      FD_ISSET(mgr->ctl[1], &read_set)) {
    mg_mgr_handle_ctl_sock(mgr);
  }
#endif

  for (nc = mgr->active_connections; nc != NULL; nc = tmp) {
    int fd_flags = 0;
    if (nc->sock != INVALID_SOCKET) {
      if (num_ev > 0) {
        fd_flags = (FD_ISSET(nc->sock, &read_set) &&
                            (!(nc->flags & MG_F_UDP) || nc->listener == NULL)
                        ? _MG_F_FD_CAN_READ
                        : 0) |
                   (FD_ISSET(nc->sock, &write_set) ? _MG_F_FD_CAN_WRITE : 0) |
                   (FD_ISSET(nc->sock, &err_set) ? _MG_F_FD_ERROR : 0);
      }
#ifdef MG_LWIP
      /* With LWIP socket emulation layer, we don't get write events */
      fd_flags |= _MG_F_FD_CAN_WRITE;
#endif
    }
    tmp = nc->next;
    mg_mgr_handle_conn(nc, fd_flags, now);
  }

  for (nc = mgr->active_connections; nc != NULL; nc = tmp) {
    tmp = nc->next;
    if ((nc->flags & MG_F_CLOSE_IMMEDIATELY) ||
        (nc->send_mbuf.len == 0 && (nc->flags & MG_F_SEND_AND_CLOSE))) {
      mg_close_conn(nc);
    }
  }

  return now;
}

#ifndef MG_DISABLE_SOCKETPAIR
int mg_socketpair(sock_t sp[2], int sock_type) {
  union socket_address sa;
  sock_t sock;
  socklen_t len = sizeof(sa.sin);
  int ret = 0;

  sock = sp[0] = sp[1] = INVALID_SOCKET;

  (void) memset(&sa, 0, sizeof(sa));
  sa.sin.sin_family = AF_INET;
  sa.sin.sin_port = htons(0);
  sa.sin.sin_addr.s_addr = htonl(0x7f000001); /* 127.0.0.1 */

  if ((sock = socket(AF_INET, sock_type, 0)) == INVALID_SOCKET) {
  } else if (bind(sock, &sa.sa, len) != 0) {
  } else if (sock_type == SOCK_STREAM && listen(sock, 1) != 0) {
  } else if (getsockname(sock, &sa.sa, &len) != 0) {
  } else if ((sp[0] = socket(AF_INET, sock_type, 0)) == INVALID_SOCKET) {
  } else if (connect(sp[0], &sa.sa, len) != 0) {
  } else if (sock_type == SOCK_DGRAM &&
             (getsockname(sp[0], &sa.sa, &len) != 0 ||
              connect(sock, &sa.sa, len) != 0)) {
  } else if ((sp[1] = (sock_type == SOCK_DGRAM ? sock
                                               : accept(sock, &sa.sa, &len))) ==
             INVALID_SOCKET) {
  } else {
    mg_set_close_on_exec(sp[0]);
    mg_set_close_on_exec(sp[1]);
    if (sock_type == SOCK_STREAM) closesocket(sock);
    ret = 1;
  }

  if (!ret) {
    if (sp[0] != INVALID_SOCKET) closesocket(sp[0]);
    if (sp[1] != INVALID_SOCKET) closesocket(sp[1]);
    if (sock != INVALID_SOCKET) closesocket(sock);
    sock = sp[0] = sp[1] = INVALID_SOCKET;
  }

  return ret;
}
#endif /* MG_DISABLE_SOCKETPAIR */

static void mg_sock_get_addr(sock_t sock, int remote,
                             union socket_address *sa) {
  socklen_t slen = sizeof(*sa);
  memset(sa, 0, slen);
  if (remote) {
    getpeername(sock, &sa->sa, &slen);
  } else {
    getsockname(sock, &sa->sa, &slen);
  }
}

void mg_sock_to_str(sock_t sock, char *buf, size_t len, int flags) {
  union socket_address sa;
  mg_sock_get_addr(sock, flags & MG_SOCK_STRINGIFY_REMOTE, &sa);
  mg_sock_addr_to_str(&sa, buf, len, flags);
}

void mg_if_get_conn_addr(struct mg_connection *nc, int remote,
                         union socket_address *sa) {
  mg_sock_get_addr(nc->sock, remote, sa);
}

#endif /* !defined(MG_DISABLE_SOCKET_IF) && !defined(MG_SOCKET_SIMPLELINK) */
#ifdef MG_MODULE_LINES
#line 1 "./src/multithreading.c"
#endif
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

/* Amalgamated: #include "mongoose/src/internal.h" */
/* Amalgamated: #include "mongoose/src/util.h" */

#ifdef MG_ENABLE_THREADS

static void multithreaded_ev_handler(struct mg_connection *c, int ev, void *p);

/*
 * This thread function executes user event handler.
 * It runs an event manager that has only one connection, until that
 * connection is alive.
 */
static void *per_connection_thread_function(void *param) {
  struct mg_connection *c = (struct mg_connection *) param;
  struct mg_mgr m;

  mg_mgr_init(&m, NULL);
  mg_add_conn(&m, c);
  while (m.active_connections != NULL) {
    mg_mgr_poll(&m, 1000);
  }
  mg_mgr_free(&m);

  return param;
}

static void link_conns(struct mg_connection *c1, struct mg_connection *c2) {
  c1->priv_2 = c2;
  c2->priv_2 = c1;
}

static void unlink_conns(struct mg_connection *c) {
  struct mg_connection *peer = (struct mg_connection *) c->priv_2;
  if (peer != NULL) {
    peer->flags |= MG_F_SEND_AND_CLOSE;
    peer->priv_2 = NULL;
  }
  c->priv_2 = NULL;
}

static void forwarder_ev_handler(struct mg_connection *c, int ev, void *p) {
  (void) p;
  if (ev == MG_EV_RECV && c->priv_2) {
    mg_forward(c, (struct mg_connection *) c->priv_2);
  } else if (ev == MG_EV_CLOSE) {
    unlink_conns(c);
  }
}

static void spawn_handling_thread(struct mg_connection *nc) {
  struct mg_mgr dummy;
  sock_t sp[2];
  struct mg_connection *c[2];

  /*
   * Create a socket pair, and wrap each socket into the connection with
   * dummy event manager.
   * c[0] stays in this thread, c[1] goes to another thread.
   */
  mg_socketpair(sp, SOCK_STREAM);
  memset(&dummy, 0, sizeof(dummy));
  c[0] = mg_add_sock(&dummy, sp[0], forwarder_ev_handler);
  c[1] = mg_add_sock(&dummy, sp[1], nc->listener->priv_1.f);

  /* Interlink client connection with c[0] */
  link_conns(c[0], nc);

  /*
   * Switch c[0] manager from the dummy one to the real one. c[1] manager
   * will be set in another thread, allocated on stack of that thread.
   */
  mg_add_conn(nc->mgr, c[0]);

  /*
   * Dress c[1] as nc.
   * TODO(lsm): code in accept_conn() looks similar. Refactor.
   */
  c[1]->listener = nc->listener;
  c[1]->proto_handler = nc->proto_handler;
  c[1]->proto_data = nc->proto_data;
  c[1]->user_data = nc->user_data;

  mg_start_thread(per_connection_thread_function, c[1]);
}

static void multithreaded_ev_handler(struct mg_connection *c, int ev, void *p) {
  (void) p;
  if (ev == MG_EV_ACCEPT) {
    spawn_handling_thread(c);
    c->handler = forwarder_ev_handler;
  }
}

void mg_enable_multithreading(struct mg_connection *nc) {
  /* Wrap user event handler into our multithreaded_ev_handler */
  nc->priv_1.f = nc->handler;
  nc->handler = multithreaded_ev_handler;
}
#endif
#ifdef MG_MODULE_LINES
#line 1 "./src/uri.c"
#endif
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

/* Amalgamated: #include "mongoose/src/internal.h" */
/* Amalgamated: #include "mongoose/src/uri.h" */

/*
 * scan string until `sep`, keeping track of component boundaries in `res`.
 *
 * `p` will point to the char after the separator or it will be `end`.
 */
static void parse_uri_component(const char **p, const char *end, char sep,
                                struct mg_str *res) {
  res->p = *p;
  for (; *p < end; (*p)++) {
    if (**p == sep) {
      break;
    }
  }
  res->len = (*p) - res->p;
  if (*p < end) (*p)++;
}

int mg_parse_uri(struct mg_str uri, struct mg_str *scheme,
                 struct mg_str *user_info, struct mg_str *host,
                 unsigned int *port, struct mg_str *path, struct mg_str *query,
                 struct mg_str *fragment) {
  struct mg_str rscheme = {0, 0}, ruser_info = {0, 0}, rhost = {0, 0},
                rpath = {0, 0}, rquery = {0, 0}, rfragment = {0, 0};
  unsigned int rport = 0;
  enum {
    P_START,
    P_SCHEME_OR_PORT,
    P_USER_INFO,
    P_HOST,
    P_PORT,
    P_REST
  } state = P_START;

  const char *p = uri.p, *end = p + uri.len;
  while (p < end) {
    switch (state) {
      case P_START:
        /*
         * expecting on of:
         * - `scheme://xxxx`
         * - `xxxx:port`
         * - `xxxx/path`
         */
        for (; p < end; p++) {
          if (*p == ':') {
            state = P_SCHEME_OR_PORT;
            break;
          } else if (*p == '/') {
            state = P_REST;
            break;
          }
        }
        if (state == P_START || state == P_REST) {
          rhost.p = uri.p;
          rhost.len = p - uri.p;
        }
        break;
      case P_SCHEME_OR_PORT:
        if (end - p >= 3 && memcmp(p, "://", 3) == 0) {
          rscheme.p = uri.p;
          rscheme.len = p - uri.p;
          state = P_USER_INFO;
          p += 2; /* point to last separator char */
        } else {
          rhost.p = uri.p;
          rhost.len = p - uri.p;
          state = P_PORT;
        }
        break;
      case P_USER_INFO:
        p++;
        ruser_info.p = p;
        for (; p < end; p++) {
          if (*p == '@') {
            state = P_HOST;
            break;
          } else if (*p == '/') {
            break;
          }
        }
        if (p == end || *p == '/') {
          /* backtrack and parse as host */
          state = P_HOST;
          p = ruser_info.p;
        }
        ruser_info.len = p - ruser_info.p;
        break;
      case P_HOST:
        if (*p == '@') p++;
        rhost.p = p;
        for (; p < end; p++) {
          if (*p == ':') {
            state = P_PORT;
            break;
          } else if (*p == '/') {
            state = P_REST;
            break;
          }
        }
        rhost.len = p - rhost.p;
        break;
      case P_PORT:
        p++;
        for (; p < end; p++) {
          if (*p == '/') {
            state = P_REST;
            break;
          }
          rport *= 10;
          rport += *p - '0';
        }
        break;
      case P_REST:
        /* `p` points to separator. `path` includes the separator */
        parse_uri_component(&p, end, '?', &rpath);
        parse_uri_component(&p, end, '#', &rquery);
        parse_uri_component(&p, end, '\0', &rfragment);
        break;
    }
  }

  if (scheme != 0) *scheme = rscheme;
  if (user_info != 0) *user_info = ruser_info;
  if (host != 0) *host = rhost;
  if (port != 0) *port = rport;
  if (path != 0) *path = rpath;
  if (query != 0) *query = rquery;
  if (fragment != 0) *fragment = rfragment;

  return 0;
}

/* Normalize the URI path. Remove/resolve "." and "..". */
int mg_normalize_uri_path(const struct mg_str *in, struct mg_str *out) {
  const char *s = in->p, *se = s + in->len;
  char *cp = (char *) out->p, *d;

  if (in->len == 0 || *s != '/') {
    out->len = 0;
    return 0;
  }

  d = cp;

  while (s < se) {
    const char *next = s;
    struct mg_str component;
    parse_uri_component(&next, se, '/', &component);
    if (mg_vcmp(&component, ".") == 0) {
      /* Yum. */
    } else if (mg_vcmp(&component, "..") == 0) {
      /* Backtrack to previous slash. */
      if (d > cp + 1 && *(d - 1) == '/') d--;
      while (d > cp && *(d - 1) != '/') d--;
    } else {
      memmove(d, s, next - s);
      d += next - s;
    }
    s = next;
  }
  if (d == cp) *d++ = '/';

  out->p = cp;
  out->len = d - cp;
  return 1;
}
#ifdef MG_MODULE_LINES
#line 1 "./src/http.c"
#endif
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#ifndef MG_DISABLE_HTTP

/* Amalgamated: #include "mongoose/src/internal.h" */
/* Amalgamated: #include "mongoose/src/util.h" */
/* Amalgamated: #include "common/sha1.h" */
/* Amalgamated: #include "common/md5.h" */

#ifndef MG_DISABLE_HTTP_WEBSOCKET
#define MG_WS_NO_HOST_HEADER_MAGIC ((char *) 0x1)
#endif

/* CGI requires socketpair. */
#if defined(MG_DISABLE_SOCKETPAIR) && !defined(MG_DISABLE_CGI)
#define MG_DISABLE_CGI 1
#endif

static const char *mg_version_header = "Mongoose/" MG_VERSION;

enum mg_http_proto_data_type { DATA_NONE, DATA_FILE, DATA_PUT };

struct mg_http_proto_data_file {
  FILE *fp;      /* Opened file. */
  int64_t cl;    /* Content-Length. How many bytes to send. */
  int64_t sent;  /* How many bytes have been already sent. */
  int keepalive; /* Keep connection open after sending. */
  enum mg_http_proto_data_type type;
};

struct mg_http_proto_data_cgi {
  struct mg_connection *cgi_nc;
};

struct mg_http_proto_data_chuncked {
  int64_t body_len; /* How many bytes of chunked body was reassembled. */
};

struct mg_http_endpoint {
  struct mg_http_endpoint *next;
  const char *name;
  size_t name_len;
  mg_event_handler_t handler;
};

enum mg_http_multipart_stream_state {
  MPS_BEGIN,
  MPS_WAITING_FOR_BOUNDARY,
  MPS_WAITING_FOR_CHUNK,
  MPS_GOT_CHUNK,
  MPS_GOT_BOUNDARY,
  MPS_FINALIZE,
  MPS_FINISHED
};

struct mg_http_multipart_stream {
  const char *boundary;
  int boundary_len;
  const char *var_name;
  const char *file_name;
  void *user_data;
  int prev_io_len;
  enum mg_http_multipart_stream_state state;
  int processing_part;
};

struct mg_http_proto_data {
#ifndef MG_DISABLE_FILESYSTEM
  struct mg_http_proto_data_file file;
#endif
#ifndef MG_DISABLE_CGI
  struct mg_http_proto_data_cgi cgi;
#endif
#ifdef MG_ENABLE_HTTP_STREAMING_MULTIPART
  struct mg_http_multipart_stream mp_stream;
#endif
  struct mg_http_proto_data_chuncked chunk;
  struct mg_http_endpoint *endpoints;
  mg_event_handler_t endpoint_handler;
};

static void mg_http_conn_destructor(void *proto_data);

static struct mg_http_proto_data *mg_http_get_proto_data(
    struct mg_connection *c) {
  if (c->proto_data == NULL) {
    c->proto_data = MG_CALLOC(1, sizeof(struct mg_http_proto_data));
    c->proto_data_destructor = mg_http_conn_destructor;
  }

  return (struct mg_http_proto_data *) c->proto_data;
}

#ifdef MG_ENABLE_HTTP_STREAMING_MULTIPART
static void mg_http_free_proto_data_mp_stream(
    struct mg_http_multipart_stream *mp) {
  free((void *) mp->boundary);
  free((void *) mp->var_name);
  free((void *) mp->file_name);
  memset(mp, 0, sizeof(*mp));
}
#endif

#ifndef MG_DISABLE_FILESYSTEM
static void mg_http_free_proto_data_file(struct mg_http_proto_data_file *d) {
  if (d != NULL) {
    if (d->fp != NULL) {
      fclose(d->fp);
    }
    memset(d, 0, sizeof(struct mg_http_proto_data_file));
  }
}
#endif

#ifndef MG_DISABLE_CGI
static void mg_http_free_proto_data_cgi(struct mg_http_proto_data_cgi *d) {
  if (d != NULL) {
    if (d->cgi_nc != NULL) d->cgi_nc->flags |= MG_F_CLOSE_IMMEDIATELY;
    memset(d, 0, sizeof(struct mg_http_proto_data_cgi));
  }
}
#endif

static void mg_http_free_proto_data_endpoints(struct mg_http_endpoint **ep) {
  struct mg_http_endpoint *current = *ep;

  while (current != NULL) {
    struct mg_http_endpoint *tmp = current->next;
    free((void *) current->name);
    free(current);
    current = tmp;
  }

  ep = NULL;
}

static void mg_http_conn_destructor(void *proto_data) {
  struct mg_http_proto_data *pd = (struct mg_http_proto_data *) proto_data;
#ifndef MG_DISABLE_FILESYSTEM
  mg_http_free_proto_data_file(&pd->file);
#endif
#ifndef MG_DISABLE_CGI
  mg_http_free_proto_data_cgi(&pd->cgi);
#endif
#ifdef MG_ENABLE_HTTP_STREAMING_MULTIPART
  mg_http_free_proto_data_mp_stream(&pd->mp_stream);
#endif
  mg_http_free_proto_data_endpoints(&pd->endpoints);
  free(proto_data);
}

/*
 * This structure helps to create an environment for the spawned CGI program.
 * Environment is an array of "VARIABLE=VALUE\0" ASCIIZ strings,
 * last element must be NULL.
 * However, on Windows there is a requirement that all these VARIABLE=VALUE\0
 * strings must reside in a contiguous buffer. The end of the buffer is
 * marked by two '\0' characters.
 * We satisfy both worlds: we create an envp array (which is vars), all
 * entries are actually pointers inside buf.
 */
struct mg_cgi_env_block {
  struct mg_connection *nc;
  char buf[MG_CGI_ENVIRONMENT_SIZE];       /* Environment buffer */
  const char *vars[MG_MAX_CGI_ENVIR_VARS]; /* char *envp[] */
  int len;                                 /* Space taken */
  int nvars;                               /* Number of variables in envp[] */
};

#ifndef MG_DISABLE_FILESYSTEM

#define MIME_ENTRY(_ext, _type) \
  { _ext, sizeof(_ext) - 1, _type }
static const struct {
  const char *extension;
  size_t ext_len;
  const char *mime_type;
} mg_static_builtin_mime_types[] = {
    MIME_ENTRY("html", "text/html"),
    MIME_ENTRY("html", "text/html"),
    MIME_ENTRY("htm", "text/html"),
    MIME_ENTRY("shtm", "text/html"),
    MIME_ENTRY("shtml", "text/html"),
    MIME_ENTRY("css", "text/css"),
    MIME_ENTRY("js", "application/x-javascript"),
    MIME_ENTRY("ico", "image/x-icon"),
    MIME_ENTRY("gif", "image/gif"),
    MIME_ENTRY("jpg", "image/jpeg"),
    MIME_ENTRY("jpeg", "image/jpeg"),
    MIME_ENTRY("png", "image/png"),
    MIME_ENTRY("svg", "image/svg+xml"),
    MIME_ENTRY("txt", "text/plain"),
    MIME_ENTRY("torrent", "application/x-bittorrent"),
    MIME_ENTRY("wav", "audio/x-wav"),
    MIME_ENTRY("mp3", "audio/x-mp3"),
    MIME_ENTRY("mid", "audio/mid"),
    MIME_ENTRY("m3u", "audio/x-mpegurl"),
    MIME_ENTRY("ogg", "application/ogg"),
    MIME_ENTRY("ram", "audio/x-pn-realaudio"),
    MIME_ENTRY("xml", "text/xml"),
    MIME_ENTRY("ttf", "application/x-font-ttf"),
    MIME_ENTRY("json", "application/json"),
    MIME_ENTRY("xslt", "application/xml"),
    MIME_ENTRY("xsl", "application/xml"),
    MIME_ENTRY("ra", "audio/x-pn-realaudio"),
    MIME_ENTRY("doc", "application/msword"),
    MIME_ENTRY("exe", "application/octet-stream"),
    MIME_ENTRY("zip", "application/x-zip-compressed"),
    MIME_ENTRY("xls", "application/excel"),
    MIME_ENTRY("tgz", "application/x-tar-gz"),
    MIME_ENTRY("tar", "application/x-tar"),
    MIME_ENTRY("gz", "application/x-gunzip"),
    MIME_ENTRY("arj", "application/x-arj-compressed"),
    MIME_ENTRY("rar", "application/x-rar-compressed"),
    MIME_ENTRY("rtf", "application/rtf"),
    MIME_ENTRY("pdf", "application/pdf"),
    MIME_ENTRY("swf", "application/x-shockwave-flash"),
    MIME_ENTRY("mpg", "video/mpeg"),
    MIME_ENTRY("webm", "video/webm"),
    MIME_ENTRY("mpeg", "video/mpeg"),
    MIME_ENTRY("mov", "video/quicktime"),
    MIME_ENTRY("mp4", "video/mp4"),
    MIME_ENTRY("m4v", "video/x-m4v"),
    MIME_ENTRY("asf", "video/x-ms-asf"),
    MIME_ENTRY("avi", "video/x-msvideo"),
    MIME_ENTRY("bmp", "image/bmp"),
    {NULL, 0, NULL}};

#ifndef MG_DISABLE_DAV
static int mg_mkdir(const char *path, uint32_t mode) {
#ifndef _WIN32
  return mkdir(path, mode);
#else
  (void) mode;
  return _mkdir(path);
#endif
}
#endif

static struct mg_str mg_get_mime_type(const char *path, const char *dflt,
                                      const struct mg_serve_http_opts *opts) {
  const char *ext, *overrides;
  size_t i, path_len;
  struct mg_str r, k, v;

  path_len = strlen(path);

  overrides = opts->custom_mime_types;
  while ((overrides = mg_next_comma_list_entry(overrides, &k, &v)) != NULL) {
    ext = path + (path_len - k.len);
    if (path_len > k.len && mg_vcasecmp(&k, ext) == 0) {
      return v;
    }
  }

  for (i = 0; mg_static_builtin_mime_types[i].extension != NULL; i++) {
    ext = path + (path_len - mg_static_builtin_mime_types[i].ext_len);
    if (path_len > mg_static_builtin_mime_types[i].ext_len && ext[-1] == '.' &&
        mg_casecmp(ext, mg_static_builtin_mime_types[i].extension) == 0) {
      r.p = mg_static_builtin_mime_types[i].mime_type;
      r.len = strlen(r.p);
      return r;
    }
  }

  r.p = dflt;
  r.len = strlen(r.p);
  return r;
}
#endif

/*
 * Check whether full request is buffered. Return:
 *   -1  if request is malformed
 *    0  if request is not yet fully buffered
 *   >0  actual request length, including last \r\n\r\n
 */
static int mg_http_get_request_len(const char *s, int buf_len) {
  const unsigned char *buf = (unsigned char *) s;
  int i;

  for (i = 0; i < buf_len; i++) {
    if (!isprint(buf[i]) && buf[i] != '\r' && buf[i] != '\n' && buf[i] < 128) {
      return -1;
    } else if (buf[i] == '\n' && i + 1 < buf_len && buf[i + 1] == '\n') {
      return i + 2;
    } else if (buf[i] == '\n' && i + 2 < buf_len && buf[i + 1] == '\r' &&
               buf[i + 2] == '\n') {
      return i + 3;
    }
  }

  return 0;
}

static const char *mg_http_parse_headers(const char *s, const char *end,
                                         int len, struct http_message *req) {
  int i;
  for (i = 0; i < (int) ARRAY_SIZE(req->header_names) - 1; i++) {
    struct mg_str *k = &req->header_names[i], *v = &req->header_values[i];

    s = mg_skip(s, end, ": ", k);
    s = mg_skip(s, end, "\r\n", v);

    while (v->len > 0 && v->p[v->len - 1] == ' ') {
      v->len--; /* Trim trailing spaces in header value */
    }

    if (k->len == 0 || v->len == 0) {
      k->p = v->p = NULL;
      k->len = v->len = 0;
      break;
    }

    if (!mg_ncasecmp(k->p, "Content-Length", 14)) {
      req->body.len = to64(v->p);
      req->message.len = len + req->body.len;
    }
  }

  return s;
}

int mg_parse_http(const char *s, int n, struct http_message *hm, int is_req) {
  const char *end, *qs;
  int len = mg_http_get_request_len(s, n);

  if (len <= 0) return len;

  memset(hm, 0, sizeof(*hm));
  hm->message.p = s;
  hm->body.p = s + len;
  hm->message.len = hm->body.len = (size_t) ~0;
  end = s + len;

  /* Request is fully buffered. Skip leading whitespaces. */
  while (s < end && isspace(*(unsigned char *) s)) s++;

  if (is_req) {
    /* Parse request line: method, URI, proto */
    s = mg_skip(s, end, " ", &hm->method);
    s = mg_skip(s, end, " ", &hm->uri);
    s = mg_skip(s, end, "\r\n", &hm->proto);
    if (hm->uri.p <= hm->method.p || hm->proto.p <= hm->uri.p) return -1;

    /* If URI contains '?' character, initialize query_string */
    if ((qs = (char *) memchr(hm->uri.p, '?', hm->uri.len)) != NULL) {
      hm->query_string.p = qs + 1;
      hm->query_string.len = &hm->uri.p[hm->uri.len] - (qs + 1);
      hm->uri.len = qs - hm->uri.p;
    }
  } else {
    s = mg_skip(s, end, " ", &hm->proto);
    if (end - s < 4 || s[3] != ' ') return -1;
    hm->resp_code = atoi(s);
    if (hm->resp_code < 100 || hm->resp_code >= 600) return -1;
    s += 4;
    s = mg_skip(s, end, "\r\n", &hm->resp_status_msg);
  }

  s = mg_http_parse_headers(s, end, len, hm);

  /*
   * mg_parse_http() is used to parse both HTTP requests and HTTP
   * responses. If HTTP response does not have Content-Length set, then
   * body is read until socket is closed, i.e. body.len is infinite (~0).
   *
   * For HTTP requests though, according to
   * http://tools.ietf.org/html/rfc7231#section-8.1.3,
   * only POST and PUT methods have defined body semantics.
   * Therefore, if Content-Length is not specified and methods are
   * not one of PUT or POST, set body length to 0.
   *
   * So,
   * if it is HTTP request, and Content-Length is not set,
   * and method is not (PUT or POST) then reset body length to zero.
   */
  if (hm->body.len == (size_t) ~0 && is_req &&
      mg_vcasecmp(&hm->method, "PUT") != 0 &&
      mg_vcasecmp(&hm->method, "POST") != 0) {
    hm->body.len = 0;
    hm->message.len = len;
  }

  return len;
}

struct mg_str *mg_get_http_header(struct http_message *hm, const char *name) {
  size_t i, len = strlen(name);

  for (i = 0; hm->header_names[i].len > 0; i++) {
    struct mg_str *h = &hm->header_names[i], *v = &hm->header_values[i];
    if (h->p != NULL && h->len == len && !mg_ncasecmp(h->p, name, len))
      return v;
  }

  return NULL;
}

#ifndef MG_DISABLE_HTTP_WEBSOCKET

static int mg_is_ws_fragment(unsigned char flags) {
  return (flags & 0x80) == 0 || (flags & 0x0f) == 0;
}

static int mg_is_ws_first_fragment(unsigned char flags) {
  return (flags & 0x80) == 0 && (flags & 0x0f) != 0;
}

static void mg_handle_incoming_websocket_frame(struct mg_connection *nc,
                                               struct websocket_message *wsm) {
  if (wsm->flags & 0x8) {
    mg_call(nc, nc->handler, MG_EV_WEBSOCKET_CONTROL_FRAME, wsm);
  } else {
    mg_call(nc, nc->handler, MG_EV_WEBSOCKET_FRAME, wsm);
  }
}

static int mg_deliver_websocket_data(struct mg_connection *nc) {
  /* Using unsigned char *, cause of integer arithmetic below */
  uint64_t i, data_len = 0, frame_len = 0, buf_len = nc->recv_mbuf.len, len,
              mask_len = 0, header_len = 0;
  unsigned char *p = (unsigned char *) nc->recv_mbuf.buf, *buf = p,
                *e = p + buf_len;
  unsigned *sizep = (unsigned *) &p[1]; /* Size ptr for defragmented frames */
  int ok, reass = buf_len > 0 && mg_is_ws_fragment(p[0]) &&
                  !(nc->flags & MG_F_WEBSOCKET_NO_DEFRAG);

  /* If that's a continuation frame that must be reassembled, handle it */
  if (reass && !mg_is_ws_first_fragment(p[0]) &&
      buf_len >= 1 + sizeof(*sizep) && buf_len >= 1 + sizeof(*sizep) + *sizep) {
    buf += 1 + sizeof(*sizep) + *sizep;
    buf_len -= 1 + sizeof(*sizep) + *sizep;
  }

  if (buf_len >= 2) {
    len = buf[1] & 127;
    mask_len = buf[1] & 128 ? 4 : 0;
    if (len < 126 && buf_len >= mask_len) {
      data_len = len;
      header_len = 2 + mask_len;
    } else if (len == 126 && buf_len >= 4 + mask_len) {
      header_len = 4 + mask_len;
      data_len = ntohs(*(uint16_t *) &buf[2]);
    } else if (buf_len >= 10 + mask_len) {
      header_len = 10 + mask_len;
      data_len = (((uint64_t) ntohl(*(uint32_t *) &buf[2])) << 32) +
                 ntohl(*(uint32_t *) &buf[6]);
    }
  }

  frame_len = header_len + data_len;
  ok = frame_len > 0 && frame_len <= buf_len;

  if (ok) {
    struct websocket_message wsm;

    wsm.size = (size_t) data_len;
    wsm.data = buf + header_len;
    wsm.flags = buf[0];

    /* Apply mask if necessary */
    if (mask_len > 0) {
      for (i = 0; i < data_len; i++) {
        buf[i + header_len] ^= (buf + header_len - mask_len)[i % 4];
      }
    }

    if (reass) {
      /* On first fragmented frame, nullify size */
      if (mg_is_ws_first_fragment(wsm.flags)) {
        mbuf_resize(&nc->recv_mbuf, nc->recv_mbuf.size + sizeof(*sizep));
        p[0] &= ~0x0f; /* Next frames will be treated as continuation */
        buf = p + 1 + sizeof(*sizep);
        *sizep = 0; /* TODO(lsm): fix. this can stomp over frame data */
      }

      /* Append this frame to the reassembled buffer */
      memmove(buf, wsm.data, e - wsm.data);
      (*sizep) += wsm.size;
      nc->recv_mbuf.len -= wsm.data - buf;

      /* On last fragmented frame - call user handler and remove data */
      if (wsm.flags & 0x80) {
        wsm.data = p + 1 + sizeof(*sizep);
        wsm.size = *sizep;
        mg_handle_incoming_websocket_frame(nc, &wsm);
        mbuf_remove(&nc->recv_mbuf, 1 + sizeof(*sizep) + *sizep);
      }
    } else {
      /* TODO(lsm): properly handle OOB control frames during defragmentation */
      mg_handle_incoming_websocket_frame(nc, &wsm);
      mbuf_remove(&nc->recv_mbuf, (size_t) frame_len); /* Cleanup frame */
    }

    /* If client closes, close too */
    if ((buf[0] & 0x0f) == WEBSOCKET_OP_CLOSE) {
      nc->flags |= MG_F_SEND_AND_CLOSE;
    }
  }

  return ok;
}

struct ws_mask_ctx {
  size_t pos; /* zero means unmasked */
  uint32_t mask;
};

static uint32_t mg_ws_random_mask(void) {
  uint32_t mask;
/*
 * The spec requires WS client to generate hard to
 * guess mask keys. From RFC6455, Section 5.3:
 *
 * The unpredictability of the masking key is essential to prevent
 * authors of malicious applications from selecting the bytes that appear on
 * the wire.
 *
 * Hence this feature is essential when the actual end user of this API
 * is untrusted code that wouldn't have access to a lower level net API
 * anyway (e.g. web browsers). Hence this feature is low prio for most
 * mongoose use cases and thus can be disabled, e.g. when porting to a platform
 * that lacks random().
 */
#ifdef MG_DISABLE_WS_RANDOM_MASK
  mask = 0xefbeadde; /* generated with a random number generator, I swear */
#else
  if (sizeof(long) >= 4) {
    mask = (uint32_t) random();
  } else if (sizeof(long) == 2) {
    mask = (uint32_t) random() << 16 | (uint32_t) random();
  }
#endif
  return mask;
}

static void mg_send_ws_header(struct mg_connection *nc, int op, size_t len,
                              struct ws_mask_ctx *ctx) {
  int header_len;
  unsigned char header[10];

  header[0] = (op & WEBSOCKET_DONT_FIN ? 0x0 : 0x80) + (op & 0x0f);
  if (len < 126) {
    header[1] = len;
    header_len = 2;
  } else if (len < 65535) {
    uint16_t tmp = htons((uint16_t) len);
    header[1] = 126;
    memcpy(&header[2], &tmp, sizeof(tmp));
    header_len = 4;
  } else {
    uint32_t tmp;
    header[1] = 127;
    tmp = htonl((uint32_t)((uint64_t) len >> 32));
    memcpy(&header[2], &tmp, sizeof(tmp));
    tmp = htonl((uint32_t)(len & 0xffffffff));
    memcpy(&header[6], &tmp, sizeof(tmp));
    header_len = 10;
  }

  /* client connections enable masking */
  if (nc->listener == NULL) {
    header[1] |= 1 << 7; /* set masking flag */
    mg_send(nc, header, header_len);
    ctx->mask = mg_ws_random_mask();
    mg_send(nc, &ctx->mask, sizeof(ctx->mask));
    ctx->pos = nc->send_mbuf.len;
  } else {
    mg_send(nc, header, header_len);
    ctx->pos = 0;
  }
}

static void mg_ws_mask_frame(struct mbuf *mbuf, struct ws_mask_ctx *ctx) {
  size_t i;
  if (ctx->pos == 0) return;
  for (i = 0; i < (mbuf->len - ctx->pos); i++) {
    mbuf->buf[ctx->pos + i] ^= ((char *) &ctx->mask)[i % 4];
  }
}

void mg_send_websocket_frame(struct mg_connection *nc, int op, const void *data,
                             size_t len) {
  struct ws_mask_ctx ctx;
  DBG(("%p %d %d", nc, op, (int) len));
  mg_send_ws_header(nc, op, len, &ctx);
  mg_send(nc, data, len);

  mg_ws_mask_frame(&nc->send_mbuf, &ctx);

  if (op == WEBSOCKET_OP_CLOSE) {
    nc->flags |= MG_F_SEND_AND_CLOSE;
  }
}

void mg_send_websocket_framev(struct mg_connection *nc, int op,
                              const struct mg_str *strv, int strvcnt) {
  struct ws_mask_ctx ctx;
  int i;
  int len = 0;
  for (i = 0; i < strvcnt; i++) {
    len += strv[i].len;
  }

  mg_send_ws_header(nc, op, len, &ctx);

  for (i = 0; i < strvcnt; i++) {
    mg_send(nc, strv[i].p, strv[i].len);
  }

  mg_ws_mask_frame(&nc->send_mbuf, &ctx);

  if (op == WEBSOCKET_OP_CLOSE) {
    nc->flags |= MG_F_SEND_AND_CLOSE;
  }
}

void mg_printf_websocket_frame(struct mg_connection *nc, int op,
                               const char *fmt, ...) {
  char mem[MG_VPRINTF_BUFFER_SIZE], *buf = mem;
  va_list ap;
  int len;

  va_start(ap, fmt);
  if ((len = mg_avprintf(&buf, sizeof(mem), fmt, ap)) > 0) {
    mg_send_websocket_frame(nc, op, buf, len);
  }
  va_end(ap);

  if (buf != mem && buf != NULL) {
    MG_FREE(buf);
  }
}

static void mg_websocket_handler(struct mg_connection *nc, int ev,
                                 void *ev_data) {
  mg_call(nc, nc->handler, ev, ev_data);

  switch (ev) {
    case MG_EV_RECV:
      do {
      } while (mg_deliver_websocket_data(nc));
      break;
    case MG_EV_POLL:
      /* Ping idle websocket connections */
      {
        time_t now = *(time_t *) ev_data;
        if (nc->flags & MG_F_IS_WEBSOCKET &&
            now > nc->last_io_time + MG_WEBSOCKET_PING_INTERVAL_SECONDS) {
          mg_send_websocket_frame(nc, WEBSOCKET_OP_PING, "", 0);
        }
      }
      break;
    default:
      break;
  }
}

#ifndef MG_EXT_SHA1
static void mg_hash_sha1_v(size_t num_msgs, const uint8_t *msgs[],
                           const size_t *msg_lens, uint8_t *digest) {
  size_t i;
  cs_sha1_ctx sha_ctx;
  cs_sha1_init(&sha_ctx);
  for (i = 0; i < num_msgs; i++) {
    cs_sha1_update(&sha_ctx, msgs[i], msg_lens[i]);
  }
  cs_sha1_final(digest, &sha_ctx);
}
#else
extern void mg_hash_sha1_v(size_t num_msgs, const uint8_t *msgs[],
                           const size_t *msg_lens, uint8_t *digest);
#endif

static void mg_ws_handshake(struct mg_connection *nc,
                            const struct mg_str *key) {
  static const char *magic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
  const uint8_t *msgs[2] = {(const uint8_t *) key->p, (const uint8_t *) magic};
  const size_t msg_lens[2] = {key->len, 36};
  unsigned char sha[20];
  char b64_sha[30];

  mg_hash_sha1_v(2, msgs, msg_lens, sha);
  mg_base64_encode(sha, sizeof(sha), b64_sha);
  mg_printf(nc, "%s%s%s",
            "HTTP/1.1 101 Switching Protocols\r\n"
            "Upgrade: websocket\r\n"
            "Connection: Upgrade\r\n"
            "Sec-WebSocket-Accept: ",
            b64_sha, "\r\n\r\n");
  DBG(("%p %.*s %s", nc, (int) key->len, key->p, b64_sha));
}

#endif /* MG_DISABLE_HTTP_WEBSOCKET */

#ifndef MG_DISABLE_FILESYSTEM
static void mg_http_transfer_file_data(struct mg_connection *nc) {
  struct mg_http_proto_data *pd = mg_http_get_proto_data(nc);
  char buf[MG_MAX_HTTP_SEND_MBUF];
  int64_t left = pd->file.cl - pd->file.sent;
  size_t n = 0, to_read = 0;

  if (pd->file.type == DATA_FILE) {
    struct mbuf *io = &nc->send_mbuf;
    if (io->len < sizeof(buf)) {
      to_read = sizeof(buf) - io->len;
    }

    if (left > 0 && to_read > (size_t) left) {
      to_read = left;
    }

    if (to_read == 0) {
      /* Rate limiting. send_mbuf is too full, wait until it's drained. */
    } else if (pd->file.sent < pd->file.cl &&
               (n = fread(buf, 1, to_read, pd->file.fp)) > 0) {
      mg_send(nc, buf, n);
      pd->file.sent += n;
    } else {
      if (!pd->file.keepalive) nc->flags |= MG_F_SEND_AND_CLOSE;
      mg_http_free_proto_data_file(&pd->file);
    }
  } else if (pd->file.type == DATA_PUT) {
    struct mbuf *io = &nc->recv_mbuf;
    size_t to_write =
        left <= 0 ? 0 : left < (int64_t) io->len ? (size_t) left : io->len;
    size_t n = fwrite(io->buf, 1, to_write, pd->file.fp);
    if (n > 0) {
      mbuf_remove(io, n);
      pd->file.sent += n;
    }
    if (n == 0 || pd->file.sent >= pd->file.cl) {
      if (!pd->file.keepalive) nc->flags |= MG_F_SEND_AND_CLOSE;
      mg_http_free_proto_data_file(&pd->file);
    }
  }
#ifndef MG_DISABLE_CGI
  else if (pd->cgi.cgi_nc != NULL) {
    /* This is POST data that needs to be forwarded to the CGI process */
    if (pd->cgi.cgi_nc != NULL) {
      mg_forward(nc, pd->cgi.cgi_nc);
    } else {
      nc->flags |= MG_F_SEND_AND_CLOSE;
    }
  }
#endif
}
#endif /* MG_DISABLE_FILESYSTEM */

/*
 * Parse chunked-encoded buffer. Return 0 if the buffer is not encoded, or
 * if it's incomplete. If the chunk is fully buffered, return total number of
 * bytes in a chunk, and store data in `data`, `data_len`.
 */
static size_t mg_http_parse_chunk(char *buf, size_t len, char **chunk_data,
                                  size_t *chunk_len) {
  unsigned char *s = (unsigned char *) buf;
  size_t n = 0; /* scanned chunk length */
  size_t i = 0; /* index in s */

  /* Scan chunk length. That should be a hexadecimal number. */
  while (i < len && isxdigit(s[i])) {
    n *= 16;
    n += (s[i] >= '0' && s[i] <= '9') ? s[i] - '0' : tolower(s[i]) - 'a' + 10;
    i++;
  }

  /* Skip new line */
  if (i == 0 || i + 2 > len || s[i] != '\r' || s[i + 1] != '\n') {
    return 0;
  }
  i += 2;

  /* Record where the data is */
  *chunk_data = (char *) s + i;
  *chunk_len = n;

  /* Skip data */
  i += n;

  /* Skip new line */
  if (i == 0 || i + 2 > len || s[i] != '\r' || s[i + 1] != '\n') {
    return 0;
  }
  return i + 2;
}

MG_INTERNAL size_t mg_handle_chunked(struct mg_connection *nc,
                                     struct http_message *hm, char *buf,
                                     size_t blen) {
  struct mg_http_proto_data *pd = mg_http_get_proto_data(nc);
  char *data;
  size_t i, n, data_len, body_len, zero_chunk_received = 0;

  /* Find out piece of received data that is not yet reassembled */
  body_len = pd->chunk.body_len;
  assert(blen >= body_len);

  /* Traverse all fully buffered chunks */
  for (i = body_len;
       (n = mg_http_parse_chunk(buf + i, blen - i, &data, &data_len)) > 0;
       i += n) {
    /* Collapse chunk data to the rest of HTTP body */
    memmove(buf + body_len, data, data_len);
    body_len += data_len;
    hm->body.len = body_len;

    if (data_len == 0) {
      zero_chunk_received = 1;
      i += n;
      break;
    }
  }

  if (i > body_len) {
    /* Shift unparsed content to the parsed body */
    assert(i <= blen);
    memmove(buf + body_len, buf + i, blen - i);
    memset(buf + body_len + blen - i, 0, i - body_len);
    nc->recv_mbuf.len -= i - body_len;
    pd->chunk.body_len = body_len;

    /* Send MG_EV_HTTP_CHUNK event */
    nc->flags &= ~MG_F_DELETE_CHUNK;
    mg_call(nc, nc->handler, MG_EV_HTTP_CHUNK, hm);

    /* Delete processed data if user set MG_F_DELETE_CHUNK flag */
    if (nc->flags & MG_F_DELETE_CHUNK) {
      memset(buf, 0, body_len);
      memmove(buf, buf + body_len, blen - i);
      nc->recv_mbuf.len -= body_len;
      hm->body.len = pd->chunk.body_len = 0;
    }

    if (zero_chunk_received) {
      hm->message.len = pd->chunk.body_len + blen - i;
    }
  }

  return body_len;
}

static mg_event_handler_t mg_http_get_endpoint_handler(
    struct mg_connection *nc, struct mg_str *uri_path) {
  struct mg_http_proto_data *pd;
  mg_event_handler_t ret = NULL;
  int matched, matched_max = 0;
  struct mg_http_endpoint *ep;

  if (nc == NULL) {
    return NULL;
  }

  pd = mg_http_get_proto_data(nc);

  ep = pd->endpoints;
  while (ep != NULL) {
    const struct mg_str name_s = {ep->name, ep->name_len};
    if ((matched = mg_match_prefix_n(name_s, *uri_path)) != -1) {
      if (matched > matched_max) {
        /* Looking for the longest suitable handler */
        ret = ep->handler;
        matched_max = matched;
      }
    }

    ep = ep->next;
  }

  return ret;
}

static void mg_http_call_endpoint_handler(struct mg_connection *nc, int ev,
                                          struct http_message *hm) {
  struct mg_http_proto_data *pd = mg_http_get_proto_data(nc);

  if (pd->endpoint_handler == NULL || ev == MG_EV_HTTP_REQUEST) {
    pd->endpoint_handler =
        ev == MG_EV_HTTP_REQUEST
            ? mg_http_get_endpoint_handler(nc->listener, &hm->uri)
            : NULL;
  }
  mg_call(nc, pd->endpoint_handler ? pd->endpoint_handler : nc->handler, ev,
          hm);
}

#ifdef MG_ENABLE_HTTP_STREAMING_MULTIPART
static void mg_http_multipart_continue(struct mg_connection *nc);

static void mg_http_multipart_begin(struct mg_connection *nc,
                                    struct http_message *hm, int req_len);

#endif

/*
 * lx106 compiler has a bug (TODO(mkm) report and insert tracking bug here)
 * If a big structure is declared in a big function, lx106 gcc will make it
 * even bigger (round up to 4k, from 700 bytes of actual size).
 */
#ifdef __xtensa__
static void mg_http_handler2(struct mg_connection *nc, int ev, void *ev_data,
                             struct http_message *hm) __attribute__((noinline));

void mg_http_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message hm;
  mg_http_handler2(nc, ev, ev_data, &hm);
}

static void mg_http_handler2(struct mg_connection *nc, int ev, void *ev_data,
                             struct http_message *hm) {
#else  /* !__XTENSA__ */
void mg_http_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message shm;
  struct http_message *hm = &shm;
#endif /* __XTENSA__ */
  struct mg_http_proto_data *pd = mg_http_get_proto_data(nc);
  struct mbuf *io = &nc->recv_mbuf;
  int req_len;
  const int is_req = (nc->listener != NULL);
#ifndef MG_DISABLE_HTTP_WEBSOCKET
  struct mg_str *vec;
#endif
  if (ev == MG_EV_CLOSE) {
#ifdef MG_ENABLE_HTTP_STREAMING_MULTIPART
    if (pd->mp_stream.boundary != NULL) {
      /*
       * Multipart message is in progress, but we get close
       * MG_EV_HTTP_PART_END with error flag
       */
      struct mg_http_multipart_part mp;
      memset(&mp, 0, sizeof(mp));

      mp.status = -1;
      mp.var_name = pd->mp_stream.var_name;
      mp.file_name = pd->mp_stream.file_name;
      mg_call(nc, (pd->endpoint_handler ? pd->endpoint_handler : nc->handler),
              MG_EV_HTTP_PART_END, &mp);
    } else
#endif
        if (io->len > 0 && mg_parse_http(io->buf, io->len, hm, is_req) > 0) {
      /*
      * For HTTP messages without Content-Length, always send HTTP message
      * before MG_EV_CLOSE message.
      */
      int ev2 = is_req ? MG_EV_HTTP_REQUEST : MG_EV_HTTP_REPLY;
      hm->message.len = io->len;
      hm->body.len = io->buf + io->len - hm->body.p;
      mg_http_call_endpoint_handler(nc, ev2, hm);
    }
  }

#ifndef MG_DISABLE_FILESYSTEM
  if (pd->file.fp != NULL) {
    mg_http_transfer_file_data(nc);
  }
#endif

  mg_call(nc, nc->handler, ev, ev_data);

  if (ev == MG_EV_RECV) {
    struct mg_str *s;

#ifdef MG_ENABLE_HTTP_STREAMING_MULTIPART
    if (pd->mp_stream.boundary != NULL) {
      mg_http_multipart_continue(nc);
      return;
    }
#endif /* MG_ENABLE_HTTP_STREAMING_MULTIPART */

    req_len = mg_parse_http(io->buf, io->len, hm, is_req);

    if (req_len > 0 &&
        (s = mg_get_http_header(hm, "Transfer-Encoding")) != NULL &&
        mg_vcasecmp(s, "chunked") == 0) {
      mg_handle_chunked(nc, hm, io->buf + req_len, io->len - req_len);
    }

#ifdef MG_ENABLE_HTTP_STREAMING_MULTIPART
    if (req_len > 0 && (s = mg_get_http_header(hm, "Content-Type")) != NULL &&
        s->len >= 9 && strncmp(s->p, "multipart", 9) == 0) {
      mg_http_multipart_begin(nc, hm, req_len);
      mg_http_multipart_continue(nc);
      return;
    }
#endif /* MG_ENABLE_HTTP_STREAMING_MULTIPART */

    /* TODO(alashkin): refactor this ifelseifelseifelseifelse */
    if ((req_len < 0 ||
         (req_len == 0 && io->len >= MG_MAX_HTTP_REQUEST_SIZE))) {
      DBG(("invalid request"));
      nc->flags |= MG_F_CLOSE_IMMEDIATELY;
    } else if (req_len == 0) {
      /* Do nothing, request is not yet fully buffered */
    }
#ifndef MG_DISABLE_HTTP_WEBSOCKET
    else if (nc->listener == NULL &&
             mg_get_http_header(hm, "Sec-WebSocket-Accept")) {
      /* We're websocket client, got handshake response from server. */
      /* TODO(lsm): check the validity of accept Sec-WebSocket-Accept */
      mbuf_remove(io, req_len);
      nc->proto_handler = mg_websocket_handler;
      nc->flags |= MG_F_IS_WEBSOCKET;
      mg_call(nc, nc->handler, MG_EV_WEBSOCKET_HANDSHAKE_DONE, NULL);
      mg_websocket_handler(nc, MG_EV_RECV, ev_data);
    } else if (nc->listener != NULL &&
               (vec = mg_get_http_header(hm, "Sec-WebSocket-Key")) != NULL) {
      /* This is a websocket request. Switch protocol handlers. */
      mbuf_remove(io, req_len);
      nc->proto_handler = mg_websocket_handler;
      nc->flags |= MG_F_IS_WEBSOCKET;

      /* Send handshake */
      mg_call(nc, nc->handler, MG_EV_WEBSOCKET_HANDSHAKE_REQUEST, hm);
      if (!(nc->flags & MG_F_CLOSE_IMMEDIATELY)) {
        if (nc->send_mbuf.len == 0) {
          mg_ws_handshake(nc, vec);
        }
        mg_call(nc, nc->handler, MG_EV_WEBSOCKET_HANDSHAKE_DONE, NULL);
        mg_websocket_handler(nc, MG_EV_RECV, ev_data);
      }
    }
#endif /* MG_DISABLE_HTTP_WEBSOCKET */
    else if (hm->message.len <= io->len) {
      int trigger_ev = nc->listener ? MG_EV_HTTP_REQUEST : MG_EV_HTTP_REPLY;

/* Whole HTTP message is fully buffered, call event handler */

#ifdef MG_ENABLE_JAVASCRIPT
      v7_val_t v1, v2, headers, req, args, res;
      struct v7 *v7 = nc->mgr->v7;
      const char *ev_name = trigger_ev == MG_EV_HTTP_REPLY ? "onsnd" : "onrcv";
      int i, js_callback_handled_request = 0;

      if (v7 != NULL) {
        /* Lookup JS callback */
        v1 = v7_get(v7, v7_get_global(v7), "Http", ~0);
        v2 = v7_get(v7, v1, ev_name, ~0);

        /* Create callback params. TODO(lsm): own/disown those */
        args = v7_mk_array(v7);
        req = v7_mk_object(v7);
        headers = v7_mk_object(v7);

        /* Populate request object */
        v7_set(v7, req, "method", ~0,
               v7_mk_string(v7, hm->method.p, hm->method.len, 1));
        v7_set(v7, req, "uri", ~0, v7_mk_string(v7, hm->uri.p, hm->uri.len, 1));
        v7_set(v7, req, "body", ~0,
               v7_mk_string(v7, hm->body.p, hm->body.len, 1));
        v7_set(v7, req, "headers", ~0, headers);
        for (i = 0; hm->header_names[i].len > 0; i++) {
          const struct mg_str *name = &hm->header_names[i];
          const struct mg_str *value = &hm->header_values[i];
          v7_set(v7, headers, name->p, name->len,
                 v7_mk_string(v7, value->p, value->len, 1));
        }

        /* Invoke callback. TODO(lsm): report errors */
        v7_array_push(v7, args, v7_mk_foreign(v7, nc));
        v7_array_push(v7, args, req);
        if (v7_apply(v7, v2, V7_UNDEFINED, args, &res) == V7_OK &&
            v7_is_truthy(v7, res)) {
          js_callback_handled_request++;
        }
      }

      /* If JS callback returns true, stop request processing */
      if (js_callback_handled_request) {
        nc->flags |= MG_F_SEND_AND_CLOSE;
      } else {
        mg_http_call_endpoint_handler(nc, trigger_ev, hm);
      }
#else
      mg_http_call_endpoint_handler(nc, trigger_ev, hm);
#endif
      mbuf_remove(io, hm->message.len);
    }
  }
  (void) pd;
}

static size_t mg_get_line_len(const char *buf, size_t buf_len) {
  size_t len = 0;
  while (len < buf_len && buf[len] != '\n') len++;
  return len == buf_len ? 0 : len + 1;
}

#ifdef MG_ENABLE_HTTP_STREAMING_MULTIPART
static void mg_http_multipart_begin(struct mg_connection *nc,
                                    struct http_message *hm, int req_len) {
  struct mg_http_proto_data *pd = mg_http_get_proto_data(nc);
  struct mg_str *ct;
  struct mbuf *io = &nc->recv_mbuf;

  char boundary[100];
  int boundary_len;

  if (nc->listener == NULL) {
    /* No streaming for replies now */
    goto exit_mp;
  }

  ct = mg_get_http_header(hm, "Content-Type");
  if (ct == NULL) {
    /* We need more data - or it isn't multipart mesage */
    goto exit_mp;
  }

  /* Content-type should start with "multipart" */
  if (ct->len < 9 || strncmp(ct->p, "multipart", 9) != 0) {
    goto exit_mp;
  }

  boundary_len =
      mg_http_parse_header(ct, "boundary", boundary, sizeof(boundary));
  if (boundary_len == 0) {
    /*
     * Content type is multipart, but there is no boundary,
     * probably malformed request
     */
    nc->flags = MG_F_CLOSE_IMMEDIATELY;
    DBG(("invalid request"));
    goto exit_mp;
  }

  /* If we reach this place - that is multipart request */

  if (pd->mp_stream.boundary != NULL) {
    /*
     * Another streaming request was in progress,
     * looks like protocol error
     */
    nc->flags |= MG_F_CLOSE_IMMEDIATELY;
  } else {
    pd->mp_stream.state = MPS_BEGIN;
    pd->mp_stream.boundary = strdup(boundary);
    pd->mp_stream.boundary_len = strlen(boundary);
    pd->mp_stream.var_name = pd->mp_stream.file_name = NULL;

    pd->endpoint_handler = mg_http_get_endpoint_handler(nc->listener, &hm->uri);
    if (pd->endpoint_handler == NULL) {
      pd->endpoint_handler = nc->handler;
    }

    mg_call(nc, pd->endpoint_handler, MG_EV_HTTP_MULTIPART_REQUEST, hm);

    mbuf_remove(io, req_len);
  }
exit_mp:
  ;
}

#define CONTENT_DISPOSITION "Content-Disposition: "

static void mg_http_multipart_call_handler(struct mg_connection *c, int ev,
                                           const char *data, size_t data_len) {
  struct mg_http_multipart_part mp;
  struct mg_http_proto_data *pd = mg_http_get_proto_data(c);
  memset(&mp, 0, sizeof(mp));

  mp.var_name = pd->mp_stream.var_name;
  mp.file_name = pd->mp_stream.file_name;
  mp.user_data = pd->mp_stream.user_data;
  mp.data.p = data;
  mp.data.len = data_len;
  mg_call(c, pd->endpoint_handler, ev, &mp);
  pd->mp_stream.user_data = mp.user_data;
}

static int mg_http_multipart_got_chunk(struct mg_connection *c) {
  struct mg_http_proto_data *pd = mg_http_get_proto_data(c);
  struct mbuf *io = &c->recv_mbuf;

  mg_http_multipart_call_handler(c, MG_EV_HTTP_PART_DATA, io->buf,
                                 pd->mp_stream.prev_io_len);
  mbuf_remove(io, pd->mp_stream.prev_io_len);
  pd->mp_stream.prev_io_len = 0;
  pd->mp_stream.state = MPS_WAITING_FOR_CHUNK;

  return 0;
}

static int mg_http_multipart_finalize(struct mg_connection *c) {
  struct mg_http_proto_data *pd = mg_http_get_proto_data(c);

  mg_http_multipart_call_handler(c, MG_EV_HTTP_PART_END, NULL, 0);
  mg_http_free_proto_data_mp_stream(&pd->mp_stream);
  pd->mp_stream.state = MPS_FINISHED;

  return 1;
}

static int mg_http_multipart_wait_for_boundary(struct mg_connection *c) {
  const char *boundary;
  struct mbuf *io = &c->recv_mbuf;
  struct mg_http_proto_data *pd = mg_http_get_proto_data(c);

  if ((int) io->len < pd->mp_stream.boundary_len + 2) {
    return 0;
  }

  boundary = c_strnstr(io->buf, pd->mp_stream.boundary, io->len);
  if (boundary != NULL) {
    if (io->len - (boundary - io->buf) < 4) {
      return 0;
    }
    if (memcmp(boundary + pd->mp_stream.boundary_len, "--", 2) == 0) {
      pd->mp_stream.state = MPS_FINALIZE;
    } else {
      pd->mp_stream.state = MPS_GOT_BOUNDARY;
    }
  } else {
    return 0;
  }

  return 1;
}

static int mg_http_multipart_process_boundary(struct mg_connection *c) {
  int data_size;
  const char *boundary, *block_begin;
  struct mbuf *io = &c->recv_mbuf;
  struct mg_http_proto_data *pd = mg_http_get_proto_data(c);
  char file_name[100], var_name[100];
  int line_len;
  boundary = c_strnstr(io->buf, pd->mp_stream.boundary, io->len);
  block_begin = boundary + pd->mp_stream.boundary_len + 2;
  data_size = io->len - (block_begin - io->buf);

  while (data_size > 0 &&
         (line_len = mg_get_line_len(block_begin, data_size)) != 0) {
    if (line_len > (int) sizeof(CONTENT_DISPOSITION) &&
        mg_ncasecmp(block_begin, CONTENT_DISPOSITION,
                    sizeof(CONTENT_DISPOSITION) - 1) == 0) {
      struct mg_str header;

      header.p = block_begin + sizeof(CONTENT_DISPOSITION) - 1;
      header.len = line_len - sizeof(CONTENT_DISPOSITION) - 1;
      mg_http_parse_header(&header, "name", var_name, sizeof(var_name) - 2);
      mg_http_parse_header(&header, "filename", file_name,
                           sizeof(file_name) - 2);
      block_begin += line_len;
      data_size -= line_len;
      continue;
    }

    if (line_len == 2 && mg_ncasecmp(block_begin, "\r\n", 2) == 0) {
      mbuf_remove(io, block_begin - io->buf + 2);

      if (pd->mp_stream.processing_part != 0) {
        mg_http_multipart_call_handler(c, MG_EV_HTTP_PART_END, NULL, 0);
      }

      free((void *) pd->mp_stream.file_name);
      pd->mp_stream.file_name = strdup(file_name);
      free((void *) pd->mp_stream.var_name);
      pd->mp_stream.var_name = strdup(var_name);

      mg_http_multipart_call_handler(c, MG_EV_HTTP_PART_BEGIN, NULL, 0);
      pd->mp_stream.state = MPS_WAITING_FOR_CHUNK;
      pd->mp_stream.processing_part++;
      return 1;
    }

    block_begin += line_len;
  }

  pd->mp_stream.state = MPS_WAITING_FOR_BOUNDARY;

  return 0;
}

static int mg_http_multipart_continue_wait_for_chunk(struct mg_connection *c) {
  struct mg_http_proto_data *pd = mg_http_get_proto_data(c);
  struct mbuf *io = &c->recv_mbuf;

  const char *boundary;
  if ((int) io->len < pd->mp_stream.boundary_len + 6 /* \r\n, --, -- */) {
    return 0;
  }

  boundary = c_strnstr(io->buf, pd->mp_stream.boundary, io->len);
  if (boundary == NULL && pd->mp_stream.prev_io_len == 0) {
    pd->mp_stream.prev_io_len = io->len;
    return 0;
  } else if (boundary == NULL &&
             (int) io->len >
                 pd->mp_stream.prev_io_len + pd->mp_stream.boundary_len + 4) {
    pd->mp_stream.state = MPS_GOT_CHUNK;
    return 1;
  } else if (boundary != NULL) {
    int data_size = (boundary - io->buf - 4);
    mg_http_multipart_call_handler(c, MG_EV_HTTP_PART_DATA, io->buf, data_size);
    mbuf_remove(io, (boundary - io->buf));
    pd->mp_stream.prev_io_len = 0;
    pd->mp_stream.state = MPS_WAITING_FOR_BOUNDARY;
    return 1;
  } else {
    return 0;
  }
}

static void mg_http_multipart_continue(struct mg_connection *c) {
  struct mg_http_proto_data *pd = mg_http_get_proto_data(c);
  while (1) {
    switch (pd->mp_stream.state) {
      case MPS_BEGIN: {
        pd->mp_stream.state = MPS_WAITING_FOR_BOUNDARY;
        break;
      }
      case MPS_WAITING_FOR_BOUNDARY: {
        if (mg_http_multipart_wait_for_boundary(c) == 0) {
          return;
        }
        break;
      }
      case MPS_GOT_BOUNDARY: {
        if (mg_http_multipart_process_boundary(c) == 0) {
          return;
        }
        break;
      }
      case MPS_WAITING_FOR_CHUNK: {
        if (mg_http_multipart_continue_wait_for_chunk(c) == 0) {
          return;
        }
        break;
      }
      case MPS_GOT_CHUNK: {
        if (mg_http_multipart_got_chunk(c) == 0) {
          return;
        }
        break;
      }
      case MPS_FINALIZE: {
        if (mg_http_multipart_finalize(c) == 0) {
          return;
        }
        break;
      }
      case MPS_FINISHED: {
        mbuf_remove(&c->recv_mbuf, c->recv_mbuf.len);
        return;
      }
    }
  }
}

struct file_upload_state {
  char *lfn;
  size_t num_recd;
  FILE *fp;
};

void mg_file_upload_handler(struct mg_connection *nc, int ev, void *ev_data,
                            mg_fu_fname_fn local_name_fn) {
  switch (ev) {
    case MG_EV_HTTP_PART_BEGIN: {
      struct mg_http_multipart_part *mp =
          (struct mg_http_multipart_part *) ev_data;
      struct file_upload_state *fus =
          (struct file_upload_state *) calloc(1, sizeof(*fus));
      mp->user_data = NULL;

      struct mg_str lfn = local_name_fn(nc, mg_mk_str(mp->file_name));
      if (lfn.p == NULL || lfn.len == 0) {
        LOG(LL_ERROR, ("%p Not allowed to upload %s", nc, mp->file_name));
        mg_printf(nc,
                  "HTTP/1.1 403 Not Allowed\r\n"
                  "Content-Type: text/plain\r\n"
                  "Connection: close\r\n\r\n"
                  "Not allowed to upload %s\r\n",
                  mp->file_name);
        nc->flags |= MG_F_SEND_AND_CLOSE;
        return;
      }
      fus->lfn = (char *) malloc(lfn.len + 1);
      memcpy(fus->lfn, lfn.p, lfn.len);
      fus->lfn[lfn.len] = '\0';
      if (lfn.p != mp->file_name) free((char *) lfn.p);
      LOG(LL_DEBUG,
          ("%p Receiving file %s -> %s", nc, mp->file_name, fus->lfn));
      fus->fp = fopen(fus->lfn, "w");
      if (fus->fp == NULL) {
        mg_printf(nc,
                  "HTTP/1.1 500 Internal Server Error\r\n"
                  "Content-Type: text/plain\r\n"
                  "Connection: close\r\n\r\n");
        LOG(LL_ERROR, ("Failed to open %s: %d\n", fus->lfn, errno));
        mg_printf(nc, "Failed to open %s: %d\n", fus->lfn, errno);
        /* Do not close the connection just yet, discard remainder of the data.
         * This is because at the time of writing some browsers (Chrome) fail to
         * render response before all the data is sent. */
      }
      mp->user_data = (void *) fus;
      break;
    }
    case MG_EV_HTTP_PART_DATA: {
      struct mg_http_multipart_part *mp =
          (struct mg_http_multipart_part *) ev_data;
      struct file_upload_state *fus =
          (struct file_upload_state *) mp->user_data;
      if (fus == NULL || fus->fp == NULL) break;
      if (fwrite(mp->data.p, 1, mp->data.len, fus->fp) != mp->data.len) {
        LOG(LL_ERROR, ("Failed to write to %s: %d, wrote %d", fus->lfn, errno,
                       (int) fus->num_recd));
        if (errno == ENOSPC
#ifdef SPIFFS_ERR_FULL
            || errno == SPIFFS_ERR_FULL
#endif
            ) {
          mg_printf(nc,
                    "HTTP/1.1 413 Payload Too Large\r\n"
                    "Content-Type: text/plain\r\n"
                    "Connection: close\r\n\r\n");
          mg_printf(nc, "Failed to write to %s: no space left; wrote %d\r\n",
                    fus->lfn, (int) fus->num_recd);
        } else {
          mg_printf(nc,
                    "HTTP/1.1 500 Internal Server Error\r\n"
                    "Content-Type: text/plain\r\n"
                    "Connection: close\r\n\r\n");
          mg_printf(nc, "Failed to write to %s: %d, wrote %d", mp->file_name,
                    errno, (int) fus->num_recd);
        }
        fclose(fus->fp);
        remove(fus->lfn);
        fus->fp = NULL;
        /* Do not close the connection just yet, discard remainder of the data.
         * This is because at the time of writing some browsers (Chrome) fail to
         * render response before all the data is sent. */
        return;
      }
      fus->num_recd += mp->data.len;
      LOG(LL_DEBUG, ("%p rec'd %d bytes, %d total", nc, (int) mp->data.len,
                     (int) fus->num_recd));
      break;
    }
    case MG_EV_HTTP_PART_END: {
      struct mg_http_multipart_part *mp =
          (struct mg_http_multipart_part *) ev_data;
      struct file_upload_state *fus =
          (struct file_upload_state *) mp->user_data;
      if (fus == NULL) break;
      if (mp->status >= 0 && fus->fp != NULL) {
        LOG(LL_DEBUG, ("%p Uploaded %s (%s), %d bytes", nc, mp->file_name,
                       fus->lfn, (int) fus->num_recd));
        mg_printf(nc,
                  "HTTP/1.1 200 OK\r\n"
                  "Content-Type: text/plain\r\n"
                  "Connection: close\r\n\r\n"
                  "Ok, %s - %d bytes.\r\n",
                  mp->file_name, (int) fus->num_recd);
      } else {
        LOG(LL_ERROR, ("Failed to store %s (%s)", mp->file_name, fus->lfn));
        /*
         * mp->status < 0 means connection was terminated, so no reason to send
         * HTTP reply
         */
      }
      if (fus->fp != NULL) fclose(fus->fp);
      free(fus->lfn);
      free(fus);
      mp->user_data = NULL;
      nc->flags |= MG_F_SEND_AND_CLOSE;
      break;
    }
  }
}

#endif /* MG_ENABLE_HTTP_STREAMING_MULTIPART */

void mg_set_protocol_http_websocket(struct mg_connection *nc) {
  nc->proto_handler = mg_http_handler;
}

#ifndef MG_DISABLE_HTTP_WEBSOCKET

void mg_send_websocket_handshake2(struct mg_connection *nc, const char *path,
                                  const char *host, const char *protocol,
                                  const char *extra_headers) {
  /* pretty poor source of randomness, TODO fix */
  unsigned long random = (unsigned long) path;
  char key[sizeof(random) * 3];

  mg_base64_encode((unsigned char *) &random, sizeof(random), key);
  mg_printf(nc,
            "GET %s HTTP/1.1\r\n"
            "Upgrade: websocket\r\n"
            "Connection: Upgrade\r\n"
            "Sec-WebSocket-Version: 13\r\n"
            "Sec-WebSocket-Key: %s\r\n",
            path, key);

  /* TODO(mkm): take default hostname from http proto data if host == NULL */
  if (host != MG_WS_NO_HOST_HEADER_MAGIC) {
    mg_printf(nc, "Host: %s\r\n", host);
  }
  if (protocol != NULL) {
    mg_printf(nc, "Sec-WebSocket-Protocol: %s\r\n", protocol);
  }
  if (extra_headers != NULL) {
    mg_printf(nc, "%s", extra_headers);
  }
  mg_printf(nc, "\r\n");
}

void mg_send_websocket_handshake(struct mg_connection *nc, const char *path,
                                 const char *extra_headers) {
  mg_send_websocket_handshake2(nc, path, MG_WS_NO_HOST_HEADER_MAGIC, NULL,
                               extra_headers);
}

#endif /* MG_DISABLE_HTTP_WEBSOCKET */

void mg_send_response_line(struct mg_connection *nc, int status_code,
                           const char *extra_headers) {
  const char *status_message = "OK";
  switch (status_code) {
    case 206:
      status_message = "Partial Content";
      break;
    case 301:
      status_message = "Moved";
      break;
    case 302:
      status_message = "Found";
      break;
    case 401:
      status_message = "Unauthorized";
      break;
    case 403:
      status_message = "Forbidden";
      break;
    case 404:
      status_message = "Not Found";
      break;
    case 416:
      status_message = "Requested range not satisfiable";
      break;
    case 418:
      status_message = "I'm a teapot";
      break;
    case 500:
      status_message = "Internal Server Error";
      break;
  }
  mg_printf(nc, "HTTP/1.1 %d %s\r\nServer: %s\r\n", status_code, status_message,
            mg_version_header);
  if (extra_headers != NULL) {
    mg_printf(nc, "%s\r\n", extra_headers);
  }
}

void mg_send_head(struct mg_connection *c, int status_code,
                  int64_t content_length, const char *extra_headers) {
  mg_send_response_line(c, status_code, extra_headers);
  if (content_length < 0) {
    mg_printf(c, "%s", "Transfer-Encoding: chunked\r\n");
  } else {
    mg_printf(c, "Content-Length: %" INT64_FMT "\r\n", content_length);
  }
  mg_send(c, "\r\n", 2);
}

#ifdef MG_DISABLE_FILESYSTEM
void mg_serve_http(struct mg_connection *nc, struct http_message *hm,
                   struct mg_serve_http_opts opts) {
  mg_send_head(nc, 501, 0, NULL);
}
#else
static void mg_http_send_error(struct mg_connection *nc, int code,
                               const char *reason) {
  if (!reason) reason = "";
  DBG(("%p %d %s", nc, code, reason));
  mg_send_head(nc, code, strlen(reason),
               "Content-Type: text/plain\r\nConnection: close");
  mg_send(nc, reason, strlen(reason));
  nc->flags |= MG_F_SEND_AND_CLOSE;
}
#ifndef MG_DISABLE_SSI
static void mg_send_ssi_file(struct mg_connection *, const char *, FILE *, int,
                             const struct mg_serve_http_opts *);

static void mg_send_file_data(struct mg_connection *nc, FILE *fp) {
  char buf[BUFSIZ];
  size_t n;
  while ((n = fread(buf, 1, sizeof(buf), fp)) > 0) {
    mg_send(nc, buf, n);
  }
}

static void mg_do_ssi_include(struct mg_connection *nc, const char *ssi,
                              char *tag, int include_level,
                              const struct mg_serve_http_opts *opts) {
  char file_name[BUFSIZ], path[MAX_PATH_SIZE], *p;
  FILE *fp;

  /*
   * sscanf() is safe here, since send_ssi_file() also uses buffer
   * of size MG_BUF_LEN to get the tag. So strlen(tag) is always < MG_BUF_LEN.
   */
  if (sscanf(tag, " virtual=\"%[^\"]\"", file_name) == 1) {
    /* File name is relative to the webserver root */
    snprintf(path, sizeof(path), "%s/%s", opts->document_root, file_name);
  } else if (sscanf(tag, " abspath=\"%[^\"]\"", file_name) == 1) {
    /*
     * File name is relative to the webserver working directory
     * or it is absolute system path
     */
    snprintf(path, sizeof(path), "%s", file_name);
  } else if (sscanf(tag, " file=\"%[^\"]\"", file_name) == 1 ||
             sscanf(tag, " \"%[^\"]\"", file_name) == 1) {
    /* File name is relative to the currect document */
    snprintf(path, sizeof(path), "%s", ssi);
    if ((p = strrchr(path, DIRSEP)) != NULL) {
      p[1] = '\0';
    }
    snprintf(path + strlen(path), sizeof(path) - strlen(path), "%s", file_name);
  } else {
    mg_printf(nc, "Bad SSI #include: [%s]", tag);
    return;
  }

  if ((fp = fopen(path, "rb")) == NULL) {
    mg_printf(nc, "SSI include error: fopen(%s): %s", path, strerror(errno));
  } else {
    mg_set_close_on_exec(fileno(fp));
    if (mg_match_prefix(opts->ssi_pattern, strlen(opts->ssi_pattern), path) >
        0) {
      mg_send_ssi_file(nc, path, fp, include_level + 1, opts);
    } else {
      mg_send_file_data(nc, fp);
    }
    fclose(fp);
  }
}

#ifndef MG_DISABLE_POPEN
static void do_ssi_exec(struct mg_connection *nc, char *tag) {
  char cmd[BUFSIZ];
  FILE *fp;

  if (sscanf(tag, " \"%[^\"]\"", cmd) != 1) {
    mg_printf(nc, "Bad SSI #exec: [%s]", tag);
  } else if ((fp = popen(cmd, "r")) == NULL) {
    mg_printf(nc, "Cannot SSI #exec: [%s]: %s", cmd, strerror(errno));
  } else {
    mg_send_file_data(nc, fp);
    pclose(fp);
  }
}
#endif /* !MG_DISABLE_POPEN */

static void mg_do_ssi_call(struct mg_connection *nc, char *tag) {
  mg_call(nc, NULL, MG_EV_SSI_CALL, tag);
}

/*
 * SSI directive has the following format:
 * <!--#directive parameter=value parameter=value -->
 */
static void mg_send_ssi_file(struct mg_connection *nc, const char *path,
                             FILE *fp, int include_level,
                             const struct mg_serve_http_opts *opts) {
  static const struct mg_str btag = MG_MK_STR("<!--#");
  static const struct mg_str d_include = MG_MK_STR("include");
  static const struct mg_str d_call = MG_MK_STR("call");
#ifndef MG_DISABLE_POPEN
  static const struct mg_str d_exec = MG_MK_STR("exec");
#endif
  char buf[BUFSIZ], *p = buf + btag.len; /* p points to SSI directive */
  int ch, len, in_ssi_tag;

  if (include_level > 10) {
    mg_printf(nc, "SSI #include level is too deep (%s)", path);
    return;
  }

  in_ssi_tag = len = 0;
  while ((ch = fgetc(fp)) != EOF) {
    if (in_ssi_tag && ch == '>' && buf[len - 1] == '-' && buf[len - 2] == '-') {
      size_t i = len - 2;
      in_ssi_tag = 0;

      /* Trim closing --> */
      buf[i--] = '\0';
      while (i > 0 && buf[i] == ' ') {
        buf[i--] = '\0';
      }

      /* Handle known SSI directives */
      if (memcmp(p, d_include.p, d_include.len) == 0) {
        mg_do_ssi_include(nc, path, p + d_include.len + 1, include_level, opts);
      } else if (memcmp(p, d_call.p, d_call.len) == 0) {
        mg_do_ssi_call(nc, p + d_call.len + 1);
#ifndef MG_DISABLE_POPEN
      } else if (memcmp(p, d_exec.p, d_exec.len) == 0) {
        do_ssi_exec(nc, p + d_exec.len + 1);
#endif
      } else {
        /* Silently ignore unknown SSI directive. */
      }
      len = 0;
    } else if (ch == '<') {
      in_ssi_tag = 1;
      if (len > 0) {
        mg_send(nc, buf, (size_t) len);
      }
      len = 0;
      buf[len++] = ch & 0xff;
    } else if (in_ssi_tag) {
      if (len == (int) btag.len && memcmp(buf, btag.p, btag.len) != 0) {
        /* Not an SSI tag */
        in_ssi_tag = 0;
      } else if (len == (int) sizeof(buf) - 2) {
        mg_printf(nc, "%s: SSI tag is too large", path);
        len = 0;
      }
      buf[len++] = ch & 0xff;
    } else {
      buf[len++] = ch & 0xff;
      if (len == (int) sizeof(buf)) {
        mg_send(nc, buf, (size_t) len);
        len = 0;
      }
    }
  }

  /* Send the rest of buffered data */
  if (len > 0) {
    mg_send(nc, buf, (size_t) len);
  }
}

static void mg_handle_ssi_request(struct mg_connection *nc, const char *path,
                                  const struct mg_serve_http_opts *opts) {
  FILE *fp;
  struct mg_str mime_type;

  if ((fp = fopen(path, "rb")) == NULL) {
    mg_http_send_error(nc, 404, NULL);
  } else {
    mg_set_close_on_exec(fileno(fp));

    mime_type = mg_get_mime_type(path, "text/plain", opts);
    mg_send_response_line(nc, 200, opts->extra_headers);
    mg_printf(nc,
              "Content-Type: %.*s\r\n"
              "Connection: close\r\n\r\n",
              (int) mime_type.len, mime_type.p);
    mg_send_ssi_file(nc, path, fp, 0, opts);
    fclose(fp);
    nc->flags |= MG_F_SEND_AND_CLOSE;
  }
}
#else
static void mg_handle_ssi_request(struct mg_connection *nc, const char *path,
                                  const struct mg_serve_http_opts *opts) {
  (void) path;
  (void) opts;
  mg_http_send_error(nc, 500, "SSI disabled");
}
#endif /* MG_DISABLE_SSI */

static void mg_http_construct_etag(char *buf, size_t buf_len,
                                   const cs_stat_t *st) {
  snprintf(buf, buf_len, "\"%lx.%" INT64_FMT "\"", (unsigned long) st->st_mtime,
           (int64_t) st->st_size);
}
static void mg_gmt_time_string(char *buf, size_t buf_len, time_t *t) {
  strftime(buf, buf_len, "%a, %d %b %Y %H:%M:%S GMT", gmtime(t));
}

static int mg_http_parse_range_header(const struct mg_str *header, int64_t *a,
                                      int64_t *b) {
  /*
   * There is no snscanf. Headers are not guaranteed to be NUL-terminated,
   * so we have this. Ugh.
   */
  int result;
  char *p = (char *) MG_MALLOC(header->len + 1);
  if (p == NULL) return 0;
  memcpy(p, header->p, header->len);
  p[header->len] = '\0';
  result = sscanf(p, "bytes=%" INT64_FMT "-%" INT64_FMT, a, b);
  MG_FREE(p);
  return result;
}

static void mg_http_send_file2(struct mg_connection *nc, const char *path,
                               cs_stat_t *st, struct http_message *hm,
                               struct mg_serve_http_opts *opts) {
  struct mg_http_proto_data *pd = mg_http_get_proto_data(nc);
  struct mg_str mime_type;

  DBG(("%p [%s]", nc, path));
  mg_http_free_proto_data_file(&pd->file);
  if ((pd->file.fp = fopen(path, "rb")) == NULL) {
    int code;
    switch (errno) {
      case EACCES:
        code = 403;
        break;
      case ENOENT:
        code = 404;
        break;
      default:
        code = 500;
    };
    mg_http_send_error(nc, code, "Open failed");
  } else if (mg_match_prefix(opts->ssi_pattern, strlen(opts->ssi_pattern),
                             path) > 0) {
    mg_handle_ssi_request(nc, path, opts);
  } else {
    char etag[50], current_time[50], last_modified[50], range[70];
    time_t t = time(NULL);
    int64_t r1 = 0, r2 = 0, cl = st->st_size;
    struct mg_str *range_hdr = mg_get_http_header(hm, "Range");
    int n, status_code = 200;

    /* Handle Range header */
    range[0] = '\0';
    if (range_hdr != NULL &&
        (n = mg_http_parse_range_header(range_hdr, &r1, &r2)) > 0 && r1 >= 0 &&
        r2 >= 0) {
      /* If range is specified like "400-", set second limit to content len */
      if (n == 1) {
        r2 = cl - 1;
      }
      if (r1 > r2 || r2 >= cl) {
        status_code = 416;
        cl = 0;
        snprintf(range, sizeof(range),
                 "Content-Range: bytes */%" INT64_FMT "\r\n",
                 (int64_t) st->st_size);
      } else {
        status_code = 206;
        cl = r2 - r1 + 1;
        snprintf(range, sizeof(range), "Content-Range: bytes %" INT64_FMT
                                       "-%" INT64_FMT "/%" INT64_FMT "\r\n",
                 r1, r1 + cl - 1, (int64_t) st->st_size);
#if _FILE_OFFSET_BITS == 64 || _POSIX_C_SOURCE >= 200112L || \
    _XOPEN_SOURCE >= 600
        fseeko(pd->file.fp, r1, SEEK_SET);
#else
        fseek(pd->file.fp, r1, SEEK_SET);
#endif
      }
    }

#ifndef MG_DISABLE_HTTP_KEEP_ALIVE
    {
      struct mg_str *conn_hdr = mg_get_http_header(hm, "Connection");
      if (conn_hdr != NULL) {
        pd->file.keepalive = (mg_vcasecmp(conn_hdr, "keep-alive") == 0);
      } else {
        pd->file.keepalive = (mg_vcmp(&hm->proto, "HTTP/1.1") == 0);
      }
    }
#endif

    mg_http_construct_etag(etag, sizeof(etag), st);
    mg_gmt_time_string(current_time, sizeof(current_time), &t);
    mg_gmt_time_string(last_modified, sizeof(last_modified), &st->st_mtime);
    mime_type = mg_get_mime_type(path, "text/plain", opts);
    /*
     * Content length casted to size_t because:
     * 1) that's the maximum buffer size anyway
     * 2) ESP8266 RTOS SDK newlib vprintf cannot contain a 64bit arg at non-last
     *    position
     * TODO(mkm): fix ESP8266 RTOS SDK
     */
    mg_send_response_line(nc, status_code, opts->extra_headers);
    mg_printf(nc,
              "Date: %s\r\n"
              "Last-Modified: %s\r\n"
              "Accept-Ranges: bytes\r\n"
              "Content-Type: %.*s\r\n"
              "Connection: %s\r\n"
              "Content-Length: %" SIZE_T_FMT
              "\r\n"
              "%sEtag: %s\r\n\r\n",
              current_time, last_modified, (int) mime_type.len, mime_type.p,
              (pd->file.keepalive ? "keep-alive" : "close"), (size_t) cl, range,
              etag);

    pd->file.cl = cl;
    pd->file.type = DATA_FILE;
    mg_http_transfer_file_data(nc);
  }
}

#endif

int mg_url_decode(const char *src, int src_len, char *dst, int dst_len,
                  int is_form_url_encoded) {
  int i, j, a, b;
#define HEXTOI(x) (isdigit(x) ? x - '0' : x - 'W')

  for (i = j = 0; i < src_len && j < dst_len - 1; i++, j++) {
    if (src[i] == '%') {
      if (i < src_len - 2 && isxdigit(*(const unsigned char *) (src + i + 1)) &&
          isxdigit(*(const unsigned char *) (src + i + 2))) {
        a = tolower(*(const unsigned char *) (src + i + 1));
        b = tolower(*(const unsigned char *) (src + i + 2));
        dst[j] = (char) ((HEXTOI(a) << 4) | HEXTOI(b));
        i += 2;
      } else {
        return -1;
      }
    } else if (is_form_url_encoded && src[i] == '+') {
      dst[j] = ' ';
    } else {
      dst[j] = src[i];
    }
  }

  dst[j] = '\0'; /* Null-terminate the destination */

  return i >= src_len ? j : -1;
}

int mg_get_http_var(const struct mg_str *buf, const char *name, char *dst,
                    size_t dst_len) {
  const char *p, *e, *s;
  size_t name_len;
  int len;

  if (dst == NULL || dst_len == 0) {
    len = -2;
  } else if (buf->p == NULL || name == NULL || buf->len == 0) {
    len = -1;
    dst[0] = '\0';
  } else {
    name_len = strlen(name);
    e = buf->p + buf->len;
    len = -1;
    dst[0] = '\0';

    for (p = buf->p; p + name_len < e; p++) {
      if ((p == buf->p || p[-1] == '&') && p[name_len] == '=' &&
          !mg_ncasecmp(name, p, name_len)) {
        p += name_len + 1;
        s = (const char *) memchr(p, '&', (size_t)(e - p));
        if (s == NULL) {
          s = e;
        }
        len = mg_url_decode(p, (size_t)(s - p), dst, dst_len, 1);
        if (len == -1) {
          len = -2;
        }
        break;
      }
    }
  }

  return len;
}

void mg_send_http_chunk(struct mg_connection *nc, const char *buf, size_t len) {
  char chunk_size[50];
  int n;

  n = snprintf(chunk_size, sizeof(chunk_size), "%lX\r\n", (unsigned long) len);
  mg_send(nc, chunk_size, n);
  mg_send(nc, buf, len);
  mg_send(nc, "\r\n", 2);
}

void mg_printf_http_chunk(struct mg_connection *nc, const char *fmt, ...) {
  char mem[MG_VPRINTF_BUFFER_SIZE], *buf = mem;
  int len;
  va_list ap;

  va_start(ap, fmt);
  len = mg_avprintf(&buf, sizeof(mem), fmt, ap);
  va_end(ap);

  if (len >= 0) {
    mg_send_http_chunk(nc, buf, len);
  }

  /* LCOV_EXCL_START */
  if (buf != mem && buf != NULL) {
    MG_FREE(buf);
  }
  /* LCOV_EXCL_STOP */
}

void mg_printf_html_escape(struct mg_connection *nc, const char *fmt, ...) {
  char mem[MG_VPRINTF_BUFFER_SIZE], *buf = mem;
  int i, j, len;
  va_list ap;

  va_start(ap, fmt);
  len = mg_avprintf(&buf, sizeof(mem), fmt, ap);
  va_end(ap);

  if (len >= 0) {
    for (i = j = 0; i < len; i++) {
      if (buf[i] == '<' || buf[i] == '>') {
        mg_send(nc, buf + j, i - j);
        mg_send(nc, buf[i] == '<' ? "&lt;" : "&gt;", 4);
        j = i + 1;
      }
    }
    mg_send(nc, buf + j, i - j);
  }

  /* LCOV_EXCL_START */
  if (buf != mem && buf != NULL) {
    MG_FREE(buf);
  }
  /* LCOV_EXCL_STOP */
}

int mg_http_parse_header(struct mg_str *hdr, const char *var_name, char *buf,
                         size_t buf_size) {
  int ch = ' ', ch1 = ',', len = 0, n = strlen(var_name);
  const char *p, *end = hdr ? hdr->p + hdr->len : NULL, *s = NULL;

  if (buf != NULL && buf_size > 0) buf[0] = '\0';
  if (hdr == NULL) return 0;

  /* Find where variable starts */
  for (s = hdr->p; s != NULL && s + n < end; s++) {
    if ((s == hdr->p || s[-1] == ch || s[-1] == ch1) && s[n] == '=' &&
        !memcmp(s, var_name, n))
      break;
  }

  if (s != NULL && &s[n + 1] < end) {
    s += n + 1;
    if (*s == '"' || *s == '\'') {
      ch = ch1 = *s++;
    }
    p = s;
    while (p < end && p[0] != ch && p[0] != ch1 && len < (int) buf_size) {
      if (ch != ' ' && p[0] == '\\' && p[1] == ch) p++;
      buf[len++] = *p++;
    }
    if (len >= (int) buf_size || (ch != ' ' && *p != ch)) {
      len = 0;
    } else {
      if (len > 0 && s[len - 1] == ',') len--;
      if (len > 0 && s[len - 1] == ';') len--;
      buf[len] = '\0';
    }
  }

  return len;
}

#ifndef MG_DISABLE_FILESYSTEM
static int mg_is_file_hidden(const char *path,
                             const struct mg_serve_http_opts *opts,
                             int exclude_specials) {
  const char *p1 = opts->per_directory_auth_file;
  const char *p2 = opts->hidden_file_pattern;

  /* Strip directory path from the file name */
  const char *pdir = strrchr(path, DIRSEP);
  if (pdir != NULL) {
    path = pdir + 1;
  }

  return (exclude_specials && (!strcmp(path, ".") || !strcmp(path, ".."))) ||
         (p1 != NULL &&
          mg_match_prefix(p1, strlen(p1), path) == (int) strlen(p1)) ||
         (p2 != NULL && mg_match_prefix(p2, strlen(p2), path) > 0);
}

#ifndef MG_DISABLE_HTTP_DIGEST_AUTH
static void mg_mkmd5resp(const char *method, size_t method_len, const char *uri,
                         size_t uri_len, const char *ha1, size_t ha1_len,
                         const char *nonce, size_t nonce_len, const char *nc,
                         size_t nc_len, const char *cnonce, size_t cnonce_len,
                         const char *qop, size_t qop_len, char *resp) {
  static const char colon[] = ":";
  static const size_t one = 1;
  char ha2[33];

  cs_md5(ha2, method, method_len, colon, one, uri, uri_len, NULL);
  cs_md5(resp, ha1, ha1_len, colon, one, nonce, nonce_len, colon, one, nc,
         nc_len, colon, one, cnonce, cnonce_len, colon, one, qop, qop_len,
         colon, one, ha2, sizeof(ha2) - 1, NULL);
}

int mg_http_create_digest_auth_header(char *buf, size_t buf_len,
                                      const char *method, const char *uri,
                                      const char *auth_domain, const char *user,
                                      const char *passwd) {
  static const char colon[] = ":", qop[] = "auth";
  static const size_t one = 1;
  char ha1[33], resp[33], cnonce[40];

  snprintf(cnonce, sizeof(cnonce), "%x", (unsigned int) time(NULL));
  cs_md5(ha1, user, (size_t) strlen(user), colon, one, auth_domain,
         (size_t) strlen(auth_domain), colon, one, passwd,
         (size_t) strlen(passwd), NULL);
  mg_mkmd5resp(method, strlen(method), uri, strlen(uri), ha1, sizeof(ha1) - 1,
               cnonce, strlen(cnonce), "1", one, cnonce, strlen(cnonce), qop,
               sizeof(qop) - 1, resp);
  return snprintf(buf, buf_len,
                  "Authorization: Digest username=\"%s\","
                  "realm=\"%s\",uri=\"%s\",qop=%s,nc=1,cnonce=%s,"
                  "nonce=%s,response=%s\r\n",
                  user, auth_domain, uri, qop, cnonce, cnonce, resp);
}

/*
 * Check for authentication timeout.
 * Clients send time stamp encoded in nonce. Make sure it is not too old,
 * to prevent replay attacks.
 * Assumption: nonce is a hexadecimal number of seconds since 1970.
 */
static int mg_check_nonce(const char *nonce) {
  unsigned long now = (unsigned long) time(NULL);
  unsigned long val = (unsigned long) strtoul(nonce, NULL, 16);
  return now < val || now - val < 3600;
}

int mg_http_check_digest_auth(struct http_message *hm, const char *auth_domain,
                              FILE *fp) {
  struct mg_str *hdr;
  char buf[128], f_user[sizeof(buf)], f_ha1[sizeof(buf)], f_domain[sizeof(buf)];
  char user[50], cnonce[33], response[40], uri[200], qop[20], nc[20], nonce[30];
  char expected_response[33];

  /* Parse "Authorization:" header, fail fast on parse error */
  if (hm == NULL || fp == NULL ||
      (hdr = mg_get_http_header(hm, "Authorization")) == NULL ||
      mg_http_parse_header(hdr, "username", user, sizeof(user)) == 0 ||
      mg_http_parse_header(hdr, "cnonce", cnonce, sizeof(cnonce)) == 0 ||
      mg_http_parse_header(hdr, "response", response, sizeof(response)) == 0 ||
      mg_http_parse_header(hdr, "uri", uri, sizeof(uri)) == 0 ||
      mg_http_parse_header(hdr, "qop", qop, sizeof(qop)) == 0 ||
      mg_http_parse_header(hdr, "nc", nc, sizeof(nc)) == 0 ||
      mg_http_parse_header(hdr, "nonce", nonce, sizeof(nonce)) == 0 ||
      mg_check_nonce(nonce) == 0) {
    return 0;
  }

  /*
   * Read passwords file line by line. If should have htdigest format,
   * i.e. each line should be a colon-separated sequence:
   * USER_NAME:DOMAIN_NAME:HA1_HASH_OF_USER_DOMAIN_AND_PASSWORD
   */
  while (fgets(buf, sizeof(buf), fp) != NULL) {
    if (sscanf(buf, "%[^:]:%[^:]:%s", f_user, f_domain, f_ha1) == 3 &&
        strcmp(user, f_user) == 0 &&
        /* NOTE(lsm): due to a bug in MSIE, we do not compare URIs */
        strcmp(auth_domain, f_domain) == 0) {
      /* User and domain matched, check the password */
      mg_mkmd5resp(
          hm->method.p, hm->method.len, hm->uri.p,
          hm->uri.len + (hm->query_string.len ? hm->query_string.len + 1 : 0),
          f_ha1, strlen(f_ha1), nonce, strlen(nonce), nc, strlen(nc), cnonce,
          strlen(cnonce), qop, strlen(qop), expected_response);
      return mg_casecmp(response, expected_response) == 0;
    }
  }

  /* None of the entries in the passwords file matched - return failure */
  return 0;
}

static int mg_is_authorized(struct http_message *hm, const char *path,
                            int is_directory, const char *domain,
                            const char *passwords_file,
                            int is_global_pass_file) {
  char buf[MG_MAX_PATH];
  const char *p;
  FILE *fp;
  int authorized = 1;

  if (domain != NULL && passwords_file != NULL) {
    if (is_global_pass_file) {
      fp = fopen(passwords_file, "r");
    } else if (is_directory) {
      snprintf(buf, sizeof(buf), "%s%c%s", path, DIRSEP, passwords_file);
      fp = fopen(buf, "r");
    } else {
      p = strrchr(path, DIRSEP);
      if (p == NULL) p = path;
      snprintf(buf, sizeof(buf), "%.*s%c%s", (int) (p - path), path, DIRSEP,
               passwords_file);
      fp = fopen(buf, "r");
    }

    if (fp != NULL) {
      authorized = mg_http_check_digest_auth(hm, domain, fp);
      fclose(fp);
    }
  }

  DBG(("%s %s %d %d", path, passwords_file ? passwords_file : "",
       is_global_pass_file, authorized));
  return authorized;
}
#else
static int mg_is_authorized(struct http_message *hm, const char *path,
                            int is_directory, const char *domain,
                            const char *passwords_file,
                            int is_global_pass_file) {
  (void) hm;
  (void) path;
  (void) is_directory;
  (void) domain;
  (void) passwords_file;
  (void) is_global_pass_file;
  return 1;
}
#endif

#ifndef MG_DISABLE_DIRECTORY_LISTING
static size_t mg_url_encode(const char *src, size_t s_len, char *dst,
                            size_t dst_len) {
  static const char *dont_escape = "._-$,;~()/";
  static const char *hex = "0123456789abcdef";
  size_t i = 0, j = 0;

  for (i = j = 0; dst_len > 0 && i < s_len && j + 2 < dst_len - 1; i++, j++) {
    if (isalnum(*(const unsigned char *) (src + i)) ||
        strchr(dont_escape, *(const unsigned char *) (src + i)) != NULL) {
      dst[j] = src[i];
    } else if (j + 3 < dst_len) {
      dst[j] = '%';
      dst[j + 1] = hex[(*(const unsigned char *) (src + i)) >> 4];
      dst[j + 2] = hex[(*(const unsigned char *) (src + i)) & 0xf];
      j += 2;
    }
  }

  dst[j] = '\0';
  return j;
}

static void mg_escape(const char *src, char *dst, size_t dst_len) {
  size_t n = 0;
  while (*src != '\0' && n + 5 < dst_len) {
    unsigned char ch = *(unsigned char *) src++;
    if (ch == '<') {
      n += snprintf(dst + n, dst_len - n, "%s", "&lt;");
    } else {
      dst[n++] = ch;
    }
  }
  dst[n] = '\0';
}

static void mg_print_dir_entry(struct mg_connection *nc, const char *file_name,
                               cs_stat_t *stp) {
  char size[64], mod[64], href[MAX_PATH_SIZE * 3], path[MAX_PATH_SIZE];
  int64_t fsize = stp->st_size;
  int is_dir = S_ISDIR(stp->st_mode);
  const char *slash = is_dir ? "/" : "";

  if (is_dir) {
    snprintf(size, sizeof(size), "%s", "[DIRECTORY]");
  } else {
    /*
     * We use (double) cast below because MSVC 6 compiler cannot
     * convert unsigned __int64 to double.
     */
    if (fsize < 1024) {
      snprintf(size, sizeof(size), "%d", (int) fsize);
    } else if (fsize < 0x100000) {
      snprintf(size, sizeof(size), "%.1fk", (double) fsize / 1024.0);
    } else if (fsize < 0x40000000) {
      snprintf(size, sizeof(size), "%.1fM", (double) fsize / 1048576);
    } else {
      snprintf(size, sizeof(size), "%.1fG", (double) fsize / 1073741824);
    }
  }
  strftime(mod, sizeof(mod), "%d-%b-%Y %H:%M", localtime(&stp->st_mtime));
  mg_escape(file_name, path, sizeof(path));
  mg_url_encode(file_name, strlen(file_name), href, sizeof(href));
  mg_printf_http_chunk(nc,
                       "<tr><td><a href=\"%s%s\">%s%s</a></td>"
                       "<td>%s</td><td name=%" INT64_FMT ">%s</td></tr>\n",
                       href, slash, path, slash, mod, is_dir ? -1 : fsize,
                       size);
}

static void mg_scan_directory(struct mg_connection *nc, const char *dir,
                              const struct mg_serve_http_opts *opts,
                              void (*func)(struct mg_connection *, const char *,
                                           cs_stat_t *)) {
  char path[MAX_PATH_SIZE];
  cs_stat_t st;
  struct dirent *dp;
  DIR *dirp;

  DBG(("%p [%s]", nc, dir));
  if ((dirp = (opendir(dir))) != NULL) {
    while ((dp = readdir(dirp)) != NULL) {
      /* Do not show current dir and hidden files */
      if (mg_is_file_hidden((const char *) dp->d_name, opts, 1)) {
        continue;
      }
      snprintf(path, sizeof(path), "%s/%s", dir, dp->d_name);
      if (mg_stat(path, &st) == 0) {
        func(nc, (const char *) dp->d_name, &st);
      }
    }
    closedir(dirp);
  } else {
    DBG(("%p opendir(%s) -> %d", nc, dir, errno));
  }
}

static void mg_send_directory_listing(struct mg_connection *nc, const char *dir,
                                      struct http_message *hm,
                                      struct mg_serve_http_opts *opts) {
  static const char *sort_js_code =
      "<script>function srt(tb, sc, so, d) {"
      "var tr = Array.prototype.slice.call(tb.rows, 0),"
      "tr = tr.sort(function (a, b) { var c1 = a.cells[sc], c2 = b.cells[sc],"
      "n1 = c1.getAttribute('name'), n2 = c2.getAttribute('name'), "
      "t1 = a.cells[2].getAttribute('name'), "
      "t2 = b.cells[2].getAttribute('name'); "
      "return so * (t1 < 0 && t2 >= 0 ? -1 : t2 < 0 && t1 >= 0 ? 1 : "
      "n1 ? parseInt(n2) - parseInt(n1) : "
      "c1.textContent.trim().localeCompare(c2.textContent.trim())); });";
  static const char *sort_js_code2 =
      "for (var i = 0; i < tr.length; i++) tb.appendChild(tr[i]); "
      "if (!d) window.location.hash = ('sc=' + sc + '&so=' + so); "
      "};"
      "window.onload = function() {"
      "var tb = document.getElementById('tb');"
      "var m = /sc=([012]).so=(1|-1)/.exec(window.location.hash) || [0, 2, 1];"
      "var sc = m[1], so = m[2]; document.onclick = function(ev) { "
      "var c = ev.target.rel; if (c) {if (c == sc) so *= -1; srt(tb, c, so); "
      "sc = c; ev.preventDefault();}};"
      "srt(tb, sc, so, true);"
      "}"
      "</script>";

  mg_send_response_line(nc, 200, opts->extra_headers);
  mg_printf(nc, "%s: %s\r\n%s: %s\r\n\r\n", "Transfer-Encoding", "chunked",
            "Content-Type", "text/html; charset=utf-8");

  mg_printf_http_chunk(
      nc,
      "<html><head><title>Index of %.*s</title>%s%s"
      "<style>th,td {text-align: left; padding-right: 1em; "
      "font-family: monospace; }</style></head>\n"
      "<body><h1>Index of %.*s</h1>\n<table cellpadding=0><thead>"
      "<tr><th><a href=# rel=0>Name</a></th><th>"
      "<a href=# rel=1>Modified</a</th>"
      "<th><a href=# rel=2>Size</a></th></tr>"
      "<tr><td colspan=3><hr></td></tr>\n"
      "</thead>\n"
      "<tbody id=tb>",
      (int) hm->uri.len, hm->uri.p, sort_js_code, sort_js_code2,
      (int) hm->uri.len, hm->uri.p);
  mg_scan_directory(nc, dir, opts, mg_print_dir_entry);
  mg_printf_http_chunk(nc,
                       "</tbody><tr><td colspan=3><hr></td></tr>\n"
                       "</table>\n"
                       "<address>%s</address>\n"
                       "</body></html>",
                       mg_version_header);
  mg_send_http_chunk(nc, "", 0);
  /* TODO(rojer): Remove when cesanta/dev/issues/197 is fixed. */
  nc->flags |= MG_F_SEND_AND_CLOSE;
}
#endif /* MG_DISABLE_DIRECTORY_LISTING */

#ifndef MG_DISABLE_DAV
static void mg_print_props(struct mg_connection *nc, const char *name,
                           cs_stat_t *stp) {
  char mtime[64], buf[MAX_PATH_SIZE * 3];
  time_t t = stp->st_mtime; /* store in local variable for NDK compile */
  mg_gmt_time_string(mtime, sizeof(mtime), &t);
  mg_url_encode(name, strlen(name), buf, sizeof(buf));
  mg_printf(nc,
            "<d:response>"
            "<d:href>%s</d:href>"
            "<d:propstat>"
            "<d:prop>"
            "<d:resourcetype>%s</d:resourcetype>"
            "<d:getcontentlength>%" INT64_FMT
            "</d:getcontentlength>"
            "<d:getlastmodified>%s</d:getlastmodified>"
            "</d:prop>"
            "<d:status>HTTP/1.1 200 OK</d:status>"
            "</d:propstat>"
            "</d:response>\n",
            buf, S_ISDIR(stp->st_mode) ? "<d:collection/>" : "",
            (int64_t) stp->st_size, mtime);
}

static void mg_handle_propfind(struct mg_connection *nc, const char *path,
                               cs_stat_t *stp, struct http_message *hm,
                               struct mg_serve_http_opts *opts) {
  static const char header[] =
      "HTTP/1.1 207 Multi-Status\r\n"
      "Connection: close\r\n"
      "Content-Type: text/xml; charset=utf-8\r\n\r\n"
      "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
      "<d:multistatus xmlns:d='DAV:'>\n";
  static const char footer[] = "</d:multistatus>\n";
  const struct mg_str *depth = mg_get_http_header(hm, "Depth");

  /* Print properties for the requested resource itself */
  if (S_ISDIR(stp->st_mode) &&
      strcmp(opts->enable_directory_listing, "yes") != 0) {
    mg_printf(nc, "%s", "HTTP/1.1 403 Directory Listing Denied\r\n\r\n");
  } else {
    char uri[MAX_PATH_SIZE];
    mg_send(nc, header, sizeof(header) - 1);
    snprintf(uri, sizeof(uri), "%.*s", (int) hm->uri.len, hm->uri.p);
    mg_print_props(nc, uri, stp);
    if (S_ISDIR(stp->st_mode) && (depth == NULL || mg_vcmp(depth, "0") != 0)) {
      mg_scan_directory(nc, path, opts, mg_print_props);
    }
    mg_send(nc, footer, sizeof(footer) - 1);
    nc->flags |= MG_F_SEND_AND_CLOSE;
  }
}

#ifdef MG_ENABLE_FAKE_DAVLOCK
/*
 * Windows explorer (probably there are another WebDav clients like it)
 * requires LOCK support in webdav. W/out this, it still works, but fails
 * to save file: shows error message and offers "Save As".
 * "Save as" works, but this message is very annoying.
 * This is fake lock, which doesn't lock something, just returns LOCK token,
 * UNLOCK always answers "OK".
 * With this fake LOCK Windows Explorer looks happy and saves file.
 * NOTE: that is not DAV LOCK imlementation, it is just a way to shut up
 * Windows native DAV client. This is why FAKE LOCK is not enabed by default
 */
static void mg_handle_lock(struct mg_connection *nc, const char *path) {
  static const char *reply =
      "HTTP/1.1 207 Multi-Status\r\n"
      "Connection: close\r\n"
      "Content-Type: text/xml; charset=utf-8\r\n\r\n"
      "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
      "<d:multistatus xmlns:d='DAV:'>\n"
      "<D:lockdiscovery>\n"
      "<D:activelock>\n"
      "<D:locktoken>\n"
      "<D:href>\n"
      "opaquelocktoken:%s%u"
      "</D:href>"
      "</D:locktoken>"
      "</D:activelock>\n"
      "</D:lockdiscovery>"
      "</d:multistatus>\n";
  mg_printf(nc, reply, path, (unsigned int) time(NULL));
  nc->flags |= MG_F_SEND_AND_CLOSE;
}
#endif

static void mg_handle_mkcol(struct mg_connection *nc, const char *path,
                            struct http_message *hm) {
  int status_code = 500;
  if (hm->body.len != (size_t) ~0 && hm->body.len > 0) {
    status_code = 415;
  } else if (!mg_mkdir(path, 0755)) {
    status_code = 201;
  } else if (errno == EEXIST) {
    status_code = 405;
  } else if (errno == EACCES) {
    status_code = 403;
  } else if (errno == ENOENT) {
    status_code = 409;
  } else {
    status_code = 500;
  }
  mg_http_send_error(nc, status_code, NULL);
}

static int mg_remove_directory(const struct mg_serve_http_opts *opts,
                               const char *dir) {
  char path[MAX_PATH_SIZE];
  struct dirent *dp;
  cs_stat_t st;
  DIR *dirp;

  if ((dirp = opendir(dir)) == NULL) return 0;

  while ((dp = readdir(dirp)) != NULL) {
    if (mg_is_file_hidden((const char *) dp->d_name, opts, 1)) {
      continue;
    }
    snprintf(path, sizeof(path), "%s%c%s", dir, '/', dp->d_name);
    mg_stat(path, &st);
    if (S_ISDIR(st.st_mode)) {
      mg_remove_directory(opts, path);
    } else {
      remove(path);
    }
  }
  closedir(dirp);
  rmdir(dir);

  return 1;
}

static void mg_handle_move(struct mg_connection *c,
                           const struct mg_serve_http_opts *opts,
                           const char *path, struct http_message *hm) {
  const struct mg_str *dest = mg_get_http_header(hm, "Destination");
  if (dest == NULL) {
    mg_http_send_error(c, 411, NULL);
  } else {
    const char *p = (char *) memchr(dest->p, '/', dest->len);
    if (p != NULL && p[1] == '/' &&
        (p = (char *) memchr(p + 2, '/', dest->p + dest->len - p)) != NULL) {
      char buf[MAX_PATH_SIZE];
      snprintf(buf, sizeof(buf), "%s%.*s", opts->dav_document_root,
               (int) (dest->p + dest->len - p), p);
      if (rename(path, buf) == 0) {
        mg_http_send_error(c, 200, NULL);
      } else {
        mg_http_send_error(c, 418, NULL);
      }
    } else {
      mg_http_send_error(c, 500, NULL);
    }
  }
}

static void mg_handle_delete(struct mg_connection *nc,
                             const struct mg_serve_http_opts *opts,
                             const char *path) {
  cs_stat_t st;
  if (mg_stat(path, &st) != 0) {
    mg_http_send_error(nc, 404, NULL);
  } else if (S_ISDIR(st.st_mode)) {
    mg_remove_directory(opts, path);
    mg_http_send_error(nc, 204, NULL);
  } else if (remove(path) == 0) {
    mg_http_send_error(nc, 204, NULL);
  } else {
    mg_http_send_error(nc, 423, NULL);
  }
}

/* Return -1 on error, 1 on success. */
static int mg_create_itermediate_directories(const char *path) {
  const char *s;

  /* Create intermediate directories if they do not exist */
  for (s = path + 1; *s != '\0'; s++) {
    if (*s == '/') {
      char buf[MAX_PATH_SIZE];
      cs_stat_t st;
      snprintf(buf, sizeof(buf), "%.*s", (int) (s - path), path);
      buf[sizeof(buf) - 1] = '\0';
      if (mg_stat(buf, &st) != 0 && mg_mkdir(buf, 0755) != 0) {
        return -1;
      }
    }
  }

  return 1;
}

static void mg_handle_put(struct mg_connection *nc, const char *path,
                          struct http_message *hm) {
  struct mg_http_proto_data *pd = mg_http_get_proto_data(nc);
  cs_stat_t st;
  const struct mg_str *cl_hdr = mg_get_http_header(hm, "Content-Length");
  int rc, status_code = mg_stat(path, &st) == 0 ? 200 : 201;

  mg_http_free_proto_data_file(&pd->file);
  if ((rc = mg_create_itermediate_directories(path)) == 0) {
    mg_printf(nc, "HTTP/1.1 %d OK\r\nContent-Length: 0\r\n\r\n", status_code);
  } else if (rc == -1) {
    mg_http_send_error(nc, 500, NULL);
  } else if (cl_hdr == NULL) {
    mg_http_send_error(nc, 411, NULL);
  } else if ((pd->file.fp = fopen(path, "w+b")) == NULL) {
    mg_http_send_error(nc, 500, NULL);
  } else {
    const struct mg_str *range_hdr = mg_get_http_header(hm, "Content-Range");
    int64_t r1 = 0, r2 = 0;
    pd->file.type = DATA_PUT;
    mg_set_close_on_exec(fileno(pd->file.fp));
    pd->file.cl = to64(cl_hdr->p);
    if (range_hdr != NULL &&
        mg_http_parse_range_header(range_hdr, &r1, &r2) > 0) {
      status_code = 206;
      fseeko(pd->file.fp, r1, SEEK_SET);
      pd->file.cl = r2 > r1 ? r2 - r1 + 1 : pd->file.cl - r1;
    }
    mg_printf(nc, "HTTP/1.1 %d OK\r\nContent-Length: 0\r\n\r\n", status_code);
    /* Remove HTTP request from the mbuf, leave only payload */
    mbuf_remove(&nc->recv_mbuf, hm->message.len - hm->body.len);
    mg_http_transfer_file_data(nc);
  }
}
#endif /* MG_DISABLE_DAV */

static int mg_is_dav_request(const struct mg_str *s) {
  static const char *methods[] = {"PUT", "DELETE", "MKCOL", "PROPFIND", "MOVE"
#ifdef MG_ENABLE_FAKE_DAVLOCK
                                  ,
                                  "LOCK", "UNLOCK"
#endif
  };
  size_t i;

  for (i = 0; i < ARRAY_SIZE(methods); i++) {
    if (mg_vcmp(s, methods[i]) == 0) {
      return 1;
    }
  }

  return 0;
}

/*
 * Given a directory path, find one of the files specified in the
 * comma-separated list of index files `list`.
 * First found index file wins. If an index file is found, then gets
 * appended to the `path`, stat-ed, and result of `stat()` passed to `stp`.
 * If index file is not found, then `path` and `stp` remain unchanged.
 */
MG_INTERNAL void mg_find_index_file(const char *path, const char *list,
                                    char **index_file, cs_stat_t *stp) {
  struct mg_str vec;
  size_t path_len = strlen(path);
  int found = 0;
  *index_file = NULL;

  /* Traverse index files list. For each entry, append it to the given */
  /* path and see if the file exists. If it exists, break the loop */
  while ((list = mg_next_comma_list_entry(list, &vec, NULL)) != NULL) {
    cs_stat_t st;
    size_t len = path_len + 1 + vec.len + 1;
    *index_file = (char *) MG_REALLOC(*index_file, len);
    if (*index_file == NULL) break;
    snprintf(*index_file, len, "%s%c%.*s", path, DIRSEP, (int) vec.len, vec.p);

    /* Does it exist? Is it a file? */
    if (mg_stat(*index_file, &st) == 0 && S_ISREG(st.st_mode)) {
      /* Yes it does, break the loop */
      *stp = st;
      found = 1;
      break;
    }
  }
  if (!found) {
    MG_FREE(*index_file);
    *index_file = NULL;
  }
  DBG(("[%s] [%s]", path, (*index_file ? *index_file : "")));
}

static int mg_http_send_port_based_redirect(
    struct mg_connection *c, struct http_message *hm,
    const struct mg_serve_http_opts *opts) {
  const char *rewrites = opts->url_rewrites;
  struct mg_str a, b;
  char local_port[20] = {'%'};

  mg_conn_addr_to_str(c, local_port + 1, sizeof(local_port) - 1,
                      MG_SOCK_STRINGIFY_PORT);

  while ((rewrites = mg_next_comma_list_entry(rewrites, &a, &b)) != NULL) {
    if (mg_vcmp(&a, local_port) == 0) {
      mg_send_response_line(c, 301, NULL);
      mg_printf(c, "Content-Length: 0\r\nLocation: %.*s%.*s\r\n\r\n",
                (int) b.len, b.p, (int) (hm->proto.p - hm->uri.p - 1),
                hm->uri.p);
      return 1;
    }
  }

  return 0;
}

MG_INTERNAL int mg_uri_to_local_path(struct http_message *hm,
                                     const struct mg_serve_http_opts *opts,
                                     char **local_path,
                                     struct mg_str *remainder) {
  int ok = 1;
  const char *cp = hm->uri.p, *cp_end = hm->uri.p + hm->uri.len;
  struct mg_str root = {NULL, 0};
  const char *file_uri_start = cp;
  *local_path = NULL;
  remainder->p = NULL;
  remainder->len = 0;

  { /* 1. Determine which root to use. */
    const char *rewrites = opts->url_rewrites;
    struct mg_str *hh = mg_get_http_header(hm, "Host");
    struct mg_str a, b;
    /* Check rewrites first. */
    while ((rewrites = mg_next_comma_list_entry(rewrites, &a, &b)) != NULL) {
      if (a.len > 1 && a.p[0] == '@') {
        /* Host rewrite. */
        if (hh != NULL && hh->len == a.len - 1 &&
            mg_ncasecmp(a.p + 1, hh->p, a.len - 1) == 0) {
          root = b;
          break;
        }
      } else {
        /* Regular rewrite, URI=directory */
        int match_len = mg_match_prefix_n(a, hm->uri);
        if (match_len > 0) {
          file_uri_start = hm->uri.p + match_len;
          if (*file_uri_start == '/' || file_uri_start == cp_end) {
            /* Match ended at component boundary, ok. */
          } else if (*(file_uri_start - 1) == '/') {
            /* Pattern ends with '/', backtrack. */
            file_uri_start--;
          } else {
            /* No match: must fall on the component boundary. */
            continue;
          }
          root = b;
          break;
        }
      }
    }
    /* If no rewrite rules matched, use DAV or regular document root. */
    if (root.p == NULL) {
#ifndef MG_DISABLE_DAV
      if (opts->dav_document_root != NULL && mg_is_dav_request(&hm->method)) {
        root.p = opts->dav_document_root;
        root.len = strlen(opts->dav_document_root);
      } else
#endif
      {
        root.p = opts->document_root;
        root.len = strlen(opts->document_root);
      }
    }
    assert(root.p != NULL && root.len > 0);
  }

  { /* 2. Find where in the canonical URI path the local path ends. */
    const char *u = file_uri_start + 1;
    char *lp = (char *) MG_MALLOC(root.len + hm->uri.len + 1);
    char *lp_end = lp + root.len + hm->uri.len + 1;
    char *p = lp, *ps;
    int exists = 1;
    if (lp == NULL) {
      ok = 0;
      goto out;
    }
    memcpy(p, root.p, root.len);
    p += root.len;
    if (*(p - 1) == DIRSEP) p--;
    *p = '\0';
    ps = p;

    /* Chop off URI path components one by one and build local path. */
    while (u <= cp_end) {
      const char *next = u;
      struct mg_str component;
      if (exists) {
        cs_stat_t st;
        exists = (mg_stat(lp, &st) == 0);
        if (exists && S_ISREG(st.st_mode)) {
          /* We found the terminal, the rest of the URI (if any) is path_info.
           */
          if (*(u - 1) == '/') u--;
          break;
        }
      }
      if (u >= cp_end) break;
      parse_uri_component((const char **) &next, cp_end, '/', &component);
      if (component.len > 0) {
        int len;
        memmove(p + 1, component.p, component.len);
        len = mg_url_decode(p + 1, component.len, p + 1, lp_end - p - 1, 0);
        if (len <= 0) {
          ok = 0;
          break;
        }
        component.p = p + 1;
        component.len = len;
        if (mg_vcmp(&component, ".") == 0) {
          /* Yum. */
        } else if (mg_vcmp(&component, "..") == 0) {
          while (p > ps && *p != DIRSEP) p--;
          *p = '\0';
        } else {
          size_t i;
#ifdef _WIN32
          /* On Windows, make sure it's valid Unicode (no funny stuff). */
          wchar_t buf[MG_MAX_PATH * 2];
          if (to_wchar(component.p, buf, MG_MAX_PATH) == 0) {
            DBG(("[%.*s] smells funny", (int) component.len, component.p));
            ok = 0;
            break;
          }
#endif
          *p++ = DIRSEP;
          /* No NULs and DIRSEPs in the component (percent-encoded). */
          for (i = 0; i < component.len; i++, p++) {
            if (*p == '\0' || *p == DIRSEP
#ifdef _WIN32
                /* On Windows, "/" is also accepted, so check for that too. */
                ||
                *p == '/'
#endif
                ) {
              ok = 0;
              break;
            }
          }
        }
      }
      u = next;
    }
    if (ok) {
      *local_path = lp;
      remainder->p = u;
      remainder->len = cp_end - u;
    } else {
      MG_FREE(lp);
    }
  }

out:
  DBG(("'%.*s' -> '%s' + '%.*s'", (int) hm->uri.len, hm->uri.p,
       *local_path ? *local_path : "", (int) remainder->len, remainder->p));
  return ok;
}

#ifndef MG_DISABLE_CGI
#ifdef _WIN32
struct mg_threadparam {
  sock_t s;
  HANDLE hPipe;
};

static int mg_wait_until_ready(sock_t sock, int for_read) {
  fd_set set;
  FD_ZERO(&set);
  FD_SET(sock, &set);
  return select(sock + 1, for_read ? &set : 0, for_read ? 0 : &set, 0, 0) == 1;
}

static void *mg_push_to_stdin(void *arg) {
  struct mg_threadparam *tp = (struct mg_threadparam *) arg;
  int n, sent, stop = 0;
  DWORD k;
  char buf[BUFSIZ];

  while (!stop && mg_wait_until_ready(tp->s, 1) &&
         (n = recv(tp->s, buf, sizeof(buf), 0)) > 0) {
    if (n == -1 && GetLastError() == WSAEWOULDBLOCK) continue;
    for (sent = 0; !stop && sent < n; sent += k) {
      if (!WriteFile(tp->hPipe, buf + sent, n - sent, &k, 0)) stop = 1;
    }
  }
  DBG(("%s", "FORWARED EVERYTHING TO CGI"));
  CloseHandle(tp->hPipe);
  MG_FREE(tp);
  _endthread();
  return NULL;
}

static void *mg_pull_from_stdout(void *arg) {
  struct mg_threadparam *tp = (struct mg_threadparam *) arg;
  int k = 0, stop = 0;
  DWORD n, sent;
  char buf[BUFSIZ];

  while (!stop && ReadFile(tp->hPipe, buf, sizeof(buf), &n, NULL)) {
    for (sent = 0; !stop && sent < n; sent += k) {
      if (mg_wait_until_ready(tp->s, 0) &&
          (k = send(tp->s, buf + sent, n - sent, 0)) <= 0)
        stop = 1;
    }
  }
  DBG(("%s", "EOF FROM CGI"));
  CloseHandle(tp->hPipe);
  shutdown(tp->s, 2);  // Without this, IO thread may get truncated data
  closesocket(tp->s);
  MG_FREE(tp);
  _endthread();
  return NULL;
}

static void mg_spawn_stdio_thread(sock_t sock, HANDLE hPipe,
                                  void *(*func)(void *)) {
  struct mg_threadparam *tp = (struct mg_threadparam *) MG_MALLOC(sizeof(*tp));
  if (tp != NULL) {
    tp->s = sock;
    tp->hPipe = hPipe;
    mg_start_thread(func, tp);
  }
}

static void mg_abs_path(const char *utf8_path, char *abs_path, size_t len) {
  wchar_t buf[MAX_PATH_SIZE], buf2[MAX_PATH_SIZE];
  to_wchar(utf8_path, buf, ARRAY_SIZE(buf));
  GetFullPathNameW(buf, ARRAY_SIZE(buf2), buf2, NULL);
  WideCharToMultiByte(CP_UTF8, 0, buf2, wcslen(buf2) + 1, abs_path, len, 0, 0);
}

static pid_t mg_start_process(const char *interp, const char *cmd,
                              const char *env, const char *envp[],
                              const char *dir, sock_t sock) {
  STARTUPINFOW si;
  PROCESS_INFORMATION pi;
  HANDLE a[2], b[2], me = GetCurrentProcess();
  wchar_t wcmd[MAX_PATH_SIZE], full_dir[MAX_PATH_SIZE];
  char buf[MAX_PATH_SIZE], buf2[MAX_PATH_SIZE], buf5[MAX_PATH_SIZE],
      buf4[MAX_PATH_SIZE], cmdline[MAX_PATH_SIZE];
  DWORD flags = DUPLICATE_CLOSE_SOURCE | DUPLICATE_SAME_ACCESS;
  FILE *fp;

  memset(&si, 0, sizeof(si));
  memset(&pi, 0, sizeof(pi));

  si.cb = sizeof(si);
  si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
  si.wShowWindow = SW_HIDE;
  si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

  CreatePipe(&a[0], &a[1], NULL, 0);
  CreatePipe(&b[0], &b[1], NULL, 0);
  DuplicateHandle(me, a[0], me, &si.hStdInput, 0, TRUE, flags);
  DuplicateHandle(me, b[1], me, &si.hStdOutput, 0, TRUE, flags);

  if (interp == NULL && (fp = fopen(cmd, "r")) != NULL) {
    buf[0] = buf[1] = '\0';
    fgets(buf, sizeof(buf), fp);
    buf[sizeof(buf) - 1] = '\0';
    if (buf[0] == '#' && buf[1] == '!') {
      interp = buf + 2;
      /* Trim leading spaces: https://github.com/cesanta/mongoose/issues/489 */
      while (*interp != '\0' && isspace(*(unsigned char *) interp)) {
        interp++;
      }
    }
    fclose(fp);
  }

  snprintf(buf, sizeof(buf), "%s/%s", dir, cmd);
  mg_abs_path(buf, buf2, ARRAY_SIZE(buf2));

  mg_abs_path(dir, buf5, ARRAY_SIZE(buf5));
  to_wchar(dir, full_dir, ARRAY_SIZE(full_dir));

  if (interp != NULL) {
    mg_abs_path(interp, buf4, ARRAY_SIZE(buf4));
    snprintf(cmdline, sizeof(cmdline), "%s \"%s\"", buf4, buf2);
  } else {
    snprintf(cmdline, sizeof(cmdline), "\"%s\"", buf2);
  }
  to_wchar(cmdline, wcmd, ARRAY_SIZE(wcmd));

  if (CreateProcessW(NULL, wcmd, NULL, NULL, TRUE, CREATE_NEW_PROCESS_GROUP,
                     (void *) env, full_dir, &si, &pi) != 0) {
    mg_spawn_stdio_thread(sock, a[1], mg_push_to_stdin);
    mg_spawn_stdio_thread(sock, b[0], mg_pull_from_stdout);
  } else {
    CloseHandle(a[1]);
    CloseHandle(b[0]);
    closesocket(sock);
  }
  DBG(("CGI command: [%ls] -> %p", wcmd, pi.hProcess));

  /* Not closing a[0] and b[1] because we've used DUPLICATE_CLOSE_SOURCE */
  CloseHandle(si.hStdOutput);
  CloseHandle(si.hStdInput);
  /* TODO(lsm): check if we need close process and thread handles too */
  /* CloseHandle(pi.hThread); */
  /* CloseHandle(pi.hProcess); */

  return pi.hProcess;
}
#else
static pid_t mg_start_process(const char *interp, const char *cmd,
                              const char *env, const char *envp[],
                              const char *dir, sock_t sock) {
  char buf[500];
  pid_t pid = fork();
  (void) env;

  if (pid == 0) {
    /*
     * In Linux `chdir` declared with `warn_unused_result` attribute
     * To shutup compiler we have yo use result in some way
     */
    int tmp = chdir(dir);
    (void) tmp;
    (void) dup2(sock, 0);
    (void) dup2(sock, 1);
    closesocket(sock);

    /*
     * After exec, all signal handlers are restored to their default values,
     * with one exception of SIGCHLD. According to POSIX.1-2001 and Linux's
     * implementation, SIGCHLD's handler will leave unchanged after exec
     * if it was set to be ignored. Restore it to default action.
     */
    signal(SIGCHLD, SIG_DFL);

    if (interp == NULL) {
      execle(cmd, cmd, (char *) 0, envp); /* (char *) 0 to squash warning */
    } else {
      execle(interp, interp, cmd, (char *) 0, envp);
    }
    snprintf(buf, sizeof(buf),
             "Status: 500\r\n\r\n"
             "500 Server Error: %s%s%s: %s",
             interp == NULL ? "" : interp, interp == NULL ? "" : " ", cmd,
             strerror(errno));
    send(1, buf, strlen(buf), 0);
    exit(EXIT_FAILURE); /* exec call failed */
  }

  return pid;
}
#endif /* _WIN32 */

/*
 * Append VARIABLE=VALUE\0 string to the buffer, and add a respective
 * pointer into the vars array.
 */
static char *mg_addenv(struct mg_cgi_env_block *block, const char *fmt, ...) {
  int n, space;
  char *added = block->buf + block->len;
  va_list ap;

  /* Calculate how much space is left in the buffer */
  space = sizeof(block->buf) - (block->len + 2);
  if (space > 0) {
    /* Copy VARIABLE=VALUE\0 string into the free space */
    va_start(ap, fmt);
    n = vsnprintf(added, (size_t) space, fmt, ap);
    va_end(ap);

    /* Make sure we do not overflow buffer and the envp array */
    if (n > 0 && n + 1 < space &&
        block->nvars < (int) ARRAY_SIZE(block->vars) - 2) {
      /* Append a pointer to the added string into the envp array */
      block->vars[block->nvars++] = added;
      /* Bump up used length counter. Include \0 terminator */
      block->len += n + 1;
    }
  }

  return added;
}

static void mg_addenv2(struct mg_cgi_env_block *blk, const char *name) {
  const char *s;
  if ((s = getenv(name)) != NULL) mg_addenv(blk, "%s=%s", name, s);
}

static void mg_prepare_cgi_environment(struct mg_connection *nc,
                                       const char *prog,
                                       const struct mg_str *path_info,
                                       const struct http_message *hm,
                                       const struct mg_serve_http_opts *opts,
                                       struct mg_cgi_env_block *blk) {
  const char *s;
  struct mg_str *h;
  char *p;
  size_t i;
  char buf[100];

  blk->len = blk->nvars = 0;
  blk->nc = nc;

  if ((s = getenv("SERVER_NAME")) != NULL) {
    mg_addenv(blk, "SERVER_NAME=%s", s);
  } else {
    mg_sock_to_str(nc->sock, buf, sizeof(buf), 3);
    mg_addenv(blk, "SERVER_NAME=%s", buf);
  }
  mg_addenv(blk, "SERVER_ROOT=%s", opts->document_root);
  mg_addenv(blk, "DOCUMENT_ROOT=%s", opts->document_root);
  mg_addenv(blk, "SERVER_SOFTWARE=%s/%s", "Mongoose", MG_VERSION);

  /* Prepare the environment block */
  mg_addenv(blk, "%s", "GATEWAY_INTERFACE=CGI/1.1");
  mg_addenv(blk, "%s", "SERVER_PROTOCOL=HTTP/1.1");
  mg_addenv(blk, "%s", "REDIRECT_STATUS=200"); /* For PHP */

  mg_addenv(blk, "REQUEST_METHOD=%.*s", (int) hm->method.len, hm->method.p);

  mg_addenv(blk, "REQUEST_URI=%.*s%s%.*s", (int) hm->uri.len, hm->uri.p,
            hm->query_string.len == 0 ? "" : "?", (int) hm->query_string.len,
            hm->query_string.p);

  mg_conn_addr_to_str(nc, buf, sizeof(buf),
                      MG_SOCK_STRINGIFY_REMOTE | MG_SOCK_STRINGIFY_IP);
  mg_addenv(blk, "REMOTE_ADDR=%s", buf);
  mg_conn_addr_to_str(nc, buf, sizeof(buf), MG_SOCK_STRINGIFY_PORT);
  mg_addenv(blk, "SERVER_PORT=%s", buf);

  s = hm->uri.p + hm->uri.len - path_info->len - 1;
  if (*s == '/') {
    const char *base_name = strrchr(prog, DIRSEP);
    mg_addenv(blk, "SCRIPT_NAME=%.*s/%s", (int) (s - hm->uri.p), hm->uri.p,
              (base_name != NULL ? base_name + 1 : prog));
  } else {
    mg_addenv(blk, "SCRIPT_NAME=%.*s", (int) (s - hm->uri.p + 1), hm->uri.p);
  }
  mg_addenv(blk, "SCRIPT_FILENAME=%s", prog);

  if (path_info != NULL && path_info->len > 0) {
    mg_addenv(blk, "PATH_INFO=%.*s", (int) path_info->len, path_info->p);
    /* Not really translated... */
    mg_addenv(blk, "PATH_TRANSLATED=%.*s", (int) path_info->len, path_info->p);
  }

#ifdef MG_ENABLE_SSL
  mg_addenv(blk, "HTTPS=%s", nc->ssl != NULL ? "on" : "off");
#else
  mg_addenv(blk, "HTTPS=off");
#endif

  if ((h = mg_get_http_header((struct http_message *) hm, "Content-Type")) !=
      NULL) {
    mg_addenv(blk, "CONTENT_TYPE=%.*s", (int) h->len, h->p);
  }

  if (hm->query_string.len > 0) {
    mg_addenv(blk, "QUERY_STRING=%.*s", (int) hm->query_string.len,
              hm->query_string.p);
  }

  if ((h = mg_get_http_header((struct http_message *) hm, "Content-Length")) !=
      NULL) {
    mg_addenv(blk, "CONTENT_LENGTH=%.*s", (int) h->len, h->p);
  }

  mg_addenv2(blk, "PATH");
  mg_addenv2(blk, "TMP");
  mg_addenv2(blk, "TEMP");
  mg_addenv2(blk, "TMPDIR");
  mg_addenv2(blk, "PERLLIB");
  mg_addenv2(blk, MG_ENV_EXPORT_TO_CGI);

#if defined(_WIN32)
  mg_addenv2(blk, "COMSPEC");
  mg_addenv2(blk, "SYSTEMROOT");
  mg_addenv2(blk, "SystemDrive");
  mg_addenv2(blk, "ProgramFiles");
  mg_addenv2(blk, "ProgramFiles(x86)");
  mg_addenv2(blk, "CommonProgramFiles(x86)");
#else
  mg_addenv2(blk, "LD_LIBRARY_PATH");
#endif /* _WIN32 */

  /* Add all headers as HTTP_* variables */
  for (i = 0; hm->header_names[i].len > 0; i++) {
    p = mg_addenv(blk, "HTTP_%.*s=%.*s", (int) hm->header_names[i].len,
                  hm->header_names[i].p, (int) hm->header_values[i].len,
                  hm->header_values[i].p);

    /* Convert variable name into uppercase, and change - to _ */
    for (; *p != '=' && *p != '\0'; p++) {
      if (*p == '-') *p = '_';
      *p = (char) toupper(*(unsigned char *) p);
    }
  }

  blk->vars[blk->nvars++] = NULL;
  blk->buf[blk->len++] = '\0';
}

static void mg_cgi_ev_handler(struct mg_connection *cgi_nc, int ev,
                              void *ev_data) {
  struct mg_connection *nc = (struct mg_connection *) cgi_nc->user_data;
  (void) ev_data;

  if (nc == NULL) return;

  switch (ev) {
    case MG_EV_RECV:
      /*
       * CGI script does not output reply line, like "HTTP/1.1 CODE XXXXX\n"
       * It outputs headers, then body. Headers might include "Status"
       * header, which changes CODE, and it might include "Location" header
       * which changes CODE to 302.
       *
       * Therefore we do not send the output from the CGI script to the user
       * until all CGI headers are received.
       *
       * Here we parse the output from the CGI script, and if all headers has
       * been received, send appropriate reply line, and forward all
       * received headers to the client.
       */
      if (nc->flags & MG_F_USER_1) {
        struct mbuf *io = &cgi_nc->recv_mbuf;
        int len = mg_http_get_request_len(io->buf, io->len);

        if (len == 0) break;
        if (len < 0 || io->len > MG_MAX_HTTP_REQUEST_SIZE) {
          cgi_nc->flags |= MG_F_CLOSE_IMMEDIATELY;
          mg_http_send_error(nc, 500, "Bad headers");
        } else {
          struct http_message hm;
          struct mg_str *h;
          mg_http_parse_headers(io->buf, io->buf + io->len, io->len, &hm);
          if (mg_get_http_header(&hm, "Location") != NULL) {
            mg_printf(nc, "%s", "HTTP/1.1 302 Moved\r\n");
          } else if ((h = mg_get_http_header(&hm, "Status")) != NULL) {
            mg_printf(nc, "HTTP/1.1 %.*s\r\n", (int) h->len, h->p);
          } else {
            mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\n");
          }
        }
        nc->flags &= ~MG_F_USER_1;
      }
      if (!(nc->flags & MG_F_USER_1)) {
        mg_forward(cgi_nc, nc);
      }
      break;
    case MG_EV_CLOSE:
      mg_http_free_proto_data_cgi(&mg_http_get_proto_data(cgi_nc)->cgi);
      nc->flags |= MG_F_SEND_AND_CLOSE;
      break;
  }
}

static void mg_handle_cgi(struct mg_connection *nc, const char *prog,
                          const struct mg_str *path_info,
                          const struct http_message *hm,
                          const struct mg_serve_http_opts *opts) {
  struct mg_cgi_env_block blk;
  char dir[MAX_PATH_SIZE];
  const char *p;
  sock_t fds[2];

  DBG(("%p [%s]", nc, prog));
  mg_prepare_cgi_environment(nc, prog, path_info, hm, opts, &blk);
  /*
   * CGI must be executed in its own directory. 'dir' must point to the
   * directory containing executable program, 'p' must point to the
   * executable program name relative to 'dir'.
   */
  if ((p = strrchr(prog, DIRSEP)) == NULL) {
    snprintf(dir, sizeof(dir), "%s", ".");
  } else {
    snprintf(dir, sizeof(dir), "%.*s", (int) (p - prog), prog);
    prog = p + 1;
  }

  /*
   * Try to create socketpair in a loop until success. mg_socketpair()
   * can be interrupted by a signal and fail.
   * TODO(lsm): use sigaction to restart interrupted syscall
   */
  do {
    mg_socketpair(fds, SOCK_STREAM);
  } while (fds[0] == INVALID_SOCKET);

  if (mg_start_process(opts->cgi_interpreter, prog, blk.buf, blk.vars, dir,
                       fds[1]) != 0) {
    size_t n = nc->recv_mbuf.len - (hm->message.len - hm->body.len);
    struct mg_connection *cgi_nc =
        mg_add_sock(nc->mgr, fds[0], mg_cgi_ev_handler);
    struct mg_http_proto_data *cgi_pd = mg_http_get_proto_data(cgi_nc);
    cgi_pd->cgi.cgi_nc = cgi_nc;
    cgi_pd->cgi.cgi_nc->user_data = nc;
    nc->flags |= MG_F_USER_1;
    /* Push POST data to the CGI */
    if (n > 0 && n < nc->recv_mbuf.len) {
      mg_send(cgi_pd->cgi.cgi_nc, hm->body.p, n);
    }
    mbuf_remove(&nc->recv_mbuf, nc->recv_mbuf.len);
  } else {
    closesocket(fds[0]);
    mg_http_send_error(nc, 500, "CGI failure");
  }

#ifndef _WIN32
  closesocket(fds[1]); /* On Windows, CGI stdio thread closes that socket */
#endif
}
#endif

static int mg_get_month_index(const char *s) {
  static const char *month_names[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  size_t i;

  for (i = 0; i < ARRAY_SIZE(month_names); i++)
    if (!strcmp(s, month_names[i])) return (int) i;

  return -1;
}

static int mg_num_leap_years(int year) {
  return year / 4 - year / 100 + year / 400;
}

/* Parse UTC date-time string, and return the corresponding time_t value. */
MG_INTERNAL time_t mg_parse_date_string(const char *datetime) {
  static const unsigned short days_before_month[] = {
      0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
  char month_str[32];
  int second, minute, hour, day, month, year, leap_days, days;
  time_t result = (time_t) 0;

  if (((sscanf(datetime, "%d/%3s/%d %d:%d:%d", &day, month_str, &year, &hour,
               &minute, &second) == 6) ||
       (sscanf(datetime, "%d %3s %d %d:%d:%d", &day, month_str, &year, &hour,
               &minute, &second) == 6) ||
       (sscanf(datetime, "%*3s, %d %3s %d %d:%d:%d", &day, month_str, &year,
               &hour, &minute, &second) == 6) ||
       (sscanf(datetime, "%d-%3s-%d %d:%d:%d", &day, month_str, &year, &hour,
               &minute, &second) == 6)) &&
      year > 1970 && (month = mg_get_month_index(month_str)) != -1) {
    leap_days = mg_num_leap_years(year) - mg_num_leap_years(1970);
    year -= 1970;
    days = year * 365 + days_before_month[month] + (day - 1) + leap_days;
    result = days * 24 * 3600 + hour * 3600 + minute * 60 + second;
  }

  return result;
}

MG_INTERNAL int mg_is_not_modified(struct http_message *hm, cs_stat_t *st) {
  struct mg_str *hdr;
  if ((hdr = mg_get_http_header(hm, "If-None-Match")) != NULL) {
    char etag[64];
    mg_http_construct_etag(etag, sizeof(etag), st);
    return mg_vcasecmp(hdr, etag) == 0;
  } else if ((hdr = mg_get_http_header(hm, "If-Modified-Since")) != NULL) {
    return st->st_mtime <= mg_parse_date_string(hdr->p);
  } else {
    return 0;
  }
}

static void mg_http_send_digest_auth_request(struct mg_connection *c,
                                             const char *domain) {
  mg_printf(c,
            "HTTP/1.1 401 Unauthorized\r\n"
            "WWW-Authenticate: Digest qop=\"auth\", "
            "realm=\"%s\", nonce=\"%lu\"\r\n"
            "Content-Length: 0\r\n\r\n",
            domain, (unsigned long) time(NULL));
}

static void mg_http_send_options(struct mg_connection *nc) {
  mg_printf(nc, "%s",
            "HTTP/1.1 200 OK\r\nAllow: GET, POST, HEAD, CONNECT, OPTIONS"
#ifndef MG_DISABLE_DAV
            ", MKCOL, PUT, DELETE, PROPFIND, MOVE\r\nDAV: 1,2"
#endif
            "\r\n\r\n");
  nc->flags |= MG_F_SEND_AND_CLOSE;
}

static int mg_is_creation_request(const struct http_message *hm) {
  return mg_vcmp(&hm->method, "MKCOL") == 0 || mg_vcmp(&hm->method, "PUT") == 0;
}

MG_INTERNAL void mg_send_http_file(struct mg_connection *nc, char *path,
                                   const struct mg_str *path_info,
                                   struct http_message *hm,
                                   struct mg_serve_http_opts *opts) {
  int exists, is_directory, is_dav = mg_is_dav_request(&hm->method);
  int is_cgi;
  char *index_file = NULL;
  cs_stat_t st;

  exists = (mg_stat(path, &st) == 0);
  is_directory = exists && S_ISDIR(st.st_mode);

  if (is_directory)
    mg_find_index_file(path, opts->index_files, &index_file, &st);

  is_cgi =
      (mg_match_prefix(opts->cgi_file_pattern, strlen(opts->cgi_file_pattern),
                       index_file ? index_file : path) > 0);

  DBG(("%p %.*s [%s] exists=%d is_dir=%d is_dav=%d is_cgi=%d index=%s", nc,
       (int) hm->method.len, hm->method.p, path, exists, is_directory, is_dav,
       is_cgi, index_file ? index_file : ""));

  if (is_directory && hm->uri.p[hm->uri.len - 1] != '/' && !is_dav) {
    mg_printf(nc,
              "HTTP/1.1 301 Moved\r\nLocation: %.*s/\r\n"
              "Content-Length: 0\r\n\r\n",
              (int) hm->uri.len, hm->uri.p);
    MG_FREE(index_file);
    return;
  }

  /* If we have path_info, the only way to handle it is CGI. */
  if (path_info->len > 0 && !is_cgi) {
    mg_http_send_error(nc, 501, NULL);
    MG_FREE(index_file);
    return;
  }

  if (is_dav && opts->dav_document_root == NULL) {
    mg_http_send_error(nc, 501, NULL);
  } else if (!mg_is_authorized(hm, path, is_directory, opts->auth_domain,
                               opts->global_auth_file, 1) ||
             !mg_is_authorized(hm, path, is_directory, opts->auth_domain,
                               opts->per_directory_auth_file, 0)) {
    mg_http_send_digest_auth_request(nc, opts->auth_domain);
  } else if (is_cgi) {
#if !defined(MG_DISABLE_CGI)
    mg_handle_cgi(nc, index_file ? index_file : path, path_info, hm, opts);
#else
    mg_http_send_error(nc, 501, NULL);
#endif /* MG_DISABLE_CGI */
  } else if ((!exists ||
              mg_is_file_hidden(path, opts, 0 /* specials are ok */)) &&
             !mg_is_creation_request(hm)) {
    mg_http_send_error(nc, 404, NULL);
#ifndef MG_DISABLE_DAV
  } else if (!mg_vcmp(&hm->method, "PROPFIND")) {
    mg_handle_propfind(nc, path, &st, hm, opts);
#ifndef MG_DISABLE_DAV_AUTH
  } else if (is_dav &&
             (opts->dav_auth_file == NULL ||
              (strcmp(opts->dav_auth_file, "-") != 0 &&
               !mg_is_authorized(hm, path, is_directory, opts->auth_domain,
                                 opts->dav_auth_file, 1)))) {
    mg_http_send_digest_auth_request(nc, opts->auth_domain);
#endif
  } else if (!mg_vcmp(&hm->method, "MKCOL")) {
    mg_handle_mkcol(nc, path, hm);
  } else if (!mg_vcmp(&hm->method, "DELETE")) {
    mg_handle_delete(nc, opts, path);
  } else if (!mg_vcmp(&hm->method, "PUT")) {
    mg_handle_put(nc, path, hm);
  } else if (!mg_vcmp(&hm->method, "MOVE")) {
    mg_handle_move(nc, opts, path, hm);
#ifdef MG_ENABLE_FAKE_DAVLOCK
  } else if (!mg_vcmp(&hm->method, "LOCK")) {
    mg_handle_lock(nc, path);
#endif
#endif
  } else if (!mg_vcmp(&hm->method, "OPTIONS")) {
    mg_http_send_options(nc);
  } else if (is_directory && index_file == NULL) {
#ifndef MG_DISABLE_DIRECTORY_LISTING
    if (strcmp(opts->enable_directory_listing, "yes") == 0) {
      mg_send_directory_listing(nc, path, hm, opts);
    } else {
      mg_http_send_error(nc, 403, NULL);
    }
#else
    mg_http_send_error(nc, 501, NULL);
#endif
  } else if (mg_is_not_modified(hm, &st)) {
    mg_http_send_error(nc, 304, "Not Modified");
  } else {
    mg_http_send_file2(nc, index_file ? index_file : path, &st, hm, opts);
  }
  MG_FREE(index_file);
}

void mg_serve_http(struct mg_connection *nc, struct http_message *hm,
                   struct mg_serve_http_opts opts) {
  char *path = NULL;
  struct mg_str *hdr, path_info;
  uint32_t remote_ip = ntohl(*(uint32_t *) &nc->sa.sin.sin_addr);

  if (mg_check_ip_acl(opts.ip_acl, remote_ip) != 1) {
    /* Not allowed to connect */
    mg_http_send_error(nc, 403, NULL);
    nc->flags |= MG_F_SEND_AND_CLOSE;
    return;
  }

  if (mg_http_send_port_based_redirect(nc, hm, &opts)) {
    return;
  }

  if (opts.document_root == NULL) {
    opts.document_root = ".";
  }
  if (opts.per_directory_auth_file == NULL) {
    opts.per_directory_auth_file = ".htpasswd";
  }
  if (opts.enable_directory_listing == NULL) {
    opts.enable_directory_listing = "yes";
  }
  if (opts.cgi_file_pattern == NULL) {
    opts.cgi_file_pattern = "**.cgi$|**.php$";
  }
  if (opts.ssi_pattern == NULL) {
    opts.ssi_pattern = "**.shtml$|**.shtm$";
  }
  if (opts.index_files == NULL) {
    opts.index_files = "index.html,index.htm,index.shtml,index.cgi,index.php";
  }
  /* Normalize path - resolve "." and ".." (in-place). */
  if (!mg_normalize_uri_path(&hm->uri, &hm->uri)) {
    mg_http_send_error(nc, 400, NULL);
    return;
  }
  if (mg_uri_to_local_path(hm, &opts, &path, &path_info) == 0) {
    mg_http_send_error(nc, 404, NULL);
    return;
  }
  mg_send_http_file(nc, path, &path_info, hm, &opts);

  MG_FREE(path);
  path = NULL;

  /* Close connection for non-keep-alive requests */
  if (mg_vcmp(&hm->proto, "HTTP/1.1") != 0 ||
      ((hdr = mg_get_http_header(hm, "Connection")) != NULL &&
       mg_vcmp(hdr, "keep-alive") != 0)) {
#if 0
    nc->flags |= MG_F_SEND_AND_CLOSE;
#endif
  }
}

#endif /* MG_DISABLE_FILESYSTEM */

/* returns 0 on success, -1 on error */
static int mg_http_common_url_parse(const char *url, const char *schema,
                                    const char *schema_tls, int *use_ssl,
                                    char **addr, int *port_i,
                                    const char **path) {
  int addr_len = 0;

  if (memcmp(url, schema, strlen(schema)) == 0) {
    url += strlen(schema);
  } else if (memcmp(url, schema_tls, strlen(schema_tls)) == 0) {
    url += strlen(schema_tls);
    *use_ssl = 1;
#ifndef MG_ENABLE_SSL
    return -1; /* SSL is not enabled, cannot do HTTPS URLs */
#endif
  }

  while (*url != '\0') {
    *addr = (char *) MG_REALLOC(*addr, addr_len + 6 /* space for port too. */);
    if (*addr == NULL) {
      DBG(("OOM"));
      return -1;
    }
    if (*url == '/') {
      break;
    }
    if (*url == ':') *port_i = addr_len;
    (*addr)[addr_len++] = *url;
    (*addr)[addr_len] = '\0';
    url++;
  }
  if (addr_len == 0) goto cleanup;
  if (*port_i < 0) {
    *port_i = addr_len;
    strcpy(*addr + *port_i, *use_ssl ? ":443" : ":80");
  } else {
    *port_i = -1;
  }

  if (*path == NULL) *path = url;

  if (**path == '\0') *path = "/";

  DBG(("%s %s", *addr, *path));

  return 0;

cleanup:
  MG_FREE(*addr);
  return -1;
}

struct mg_connection *mg_connect_http_base(
    struct mg_mgr *mgr, mg_event_handler_t ev_handler,
    struct mg_connect_opts opts, const char *schema, const char *schema_ssl,
    const char *url, const char **path, char **addr) {
  struct mg_connection *nc = NULL;
  int port_i = -1;
  int use_ssl = 0;

  if (mg_http_common_url_parse(url, schema, schema_ssl, &use_ssl, addr, &port_i,
                               path) < 0) {
    return NULL;
  }

#ifndef MG_ENABLE_SSL
  if (use_ssl) {
    MG_SET_PTRPTR(opts.error_string, "ssl is disabled");
    MG_FREE(addr);
    return NULL;
  }
#endif
#if defined(MG_ENABLE_SSL) && defined(MG_SOCKET_SIMPLELINK)
  if (use_ssl && opts.ssl_ca_cert == NULL) {
    opts.ssl_ca_cert = "";
  }
#endif

  if ((nc = mg_connect_opt(mgr, *addr, ev_handler, opts)) != NULL) {
#if defined(MG_ENABLE_SSL) && !defined(MG_SOCKET_SIMPLELINK)
    if (use_ssl && nc->ssl_ctx == NULL) {
      /*
       * Schema requires SSL, but no SSL parameters were provided in
       * opts. In order to maintain backward compatibility, use
       * NULL, NULL
       */
      mg_set_ssl(nc, NULL, NULL);
    }
#endif
    mg_set_protocol_http_websocket(nc);

    /* If the port was addred by us, restore the original host. */
    if (port_i >= 0) (*addr)[port_i] = '\0';
  }

  return nc;
}

struct mg_connection *mg_connect_http_opt(struct mg_mgr *mgr,
                                          mg_event_handler_t ev_handler,
                                          struct mg_connect_opts opts,
                                          const char *url,
                                          const char *extra_headers,
                                          const char *post_data) {
  char *addr = NULL;
  const char *path = NULL;
  struct mg_connection *nc = mg_connect_http_base(
      mgr, ev_handler, opts, "http://", "https://", url, &path, &addr);

  if (nc == NULL) {
    return NULL;
  }

  mg_printf(nc, "%s %s HTTP/1.1\r\nHost: %s\r\nContent-Length: %" SIZE_T_FMT
                "\r\n%s\r\n%s",
            post_data == NULL ? "GET" : "POST", path, addr,
            post_data == NULL ? 0 : strlen(post_data),
            extra_headers == NULL ? "" : extra_headers,
            post_data == NULL ? "" : post_data);

  MG_FREE(addr);
  return nc;
}

struct mg_connection *mg_connect_http(struct mg_mgr *mgr,
                                      mg_event_handler_t ev_handler,
                                      const char *url,
                                      const char *extra_headers,
                                      const char *post_data) {
  struct mg_connect_opts opts;
  memset(&opts, 0, sizeof(opts));
  return mg_connect_http_opt(mgr, ev_handler, opts, url, extra_headers,
                             post_data);
}

#ifndef MG_DISABLE_HTTP_WEBSOCKET
struct mg_connection *mg_connect_ws_opt(struct mg_mgr *mgr,
                                        mg_event_handler_t ev_handler,
                                        struct mg_connect_opts opts,
                                        const char *url, const char *protocol,
                                        const char *extra_headers) {
  char *addr = NULL;
  const char *path = NULL;
  struct mg_connection *nc = mg_connect_http_base(
      mgr, ev_handler, opts, "ws://", "wss://", url, &path, &addr);

  if (nc != NULL) {
    mg_send_websocket_handshake2(nc, path, addr, protocol, extra_headers);
  }

  MG_FREE(addr);
  return nc;
}

struct mg_connection *mg_connect_ws(struct mg_mgr *mgr,
                                    mg_event_handler_t ev_handler,
                                    const char *url, const char *protocol,
                                    const char *extra_headers) {
  struct mg_connect_opts opts;
  memset(&opts, 0, sizeof(opts));
  return mg_connect_ws_opt(mgr, ev_handler, opts, url, protocol, extra_headers);
}
#endif /* MG_DISABLE_HTTP_WEBSOCKET */

size_t mg_parse_multipart(const char *buf, size_t buf_len, char *var_name,
                          size_t var_name_len, char *file_name,
                          size_t file_name_len, const char **data,
                          size_t *data_len) {
  static const char cd[] = "Content-Disposition: ";
  size_t hl, bl, n, ll, pos, cdl = sizeof(cd) - 1;

  if (buf == NULL || buf_len <= 0) return 0;
  if ((hl = mg_http_get_request_len(buf, buf_len)) <= 0) return 0;
  if (buf[0] != '-' || buf[1] != '-' || buf[2] == '\n') return 0;

  /* Get boundary length */
  bl = mg_get_line_len(buf, buf_len);

  /* Loop through headers, fetch variable name and file name */
  var_name[0] = file_name[0] = '\0';
  for (n = bl; (ll = mg_get_line_len(buf + n, hl - n)) > 0; n += ll) {
    if (mg_ncasecmp(cd, buf + n, cdl) == 0) {
      struct mg_str header;
      header.p = buf + n + cdl;
      header.len = ll - (cdl + 2);
      mg_http_parse_header(&header, "name", var_name, var_name_len);
      mg_http_parse_header(&header, "filename", file_name, file_name_len);
    }
  }

  /* Scan through the body, search for terminating boundary */
  for (pos = hl; pos + (bl - 2) < buf_len; pos++) {
    if (buf[pos] == '-' && !memcmp(buf, &buf[pos], bl - 2)) {
      if (data_len != NULL) *data_len = (pos - 2) - hl;
      if (data != NULL) *data = buf + hl;
      return pos;
    }
  }

  return 0;
}

void mg_register_http_endpoint(struct mg_connection *nc, const char *uri_path,
                               mg_event_handler_t handler) {
  struct mg_http_proto_data *pd = mg_http_get_proto_data(nc);
  struct mg_http_endpoint *new_ep =
      (struct mg_http_endpoint *) calloc(1, sizeof(*new_ep));
  new_ep->name = strdup(uri_path);
  new_ep->name_len = strlen(new_ep->name);
  new_ep->handler = handler;
  new_ep->next = pd->endpoints;
  pd->endpoints = new_ep;
}

#endif /* MG_DISABLE_HTTP */
#ifdef MG_MODULE_LINES
#line 1 "./src/util.c"
#endif
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

/* Amalgamated: #include "common/base64.h" */
/* Amalgamated: #include "mongoose/src/internal.h" */
/* Amalgamated: #include "mongoose/src/util.h" */

const char *mg_skip(const char *s, const char *end, const char *delims,
                    struct mg_str *v) {
  v->p = s;
  while (s < end && strchr(delims, *(unsigned char *) s) == NULL) s++;
  v->len = s - v->p;
  while (s < end && strchr(delims, *(unsigned char *) s) != NULL) s++;
  return s;
}

static int lowercase(const char *s) {
  return tolower(*(const unsigned char *) s);
}

int mg_ncasecmp(const char *s1, const char *s2, size_t len) {
  int diff = 0;

  if (len > 0) do {
      diff = lowercase(s1++) - lowercase(s2++);
    } while (diff == 0 && s1[-1] != '\0' && --len > 0);

  return diff;
}

int mg_casecmp(const char *s1, const char *s2) {
  return mg_ncasecmp(s1, s2, (size_t) ~0);
}

#ifndef MG_DISABLE_FILESYSTEM
int mg_stat(const char *path, cs_stat_t *st) {
#ifdef _WIN32
  wchar_t wpath[MAX_PATH_SIZE];
  to_wchar(path, wpath, ARRAY_SIZE(wpath));
  DBG(("[%ls] -> %d", wpath, _wstati64(wpath, st)));
  return _wstati64(wpath, (struct _stati64 *) st);
#else
  return stat(path, st);
#endif
}

FILE *mg_fopen(const char *path, const char *mode) {
#ifdef _WIN32
  wchar_t wpath[MAX_PATH_SIZE], wmode[10];
  to_wchar(path, wpath, ARRAY_SIZE(wpath));
  to_wchar(mode, wmode, ARRAY_SIZE(wmode));
  return _wfopen(wpath, wmode);
#else
  return fopen(path, mode);
#endif
}

int mg_open(const char *path, int flag, int mode) { /* LCOV_EXCL_LINE */
#ifdef _WIN32
  wchar_t wpath[MAX_PATH_SIZE];
  to_wchar(path, wpath, ARRAY_SIZE(wpath));
  return _wopen(wpath, flag, mode);
#else
  return open(path, flag, mode); /* LCOV_EXCL_LINE */
#endif
}
#endif

void mg_base64_encode(const unsigned char *src, int src_len, char *dst) {
  cs_base64_encode(src, src_len, dst);
}

int mg_base64_decode(const unsigned char *s, int len, char *dst) {
  return cs_base64_decode(s, len, dst);
}

#ifdef MG_ENABLE_THREADS
void *mg_start_thread(void *(*f)(void *), void *p) {
#ifdef _WIN32
  return (void *) _beginthread((void(__cdecl *) (void *) ) f, 0, p);
#else
  pthread_t thread_id = (pthread_t) 0;
  pthread_attr_t attr;

  (void) pthread_attr_init(&attr);
  (void) pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

#if defined(MG_STACK_SIZE) && MG_STACK_SIZE > 1
  (void) pthread_attr_setstacksize(&attr, MG_STACK_SIZE);
#endif

  pthread_create(&thread_id, &attr, f, p);
  pthread_attr_destroy(&attr);

  return (void *) thread_id;
#endif
}
#endif /* MG_ENABLE_THREADS */

/* Set close-on-exec bit for a given socket. */
void mg_set_close_on_exec(sock_t sock) {
#ifdef _WIN32
  (void) SetHandleInformation((HANDLE) sock, HANDLE_FLAG_INHERIT, 0);
#elif defined(__unix__)
  fcntl(sock, F_SETFD, FD_CLOEXEC);
#else
  (void) sock;
#endif
}

void mg_sock_addr_to_str(const union socket_address *sa, char *buf, size_t len,
                         int flags) {
  int is_v6;
  if (buf == NULL || len <= 0) return;
  buf[0] = '\0';
#if defined(MG_ENABLE_IPV6)
  is_v6 = sa->sa.sa_family == AF_INET6;
#else
  is_v6 = 0;
#endif
  if (flags & MG_SOCK_STRINGIFY_IP) {
#if defined(MG_ENABLE_IPV6)
    const void *addr = NULL;
    char *start = buf;
    socklen_t capacity = len;
    if (!is_v6) {
      addr = &sa->sin.sin_addr;
    } else {
      addr = (void *) &sa->sin6.sin6_addr;
      if (flags & MG_SOCK_STRINGIFY_PORT) {
        *buf = '[';
        start++;
        capacity--;
      }
    }
    if (inet_ntop(sa->sa.sa_family, addr, start, capacity) == NULL) {
      *buf = '\0';
    }
#elif defined(_WIN32) || defined(MG_LWIP)
    /* Only Windoze Vista (and newer) have inet_ntop() */
    strncpy(buf, inet_ntoa(sa->sin.sin_addr), len);
#else
    inet_ntop(AF_INET, (void *) &sa->sin.sin_addr, buf, len);
#endif
  }
  if (flags & MG_SOCK_STRINGIFY_PORT) {
    int port = ntohs(sa->sin.sin_port);
    if (flags & MG_SOCK_STRINGIFY_IP) {
      snprintf(buf + strlen(buf), len - (strlen(buf) + 1), "%s:%d",
               (is_v6 ? "]" : ""), port);
    } else {
      snprintf(buf, len, "%d", port);
    }
  }
}

void mg_conn_addr_to_str(struct mg_connection *nc, char *buf, size_t len,
                         int flags) {
  union socket_address sa;
  memset(&sa, 0, sizeof(sa));
  mg_if_get_conn_addr(nc, flags & MG_SOCK_STRINGIFY_REMOTE, &sa);
  mg_sock_addr_to_str(&sa, buf, len, flags);
}

#ifndef MG_DISABLE_HEXDUMP
int mg_hexdump(const void *buf, int len, char *dst, int dst_len) {
  const unsigned char *p = (const unsigned char *) buf;
  char ascii[17] = "";
  int i, idx, n = 0;

  for (i = 0; i < len; i++) {
    idx = i % 16;
    if (idx == 0) {
      if (i > 0) n += snprintf(dst + n, dst_len - n, "  %s\n", ascii);
      n += snprintf(dst + n, dst_len - n, "%04x ", i);
    }
    n += snprintf(dst + n, dst_len - n, " %02x", p[i]);
    ascii[idx] = p[i] < 0x20 || p[i] > 0x7e ? '.' : p[i];
    ascii[idx + 1] = '\0';
  }

  while (i++ % 16) n += snprintf(dst + n, dst_len - n, "%s", "   ");
  n += snprintf(dst + n, dst_len - n, "  %s\n\n", ascii);

  return n;
}
#endif

int mg_avprintf(char **buf, size_t size, const char *fmt, va_list ap) {
  va_list ap_copy;
  int len;

  va_copy(ap_copy, ap);
  len = vsnprintf(*buf, size, fmt, ap_copy);
  va_end(ap_copy);

  if (len < 0) {
    /* eCos and Windows are not standard-compliant and return -1 when
     * the buffer is too small. Keep allocating larger buffers until we
     * succeed or out of memory. */
    *buf = NULL; /* LCOV_EXCL_START */
    while (len < 0) {
      MG_FREE(*buf);
      size *= 2;
      if ((*buf = (char *) MG_MALLOC(size)) == NULL) break;
      va_copy(ap_copy, ap);
      len = vsnprintf(*buf, size, fmt, ap_copy);
      va_end(ap_copy);
    }
    /* LCOV_EXCL_STOP */
  } else if (len >= (int) size) {
    /* Standard-compliant code path. Allocate a buffer that is large enough. */
    if ((*buf = (char *) MG_MALLOC(len + 1)) == NULL) {
      len = -1; /* LCOV_EXCL_LINE */
    } else {    /* LCOV_EXCL_LINE */
      va_copy(ap_copy, ap);
      len = vsnprintf(*buf, len + 1, fmt, ap_copy);
      va_end(ap_copy);
    }
  }

  return len;
}

#if !defined(MG_DISABLE_HEXDUMP)
void mg_hexdump_connection(struct mg_connection *nc, const char *path,
                           const void *buf, int num_bytes, int ev) {
#if !defined(NO_LIBC) && !defined(MG_DISABLE_STDIO)
  FILE *fp = NULL;
  char *hexbuf, src[60], dst[60];
  int buf_size = num_bytes * 5 + 100;

  if (strcmp(path, "-") == 0) {
    fp = stdout;
  } else if (strcmp(path, "--") == 0) {
    fp = stderr;
#ifndef MG_DISABLE_FILESYSTEM
  } else {
    fp = fopen(path, "a");
#endif
  }
  if (fp == NULL) return;

  mg_conn_addr_to_str(nc, src, sizeof(src),
                      MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
  mg_conn_addr_to_str(nc, dst, sizeof(dst), MG_SOCK_STRINGIFY_IP |
                                                MG_SOCK_STRINGIFY_PORT |
                                                MG_SOCK_STRINGIFY_REMOTE);
  fprintf(
      fp, "%lu %p %s %s %s %d\n", (unsigned long) time(NULL), (void *) nc, src,
      ev == MG_EV_RECV ? "<-" : ev == MG_EV_SEND
                                    ? "->"
                                    : ev == MG_EV_ACCEPT
                                          ? "<A"
                                          : ev == MG_EV_CONNECT ? "C>" : "XX",
      dst, num_bytes);
  if (num_bytes > 0 && (hexbuf = (char *) MG_MALLOC(buf_size)) != NULL) {
    mg_hexdump(buf, num_bytes, hexbuf, buf_size);
    fprintf(fp, "%s", hexbuf);
    MG_FREE(hexbuf);
  }
  if (fp != stdin && fp != stdout) fclose(fp);
#endif
}
#endif

int mg_is_big_endian(void) {
  static const int n = 1;
  /* TODO(mkm) use compiletime check with 4-byte char literal */
  return ((char *) &n)[0] == 0;
}

const char *mg_next_comma_list_entry(const char *list, struct mg_str *val,
                                     struct mg_str *eq_val) {
  if (list == NULL || *list == '\0') {
    /* End of the list */
    list = NULL;
  } else {
    val->p = list;
    if ((list = strchr(val->p, ',')) != NULL) {
      /* Comma found. Store length and shift the list ptr */
      val->len = list - val->p;
      list++;
    } else {
      /* This value is the last one */
      list = val->p + strlen(val->p);
      val->len = list - val->p;
    }

    if (eq_val != NULL) {
      /* Value has form "x=y", adjust pointers and lengths */
      /* so that val points to "x", and eq_val points to "y". */
      eq_val->len = 0;
      eq_val->p = (const char *) memchr(val->p, '=', val->len);
      if (eq_val->p != NULL) {
        eq_val->p++; /* Skip over '=' character */
        eq_val->len = val->p + val->len - eq_val->p;
        val->len = (eq_val->p - val->p) - 1;
      }
    }
  }

  return list;
}

int mg_match_prefix_n(const struct mg_str pattern, const struct mg_str str) {
  const char *or_str;
  size_t len, i = 0, j = 0;
  int res;

  if ((or_str = (const char *) memchr(pattern.p, '|', pattern.len)) != NULL) {
    struct mg_str pstr = {pattern.p, (size_t)(or_str - pattern.p)};
    res = mg_match_prefix_n(pstr, str);
    if (res > 0) return res;
    pstr.p = or_str + 1;
    pstr.len = (pattern.p + pattern.len) - (or_str + 1);
    return mg_match_prefix_n(pstr, str);
  }

  for (; i < pattern.len; i++, j++) {
    if (pattern.p[i] == '?' && j != str.len) {
      continue;
    } else if (pattern.p[i] == '$') {
      return j == str.len ? (int) j : -1;
    } else if (pattern.p[i] == '*') {
      i++;
      if (pattern.p[i] == '*') {
        i++;
        len = str.len - j;
      } else {
        len = 0;
        while (j + len != str.len && str.p[len] != '/') {
          len++;
        }
      }
      if (i == pattern.len) {
        return j + len;
      }
      do {
        const struct mg_str pstr = {pattern.p + i, pattern.len - i};
        const struct mg_str sstr = {str.p + j + len, str.len - j - len};
        res = mg_match_prefix_n(pstr, sstr);
      } while (res == -1 && len-- > 0);
      return res == -1 ? -1 : (int) (j + res + len);
    } else if (lowercase(&pattern.p[i]) != lowercase(&str.p[j])) {
      return -1;
    }
  }
  return j;
}

int mg_match_prefix(const char *pattern, int pattern_len, const char *str) {
  const struct mg_str pstr = {pattern, (size_t) pattern_len};
  return mg_match_prefix_n(pstr, mg_mk_str(str));
}
#ifdef MG_MODULE_LINES
#line 1 "./src/mqtt.c"
#endif
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#ifndef MG_DISABLE_MQTT

#include <string.h>

/* Amalgamated: #include "mongoose/src/internal.h" */
/* Amalgamated: #include "mongoose/src/mqtt.h" */

MG_INTERNAL int parse_mqtt(struct mbuf *io, struct mg_mqtt_message *mm) {
  uint8_t header;
  int cmd;
  size_t len = 0;
  int var_len = 0;
  char *vlen = &io->buf[1];

  if (io->len < 2) return -1;

  header = io->buf[0];
  cmd = header >> 4;

  /* decode mqtt variable length */
  do {
    len += (*vlen & 127) << 7 * (vlen - &io->buf[1]);
  } while ((*vlen++ & 128) != 0 && ((size_t)(vlen - io->buf) <= io->len));

  if (len != 0 && io->len < (size_t)(len - 1)) return -1;

  mbuf_remove(io, 1 + (vlen - &io->buf[1]));
  mm->cmd = cmd;
  mm->qos = MG_MQTT_GET_QOS(header);

  switch (cmd) {
    case MG_MQTT_CMD_CONNECT:
      /* TODO(mkm): parse keepalive and will */
      break;
    case MG_MQTT_CMD_CONNACK:
      mm->connack_ret_code = io->buf[1];
      var_len = 2;
      break;
    case MG_MQTT_CMD_PUBACK:
    case MG_MQTT_CMD_PUBREC:
    case MG_MQTT_CMD_PUBREL:
    case MG_MQTT_CMD_PUBCOMP:
    case MG_MQTT_CMD_SUBACK:
      mm->message_id = ntohs(*(uint16_t *) io->buf);
      var_len = 2;
      break;
    case MG_MQTT_CMD_PUBLISH: {
      uint16_t topic_len = ntohs(*(uint16_t *) io->buf);
      mm->topic = (char *) MG_MALLOC(topic_len + 1);
      mm->topic[topic_len] = 0;
      strncpy(mm->topic, io->buf + 2, topic_len);
      var_len = topic_len + 2;

      if (MG_MQTT_GET_QOS(header) > 0) {
        mm->message_id = ntohs(*(uint16_t *) io->buf);
        var_len += 2;
      }
    } break;
    case MG_MQTT_CMD_SUBSCRIBE:
      /*
       * topic expressions are left in the payload and can be parsed with
       * `mg_mqtt_next_subscribe_topic`
       */
      mm->message_id = ntohs(*(uint16_t *) io->buf);
      var_len = 2;
      break;
    default:
      /* Unhandled command */
      break;
  }

  mbuf_remove(io, var_len);
  return len - var_len;
}

static void mqtt_handler(struct mg_connection *nc, int ev, void *ev_data) {
  int len;
  struct mbuf *io = &nc->recv_mbuf;
  struct mg_mqtt_message mm;
  memset(&mm, 0, sizeof(mm));

  nc->handler(nc, ev, ev_data);

  switch (ev) {
    case MG_EV_RECV:
      len = parse_mqtt(io, &mm);
      if (len == -1) break; /* not fully buffered */
      mm.payload.p = io->buf;
      mm.payload.len = len;

      nc->handler(nc, MG_MQTT_EVENT_BASE + mm.cmd, &mm);

      if (mm.topic) {
        MG_FREE(mm.topic);
      }
      mbuf_remove(io, mm.payload.len);
      break;
  }
}

void mg_set_protocol_mqtt(struct mg_connection *nc) {
  nc->proto_handler = mqtt_handler;
}

void mg_send_mqtt_handshake(struct mg_connection *nc, const char *client_id) {
  static struct mg_send_mqtt_handshake_opts opts;
  mg_send_mqtt_handshake_opt(nc, client_id, opts);
}

void mg_send_mqtt_handshake_opt(struct mg_connection *nc, const char *client_id,
                                struct mg_send_mqtt_handshake_opts opts) {
  uint8_t header = MG_MQTT_CMD_CONNECT << 4;
  uint8_t rem_len;
  uint16_t keep_alive;
  uint16_t len;

  /*
   * 9: version_header(len, magic_string, version_number), 1: flags, 2:
   * keep-alive timer,
   * 2: client_identifier_len, n: client_id
   */
  rem_len = 9 + 1 + 2 + 2 + strlen(client_id);

  if (opts.user_name != NULL) {
    opts.flags |= MG_MQTT_HAS_USER_NAME;
    rem_len += strlen(opts.user_name) + 2;
  }
  if (opts.password != NULL) {
    opts.flags |= MG_MQTT_HAS_PASSWORD;
    rem_len += strlen(opts.password) + 2;
  }

  mg_send(nc, &header, 1);
  mg_send(nc, &rem_len, 1);
  mg_send(nc, "\00\06MQIsdp\03", 9);
  mg_send(nc, &opts.flags, 1);

  if (opts.keep_alive == 0) {
    opts.keep_alive = 60;
  }
  keep_alive = htons(opts.keep_alive);
  mg_send(nc, &keep_alive, 2);

  len = htons(strlen(client_id));
  mg_send(nc, &len, 2);
  mg_send(nc, client_id, strlen(client_id));

  if (opts.flags & MG_MQTT_HAS_USER_NAME) {
    len = htons(strlen(opts.user_name));
    mg_send(nc, &len, 2);
    mg_send(nc, opts.user_name, strlen(opts.user_name));
  }
  if (opts.flags & MG_MQTT_HAS_PASSWORD) {
    len = htons(strlen(opts.password));
    mg_send(nc, &len, 2);
    mg_send(nc, opts.password, strlen(opts.password));
  }
}

static void mg_mqtt_prepend_header(struct mg_connection *nc, uint8_t cmd,
                                   uint8_t flags, size_t len) {
  size_t off = nc->send_mbuf.len - len;
  uint8_t header = cmd << 4 | (uint8_t) flags;

  uint8_t buf[1 + sizeof(size_t)];
  uint8_t *vlen = &buf[1];

  assert(nc->send_mbuf.len >= len);

  buf[0] = header;

  /* mqtt variable length encoding */
  do {
    *vlen = len % 0x80;
    len /= 0x80;
    if (len > 0) *vlen |= 0x80;
    vlen++;
  } while (len > 0);

  mbuf_insert(&nc->send_mbuf, off, buf, vlen - buf);
}

void mg_mqtt_publish(struct mg_connection *nc, const char *topic,
                     uint16_t message_id, int flags, const void *data,
                     size_t len) {
  size_t old_len = nc->send_mbuf.len;

  uint16_t topic_len = htons(strlen(topic));
  uint16_t message_id_net = htons(message_id);

  mg_send(nc, &topic_len, 2);
  mg_send(nc, topic, strlen(topic));
  if (MG_MQTT_GET_QOS(flags) > 0) {
    mg_send(nc, &message_id_net, 2);
  }
  mg_send(nc, data, len);

  mg_mqtt_prepend_header(nc, MG_MQTT_CMD_PUBLISH, flags,
                         nc->send_mbuf.len - old_len);
}

void mg_mqtt_subscribe(struct mg_connection *nc,
                       const struct mg_mqtt_topic_expression *topics,
                       size_t topics_len, uint16_t message_id) {
  size_t old_len = nc->send_mbuf.len;

  uint16_t message_id_n = htons(message_id);
  size_t i;

  mg_send(nc, (char *) &message_id_n, 2);
  for (i = 0; i < topics_len; i++) {
    uint16_t topic_len_n = htons(strlen(topics[i].topic));
    mg_send(nc, &topic_len_n, 2);
    mg_send(nc, topics[i].topic, strlen(topics[i].topic));
    mg_send(nc, &topics[i].qos, 1);
  }

  mg_mqtt_prepend_header(nc, MG_MQTT_CMD_SUBSCRIBE, MG_MQTT_QOS(1),
                         nc->send_mbuf.len - old_len);
}

int mg_mqtt_next_subscribe_topic(struct mg_mqtt_message *msg,
                                 struct mg_str *topic, uint8_t *qos, int pos) {
  unsigned char *buf = (unsigned char *) msg->payload.p + pos;
  if ((size_t) pos >= msg->payload.len) {
    return -1;
  }

  topic->len = buf[0] << 8 | buf[1];
  topic->p = (char *) buf + 2;
  *qos = buf[2 + topic->len];
  return pos + 2 + topic->len + 1;
}

void mg_mqtt_unsubscribe(struct mg_connection *nc, char **topics,
                         size_t topics_len, uint16_t message_id) {
  size_t old_len = nc->send_mbuf.len;

  uint16_t message_id_n = htons(message_id);
  size_t i;

  mg_send(nc, (char *) &message_id_n, 2);
  for (i = 0; i < topics_len; i++) {
    uint16_t topic_len_n = htons(strlen(topics[i]));
    mg_send(nc, &topic_len_n, 2);
    mg_send(nc, topics[i], strlen(topics[i]));
  }

  mg_mqtt_prepend_header(nc, MG_MQTT_CMD_UNSUBSCRIBE, MG_MQTT_QOS(1),
                         nc->send_mbuf.len - old_len);
}

void mg_mqtt_connack(struct mg_connection *nc, uint8_t return_code) {
  uint8_t unused = 0;
  mg_send(nc, &unused, 1);
  mg_send(nc, &return_code, 1);
  mg_mqtt_prepend_header(nc, MG_MQTT_CMD_CONNACK, 0, 2);
}

/*
 * Sends a command which contains only a `message_id` and a QoS level of 1.
 *
 * Helper function.
 */
static void mg_send_mqtt_short_command(struct mg_connection *nc, uint8_t cmd,
                                       uint16_t message_id) {
  uint16_t message_id_net = htons(message_id);
  mg_send(nc, &message_id_net, 2);
  mg_mqtt_prepend_header(nc, cmd, MG_MQTT_QOS(1), 2);
}

void mg_mqtt_puback(struct mg_connection *nc, uint16_t message_id) {
  mg_send_mqtt_short_command(nc, MG_MQTT_CMD_PUBACK, message_id);
}

void mg_mqtt_pubrec(struct mg_connection *nc, uint16_t message_id) {
  mg_send_mqtt_short_command(nc, MG_MQTT_CMD_PUBREC, message_id);
}

void mg_mqtt_pubrel(struct mg_connection *nc, uint16_t message_id) {
  mg_send_mqtt_short_command(nc, MG_MQTT_CMD_PUBREL, message_id);
}

void mg_mqtt_pubcomp(struct mg_connection *nc, uint16_t message_id) {
  mg_send_mqtt_short_command(nc, MG_MQTT_CMD_PUBCOMP, message_id);
}

void mg_mqtt_suback(struct mg_connection *nc, uint8_t *qoss, size_t qoss_len,
                    uint16_t message_id) {
  size_t i;
  uint16_t message_id_net = htons(message_id);
  mg_send(nc, &message_id_net, 2);
  for (i = 0; i < qoss_len; i++) {
    mg_send(nc, &qoss[i], 1);
  }
  mg_mqtt_prepend_header(nc, MG_MQTT_CMD_SUBACK, MG_MQTT_QOS(1), 2 + qoss_len);
}

void mg_mqtt_unsuback(struct mg_connection *nc, uint16_t message_id) {
  mg_send_mqtt_short_command(nc, MG_MQTT_CMD_UNSUBACK, message_id);
}

void mg_mqtt_ping(struct mg_connection *nc) {
  mg_mqtt_prepend_header(nc, MG_MQTT_CMD_PINGREQ, 0, 0);
}

void mg_mqtt_pong(struct mg_connection *nc) {
  mg_mqtt_prepend_header(nc, MG_MQTT_CMD_PINGRESP, 0, 0);
}

void mg_mqtt_disconnect(struct mg_connection *nc) {
  mg_mqtt_prepend_header(nc, MG_MQTT_CMD_DISCONNECT, 0, 0);
}

#endif /* MG_DISABLE_MQTT */
#ifdef MG_MODULE_LINES
#line 1 "./src/mqtt-broker.c"
#endif
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

/* Amalgamated: #include "mongoose/src/internal.h" */
/* Amalgamated: #include "mongoose/src/mqtt-broker.h" */

#ifdef MG_ENABLE_MQTT_BROKER

static void mg_mqtt_session_init(struct mg_mqtt_broker *brk,
                                 struct mg_mqtt_session *s,
                                 struct mg_connection *nc) {
  s->brk = brk;
  s->subscriptions = NULL;
  s->num_subscriptions = 0;
  s->nc = nc;
}

static void mg_mqtt_add_session(struct mg_mqtt_session *s) {
  s->next = s->brk->sessions;
  s->brk->sessions = s;
  s->prev = NULL;
  if (s->next != NULL) s->next->prev = s;
}

static void mg_mqtt_remove_session(struct mg_mqtt_session *s) {
  if (s->prev == NULL) s->brk->sessions = s->next;
  if (s->prev) s->prev->next = s->next;
  if (s->next) s->next->prev = s->prev;
}

static void mg_mqtt_destroy_session(struct mg_mqtt_session *s) {
  size_t i;
  for (i = 0; i < s->num_subscriptions; i++) {
    MG_FREE((void *) s->subscriptions[i].topic);
  }
  MG_FREE(s->subscriptions);
  MG_FREE(s);
}

static void mg_mqtt_close_session(struct mg_mqtt_session *s) {
  mg_mqtt_remove_session(s);
  mg_mqtt_destroy_session(s);
}

void mg_mqtt_broker_init(struct mg_mqtt_broker *brk, void *user_data) {
  brk->sessions = NULL;
  brk->user_data = user_data;
}

static void mg_mqtt_broker_handle_connect(struct mg_mqtt_broker *brk,
                                          struct mg_connection *nc) {
  struct mg_mqtt_session *s = (struct mg_mqtt_session *) malloc(sizeof *s);
  if (s == NULL) {
    /* LCOV_EXCL_START */
    mg_mqtt_connack(nc, MG_EV_MQTT_CONNACK_SERVER_UNAVAILABLE);
    return;
    /* LCOV_EXCL_STOP */
  }

  /* TODO(mkm): check header (magic and version) */

  mg_mqtt_session_init(brk, s, nc);
  s->user_data = nc->user_data;
  nc->user_data = s;
  mg_mqtt_add_session(s);

  mg_mqtt_connack(nc, MG_EV_MQTT_CONNACK_ACCEPTED);
}

static void mg_mqtt_broker_handle_subscribe(struct mg_connection *nc,
                                            struct mg_mqtt_message *msg) {
  struct mg_mqtt_session *ss = (struct mg_mqtt_session *) nc->user_data;
  uint8_t qoss[512];
  size_t qoss_len = 0;
  struct mg_str topic;
  uint8_t qos;
  int pos;
  struct mg_mqtt_topic_expression *te;

  for (pos = 0;
       (pos = mg_mqtt_next_subscribe_topic(msg, &topic, &qos, pos)) != -1;) {
    qoss[qoss_len++] = qos;
  }

  ss->subscriptions = (struct mg_mqtt_topic_expression *) realloc(
      ss->subscriptions, sizeof(*ss->subscriptions) * qoss_len);
  for (pos = 0;
       (pos = mg_mqtt_next_subscribe_topic(msg, &topic, &qos, pos)) != -1;
       ss->num_subscriptions++) {
    te = &ss->subscriptions[ss->num_subscriptions];
    te->topic = (char *) malloc(topic.len + 1);
    te->qos = qos;
    strncpy((char *) te->topic, topic.p, topic.len + 1);
  }

  mg_mqtt_suback(nc, qoss, qoss_len, msg->message_id);
}

/*
 * Matches a topic against a topic expression
 *
 * See http://goo.gl/iWk21X
 *
 * Returns 1 if it matches; 0 otherwise.
 */
static int mg_mqtt_match_topic_expression(const char *exp, const char *topic) {
  /* TODO(mkm): implement real matching */
  int len = strlen(exp);
  if (strchr(exp, '#')) {
    len -= 2;
  }
  return strncmp(exp, topic, len) == 0;
}

static void mg_mqtt_broker_handle_publish(struct mg_mqtt_broker *brk,
                                          struct mg_mqtt_message *msg) {
  struct mg_mqtt_session *s;
  size_t i;

  for (s = mg_mqtt_next(brk, NULL); s != NULL; s = mg_mqtt_next(brk, s)) {
    for (i = 0; i < s->num_subscriptions; i++) {
      if (mg_mqtt_match_topic_expression(s->subscriptions[i].topic,
                                         msg->topic)) {
        mg_mqtt_publish(s->nc, msg->topic, 0, 0, msg->payload.p,
                        msg->payload.len);
        break;
      }
    }
  }
}

void mg_mqtt_broker(struct mg_connection *nc, int ev, void *data) {
  struct mg_mqtt_message *msg = (struct mg_mqtt_message *) data;
  struct mg_mqtt_broker *brk;

  if (nc->listener) {
    brk = (struct mg_mqtt_broker *) nc->listener->user_data;
  } else {
    brk = (struct mg_mqtt_broker *) nc->user_data;
  }

  switch (ev) {
    case MG_EV_ACCEPT:
      mg_set_protocol_mqtt(nc);
      break;
    case MG_EV_MQTT_CONNECT:
      mg_mqtt_broker_handle_connect(brk, nc);
      break;
    case MG_EV_MQTT_SUBSCRIBE:
      mg_mqtt_broker_handle_subscribe(nc, msg);
      break;
    case MG_EV_MQTT_PUBLISH:
      mg_mqtt_broker_handle_publish(brk, msg);
      break;
    case MG_EV_CLOSE:
      if (nc->listener) {
        mg_mqtt_close_session((struct mg_mqtt_session *) nc->user_data);
      }
      break;
  }
}

struct mg_mqtt_session *mg_mqtt_next(struct mg_mqtt_broker *brk,
                                     struct mg_mqtt_session *s) {
  return s == NULL ? brk->sessions : s->next;
}

#endif /* MG_ENABLE_MQTT_BROKER */
#ifdef MG_MODULE_LINES
#line 1 "./src/dns.c"
#endif
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#ifndef MG_DISABLE_DNS

/* Amalgamated: #include "mongoose/src/internal.h" */
/* Amalgamated: #include "mongoose/src/dns.h" */

static int mg_dns_tid = 0xa0;

struct mg_dns_header {
  uint16_t transaction_id;
  uint16_t flags;
  uint16_t num_questions;
  uint16_t num_answers;
  uint16_t num_authority_prs;
  uint16_t num_other_prs;
};

struct mg_dns_resource_record *mg_dns_next_record(
    struct mg_dns_message *msg, int query,
    struct mg_dns_resource_record *prev) {
  struct mg_dns_resource_record *rr;

  for (rr = (prev == NULL ? msg->answers : prev + 1);
       rr - msg->answers < msg->num_answers; rr++) {
    if (rr->rtype == query) {
      return rr;
    }
  }
  return NULL;
}

int mg_dns_parse_record_data(struct mg_dns_message *msg,
                             struct mg_dns_resource_record *rr, void *data,
                             size_t data_len) {
  switch (rr->rtype) {
    case MG_DNS_A_RECORD:
      if (data_len < sizeof(struct in_addr)) {
        return -1;
      }
      if (rr->rdata.p + data_len > msg->pkt.p + msg->pkt.len) {
        return -1;
      }
      memcpy(data, rr->rdata.p, data_len);
      return 0;
#ifdef MG_ENABLE_IPV6
    case MG_DNS_AAAA_RECORD:
      if (data_len < sizeof(struct in6_addr)) {
        return -1; /* LCOV_EXCL_LINE */
      }
      memcpy(data, rr->rdata.p, data_len);
      return 0;
#endif
    case MG_DNS_CNAME_RECORD:
      mg_dns_uncompress_name(msg, &rr->rdata, (char *) data, data_len);
      return 0;
  }

  return -1;
}

int mg_dns_insert_header(struct mbuf *io, size_t pos,
                         struct mg_dns_message *msg) {
  struct mg_dns_header header;

  memset(&header, 0, sizeof(header));
  header.transaction_id = msg->transaction_id;
  header.flags = htons(msg->flags);
  header.num_questions = htons(msg->num_questions);
  header.num_answers = htons(msg->num_answers);

  return mbuf_insert(io, pos, &header, sizeof(header));
}

int mg_dns_copy_questions(struct mbuf *io, struct mg_dns_message *msg) {
  unsigned char *begin, *end;
  struct mg_dns_resource_record *last_q;
  if (msg->num_questions <= 0) return 0;
  begin = (unsigned char *) msg->pkt.p + sizeof(struct mg_dns_header);
  last_q = &msg->questions[msg->num_questions - 1];
  end = (unsigned char *) last_q->name.p + last_q->name.len + 4;
  return mbuf_append(io, begin, end - begin);
}

static int mg_dns_encode_name(struct mbuf *io, const char *name, size_t len) {
  const char *s;
  unsigned char n;
  size_t pos = io->len;

  do {
    if ((s = strchr(name, '.')) == NULL) {
      s = name + len;
    }

    if (s - name > 127) {
      return -1; /* TODO(mkm) cover */
    }
    n = s - name;           /* chunk length */
    mbuf_append(io, &n, 1); /* send length */
    mbuf_append(io, name, n);

    if (*s == '.') {
      n++;
    }

    name += n;
    len -= n;
  } while (*s != '\0');
  mbuf_append(io, "\0", 1); /* Mark end of host name */

  return io->len - pos;
}

int mg_dns_encode_record(struct mbuf *io, struct mg_dns_resource_record *rr,
                         const char *name, size_t nlen, const void *rdata,
                         size_t rlen) {
  size_t pos = io->len;
  uint16_t u16;
  uint32_t u32;

  if (rr->kind == MG_DNS_INVALID_RECORD) {
    return -1; /* LCOV_EXCL_LINE */
  }

  if (mg_dns_encode_name(io, name, nlen) == -1) {
    return -1;
  }

  u16 = htons(rr->rtype);
  mbuf_append(io, &u16, 2);
  u16 = htons(rr->rclass);
  mbuf_append(io, &u16, 2);

  if (rr->kind == MG_DNS_ANSWER) {
    u32 = htonl(rr->ttl);
    mbuf_append(io, &u32, 4);

    if (rr->rtype == MG_DNS_CNAME_RECORD) {
      int clen;
      /* fill size after encoding */
      size_t off = io->len;
      mbuf_append(io, &u16, 2);
      if ((clen = mg_dns_encode_name(io, (const char *) rdata, rlen)) == -1) {
        return -1;
      }
      u16 = clen;
      io->buf[off] = u16 >> 8;
      io->buf[off + 1] = u16 & 0xff;
    } else {
      u16 = htons(rlen);
      mbuf_append(io, &u16, 2);
      mbuf_append(io, rdata, rlen);
    }
  }

  return io->len - pos;
}

void mg_send_dns_query(struct mg_connection *nc, const char *name,
                       int query_type) {
  struct mg_dns_message *msg =
      (struct mg_dns_message *) MG_CALLOC(1, sizeof(*msg));
  struct mbuf pkt;
  struct mg_dns_resource_record *rr = &msg->questions[0];

  DBG(("%s %d", name, query_type));

  mbuf_init(&pkt, 64 /* Start small, it'll grow as needed. */);

  msg->transaction_id = ++mg_dns_tid;
  msg->flags = 0x100;
  msg->num_questions = 1;

  mg_dns_insert_header(&pkt, 0, msg);

  rr->rtype = query_type;
  rr->rclass = 1; /* Class: inet */
  rr->kind = MG_DNS_QUESTION;

  if (mg_dns_encode_record(&pkt, rr, name, strlen(name), NULL, 0) == -1) {
    /* TODO(mkm): return an error code */
    goto cleanup; /* LCOV_EXCL_LINE */
  }

  /* TCP DNS requires messages to be prefixed with len */
  if (!(nc->flags & MG_F_UDP)) {
    uint16_t len = htons(pkt.len);
    mbuf_insert(&pkt, 0, &len, 2);
  }

  mg_send(nc, pkt.buf, pkt.len);
  mbuf_free(&pkt);

cleanup:
  MG_FREE(msg);
}

static unsigned char *mg_parse_dns_resource_record(
    unsigned char *data, unsigned char *end, struct mg_dns_resource_record *rr,
    int reply) {
  unsigned char *name = data;
  int chunk_len, data_len;

  while (data < end && (chunk_len = *data)) {
    if (((unsigned char *) data)[0] & 0xc0) {
      data += 1;
      break;
    }
    data += chunk_len + 1;
  }

  if (data > end - 5) {
    return NULL;
  }

  rr->name.p = (char *) name;
  rr->name.len = data - name + 1;
  data++;

  rr->rtype = data[0] << 8 | data[1];
  data += 2;

  rr->rclass = data[0] << 8 | data[1];
  data += 2;

  rr->kind = reply ? MG_DNS_ANSWER : MG_DNS_QUESTION;
  if (reply) {
    if (data >= end - 6) {
      return NULL;
    }

    rr->ttl = (uint32_t) data[0] << 24 | (uint32_t) data[1] << 16 |
              data[2] << 8 | data[3];
    data += 4;

    data_len = *data << 8 | *(data + 1);
    data += 2;

    rr->rdata.p = (char *) data;
    rr->rdata.len = data_len;
    data += data_len;
  }
  return data;
}

int mg_parse_dns(const char *buf, int len, struct mg_dns_message *msg) {
  struct mg_dns_header *header = (struct mg_dns_header *) buf;
  unsigned char *data = (unsigned char *) buf + sizeof(*header);
  unsigned char *end = (unsigned char *) buf + len;
  int i;

  memset(msg, 0, sizeof(*msg));
  msg->pkt.p = buf;
  msg->pkt.len = len;

  if (len < (int) sizeof(*header)) return -1;

  msg->transaction_id = header->transaction_id;
  msg->flags = ntohs(header->flags);
  msg->num_questions = ntohs(header->num_questions);
  if (msg->num_questions > (int) ARRAY_SIZE(msg->questions)) {
    msg->num_questions = (int) ARRAY_SIZE(msg->questions);
  }
  msg->num_answers = ntohs(header->num_answers);
  if (msg->num_answers > (int) ARRAY_SIZE(msg->answers)) {
    msg->num_answers = (int) ARRAY_SIZE(msg->answers);
  }

  for (i = 0; i < msg->num_questions; i++) {
    data = mg_parse_dns_resource_record(data, end, &msg->questions[i], 0);
    if (data == NULL) return -1;
  }

  for (i = 0; i < msg->num_answers; i++) {
    data = mg_parse_dns_resource_record(data, end, &msg->answers[i], 1);
    if (data == NULL) return -1;
  }

  return 0;
}

size_t mg_dns_uncompress_name(struct mg_dns_message *msg, struct mg_str *name,
                              char *dst, int dst_len) {
  int chunk_len;
  char *old_dst = dst;
  const unsigned char *data = (unsigned char *) name->p;
  const unsigned char *end = (unsigned char *) msg->pkt.p + msg->pkt.len;

  if (data >= end) {
    return 0;
  }

  while ((chunk_len = *data++)) {
    int leeway = dst_len - (dst - old_dst);
    if (data >= end) {
      return 0;
    }

    if (chunk_len & 0xc0) {
      uint16_t off = (data[-1] & (~0xc0)) << 8 | data[0];
      if (off >= msg->pkt.len) {
        return 0;
      }
      data = (unsigned char *) msg->pkt.p + off;
      continue;
    }
    if (chunk_len > leeway) {
      chunk_len = leeway;
    }

    if (data + chunk_len >= end) {
      return 0;
    }

    memcpy(dst, data, chunk_len);
    data += chunk_len;
    dst += chunk_len;
    leeway -= chunk_len;
    if (leeway == 0) {
      return dst - old_dst;
    }
    *dst++ = '.';
  }

  if (dst != old_dst) {
    *--dst = 0;
  }
  return dst - old_dst;
}

static void dns_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct mbuf *io = &nc->recv_mbuf;
  struct mg_dns_message msg;

  /* Pass low-level events to the user handler */
  nc->handler(nc, ev, ev_data);

  switch (ev) {
    case MG_EV_RECV:
      if (!(nc->flags & MG_F_UDP)) {
        mbuf_remove(&nc->recv_mbuf, 2);
      }
      if (mg_parse_dns(nc->recv_mbuf.buf, nc->recv_mbuf.len, &msg) == -1) {
        /* reply + recursion allowed + format error */
        memset(&msg, 0, sizeof(msg));
        msg.flags = 0x8081;
        mg_dns_insert_header(io, 0, &msg);
        if (!(nc->flags & MG_F_UDP)) {
          uint16_t len = htons(io->len);
          mbuf_insert(io, 0, &len, 2);
        }
        mg_send(nc, io->buf, io->len);
      } else {
        /* Call user handler with parsed message */
        nc->handler(nc, MG_DNS_MESSAGE, &msg);
      }
      mbuf_remove(io, io->len);
      break;
  }
}

void mg_set_protocol_dns(struct mg_connection *nc) {
  nc->proto_handler = dns_handler;
}

#endif /* MG_DISABLE_DNS */
#ifdef MG_MODULE_LINES
#line 1 "./src/dns-server.c"
#endif
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#ifdef MG_ENABLE_DNS_SERVER

/* Amalgamated: #include "mongoose/src/internal.h" */
/* Amalgamated: #include "mongoose/src/dns-server.h" */

struct mg_dns_reply mg_dns_create_reply(struct mbuf *io,
                                        struct mg_dns_message *msg) {
  struct mg_dns_reply rep;
  rep.msg = msg;
  rep.io = io;
  rep.start = io->len;

  /* reply + recursion allowed */
  msg->flags |= 0x8080;
  mg_dns_copy_questions(io, msg);

  msg->num_answers = 0;
  return rep;
}

void mg_dns_send_reply(struct mg_connection *nc, struct mg_dns_reply *r) {
  size_t sent = r->io->len - r->start;
  mg_dns_insert_header(r->io, r->start, r->msg);
  if (!(nc->flags & MG_F_UDP)) {
    uint16_t len = htons(sent);
    mbuf_insert(r->io, r->start, &len, 2);
  }

  if (&nc->send_mbuf != r->io) {
    mg_send(nc, r->io->buf + r->start, r->io->len - r->start);
    r->io->len = r->start;
  }
}

int mg_dns_reply_record(struct mg_dns_reply *reply,
                        struct mg_dns_resource_record *question,
                        const char *name, int rtype, int ttl, const void *rdata,
                        size_t rdata_len) {
  struct mg_dns_message *msg = (struct mg_dns_message *) reply->msg;
  char rname[512];
  struct mg_dns_resource_record *ans = &msg->answers[msg->num_answers];
  if (msg->num_answers >= MG_MAX_DNS_ANSWERS) {
    return -1; /* LCOV_EXCL_LINE */
  }

  if (name == NULL) {
    name = rname;
    rname[511] = 0;
    mg_dns_uncompress_name(msg, &question->name, rname, sizeof(rname) - 1);
  }

  *ans = *question;
  ans->kind = MG_DNS_ANSWER;
  ans->rtype = rtype;
  ans->ttl = ttl;

  if (mg_dns_encode_record(reply->io, ans, name, strlen(name), rdata,
                           rdata_len) == -1) {
    return -1; /* LCOV_EXCL_LINE */
  };

  msg->num_answers++;
  return 0;
}

#endif /* MG_ENABLE_DNS_SERVER */
#ifdef MG_MODULE_LINES
#line 1 "./src/resolv.c"
#endif
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#ifndef MG_DISABLE_RESOLVER

/* Amalgamated: #include "mongoose/src/internal.h" */
/* Amalgamated: #include "mongoose/src/resolv.h" */

#ifndef MG_DEFAULT_NAMESERVER
#define MG_DEFAULT_NAMESERVER "8.8.8.8"
#endif

static const char *mg_default_dns_server = "udp://" MG_DEFAULT_NAMESERVER ":53";

MG_INTERNAL char mg_dns_server[256];

struct mg_resolve_async_request {
  char name[1024];
  int query;
  mg_resolve_callback_t callback;
  void *data;
  time_t timeout;
  int max_retries;
  enum mg_resolve_err err;

  /* state */
  time_t last_time;
  int retries;
};

/*
 * Find what nameserver to use.
 *
 * Return 0 if OK, -1 if error
 */
static int mg_get_ip_address_of_nameserver(char *name, size_t name_len) {
  int ret = -1;

#ifdef _WIN32
  int i;
  LONG err;
  HKEY hKey, hSub;
  char subkey[512], value[128],
      *key = "SYSTEM\\ControlSet001\\Services\\Tcpip\\Parameters\\Interfaces";

  if ((err = RegOpenKeyA(HKEY_LOCAL_MACHINE, key, &hKey)) != ERROR_SUCCESS) {
    fprintf(stderr, "cannot open reg key %s: %d\n", key, err);
    ret = -1;
  } else {
    for (ret = -1, i = 0;
         RegEnumKeyA(hKey, i, subkey, sizeof(subkey)) == ERROR_SUCCESS; i++) {
      DWORD type, len = sizeof(value);
      if (RegOpenKeyA(hKey, subkey, &hSub) == ERROR_SUCCESS &&
          (RegQueryValueExA(hSub, "NameServer", 0, &type, (void *) value,
                            &len) == ERROR_SUCCESS ||
           RegQueryValueExA(hSub, "DhcpNameServer", 0, &type, (void *) value,
                            &len) == ERROR_SUCCESS)) {
        /*
         * See https://github.com/cesanta/mongoose/issues/176
         * The value taken from the registry can be empty, a single
         * IP address, or multiple IP addresses separated by comma.
         * If it's empty, check the next interface.
         * If it's multiple IP addresses, take the first one.
         */
        char *comma = strchr(value, ',');
        if (value[0] == '\0') {
          continue;
        }
        if (comma != NULL) {
          *comma = '\0';
        }
        snprintf(name, name_len, "udp://%s:53", value);
        ret = 0;
        RegCloseKey(hSub);
        break;
      }
    }
    RegCloseKey(hKey);
  }
#elif !defined(MG_DISABLE_FILESYSTEM)
  FILE *fp;
  char line[512];

  if ((fp = fopen("/etc/resolv.conf", "r")) == NULL) {
    ret = -1;
  } else {
    /* Try to figure out what nameserver to use */
    for (ret = -1; fgets(line, sizeof(line), fp) != NULL;) {
      char buf[256];
      if (sscanf(line, "nameserver %255[^\n\t #]s", buf) == 1) {
        snprintf(name, name_len, "udp://%s:53", buf);
        ret = 0;
        break;
      }
    }
    (void) fclose(fp);
  }
#else
  snprintf(name, name_len, "%s", mg_default_dns_server);
#endif /* _WIN32 */

  return ret;
}

int mg_resolve_from_hosts_file(const char *name, union socket_address *usa) {
#ifndef MG_DISABLE_FILESYSTEM
  /* TODO(mkm) cache /etc/hosts */
  FILE *fp;
  char line[1024];
  char *p;
  char alias[256];
  unsigned int a, b, c, d;
  int len = 0;

  if ((fp = fopen("/etc/hosts", "r")) == NULL) {
    return -1;
  }

  for (; fgets(line, sizeof(line), fp) != NULL;) {
    if (line[0] == '#') continue;

    if (sscanf(line, "%u.%u.%u.%u%n", &a, &b, &c, &d, &len) == 0) {
      /* TODO(mkm): handle ipv6 */
      continue;
    }
    for (p = line + len; sscanf(p, "%s%n", alias, &len) == 1; p += len) {
      if (strcmp(alias, name) == 0) {
        usa->sin.sin_addr.s_addr = htonl(a << 24 | b << 16 | c << 8 | d);
        fclose(fp);
        return 0;
      }
    }
  }

  fclose(fp);
#endif

  return -1;
}

static void mg_resolve_async_eh(struct mg_connection *nc, int ev, void *data) {
  time_t now = time(NULL);
  struct mg_resolve_async_request *req;
  struct mg_dns_message *msg;

  DBG(("ev=%d user_data=%p", ev, nc->user_data));

  req = (struct mg_resolve_async_request *) nc->user_data;

  if (req == NULL) {
    return;
  }

  switch (ev) {
    case MG_EV_CONNECT:
    case MG_EV_POLL:
      if (req->retries > req->max_retries) {
        req->err = MG_RESOLVE_EXCEEDED_RETRY_COUNT;
        nc->flags |= MG_F_CLOSE_IMMEDIATELY;
        break;
      }
      if (now - req->last_time >= req->timeout) {
        mg_send_dns_query(nc, req->name, req->query);
        req->last_time = now;
        req->retries++;
      }
      break;
    case MG_EV_RECV:
      msg = (struct mg_dns_message *) MG_MALLOC(sizeof(*msg));
      if (mg_parse_dns(nc->recv_mbuf.buf, *(int *) data, msg) == 0 &&
          msg->num_answers > 0) {
        req->callback(msg, req->data, MG_RESOLVE_OK);
        nc->user_data = NULL;
        MG_FREE(req);
      } else {
        req->err = MG_RESOLVE_NO_ANSWERS;
      }
      MG_FREE(msg);
      nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      break;
    case MG_EV_SEND:
      /*
       * If a send error occurs, prevent closing of the connection by the core.
       * We will retry after timeout.
       */
      nc->flags &= ~MG_F_CLOSE_IMMEDIATELY;
      mbuf_remove(&nc->send_mbuf, nc->send_mbuf.len);
      break;
    case MG_EV_TIMER:
      req->err = MG_RESOLVE_TIMEOUT;
      nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      break;
    case MG_EV_CLOSE:
      /* If we got here with request still not done, fire an error callback. */
      if (req != NULL) {
        req->callback(NULL, req->data, req->err);
        nc->user_data = NULL;
        MG_FREE(req);
      }
      break;
  }
}

int mg_resolve_async(struct mg_mgr *mgr, const char *name, int query,
                     mg_resolve_callback_t cb, void *data) {
  struct mg_resolve_async_opts opts;
  memset(&opts, 0, sizeof(opts));
  return mg_resolve_async_opt(mgr, name, query, cb, data, opts);
}

int mg_resolve_async_opt(struct mg_mgr *mgr, const char *name, int query,
                         mg_resolve_callback_t cb, void *data,
                         struct mg_resolve_async_opts opts) {
  struct mg_resolve_async_request *req;
  struct mg_connection *dns_nc;
  const char *nameserver = opts.nameserver_url;

  DBG(("%s %d %p", name, query, opts.dns_conn));

  /* resolve with DNS */
  req = (struct mg_resolve_async_request *) MG_CALLOC(1, sizeof(*req));
  if (req == NULL) {
    return -1;
  }

  strncpy(req->name, name, sizeof(req->name));
  req->query = query;
  req->callback = cb;
  req->data = data;
  /* TODO(mkm): parse defaults out of resolve.conf */
  req->max_retries = opts.max_retries ? opts.max_retries : 2;
  req->timeout = opts.timeout ? opts.timeout : 5;

  /* Lazily initialize dns server */
  if (nameserver == NULL && mg_dns_server[0] == '\0' &&
      mg_get_ip_address_of_nameserver(mg_dns_server, sizeof(mg_dns_server)) ==
          -1) {
    strncpy(mg_dns_server, mg_default_dns_server, sizeof(mg_dns_server));
  }

  if (nameserver == NULL) {
    nameserver = mg_dns_server;
  }

  dns_nc = mg_connect(mgr, nameserver, mg_resolve_async_eh);
  if (dns_nc == NULL) {
    free(req);
    return -1;
  }
  dns_nc->user_data = req;
  if (opts.dns_conn != NULL) {
    *opts.dns_conn = dns_nc;
  }

  return 0;
}

#endif /* MG_DISABLE_RESOLVE */
#ifdef MG_MODULE_LINES
#line 1 "./src/coap.c"
#endif
/*
 * Copyright (c) 2015 Cesanta Software Limited
 * All rights reserved
 * This software is dual-licensed: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. For the terms of this
 * license, see <http://www.gnu.org/licenses/>.
 *
 * You are free to use this software under the terms of the GNU General
 * Public License, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * Alternatively, you can license this software under a commercial
 * license, as set out in <https://www.cesanta.com/license>.
 */

/* Amalgamated: #include "mongoose/src/internal.h" */
/* Amalgamated: #include "mongoose/src/coap.h" */

#ifdef MG_ENABLE_COAP

void mg_coap_free_options(struct mg_coap_message *cm) {
  while (cm->options != NULL) {
    struct mg_coap_option *next = cm->options->next;
    MG_FREE(cm->options);
    cm->options = next;
  }
}

struct mg_coap_option *mg_coap_add_option(struct mg_coap_message *cm,
                                          uint32_t number, char *value,
                                          size_t len) {
  struct mg_coap_option *new_option =
      (struct mg_coap_option *) MG_CALLOC(1, sizeof(*new_option));

  new_option->number = number;
  new_option->value.p = value;
  new_option->value.len = len;

  if (cm->options == NULL) {
    cm->options = cm->optiomg_tail = new_option;
  } else {
    /*
     * A very simple attention to help clients to compose options:
     * CoAP wants to see options ASC ordered.
     * Could be change by using sort in coap_compose
     */
    if (cm->optiomg_tail->number <= new_option->number) {
      /* if option is already ordered just add it */
      cm->optiomg_tail = cm->optiomg_tail->next = new_option;
    } else {
      /* looking for appropriate position */
      struct mg_coap_option *current_opt = cm->options;
      struct mg_coap_option *prev_opt = 0;

      while (current_opt != NULL) {
        if (current_opt->number > new_option->number) {
          break;
        }
        prev_opt = current_opt;
        current_opt = current_opt->next;
      }

      if (prev_opt != NULL) {
        prev_opt->next = new_option;
        new_option->next = current_opt;
      } else {
        /* insert new_option to the beginning */
        new_option->next = cm->options;
        cm->options = new_option;
      }
    }
  }

  return new_option;
}

/*
 * Fills CoAP header in mg_coap_message.
 *
 * Helper function.
 */
static char *coap_parse_header(char *ptr, struct mbuf *io,
                               struct mg_coap_message *cm) {
  if (io->len < sizeof(uint32_t)) {
    cm->flags |= MG_COAP_NOT_ENOUGH_DATA;
    return NULL;
  }

  /*
   * Version (Ver):  2-bit unsigned integer.  Indicates the CoAP version
   * number.  Implementations of this specification MUST set this field
   * to 1 (01 binary).  Other values are reserved for future versions.
   * Messages with unknown version numbers MUST be silently ignored.
   */
  if (((uint8_t) *ptr >> 6) != 1) {
    cm->flags |= MG_COAP_IGNORE;
    return NULL;
  }

  /*
   * Type (T):  2-bit unsigned integer.  Indicates if this message is of
   * type Confirmable (0), Non-confirmable (1), Acknowledgement (2), or
   * Reset (3).
   */
  cm->msg_type = ((uint8_t) *ptr & 0x30) >> 4;
  cm->flags |= MG_COAP_MSG_TYPE_FIELD;

  /*
   * Token Length (TKL):  4-bit unsigned integer.  Indicates the length of
   * the variable-length Token field (0-8 bytes).  Lengths 9-15 are
   * reserved, MUST NOT be sent, and MUST be processed as a message
   * format error.
   */
  cm->token.len = *ptr & 0x0F;
  if (cm->token.len > 8) {
    cm->flags |= MG_COAP_FORMAT_ERROR;
    return NULL;
  }

  ptr++;

  /*
   * Code:  8-bit unsigned integer, split into a 3-bit class (most
   * significant bits) and a 5-bit detail (least significant bits)
   */
  cm->code_class = (uint8_t) *ptr >> 5;
  cm->code_detail = *ptr & 0x1F;
  cm->flags |= (MG_COAP_CODE_CLASS_FIELD | MG_COAP_CODE_DETAIL_FIELD);

  ptr++;

  /* Message ID:  16-bit unsigned integer in network byte order. */
  cm->msg_id = (uint8_t) *ptr << 8 | (uint8_t) * (ptr + 1);
  cm->flags |= MG_COAP_MSG_ID_FIELD;

  ptr += 2;

  return ptr;
}

/*
 * Fills token information in mg_coap_message.
 *
 * Helper function.
 */
static char *coap_get_token(char *ptr, struct mbuf *io,
                            struct mg_coap_message *cm) {
  if (cm->token.len != 0) {
    if (ptr + cm->token.len > io->buf + io->len) {
      cm->flags |= MG_COAP_NOT_ENOUGH_DATA;
      return NULL;
    } else {
      cm->token.p = ptr;
      ptr += cm->token.len;
      cm->flags |= MG_COAP_TOKEN_FIELD;
    }
  }

  return ptr;
}

/*
 * Returns Option Delta or Length.
 *
 * Helper function.
 */
static int coap_get_ext_opt(char *ptr, struct mbuf *io, uint16_t *opt_info) {
  int ret = 0;

  if (*opt_info == 13) {
    /*
     * 13:  An 8-bit unsigned integer follows the initial byte and
     * indicates the Option Delta/Length minus 13.
     */
    if (ptr < io->buf + io->len) {
      *opt_info = (uint8_t) *ptr + 13;
      ret = sizeof(uint8_t);
    } else {
      ret = -1; /* LCOV_EXCL_LINE */
    }
  } else if (*opt_info == 14) {
    /*
     * 14:  A 16-bit unsigned integer in network byte order follows the
     * initial byte and indicates the Option Delta/Length minus 269.
     */
    if (ptr + sizeof(uint8_t) < io->buf + io->len) {
      *opt_info = ((uint8_t) *ptr << 8 | (uint8_t) * (ptr + 1)) + 269;
      ret = sizeof(uint16_t);
    } else {
      ret = -1; /* LCOV_EXCL_LINE */
    }
  }

  return ret;
}

/*
 * Fills options in mg_coap_message.
 *
 * Helper function.
 *
 * General options format:
 * +---------------+---------------+
 * | Option Delta  | Option Length |  1 byte
 * +---------------+---------------+
 * \    Option Delta (extended)    \  0-2 bytes
 * +-------------------------------+
 * / Option Length  (extended)     \  0-2 bytes
 * +-------------------------------+
 * \         Option Value          \  0 or more bytes
 * +-------------------------------+
 */
static char *coap_get_options(char *ptr, struct mbuf *io,
                              struct mg_coap_message *cm) {
  uint16_t prev_opt = 0;

  if (ptr == io->buf + io->len) {
    /* end of packet, ok */
    return NULL;
  }

  /* 0xFF is payload marker */
  while (ptr < io->buf + io->len && (uint8_t) *ptr != 0xFF) {
    uint16_t option_delta, option_lenght;
    int optinfo_len;

    /* Option Delta:  4-bit unsigned integer */
    option_delta = ((uint8_t) *ptr & 0xF0) >> 4;
    /* Option Length:  4-bit unsigned integer */
    option_lenght = *ptr & 0x0F;

    if (option_delta == 15 || option_lenght == 15) {
      /*
       * 15:  Reserved for future use.  If the field is set to this value,
       * it MUST be processed as a message format error
       */
      cm->flags |= MG_COAP_FORMAT_ERROR;
      break;
    }

    ptr++;

    /* check for extended option delta */
    optinfo_len = coap_get_ext_opt(ptr, io, &option_delta);
    if (optinfo_len == -1) {
      cm->flags |= MG_COAP_NOT_ENOUGH_DATA; /* LCOV_EXCL_LINE */
      break;                                /* LCOV_EXCL_LINE */
    }

    ptr += optinfo_len;

    /* check or extended option lenght */
    optinfo_len = coap_get_ext_opt(ptr, io, &option_lenght);
    if (optinfo_len == -1) {
      cm->flags |= MG_COAP_NOT_ENOUGH_DATA; /* LCOV_EXCL_LINE */
      break;                                /* LCOV_EXCL_LINE */
    }

    ptr += optinfo_len;

    /*
     * Instead of specifying the Option Number directly, the instances MUST
     * appear in order of their Option Numbers and a delta encoding is used
     * between them.
     */
    option_delta += prev_opt;

    mg_coap_add_option(cm, option_delta, ptr, option_lenght);

    prev_opt = option_delta;

    if (ptr + option_lenght > io->buf + io->len) {
      cm->flags |= MG_COAP_NOT_ENOUGH_DATA; /* LCOV_EXCL_LINE */
      break;                                /* LCOV_EXCL_LINE */
    }

    ptr += option_lenght;
  }

  if ((cm->flags & MG_COAP_ERROR) != 0) {
    mg_coap_free_options(cm);
    return NULL;
  }

  cm->flags |= MG_COAP_OPTIOMG_FIELD;

  if (ptr == io->buf + io->len) {
    /* end of packet, ok */
    return NULL;
  }

  ptr++;

  return ptr;
}

uint32_t mg_coap_parse(struct mbuf *io, struct mg_coap_message *cm) {
  char *ptr;

  memset(cm, 0, sizeof(*cm));

  if ((ptr = coap_parse_header(io->buf, io, cm)) == NULL) {
    return cm->flags;
  }

  if ((ptr = coap_get_token(ptr, io, cm)) == NULL) {
    return cm->flags;
  }

  if ((ptr = coap_get_options(ptr, io, cm)) == NULL) {
    return cm->flags;
  }

  /* the rest is payload */
  cm->payload.len = io->len - (ptr - io->buf);
  if (cm->payload.len != 0) {
    cm->payload.p = ptr;
    cm->flags |= MG_COAP_PAYLOAD_FIELD;
  }

  return cm->flags;
}

/*
 * Calculates extended size of given Opt Number/Length in coap message.
 *
 * Helper function.
 */
static size_t coap_get_ext_opt_size(uint32_t value) {
  int ret = 0;

  if (value >= 13 && value <= 0xFF + 13) {
    ret = sizeof(uint8_t);
  } else if (value > 0xFF + 13 && value <= 0xFFFF + 269) {
    ret = sizeof(uint16_t);
  }

  return ret;
}

/*
 * Splits given Opt Number/Length into base and ext values.
 *
 * Helper function.
 */
static int coap_split_opt(uint32_t value, uint8_t *base, uint16_t *ext) {
  int ret = 0;

  if (value < 13) {
    *base = value;
  } else if (value >= 13 && value <= 0xFF + 13) {
    *base = 13;
    *ext = value - 13;
    ret = sizeof(uint8_t);
  } else if (value > 0xFF + 13 && value <= 0xFFFF + 269) {
    *base = 14;
    *ext = value - 269;
    ret = sizeof(uint16_t);
  }

  return ret;
}

/*
 * Puts uint16_t (in network order) into given char stream.
 *
 * Helper function.
 */
static char *coap_add_uint16(char *ptr, uint16_t val) {
  *ptr = val >> 8;
  ptr++;
  *ptr = val & 0x00FF;
  ptr++;
  return ptr;
}

/*
 * Puts extended value of Opt Number/Length into given char stream.
 *
 * Helper function.
 */
static char *coap_add_opt_info(char *ptr, uint16_t val, size_t len) {
  if (len == sizeof(uint8_t)) {
    *ptr = val;
    ptr++;
  } else if (len == sizeof(uint16_t)) {
    ptr = coap_add_uint16(ptr, val);
  }

  return ptr;
}

/*
 * Verifies given mg_coap_message and calculates message size for it.
 *
 * Helper function.
 */
static uint32_t coap_calculate_packet_size(struct mg_coap_message *cm,
                                           size_t *len) {
  struct mg_coap_option *opt;
  uint32_t prev_opt_number;

  *len = 4; /* header */
  if (cm->msg_type > MG_COAP_MSG_MAX) {
    return MG_COAP_ERROR | MG_COAP_MSG_TYPE_FIELD;
  }
  if (cm->token.len > 8) {
    return MG_COAP_ERROR | MG_COAP_TOKEN_FIELD;
  }
  if (cm->code_class > 7) {
    return MG_COAP_ERROR | MG_COAP_CODE_CLASS_FIELD;
  }
  if (cm->code_detail > 31) {
    return MG_COAP_ERROR | MG_COAP_CODE_DETAIL_FIELD;
  }

  *len += cm->token.len;
  if (cm->payload.len != 0) {
    *len += cm->payload.len + 1; /* ... + 1; add payload marker */
  }

  opt = cm->options;
  prev_opt_number = 0;
  while (opt != NULL) {
    *len += 1; /* basic delta/length */
    *len += coap_get_ext_opt_size(opt->number);
    *len += coap_get_ext_opt_size((uint32_t) opt->value.len);
    /*
     * Current implementation performs check if
     * option_number > previous option_number and produces an error
     * TODO(alashkin): write design doc with limitations
     * May be resorting is more suitable solution.
     */
    if ((opt->next != NULL && opt->number > opt->next->number) ||
        opt->value.len > 0xFFFF + 269 ||
        opt->number - prev_opt_number > 0xFFFF + 269) {
      return MG_COAP_ERROR | MG_COAP_OPTIOMG_FIELD;
    }
    *len += opt->value.len;
    opt = opt->next;
  }

  return 0;
}

uint32_t mg_coap_compose(struct mg_coap_message *cm, struct mbuf *io) {
  struct mg_coap_option *opt;
  uint32_t res, prev_opt_number;
  size_t prev_io_len, packet_size;
  char *ptr;

  res = coap_calculate_packet_size(cm, &packet_size);
  if (res != 0) {
    return res;
  }

  /* saving previous lenght to handle non-empty mbuf */
  prev_io_len = io->len;
  mbuf_append(io, NULL, packet_size);
  ptr = io->buf + prev_io_len;

  /*
   * since cm is verified, it is possible to use bits shift operator
   * without additional zeroing of unused bits
   */

  /* ver: 2 bits, msg_type: 2 bits, toklen: 4 bits */
  *ptr = (1 << 6) | (cm->msg_type << 4) | (cm->token.len);
  ptr++;

  /* code class: 3 bits, code detail: 5 bits */
  *ptr = (cm->code_class << 5) | (cm->code_detail);
  ptr++;

  ptr = coap_add_uint16(ptr, cm->msg_id);

  if (cm->token.len != 0) {
    memcpy(ptr, cm->token.p, cm->token.len);
    ptr += cm->token.len;
  }

  opt = cm->options;
  prev_opt_number = 0;
  while (opt != NULL) {
    uint8_t delta_base = 0, length_base = 0;
    uint16_t delta_ext, length_ext;

    size_t opt_delta_len =
        coap_split_opt(opt->number - prev_opt_number, &delta_base, &delta_ext);
    size_t opt_lenght_len =
        coap_split_opt((uint32_t) opt->value.len, &length_base, &length_ext);

    *ptr = (delta_base << 4) | length_base;
    ptr++;

    ptr = coap_add_opt_info(ptr, delta_ext, opt_delta_len);
    ptr = coap_add_opt_info(ptr, length_ext, opt_lenght_len);

    if (opt->value.len != 0) {
      memcpy(ptr, opt->value.p, opt->value.len);
      ptr += opt->value.len;
    }

    prev_opt_number = opt->number;
    opt = opt->next;
  }

  if (cm->payload.len != 0) {
    *ptr = -1;
    ptr++;
    memcpy(ptr, cm->payload.p, cm->payload.len);
  }

  return 0;
}

uint32_t mg_coap_send_message(struct mg_connection *nc,
                              struct mg_coap_message *cm) {
  struct mbuf packet_out;
  uint32_t compose_res;

  mbuf_init(&packet_out, 0);
  compose_res = mg_coap_compose(cm, &packet_out);
  if (compose_res != 0) {
    return compose_res; /* LCOV_EXCL_LINE */
  }

  mg_send(nc, packet_out.buf, (int) packet_out.len);
  mbuf_free(&packet_out);

  return 0;
}

uint32_t mg_coap_send_ack(struct mg_connection *nc, uint16_t msg_id) {
  struct mg_coap_message cm;
  memset(&cm, 0, sizeof(cm));
  cm.msg_type = MG_COAP_MSG_ACK;
  cm.msg_id = msg_id;

  return mg_coap_send_message(nc, &cm);
}

static void coap_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct mbuf *io = &nc->recv_mbuf;
  struct mg_coap_message cm;
  uint32_t parse_res;

  memset(&cm, 0, sizeof(cm));

  nc->handler(nc, ev, ev_data);

  switch (ev) {
    case MG_EV_RECV:
      parse_res = mg_coap_parse(io, &cm);
      if ((parse_res & MG_COAP_IGNORE) == 0) {
        if ((cm.flags & MG_COAP_NOT_ENOUGH_DATA) != 0) {
          /*
           * Since we support UDP only
           * MG_COAP_NOT_ENOUGH_DATA == MG_COAP_FORMAT_ERROR
           */
          cm.flags |= MG_COAP_FORMAT_ERROR; /* LCOV_EXCL_LINE */
        }                                   /* LCOV_EXCL_LINE */
        nc->handler(nc, MG_COAP_EVENT_BASE + cm.msg_type, &cm);
      }

      mg_coap_free_options(&cm);
      mbuf_remove(io, io->len);
      break;
  }
}
/*
 * Attach built-in CoAP event handler to the given connection.
 *
 * The user-defined event handler will receive following extra events:
 *
 * - MG_EV_COAP_CON
 * - MG_EV_COAP_NOC
 * - MG_EV_COAP_ACK
 * - MG_EV_COAP_RST
 */
int mg_set_protocol_coap(struct mg_connection *nc) {
  /* supports UDP only */
  if ((nc->flags & MG_F_UDP) == 0) {
    return -1;
  }

  nc->proto_handler = coap_handler;

  return 0;
}

#endif /* MG_DISABLE_COAP */
#ifdef MG_MODULE_LINES
#line 1 "./src/../../common/platforms/cc3200/cc3200_libc.c"
#endif
/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#if CS_PLATFORM == CS_P_CC3200

#include <stdio.h>
#include <string.h>

#ifndef __TI_COMPILER_VERSION__
#include <reent.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#endif

#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <driverlib/prcm.h>
#include <driverlib/rom.h>
#include <driverlib/rom_map.h>
#include <driverlib/uart.h>
#include <driverlib/utils.h>

#define CONSOLE_UART UARTA0_BASE

#ifdef __TI_COMPILER_VERSION__
int asprintf(char **strp, const char *fmt, ...) {
  va_list ap;
  int len;

  *strp = malloc(BUFSIZ);
  if (*strp == NULL) return -1;

  va_start(ap, fmt);
  len = vsnprintf(*strp, BUFSIZ, fmt, ap);
  va_end(ap);

  if (len > 0) {
    *strp = realloc(*strp, len + 1);
    if (*strp == NULL) return -1;
  }

  if (len >= BUFSIZ) {
    va_start(ap, fmt);
    len = vsnprintf(*strp, len + 1, fmt, ap);
    va_end(ap);
  }

  return len;
}

#if MG_TI_NO_HOST_INTERFACE
time_t HOSTtime() {
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return tp.tv_sec;
}
#endif

#endif /* __TI_COMPILER_VERSION__ */

#ifndef __TI_COMPILER_VERSION__
int _gettimeofday_r(struct _reent *r, struct timeval *tp, void *tzp) {
#else
int gettimeofday(struct timeval *tp, void *tzp) {
#endif
  unsigned long long r1 = 0, r2;
  /* Achieve two consecutive reads of the same value. */
  do {
    r2 = r1;
    r1 = PRCMSlowClkCtrFastGet();
  } while (r1 != r2);
  /* This is a 32768 Hz counter. */
  tp->tv_sec = (r1 >> 15);
  /* 1/32768-th of a second is 30.517578125 microseconds, approx. 31,
   * but we round down so it doesn't overflow at 32767 */
  tp->tv_usec = (r1 & 0x7FFF) * 30;
  return 0;
}

long int random(void) {
  return 42; /* FIXME */
}

void fprint_str(FILE *fp, const char *str) {
  while (*str != '\0') {
    if (*str == '\n') MAP_UARTCharPut(CONSOLE_UART, '\r');
    MAP_UARTCharPut(CONSOLE_UART, *str++);
  }
}

void _exit(int status) {
  fprint_str(stderr, "_exit\n");
  /* cause an unaligned access exception, that will drop you into gdb */
  *(int *) 1 = status;
  while (1)
    ; /* avoid gcc warning because stdlib abort() has noreturn attribute */
}

void _not_implemented(const char *what) {
  fprint_str(stderr, what);
  fprint_str(stderr, " is not implemented\n");
  _exit(42);
}

int _kill(int pid, int sig) {
  (void) pid;
  (void) sig;
  _not_implemented("_kill");
  return -1;
}

int _getpid() {
  fprint_str(stderr, "_getpid is not implemented\n");
  return 42;
}

int _isatty(int fd) {
  /* 0, 1 and 2 are TTYs. */
  return fd < 2;
}

#endif /* CS_PLATFORM == CS_P_CC3200 */
#ifdef MG_MODULE_LINES
#line 1 "./src/../../common/platforms/msp432/msp432_libc.c"
#endif
/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#if CS_PLATFORM == CS_P_MSP432

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>

int gettimeofday(struct timeval *tp, void *tzp) {
  uint32_t ticks = Clock_getTicks();
  tp->tv_sec = ticks / 1000;
  tp->tv_usec = (ticks % 1000) * 1000;
  return 0;
}

long int random(void) {
  return 42; /* FIXME */
}

#endif /* CS_PLATFORM == CS_P_MSP432 */
#ifdef MG_MODULE_LINES
#line 1 "./src/../../common/platforms/simplelink/sl_fs_slfs.h"
#endif
/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_PLATFORMS_SIMPLELINK_SL_FS_SLFS_H_
#define CS_COMMON_PLATFORMS_SIMPLELINK_SL_FS_SLFS_H_

#if defined(MG_FS_SLFS)

#include <stdio.h>
#ifndef __TI_COMPILER_VERSION__
#include <unistd.h>
#include <sys/stat.h>
#endif

#define MAX_OPEN_SLFS_FILES 8

/* Indirect libc interface - same functions, different names. */
int fs_slfs_open(const char *pathname, int flags, mode_t mode);
int fs_slfs_close(int fd);
ssize_t fs_slfs_read(int fd, void *buf, size_t count);
ssize_t fs_slfs_write(int fd, const void *buf, size_t count);
int fs_slfs_stat(const char *pathname, struct stat *s);
int fs_slfs_fstat(int fd, struct stat *s);
off_t fs_slfs_lseek(int fd, off_t offset, int whence);
int fs_slfs_unlink(const char *filename);
int fs_slfs_rename(const char *from, const char *to);

void fs_slfs_set_new_file_size(const char *name, size_t size);

#endif /* defined(MG_FS_SLFS) */

#endif /* CS_COMMON_PLATFORMS_SIMPLELINK_SL_FS_SLFS_H_ */
#ifdef MG_MODULE_LINES
#line 1 "./src/../../common/platforms/simplelink/sl_fs_slfs.c"
#endif
/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

/* Standard libc interface to TI SimpleLink FS. */

#if defined(MG_FS_SLFS) || defined(CC3200_FS_SLFS)

/* Amalgamated: #include "common/platforms/simplelink/sl_fs_slfs.h" */

#include <errno.h>

#if CS_PLATFORM == CS_P_CC3200
#include <inc/hw_types.h>
#endif
#include <simplelink/include/simplelink.h>
#include <simplelink/include/fs.h>

/* Amalgamated: #include "common/cs_dbg.h" */

extern int set_errno(int e); /* From sl_fs.c */

/*
 * With SLFS, you have to pre-declare max file size. Yes. Really.
 * 64K should be enough for everyone. Right?
 */
#ifndef FS_SLFS_MAX_FILE_SIZE
#define FS_SLFS_MAX_FILE_SIZE (64 * 1024)
#endif

struct sl_file_size_hint {
  char *name;
  size_t size;
};

struct sl_fd_info {
  _i32 fh;
  _off_t pos;
  size_t size;
};

static struct sl_fd_info s_sl_fds[MAX_OPEN_SLFS_FILES];
static struct sl_file_size_hint s_sl_file_size_hints[MAX_OPEN_SLFS_FILES];

static int sl_fs_to_errno(_i32 r) {
  DBG(("SL error: %d", (int) r));
  switch (r) {
    case SL_FS_OK:
      return 0;
    case SL_FS_FILE_NAME_EXIST:
      return EEXIST;
    case SL_FS_WRONG_FILE_NAME:
      return EINVAL;
    case SL_FS_ERR_NO_AVAILABLE_NV_INDEX:
    case SL_FS_ERR_NO_AVAILABLE_BLOCKS:
      return ENOSPC;
    case SL_FS_ERR_FAILED_TO_ALLOCATE_MEM:
      return ENOMEM;
    case SL_FS_ERR_FILE_NOT_EXISTS:
      return ENOENT;
    case SL_FS_ERR_NOT_SUPPORTED:
      return ENOTSUP;
  }
  return ENXIO;
}

int fs_slfs_open(const char *pathname, int flags, mode_t mode) {
  int fd;
  for (fd = 0; fd < MAX_OPEN_SLFS_FILES; fd++) {
    if (s_sl_fds[fd].fh <= 0) break;
  }
  if (fd >= MAX_OPEN_SLFS_FILES) return set_errno(ENOMEM);
  struct sl_fd_info *fi = &s_sl_fds[fd];

  _u32 am = 0;
  fi->size = (size_t) -1;
  if (pathname[0] == '/') pathname++;
  int rw = (flags & 3);
  if (rw == O_RDONLY) {
    SlFsFileInfo_t sl_fi;
    _i32 r = sl_FsGetInfo((const _u8 *) pathname, 0, &sl_fi);
    if (r == SL_FS_OK) {
      fi->size = sl_fi.FileLen;
    }
    am = FS_MODE_OPEN_READ;
  } else {
    if (!(flags & O_TRUNC) || (flags & O_APPEND)) {
      // FailFS files cannot be opened for append and will be truncated
      // when opened for write.
      return set_errno(ENOTSUP);
    }
    if (flags & O_CREAT) {
      size_t i, size = FS_SLFS_MAX_FILE_SIZE;
      for (i = 0; i < MAX_OPEN_SLFS_FILES; i++) {
        if (s_sl_file_size_hints[i].name != NULL &&
            strcmp(s_sl_file_size_hints[i].name, pathname) == 0) {
          size = s_sl_file_size_hints[i].size;
          free(s_sl_file_size_hints[i].name);
          s_sl_file_size_hints[i].name = NULL;
          break;
        }
      }
      DBG(("creating %s with max size %d", pathname, (int) size));
      am = FS_MODE_OPEN_CREATE(size, 0);
    } else {
      am = FS_MODE_OPEN_WRITE;
    }
  }
  _i32 r = sl_FsOpen((_u8 *) pathname, am, NULL, &fi->fh);
  DBG(("sl_FsOpen(%s, 0x%x) = %d, %d", pathname, (int) am, (int) r,
       (int) fi->fh));
  if (r == SL_FS_OK) {
    fi->pos = 0;
    r = fd;
  } else {
    fi->fh = -1;
    r = set_errno(sl_fs_to_errno(r));
  }
  return r;
}

int fs_slfs_close(int fd) {
  struct sl_fd_info *fi = &s_sl_fds[fd];
  if (fi->fh <= 0) return set_errno(EBADF);
  _i32 r = sl_FsClose(fi->fh, NULL, NULL, 0);
  DBG(("sl_FsClose(%d) = %d", (int) fi->fh, (int) r));
  s_sl_fds[fd].fh = -1;
  return set_errno(sl_fs_to_errno(r));
}

ssize_t fs_slfs_read(int fd, void *buf, size_t count) {
  struct sl_fd_info *fi = &s_sl_fds[fd];
  if (fi->fh <= 0) return set_errno(EBADF);
  /* Simulate EOF. sl_FsRead @ file_size return SL_FS_ERR_OFFSET_OUT_OF_RANGE.
   */
  if (fi->pos == fi->size) return 0;
  _i32 r = sl_FsRead(fi->fh, fi->pos, buf, count);
  DBG(("sl_FsRead(%d, %d, %d) = %d", (int) fi->fh, (int) fi->pos, (int) count,
       (int) r));
  if (r >= 0) {
    fi->pos += r;
    return r;
  }
  return set_errno(sl_fs_to_errno(r));
}

ssize_t fs_slfs_write(int fd, const void *buf, size_t count) {
  struct sl_fd_info *fi = &s_sl_fds[fd];
  if (fi->fh <= 0) return set_errno(EBADF);
  _i32 r = sl_FsWrite(fi->fh, fi->pos, (_u8 *) buf, count);
  DBG(("sl_FsWrite(%d, %d, %d) = %d", (int) fi->fh, (int) fi->pos, (int) count,
       (int) r));
  if (r >= 0) {
    fi->pos += r;
    return r;
  }
  return set_errno(sl_fs_to_errno(r));
}

int fs_slfs_stat(const char *pathname, struct stat *s) {
  SlFsFileInfo_t sl_fi;
  _i32 r = sl_FsGetInfo((const _u8 *) pathname, 0, &sl_fi);
  if (r == SL_FS_OK) {
    s->st_mode = S_IFREG | 0666;
    s->st_nlink = 1;
    s->st_size = sl_fi.FileLen;
    return 0;
  }
  return set_errno(sl_fs_to_errno(r));
}

int fs_slfs_fstat(int fd, struct stat *s) {
  struct sl_fd_info *fi = &s_sl_fds[fd];
  if (fi->fh <= 0) return set_errno(EBADF);
  s->st_mode = 0666;
  s->st_mode = S_IFREG | 0666;
  s->st_nlink = 1;
  s->st_size = fi->size;
  return 0;
}

off_t fs_slfs_lseek(int fd, off_t offset, int whence) {
  if (s_sl_fds[fd].fh <= 0) return set_errno(EBADF);
  switch (whence) {
    case SEEK_SET:
      s_sl_fds[fd].pos = offset;
      break;
    case SEEK_CUR:
      s_sl_fds[fd].pos += offset;
      break;
    case SEEK_END:
      return set_errno(ENOTSUP);
  }
  return 0;
}

int fs_slfs_unlink(const char *filename) {
  return set_errno(sl_fs_to_errno(sl_FsDel((const _u8 *) filename, 0)));
}

int fs_slfs_rename(const char *from, const char *to) {
  return set_errno(ENOTSUP);
}

void fs_slfs_set_new_file_size(const char *name, size_t size) {
  int i;
  for (i = 0; i < MAX_OPEN_SLFS_FILES; i++) {
    if (s_sl_file_size_hints[i].name == NULL) {
      DBG(("File size hint: %s %d", name, (int) size));
      s_sl_file_size_hints[i].name = strdup(name);
      s_sl_file_size_hints[i].size = size;
      break;
    }
  }
}

#endif /* defined(MG_FS_SLFS) || defined(CC3200_FS_SLFS) */
#ifdef MG_MODULE_LINES
#line 1 "./src/../../common/platforms/simplelink/sl_fs.c"
#endif
/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#if defined(MG_SOCKET_SIMPLELINK) && \
    (defined(MG_FS_SLFS) || defined(MG_FS_SPIFFS))

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __TI_COMPILER_VERSION__
#include <file.h>
#endif

#if CS_PLATFORM == CS_P_CC3200
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <driverlib/rom.h>
#include <driverlib/rom_map.h>
#include <driverlib/uart.h>
#endif

/* Amalgamated: #include "common/cs_dbg.h" */
/* Amalgamated: #include "common/platform.h" */

#ifdef CC3200_FS_SPIFFS
/* Amalgamated: #include "cc3200_fs_spiffs.h" */
#endif

#ifdef MG_FS_SLFS
/* Amalgamated: #include "sl_fs_slfs.h" */
#endif

#define NUM_SYS_FDS 3
#define SPIFFS_FD_BASE 10
#define SLFS_FD_BASE 100

#define CONSOLE_UART UARTA0_BASE

int set_errno(int e) {
  errno = e;
  return (e == 0 ? 0 : -1);
}

static int is_sl_fname(const char *fname) {
  return strncmp(fname, "SL:", 3) == 0;
}

static const char *sl_fname(const char *fname) {
  return fname + 3;
}

static const char *drop_dir(const char *fname) {
  if (*fname == '.') fname++;
  if (*fname == '/') fname++;
  return fname;
}

enum fd_type {
  FD_INVALID,
  FD_SYS,
#ifdef CC3200_FS_SPIFFS
  FD_SPIFFS,
#endif
#ifdef MG_FS_SLFS
  FD_SLFS
#endif
};
static int fd_type(int fd) {
  if (fd >= 0 && fd < NUM_SYS_FDS) return FD_SYS;
#ifdef CC3200_FS_SPIFFS
  if (fd >= SPIFFS_FD_BASE && fd < SPIFFS_FD_BASE + MAX_OPEN_SPIFFS_FILES) {
    return FD_SPIFFS;
  }
#endif
#ifdef MG_FS_SLFS
  if (fd >= SLFS_FD_BASE && fd < SLFS_FD_BASE + MAX_OPEN_SLFS_FILES) {
    return FD_SLFS;
  }
#endif
  return FD_INVALID;
}

#if MG_TI_NO_HOST_INTERFACE
int open(const char *pathname, unsigned flags, int mode) {
#else
int _open(const char *pathname, int flags, mode_t mode) {
#endif
  int fd = -1;
  pathname = drop_dir(pathname);
  if (is_sl_fname(pathname)) {
#ifdef MG_FS_SLFS
    fd = fs_slfs_open(sl_fname(pathname), flags, mode);
    if (fd >= 0) fd += SLFS_FD_BASE;
#endif
  } else {
#ifdef CC3200_FS_SPIFFS
    fd = fs_spiffs_open(pathname, flags, mode);
    if (fd >= 0) fd += SPIFFS_FD_BASE;
#endif
  }
  DBG(("open(%s, 0x%x) = %d", pathname, flags, fd));
  return fd;
}

int _stat(const char *pathname, struct stat *st) {
  int res = -1;
  const char *fname = pathname;
  int is_sl = is_sl_fname(pathname);
  if (is_sl) fname = sl_fname(pathname);
  fname = drop_dir(fname);
  memset(st, 0, sizeof(*st));
  /* Simulate statting the root directory. */
  if (strcmp(fname, "") == 0) {
    st->st_ino = 0;
    st->st_mode = S_IFDIR | 0777;
    st->st_nlink = 1;
    st->st_size = 0;
    return 0;
  }
  if (is_sl) {
#ifdef MG_FS_SLFS
    res = fs_slfs_stat(fname, st);
#endif
  } else {
#ifdef CC3200_FS_SPIFFS
    res = fs_spiffs_stat(fname, st);
#endif
  }
  DBG(("stat(%s) = %d; fname = %s", pathname, res, fname));
  return res;
}

#if MG_TI_NO_HOST_INTERFACE
int close(int fd) {
#else
int _close(int fd) {
#endif
  int r = -1;
  switch (fd_type(fd)) {
    case FD_INVALID:
      r = set_errno(EBADF);
      break;
    case FD_SYS:
      r = set_errno(EACCES);
      break;
#ifdef CC3200_FS_SPIFFS
    case FD_SPIFFS:
      r = fs_spiffs_close(fd - SPIFFS_FD_BASE);
      break;
#endif
#ifdef MG_FS_SLFS
    case FD_SLFS:
      r = fs_slfs_close(fd - SLFS_FD_BASE);
      break;
#endif
  }
  DBG(("close(%d) = %d", fd, r));
  return r;
}

#if MG_TI_NO_HOST_INTERFACE
off_t lseek(int fd, off_t offset, int whence) {
#else
off_t _lseek(int fd, off_t offset, int whence) {
#endif
  int r = -1;
  switch (fd_type(fd)) {
    case FD_INVALID:
      r = set_errno(EBADF);
      break;
    case FD_SYS:
      r = set_errno(ESPIPE);
      break;
#ifdef CC3200_FS_SPIFFS
    case FD_SPIFFS:
      r = fs_spiffs_lseek(fd - SPIFFS_FD_BASE, offset, whence);
      break;
#endif
#ifdef MG_FS_SLFS
    case FD_SLFS:
      r = fs_slfs_lseek(fd - SLFS_FD_BASE, offset, whence);
      break;
#endif
  }
  DBG(("lseek(%d, %d, %d) = %d", fd, (int) offset, whence, r));
  return r;
}

int _fstat(int fd, struct stat *s) {
  int r = -1;
  memset(s, 0, sizeof(*s));
  switch (fd_type(fd)) {
    case FD_INVALID:
      r = set_errno(EBADF);
      break;
    case FD_SYS: {
      /* Create barely passable stats for STD{IN,OUT,ERR}. */
      memset(s, 0, sizeof(*s));
      s->st_ino = fd;
      s->st_mode = S_IFCHR | 0666;
      r = 0;
      break;
    }
#ifdef CC3200_FS_SPIFFS
    case FD_SPIFFS:
      r = fs_spiffs_fstat(fd - SPIFFS_FD_BASE, s);
      break;
#endif
#ifdef MG_FS_SLFS
    case FD_SLFS:
      r = fs_slfs_fstat(fd - SLFS_FD_BASE, s);
      break;
#endif
  }
  DBG(("fstat(%d) = %d", fd, r));
  return r;
}

#if MG_TI_NO_HOST_INTERFACE
int read(int fd, char *buf, unsigned count) {
#else
ssize_t _read(int fd, void *buf, size_t count) {
#endif
  int r = -1;
  switch (fd_type(fd)) {
    case FD_INVALID:
      r = set_errno(EBADF);
      break;
    case FD_SYS: {
      if (fd != 0) {
        r = set_errno(EACCES);
        break;
      }
      /* Should we allow reading from stdin = uart? */
      r = set_errno(ENOTSUP);
      break;
    }
#ifdef CC3200_FS_SPIFFS
    case FD_SPIFFS:
      r = fs_spiffs_read(fd - SPIFFS_FD_BASE, buf, count);
      break;
#endif
#ifdef MG_FS_SLFS
    case FD_SLFS:
      r = fs_slfs_read(fd - SLFS_FD_BASE, buf, count);
      break;
#endif
  }
  DBG(("read(%d, %u) = %d", fd, count, r));
  return r;
}

#if MG_TI_NO_HOST_INTERFACE
int write(int fd, const char *buf, unsigned count) {
#else
ssize_t _write(int fd, const void *buf, size_t count) {
#endif
  int r = -1;
  size_t i = 0;
  switch (fd_type(fd)) {
    case FD_INVALID:
      r = set_errno(EBADF);
      break;
    case FD_SYS: {
      if (fd == 0) {
        r = set_errno(EACCES);
        break;
      }
#if CS_PLATFORM == CS_P_CC3200
      for (i = 0; i < count; i++) {
        const char c = ((const char *) buf)[i];
        if (c == '\n') MAP_UARTCharPut(CONSOLE_UART, '\r');
        MAP_UARTCharPut(CONSOLE_UART, c);
      }
#else
      (void) i;
#endif
      r = count;
      break;
    }
#ifdef CC3200_FS_SPIFFS
    case FD_SPIFFS:
      r = fs_spiffs_write(fd - SPIFFS_FD_BASE, buf, count);
      break;
#endif
#ifdef MG_FS_SLFS
    case FD_SLFS:
      r = fs_slfs_write(fd - SLFS_FD_BASE, buf, count);
      break;
#endif
  }
  return r;
}

/*
 * On Newlib we override rename directly too, because the default
 * implementation using _link and _unlink doesn't work for us.
 */
#if MG_TI_NO_HOST_INTERFACE || defined(_NEWLIB_VERSION)
int rename(const char *from, const char *to) {
  int r = -1;
  from = drop_dir(from);
  to = drop_dir(to);
  if (is_sl_fname(from) || is_sl_fname(to)) {
#ifdef MG_FS_SLFS
    r = fs_slfs_rename(sl_fname(from), sl_fname(to));
#endif
  } else {
#ifdef CC3200_FS_SPIFFS
    r = fs_spiffs_rename(from, to);
#endif
  }
  DBG(("rename(%s, %s) = %d", from, to, r));
  return r;
}
#endif /* MG_TI_NO_HOST_INTERFACE || defined(_NEWLIB_VERSION) */

#if MG_TI_NO_HOST_INTERFACE
int unlink(const char *filename) {
#else
int _unlink(const char *filename) {
#endif
  int r = -1;
  filename = drop_dir(filename);
  if (is_sl_fname(filename)) {
#ifdef MG_FS_SLFS
    r = fs_slfs_unlink(sl_fname(filename));
#endif
  } else {
#ifdef CC3200_FS_SPIFFS
    r = fs_spiffs_unlink(filename);
#endif
  }
  DBG(("unlink(%s) = %d", filename, r));
  return r;
}

#ifdef CC3200_FS_SPIFFS /* FailFS does not support listing files. */
DIR *opendir(const char *dir_name) {
  DIR *r = NULL;
  if (is_sl_fname(dir_name)) {
    r = NULL;
    set_errno(ENOTSUP);
  } else {
    r = fs_spiffs_opendir(dir_name);
  }
  DBG(("opendir(%s) = %p", dir_name, r));
  return r;
}

struct dirent *readdir(DIR *dir) {
  struct dirent *res = fs_spiffs_readdir(dir);
  DBG(("readdir(%p) = %p", dir, res));
  return res;
}

int closedir(DIR *dir) {
  int res = fs_spiffs_closedir(dir);
  DBG(("closedir(%p) = %d", dir, res));
  return res;
}

int rmdir(const char *path) {
  return fs_spiffs_rmdir(path);
}

int mkdir(const char *path, mode_t mode) {
  (void) path;
  (void) mode;
  /* for spiffs supports only root dir, which comes from mongoose as '.' */
  return (strlen(path) == 1 && *path == '.') ? 0 : ENOTDIR;
}
#endif

int sl_fs_init() {
  int ret = 1;
#ifdef __TI_COMPILER_VERSION__
#ifdef MG_FS_SLFS
#pragma diag_push
#pragma diag_suppress 169 /* Nothing we can do about the prototype mismatch. */
  ret = (add_device("SL", _MSA, fs_slfs_open, fs_slfs_close, fs_slfs_read,
                    fs_slfs_write, fs_slfs_lseek, fs_slfs_unlink,
                    fs_slfs_rename) == 0);
#pragma diag_pop
#endif
#endif
  return ret;
}

#endif /* defined(MG_SOCKET_SIMPLELINK) && (defined(MG_FS_SLFS) || \
          defined(MG_FS_SPIFFS)) */
#ifdef MG_MODULE_LINES
#line 1 "./src/../../common/platforms/simplelink/sl_socket.c"
#endif
/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifdef MG_SOCKET_SIMPLELINK

#include <errno.h>
#include <stdio.h>

/* Amalgamated: #include "common/platform.h" */

#include <simplelink/include/netapp.h>

const char *inet_ntop(int af, const void *src, char *dst, socklen_t size) {
  int res;
  struct in_addr *in = (struct in_addr *) src;
  if (af != AF_INET) {
    errno = EAFNOSUPPORT;
    return NULL;
  }
  res = snprintf(dst, size, "%lu.%lu.%lu.%lu", SL_IPV4_BYTE(in->s_addr, 0),
                 SL_IPV4_BYTE(in->s_addr, 1), SL_IPV4_BYTE(in->s_addr, 2),
                 SL_IPV4_BYTE(in->s_addr, 3));
  return res > 0 ? dst : NULL;
}

char *inet_ntoa(struct in_addr n) {
  static char a[16];
  return (char *) inet_ntop(AF_INET, &n, a, sizeof(a));
}

int inet_pton(int af, const char *src, void *dst) {
  uint32_t a0, a1, a2, a3;
  uint8_t *db = (uint8_t *) dst;
  if (af != AF_INET) {
    errno = EAFNOSUPPORT;
    return 0;
  }
  if (sscanf(src, "%lu.%lu.%lu.%lu", &a0, &a1, &a2, &a3) != 4) {
    return 0;
  }
  *db = a3;
  *(db + 1) = a2;
  *(db + 2) = a1;
  *(db + 3) = a0;
  return 1;
}

#endif /* CS_COMMON_PLATFORMS_SIMPLELINK_SL_SOCKET_C_ */
#ifdef MG_MODULE_LINES
#line 1 "./src/../../common/platforms/simplelink/sl_mg_task.c"
#endif
#if defined(MG_SOCKET_SIMPLELINK)

/* Amalgamated: #include "mg_task.h" */

#include <oslib/osi.h>

enum mg_q_msg_type {
  MG_Q_MSG_CB,
};
struct mg_q_msg {
  enum mg_q_msg_type type;
  void (*cb)(struct mg_mgr *mgr, void *arg);
  void *arg;
};
static OsiMsgQ_t s_mg_q;
static void mg_task(void *arg);

bool mg_start_task(int priority, int stack_size, mg_init_cb mg_init) {
  if (osi_MsgQCreate(&s_mg_q, "MG", sizeof(struct mg_q_msg), 16) != OSI_OK) {
    return false;
  }
  if (osi_TaskCreate(mg_task, (const signed char *) "MG", stack_size,
                     (void *) mg_init, priority, NULL) != OSI_OK) {
    return false;
  }
  return true;
}

static void mg_task(void *arg) {
  struct mg_mgr mgr;
  mg_init_cb mg_init = (mg_init_cb) arg;
  mg_mgr_init(&mgr, NULL);
  mg_init(&mgr);
  while (1) {
    struct mg_q_msg msg;
    mg_mgr_poll(&mgr, 1);
    if (osi_MsgQRead(&s_mg_q, &msg, 1) != OSI_OK) continue;
    switch (msg.type) {
      case MG_Q_MSG_CB: {
        msg.cb(&mgr, msg.arg);
      }
    }
  }
}

void mg_run_in_task(void (*cb)(struct mg_mgr *mgr, void *arg), void *cb_arg) {
  struct mg_q_msg msg = {MG_Q_MSG_CB, cb, cb_arg};
  osi_MsgQWrite(&s_mg_q, &msg, OSI_NO_WAIT);
}

#endif /* defined(MG_SOCKET_SIMPLELINK) */
#ifdef MG_MODULE_LINES
#line 1 "./src/../../common/platforms/simplelink/sl_net_if.c"
#endif
/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#if !defined(MG_DISABLE_SOCKET_IF) && defined(MG_SOCKET_SIMPLELINK)

/* Amalgamated: #include "mongoose/src/internal.h" */
/* Amalgamated: #include "mongoose/src/util.h" */

#define MG_TCP_RECV_BUFFER_SIZE 1024
#define MG_UDP_RECV_BUFFER_SIZE 1500

static sock_t mg_open_listening_socket(union socket_address *sa, int type,
                                       int proto);

#ifdef MG_ENABLE_SSL
const char *mg_set_ssl2(struct mg_connection *nc, const char *cert,
                        const char *key, const char *ca_cert) {
  DBG(("%p %s,%s,%s", nc, (cert ? cert : ""), (key ? key : ""),
       (ca_cert ? ca_cert : "")));

  if (nc->flags & MG_F_UDP) {
    return "SSL for UDP is not supported";
  }

  if (cert != NULL || key != NULL) {
    if (cert != NULL && key != NULL) {
      nc->ssl_cert = strdup(cert);
      nc->ssl_key = strdup(key);
    } else {
      return "both cert and key are required";
    }
  }
  if (ca_cert != NULL) nc->ssl_ca_cert = strdup(ca_cert);

  nc->flags |= MG_F_SSL;

  return NULL;
}

int sl_set_ssl_opts(struct mg_connection *nc) {
  int err;
  DBG(("%p %s,%s,%s,%s", nc, (nc->ssl_cert ? nc->ssl_cert : ""),
       (nc->ssl_key ? nc->ssl_cert : ""),
       (nc->ssl_ca_cert ? nc->ssl_ca_cert : ""),
       (nc->ssl_server_name ? nc->ssl_server_name : "")));
  if (nc->ssl_cert != NULL && nc->ssl_key != NULL) {
    err = sl_SetSockOpt(nc->sock, SL_SOL_SOCKET,
                        SL_SO_SECURE_FILES_CERTIFICATE_FILE_NAME, nc->ssl_cert,
                        strlen(nc->ssl_cert));
    DBG(("CERTIFICATE_FILE_NAME %s -> %d", nc->ssl_cert, err));
    if (err != 0) return err;
    err = sl_SetSockOpt(nc->sock, SL_SOL_SOCKET,
                        SL_SO_SECURE_FILES_PRIVATE_KEY_FILE_NAME, nc->ssl_key,
                        strlen(nc->ssl_key));
    DBG(("PRIVATE_KEY_FILE_NAME %s -> %d", nc->ssl_key, nc->err));
    if (err != 0) return err;
  }
  if (nc->ssl_ca_cert != NULL) {
    if (nc->ssl_ca_cert[0] != '\0') {
      err = sl_SetSockOpt(nc->sock, SL_SOL_SOCKET,
                          SL_SO_SECURE_FILES_CA_FILE_NAME, nc->ssl_ca_cert,
                          strlen(nc->ssl_ca_cert));
      DBG(("CA_FILE_NAME %s -> %d", nc->ssl_ca_cert, err));
      if (err != 0) return err;
    }
  }
  if (nc->ssl_server_name != NULL) {
    err = sl_SetSockOpt(nc->sock, SL_SOL_SOCKET,
                        SO_SECURE_DOMAIN_NAME_VERIFICATION, nc->ssl_server_name,
                        strlen(nc->ssl_server_name));
    DBG(("DOMAIN_NAME_VERIFICATION %s -> %d", nc->ssl_server_name, err));
    /* Domain name verificationw as added in a NWP service pack, older versions
     * return SL_ENOPROTOOPT. There isn't much we can do about it, so we ignore
     * the error. */
    if (err != 0 && err != SL_ENOPROTOOPT) return err;
  }
  return 0;
}
#endif

void mg_set_non_blocking_mode(sock_t sock) {
  SlSockNonblocking_t opt;
  opt.NonblockingEnabled = 1;
  sl_SetSockOpt(sock, SL_SOL_SOCKET, SL_SO_NONBLOCKING, &opt, sizeof(opt));
}

static int mg_is_error(int n) {
  return (n < 0 && n != SL_EALREADY);
}

void mg_if_connect_tcp(struct mg_connection *nc,
                       const union socket_address *sa) {
  int proto = 0;
  if (nc->flags & MG_F_SSL) proto = SL_SEC_SOCKET;
  sock_t sock = sl_Socket(AF_INET, SOCK_STREAM, proto);
  if (sock < 0) {
    nc->err = sock;
    goto out;
  }
  mg_sock_set(nc, sock);
#ifdef MG_ENABLE_SSL
  nc->err = sl_set_ssl_opts(nc);
  if (nc->err != 0) goto out;
#endif
  nc->err = sl_Connect(sock, &sa->sa, sizeof(sa->sin));
out:
  DBG(("%p to %s:%d sock %d err %d", nc, inet_ntoa(sa->sin.sin_addr),
       ntohs(sa->sin.sin_port), nc->sock, nc->err));
}

void mg_if_connect_udp(struct mg_connection *nc) {
  sock_t sock = sl_Socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
    nc->err = sock;
    return;
  }
  mg_sock_set(nc, sock);
  nc->err = 0;
}

int mg_if_listen_tcp(struct mg_connection *nc, union socket_address *sa) {
  int proto = 0;
  if (nc->flags & MG_F_SSL) proto = SL_SEC_SOCKET;
  sock_t sock = mg_open_listening_socket(sa, SOCK_STREAM, proto);
  if (sock < 0) return sock;
  mg_sock_set(nc, sock);
#ifdef MG_ENABLE_SSL
  return sl_set_ssl_opts(nc);
#else
  return 0;
#endif
}

int mg_if_listen_udp(struct mg_connection *nc, union socket_address *sa) {
  sock_t sock = mg_open_listening_socket(sa, SOCK_DGRAM, 0);
  if (sock == INVALID_SOCKET) return (errno ? errno : 1);
  mg_sock_set(nc, sock);
  return 0;
}

void mg_if_tcp_send(struct mg_connection *nc, const void *buf, size_t len) {
  mbuf_append(&nc->send_mbuf, buf, len);
}

void mg_if_udp_send(struct mg_connection *nc, const void *buf, size_t len) {
  mbuf_append(&nc->send_mbuf, buf, len);
}

void mg_if_recved(struct mg_connection *nc, size_t len) {
  (void) nc;
  (void) len;
}

int mg_if_create_conn(struct mg_connection *nc) {
  (void) nc;
  return 1;
}

void mg_if_destroy_conn(struct mg_connection *nc) {
  if (nc->sock == INVALID_SOCKET) return;
  if (!(nc->flags & MG_F_UDP)) {
    sl_Close(nc->sock);
  } else {
    /* Only close outgoing UDP sockets or listeners. */
    if (nc->listener == NULL) sl_Close(nc->sock);
  }
  nc->sock = INVALID_SOCKET;
#ifdef MG_ENABLE_SSL
  MG_FREE(nc->ssl_cert);
  MG_FREE(nc->ssl_key);
  MG_FREE(nc->ssl_ca_cert);
  MG_FREE(nc->ssl_server_name);
#endif
}

static int mg_accept_conn(struct mg_connection *lc) {
  struct mg_connection *nc;
  union socket_address sa;
  socklen_t sa_len = sizeof(sa);
  sock_t sock = sl_Accept(lc->sock, &sa.sa, &sa_len);
  if (sock < 0) {
    DBG(("%p: failed to accept: %d", lc, sock));
    return 0;
  }
  nc = mg_if_accept_new_conn(lc);
  if (nc == NULL) {
    sl_Close(sock);
    return 0;
  }
  DBG(("%p conn from %s:%d", nc, inet_ntoa(sa.sin.sin_addr),
       ntohs(sa.sin.sin_port)));
  mg_sock_set(nc, sock);
  if (nc->flags & MG_F_SSL) nc->flags |= MG_F_SSL_HANDSHAKE_DONE;
  mg_if_accept_tcp_cb(nc, &sa, sa_len);
  return 1;
}

/* 'sa' must be an initialized address to bind to */
static sock_t mg_open_listening_socket(union socket_address *sa, int type,
                                       int proto) {
  int r;
  socklen_t sa_len =
      (sa->sa.sa_family == AF_INET) ? sizeof(sa->sin) : sizeof(sa->sin6);
  sock_t sock = sl_Socket(sa->sa.sa_family, type, proto);
  if (sock < 0) return sock;
  if ((r = bind(sock, &sa->sa, sa_len)) < 0) {
    sl_Close(sock);
    return r;
  }
  if (type != SOCK_DGRAM && (r = sl_Listen(sock, SOMAXCONN)) < 0) {
    sl_Close(sock);
    return r;
  }
  mg_set_non_blocking_mode(sock);
  return sock;
}

static void mg_write_to_socket(struct mg_connection *nc) {
  struct mbuf *io = &nc->send_mbuf;
  int n = 0;

  if (nc->flags & MG_F_UDP) {
    n = sl_SendTo(nc->sock, io->buf, io->len, 0, &nc->sa.sa,
                      sizeof(nc->sa.sin));
    DBG(("%p %d %d %d %s:%hu", nc, nc->sock, n, errno,
         inet_ntoa(nc->sa.sin.sin_addr), ntohs(nc->sa.sin.sin_port)));
  } else {
    n = (int) sl_Send(nc->sock, io->buf, io->len, 0);
    DBG(("%p %d bytes -> %d", nc, n, nc->sock));
  }

  if (n > 0) {
    mbuf_remove(io, n);
    mg_if_sent_cb(nc, n);
  } else if (n < 0 && mg_is_error(n)) {
    /* Something went wrong, drop the connection. */
    nc->flags |= MG_F_CLOSE_IMMEDIATELY;
  }
}

MG_INTERNAL size_t recv_avail_size(struct mg_connection *conn, size_t max) {
  size_t avail;
  if (conn->recv_mbuf_limit < conn->recv_mbuf.len) return 0;
  avail = conn->recv_mbuf_limit - conn->recv_mbuf.len;
  return avail > max ? max : avail;
}

static void mg_handle_tcp_read(struct mg_connection *conn) {
  int n = 0;
  char *buf = (char *) MG_MALLOC(MG_TCP_RECV_BUFFER_SIZE);

  if (buf == NULL) {
    DBG(("OOM"));
    return;
  }

  n = (int) sl_Recv(conn->sock, buf,
                    recv_avail_size(conn, MG_TCP_RECV_BUFFER_SIZE), 0);
  DBG(("%p %d bytes <- %d", conn, n, conn->sock));
  if (n > 0) {
    mg_if_recv_tcp_cb(conn, buf, n);
  } else {
    MG_FREE(buf);
  }
  if (n == 0) {
    /* Orderly shutdown of the socket, try flushing output. */
    conn->flags |= MG_F_SEND_AND_CLOSE;
  } else if (mg_is_error(n)) {
    conn->flags |= MG_F_CLOSE_IMMEDIATELY;
  }
}

static void mg_handle_udp_read(struct mg_connection *nc) {
  char *buf = (char *) MG_MALLOC(MG_UDP_RECV_BUFFER_SIZE);
  if (buf == NULL) return;
  union socket_address sa;
  socklen_t sa_len = sizeof(sa);
  int n = sl_RecvFrom(nc->sock, buf, MG_UDP_RECV_BUFFER_SIZE, 0,
                      (SlSockAddr_t *) &sa, &sa_len);
  DBG(("%p %d bytes from %s:%d", nc, n, inet_ntoa(nc->sa.sin.sin_addr),
       ntohs(nc->sa.sin.sin_port)));
  if (n > 0) {
    mg_if_recv_udp_cb(nc, buf, n, &sa, sa_len);
  } else {
    MG_FREE(buf);
  }
}

#define _MG_F_FD_CAN_READ 1
#define _MG_F_FD_CAN_WRITE 1 << 1
#define _MG_F_FD_ERROR 1 << 2

void mg_mgr_handle_conn(struct mg_connection *nc, int fd_flags, double now) {
  DBG(("%p fd=%d fd_flags=%d nc_flags=%lu rmbl=%d smbl=%d", nc, nc->sock,
       fd_flags, nc->flags, (int) nc->recv_mbuf.len, (int) nc->send_mbuf.len));

  if (nc->flags & MG_F_CONNECTING) {
    if (nc->flags & MG_F_UDP || nc->err != SL_EALREADY) {
      mg_if_connect_cb(nc, nc->err);
    } else {
      /* In SimpleLink, to get status of non-blocking connect() we need to wait
       * until socket is writable and repeat the call to sl_Connect again,
       * which will now return the real status. */
      if (fd_flags & _MG_F_FD_CAN_WRITE) {
        nc->err = sl_Connect(nc->sock, &nc->sa.sa, sizeof(nc->sa.sin));
        if (nc->err == SL_ESECSNOVERIFY ||
            /* TODO(rojer): Provide API to set the date for verification. */
            nc->err == SL_ESECDATEERROR) {
          nc->err = 0;
        }
        if (nc->flags & MG_F_SSL && nc->err == 0) {
          nc->flags |= MG_F_SSL_HANDSHAKE_DONE;
        }
        mg_if_connect_cb(nc, nc->err);
      }
    }
    /* Ignore read/write in further processing, we've handled it. */
    fd_flags &= ~(_MG_F_FD_CAN_READ | _MG_F_FD_CAN_WRITE);
  }

  if (fd_flags & _MG_F_FD_CAN_READ) {
    if (nc->flags & MG_F_UDP) {
      mg_handle_udp_read(nc);
    } else {
      if (nc->flags & MG_F_LISTENING) {
        mg_accept_conn(nc);
      } else {
        mg_handle_tcp_read(nc);
      }
    }
  }

  if (!(nc->flags & MG_F_CLOSE_IMMEDIATELY)) {
    if ((fd_flags & _MG_F_FD_CAN_WRITE) && nc->send_mbuf.len > 0) {
      mg_write_to_socket(nc);
    }

    if (!(fd_flags & (_MG_F_FD_CAN_READ | _MG_F_FD_CAN_WRITE))) {
      mg_if_poll(nc, now);
    }
    mg_if_timer(nc, now);
  }

  DBG(("%p after fd=%d nc_flags=%lu rmbl=%d smbl=%d", nc, nc->sock, nc->flags,
       (int) nc->recv_mbuf.len, (int) nc->send_mbuf.len));
}

/* Associate a socket to a connection. */
void mg_sock_set(struct mg_connection *nc, sock_t sock) {
  mg_set_non_blocking_mode(sock);
  nc->sock = sock;
  DBG(("%p %d", nc, sock));
}

void mg_ev_mgr_init(struct mg_mgr *mgr) {
  (void) mgr;
  DBG(("%p using sl_Select()", mgr));
}

void mg_ev_mgr_free(struct mg_mgr *mgr) {
  (void) mgr;
}

void mg_ev_mgr_add_conn(struct mg_connection *nc) {
  (void) nc;
}

void mg_ev_mgr_remove_conn(struct mg_connection *nc) {
  (void) nc;
}

time_t mg_mgr_poll(struct mg_mgr *mgr, int timeout_ms) {
  double now = mg_time();
  double min_timer;
  struct mg_connection *nc, *tmp;
  struct SlTimeval_t tv;
  fd_set read_set, write_set, err_set;
  sock_t max_fd = INVALID_SOCKET;
  int num_fds, num_ev, num_timers = 0;

  FD_ZERO(&read_set);
  FD_ZERO(&write_set);
  FD_ZERO(&err_set);

  /*
   * Note: it is ok to have connections with sock == INVALID_SOCKET in the list,
   * e.g. timer-only "connections".
   */
  min_timer = 0;
  for (nc = mgr->active_connections, num_fds = 0; nc != NULL; nc = tmp) {
    tmp = nc->next;

    if (nc->sock != INVALID_SOCKET) {
      num_fds++;

      if (!(nc->flags & MG_F_WANT_WRITE) &&
          nc->recv_mbuf.len < nc->recv_mbuf_limit &&
          (!(nc->flags & MG_F_UDP) || nc->listener == NULL)) {
        FD_SET(nc->sock, &read_set);
        if (max_fd == INVALID_SOCKET || nc->sock > max_fd) max_fd = nc->sock;
      }

      if (((nc->flags & MG_F_CONNECTING) && !(nc->flags & MG_F_WANT_READ)) ||
          (nc->send_mbuf.len > 0 && !(nc->flags & MG_F_CONNECTING))) {
        FD_SET(nc->sock, &write_set);
        FD_SET(nc->sock, &err_set);
        if (max_fd == INVALID_SOCKET || nc->sock > max_fd) max_fd = nc->sock;
      }
    }

    if (nc->ev_timer_time > 0) {
      if (num_timers == 0 || nc->ev_timer_time < min_timer) {
        min_timer = nc->ev_timer_time;
      }
      num_timers++;
    }
  }

  /*
   * If there is a timer to be fired earlier than the requested timeout,
   * adjust the timeout.
   */
  if (num_timers > 0) {
    double timer_timeout_ms = (min_timer - mg_time()) * 1000 + 1 /* rounding */;
    if (timer_timeout_ms < timeout_ms) {
      timeout_ms = timer_timeout_ms;
    }
  }
  if (timeout_ms < 0) timeout_ms = 0;

  tv.tv_sec = timeout_ms / 1000;
  tv.tv_usec = (timeout_ms % 1000) * 1000;

  num_ev = sl_Select((int) max_fd + 1, &read_set, &write_set, &err_set, &tv);
  now = mg_time();
  DBG(("sl_Select @ %ld num_ev=%d of %d, timeout=%d", (long) now, num_ev,
       num_fds, timeout_ms));

  for (nc = mgr->active_connections; nc != NULL; nc = tmp) {
    int fd_flags = 0;
    if (nc->sock != INVALID_SOCKET) {
      if (num_ev > 0) {
        fd_flags = (FD_ISSET(nc->sock, &read_set) &&
                            (!(nc->flags & MG_F_UDP) || nc->listener == NULL)
                        ? _MG_F_FD_CAN_READ
                        : 0) |
                   (FD_ISSET(nc->sock, &write_set) ? _MG_F_FD_CAN_WRITE : 0) |
                   (FD_ISSET(nc->sock, &err_set) ? _MG_F_FD_ERROR : 0);
      }
      /* SimpleLink does not report UDP sockets as writeable. */
      if (nc->flags & MG_F_UDP && nc->send_mbuf.len > 0) {
        fd_flags |= _MG_F_FD_CAN_WRITE;
      }
    }
    tmp = nc->next;
    mg_mgr_handle_conn(nc, fd_flags, now);
  }

  for (nc = mgr->active_connections; nc != NULL; nc = tmp) {
    tmp = nc->next;
    if ((nc->flags & MG_F_CLOSE_IMMEDIATELY) ||
        (nc->send_mbuf.len == 0 && (nc->flags & MG_F_SEND_AND_CLOSE))) {
      mg_close_conn(nc);
    }
  }

  return now;
}

void mg_if_get_conn_addr(struct mg_connection *nc, int remote,
                         union socket_address *sa) {
  /* SimpleLink does not provide a way to get socket's peer address after
   * accept or connect. Address hould have been preserved in the connection,
   * so we do our best here by using it. */
  if (remote) memcpy(sa, &nc->sa, sizeof(*sa));
}

void sl_restart_cb(struct mg_mgr *mgr) {
  /*
   * SimpleLink has been restarted, meaning all sockets have been invalidated.
   * We try our best - we'll restart the listeners, but for outgoing
   * connections we have no option but to terminate.
   */
  struct mg_connection *nc;
  for (nc = mg_next(mgr, NULL); nc != NULL; nc = mg_next(mgr, nc)) {
    if (nc->sock == INVALID_SOCKET) continue; /* Could be a timer */
    if (nc->flags & MG_F_LISTENING) {
      DBG(("restarting %p %s:%d", nc, inet_ntoa(nc->sa.sin.sin_addr),
           ntohs(nc->sa.sin.sin_port)));
      int res = (nc->flags & MG_F_UDP ? mg_if_listen_udp(nc, &nc->sa)
                                      : mg_if_listen_tcp(nc, &nc->sa));
      if (res == 0) continue;
      /* Well, we tried and failed. Fall through to closing. */
    }
    nc->sock = INVALID_SOCKET;
    DBG(("terminating %p %s:%d", nc, inet_ntoa(nc->sa.sin.sin_addr),
         ntohs(nc->sa.sin.sin_port)));
    /* TODO(rojer): Outgoing UDP? */
    nc->flags |= MG_F_CLOSE_IMMEDIATELY;
  }
}

#endif /* !defined(MG_DISABLE_SOCKET_IF) && defined(MG_SOCKET_SIMPLELINK) */
