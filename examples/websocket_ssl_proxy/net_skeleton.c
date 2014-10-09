// Copyright (c) 2014 Cesanta Software Limited
// All rights reserved
//
// This software is dual-licensed: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation. For the terms of this
// license, see <http://www.gnu.org/licenses/>.
//
// You are free to use this software under the terms of the GNU General
// Public License, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// Alternatively, you can license this software under a commercial
// license, as set out in <http://cesanta.com/>.
//
// $Date: 2014-09-28 05:04:41 UTC $

#include "net_skeleton.h"

#ifndef NS_MALLOC
#define NS_MALLOC malloc
#endif

#ifndef NS_REALLOC
#define NS_REALLOC realloc
#endif

#ifndef NS_FREE
#define NS_FREE free
#endif

#define NS_UDP_RECEIVE_BUFFER_SIZE  2000
#define NS_VPRINTF_BUFFER_SIZE      500

struct ctl_msg {
  ns_callback_t callback;
  char message[1024 * 8];
};

void iobuf_resize(struct iobuf *io, size_t new_size) {
  char *p;
  if ((new_size > io->size || (new_size < io->size && new_size >= io->len)) &&
      (p = (char *) NS_REALLOC(io->buf, new_size)) != NULL) {
    io->size = new_size;
    io->buf = p;
  }
}

void iobuf_init(struct iobuf *iobuf, size_t initial_size) {
  iobuf->len = iobuf->size = 0;
  iobuf->buf = NULL;
  iobuf_resize(iobuf, initial_size);
}

void iobuf_free(struct iobuf *iobuf) {
  if (iobuf != NULL) {
    if (iobuf->buf != NULL) NS_FREE(iobuf->buf);
    iobuf_init(iobuf, 0);
  }
}

size_t iobuf_append(struct iobuf *io, const void *buf, size_t len) {
  char *p = NULL;

  assert(io != NULL);
  assert(io->len <= io->size);

  if (len <= 0) {
  } else if (io->len + len <= io->size) {
    memcpy(io->buf + io->len, buf, len);
    io->len += len;
  } else if ((p = (char *) NS_REALLOC(io->buf, io->len + len)) != NULL) {
    io->buf = p;
    memcpy(io->buf + io->len, buf, len);
    io->len += len;
    io->size = io->len;
  } else {
    len = 0;
  }

  return len;
}

void iobuf_remove(struct iobuf *io, size_t n) {
  if (n > 0 && n <= io->len) {
    memmove(io->buf, io->buf + n, io->len - n);
    io->len -= n;
  }
}

static size_t ns_out(struct ns_connection *nc, const void *buf, size_t len) {
  if (nc->flags & NSF_UDP) {
    long n = sendto(nc->sock, buf, len, 0, &nc->sa.sa, sizeof(nc->sa.sin));
    DBG(("%p %d send %ld (%d %s)", nc, nc->sock, n, errno, strerror(errno)));
    return n < 0 ? 0 : n;
  } else {
    return iobuf_append(&nc->send_iobuf, buf, len);
  }
}

#ifndef NS_DISABLE_THREADS
void *ns_start_thread(void *(*f)(void *), void *p) {
#ifdef _WIN32
  return (void *) _beginthread((void (__cdecl *)(void *)) f, 0, p);
#else
  pthread_t thread_id = (pthread_t) 0;
  pthread_attr_t attr;

  (void) pthread_attr_init(&attr);
  (void) pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

#if defined(NS_STACK_SIZE) && NS_STACK_SIZE > 1
  (void) pthread_attr_setstacksize(&attr, NS_STACK_SIZE);
#endif

  pthread_create(&thread_id, &attr, f, p);
  pthread_attr_destroy(&attr);

  return (void *) thread_id;
#endif
}
#endif  // NS_DISABLE_THREADS

static void ns_add_conn(struct ns_mgr *mgr, struct ns_connection *c) {
  c->next = mgr->active_connections;
  mgr->active_connections = c;
  c->prev = NULL;
  if (c->next != NULL) c->next->prev = c;
}

static void ns_remove_conn(struct ns_connection *conn) {
  if (conn->prev == NULL) conn->mgr->active_connections = conn->next;
  if (conn->prev) conn->prev->next = conn->next;
  if (conn->next) conn->next->prev = conn->prev;
}

