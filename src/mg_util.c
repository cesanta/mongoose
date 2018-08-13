/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#include "common/cs_base64.h"
#include "mg_internal.h"
#include "mg_util.h"

/* For platforms with limited libc */
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

const char *mg_skip(const char *s, const char *end, const char *delims,
                    struct mg_str *v) {
  v->p = s;
  while (s < end && strchr(delims, *(unsigned char *) s) == NULL) s++;
  v->len = s - v->p;
  while (s < end && strchr(delims, *(unsigned char *) s) != NULL) s++;
  return s;
}

#if MG_ENABLE_FILESYSTEM && !defined(MG_USER_FILE_FUNCTIONS)
int mg_stat(const char *path, cs_stat_t *st) {
#ifdef _WIN32
  wchar_t wpath[MG_MAX_PATH];
  to_wchar(path, wpath, ARRAY_SIZE(wpath));
  DBG(("[%ls] -> %d", wpath, _wstati64(wpath, st)));
  return _wstati64(wpath, st);
#else
  return stat(path, st);
#endif
}

FILE *mg_fopen(const char *path, const char *mode) {
#ifdef _WIN32
  wchar_t wpath[MG_MAX_PATH], wmode[10];
  to_wchar(path, wpath, ARRAY_SIZE(wpath));
  to_wchar(mode, wmode, ARRAY_SIZE(wmode));
  return _wfopen(wpath, wmode);
#else
  return fopen(path, mode);
#endif
}

int mg_open(const char *path, int flag, int mode) { /* LCOV_EXCL_LINE */
#if defined(_WIN32) && !defined(WINCE)
  wchar_t wpath[MG_MAX_PATH];
  to_wchar(path, wpath, ARRAY_SIZE(wpath));
  return _wopen(wpath, flag, mode);
#else
  return open(path, flag, mode); /* LCOV_EXCL_LINE */
#endif
}

size_t mg_fread(void *ptr, size_t size, size_t count, FILE *f) {
  return fread(ptr, size, count, f);
}

size_t mg_fwrite(const void *ptr, size_t size, size_t count, FILE *f) {
  return fwrite(ptr, size, count, f);
}
#endif

void mg_base64_encode(const unsigned char *src, int src_len, char *dst) {
  cs_base64_encode(src, src_len, dst);
}

int mg_base64_decode(const unsigned char *s, int len, char *dst) {
  return cs_base64_decode(s, len, dst, NULL);
}

#if MG_ENABLE_THREADS
void *mg_start_thread(void *(*f)(void *), void *p) {
#ifdef WINCE
  return (void *) CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) f, p, 0, NULL);
#elif defined(_WIN32)
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
#if defined(_WIN32) && !defined(WINCE)
  (void) SetHandleInformation((HANDLE) sock, HANDLE_FLAG_INHERIT, 0);
#elif defined(__unix__)
  fcntl(sock, F_SETFD, FD_CLOEXEC);
#else
  (void) sock;
#endif
}

int mg_sock_addr_to_str(const union socket_address *sa, char *buf, size_t len,
                        int flags) {
  int is_v6;
  if (buf == NULL || len <= 0) return 0;
  memset(buf, 0, len);
#if MG_ENABLE_IPV6
  is_v6 = sa->sa.sa_family == AF_INET6;
#else
  is_v6 = 0;
#endif
  if (flags & MG_SOCK_STRINGIFY_IP) {
#if MG_ENABLE_IPV6
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
      goto cleanup;
    }
#elif defined(_WIN32) || MG_LWIP || (MG_NET_IF == MG_NET_IF_PIC32)
    /* Only Windoze Vista (and newer) have inet_ntop() */
    char *addr_str = inet_ntoa(sa->sin.sin_addr);
    if (addr_str != NULL) {
      strncpy(buf, inet_ntoa(sa->sin.sin_addr), len - 1);
    } else {
      goto cleanup;
    }
#else
    if (inet_ntop(AF_INET, (void *) &sa->sin.sin_addr, buf, len) == NULL) {
      goto cleanup;
    }
#endif
  }
  if (flags & MG_SOCK_STRINGIFY_PORT) {
    int port = ntohs(sa->sin.sin_port);
    if (flags & MG_SOCK_STRINGIFY_IP) {
      int buf_len = strlen(buf);
      snprintf(buf + buf_len, len - (buf_len + 1), "%s:%d", (is_v6 ? "]" : ""),
               port);
    } else {
      snprintf(buf, len, "%d", port);
    }
  }

  return strlen(buf);

cleanup:
  *buf = '\0';
  return 0;
}

int mg_conn_addr_to_str(struct mg_connection *nc, char *buf, size_t len,
                        int flags) {
  union socket_address sa;
  memset(&sa, 0, sizeof(sa));
  mg_if_get_conn_addr(nc, flags & MG_SOCK_STRINGIFY_REMOTE, &sa);
  return mg_sock_addr_to_str(&sa, buf, len, flags);
}

#if MG_ENABLE_HEXDUMP
static int mg_hexdump_n(const void *buf, int len, char *dst, int dst_len,
                        int offset) {
  const unsigned char *p = (const unsigned char *) buf;
  char ascii[17] = "";
  int i, idx, n = 0;

  for (i = 0; i < len; i++) {
    idx = i % 16;
    if (idx == 0) {
      if (i > 0) n += snprintf(dst + n, MAX(dst_len - n, 0), "  %s\n", ascii);
      n += snprintf(dst + n, MAX(dst_len - n, 0), "%04x ", i + offset);
    }
    if (dst_len - n < 0) {
      return n;
    }
    n += snprintf(dst + n, MAX(dst_len - n, 0), " %02x", p[i]);
    ascii[idx] = p[i] < 0x20 || p[i] > 0x7e ? '.' : p[i];
    ascii[idx + 1] = '\0';
  }

  while (i++ % 16) n += snprintf(dst + n, MAX(dst_len - n, 0), "%s", "   ");
  n += snprintf(dst + n, MAX(dst_len - n, 0), "  %s\n", ascii);

  return n;
}