// Print message to buffer. If buffer is large enough to hold the message,
// return buffer. If buffer is to small, allocate large enough buffer on heap,
// and return allocated buffer.
int ns_avprintf(char **buf, size_t size, const char *fmt, va_list ap) {
  va_list ap_copy;
  int len;

  va_copy(ap_copy, ap);
  len = vsnprintf(*buf, size, fmt, ap_copy);
  va_end(ap_copy);

  if (len < 0) {
    // eCos and Windows are not standard-compliant and return -1 when
    // the buffer is too small. Keep allocating larger buffers until we
    // succeed or out of memory.
    *buf = NULL;
    while (len < 0) {
      if (*buf) free(*buf);
      size *= 2;
      if ((*buf = (char *) NS_MALLOC(size)) == NULL) break;
      va_copy(ap_copy, ap);
      len = vsnprintf(*buf, size, fmt, ap_copy);
      va_end(ap_copy);
    }
  } else if (len > (int) size) {
    // Standard-compliant code path. Allocate a buffer that is large enough.
    if ((*buf = (char *) NS_MALLOC(len + 1)) == NULL) {
      len = -1;
    } else {
      va_copy(ap_copy, ap);
      len = vsnprintf(*buf, len + 1, fmt, ap_copy);
      va_end(ap_copy);
    }
  }

  return len;
}

int ns_vprintf(struct ns_connection *nc, const char *fmt, va_list ap) {
  char mem[NS_VPRINTF_BUFFER_SIZE], *buf = mem;
  int len;

  if ((len = ns_avprintf(&buf, sizeof(mem), fmt, ap)) > 0) {
    ns_out(nc, buf, len);
  }
  if (buf != mem && buf != NULL) {
    free(buf);
  }

  return len;
}

int ns_printf(struct ns_connection *conn, const char *fmt, ...) {
  int len;
  va_list ap;
  va_start(ap, fmt);
  len = ns_vprintf(conn, fmt, ap);
  va_end(ap);
  return len;
}

static void hexdump(struct ns_connection *nc, const char *path,
                    int num_bytes, int ev) {
  const struct iobuf *io = ev == NS_SEND ? &nc->send_iobuf : &nc->recv_iobuf;
  FILE *fp;
  char *buf, src[60], dst[60];
  int buf_size = num_bytes * 5 + 100;

  if ((fp = fopen(path, "a")) != NULL) {
    ns_sock_to_str(nc->sock, src, sizeof(src), 3);
    ns_sock_to_str(nc->sock, dst, sizeof(dst), 7);
    fprintf(fp, "%lu %p %s %s %s %d\n", (unsigned long) time(NULL),
            nc->user_data, src,
            ev == NS_RECV ? "<-" : ev == NS_SEND ? "->" :
            ev == NS_ACCEPT ? "<A" : ev == NS_CONNECT ? "C>" : "XX",
            dst, num_bytes);
    if (num_bytes > 0 && (buf = (char *) NS_MALLOC(buf_size)) != NULL) {
      ns_hexdump(io->buf + (ev == NS_SEND ? 0 : io->len) -
        (ev == NS_SEND ? 0 : num_bytes), num_bytes, buf, buf_size);
      fprintf(fp, "%s", buf);
      free(buf);
    }
    fclose(fp);
  }
}

static void ns_call(struct ns_connection *nc, int ev, void *p) {
  if (nc->mgr->hexdump_file != NULL && ev != NS_POLL) {
    int len = (ev == NS_RECV || ev == NS_SEND) ? * (int *) p : 0;
    hexdump(nc, nc->mgr->hexdump_file, len, ev);
  }

  nc->callback(nc, ev, p);
}

static void ns_destroy_conn(struct ns_connection *conn) {
  closesocket(conn->sock);
  iobuf_free(&conn->recv_iobuf);
  iobuf_free(&conn->send_iobuf);
#ifdef NS_ENABLE_SSL
  if (conn->ssl != NULL) {
    SSL_free(conn->ssl);
  }
  if (conn->ssl_ctx != NULL) {
    SSL_CTX_free(conn->ssl_ctx);
  }
#endif
  NS_FREE(conn);
}

static void ns_close_conn(struct ns_connection *conn) {
  DBG(("%p %d", conn, conn->flags));
  ns_call(conn, NS_CLOSE, NULL);
  ns_remove_conn(conn);
  ns_destroy_conn(conn);
}

void ns_set_close_on_exec(sock_t sock) {
#ifdef _WIN32
  (void) SetHandleInformation((HANDLE) sock, HANDLE_FLAG_INHERIT, 0);
#else
  fcntl(sock, F_SETFD, FD_CLOEXEC);
#endif
}

static void ns_set_non_blocking_mode(sock_t sock) {
#ifdef _WIN32
  unsigned long on = 1;
  ioctlsocket(sock, FIONBIO, &on);
#else
  int flags = fcntl(sock, F_GETFL, 0);
  fcntl(sock, F_SETFL, flags | O_NONBLOCK);
#endif
}

#ifndef NS_DISABLE_SOCKETPAIR
int ns_socketpair2(sock_t sp[2], int sock_type) {
  union socket_address sa;
  sock_t sock;
  socklen_t len = sizeof(sa.sin);
  int ret = 0;

  sp[0] = sp[1] = INVALID_SOCKET;

  (void) memset(&sa, 0, sizeof(sa));
  sa.sin.sin_family = AF_INET;
  sa.sin.sin_port = htons(0);
  sa.sin.sin_addr.s_addr = htonl(0x7f000001);

  if ((sock = socket(AF_INET, sock_type, 0)) != INVALID_SOCKET &&
      !bind(sock, &sa.sa, len) &&
      (sock_type == SOCK_DGRAM || !listen(sock, 1)) &&
      !getsockname(sock, &sa.sa, &len) &&
      (sp[0] = socket(AF_INET, sock_type, 0)) != INVALID_SOCKET &&
      !connect(sp[0], &sa.sa, len) &&
      (sock_type == SOCK_STREAM ||
       (!getsockname(sp[0], &sa.sa, &len) && !connect(sock, &sa.sa, len))) &&
      (sp[1] = (sock_type == SOCK_DGRAM ? sock :
                accept(sock, &sa.sa, &len))) != INVALID_SOCKET) {
    ns_set_close_on_exec(sp[0]);
    ns_set_close_on_exec(sp[1]);
    ret = 1;
  } else {
    if (sp[0] != INVALID_SOCKET) closesocket(sp[0]);
    if (sp[1] != INVALID_SOCKET) closesocket(sp[1]);
    sp[0] = sp[1] = INVALID_SOCKET;
  }
  if (sock_type != SOCK_DGRAM) closesocket(sock);

  return ret;
}

int ns_socketpair(sock_t sp[2]) {
  return ns_socketpair2(sp, SOCK_STREAM);
}
#endif  // NS_DISABLE_SOCKETPAIR

// TODO(lsm): use non-blocking resolver
static int ns_resolve2(const char *host, struct in_addr *ina) {
  struct hostent *he;
  if ((he = gethostbyname(host)) == NULL) {
    DBG(("gethostbyname(%s) failed: %s", host, strerror(errno)));
  } else {
    memcpy(ina, he->h_addr_list[0], sizeof(*ina));
    return 1;
  }
  return 0;
}

// Resolve FDQN "host", store IP address in the "ip".
// Return > 0 (IP address length) on success.
int ns_resolve(const char *host, char *buf, size_t n) {
  struct in_addr ad;
  return ns_resolve2(host, &ad) ? snprintf(buf, n, "%s", inet_ntoa(ad)) : 0;
}

// Address format: [PROTO://][IP_ADDRESS:]PORT[:CERT][:CA_CERT]
static int ns_parse_address(const char *str, union socket_address *sa,
                            int *proto, int *use_ssl, char *cert, char *ca) {
  unsigned int a, b, c, d, port;
  int n = 0, len = 0;
  char host[200];
#ifdef NS_ENABLE_IPV6
  char buf[100];
#endif

  // MacOS needs that. If we do not zero it, subsequent bind() will fail.
  // Also, all-zeroes in the socket address means binding to all addresses
  // for both IPv4 and IPv6 (INADDR_ANY and IN6ADDR_ANY_INIT).
  memset(sa, 0, sizeof(*sa));
  sa->sin.sin_family = AF_INET;

  *proto = SOCK_STREAM;
  *use_ssl = 0;
  cert[0] = ca[0] = '\0';

  if (memcmp(str, "ssl://", 6) == 0) {
    str += 6;
    *use_ssl = 1;
  } else if (memcmp(str, "udp://", 6) == 0) {
    str += 6;
    *proto = SOCK_DGRAM;
  } else if (memcmp(str, "tcp://", 6) == 0) {
    str += 6;
  }

  if (sscanf(str, "%u.%u.%u.%u:%u%n", &a, &b, &c, &d, &port, &len) == 5) {
    // Bind to a specific IPv4 address, e.g. 192.168.1.5:8080
    sa->sin.sin_addr.s_addr = htonl((a << 24) | (b << 16) | (c << 8) | d);
    sa->sin.sin_port = htons((uint16_t) port);
#ifdef NS_ENABLE_IPV6
  } else if (sscanf(str, "[%99[^]]]:%u%n", buf, &port, &len) == 2 &&
             inet_pton(AF_INET6, buf, &sa->sin6.sin6_addr)) {
    // IPv6 address, e.g. [3ffe:2a00:100:7031::1]:8080
    sa->sin6.sin6_family = AF_INET6;
    sa->sin6.sin6_port = htons((uint16_t) port);
#endif
  } else if (sscanf(str, "%199[^ :]:%u%n", host, &port, &len) == 2) {
    sa->sin.sin_port = htons((uint16_t) port);
    ns_resolve2(host, &sa->sin.sin_addr);
  } else if (sscanf(str, "%u%n", &port, &len) == 1) {
    // If only port is specified, bind to IPv4, INADDR_ANY
    sa->sin.sin_port = htons((uint16_t) port);
  }

  if (*use_ssl && (sscanf(str + len, ":%99[^:]:%99[^:]%n", cert, ca, &n) == 2 ||
                   sscanf(str + len, ":%99[^:]%n", cert, &n) == 1)) {
    len += n;
  }

  return port < 0xffff && str[len] == '\0' ? len : 0;
}