int mg_hexdump(const void *buf, int len, char *dst, int dst_len) {
  return mg_hexdump_n(buf, len, dst, dst_len, 0);
}

void mg_hexdumpf(FILE *fp, const void *buf, int len) {
  char tmp[80];
  int offset = 0, n;
  while (len > 0) {
    n = (len < 16 ? len : 16);
    mg_hexdump_n(((const char *) buf) + offset, n, tmp, sizeof(tmp), offset);
    fputs(tmp, fp);
    offset += n;
    len -= n;
  }
}

void mg_hexdump_connection(struct mg_connection *nc, const char *path,
                           const void *buf, int num_bytes, int ev) {
  FILE *fp = NULL;
  char src[60], dst[60];
  const char *tag = NULL;
  switch (ev) {
    case MG_EV_RECV:
      tag = "<-";
      break;
    case MG_EV_SEND:
      tag = "->";
      break;
    case MG_EV_ACCEPT:
      tag = "<A";
      break;
    case MG_EV_CONNECT:
      tag = "C>";
      break;
    case MG_EV_CLOSE:
      tag = "XX";
      break;
  }
  if (tag == NULL) return; /* Don't log MG_EV_TIMER, etc */

  if (strcmp(path, "-") == 0) {
    fp = stdout;
  } else if (strcmp(path, "--") == 0) {
    fp = stderr;
#if MG_ENABLE_FILESYSTEM
  } else {
    fp = mg_fopen(path, "a");
#endif
  }
  if (fp == NULL) return;

  mg_conn_addr_to_str(nc, src, sizeof(src),
                      MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
  mg_conn_addr_to_str(nc, dst, sizeof(dst), MG_SOCK_STRINGIFY_IP |
                                                MG_SOCK_STRINGIFY_PORT |
                                                MG_SOCK_STRINGIFY_REMOTE);
  fprintf(fp, "%lu %p %s %s %s %d\n", (unsigned long) mg_time(), (void *) nc,
          src, tag, dst, (int) num_bytes);
  if (num_bytes > 0) {
    mg_hexdumpf(fp, buf, num_bytes);
  }
  if (fp != stdout && fp != stderr) fclose(fp);
}
#endif

int mg_is_big_endian(void) {
  static const int n = 1;
  /* TODO(mkm) use compiletime check with 4-byte char literal */
  return ((char *) &n)[0] == 0;
}

DO_NOT_WARN_UNUSED MG_INTERNAL int mg_get_errno(void) {
#ifndef WINCE
  return errno;
#else
  /* TODO(alashkin): translate error codes? */
  return GetLastError();
#endif
}

void mg_mbuf_append_base64_putc(char ch, void *user_data) {
  struct mbuf *mbuf = (struct mbuf *) user_data;
  mbuf_append(mbuf, &ch, sizeof(ch));
}

void mg_mbuf_append_base64(struct mbuf *mbuf, const void *data, size_t len) {
  struct cs_base64_ctx ctx;
  cs_base64_init(&ctx, mg_mbuf_append_base64_putc, mbuf);
  cs_base64_update(&ctx, (const char *) data, len);
  cs_base64_finish(&ctx);
}

void mg_basic_auth_header(const struct mg_str user, const struct mg_str pass,
                          struct mbuf *buf) {
  const char *header_prefix = "Authorization: Basic ";
  const char *header_suffix = "\r\n";

  struct cs_base64_ctx ctx;
  cs_base64_init(&ctx, mg_mbuf_append_base64_putc, buf);

  mbuf_append(buf, header_prefix, strlen(header_prefix));

  cs_base64_update(&ctx, user.p, user.len);
  if (pass.len > 0) {
    cs_base64_update(&ctx, ":", 1);
    cs_base64_update(&ctx, pass.p, pass.len);
  }
  cs_base64_finish(&ctx);
  mbuf_append(buf, header_suffix, strlen(header_suffix));
}

struct mg_str mg_url_encode_opt(const struct mg_str src,
                                const struct mg_str safe, unsigned int flags) {
  const char *hex =
      (flags & MG_URL_ENCODE_F_UPPERCASE_HEX ? "0123456789ABCDEF"
                                             : "0123456789abcdef");
  size_t i = 0;
  struct mbuf mb;
  mbuf_init(&mb, src.len);

  for (i = 0; i < src.len; i++) {
    const unsigned char c = *((const unsigned char *) src.p + i);
    if (isalnum(c) || mg_strchr(safe, c) != NULL) {
      mbuf_append(&mb, &c, 1);
    } else if (c == ' ' && (flags & MG_URL_ENCODE_F_SPACE_AS_PLUS)) {
      mbuf_append(&mb, "+", 1);
    } else {
      mbuf_append(&mb, "%", 1);
      mbuf_append(&mb, &hex[c >> 4], 1);
      mbuf_append(&mb, &hex[c & 15], 1);
    }
  }
  mbuf_append(&mb, "", 1);
  mbuf_trim(&mb);
  return mg_mk_str_n(mb.buf, mb.len - 1);
}

struct mg_str mg_url_encode(const struct mg_str src) {
  return mg_url_encode_opt(src, mg_mk_str("._-$,;~()/"), 0);
}