// 'sa' must be an initialized address to bind to
static sock_t ns_open_listening_socket(union socket_address *sa, int proto) {
  socklen_t sa_len = (sa->sa.sa_family == AF_INET) ?
    sizeof(sa->sin) : sizeof(sa->sin6);
  sock_t sock = INVALID_SOCKET;
#ifndef _WIN32
  int on = 1;
#endif

  if ((sock = socket(sa->sa.sa_family, proto, 0)) != INVALID_SOCKET &&
#ifndef _WIN32
      // SO_RESUSEADDR is not enabled on Windows because the semantics of
      // SO_REUSEADDR on UNIX and Windows is different. On Windows,
      // SO_REUSEADDR allows to bind a socket to a port without error even if
      // the port is already open by another program. This is not the behavior
      // SO_REUSEADDR was designed for, and leads to hard-to-track failure
      // scenarios. Therefore, SO_REUSEADDR was disabled on Windows.
      !setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *) &on, sizeof(on)) &&
#endif
      !bind(sock, &sa->sa, sa_len) &&
      (proto == SOCK_DGRAM || listen(sock, SOMAXCONN) == 0)) {
    ns_set_non_blocking_mode(sock);
    // In case port was set to 0, get the real port number
    (void) getsockname(sock, &sa->sa, &sa_len);
  } else if (sock != INVALID_SOCKET) {
    closesocket(sock);
    sock = INVALID_SOCKET;
  }

  return sock;
}

#ifdef NS_ENABLE_SSL
// Certificate generation script is at
// https://github.com/cesanta/net_skeleton/blob/master/scripts/gen_certs.sh

static int ns_use_ca_cert(SSL_CTX *ctx, const char *cert) {
  if (ctx == NULL) {
    return -1;
  } else if (cert == NULL || cert[0] == '\0') {
    return 0;
  }
  SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, 0);
  return SSL_CTX_load_verify_locations(ctx, cert, NULL) == 1 ? 0 : -2;
}

static int ns_use_cert(SSL_CTX *ctx, const char *pem_file) {
  if (ctx == NULL) {
    return -1;
  } else if (pem_file == NULL || pem_file[0] == '\0') {
    return 0;
  } else if (SSL_CTX_use_certificate_file(ctx, pem_file, 1) == 0 ||
             SSL_CTX_use_PrivateKey_file(ctx, pem_file, 1) == 0) {
    return -2;
  } else {
    SSL_CTX_set_mode(ctx, SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);
    SSL_CTX_use_certificate_chain_file(ctx, pem_file);
    return 0;
  }
}
#endif  // NS_ENABLE_SSL

struct ns_connection *ns_bind(struct ns_mgr *srv, const char *str,
                              ns_callback_t callback, void *user_data) {
  union socket_address sa;
  struct ns_connection *nc = NULL;
  int use_ssl, proto;
  char cert[100], ca_cert[100];
  sock_t sock;

  ns_parse_address(str, &sa, &proto, &use_ssl, cert, ca_cert);
  if (use_ssl && cert[0] == '\0') return NULL;

  if ((sock = ns_open_listening_socket(&sa, proto)) == INVALID_SOCKET) {
  } else if ((nc = ns_add_sock(srv, sock, callback, NULL)) == NULL) {
    closesocket(sock);
  } else {
    nc->sa = sa;
    nc->flags |= NSF_LISTENING;
    nc->user_data = user_data;
    nc->callback = callback;

    if (proto == SOCK_DGRAM) {
      nc->flags |= NSF_UDP;
    }

#ifdef NS_ENABLE_SSL
    if (use_ssl) {
      nc->ssl_ctx = SSL_CTX_new(SSLv23_server_method());
      if (ns_use_cert(nc->ssl_ctx, cert) != 0 ||
          ns_use_ca_cert(nc->ssl_ctx, ca_cert) != 0) {
        ns_close_conn(nc);
        nc = NULL;
      }
    }
#endif

    DBG(("%p sock %d/%d ssl %p %p", nc, sock, proto, nc->ssl_ctx, nc->ssl));
  }

  return nc;
}

static struct ns_connection *accept_conn(struct ns_connection *ls) {
  struct ns_connection *c = NULL;
  union socket_address sa;
  socklen_t len = sizeof(sa);
  sock_t sock = INVALID_SOCKET;

  // NOTE(lsm): on Windows, sock is always > FD_SETSIZE
  if ((sock = accept(ls->sock, &sa.sa, &len)) == INVALID_SOCKET) {
  } else if ((c = ns_add_sock(ls->mgr, sock, ls->callback,
              ls->user_data)) == NULL) {
    closesocket(sock);
#ifdef NS_ENABLE_SSL
  } else if (ls->ssl_ctx != NULL &&
             ((c->ssl = SSL_new(ls->ssl_ctx)) == NULL ||
              SSL_set_fd(c->ssl, sock) != 1)) {
    DBG(("SSL error"));
    ns_close_conn(c);
    c = NULL;
#endif
  } else {
    c->listener = ls;
    c->proto_data = ls->proto_data;
    ns_call(c, NS_ACCEPT, &sa);
    DBG(("%p %d %p %p", c, c->sock, c->ssl_ctx, c->ssl));
  }

  return c;
}

static int ns_is_error(int n) {
  return n == 0 ||
    (n < 0 && errno != EINTR && errno != EINPROGRESS &&
     errno != EAGAIN && errno != EWOULDBLOCK
#ifdef _WIN32
     && WSAGetLastError() != WSAEINTR && WSAGetLastError() != WSAEWOULDBLOCK
#endif
    );
}

void ns_sock_to_str(sock_t sock, char *buf, size_t len, int flags) {
  union socket_address sa;
  socklen_t slen = sizeof(sa);

  if (buf != NULL && len > 0) {
    buf[0] = '\0';
    memset(&sa, 0, sizeof(sa));
    if (flags & 4) {
      getpeername(sock, &sa.sa, &slen);
    } else {
      getsockname(sock, &sa.sa, &slen);
    }
    if (flags & 1) {
#if defined(NS_ENABLE_IPV6)
      inet_ntop(sa.sa.sa_family, sa.sa.sa_family == AF_INET ?
                (void *) &sa.sin.sin_addr :
                (void *) &sa.sin6.sin6_addr, buf, len);
#elif defined(_WIN32)
      // Only Windoze Vista (and newer) have inet_ntop()
      strncpy(buf, inet_ntoa(sa.sin.sin_addr), len);
#else
      inet_ntop(sa.sa.sa_family, (void *) &sa.sin.sin_addr, buf,(socklen_t)len);
#endif
    }
    if (flags & 2) {
      snprintf(buf + strlen(buf), len - (strlen(buf) + 1), "%s%d",
               flags & 1 ? ":" : "", (int) ntohs(sa.sin.sin_port));
    }
  }
}

int ns_hexdump(const void *buf, int len, char *dst, int dst_len) {
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

#ifdef NS_ENABLE_SSL
static int ns_ssl_err(struct ns_connection *conn, int res) {
  int ssl_err = SSL_get_error(conn->ssl, res);
  if (ssl_err == SSL_ERROR_WANT_READ) conn->flags |= NSF_WANT_READ;
  if (ssl_err == SSL_ERROR_WANT_WRITE) conn->flags |= NSF_WANT_WRITE;
  return ssl_err;
}
#endif

static void ns_read_from_socket(struct ns_connection *conn) {
  char buf[2048];
  int n = 0;

  if (conn->flags & NSF_CONNECTING) {
    int ok = 1, ret;
    socklen_t len = sizeof(ok);

    ret = getsockopt(conn->sock, SOL_SOCKET, SO_ERROR, (char *) &ok, &len);
    (void) ret;
#ifdef NS_ENABLE_SSL
    if (ret == 0 && ok == 0 && conn->ssl != NULL) {
      int res = SSL_connect(conn->ssl);
      int ssl_err = ns_ssl_err(conn, res);
      if (res == 1) {
        conn->flags |= NSF_SSL_HANDSHAKE_DONE;
      } else if (ssl_err == SSL_ERROR_WANT_READ ||
                 ssl_err == SSL_ERROR_WANT_WRITE) {
        return; // Call us again
      } else {
        ok = 1;
      }
    }
#endif
    conn->flags &= ~NSF_CONNECTING;
    DBG(("%p ok=%d", conn, ok));
    if (ok != 0) {
      conn->flags |= NSF_CLOSE_IMMEDIATELY;
    }
    ns_call(conn, NS_CONNECT, &ok);
    return;
  }

#ifdef NS_ENABLE_SSL
  if (conn->ssl != NULL) {
    if (conn->flags & NSF_SSL_HANDSHAKE_DONE) {
      // SSL library may have more bytes ready to read then we ask to read.
      // Therefore, read in a loop until we read everything. Without the loop,
      // we skip to the next select() cycle which can just timeout.
      while ((n = SSL_read(conn->ssl, buf, sizeof(buf))) > 0) {
        DBG(("%p %d <- %d bytes (SSL)", conn, conn->flags, n));
        iobuf_append(&conn->recv_iobuf, buf, n);
        ns_call(conn, NS_RECV, &n);
      }
      ns_ssl_err(conn, n);
    } else {
      int res = SSL_accept(conn->ssl);
      int ssl_err = ns_ssl_err(conn, res);
      if (res == 1) {
        conn->flags |= NSF_SSL_HANDSHAKE_DONE;
      } else if (ssl_err == SSL_ERROR_WANT_READ ||
                 ssl_err == SSL_ERROR_WANT_WRITE) {
        return; // Call us again
      } else {
        conn->flags |= NSF_CLOSE_IMMEDIATELY;
      }
      return;
    }
  } else
#endif
  {
    while ((n = (int) recv(conn->sock, buf, sizeof(buf), 0)) > 0) {
      DBG(("%p %d <- %d bytes (PLAIN)", conn, conn->flags, n));
      iobuf_append(&conn->recv_iobuf, buf, n);
      ns_call(conn, NS_RECV, &n);
    }
  }

  if (ns_is_error(n)) {
    conn->flags |= NSF_CLOSE_IMMEDIATELY;
  }
}

static void ns_write_to_socket(struct ns_connection *conn) {
  struct iobuf *io = &conn->send_iobuf;
  int n = 0;

#ifdef NS_ENABLE_SSL
  if (conn->ssl != NULL) {
    n = SSL_write(conn->ssl, io->buf, io->len);
    if (n <= 0) {
      int ssl_err = ns_ssl_err(conn, n);
      if (ssl_err == SSL_ERROR_WANT_READ || ssl_err == SSL_ERROR_WANT_WRITE) {
        return; // Call us again
      } else {
        conn->flags |= NSF_CLOSE_IMMEDIATELY;
      }
    }
  } else
#endif
  { n = (int) send(conn->sock, io->buf, io->len, 0); }

  DBG(("%p %d -> %d bytes", conn, conn->flags, n));

  ns_call(conn, NS_SEND, &n);
  if (ns_is_error(n)) {
    conn->flags |= NSF_CLOSE_IMMEDIATELY;
  } else if (n > 0) {
    iobuf_remove(io, n);
  }
}

int ns_send(struct ns_connection *conn, const void *buf, int len) {
  return (int) ns_out(conn, buf, len);
}

static void ns_handle_udp(struct ns_connection *ls) {
  struct ns_connection nc;
  char buf[NS_UDP_RECEIVE_BUFFER_SIZE];
  int n;
  socklen_t s_len = sizeof(nc.sa);

  memset(&nc, 0, sizeof(nc));
  n = recvfrom(ls->sock, buf, sizeof(buf), 0, &nc.sa.sa, &s_len);
  if (n <= 0) {
    DBG(("%p recvfrom: %s", ls, strerror(errno)));
  } else {
    nc.mgr = ls->mgr;
    nc.recv_iobuf.buf = buf;
    nc.recv_iobuf.len = nc.recv_iobuf.size = n;
    nc.sock = ls->sock;
    nc.callback = ls->callback;
    nc.user_data = ls->user_data;
    nc.proto_data = ls->proto_data;
    nc.mgr = ls->mgr;
    nc.listener = ls;
    nc.flags = NSF_UDP;
    DBG(("%p %d bytes received", ls, n));
    ns_call(&nc, NS_RECV, &n);
  }
}

static void ns_add_to_set(sock_t sock, fd_set *set, sock_t *max_fd) {
  if (sock != INVALID_SOCKET) {
    FD_SET(sock, set);
    if (*max_fd == INVALID_SOCKET || sock > *max_fd) {
      *max_fd = sock;
    }
  }
}

time_t ns_mgr_poll(struct ns_mgr *mgr, int milli) {
  struct ns_connection *conn, *tmp_conn;
  struct timeval tv;
  fd_set read_set, write_set;
  sock_t max_fd = INVALID_SOCKET;
  time_t current_time = time(NULL);

  FD_ZERO(&read_set);
  FD_ZERO(&write_set);
  ns_add_to_set(mgr->ctl[1], &read_set, &max_fd);

  for (conn = mgr->active_connections; conn != NULL; conn = tmp_conn) {
    tmp_conn = conn->next;
    if (!(conn->flags & (NSF_LISTENING | NSF_CONNECTING))) {
      ns_call(conn, NS_POLL, &current_time);
    }
    if (!(conn->flags & NSF_WANT_WRITE)) {
      //DBG(("%p read_set", conn));
      ns_add_to_set(conn->sock, &read_set, &max_fd);
    }
    if (((conn->flags & NSF_CONNECTING) && !(conn->flags & NSF_WANT_READ)) ||
        (conn->send_iobuf.len > 0 && !(conn->flags & NSF_CONNECTING) &&
         !(conn->flags & NSF_BUFFER_BUT_DONT_SEND))) {
      //DBG(("%p write_set", conn));
      ns_add_to_set(conn->sock, &write_set, &max_fd);
    }
    if (conn->flags & NSF_CLOSE_IMMEDIATELY) {
      ns_close_conn(conn);
    }
  }

  tv.tv_sec = milli / 1000;
  tv.tv_usec = (milli % 1000) * 1000;

  if (select((int) max_fd + 1, &read_set, &write_set, NULL, &tv) > 0) {
    // select() might have been waiting for a long time, reset current_time
    // now to prevent last_io_time being set to the past.
    current_time = time(NULL);

    // Read wakeup messages
    if (mgr->ctl[1] != INVALID_SOCKET &&
        FD_ISSET(mgr->ctl[1], &read_set)) {
      struct ctl_msg ctl_msg;
      int len = (int) recv(mgr->ctl[1], (char *) &ctl_msg, sizeof(ctl_msg), 0);
      send(mgr->ctl[1], ctl_msg.message, 1, 0);
      if (len >= (int) sizeof(ctl_msg.callback) && ctl_msg.callback != NULL) {
        struct ns_connection *c;
        for (c = ns_next(mgr, NULL); c != NULL; c = ns_next(mgr, c)) {
          ctl_msg.callback(c, NS_POLL, ctl_msg.message);
        }
      }
    }

    for (conn = mgr->active_connections; conn != NULL; conn = tmp_conn) {
      tmp_conn = conn->next;
      if (FD_ISSET(conn->sock, &read_set)) {
        if (conn->flags & NSF_LISTENING) {
          if (conn->flags & NSF_UDP) {
            ns_handle_udp(conn);
          } else {
            // We're not looping here, and accepting just one connection at
            // a time. The reason is that eCos does not respect non-blocking
            // flag on a listening socket and hangs in a loop.
            accept_conn(conn);
          }
        } else {
          conn->last_io_time = current_time;
          ns_read_from_socket(conn);
        }
      }

      if (FD_ISSET(conn->sock, &write_set)) {
        if (conn->flags & NSF_CONNECTING) {
          ns_read_from_socket(conn);
        } else if (!(conn->flags & NSF_BUFFER_BUT_DONT_SEND)) {
          conn->last_io_time = current_time;
          ns_write_to_socket(conn);
        }
      }
    }
  }

  for (conn = mgr->active_connections; conn != NULL; conn = tmp_conn) {
    tmp_conn = conn->next;
    if ((conn->flags & NSF_CLOSE_IMMEDIATELY) ||
        (conn->send_iobuf.len == 0 &&
          (conn->flags & NSF_FINISHED_SENDING_DATA))) {
      ns_close_conn(conn);
    }
  }

  return current_time;
}

struct ns_connection *ns_connect(struct ns_mgr *mgr, const char *address,
                                 ns_callback_t callback, void *user_data) {
  sock_t sock = INVALID_SOCKET;
  struct ns_connection *nc = NULL;
  union socket_address sa;
  char cert[100], ca_cert[100];
  int rc, use_ssl, proto;

  ns_parse_address(address, &sa, &proto, &use_ssl, cert, ca_cert);
  if ((sock = socket(AF_INET, proto, 0)) == INVALID_SOCKET) {
    return NULL;
  }
  ns_set_non_blocking_mode(sock);
  rc = (proto == SOCK_DGRAM) ? 0 : connect(sock, &sa.sa, sizeof(sa.sin));

  if (rc != 0 && ns_is_error(rc)) {
    closesocket(sock);
    return NULL;
  } else if ((nc = ns_add_sock(mgr, sock, callback, user_data)) == NULL) {
    closesocket(sock);
    return NULL;
  }

  nc->sa = sa;   // Important, cause UDP conns will use sendto()
  nc->flags = (proto == SOCK_DGRAM) ? NSF_UDP : NSF_CONNECTING;

#ifdef NS_ENABLE_SSL
  if (use_ssl) {
    if ((nc->ssl_ctx = SSL_CTX_new(SSLv23_client_method())) == NULL ||
        ns_use_cert(nc->ssl_ctx, cert) != 0 ||
        ns_use_ca_cert(nc->ssl_ctx, ca_cert) != 0 ||
        (nc->ssl = SSL_new(nc->ssl_ctx)) == NULL) {
      ns_close_conn(nc);
      return NULL;
    } else {
      SSL_set_fd(nc->ssl, sock);
    }
  }
#endif

  return nc;
}

struct ns_connection *ns_add_sock(struct ns_mgr *s, sock_t sock,
                                  ns_callback_t callback, void *user_data) {
  struct ns_connection *conn;
  if ((conn = (struct ns_connection *) NS_MALLOC(sizeof(*conn))) != NULL) {
    memset(conn, 0, sizeof(*conn));
    ns_set_non_blocking_mode(sock);
    ns_set_close_on_exec(sock);
    conn->sock = sock;
    conn->user_data = user_data;
    conn->callback = callback;
    conn->mgr = s;
    conn->last_io_time = time(NULL);
    ns_add_conn(s, conn);
    DBG(("%p %d", conn, sock));
  }
  return conn;
}

struct ns_connection *ns_next(struct ns_mgr *s, struct ns_connection *conn) {
  return conn == NULL ? s->active_connections : conn->next;
}

void ns_broadcast(struct ns_mgr *mgr, ns_callback_t cb,void *data, size_t len) {
  struct ctl_msg ctl_msg;
  if (mgr->ctl[0] != INVALID_SOCKET && data != NULL &&
      len < sizeof(ctl_msg.message)) {
    ctl_msg.callback = cb;
    memcpy(ctl_msg.message, data, len);
    send(mgr->ctl[0], (char *) &ctl_msg,
         offsetof(struct ctl_msg, message) + len, 0);
    recv(mgr->ctl[0], (char *) &len, 1, 0);
  }
}

void ns_mgr_init(struct ns_mgr *s, void *user_data) {
  memset(s, 0, sizeof(*s));
  s->ctl[0] = s->ctl[1] = INVALID_SOCKET;
  s->user_data = user_data;

#ifdef _WIN32
  { WSADATA data; WSAStartup(MAKEWORD(2, 2), &data); }
#else
  // Ignore SIGPIPE signal, so if client cancels the request, it
  // won't kill the whole process.
  signal(SIGPIPE, SIG_IGN);
#endif

#ifndef NS_DISABLE_SOCKETPAIR
  do {
    ns_socketpair2(s->ctl, SOCK_DGRAM);
  } while (s->ctl[0] == INVALID_SOCKET);
#endif

#ifdef NS_ENABLE_SSL
  {static int init_done; if (!init_done) { SSL_library_init(); init_done++; }}
#endif
}

void ns_mgr_free(struct ns_mgr *s) {
  struct ns_connection *conn, *tmp_conn;

  DBG(("%p", s));
  if (s == NULL) return;
  // Do one last poll, see https://github.com/cesanta/mongoose/issues/286
  ns_mgr_poll(s, 0);

  if (s->ctl[0] != INVALID_SOCKET) closesocket(s->ctl[0]);
  if (s->ctl[1] != INVALID_SOCKET) closesocket(s->ctl[1]);
  s->ctl[0] = s->ctl[1] = INVALID_SOCKET;

  for (conn = s->active_connections; conn != NULL; conn = tmp_conn) {
    tmp_conn = conn->next;
    ns_close_conn(conn);
  }
}
