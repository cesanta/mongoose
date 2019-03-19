/*
 * Copyright (c) 2014 Cesanta Software Limited
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

#include "unit_test.h"
#include "common/cs_md5.h"
#include "common/test_main.h"
#include "common/test_util.h"
#include "mongoose.h"
#include "src/mg_internal.h"

#if defined(__STDC_VERSION__) && __STDC_VERSION__ < 199901L && !defined(WIN32)
#define __func__ ""
#endif

#define FETCH_BUF_SIZE (1024 * 16)

#define HTTP_PORT "45772"
#define LOOPBACK_IP "127.0.0.1"
#define LISTENING_ADDR LOOPBACK_IP ":" HTTP_PORT

static struct mg_serve_http_opts s_http_server_opts;
static int s_listening_port = 23456;

#define TEST_MG_MALLOC malloc
#define TEST_MG_CALLOC calloc

#ifndef intptr_t
#define intptr_t long
#endif

void *(*test_malloc)(size_t) = TEST_MG_MALLOC;
void *(*test_calloc)(size_t, size_t) = TEST_MG_CALLOC;

void *failing_malloc(size_t size) {
  (void) size;
  return NULL;
}

void *failing_calloc(size_t count, size_t size) {
  (void) count;
  (void) size;
  return NULL;
}

static char *read_file(const char *path, size_t *size) {
  FILE *fp;
  struct stat st;
  char *data = NULL;
  if ((fp = mg_fopen(path, "rb")) != NULL && !fstat(fileno(fp), &st)) {
    *size = st.st_size;
    data = (char *) malloc(*size);
    fread(data, 1, *size, fp);
    fclose(fp);
  }
  return data;
}

void msleep(int millis) {
#if defined(_WIN32)
  Sleep(millis);
#else
  usleep(millis * 1000);
#endif
}

static struct mg_iface *test_iface = NULL;

static void init_test_connection(struct mg_connection *nc) {
  memset(nc, 0, sizeof(*nc));
  nc->iface = test_iface;
}

static struct mg_connection *create_test_connection() {
  struct mg_connection *nc = (struct mg_connection *) calloc(1, sizeof(*nc));
  init_test_connection(nc);
  return nc;
}

void mg_destroy_conn(struct mg_connection *conn, int destroy_if);
static void destroy_test_connection(struct mg_connection *nc) {
  mg_destroy_conn(nc, 0 /* destroy_if */);
}

static const char *test_mbuf(void) {
  struct mbuf io;
  const char *data = "TEST";
  const char *prefix = "MY";
  const char *big_prefix = "Some long prefix: ";
  size_t old_size;

  mbuf_init(&io, 0);
  ASSERT(io.buf == NULL);
  ASSERT_EQ(io.len, 0);
  ASSERT_EQ(io.size, 0);
  mbuf_free(&io);
  ASSERT(io.buf == NULL);
  ASSERT_EQ(io.len, 0);
  ASSERT_EQ(io.size, 0);

  mbuf_init(&io, 10);
  ASSERT(io.buf != NULL);
  ASSERT_EQ(io.len, 0);
  ASSERT_EQ(io.size, 10);
  mbuf_free(&io);
  ASSERT(io.buf == NULL);
  ASSERT_EQ(io.len, 0);
  ASSERT_EQ(io.size, 0);

  mbuf_init(&io, 10);
  ASSERT_EQ(mbuf_append(&io, NULL, 0), 0);
  /* test allocation failure */
  ASSERT_EQ(mbuf_append(&io, NULL, 1125899906842624), 0);

  ASSERT_EQ(mbuf_append(&io, data, strlen(data)), strlen(data));

  mbuf_resize(&io, 2);
  ASSERT_EQ(io.size, 10);
  ASSERT_EQ(io.len, strlen(data));

  ASSERT_EQ(mbuf_insert(&io, 0, prefix, strlen(prefix)), strlen(prefix));
  ASSERT_EQ(io.size, 10);
  ASSERT_EQ(io.len, strlen(data) + strlen(prefix));

  ASSERT_EQ(mbuf_insert(&io, 0, big_prefix, strlen(big_prefix)),
            strlen(big_prefix));
  ASSERT_EQ(io.size, MBUF_SIZE_MULTIPLIER *
                         (strlen(big_prefix) + strlen(prefix) + strlen(data)));
  ASSERT_STREQ_NZ(io.buf, "Some long prefix: MYTEST");

  old_size = io.size;
  ASSERT_EQ(mbuf_insert(&io, strlen(big_prefix), data, strlen(data)),
            strlen(data));
  ASSERT_EQ(io.size, old_size);
  ASSERT_STREQ_NZ(io.buf, "Some long prefix: TESTMYTEST");

  /* test allocation failure */
  ASSERT_EQ(mbuf_insert(&io, 0, NULL, 1125899906842624), 0);

  /* test overflow */
  ASSERT_EQ(mbuf_insert(&io, 0, NULL, -1), 0);
  mbuf_free(&io);
  return NULL;
}

static void eh1(struct mg_connection *nc, int ev, void *ev_data) {
  struct mbuf *io = &nc->recv_mbuf;

  switch (ev) {
    case MG_EV_CONNECT: {
      int res = *((int *) ev_data);
      if (res == 0) {
        mg_printf(nc, "%d %s there", *(int *) ev_data, "hi");
      } else {
        sprintf((char *) nc->user_data, "connect failed! %d", res);
      }
      break;
    }
    case MG_EV_RECV: {
      if (nc->listener != NULL) {
        mg_printf(nc, "%d", (int) io->len);
        mbuf_remove(io, io->len);
      } else if (io->len > 0) {
        sprintf((char *) nc->user_data, "%sok!",
                (io->len == 2 && memcmp(io->buf, "10", 2) == 0) ? "" : "NOT ");
        DBG(("%s", (const char *) nc->user_data));
        nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      }
      break;
    }
    default:
      break;
  }
}

#define S_PEM "server.pem"
#define C_PEM "client.pem"
#define CA_PEM "ca.pem"

static const char *test_mgr_with_ssl(int use_ssl) {
  char addr[100] = "127.0.0.1:0", ip[sizeof(addr)], buf[100] = "",
       short_addr[5];
  struct mg_mgr mgr;
  struct mg_connection *nc;
  struct mg_bind_opts bopts;
  int port, port2;
#if !MG_ENABLE_SSL
  (void) use_ssl;
#endif

  mg_mgr_init(&mgr, NULL);
  /* mgr.hexdump_file = "-"; */

  memset(&bopts, 0, sizeof(bopts));
#if MG_ENABLE_SSL
  if (use_ssl) {
    bopts.ssl_cert = S_PEM;
    bopts.ssl_ca_cert = CA_PEM;
  }
#endif
  ASSERT((nc = mg_bind_opt(&mgr, addr, eh1, bopts)) != NULL);
  port2 = htons(nc->sa.sin.sin_port);
  ASSERT(port2 > 0);

  mg_sock_to_str(nc->sock, addr, sizeof(addr), 3);
  ASSERT_EQ(sscanf(addr, "%[^:]:%d", ip, &port), 2);
  ASSERT_STREQ(ip, "127.0.0.1");
  ASSERT_EQ(port, port2);

  mg_sock_to_str(nc->sock, short_addr, sizeof(short_addr), 3);
#ifndef _WIN32
  ASSERT_STREQ(short_addr, "");
#else
  /* We use `inet_ntoa` in Windows, so, can get partial result */
  ASSERT_STREQ(short_addr, "127.");
#endif

  ASSERT((nc = mg_connect(&mgr, addr, eh1)) != NULL);
#if MG_ENABLE_SSL
  if (use_ssl) {
    ASSERT(mg_set_ssl(nc, C_PEM, CA_PEM) == NULL);
  }
#endif
  nc->user_data = buf;
  poll_until(&mgr, 1, c_str_ne, buf, (void *) "");
  ASSERT_STREQ(buf, "ok!");
  buf[0] = '\0';

#if MG_ENABLE_SSL
  if (use_ssl) {
    struct mg_connect_opts opts;
    memset(&opts, 0, sizeof(opts));
    opts.user_data = buf;
    opts.ssl_cert = C_PEM;
    opts.ssl_ca_cert = CA_PEM;
    opts.ssl_server_name = "*"; /* TODO(rojer): Test this too. */
    ASSERT((nc = mg_connect_opt(&mgr, addr, eh1, opts)) != NULL);
    poll_until(&mgr, 1, c_str_ne, buf, (void *) "");
    ASSERT_STREQ(buf, "ok!");
    buf[0] = '\0';
  }
#endif

  mg_mgr_free(&mgr);
  return NULL;
}

static const char *test_mgr(void) {
  return test_mgr_with_ssl(0);
}

#if MG_ENABLE_SSL
void *run_manager(void *param) {
  struct mg_mgr *mgr = (struct mg_mgr *) param;
  while (((intptr_t) mgr->user_data) == 0) {
    mg_mgr_poll(mgr, 1000);
  }
  mg_mgr_free(mgr);
  free(mgr);
  return NULL;
}

static const char *test_ssl(void) {
  return test_mgr_with_ssl(1);
}

/*
 * unfortunately krypton has no BIO so it's hard to test this way
 * We still set MG_ENABLE_SSL so that vc6 has a binary to build
 */
#ifdef OPENSSL_VERSION_NUMBER
static void eh_hello_server(struct mg_connection *nc, int ev, void *ev_data) {
  (void) ev_data;
  if (ev == MG_EV_ACCEPT) mg_printf(nc, "hello");
}

static const char *test_modern_crypto(void) {
  char addr[100] = "127.0.0.1:8000";
  struct mg_mgr *mgr = (struct mg_mgr *) malloc(sizeof(*mgr));
  struct mg_connection *nc;
  int port;

  mg_mgr_init(mgr, NULL);

  ASSERT((nc = mg_bind(mgr, addr, eh_hello_server)) != NULL);
  port = htons(nc->sa.sin.sin_port);
  ASSERT(port > 0);
  ASSERT(mg_set_ssl(nc, S_PEM, NULL /* no client certs */) == NULL);
  mg_sock_to_str(nc->sock, addr, sizeof(addr),
                 MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
  mg_start_thread(run_manager, mgr);

  {
    char buf[100];
/* For older OpenSSL version we have to allow older digests. Then it still tests
 * DH. */
#if OPENSSL_VERSION_NUMBER < 0x10000000
    const char *ciphers = "DH:!ADH:AES:MD5:SHA1";
#else
    const char *ciphers = "DH:!ADH:AES:!MD5:!SHA1";
#endif

    SSL_CTX *ctx = NULL;
    BIO *bio = NULL;
    SSL *ssl = NULL;

    ctx = SSL_CTX_new(SSLv23_client_method());
    ASSERT(ctx != NULL);
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    SSL_CTX_set_verify_depth(ctx, 1);
    /*
     * These are pretty restrictive settings and should satisfy e.g.
     * Chrome's "modern cryptography" requirements.
     */
    ASSERT_EQ(SSL_CTX_set_cipher_list(nc->ssl_ctx, ciphers), 1);
    ASSERT_EQ(SSL_CTX_load_verify_locations(ctx, CA_PEM, NULL), 1);
    bio = BIO_new_ssl_connect(ctx);
    ASSERT(bio != NULL);
    BIO_get_ssl(bio, &ssl);
    ASSERT(ssl != NULL);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
    BIO_set_conn_hostname(bio, addr);
    ASSERT_EQ(BIO_do_connect(bio), 1);
    ASSERT_EQ(BIO_do_handshake(bio), 1);
    ASSERT_EQ(SSL_get_verify_result(ssl), X509_V_OK);
    ASSERT_EQ(BIO_read(bio, buf, 5), 5);
    ASSERT_STREQ_NZ(buf, "hello");
    BIO_free_all(bio);
    SSL_CTX_free(ctx);
  }

  mgr->user_data = (void *) 1;
  return NULL;
}
#endif /* OPENSSL_VERSION_NUMBER */
#endif /* MG_ENABLE_SSL */

static const char *test_to64(void) {
  ASSERT_EQ(to64("0"), 0);
  ASSERT_EQ(to64(""), 0);
  ASSERT_EQ(to64("123"), 123);
  ASSERT_EQ(to64("-34"), -34);
  ASSERT_EQ(to64("3566626116"), 3566626116U);
  return NULL;
}

static const char *test_check_ip_acl(void) {
  uint32_t ip = 0x01020304;
  ASSERT_EQ(mg_check_ip_acl(NULL, ip), 1);
  ASSERT_EQ(mg_check_ip_acl("", ip), 1);
  ASSERT_EQ(mg_check_ip_acl("invalid", ip), -1);
  ASSERT_EQ(mg_check_ip_acl("-0.0.0.0/0", ip), 0);
  ASSERT_EQ(mg_check_ip_acl("-0.0.0.0/0,+1.0.0.0/8", ip), 1);
  ASSERT_EQ(mg_check_ip_acl("-0.0.0.0/0,+1.2.3.4", ip), 1);
  ASSERT_EQ(mg_check_ip_acl("-0.0.0.0/0,+1.0.0.0/16", ip), 0);
  return NULL;
}

static const char *test_parse_uri(void) {
  struct mg_str uri_out;
  struct mg_str scheme, user_info, host, path, query, fragment;
  unsigned int port;
  {
    struct mg_str uri = MG_MK_STR("foo");
    ASSERT_EQ(mg_parse_uri(uri, NULL, NULL, NULL, NULL, NULL, NULL, NULL), 0);
  }

  {
    struct mg_str uri = MG_MK_STR("http://user:pw@host:80/foo?bar#baz");
    ASSERT_EQ(mg_parse_uri(uri, &scheme, &user_info, &host, &port, &path,
                           &query, &fragment),
              0);

    ASSERT_MG_STREQ(scheme, "http");
    ASSERT_MG_STREQ(user_info, "user:pw");
    ASSERT_MG_STREQ(host, "host");
    ASSERT_EQ(port, 80);
    ASSERT_MG_STREQ(path, "/foo");
    ASSERT_MG_STREQ(query, "bar");
    ASSERT_MG_STREQ(fragment, "baz");

    ASSERT_EQ(mg_assemble_uri(&scheme, &user_info, &host, port, &path, &query,
                              &fragment, 0, &uri_out),
              0);
    ASSERT_MG_STREQ(uri_out, uri.p);
    free((void *) uri_out.p);
  }

  {
    struct mg_str uri = MG_MK_STR("http://host:80/foo?bar");
    ASSERT_EQ(mg_parse_uri(uri, &scheme, &user_info, &host, &port, &path,
                           &query, &fragment),
              0);

    ASSERT_MG_STREQ(scheme, "http");
    ASSERT_MG_STREQ(user_info, "");
    ASSERT_MG_STREQ(host, "host");
    ASSERT_EQ(port, 80);
    ASSERT_MG_STREQ(path, "/foo");
    ASSERT_MG_STREQ(query, "bar");
    ASSERT_MG_STREQ(fragment, "");

    ASSERT_EQ(mg_assemble_uri(&scheme, &user_info, &host, port, &path, &query,
                              &fragment, 0, &uri_out),
              0);
    ASSERT_MG_STREQ(uri_out, uri.p);
    free((void *) uri_out.p);
  }

  {
    struct mg_str uri = MG_MK_STR("http://host:80/foo#baz");
    ASSERT_EQ(mg_parse_uri(uri, &scheme, &user_info, &host, &port, &path,
                           &query, &fragment),
              0);

    ASSERT_MG_STREQ(scheme, "http");
    ASSERT_MG_STREQ(user_info, "");
    ASSERT_MG_STREQ(host, "host");
    ASSERT_EQ(port, 80);
    ASSERT_MG_STREQ(path, "/foo");
    ASSERT_MG_STREQ(query, "");
    ASSERT_MG_STREQ(fragment, "baz");

    ASSERT_EQ(mg_assemble_uri(&scheme, &user_info, &host, port, &path, &query,
                              &fragment, 0, &uri_out),
              0);
    ASSERT_MG_STREQ(uri_out, uri.p);
    free((void *) uri_out.p);
  }

  {
    struct mg_str uri = MG_MK_STR("http://host:80/foo");
    ASSERT_EQ(mg_parse_uri(uri, &scheme, &user_info, &host, &port, &path,
                           &query, &fragment),
              0);

    ASSERT_MG_STREQ(scheme, "http");
    ASSERT_MG_STREQ(user_info, "");
    ASSERT_MG_STREQ(host, "host");
    ASSERT_EQ(port, 80);
    ASSERT_MG_STREQ(path, "/foo");
    ASSERT_MG_STREQ(query, "");
    ASSERT_MG_STREQ(fragment, "");

    ASSERT_EQ(mg_assemble_uri(&scheme, &user_info, &host, port, &path, &query,
                              &fragment, 0, &uri_out),
              0);
    ASSERT_MG_STREQ(uri_out, uri.p);
    free((void *) uri_out.p);
  }

  {
    struct mg_str uri = MG_MK_STR("http://host/foo");
    ASSERT_EQ(mg_parse_uri(uri, &scheme, &user_info, &host, &port, &path,
                           &query, &fragment),
              0);

    ASSERT_MG_STREQ(scheme, "http");
    ASSERT_MG_STREQ(user_info, "");
    ASSERT_MG_STREQ(host, "host");
    ASSERT_EQ(port, 0);
    ASSERT_MG_STREQ(path, "/foo");
    ASSERT_MG_STREQ(query, "");
    ASSERT_MG_STREQ(fragment, "");

    ASSERT_EQ(mg_assemble_uri(&scheme, &user_info, &host, port, &path, &query,
                              &fragment, 0, &uri_out),
              0);
    ASSERT_MG_STREQ(uri_out, uri.p);
    free((void *) uri_out.p);
  }

  {
    struct mg_str uri = MG_MK_STR("http://host");
    ASSERT_EQ(mg_parse_uri(uri, &scheme, &user_info, &host, &port, &path,
                           &query, &fragment),
              0);
    ASSERT_MG_STREQ(scheme, "http");
    ASSERT_MG_STREQ(user_info, "");
    ASSERT_MG_STREQ(host, "host");
    ASSERT_EQ(port, 0);
    ASSERT_MG_STREQ(path, "");
    ASSERT_MG_STREQ(query, "");
    ASSERT_MG_STREQ(fragment, "");

    ASSERT_EQ(mg_assemble_uri(&scheme, &user_info, &host, port, &path, &query,
                              &fragment, 0, &uri_out),
              0);
    ASSERT_MG_STREQ(uri_out, uri.p);
    free((void *) uri_out.p);
  }

  {
    struct mg_str uri = MG_MK_STR("http://host:80");
    ASSERT_EQ(mg_parse_uri(uri, &scheme, &user_info, &host, &port, &path,
                           &query, &fragment),
              0);
    ASSERT_MG_STREQ(scheme, "http");
    ASSERT_MG_STREQ(user_info, "");
    ASSERT_MG_STREQ(host, "host");
    ASSERT_EQ(port, 80);
    ASSERT_MG_STREQ(path, "");
    ASSERT_MG_STREQ(query, "");
    ASSERT_MG_STREQ(fragment, "");

    ASSERT_EQ(mg_assemble_uri(&scheme, &user_info, &host, port, &path, &query,
                              &fragment, 0, &uri_out),
              0);
    ASSERT_MG_STREQ(uri_out, uri.p);
    free((void *) uri_out.p);
  }

  {
    struct mg_str uri = MG_MK_STR("tcp://1.2.3.4:5678");
    ASSERT_EQ(mg_parse_uri(uri, &scheme, &user_info, &host, &port, &path,
                           &query, &fragment),
              0);
    ASSERT_MG_STREQ(scheme, "tcp");
    ASSERT_MG_STREQ(user_info, "");
    ASSERT_MG_STREQ(host, "1.2.3.4");
    ASSERT_EQ(port, 5678);
    ASSERT_MG_STREQ(path, "");
    ASSERT_MG_STREQ(query, "");
    ASSERT_MG_STREQ(fragment, "");

    ASSERT_EQ(mg_assemble_uri(&scheme, &user_info, &host, port, &path, &query,
                              &fragment, 0, &uri_out),
              0);
    ASSERT_MG_STREQ(uri_out, uri.p);
    free((void *) uri_out.p);
  }

  {
    const struct mg_str uri = MG_MK_STR("tcp://[::1]:234");
    ASSERT_EQ(mg_parse_uri(uri, &scheme, &user_info, &host, &port, &path,
                           &query, &fragment),
              0);
    ASSERT_MG_STREQ(scheme, "tcp");
    ASSERT_MG_STREQ(user_info, "");
    ASSERT_MG_STREQ(host, "[::1]");
    ASSERT_EQ(port, 234);
    ASSERT_EQ(path.len, 0);
    ASSERT_EQ(query.len, 0);
    ASSERT_EQ(fragment.len, 0);
    ASSERT_MG_STREQ(path, "");
    ASSERT_MG_STREQ(query, "");
    ASSERT_MG_STREQ(fragment, "");

    ASSERT_EQ(mg_assemble_uri(&scheme, &user_info, &host, port, &path, &query,
                              &fragment, 0, &uri_out),
              0);
    ASSERT_MG_STREQ(uri_out, uri.p);
    free((void *) uri_out.p);
  }

  {
    const struct mg_str uri = MG_MK_STR("tcp://[::1]");
    ASSERT_EQ(mg_parse_uri(uri, &scheme, &user_info, &host, &port, &path,
                           &query, &fragment),
              0);
    ASSERT_MG_STREQ(scheme, "tcp");
    ASSERT_MG_STREQ(user_info, "");
    ASSERT_MG_STREQ(host, "[::1]");
    ASSERT_EQ(port, 0);
    ASSERT_MG_STREQ(path, "");
    ASSERT_MG_STREQ(query, "");
    ASSERT_MG_STREQ(fragment, "");

    ASSERT_EQ(mg_assemble_uri(&scheme, &user_info, &host, port, &path, &query,
                              &fragment, 0, &uri_out),
              0);
    ASSERT_MG_STREQ(uri_out, uri.p);
    free((void *) uri_out.p);
  }

  {
    const struct mg_str uri = MG_MK_STR("tcp://[::1");
    ASSERT_EQ(mg_parse_uri(uri, &scheme, &user_info, &host, &port, &path,
                           &query, &fragment),
              -1);
  }

  {
    struct mg_str uri = MG_MK_STR("host:80");
    ASSERT_EQ(mg_parse_uri(uri, &scheme, &user_info, &host, &port, &path,
                           &query, &fragment),
              0);
    ASSERT_MG_STREQ(scheme, "");
    ASSERT_MG_STREQ(user_info, "");
    ASSERT_MG_STREQ(host, "host");
    ASSERT_EQ(port, 80);
    ASSERT_MG_STREQ(path, "");
    ASSERT_MG_STREQ(query, "");
    ASSERT_MG_STREQ(fragment, "");

    ASSERT_EQ(mg_assemble_uri(&scheme, &user_info, &host, port, &path, &query,
                              &fragment, 0, &uri_out),
              0);
    ASSERT_MG_STREQ(uri_out, uri.p);
    free((void *) uri_out.p);
  }

  {
    const struct mg_str uri = MG_MK_STR("1.2.3.4:56789");
    ASSERT_EQ(mg_parse_uri(uri, &scheme, &user_info, &host, &port, &path,
                           &query, &fragment),
              0);
    ASSERT_MG_STREQ(scheme, "");
    ASSERT_MG_STREQ(user_info, "");
    ASSERT_MG_STREQ(host, "1.2.3.4");
    ASSERT_EQ(port, 56789);
    ASSERT_MG_STREQ(path, "");
    ASSERT_MG_STREQ(query, "");
    ASSERT_MG_STREQ(fragment, "");

    ASSERT_EQ(mg_assemble_uri(&scheme, &user_info, &host, port, &path, &query,
                              &fragment, 0, &uri_out),
              0);
    ASSERT_MG_STREQ(uri_out, uri.p);
    free((void *) uri_out.p);
  }

  {
    const struct mg_str uri = MG_MK_STR("[::1]:2345");
    ASSERT_EQ(mg_parse_uri(uri, &scheme, &user_info, &host, &port, &path,
                           &query, &fragment),
              0);
    ASSERT_MG_STREQ(scheme, "");
    ASSERT_MG_STREQ(user_info, "");
    ASSERT_MG_STREQ(host, "[::1]");
    ASSERT_EQ(port, 2345);
    ASSERT_MG_STREQ(path, "");
    ASSERT_MG_STREQ(query, "");
    ASSERT_MG_STREQ(fragment, "");

    ASSERT_EQ(mg_assemble_uri(&scheme, &user_info, &host, port, &path, &query,
                              &fragment, 0, &uri_out),
              0);
    ASSERT_MG_STREQ(uri_out, uri.p);
    free((void *) uri_out.p);
  }

  {
    const struct mg_str uri = MG_MK_STR("[::1]");
    ASSERT_EQ(mg_parse_uri(uri, &scheme, &user_info, &host, &port, &path,
                           &query, &fragment),
              0);
    ASSERT_MG_STREQ(scheme, "");
    ASSERT_MG_STREQ(user_info, "");
    ASSERT_MG_STREQ(host, "[::1]");
    ASSERT_EQ(port, 0);
    ASSERT_MG_STREQ(path, "");
    ASSERT_MG_STREQ(query, "");
    ASSERT_MG_STREQ(fragment, "");

    ASSERT_EQ(mg_assemble_uri(&scheme, &user_info, &host, port, &path, &query,
                              &fragment, 0, &uri_out),
              0);
    ASSERT_MG_STREQ(uri_out, uri.p);
    free((void *) uri_out.p);
  }

  {
    struct mg_str uri = MG_MK_STR("host/foo");
    ASSERT_EQ(mg_parse_uri(uri, &scheme, &user_info, &host, &port, &path,
                           &query, &fragment),
              0);
    ASSERT_MG_STREQ(scheme, "");
    ASSERT_MG_STREQ(user_info, "");
    ASSERT_MG_STREQ(host, "host");
    ASSERT_EQ(port, 0);
    ASSERT_MG_STREQ(path, "/foo");
    ASSERT_EQ(query.len, 0);
    ASSERT_EQ(fragment.len, 0);

    ASSERT_EQ(mg_assemble_uri(&scheme, &user_info, &host, port, &path, &query,
                              &fragment, 0, &uri_out),
              0);
    ASSERT_MG_STREQ(uri_out, uri.p);
    free((void *) uri_out.p);
  }

  {
    struct mg_str uri = MG_MK_STR("https://a.com/foo/x@y.z/fw.zip");
    ASSERT_EQ(mg_parse_uri(uri, &scheme, &user_info, &host, &port, &path,
                           &query, &fragment),
              0);
    ASSERT_MG_STREQ(scheme, "https");
    ASSERT_MG_STREQ(user_info, "");
    ASSERT_MG_STREQ(host, "a.com");
    ASSERT_EQ(port, 0);
    ASSERT_MG_STREQ(path, "/foo/x@y.z/fw.zip");
    ASSERT_EQ(query.len, 0);
    ASSERT_EQ(fragment.len, 0);

    ASSERT_EQ(mg_assemble_uri(&scheme, &user_info, &host, port, &path, &query,
                              &fragment, 0, &uri_out),
              0);
    ASSERT_MG_STREQ(uri_out, uri.p);
    free((void *) uri_out.p);
  }

  return NULL;
}

static const char *test_assemble_uri(void) {
  struct mg_str scheme, path, uri;
  {
    ASSERT_EQ(mg_assemble_uri(NULL, NULL, NULL, 0, NULL, NULL, NULL, 0, &uri),
              0);
    ASSERT_MG_STREQ(uri, "");
  }

  {
    scheme = mg_mk_str("file");
    path = mg_mk_str("/foo/bar");
    ASSERT_EQ(
        mg_assemble_uri(&scheme, NULL, NULL, 0, &path, NULL, NULL, 0, &uri), 0);
    ASSERT_MG_STREQ(uri, "file:///foo/bar");
    free((void *) uri.p);
  }

  {
    scheme = mg_mk_str("file");
    path = mg_mk_str("/foo/.././bar/baz");
    ASSERT_EQ(
        mg_assemble_uri(&scheme, NULL, NULL, 0, &path, NULL, NULL, 0, &uri), 0);
    ASSERT_MG_STREQ(uri, "file:///foo/.././bar/baz");
    free((void *) uri.p);
    ASSERT_EQ(
        mg_assemble_uri(&scheme, NULL, NULL, 0, &path, NULL, NULL, 1, &uri), 0);
    ASSERT_MG_STREQ(uri, "file:///bar/baz");
    free((void *) uri.p);
  }

  return NULL;
}

/* TODO(mkm) port these test cases to the new async parse_address */
static const char *test_parse_address(void) {
  static const char *valid[] = {
    "1",
    "1.2.3.4:1",
    "tcp://123",
    "udp://0.0.0.0:99",
#ifndef _WIN32 /* No /etc/hosts on Windows. */
    "tcp://localhost:99",
#endif
    ":8080",
#if MG_ENABLE_IPV6
    "udp://[::1]:123",
    "[3ffe:2a00:100:7031::1]:900",
#endif
    NULL
  };
  static const int protos[] = {
    SOCK_STREAM,
    SOCK_STREAM,
    SOCK_STREAM,
    SOCK_DGRAM,
    SOCK_STREAM,
    SOCK_STREAM
#if MG_ENABLE_IPV6
    ,
    SOCK_DGRAM,
    SOCK_STREAM
#endif
  };
  static const char *need_lookup[] = {"udp://a.com:53", "locl_host:12", NULL};
  static const char *invalid[] = {
      "99999", "1k", "1.2.3", "1.2.3.4:", "1.2.3.4:2p", "blah://12", ":123x",
      "veeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeery.long:12345",
      "udp://missingport", NULL};
  char host[50];
  union socket_address sa;
  int i, proto;

  for (i = 0; valid[i] != NULL; i++) {
    ASSERT(mg_parse_address(valid[i], &sa, &proto, host, sizeof(host)) > 0);
    ASSERT_EQ(proto, protos[i]);
  }

  for (i = 0; invalid[i] != NULL; i++) {
    ASSERT_EQ(mg_parse_address(invalid[i], &sa, &proto, host, sizeof(host)),
              -1);
  }

  for (i = 0; need_lookup[i] != NULL; i++) {
    ASSERT_EQ(mg_parse_address(need_lookup[i], &sa, &proto, host, sizeof(host)),
              0);
  }

  return NULL;
}

#define CHECK_MGNP(in, exp_rv, exp_out)     \
  do {                                      \
    int rv;                                 \
    struct mg_str mgs;                      \
    char *in_copy = strdup(in);             \
    mgs.p = in_copy;                        \
    mgs.len = strlen(in);                   \
    rv = mg_normalize_uri_path(&mgs, &mgs); \
    ASSERT_EQ(rv, exp_rv);                  \
    if (rv > 0) {                           \
      ASSERT_MG_STREQ(mgs, exp_out);        \
    }                                       \
    MG_FREE(in_copy);                       \
  } while (0)

static const char *test_mg_normalize_uri_path(void) {
  CHECK_MGNP("", 0, "");
  CHECK_MGNP("x", 0, "");
  CHECK_MGNP("/", 1, "/");
  CHECK_MGNP("//", 1, "//");
  CHECK_MGNP("/.", 1, "/");
  CHECK_MGNP("//.", 1, "//");
  CHECK_MGNP("/..", 1, "/");
  CHECK_MGNP("//..", 1, "/");
  CHECK_MGNP("///..", 1, "//");
  CHECK_MGNP("/./", 1, "/");
  CHECK_MGNP("/.//", 1, "//");
  CHECK_MGNP("/./.", 1, "/");
  CHECK_MGNP("/.//.", 1, "//");
  CHECK_MGNP("/foo", 1, "/foo");
  CHECK_MGNP("/foo/", 1, "/foo/");
  CHECK_MGNP("/foo/.", 1, "/foo/");
  CHECK_MGNP("/foo/./", 1, "/foo/");
  CHECK_MGNP("/foo/..", 1, "/");
  CHECK_MGNP("/foo/../", 1, "/");
  CHECK_MGNP("/foo/../bar", 1, "/bar");
  CHECK_MGNP("/foo/../bar/", 1, "/bar/");
  CHECK_MGNP("/foo/../../bar", 1, "/bar");
  CHECK_MGNP("/foo////bar/", 1, "/foo////bar/");
  /* No percent-decoding. */
  CHECK_MGNP("/fo%2fo/%2e%2e/%2e/bar", 1, "/fo%2fo/%2e%2e/%2e/bar");
  return NULL;
}

#define CHECK_U2LP(u, exp_rv, exp_lp, exp_rem)      \
  do {                                              \
    int rv;                                         \
    char *lp = NULL;                                \
    struct mg_str r;                                \
    hm.uri.p = u;                                   \
    hm.uri.len = strlen(u);                         \
    rv = mg_uri_to_local_path(&hm, &opts, &lp, &r); \
    ASSERT_EQ(rv, exp_rv);                          \
    if (rv == 1) {                                  \
      ASSERT_STREQ(lp, exp_lp);                     \
      ASSERT_MG_STREQ(r, exp_rem);                  \
      MG_FREE(lp);                                  \
    }                                               \
  } while (0)

#ifdef _WIN32
#define DS "\\"
#else
#define DS "/"
#endif

static const char *test_mg_uri_to_local_path(void) {
  struct http_message hm;
  struct mg_serve_http_opts opts;
  memset(&hm, 0, sizeof(hm));
  memset(&opts, 0, sizeof(opts));
  opts.document_root = ".";
  CHECK_U2LP("/", 1, ".", "");
  CHECK_U2LP("//", 1, ".", "");
  CHECK_U2LP("/.", 1, ".", "");
  CHECK_U2LP("/%2e", 1, ".", "");
  CHECK_U2LP("//.", 1, ".", "");
  CHECK_U2LP("/..", 1, ".", "");
  CHECK_U2LP("//..", 1, ".", "");
  CHECK_U2LP("/./", 1, ".", "");
  CHECK_U2LP("/%2E/", 1, ".", "");
  CHECK_U2LP("/.//", 1, ".", "");
  CHECK_U2LP("/./.", 1, ".", "");
  CHECK_U2LP("/.//.", 1, ".", "");
  CHECK_U2LP("/data", 1, "." DS "data", "");
  CHECK_U2LP("/data/", 1, "." DS "data", "");
  CHECK_U2LP("/data/.", 1, "." DS "data", "");
  CHECK_U2LP("/data/./", 1, "." DS "data", "");
  CHECK_U2LP("/data/..", 1, ".", "");
  CHECK_U2LP("/data/../", 1, ".", "");
  CHECK_U2LP("/data/../data", 1, "." DS "data", "");
  CHECK_U2LP("/data/../../data", 1, "." DS "data", "");
  CHECK_U2LP("/no/such/file", 1, "." DS "no" DS "such" DS "file", "");
  CHECK_U2LP("/data/no/such/file", 1, "." DS "data" DS "no" DS "such" DS "file",
             "");
  CHECK_U2LP("/data/dummy.xml/", 1, "." DS "data" DS "dummy.xml", "/");
  CHECK_U2LP("/./data/./%2E/dummy.xml", 1, "." DS "data" DS "dummy.xml", "");
  /* No DIRSEPs in path. */
  CHECK_U2LP("/data%2Fdummy%2Exml/in%2Ffo/", 0, "", "");
  /* Ok in PATH_INFO. */
  CHECK_U2LP("/data/dummy%2Exml/in%2Ffo/", 1, "." DS "data" DS "dummy.xml",
             "/in%2Ffo/");
  /* Test rewrites */
  opts.url_rewrites = "/foo=/bar,/=/xxx";
  CHECK_U2LP("/", 1, "/xxx", "");
  CHECK_U2LP("/yyy", 1, "/xxx" DS "yyy", "");
  CHECK_U2LP("/foo/baz", 1, "/bar" DS "baz", "");
  /* /foo does not match - must fall on the boundary, / matches. */
  CHECK_U2LP("/foobario/baz", 1, "/xxx" DS "foobario" DS "baz", "");
  opts.url_rewrites = "**=/xxx";
  CHECK_U2LP("/", 1, "/xxx", "");
  CHECK_U2LP("/yyy", 1, "/xxx", "");
  hm.header_names[0] = mg_mk_str("Host");
  hm.header_values[0] = mg_mk_str("example.org");
  opts.url_rewrites = "@EXAMPLE.org=/test";
  CHECK_U2LP("/hello", 1, "/test" DS "hello", "");
  return NULL;
}

static const char *test_mg_url_encode(void) {
  const struct mg_str encode_me =
      MG_MK_STR("I'm a.little_tea-pot,here's$my;spout~oink(oink)oink/!");
  {
    struct mg_str encoded = mg_url_encode(encode_me);
    ASSERT_MG_STREQ(
        encoded,
        "I%27m%20a.little_tea-pot,here%27s$my;spout~oink(oink)oink/%21");
    free((void *) encoded.p);
  }
  {
    struct mg_str encoded = mg_url_encode_opt(encode_me, mg_mk_str(NULL), 0);
    ASSERT_MG_STREQ(encoded,
                    "I%27m%20a%2elittle%5ftea%2dpot%2chere%27s%24my%3bspout%"
                    "7eoink%28oink%29oink%2f%21");
    free((void *) encoded.p);
  }
  {
    struct mg_str encoded = mg_url_encode_opt(encode_me, mg_mk_str(" /!"),
                                              MG_URL_ENCODE_F_UPPERCASE_HEX);
    ASSERT_MG_STREQ(encoded,
                    "I%27m "
                    "a%2Elittle%5Ftea%2Dpot%2Chere%27s%24my%3Bspout%7Eoink%"
                    "28oink%29oink/!");
    free((void *) encoded.p);
  }
  {
    struct mg_str encoded = mg_url_encode_opt(
        encode_me, mg_mk_str("/!"),
        MG_URL_ENCODE_F_SPACE_AS_PLUS | MG_URL_ENCODE_F_UPPERCASE_HEX);
    ASSERT_MG_STREQ(encoded,
                    "I%27m+a%2Elittle%5Ftea%2Dpot%2Chere%27s%24my%3Bspout%"
                    "7Eoink%28oink%29oink/!");
    free((void *) encoded.p);
  }
  return NULL;
}

static void connect_fail_cb(struct mg_connection *nc, int ev, void *p) {
  switch (ev) {
    case MG_EV_CONNECT:
      /* On connection success, set flag 1, else set 4 */
      *(int *) nc->user_data |= *(int *) p == 0 ? 1 : 4;
      break;
    case MG_EV_CLOSE:
      *(int *) nc->user_data |= 2;
      break;
  }
}

static const char *test_connection_errors(void) {
  struct mg_mgr mgr;
  struct mg_bind_opts bopts;
  struct mg_connect_opts copts;
  struct mg_connection *nc;
  const char *error_string;
  int data = 0;

  mg_mgr_init(&mgr, NULL);

  memset(&bopts, 0, sizeof(bopts));
  bopts.error_string = &error_string;

  ASSERT(mg_bind_opt(&mgr, "blah://12", NULL, bopts) == NULL);
  ASSERT_STREQ(error_string, "cannot parse address");

  ASSERT(mg_bind_opt(&mgr, "tcp://8.8.8.8:88", NULL, bopts) == NULL);
  ASSERT_STREQ(error_string, "failed to open listener");

#if MG_ENABLE_SSL
  bopts.ssl_cert = S_PEM;
  ASSERT(mg_bind_opt(&mgr, "udp://:0", NULL, bopts) == NULL);
  ASSERT_STREQ(error_string, "SSL for UDP is not supported");
  bopts.ssl_cert = "no_such_file";
  ASSERT(mg_bind_opt(&mgr, "tcp://:0", NULL, bopts) == NULL);
  ASSERT_STREQ(error_string, "Invalid SSL cert");
  bopts.ssl_cert = NULL;
  bopts.ssl_ca_cert = "no_such_file";
  ASSERT(mg_bind_opt(&mgr, "tcp://:0", NULL, bopts) == NULL);
  ASSERT_STREQ(error_string, "Invalid SSL CA cert");
#endif

  memset(&copts, 0, sizeof(copts));
  copts.error_string = &error_string;

#if 0
  copts.error_string = &error_string;
  ASSERT(mg_connect_opt(&mgr, "tcp://255.255.255.255:0", NULL, copts) == NULL);
  ASSERT_STREQ(error_string, "cannot connect to socket");

  copts.user_data = &data;
  ASSERT(mg_connect_opt(&mgr, "tcp://255.255.255.255:0", connect_fail_cb,
                        copts) == NULL);
  ASSERT_STREQ(error_string, "cannot connect to socket");
  /* handler isn't invoked when it fails synchronously */
  ASSERT_EQ(data, 0);
#endif

  data = 0;
  copts.user_data = &data;
  ASSERT((nc = mg_connect_opt(&mgr, "tcp://does.not.exist:8080",
                              connect_fail_cb, copts)) != NULL);

  /* handler is invoked when it fails asynchronously */
  poll_until(&mgr, 5, c_int_eq, &data, (void *) 6);
  ASSERT_EQ(data, 6);

  /* mg_bind() does not use MG_CALLOC, but async resolver does */
  test_calloc = failing_calloc;
#ifndef _WIN32
  ASSERT(mg_connect(&mgr, "some.domain.needs.async.resolv:777", NULL) == NULL);
  ASSERT(mg_bind(&mgr, ":4321", NULL) == NULL);
#endif
  test_calloc = TEST_MG_CALLOC;

#if MG_ENABLE_SSL
  copts.ssl_cert = "no_such_file";
  ASSERT((nc = mg_connect_opt(&mgr, "tcp://google.com:80", connect_fail_cb,
                              copts)) == NULL);
  ASSERT_STREQ(error_string, "Invalid SSL cert");
  copts.ssl_cert = NULL;
  copts.ssl_ca_cert = "no_such_file";
  ASSERT((nc = mg_connect_opt(&mgr, "tcp://google.com:80", connect_fail_cb,
                              copts)) == NULL);
  ASSERT_STREQ(error_string, "Invalid SSL CA cert");
#endif

  mg_mgr_free(&mgr);
  return NULL;
}

static int avt(char **buf, size_t buf_size, const char *fmt, ...) {
  int result;
  va_list ap;
  va_start(ap, fmt);
  result = mg_avprintf(buf, buf_size, fmt, ap);
  va_end(ap);
  return result;
}

static const char *test_alloc_vprintf(void) {
  char buf[5], *p = buf;

  ASSERT_EQ(avt(&p, sizeof(buf), "%d", 123), 3);
  ASSERT(p == buf);
  ASSERT_STREQ(p, "123");

  ASSERT_EQ(avt(&p, sizeof(buf), "%d", 123456789), 9);
  ASSERT(p != buf);
  ASSERT_STREQ(p, "123456789");
  free(p);

  return NULL;
}

static const char *test_socketpair(void) {
  sock_t sp[2];
  static const char foo[] = "hi there";
  char buf[20];

  ASSERT_EQ(mg_socketpair(sp, SOCK_DGRAM), 1);
  ASSERT(sizeof(foo) < sizeof(buf));

  /* Send string in one direction */
  ASSERT_EQ(send(sp[0], foo, sizeof(foo), 0), sizeof(foo));
  ASSERT_EQ(recv(sp[1], buf, sizeof(buf), 0), sizeof(foo));
  ASSERT_STREQ(buf, "hi there");

  /* Now in opposite direction */
  ASSERT_EQ(send(sp[1], foo, sizeof(foo), 0), sizeof(foo));
  ASSERT_EQ(recv(sp[0], buf, sizeof(buf), 0), sizeof(foo));
  ASSERT_STREQ(buf, "hi there");

  closesocket(sp[0]);
  closesocket(sp[1]);

  return NULL;
}

static void ev_timer_handler(struct mg_connection *c, int ev, void *ev_data) {
  (void) ev_data;
  switch (ev) {
    case MG_EV_TIMER:
      (*(int *) c->user_data)++;
      break;
    case MG_EV_CLOSE:
      /* Make sure we're alive. Make the ASSERTs fail if we're here. */
      (*(int *) c->user_data) += 100;
      break;
  }
}

static const char *test_timer(void) {
  struct mg_mgr m;
  struct mg_connection *c;
  double begin, end;
  int n = 0, i;

  mg_mgr_init(&m, NULL);
  ASSERT((c = mg_add_sock(&m, INVALID_SOCKET, ev_timer_handler)) != NULL);
  c->user_data = &n;

  /* MG_EV_TIMER should not fire - we did not set it up */
  mg_mgr_poll(&m, 1);
  ASSERT_EQ(n, 0);

  /*
   * Now, set it up. Make sure MG_EV_TIMER event fires.
   * Also, it brings forward the poll timeout.
   */
  ASSERT_EQ(mg_set_timer(c, mg_time() + 0.1), 0.0);
  begin = mg_time();
  /*
   * Windows is a bit sloppy about select() timeouts, so it may take
   * a couple iterations for the timer to actually fire.
   */
  for (i = 0; n != 1 && i < 5; i++) {
    mg_mgr_poll(&m, 1000);
  }
  end = mg_time();
  ASSERT_EQ(n, 1);
  ASSERT_LT(end - begin, 0.9);
  ASSERT_GT(end - begin, 0.09);

  /* Make sure that timer is reset - second time it does not fire */
  ASSERT_EQ(c->ev_timer_time, 0.0);
  mg_mgr_poll(&m, 1);
  ASSERT_EQ(n, 1);

  c->flags |= MG_F_CLOSE_IMMEDIATELY;
  mg_mgr_poll(&m, 1);
  ASSERT_EQ(n, 101);

  /* Async resolver codepath */
  n = 0;
  ASSERT((c = mg_connect(&m, "awful.sad:1234", ev_timer_handler)) != NULL);
  c->user_data = &n;
  mg_set_timer(c, 1);
  poll_until(&m, 1, c_int_eq, &n, (void *) 101);
  ASSERT_EQ(n, 101);

  mg_mgr_free(&m);

  return NULL;
}

struct simple_data {
  int num_accept;
  int num_connect;
  int num_recv;
  int num_send;
  int num_close;
  int num_timer;
  char to_send[100];
  char data_rcvd[100];
  char fail[200];
  struct simple_data *sclient_data;
  struct mg_connection *sclient_nc;
};

#ifndef __APPLE__
static void count_events(struct simple_data *d, int ev) {
  switch (ev) {
    case MG_EV_POLL:
      break;
    case MG_EV_ACCEPT:
      d->num_accept++;
      break;
    case MG_EV_CONNECT:
      d->num_connect++;
      break;
    case MG_EV_RECV:
      d->num_recv++;
      break;
    case MG_EV_SEND:
      d->num_send++;
      break;
    case MG_EV_CLOSE:
      d->num_close++;
      break;
    case MG_EV_TIMER:
      d->num_timer++;
      break;
    default: {
      char msg[100];
      sprintf(msg, "(unexpected event: %d)", ev);
      strcat(d->fail, msg);
    }
  }
}

static void do_send(struct simple_data *d, struct mg_connection *nc) {
  if (d->to_send[0] != '\0') {
    mg_printf(nc, "%s", d->to_send);
  }
}

static void do_recv(struct simple_data *d, struct mg_connection *nc,
                    void *ev_data) {
  if (*((int *) ev_data) != (int) nc->recv_mbuf.len) {
    char msg[100];
    sprintf(msg, "(num recv wrong: %d vs %d)", *((int *) ev_data),
            (int) strlen(d->to_send));
    strcat(d->fail, msg);
    return;
  }
  strncat(d->data_rcvd, nc->recv_mbuf.buf, nc->recv_mbuf.len);
  mbuf_remove(&nc->recv_mbuf, nc->recv_mbuf.len);
}

static void check_sent(struct simple_data *d, struct mg_connection *nc,
                       void *ev_data) {
  if (*((int *) ev_data) != (int) strlen(d->to_send)) {
    char msg[100];
    sprintf(msg, "(num sent wrong: %d vs %d)", *((int *) ev_data),
            (int) strlen(d->to_send));
    strcat(d->fail, msg);
  } else {
    d->to_send[0] = '\0';
  }
  if (nc->send_mbuf.len != 0) strcat(d->fail, "(send buf not empty)");
}

static void cb_client(struct mg_connection *nc, int ev, void *ev_data) {
  struct simple_data *d = (struct simple_data *) nc->user_data;
  count_events(d, ev);
  switch (ev) {
    case MG_EV_CONNECT:
      do_send(d, nc);
      break;
    case MG_EV_SEND:
      check_sent(d, nc, ev_data);
      break;
    case MG_EV_RECV:
      do_recv(d, nc, ev_data);
      break;
  }
}

static void cb_sclient(struct mg_connection *nc, int ev, void *ev_data) {
  struct simple_data *d = (struct simple_data *) nc->user_data;
  count_events(d, ev);
  switch (ev) {
    case MG_EV_RECV: {
      do_recv(d, nc, ev_data);
      do_send(d, nc);
      nc->flags |= MG_F_SEND_AND_CLOSE;
      break;
    }
    case MG_EV_SEND: {
      check_sent(d, nc, ev_data);
      break;
    }
  }
}

static void cb_server(struct mg_connection *nc, int ev, void *ev_data) {
  struct simple_data *d = (struct simple_data *) nc->user_data;
  (void) ev_data;
  if (ev == MG_EV_ACCEPT) {
    d->sclient_nc = nc;
    nc->user_data = d = d->sclient_data;
    nc->handler = cb_sclient;
  }
  count_events(d, ev);
}
#endif

/* The following test is only reliable on Linux. */
#ifdef __linux__
static const char *test_simple(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc_server, *nc_client, *nc_sclient;
  const char *address = "tcp://127.0.0.1:8910";
  struct simple_data client_data, server_data, sclient_data;

  (void) nc_sclient;
  mg_mgr_init(&mgr, NULL);

  ASSERT((nc_server = mg_bind(&mgr, address, cb_server)) != NULL);
  nc_server->user_data = &server_data;
  memset(&server_data, 0, sizeof(server_data));
  server_data.sclient_data = &sclient_data;
  memset(&sclient_data, 0, sizeof(sclient_data));

  mg_mgr_poll(&mgr, 1); /* 1 - nothing */

  ASSERT((nc_client = mg_connect(&mgr, address, cb_client)) != NULL);
  nc_client->user_data = &client_data;
  memset(&client_data, 0, sizeof(client_data));
  strcpy(client_data.to_send, "hi");

  mg_mgr_poll(&mgr, 1); /* 2 - client connects and sends, server accepts */
  ASSERT_EQ(server_data.num_accept, 0);
  ASSERT_EQ(sclient_data.num_accept, 1);
  ASSERT_EQ(client_data.num_connect, 1);
  ASSERT_EQ(client_data.num_send, 1);
  ASSERT_STREQ(client_data.fail, "");
  ASSERT_STREQ(server_data.fail, "");

  ASSERT(server_data.sclient_nc != NULL);
  nc_sclient = server_data.sclient_nc;
  ASSERT_EQ(sclient_data.num_send, 0);
  ASSERT_EQ(sclient_data.num_recv, 0);

  strcpy(sclient_data.to_send, "hello");
  mg_mgr_poll(&mgr, 1); /* 3 - server receives, buffers response, closes */
  ASSERT_STREQ(sclient_data.fail, "");
  ASSERT_EQ(sclient_data.num_recv, 1);
  ASSERT_EQ(sclient_data.num_send, 0);
  ASSERT_EQ(client_data.num_recv, 0);
  ASSERT_STREQ(sclient_data.data_rcvd, "hi");

  mg_mgr_poll(&mgr, 1); /* 4 - server sends */
  ASSERT_STREQ(sclient_data.fail, "");
  ASSERT_EQ(sclient_data.num_send, 1);

  mg_mgr_poll(&mgr, 1); /* 5 - client receives */
  ASSERT_STREQ(client_data.fail, "");
  ASSERT_EQ(client_data.num_recv, 1);
  ASSERT_STREQ(client_data.data_rcvd, "hello");

  mg_mgr_free(&mgr);

  ASSERT_STREQ(client_data.fail, "");
  ASSERT_STREQ(server_data.fail, "");
  ASSERT_STREQ(sclient_data.fail, "");

  ASSERT_EQ(server_data.num_accept, 0);
  ASSERT_EQ(server_data.num_connect, 0);
  ASSERT_EQ(server_data.num_recv, 0);
  ASSERT_EQ(server_data.num_send, 0);
  ASSERT_EQ(server_data.num_close, 1);

  ASSERT_EQ(client_data.num_accept, 0);
  ASSERT_EQ(client_data.num_connect, 1);
  ASSERT_EQ(client_data.num_recv, 1);
  ASSERT_EQ(client_data.num_send, 1);
  ASSERT_EQ(client_data.num_close, 1);

  ASSERT_EQ(sclient_data.num_accept, 1);
  ASSERT_EQ(sclient_data.num_connect, 0);
  ASSERT_EQ(sclient_data.num_recv, 1);
  ASSERT_EQ(sclient_data.num_send, 1);
  ASSERT_EQ(sclient_data.num_close, 1);

  return NULL;
}
#endif

#if MG_ENABLE_THREADS
static void eh2(struct mg_connection *nc, int ev, void *p) {
  (void) p;
  switch (ev) {
    case MG_EV_RECV:
      strcpy((char *) nc->user_data, nc->recv_mbuf.buf);
      break;
    default:
      break;
  }
}

static void *thread_func(void *param) {
  sock_t sock = *(sock_t *) param;
  send(sock, ":-)", 4, 0);
  return NULL;
}

static const char *test_thread(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  sock_t sp[2];
  char buf[20] = "";

  ASSERT_EQ(mg_socketpair(sp, SOCK_STREAM), 1);
  mg_start_thread(thread_func, &sp[1]);

  mg_mgr_init(&mgr, NULL);
  ASSERT((nc = mg_add_sock(&mgr, sp[0], eh2)) != NULL);
  nc->user_data = buf;
  poll_until(&mgr, 1, c_str_ne, buf, (void *) "");
  ASSERT_STREQ(buf, ":-)");
  mg_mgr_free(&mgr);
  closesocket(sp[1]);

  return NULL;
}
#endif /* MG_ENABLE_THREADS */

struct udp_res {
  struct mbuf buf_srv;
  struct mg_connection *srv_recv_conn;
  struct mbuf buf_clnt;
  struct mg_connection *clt_recv_conn;
};

static void eh3_srv(struct mg_connection *nc, int ev, void *p) {
  struct udp_res *ur = (struct udp_res *) nc->mgr->user_data;
  struct mbuf *io = &nc->recv_mbuf;
  (void) p;

  if (nc->flags & MG_F_LISTENING) {
    if (ev != MG_EV_POLL && ev != MG_EV_CLOSE) {
      mbuf_append(&ur->buf_srv, "BUG", 3);
    }
    return;
  }

  if (ev == MG_EV_ACCEPT) {
    mbuf_append(&ur->buf_srv, "ACCEPT", 6);
  } else if (ev == MG_EV_RECV) {
    mbuf_append(&ur->buf_srv, (nc->flags & MG_F_SEND_AND_CLOSE ? "+" : "-"), 1);
    mbuf_append(&ur->buf_srv, " RECV ", 6);
    mbuf_append(&ur->buf_srv, io->buf, io->len);
    mg_send(nc, io->buf, io->len);
    mbuf_remove(io, io->len);
  } else if (ev == MG_EV_CLOSE) {
    mbuf_append(&ur->buf_srv, " CLOSE", 6);
  }
}

static void eh3_clnt(struct mg_connection *nc, int ev, void *p) {
  struct udp_res *ur = (struct udp_res *) nc->mgr->user_data;
  struct mbuf *io = &nc->recv_mbuf;
  (void) p;

  if (ev == MG_EV_CONNECT) {
    mbuf_append(&ur->buf_clnt, "CONNECT", 7);
    mg_printf(nc, "%s", "boo!");
  } else if (ev == MG_EV_RECV) {
    mbuf_append(&ur->buf_clnt, " RECV ", 6);
    mbuf_append(&ur->buf_clnt, io->buf, io->len);
    mbuf_remove(io, io->len);
    nc->flags |= MG_F_CLOSE_IMMEDIATELY;
  } else if (ev == MG_EV_CLOSE) {
    mbuf_append(&ur->buf_clnt, " CLOSE", 6);
  }
}

static const char *test_udp(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc1, *nc2;
  const char *address = "udp://127.0.0.1:7878";
  struct udp_res res;
  memset(&res, 0, sizeof(res));
  mbuf_init(&res.buf_srv, 100);
  mbuf_init(&res.buf_clnt, 100);

  mg_mgr_init(&mgr, &res);
  ASSERT((nc1 = mg_bind(&mgr, address, eh3_srv)) != NULL);
  ASSERT((nc2 = mg_connect(&mgr, address, eh3_clnt)) != NULL);

  poll_until(&mgr, 1, c_int_eq, &res.buf_clnt.len, (void *) 23);
  ASSERT_STREQ_NZ(res.buf_srv.buf, "ACCEPT+ RECV boo! CLOSE");
  ASSERT_STREQ_NZ(res.buf_clnt.buf, "CONNECT RECV boo! CLOSE");

  mbuf_free(&res.buf_srv);
  mbuf_free(&res.buf_clnt);
  mg_mgr_free(&mgr);

  return NULL;
}

static const char *test_parse_http_message(void) {
  static const char *a = "GET / HTTP/1.0\n\n";
  static const char *b = "GET /blah HTTP/1.0\r\nFoo:  bar  \r\n\r\n";
  static const char *c = "get b c\nz:  k \nb: t\nvvv\n\n xx";
  static const char *d = "a b c\nContent-Length: 21 \nb: t\nvvv\n\n";
  static const char *e = "GET /foo?a=b&c=d HTTP/1.0\n\n";
  static const char *f = "POST /x HTTP/1.0\n\n";
  static const char *g = "WOHOO /x HTTP/1.0\n\n";
  static const char *h = "HTTP/1.0 200 OK\n\n";
  static const char *i = "HTTP/1.0 999 OMGWTFBBQ\n\n";
  static const char *j =
      "GET / HTTP/1.0\r\nHost: 127.0.0.1:18888\r\nCookie:\r\nX-PlayID: "
      "45455\r\nRange: 0-1\r\n\r\n";
  int idx, len;
  struct mg_str *v;
  struct http_message req;

  len = strlen(b);
  for (idx = 0; idx < len; idx++) {
    ASSERT_EQ(mg_parse_http(b, idx, &req, 1), 0);
  }

  ASSERT_EQ(mg_parse_http("\b23", 3, &req, 1), -1);
  ASSERT_EQ(mg_parse_http("\b23", 3, &req, 0), -1);
  ASSERT_EQ(mg_parse_http("get\n\n", 5, &req, 1), -1);
  ASSERT_EQ(mg_parse_http("get\n\n", 5, &req, 0), -1);
  ASSERT_EQ(mg_parse_http(a, strlen(a) - 1, &req, 1), 0);
  ASSERT_EQ(mg_parse_http(a, strlen(a), &req, 0), -1);

  ASSERT_EQ(mg_parse_http(a, strlen(a), &req, 1), (int) strlen(a));
  ASSERT_EQ(req.message.len, strlen(a));
  ASSERT_EQ(req.body.len, 0);

  ASSERT_EQ(mg_parse_http(b, strlen(b), &req, 0), -1);
  ASSERT_EQ(mg_parse_http(b, strlen(b), &req, 1), (int) strlen(b));
  ASSERT_EQ(req.header_names[0].len, 3);
  ASSERT_EQ(req.header_values[0].len, 3);
  ASSERT(req.header_names[1].p == NULL);
  ASSERT_EQ(req.query_string.len, 0);
  ASSERT_EQ(req.message.len, strlen(b));
  ASSERT_EQ(req.body.len, 0);

  ASSERT_EQ(mg_parse_http(c, strlen(c), &req, 1), (int) strlen(c) - 3);
  ASSERT(req.header_names[2].p == NULL);
  ASSERT(req.header_names[0].p != NULL);
  ASSERT(req.header_names[1].p != NULL);
  ASSERT_EQ(memcmp(req.header_values[1].p, "t", 1), 0);
  ASSERT_EQ(req.header_names[1].len, 1);
  ASSERT_EQ(req.body.len, 0);

  ASSERT_EQ(mg_parse_http(d, strlen(d), &req, 1), (int) strlen(d));
  ASSERT_EQ(req.body.len, 21);
  ASSERT_EQ(req.message.len, 21 + strlen(d));
  ASSERT(mg_get_http_header(&req, "foo") == NULL);
  ASSERT((v = mg_get_http_header(&req, "contENT-Length")) != NULL);
  ASSERT_EQ(v->len, 2);
  ASSERT_STREQ_NZ(v->p, "21");

  ASSERT_EQ(mg_parse_http(e, strlen(e), &req, 1), (int) strlen(e));
  ASSERT_EQ(mg_vcmp(&req.uri, "/foo"), 0);
  ASSERT_EQ(mg_vcmp(&req.query_string, "a=b&c=d"), 0);

  ASSERT_EQ(mg_parse_http(f, strlen(f), &req, 1), (int) strlen(f));
  ASSERT_EQ64(req.body.len, (size_t) ~0);

  ASSERT_EQ(mg_parse_http(g, strlen(g), &req, 1), (int) strlen(g));
  ASSERT_EQ(req.body.len, 0);

  ASSERT_EQ(mg_parse_http(h, strlen(h), &req, 0), (int) strlen(h));
  ASSERT_EQ(mg_vcmp(&req.proto, "HTTP/1.0"), 0);
  ASSERT_EQ(req.resp_code, 200);
  ASSERT_EQ(mg_vcmp(&req.resp_status_msg, "OK"), 0);
  ASSERT_EQ64(req.body.len, (size_t) ~0);

  ASSERT_EQ(mg_parse_http(i, strlen(i), &req, 0), -1);

  ASSERT_EQ(mg_parse_http(j, strlen(j), &req, 1), (int) strlen(j));
  ASSERT(mg_get_http_header(&req, "Host") != NULL);
  ASSERT(mg_get_http_header(&req, "Cookie") == NULL);
  ASSERT(mg_get_http_header(&req, "Range") != NULL);

  return NULL;
}

static const char *test_get_http_var(void) {
  char buf[256];
  struct mg_str body;
  body.p = "key1=value1&key2=value2&key3=value%203&key4=value+4";
  body.len = strlen(body.p);

  ASSERT(mg_get_http_var(&body, "key1", buf, sizeof(buf)) > 0);
  ASSERT_STREQ(buf, "value1");
  ASSERT(mg_get_http_var(&body, "KEY1", buf, sizeof(buf)) > 0);
  ASSERT_STREQ(buf, "value1");
  ASSERT(mg_get_http_var(&body, "key2", buf, sizeof(buf)) > 0);
  ASSERT_STREQ(buf, "value2");
  ASSERT(mg_get_http_var(&body, "key3", buf, sizeof(buf)) > 0);
  ASSERT_STREQ(buf, "value 3");
  ASSERT(mg_get_http_var(&body, "key4", buf, sizeof(buf)) > 0);
  ASSERT_STREQ(buf, "value 4");

  ASSERT_EQ(mg_get_http_var(&body, "key1", NULL, sizeof(buf)), -2);
  ASSERT_EQ(mg_get_http_var(&body, "key1", buf, 0), -2);
  ASSERT_EQ(mg_get_http_var(&body, NULL, buf, sizeof(buf)), -1);
  ASSERT_EQ(mg_get_http_var(&body, "key1", buf, 1), -3);

  body.p = "key=broken%2";
  body.len = strlen(body.p);
  ASSERT(mg_get_http_var(&body, "key", buf, sizeof(buf)) < 0);

  body.p = "key=broken%2x";
  body.len = strlen(body.p);
  ASSERT(mg_get_http_var(&body, "key", buf, sizeof(buf)) < 0);

  ASSERT_EQ(mg_get_http_var(&body, "inexistent", buf, sizeof(buf)), -4);

  return NULL;
}

static void cb1(struct mg_connection *nc, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_REQUEST) {
    struct http_message *hm = (struct http_message *) ev_data;
    if (mg_vcmp(&hm->uri, "/foo") == 0) {
      mg_printf(nc, "HTTP/1.0 200 OK\r\n\r\n[%.*s %d]", (int) hm->uri.len,
                hm->uri.p, (int) hm->body.len);
      nc->flags |= MG_F_SEND_AND_CLOSE;
    } else {
      s_http_server_opts.document_root = ".";
      s_http_server_opts.per_directory_auth_file = "passwords.txt";
      s_http_server_opts.auth_domain = "foo.com";
      s_http_server_opts.dav_document_root = "./data/dav";
      s_http_server_opts.hidden_file_pattern = "hidden_file.*$";
      s_http_server_opts.custom_mime_types =
          ".txt=text/plain; charset=windows-1251,"
          ".c=text/plain; charset=utf-8";
      mg_serve_http(nc, hm, s_http_server_opts);
    }
  } else if (ev == MG_EV_SSI_CALL) {
    mg_printf_html_escape(nc, "[ssi call: %s]", (char *) ev_data);
  } else if (ev == MG_EV_SSI_CALL_CTX) {
    struct mg_ssi_call_ctx *cctx = (struct mg_ssi_call_ctx *) ev_data;
    mg_printf_html_escape(nc, "[ssi call w/ ctx: %.*s %.*s %.*s]",
                          (int) cctx->req->uri.len, cctx->req->uri.p,
                          (int) cctx->file.len, cctx->file.p,
                          (int) cctx->arg.len, cctx->arg.p);
  }
}

static void secret_endpoint_cb(struct mg_connection *nc, int ev,
                               void *ev_data) {
  if (ev != MG_EV_HTTP_REQUEST) return;
  mg_send_response_line(nc, 200, "Content-Type: text/plain\r\n");
  mg_printf(nc, "%s", "Secret!");
  nc->flags |= MG_F_SEND_AND_CLOSE;
  (void) ev_data;
}

static void cb2(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;

  if (ev == MG_EV_HTTP_REPLY) {
    sprintf((char *) nc->user_data, "%.*s %lu", (int) hm->body.len, hm->body.p,
            (unsigned long) hm->message.len);
    nc->flags |= MG_F_CLOSE_IMMEDIATELY;
  }
}

static void cb7(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;
  size_t size;
  char *data, *user_data = (char *) nc->user_data;

  if (ev == MG_EV_HTTP_REPLY) {
    /* Make sure that we've downloaded this executable, byte-to-byte */
    data = read_file(g_argv_0, &size);
    DBG(("file %s, size %d; got %d", g_argv_0, (int) size, (int) hm->body.len));
    if (data != NULL && size == hm->body.len &&
        memcmp(hm->body.p, data, size) == 0) {
      strcpy(user_data, "success");
    } else {
      strcpy(user_data, "fail");
    }
    free(data);
    nc->flags |= MG_F_CLOSE_IMMEDIATELY;
  } else if (ev == MG_EV_RECV) {
#if 0
    LOG(LL_INFO, ("%d", (int) nc->recv_mbuf.len));
#endif
  }
}

static void cb8(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;
  DBG(("%p ev=%d", (void *) nc, ev));

  if (ev == MG_EV_HTTP_REPLY) {
    snprintf((char *) nc->user_data, FETCH_BUF_SIZE, "%.*s",
             (int) hm->message.len, hm->message.p);
    nc->flags |= MG_F_CLOSE_IMMEDIATELY;
  }
}

static void cb10(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;
  struct mg_str *s;

  if (ev == MG_EV_HTTP_REPLY &&
      (s = mg_get_http_header(hm, "Content-Type")) != NULL) {
    sprintf((char *) nc->user_data, "%.*s", (int) s->len, s->p);
  }
}

static void default_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;
  (void) ev_data;

  if (ev == MG_EV_HTTP_REQUEST) {
    if (mg_vcmp(&hm->uri, "/") == 0 || mg_vcmp(&hm->uri, "/foo") == 0) {
      mg_printf(nc, "HTTP/1.0 200 OK\r\n\r\n[%.*s%s%.*s %d]", (int) hm->uri.len,
                hm->uri.p, hm->query_string.len > 0 ? "?" : "",
                (int) hm->query_string.len, hm->query_string.p,
                (int) hm->body.len);
      nc->flags |= MG_F_SEND_AND_CLOSE;
    }
  } else if (ev == MG_EV_CLOSE) {
    if (nc->listener != NULL) {
      (*(int *) nc->listener->user_data) += 1;
      DBG(("%p == default close", nc));
    }
  }
}

static void handle_hello1(struct mg_connection *nc, int ev, void *ev_data) {
  (void) ev_data;
  switch (ev) {
    case MG_EV_HTTP_REQUEST:
      mg_printf(nc, "HTTP/1.0 200 OK\r\n\r\n[I am Hello1]");
      nc->flags |= MG_F_SEND_AND_CLOSE;
      break;
    case MG_EV_CLOSE:
      DBG(("%p == hello1 close", nc));
      (*(int *) nc->listener->user_data) += 10;
      break;
  }
}

static void handle_hello2(struct mg_connection *nc, int ev, void *ev_data) {
  (void) ev_data;
  switch (ev) {
    case MG_EV_HTTP_REQUEST:
      mg_printf(nc, "HTTP/1.0 200 OK\r\n\r\n[I am Hello two]");
      nc->flags |= MG_F_SEND_AND_CLOSE;
      break;
    case MG_EV_CLOSE:
      DBG(("%p == hello2 close", nc));
      (*(int *) nc->listener->user_data) += 100;
      break;
  }
}

static void handle_hello5(struct mg_connection *nc, int ev, void *ev_data) {
  (void) ev_data;
  switch (ev) {
    case MG_EV_HTTP_REQUEST:
      mg_printf(nc, "HTTP/1.0 200 OK\r\n\r\n[I am Hello again]");
      nc->flags |= MG_F_SEND_AND_CLOSE;
      break;
    case MG_EV_CLOSE:
      DBG(("%p == hello5 close", nc));
      (*(int *) nc->listener->user_data) += 1000;
      break;
  }
}

static void vfetch_http(char *buf, const char *auth_file,
                        const char *request_fmt, va_list ap) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  struct mg_http_endpoint_opts ep_opts;
  char local_addr[50];

  /* Setup server. Use different local port for the next invocation. */
  mg_mgr_init(&mgr, NULL);
  /* mgr.hexdump_file = "-"; */
  snprintf(local_addr, sizeof(local_addr), "127.0.0.1:%d", s_listening_port++);
  nc = mg_bind(&mgr, local_addr, cb1);
  mg_set_protocol_http_websocket(nc);
  memset(&ep_opts, 0, sizeof(ep_opts));
  ep_opts.auth_file = auth_file;
  ep_opts.auth_domain = "foo.com";
  mg_register_http_endpoint_opt(nc, "/secret", secret_endpoint_cb, ep_opts);

  /* Setup client */
  nc = mg_connect(&mgr, local_addr, cb8);
  mg_set_protocol_http_websocket(nc);
  nc->user_data = buf;
  mg_vprintf(nc, request_fmt, ap);

  /* Run event loop, destroy server */
  buf[0] = '\0';
  poll_until(&mgr, 5, c_str_ne, buf, (void *) "");

  mg_mgr_free(&mgr);
}

static void fetch_http(char *buf, const char *request_fmt, ...) {
  va_list ap;
  va_start(ap, request_fmt);
  vfetch_http(buf, "data/auth/passwords.txt", request_fmt, ap);
  va_end(ap);
}

static void fetch_http2(char *buf, const char *auth_file,
                        const char *request_fmt, ...) {
  va_list ap;
  va_start(ap, request_fmt);
  vfetch_http(buf, auth_file, request_fmt, ap);
  va_end(ap);
}

static const char *test_http_endpoints(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  const char *local_addr = "127.0.0.1:7778";
  char buf[50];
  int close_count = 0;

  mg_mgr_init(&mgr, NULL);
  /* mgr.hexdump_file = "-"; */
  ASSERT((nc = mg_bind(&mgr, local_addr, default_handler)) != NULL);
  mg_register_http_endpoint(nc, "/hello1", handle_hello1 MG_UD_ARG(NULL));
  mg_register_http_endpoint(nc, "/hello1/hello2",
                            handle_hello2 MG_UD_ARG(NULL));
  mg_register_http_endpoint(nc, "/hello5/*", handle_hello5 MG_UD_ARG(NULL));
  nc->user_data = &close_count;

  mg_set_protocol_http_websocket(nc);
  /* Valid HTTP request. Pass test buffer to the callback. */
  ASSERT((nc = mg_connect_http(&mgr, cb2, "127.0.0.1:7778", NULL,
                               "0123456789")) != NULL);
  memset(buf, 0, sizeof(buf));
  nc->user_data = buf;
  DBG(("== WAIT =="));
  poll_until(&mgr, 1, c_str_ne, buf, (void *) "");
  ASSERT_STREQ(buf, "[/ 10] 25");

  ASSERT((nc = mg_connect_http(&mgr, cb2, "http://127.0.0.1:7778/", NULL,
                               "0123456789")) != NULL);
  memset(buf, 0, sizeof(buf));
  nc->user_data = buf;
  DBG(("== WAIT =="));
  poll_until(&mgr, 1, c_str_ne, buf, (void *) "");
  ASSERT_STREQ(buf, "[/ 10] 25");

  ASSERT((nc = mg_connect_http(&mgr, cb2, "http://127.0.0.1:7778/foo?a=b", NULL,
                               "foo")) != NULL);
  memset(buf, 0, sizeof(buf));
  nc->user_data = buf;
  DBG(("== WAIT =="));
  poll_until(&mgr, 1, c_str_ne, buf, (void *) "");
  ASSERT_STREQ(buf, "[/foo?a=b 3] 31");

  ASSERT((nc = mg_connect_http(&mgr, cb2, "http://127.0.0.1:7778/foo", NULL,
                               "0123456789")) != NULL);
  memset(buf, 0, sizeof(buf));
  nc->user_data = buf;
  DBG(("== WAIT =="));
  poll_until(&mgr, 1, c_str_ne, buf, (void *) "");
  ASSERT_STREQ(buf, "[/foo 10] 28");

  ASSERT((nc = mg_connect_http(&mgr, cb2, "http://127.0.0.1:7778/hello1", NULL,
                               "0123456789")) != NULL);
  memset(buf, 0, sizeof(buf));
  nc->user_data = buf;
  DBG(("== WAIT =="));
  poll_until(&mgr, 1, c_str_ne, buf, (void *) "");
  ASSERT_STREQ(buf, "[I am Hello1] 32");

  ASSERT((nc = mg_connect_http(&mgr, cb2, "http://127.0.0.1:7778/hello1/hello2",
                               NULL, "0123456789")) != NULL);
  memset(buf, 0, sizeof(buf));
  nc->user_data = buf;
  DBG(("== WAIT =="));
  poll_until(&mgr, 1, c_str_ne, buf, (void *) "");
  ASSERT_STREQ(buf, "[I am Hello two] 35");

  ASSERT((nc = mg_connect_http(&mgr, cb2, "http://127.0.0.1:7778/hello5/dummy",
                               NULL, "0123456789")) != NULL);
  memset(buf, 0, sizeof(buf));
  nc->user_data = buf;

  poll_until(&mgr, 1, c_str_ne, buf, (void *) "");
  ASSERT_STREQ(buf, "[I am Hello again] 37");

  ASSERT_EQ(close_count, 1117);

  mg_mgr_free(&mgr);

  return NULL;
}

static const char *serve_file_verify(struct http_message *hm) {
  size_t size;
  char *data = read_file("unit_test.c", &size);
  if (data == NULL || size != hm->body.len ||
      memcmp(hm->body.p, data, size) != 0) {
    return "wrong data";
  }
  free(data);

  {
    char buf[20];
    struct mg_str *h = mg_get_http_header(hm, "Content-Length");
    ASSERT(h != NULL);
    snprintf(buf, sizeof(buf), "%d", (int) hm->body.len);
    ASSERT_MG_STREQ(*h, buf);
  }
  {
    struct mg_str *h = mg_get_http_header(hm, "Connection");
    ASSERT(h != NULL);
    ASSERT_MG_STREQ(*h, "keep-alive");
  }
  return "success";
}

static void serve_file_cb(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;
  char *user_data = (char *) nc->user_data;

  if (ev == MG_EV_HTTP_REQUEST) {
    mg_http_serve_file(nc, hm, "unit_test.c", mg_mk_str("text/plain"),
                       mg_mk_str(""));
  } else if (ev == MG_EV_HTTP_REPLY) {
    strcpy(user_data, serve_file_verify(hm));
    nc->flags |= MG_F_CLOSE_IMMEDIATELY;
  }
}

static const char *test_http_serve_file(void) {
  struct mg_mgr mgr;
  struct mg_connection *lc, *nc;
  const char *local_addr = "127.0.0.1:7777";
  char status[100];
  mg_mgr_init(&mgr, NULL);
  /* mgr.hexdump_file = "-"; */
  ASSERT((lc = mg_bind(&mgr, local_addr, serve_file_cb)) != NULL);
  mg_set_protocol_http_websocket(lc);
  ASSERT((nc = mg_connect(&mgr, local_addr, serve_file_cb)) != NULL);
  mg_set_protocol_http_websocket(nc);
  status[0] = '\0';
  nc->user_data = status;
  mg_printf(nc, "GET / HTTP/1.1\r\n\r\n");
  poll_until(&mgr, 5, c_str_ne, status, (void *) "");
  ASSERT_STREQ(status, "success");
  mg_mgr_free(&mgr);
  return NULL;
}

static void srv1(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_REQUEST) {
    mg_http_serve_file(c, (struct http_message *) ev_data, "unit_test.c",
                       mg_mk_str("text/plain"), mg_mk_str(""));
  }
}

static void srv2(struct mg_connection *c, int ev, void *ev_data) {
  static cs_md5_ctx c1, c2;
  struct http_message *hm = (struct http_message *) ev_data;

  switch (ev) {
    case MG_EV_CONNECT:
      cs_md5_init(&c1);
      cs_md5_init(&c2);
      break;
    case MG_EV_HTTP_CHUNK:
      cs_md5_update(&c1, (const unsigned char *) hm->body.p, hm->body.len);
      c->flags |= MG_F_DELETE_CHUNK;
      break;
    case MG_EV_HTTP_REPLY: {
      unsigned char sig1[16], sig2[sizeof(sig1)];
      size_t size;
      char *data = read_file("unit_test.c", &size);
      if (data != NULL) cs_md5_update(&c2, (const unsigned char *) data, size);
      free(data);
      cs_md5_final(sig1, &c1);
      cs_md5_final(sig2, &c2);
      *(int *) c->user_data = (memcmp(sig1, sig2, sizeof(sig1)) == 0) ? 1 : 2;
      break;
    }
  }
}

static const char *test_http_serve_file_streaming(void) {
  struct mg_mgr mgr;
  struct mg_connection *lc, *nc;
  const char *local_addr = "127.0.0.1:7732";
  int status = 0;
  mg_mgr_init(&mgr, NULL);
  ASSERT((lc = mg_bind(&mgr, local_addr, srv1)) != NULL);
  mg_set_protocol_http_websocket(lc);
  ASSERT((nc = mg_connect(&mgr, local_addr, srv2)) != NULL);
  mg_set_protocol_http_websocket(nc);
  nc->user_data = &status;
  mg_printf(nc, "GET / HTTP/1.0\r\n\r\n");
  poll_until(&mgr, 30, c_int_ne, &status, (void *) 0);
  ASSERT_EQ(status, 1);
  mg_mgr_free(&mgr);
  return NULL;
}

static const char *test_http(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  const char *this_binary, *local_addr = "127.0.0.1:7777";
  char buf[50] = "", status[100] = "", mime1[20] = "", mime2[100] = "";
  char opt_buf[1024] = "";
  const char *opt_answer =
      "HTTP/1.1 200 OK\r\nServer: Mongoose/" MG_VERSION
      "\r\nAllow: GET, POST, HEAD, CONNECT, OPTIONS, MKCOL, "
      "PUT, DELETE, PROPFIND, MOVE";
  char url[1000];

  mg_mgr_init(&mgr, NULL);
  /* mgr.hexdump_file = "-"; */
  ASSERT((nc = mg_bind(&mgr, local_addr, cb1)) != NULL);

  mg_set_protocol_http_websocket(nc);
  /* Valid HTTP request. Pass test buffer to the callback. */
  ASSERT((nc = mg_connect_http(&mgr, cb2, "http://127.0.0.1:7777/foo", NULL,
                               "0123456789")) != NULL);
  nc->user_data = buf;
  DBG(("== WAIT =="));
  poll_until(&mgr, 1, c_str_ne, buf, (void *) "");
  ASSERT_STREQ(buf, "[/foo 10] 28");

  /* OPTIONS request */
  ASSERT((nc = mg_connect(&mgr, local_addr, cb8)) != NULL);
  mg_set_protocol_http_websocket(nc);
  nc->user_data = opt_buf;

  mg_printf(nc, "%s", "OPTIONS / HTTP 1.0\n\n");

  poll_until(&mgr, 1, c_str_ne, opt_buf, (void *) "");
  ASSERT_STREQ_NZ(opt_buf, opt_answer);

  /* Invalid HTTP request */
  ASSERT((nc = mg_connect(&mgr, local_addr, cb2)) != NULL);
  mg_set_protocol_http_websocket(nc);

  mg_printf(nc, "%s", "bl\x03\n\n");
  /* Test static file download by downloading this executable, argv[0] */
  ASSERT((nc = mg_connect(&mgr, local_addr, cb7)) != NULL);
  mg_set_protocol_http_websocket(nc);
  nc->user_data = status;
  mbuf_resize(&nc->recv_mbuf, 10000000);

  /* Wine and GDB set argv0 to full path: strip the dir component */
  if ((this_binary = strrchr(g_argv_0, '\\')) != NULL) {
    this_binary++;
  } else if ((this_binary = strrchr(g_argv_0, '/')) != NULL) {
    this_binary++;
  } else {
    this_binary = g_argv_0;
  }
  mg_printf(nc, "GET /%s HTTP/1.0\n\n", this_binary);
  /* Test mime type for static file */
  snprintf(url, sizeof(url), "http://%s/data/dummy.xml", local_addr);
  ASSERT((nc = mg_connect_http(&mgr, cb10, url, NULL, NULL)) != NULL);
  nc->user_data = mime1;

  /* Test custom mime type for static file */
  snprintf(url, sizeof(url), "http://%s/data/range.txt", local_addr);
  ASSERT((nc = mg_connect_http(&mgr, cb10, url, NULL, NULL)) != NULL);
  nc->user_data = mime2;

  /* Run event loop. Use more cycles to let file download complete. */
  poll_until(&mgr, 15, c_str_ne, status, (void *) "");
  mg_mgr_free(&mgr);

  /* Check that test buffer has been filled by the callback properly. */
  ASSERT_STREQ(status, "success");
  ASSERT_STREQ(mime1, "text/xml");
  ASSERT_STREQ(mime2, "text/plain; charset=windows-1251");

  return NULL;
}

static void http_pipeline_handler(struct mg_connection *c, int ev,
                                  void *ev_data) {
  (void) ev_data;
  int *status = (int *) c->mgr->user_data;
  if (ev == MG_EV_HTTP_REQUEST) {
    /* Server request handler */
    mg_send_response_line(c, 200,
                          "Content-Type: text/plain\r\nContent-Length: 5\r\n");
    mg_printf(c, "Hello");
    *status = *status + 1;
  } else if (ev == MG_EV_HTTP_REPLY) {
    /* Client reply handler */
    *status = *status + 10;
  }
}

static const char *test_http_pipeline(void) {
  struct mg_mgr mgr;
  struct mg_connection *lc, *nc1;
  const char *local_addr = "127.0.0.1:7777";
  int status = 0;

  mg_mgr_init(&mgr, (void *) &status);
  ASSERT(lc = mg_bind(&mgr, local_addr, http_pipeline_handler));
  mg_set_protocol_http_websocket(lc);
  ASSERT(nc1 = mg_connect(&mgr, local_addr, http_pipeline_handler));
  mg_set_protocol_http_websocket(nc1);
  mg_printf(nc1, "GET / HTTP/1.1\r\n\r\nGET / HTTP/1.1\r\n\r\n");
  poll_until(&mgr, 1, c_int_eq, &status, (void *) 22);
  ASSERT_EQ(status, 22);
  mg_mgr_free(&mgr);
  return NULL;
}

static const char *test_http_send_redirect(void) {
  struct mg_connection nc;
  init_test_connection(&nc);

  mg_http_send_redirect(&nc, 301, mg_mk_str("http://foo"), mg_mk_str(NULL));
  mg_send(&nc, "", 1); /* NUL */
  ASSERT_STREQ_NZ(nc.send_mbuf.buf,
                  "HTTP/1.1 301 Moved\r\n"
                  "Server: Mongoose/" MG_VERSION
                  "\r\n"
                  "Location: http://foo\r\n"
                  "Content-Type: text/html\r\n"
                  "Content-Length: 41\r\n"
                  "Cache-Control: no-cache\r\n"
                  "\r\n"
                  "<p>Moved <a href='http://foo'>here</a>.\r\n");
  ASSERT_EQ(nc.send_mbuf.len, 179);
  mbuf_remove(&nc.send_mbuf, nc.send_mbuf.len);

  mg_http_send_redirect(&nc, 302, mg_mk_str("/bar"), mg_mk_str("O-M-G: WTF?!"));
  mg_send(&nc, "", 1); /* NUL */
  ASSERT_STREQ_NZ(nc.send_mbuf.buf,
                  "HTTP/1.1 302 Found\r\n"
                  "Server: Mongoose/" MG_VERSION
                  "\r\n"
                  "Location: /bar\r\n"
                  "Content-Type: text/html\r\n"
                  "Content-Length: 35\r\n"
                  "Cache-Control: no-cache\r\n"
                  "O-M-G: WTF?!\r\n"
                  "\r\n"
                  "<p>Moved <a href='/bar'>here</a>.\r\n");
  ASSERT_EQ(nc.send_mbuf.len, 181);
  mbuf_remove(&nc.send_mbuf, nc.send_mbuf.len);

  mbuf_free(&nc.send_mbuf);
  return NULL;
}

static const char *test_http_digest_auth(void) {
  char buf[FETCH_BUF_SIZE], auth_hdr[200];
  char nonce[40];
  struct mg_str bufstr;

  /* Test digest authorization popup - per-directory auth file */
  fetch_http(buf, "%s", "GET /data/auth/a.txt?a=b HTTP/1.0\r\n\r\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 401");
  bufstr = mg_mk_str(buf);
  ASSERT_GT(mg_http_parse_header(&bufstr, "nonce", nonce, sizeof(nonce)), 0);
  buf[0] = '\0';
  /* Per-endpoint auth config */
  fetch_http(buf, "%s", "GET /secret HTTP/1.0\r\n\r\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 401");
#if MG_ENABLE_HTTP_STREAMING_MULTIPART
  fetch_http(buf, "%s",
             "POST /secret/upload HTTP/1.0\r\n"
             "Content-Type: multipart/form-data;boundary=omgwtf\r\n"
             "\r\n--omgwtf\r\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 401");
#endif

  /* Test digest authorization success */
  mg_http_create_digest_auth_header(auth_hdr, sizeof(auth_hdr), "GET",
                                    "/data/auth/a.txt?a=b", "foo.com", "joe",
                                    "doe", nonce);
  fetch_http(buf, "GET /data/auth/a.txt?a=b HTTP/1.0\r\n%s\r\n\r\n", auth_hdr);
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 200");
  ASSERT_STREQ(buf + strlen(buf) - 7, "\r\n\r\nhi\n");
  mg_http_create_digest_auth_header(auth_hdr, sizeof(auth_hdr), "GET",
                                    "/secret", "foo.com", "joe", "doe", nonce);
  fetch_http(buf, "GET /secret HTTP/1.0\r\n%s\r\n\r\n", auth_hdr);
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 200");
  ASSERT_STREQ(buf + strlen(buf) - 7, "Secret!");

  /* Test digest authorization failure with non-existing passwords file */
  mg_http_create_digest_auth_header(auth_hdr, sizeof(auth_hdr), "GET",
                                    "/data/auth/a.txt?a=b", "foo.com", "joe",
                                    "doe", nonce);
  fetch_http2(buf, "data/auth/non-existing-passwords.txt",
              "GET /data/auth/a.txt?a=b HTTP/1.0\r\n%s\r\n\r\n", auth_hdr);
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 200");
  ASSERT_STREQ(buf + strlen(buf) - 7, "\r\n\r\nhi\n");
  mg_http_create_digest_auth_header(auth_hdr, sizeof(auth_hdr), "GET",
                                    "/secret", "foo.com", "joe", "doe", nonce);
  fetch_http2(buf, "data/auth/non-existing-passwords.txt",
              "GET /secret HTTP/1.0\r\n%s\r\n\r\n", auth_hdr);
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 401");

  /* Test digest authorization failure with old nonce */
  {
    /*
     * Add 10 seconds to the nonce value, so that it'll be in the future
     * and mg_check_nonce() should fail.
     */
    unsigned long nonce_val = strtoul(nonce, NULL, 16);
    sprintf(nonce, "%lx", nonce_val + 10);

    mg_http_create_digest_auth_header(auth_hdr, sizeof(auth_hdr), "GET",
                                      "/data/auth/a.txt?a=b", "foo.com", "joe",
                                      "doe", nonce);
    fetch_http(buf, "GET /data/auth/a.txt?a=b HTTP/1.0\r\n%s\r\n\r\n",
               auth_hdr);
    ASSERT_STREQ_NZ(buf, "HTTP/1.1 401");

    mg_http_create_digest_auth_header(auth_hdr, sizeof(auth_hdr), "GET",
                                      "/secret", "foo.com", "joe", "doe",
                                      nonce);

    fetch_http(buf, "GET /secret HTTP/1.0\r\n%s\r\n\r\n", auth_hdr);
    ASSERT_STREQ_NZ(buf, "HTTP/1.1 401");

    /* Make nonce 1-hour-old, so mg_check_nonce() should fail again. */
    sprintf(nonce, "%lx", nonce_val - 60 * 60);

    mg_http_create_digest_auth_header(auth_hdr, sizeof(auth_hdr), "GET",
                                      "/data/auth/a.txt?a=b", "foo.com", "joe",
                                      "doe", nonce);
    fetch_http(buf, "GET /data/auth/a.txt?a=b HTTP/1.0\r\n%s\r\n\r\n",
               auth_hdr);
    ASSERT_STREQ_NZ(buf, "HTTP/1.1 401");

    /* Renew nonce to represent the actual nonce from the server */
    bufstr = mg_mk_str(buf);
    ASSERT_GT(mg_http_parse_header(&bufstr, "nonce", nonce, sizeof(nonce)), 0);
  }

  /* Test that passwords file is hidden from view */
  mg_http_create_digest_auth_header(auth_hdr, sizeof(auth_hdr), "GET",
                                    "/data/auth/p%61sswords%2etxt", "foo.com",
                                    "joe", "doe", nonce);
  fetch_http(buf,
             "GET /data/auth/p%%61sswords%%2etxt HTTP/1.0\r\n"
             "%s\r\n\r\n",
             auth_hdr);
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 404");
  /* On case-insensitive filesystems too. */
  mg_http_create_digest_auth_header(auth_hdr, sizeof(auth_hdr), "GET",
                                    "/data/auth/Passwords.txt", "foo.com",
                                    "joe", "doe", nonce);
  fetch_http(buf,
             "GET /data/auth/Passwords.txt HTTP/1.0\r\n"
             "%s\r\n\r\n",
             auth_hdr);
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 404");

  return NULL;
}

static const char *test_http_errors(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  const char *local_addr = "127.0.0.1:7777";
  char status[1000] = "";

  mg_mgr_init(&mgr, NULL);
  s_http_server_opts.enable_directory_listing = NULL;
  ASSERT((nc = mg_bind(&mgr, local_addr, cb1)) != NULL);
  mg_set_protocol_http_websocket(nc);

#if !defined(TEST_UNDER_VIRTUALBOX) && !defined(_WIN32)
  /* Test file which exists but cannot be opened */
  ASSERT((nc = mg_connect(&mgr, local_addr, cb8)) != NULL);
  mg_set_protocol_http_websocket(nc);
  nc->user_data = status;
  system("touch test_unreadable; chmod 000 test_unreadable");
  mg_printf(nc, "GET /%s HTTP/1.0\n\n", "../test_unreadable");

  /* Run event loop. Use more cycles to let file download complete. */
  poll_until(&mgr, 1, c_str_ne, status, (void *) "");
  system("rm -f test_unreadable");

  /* Check that it failed */
  ASSERT_STREQ_NZ(status, "HTTP/1.1 403");
#endif

  /* Test non existing file */
  ASSERT((nc = mg_connect(&mgr, local_addr, cb8)) != NULL);
  mg_set_protocol_http_websocket(nc);
  status[0] = '\0';
  nc->user_data = status;
  mg_printf(nc, "GET /%s HTTP/1.0\n\n", "/please_dont_create_this_file_srsly");

  poll_until(&mgr, 1, c_str_ne, status, (void *) "");

  /* Check that it failed */
  ASSERT_STREQ_NZ(status, "HTTP/1.1 404");

  /* Cleanup */
  mg_mgr_free(&mgr);

  return NULL;
}

static const char *test_http_index(void) {
  char buf[FETCH_BUF_SIZE];
  fetch_http(buf, "%s", "GET /data/dir_with_index/ HTTP/1.0\r\n\r\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 200");
  ASSERT(strstr(buf, "Content-Length: 3\r\n") != 0);
  ASSERT_STREQ(buf + strlen(buf) - 5, "\r\nfoo");
  /* Directory listing: allowed by default. */
  s_http_server_opts.enable_directory_listing = NULL;
  fetch_http(buf, "%s", "GET /data/dir_no_index/ HTTP/1.0\r\n\r\n");
  ASSERT_STREQ_NZ(buf,
                  "HTTP/1.1 200 OK\r\n"
                  "Server: Mongoose/" MG_VERSION
                  "\r\n"
                  "Transfer-Encoding: chunked\r\n");
  ASSERT(strstr(buf, "<html><head><title>") != NULL);
  /* Allowed explicitly. */
  s_http_server_opts.enable_directory_listing = "yes";
  fetch_http(buf, "%s", "GET /data/dir_no_index/ HTTP/1.0\r\n\r\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 200");
  /* Disallowed. */
  s_http_server_opts.enable_directory_listing = "no";
  fetch_http(buf, "%s", "GET /data/dir_no_index/ HTTP/1.0\r\n\r\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 403");
  return NULL;
}

static const char *test_ssi(void) {
  char buf[FETCH_BUF_SIZE];
  fetch_http(buf, "%s", "GET /data/ssi/ HTTP/1.0\n\n");
  ASSERT_STREQ(buf,
               "HTTP/1.1 200 OK\r\n"
               "Server: Mongoose/" MG_VERSION
               "\r\n"
               "Content-Type: text/html\r\n"
               "Connection: close\r\n\r\na\n\n"
               "<b>[ssi call: foo(bar)]"
               "[ssi call w/ ctx: /data/ssi/ ." DS "data" DS "ssi" DS
               "nested.shtml foo(bar)]"
               "</b>\nb\n\n\n");
  return NULL;
}

#ifndef NO_CGI_TEST
static const char *test_cgi(void) {
  char buf[FETCH_BUF_SIZE];
  char expected_server_port[100];
  const char *post_data = "aa=1234&bb=hi there";

  fetch_http(buf,
             "POST /data/cgi/ HTTP/1.0\r\n"
             "Content-Type: application/x-www-form-urlencoded\r\n"
             "Content-Length: %d\n\n%s",
             (int) strlen(post_data), post_data);

  /* fprintf(stderr, "%s", buf); */

  snprintf(expected_server_port, sizeof(expected_server_port),
           "\nE: SERVER_PORT=%d\r\n", s_listening_port - 1);
  ASSERT(strstr(buf, expected_server_port) != NULL);

  ASSERT_STREQ_NZ(buf, "HTTP/1.1 201 Created\r\n");
  ASSERT(strstr(buf, "\nE: CONTENT_LENGTH=19\r\n") != NULL);
  ASSERT(
      strstr(buf, "\nE: CONTENT_TYPE=application/x-www-form-urlencoded\r\n") !=
      NULL);
  ASSERT(strstr(buf, "\nE: DOCUMENT_ROOT=.\r\n") != NULL);
  ASSERT(strstr(buf, "\nE: HTTPS=off\r\n") != NULL);
  ASSERT(strstr(buf, "\nE: HTTP_CONTENT_LENGTH=19\r\n") != NULL);
  ASSERT(
      strstr(buf,
             "\nE: HTTP_CONTENT_TYPE=application/x-www-form-urlencoded\r\n") !=
      NULL);
  ASSERT(strstr(buf, "\nE: REMOTE_ADDR=127.0.0.1\r\n") != NULL);
  ASSERT(strstr(buf, "\nE: REQUEST_METHOD=POST\r\n") != NULL);
  ASSERT(strstr(buf, "\nE: REQUEST_URI=/data/cgi/\r\n") != NULL);
  ASSERT(strstr(buf, "\nE: SCRIPT_NAME=/data/cgi/index.cgi\r\n") != NULL);
  ASSERT(strstr(buf, "\nE: SCRIPT_FILENAME=." DS "data" DS "cgi" DS
                     "index.cgi\r\n") != NULL);
  ASSERT(strstr(buf, "\nE: PATH_INFO=") == NULL);
  ASSERT(strstr(buf, "\nE: PATH_TRANSLATED=") == NULL);
  ASSERT(strstr(buf, "\nE: SERVER_SOFTWARE=Mongoose/" MG_VERSION "\r\n") !=
         NULL);
  ASSERT(strstr(buf, "\nP: aa=1234\r\n") != NULL);
  ASSERT(strstr(buf, "\nP: bb=hi there\r\n") != NULL);

  fetch_http(buf,
             "GET /data/cgi/index%%2ecgi/foo%%2fbar/baz/?x=y HTTP/1.0\r\n\r\n");

  ASSERT_STREQ_NZ(buf, "HTTP/1.1 201 Created\r\n");
  ASSERT(strstr(buf, "\nE: CONTENT_LENGTH=") == NULL);
  ASSERT(strstr(buf, "\nE: CONTENT_TYPE=") == NULL);
  ASSERT(strstr(buf, "\nE: DOCUMENT_ROOT=.\r\n") != NULL);
  ASSERT(strstr(buf, "\nE: HTTPS=off\r\n") != NULL);
  ASSERT(strstr(buf, "\nE: HTTP_CONTENT_LENGTH=") == NULL);
  ASSERT(strstr(buf, "\nE: HTTP_CONTENT_TYPE=") == NULL);
  ASSERT(strstr(buf, "\nE: REQUEST_METHOD=GET\r\n") != NULL);
  ASSERT(
      strstr(buf,
             "\nE: REQUEST_URI=/data/cgi/index%2ecgi/foo%2fbar/baz/?x=y\r\n") !=
      NULL);
  ASSERT(strstr(buf, "\nE: SCRIPT_NAME=/data/cgi/index%2ecgi\r\n") != NULL);
  ASSERT(strstr(buf, "\nE: SCRIPT_FILENAME=." DS "data" DS "cgi" DS
                     "index.cgi\r\n") != NULL);
  ASSERT(strstr(buf, "\nE: PATH_INFO=/foo%2fbar/baz/") != NULL);
  ASSERT(strstr(buf, "\nE: PATH_TRANSLATED=/foo%2fbar/baz/") != NULL);
  ASSERT(strstr(buf, "\nE: SERVER_SOFTWARE=Mongoose/" MG_VERSION "\r\n") !=
         NULL);
  ASSERT(strstr(buf, "\nQ: x=y\r\n") != NULL);

  return NULL;
}
#endif

static const char *test_http_rewrites(void) {
  char buf[FETCH_BUF_SIZE];

  s_http_server_opts.url_rewrites =
      "/~joe=./data/rewrites,"
      "@foo.com=./data/rewrites/foo.com";

  /* Test rewrite */
  fetch_http(buf, "%s", "GET /~joe/msg.txt HTTP/1.0\nHost: foo.co\n\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 200 OK");
  ASSERT(strstr(buf, "Content-Length: 6\r\n") != 0);
  ASSERT_STREQ(buf + strlen(buf) - 8, "\r\nworks\n");

  /* Test rewrite that points to directory, expect redirect */
  fetch_http(buf, "%s", "GET /~joe HTTP/1.0\n\n");
  ASSERT_STREQ(buf,
               "HTTP/1.1 301 Moved\r\nLocation: /~joe/\r\n"
               "Content-Length: 0\r\n\r\n");

  /* Test domain-based rewrite */
  fetch_http(buf, "%s", "GET / HTTP/1.0\nHost: foo.com\n\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 200 OK");
  ASSERT(strstr(buf, "Content-Length: 9\r\n") != 0);
  ASSERT_STREQ(buf + strlen(buf) - 11, "\r\nfoo_root\n");

  /* Test port-based rewrites */
  s_listening_port = 25555;
  s_http_server_opts.url_rewrites = "%25555=https://foo";
  fetch_http(buf, "%s", "GET / HTTP/1.0\n\n");
  ASSERT_STREQ(buf,
               "HTTP/1.1 301 Moved\r\n"
               "Server: Mongoose/" MG_VERSION
               "\r\n"
               "Content-Length: 0\r\n"
               "Location: https://foo/\r\n\r\n");

  return NULL;
}

static const char *test_http_dav(void) {
  char buf[FETCH_BUF_SIZE];
  cs_stat_t st;

  remove("./data/dav/b.txt");
  rmdir("./data/dav/d");

  /* Test PROPFIND  */
  s_http_server_opts.enable_directory_listing = "yes";
  fetch_http(buf, "%s", "PROPFIND / HTTP/1.0\n\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 207");
  ASSERT(strstr(buf, "a.txt") != NULL);
  ASSERT(strstr(buf, "hidden_file.txt") == NULL);

  /* Test MKCOL */
  fetch_http(buf, "%s", "MKCOL /d HTTP/1.0\nContent-Length:5\n\n12345");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 415");
  fetch_http(buf, "%s", "MKCOL /d HTTP/1.0\n\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 201");
  fetch_http(buf, "%s", "MKCOL /d HTTP/1.0\n\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 405");
  fetch_http(buf, "%s", "MKCOL /x/d HTTP/1.0\n\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 409");

  /* Test PUT */
  fetch_http(buf, "%s", "PUT /b.txt HTTP/1.0\nContent-Length: 5\n\n12345");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 201");
  fetch_http(buf, "%s", "GET /data/dav/b.txt HTTP/1.0\n\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 200");
  ASSERT(strstr(buf, "Content-Length: 5\r\n") != 0);
  ASSERT_STREQ(buf + strlen(buf) - 7, "\r\n12345");

  /* Test DELETE */
  fetch_http(buf, "%s", "DELETE /b.txt HTTP/1.0\n\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 204");
  ASSERT(mg_stat("./data/dav/b.txt", &st) != 0);
  fetch_http(buf, "%s", "DELETE /d HTTP/1.0\n\n");
  ASSERT(mg_stat("./data/dav/d", &st) != 0);

  /* Non-existing file */
  fetch_http(buf, "%s", "PROPFIND /__blah.txt HTTP/1.0\n\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 404");

  return NULL;
}

static const char *test_http_range(void) {
  char buf[FETCH_BUF_SIZE];

  fetch_http(buf, "%s", "GET /data/range.txt HTTP/1.0\n\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 200 OK");
  ASSERT(strstr(buf, "Content-Length: 312\r\n") != 0);

  /* Fetch a piece from the middle of the file */
  fetch_http(buf, "%s", "GET /data/range.txt HTTP/1.0\nRange: bytes=5-10\n\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 206 Partial Content");
  ASSERT(strstr(buf, "Content-Length: 6\r\n") != 0);
  ASSERT(strstr(buf, "Content-Range: bytes 5-10/312\r\n") != 0);
  ASSERT_STREQ(buf + strlen(buf) - 8, "\r\n of co");

  /* Fetch till EOF */
  fetch_http(buf, "%s", "GET /data/range.txt HTTP/1.0\nRange: bytes=300-\n\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 206 Partial Content");
  ASSERT(strstr(buf, "Content-Length: 12\r\n") != 0);
  ASSERT(strstr(buf, "Content-Range: bytes 300-311/312\r\n") != 0);
  ASSERT_STREQ(buf + strlen(buf) - 14, "\r\nis disease.\n");

  /* Fetch past EOF, must trigger 416 response */
  fetch_http(buf, "%s", "GET /data/range.txt HTTP/1.0\nRange: bytes=1000-\n\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 416");
  ASSERT(strstr(buf, "Content-Length: 0\r\n") != 0);
  ASSERT(strstr(buf, "Content-Range: bytes */312\r\n") != 0);

  /* Request range past EOF, must trigger 416 response */
  fetch_http(buf, "%s", "GET /data/range.txt HTTP/1.0\nRange: bytes=0-312\n\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 416");

  return NULL;
}

static void cb_ws_server(struct mg_connection *nc, int ev, void *ev_data) {
  struct websocket_message *wm = (struct websocket_message *) ev_data;

  if (ev == MG_EV_WEBSOCKET_FRAME || ev == MG_EV_WEBSOCKET_CONTROL_FRAME) {
    mg_printf_websocket_frame(
        nc, WEBSOCKET_OP_TEXT, "%s%.2x:{%.*s}",
        (ev == MG_EV_WEBSOCKET_CONTROL_FRAME ? "CONTROL:" : ""), wm->flags,
        (int) wm->size, wm->data);
  }
}

static int s_ws_client1_connected = false;
static void cb_ws_client1(struct mg_connection *nc, int ev, void *ev_data) {
  struct websocket_message *wm = (struct websocket_message *) ev_data;

  if (ev == MG_EV_WEBSOCKET_FRAME || ev == MG_EV_WEBSOCKET_CONTROL_FRAME) {
    char *tmp = NULL;
    int tmplen;
    tmplen =
        mg_asprintf(&tmp, 0, "%s%.2x:[%.*s]",
                    (ev == MG_EV_WEBSOCKET_CONTROL_FRAME ? "CONTROL:" : ""),
                    wm->flags, (int) wm->size, wm->data);
    mbuf_append((struct mbuf *) nc->user_data, tmp, tmplen);
    free(tmp);
  } else if (ev == MG_EV_CLOSE) {
    s_ws_client1_connected = 0;
  } else if (ev == MG_EV_WEBSOCKET_HANDSHAKE_DONE) {
    s_ws_client1_connected = 1;
  }
}

static const char *test_websocket(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  const char *local_addr = "127.0.0.1:7778";
  struct mbuf mb;
  mbuf_init(&mb, 100);

  mg_mgr_init(&mgr, NULL);
  /* mgr.hexdump_file = "-"; */
  ASSERT((nc = mg_bind(&mgr, local_addr, cb_ws_server)) != NULL);
  mg_set_protocol_http_websocket(nc);

  /*
   * Websocket request "hi" via mg_send_websocket_framev()
   */
  mb.len = 0;
  s_ws_client1_connected = 0;
  ASSERT((nc = mg_connect(&mgr, local_addr, cb_ws_client1)) != NULL);
  mg_set_protocol_http_websocket(nc);
  nc->user_data = &mb;
  mg_send_websocket_handshake(nc, "/ws", NULL);
  poll_until(&mgr, 1, c_int_ne, &s_ws_client1_connected, (void *) 1);
  {
    /* Send "hi" to server. server must reply "A". */
    struct mg_str h[2];
    h[0].p = "h";
    h[0].len = 1;
    h[1].p = "i";
    h[1].len = 1;
    mg_send_websocket_framev(nc, WEBSOCKET_OP_TEXT, h, 2);
  }
  poll_until(&mgr, 1, c_int_ne, &mb.len, (void *) 0);
  mbuf_append(&mb, "\0", 1);
  /* Check that test buffer has been filled by the callback properly. */
  ASSERT_STREQ(mb.buf, "81:[81:{hi}]");

  /*
   * Websocket request "hallloo", composed from two fragments
   */
  mb.len = 0;
  s_ws_client1_connected = 0;
  ASSERT((nc = mg_connect(&mgr, local_addr, cb_ws_client1)) != NULL);
  mg_set_protocol_http_websocket(nc);
  nc->user_data = &mb;
  mg_send_websocket_handshake(nc, "/ws", NULL);
  poll_until(&mgr, 1, c_int_ne, &s_ws_client1_connected, (void *) 1);
  ASSERT_EQ(s_ws_client1_connected, 1);

  mg_send_websocket_frame(nc, WEBSOCKET_OP_TEXT | WEBSOCKET_DONT_FIN, "hall",
                          4);
  mg_send_websocket_frame(nc, WEBSOCKET_OP_CONTINUE | WEBSOCKET_DONT_FIN, "--",
                          2);

  /* Poll 0.5 seconds, we don't expect c_str_ne predicate to return true */
  poll_until(&mgr, 0.5, c_int_ne, &mb.len, (void *) 0);
  ASSERT_EQ(mb.len, 0);

  mg_send_websocket_frame(nc, WEBSOCKET_OP_CONTINUE, "loo", 3);

  poll_until(&mgr, 1, c_int_ne, &mb.len, (void *) 0);
  mbuf_append(&mb, "\0", 1);
  /* Check that test buffer has been filled by the callback properly. */
  ASSERT_STREQ(mb.buf, "81:[81:{hall--loo}]");

  /*
   * Ping request
   */
  mb.len = 0;
  s_ws_client1_connected = 0;
  ASSERT((nc = mg_connect(&mgr, local_addr, cb_ws_client1)) != NULL);
  mg_set_protocol_http_websocket(nc);
  nc->user_data = &mb;
  mg_send_websocket_handshake(nc, "/ws", NULL);
  poll_until(&mgr, 1, c_int_ne, &s_ws_client1_connected, (void *) 1);
  ASSERT_EQ(s_ws_client1_connected, 1);

  mg_send_websocket_frame(nc, WEBSOCKET_OP_PING, "myping", 6);

  {
    /*
     * As a response to Ping, we should first receive Pong (0x8a),
     * and then text message (0x81) with our Ping (0x89) echo.
     */
    const char *expected = "CONTROL:8a:[myping]81:[CONTROL:89:{myping}]";
    poll_until(&mgr, 1, c_int_eq, &mb.len, (void *) strlen(expected));
    mbuf_append(&mb, "\0", 1);
    ASSERT_STREQ(mb.buf, expected);
  }

  /*
   * Ping request injected in between of a fragmented message
   */
  mb.len = 0;
  s_ws_client1_connected = 0;
  ASSERT((nc = mg_connect(&mgr, local_addr, cb_ws_client1)) != NULL);
  mg_set_protocol_http_websocket(nc);
  nc->user_data = &mb;
  mg_send_websocket_handshake(nc, "/ws", NULL);
  poll_until(&mgr, 1, c_int_ne, &s_ws_client1_connected, (void *) 1);
  ASSERT_EQ(s_ws_client1_connected, 1);

  mg_send_websocket_frame(nc, WEBSOCKET_OP_TEXT | WEBSOCKET_DONT_FIN, "abc", 3);
  mg_send_websocket_frame(nc, WEBSOCKET_OP_CONTINUE | WEBSOCKET_DONT_FIN, "def",
                          3);

  mg_send_websocket_frame(nc, WEBSOCKET_OP_PING, "0123", 4);

  mg_send_websocket_frame(nc, WEBSOCKET_OP_CONTINUE, "ghi", 3);

  {
    /*
     * As a response to Ping, we should first receive Pong (0x8a),
     * and then text message (0x81) with our Ping (0x89) echo.
     * And then, our fragmented text message: abcdefghi.
     */
    const char *expected =
        "CONTROL:8a:[0123]81:[CONTROL:89:{0123}]81:[81:{abcdefghi}]";
    poll_until(&mgr, 1, c_int_eq, &mb.len, (void *) strlen(expected));
    mbuf_append(&mb, "\0", 1);
    ASSERT_STREQ(mb.buf, expected);
  }

  /*
   * Test illegal text frame in the middle of a fragmented message
   */
  mb.len = 0;
  s_ws_client1_connected = 0;
  ASSERT((nc = mg_connect(&mgr, local_addr, cb_ws_client1)) != NULL);
  mg_set_protocol_http_websocket(nc);
  nc->user_data = &mb;
  mg_send_websocket_handshake(nc, "/ws", NULL);
  poll_until(&mgr, 1, c_int_ne, &s_ws_client1_connected, (void *) 1);
  ASSERT_EQ(s_ws_client1_connected, 1);

  /* Send a few parts of a fragmented message */
  mg_send_websocket_frame(nc, WEBSOCKET_OP_TEXT | WEBSOCKET_DONT_FIN, "abc", 3);
  mg_send_websocket_frame(nc, WEBSOCKET_OP_CONTINUE | WEBSOCKET_DONT_FIN, "def",
                          3);

  /* Send (illegal) text frame in the middle of a fragmented message */
  mg_send_websocket_frame(nc, WEBSOCKET_OP_TEXT, "ghi", 3);

  /* Wait until connection is closed by the server */
  poll_until(&mgr, 1, c_int_ne, &s_ws_client1_connected, (void *) 0);
  ASSERT_EQ(s_ws_client1_connected, 0);

  /* Verify the error message */
  mbuf_append(&mb, "\0", 1);
  ASSERT_STREQ(
      mb.buf,
      "CONTROL:88:[non-continuation in the middle of a fragmented message]");

  /*
   * Test closing by the client
   */
  mb.len = 0;
  s_ws_client1_connected = 0;
  ASSERT((nc = mg_connect(&mgr, local_addr, cb_ws_client1)) != NULL);
  mg_set_protocol_http_websocket(nc);
  nc->user_data = &mb;
  mg_send_websocket_handshake(nc, "/ws", NULL);
  poll_until(&mgr, 1, c_int_ne, &s_ws_client1_connected, (void *) 1);
  ASSERT_EQ(s_ws_client1_connected, 1);

  /* Send a few parts of a fragmented message */
  mg_send_websocket_frame(nc, WEBSOCKET_OP_CLOSE, "bye", 3);

  /* Wait until connection is closed by the server */
  poll_until(&mgr, 1, c_int_ne, &s_ws_client1_connected, (void *) 0);
  ASSERT_EQ(s_ws_client1_connected, 0);
  /*
   * TODO(dfrank): mongoose closes the connection automatically when
   * mg_send_websocket_frame() is called with a WEBSOCKET_OP_CLOSE op, so we
   * can't hear anything from the server. Server is actually obliged to send
   * the control frame in response, but we can't test it here.
   */

  mg_mgr_free(&mgr);
  mbuf_free(&mb);

  return NULL;
}

static void cb3(struct mg_connection *nc, int ev, void *ev_data) {
  struct websocket_message *wm = (struct websocket_message *) ev_data;
  if (ev != MG_EV_WEBSOCKET_FRAME) return;
  DBG(("server data '%.*s'", (int) wm->size, wm->data));
  const char *reply = wm->size == 2 && !memcmp(wm->data, "hi", 2) ? "A" : "B";
  mg_printf_websocket_frame(nc, WEBSOCKET_OP_TEXT, "%s", reply);
}

static void cb4(struct mg_connection *nc, int ev, void *ev_data) {
  char *buf = (char *) nc->user_data;
  if (ev == MG_EV_WEBSOCKET_FRAME) {
    struct websocket_message *wm = (struct websocket_message *) ev_data;
    DBG(("client data '%.*s'", (int) wm->size, wm->data));
    memcpy(buf, wm->data, wm->size);
    mg_send_websocket_frame(nc, WEBSOCKET_OP_CLOSE, NULL, 0);
  } else if (ev == MG_EV_WEBSOCKET_HANDSHAKE_DONE) {
    struct http_message *hm = (struct http_message *) ev_data;
    DBG(("code %d", hm->resp_code));
    if (hm->resp_code == 101) {
      /* Send "hi" to server. server must reply "A". */
      struct mg_str h[2];
      h[0].p = "h";
      h[0].len = 1;
      h[1].p = "i";
      h[1].len = 1;
      mg_send_websocket_framev(nc, WEBSOCKET_OP_TEXT, h, 2);
    } else {
      snprintf(buf, 20, "code %d", hm->resp_code);
    }
  }
}

static void cbwep(struct mg_connection *c, int ev, void *ev_data) {
  struct websocket_message *wm = (struct websocket_message *) ev_data;
  char *buf = (char *) c->user_data;

  switch (ev) {
    case MG_EV_WEBSOCKET_HANDSHAKE_REQUEST:
      strcat(buf, "R");
      if (buf[0] != '0') {
        mg_http_send_error(c, 403, "I don't like you");
      }
      break;
    case MG_EV_WEBSOCKET_HANDSHAKE_DONE:
      strcat(buf, "D");
      break;
    case MG_EV_WEBSOCKET_FRAME:
      strcat(buf, "F");
      mg_printf_websocket_frame(c, WEBSOCKET_OP_TEXT, "%s|%.*s", buf,
                                (int) wm->size, wm->data);
      break;
  }
}

static const char *test_websocket_endpoint(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  const char *local_addr = "127.0.0.1:7798";
  char buf[20] = "", buf2[20] = "0";

  mg_mgr_init(&mgr, NULL);
  /* mgr.hexdump_file = "-"; */
  ASSERT((nc = mg_bind(&mgr, local_addr, cb3)) != NULL);
  mg_set_protocol_http_websocket(nc);
  nc->user_data = buf2;
  mg_register_http_endpoint(nc, "/boo", cbwep MG_UD_ARG(NULL));

  /* Websocket request */
  ASSERT((nc = mg_connect(&mgr, local_addr, cb4)) != NULL);
  mg_set_protocol_http_websocket(nc);
  nc->user_data = buf;
  mg_send_websocket_handshake(nc, "/boo", NULL);
  poll_until(&mgr, 1, c_str_ne, buf, (void *) "");
  /* Check that test buffer has been filled by the callback properly. */
  ASSERT_STREQ(buf, "0RDF|hi");

  /* Test handshake failure */
  ASSERT((nc = mg_connect(&mgr, local_addr, cb4)) != NULL);
  mg_set_protocol_http_websocket(nc);
  buf[0] = '\0';
  buf2[0] = '1';
  buf2[1] = '\0';
  nc->user_data = buf;
  mg_send_websocket_handshake(nc, "/boo", NULL);
  poll_until(&mgr, 1, c_str_ne, buf, (void *) "");
  ASSERT_STREQ(buf, "code 403");

  mg_mgr_free(&mgr);

  return NULL;
}

static const char *test_connect_ws(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  const char *urls[] = {"ws://127.0.0.1:7778/ws", "http://127.0.0.1:7778/ws",
                        "127.0.0.1:7778/ws", NULL};
  const char **url;

  mg_mgr_init(&mgr, NULL);
  /* mgr.hexdump_file = "-"; */
  ASSERT((nc = mg_bind(&mgr, "127.0.0.1:7778", cb3)) != NULL);
  mg_set_protocol_http_websocket(nc);

  for (url = urls; *url != NULL; url++) {
    char buf[20] = "";
    ASSERT((nc = mg_connect_ws(&mgr, cb4, "ws://127.0.0.1:7778/ws", NULL,
                               NULL)) != NULL);
    nc->user_data = buf;
    poll_until(&mgr, 1, c_str_ne, buf, (void *) "");
    ASSERT_STREQ(buf, "A");
  }

  mg_mgr_free(&mgr);

  return NULL;
}

struct big_payload_params {
  size_t size;
  char *buf;
};

static void cb3_big(struct mg_connection *nc, int ev, void *ev_data) {
  struct websocket_message *wm = (struct websocket_message *) ev_data;

  if (ev == MG_EV_WEBSOCKET_FRAME) {
    int success = 1;
    size_t i;
    for (i = 0; i < wm->size; i++) {
      if (wm->data[i] != 'x') {
        success = 0;
        break;
      }
    }
    mg_printf_websocket_frame(nc, WEBSOCKET_OP_TEXT, "%s",
                              success ? "success" : "fail");
  }
}

static void cb4_big(struct mg_connection *nc, int ev, void *ev_data) {
  struct websocket_message *wm = (struct websocket_message *) ev_data;
  struct big_payload_params *params =
      (struct big_payload_params *) nc->user_data;

  if (ev == MG_EV_WEBSOCKET_FRAME) {
    memcpy(params->buf, wm->data, wm->size);
    mg_send_websocket_frame(nc, WEBSOCKET_OP_CLOSE, NULL, 0);
  } else if (ev == MG_EV_WEBSOCKET_HANDSHAKE_DONE) {
    /* Send large payload to server. server must reply "success". */
    char *payload = (char *) malloc(params->size);
    memset(payload, 'x', params->size);
    mg_printf_websocket_frame(nc, WEBSOCKET_OP_TEXT, "%.*s", params->size,
                              payload);
    free(payload);
  }
}

/* Big payloads follow a different code path because it will use the extended
 * length field and possibly mg_avprintf will need to reallocate the buffer. */
static const char *test_websocket_big(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  const char *local_addr = "127.0.0.1:7778";
  char buf[20] = "";
  struct big_payload_params params;
  params.buf = buf;

  mg_mgr_init(&mgr, NULL);
  /* mgr.hexdump_file = "-"; */
  ASSERT((nc = mg_bind(&mgr, local_addr, cb3_big)) != NULL);
  mg_set_protocol_http_websocket(nc);

  /* Websocket request */
  ASSERT((nc = mg_connect(&mgr, local_addr, cb4_big)) != NULL);
  mg_set_protocol_http_websocket(nc);
  params.size = 8192;
  nc->user_data = &params;
  params.buf[0] = '\0';
  mg_send_websocket_handshake(nc, "/ws", NULL);
  poll_until(&mgr, 1, c_str_ne, params.buf, (void *) "");

  /* Check that test buffer has been filled by the callback properly. */
  ASSERT_STREQ(buf, "success");

  /* Websocket request */
  ASSERT((nc = mg_connect(&mgr, local_addr, cb4_big)) != NULL);
  mg_set_protocol_http_websocket(nc);
  params.size = 65535;
  nc->user_data = &params;
  params.buf[0] = '\0';
  mg_send_websocket_handshake(nc, "/ws", NULL);
  poll_until(&mgr, 1, c_str_ne, params.buf, (void *) "");
  mg_mgr_free(&mgr);

  /* Check that test buffer has been filled by the callback properly. */
  ASSERT_STREQ(buf, "success");

  return NULL;
}

static const char *test_mqtt_handshake(void) {
  struct mg_send_mqtt_handshake_opts opts;
  struct mg_connection *nc = create_test_connection();
  const char *client_id = "testclient";
  const char *user_name = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
  const char *password = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
  const char *will_topic = "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC";
  const char *will_message = "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";
  double before = mg_time();
  const char *got;

  msleep(2);
  mg_set_protocol_mqtt(nc);
  mg_send_mqtt_handshake(nc, client_id);
  got = nc->send_mbuf.buf;
  /* handshake header + keepalive + client id len + client id */
  ASSERT_EQ(nc->send_mbuf.len,
            1 + 1 + 1 + 5 + 1 + 1 + 2 + 2 + strlen(client_id));
  ASSERT_EQ(got[0], 0x10);
  ASSERT_EQ(got[1], (int) nc->send_mbuf.len - 2);
  ASSERT_EQ(got[2], 0);
  ASSERT_EQ(got[3], 4);
  ASSERT_STREQ_NZ(&got[4], "MQTT");
  ASSERT_EQ(got[8], 4);
  ASSERT_EQ(got[9], 0); /* connect flags */
  ASSERT_EQ(got[10], 0);
  ASSERT_EQ(got[11], 60);
  ASSERT_EQ(got[12], 0);
  ASSERT_EQ(got[13], (char) strlen(client_id));
  ASSERT_EQ(strncmp(&got[14], client_id, strlen(client_id)), 0);
  ASSERT_EQ(((struct mg_mqtt_proto_data *) nc->proto_data)->keep_alive, 60);
  ASSERT_GT(((struct mg_mqtt_proto_data *) nc->proto_data)->last_control_time,
            before);
  mbuf_remove(&nc->send_mbuf, nc->send_mbuf.len);

  memset(&opts, 0, sizeof(opts));
  before = mg_time();
  msleep(2);
  opts.will_topic = will_topic;
  opts.will_message = will_message;
  opts.user_name = user_name;
  opts.password = password;
  opts.keep_alive = 0x1234;
  mg_send_mqtt_handshake_opt(nc, client_id, opts);
  got = nc->send_mbuf.buf;
  /* handshake header + keepalive + client id len + client id */
  ASSERT_EQ(nc->send_mbuf.len, 1 + 2 + 1 + 5 + 1 + 1 + 2 + 2 +
                                   strlen(client_id) + 2 + strlen(will_topic) +
                                   2 + strlen(will_message) + 2 +
                                   strlen(user_name) + 2 + strlen(password));
  ASSERT_EQ(got[0], 0x10);
  ASSERT_EQ(((unsigned char *) got)[1], 223);
  ASSERT_EQ(got[2], 1);
  ASSERT_EQ(got[3], 0);
  ASSERT_EQ(got[4], 4);
  ASSERT_STREQ_NZ(&got[5], "MQTT");
  ASSERT_EQ(got[9], 4);
  ASSERT_EQ(((unsigned char *) got)[10], 0xc4); /* connect flags */
  ASSERT_EQ(got[11], 0x12);
  ASSERT_EQ(got[12], 0x34);
  ASSERT_EQ(got[13], 0);
  ASSERT_EQ(got[14], (char) strlen(client_id));
  ASSERT_EQ(strncmp(&got[15], client_id, strlen(client_id)), 0);
  ASSERT_EQ(((struct mg_mqtt_proto_data *) nc->proto_data)->keep_alive, 0x1234);
  ASSERT_GT(((struct mg_mqtt_proto_data *) nc->proto_data)->last_control_time,
            before);
  mbuf_remove(&nc->send_mbuf, nc->send_mbuf.len);

  destroy_test_connection(nc);
  return NULL;
}

static const char *test_mqtt_publish(void) {
  struct mg_connection *nc = create_test_connection();
  const double before = mg_time();
  char data[] = "dummy";
  const char *got;

  mg_set_protocol_mqtt(nc);
  msleep(2);
  mg_mqtt_publish(nc, "/test", 42, MG_MQTT_QOS(1) | MG_MQTT_RETAIN, data,
                  sizeof(data));
  got = nc->send_mbuf.buf;
  ASSERT_EQ(nc->send_mbuf.len, 17);

  ASSERT(got[0] & MG_MQTT_RETAIN);
  ASSERT_EQ((got[0] & 0xf0), (MG_MQTT_CMD_PUBLISH << 4));
  ASSERT_EQ(MG_MQTT_GET_QOS(got[0]), 1);
  ASSERT_EQ((size_t) got[1], (nc->send_mbuf.len - 2));

  ASSERT_EQ(got[2], 0);
  ASSERT_EQ(got[3], 5);
  ASSERT_STREQ_NZ(&got[4], "/test");

  ASSERT_EQ(got[9], 0);
  ASSERT_EQ(got[10], 42);

  ASSERT_EQ(strncmp(&got[11], data, sizeof(data)), 0);

  ASSERT_GT(((struct mg_mqtt_proto_data *) nc->proto_data)->last_control_time,
            before);

  destroy_test_connection(nc);
  return NULL;
}

static const char *test_mqtt_subscribe(void) {
  struct mg_connection *nc = create_test_connection();
  const double before = mg_time();
  const int qos = 1;
  const char *got;
  struct mg_mqtt_topic_expression topic_expressions[] = {{"/stuff", qos}};
  mg_set_protocol_mqtt(nc);
  msleep(2);

  mg_mqtt_subscribe(nc, topic_expressions, 1, 42);
  got = nc->send_mbuf.buf;
  ASSERT_EQ(nc->send_mbuf.len, 13);
  ASSERT_EQ((got[0] & 0xf0), (MG_MQTT_CMD_SUBSCRIBE << 4));
  ASSERT_EQ((size_t) got[1], (nc->send_mbuf.len - 2));
  ASSERT_EQ(got[2], 0);
  ASSERT_EQ(got[3], 42);

  ASSERT_EQ(got[4], 0);
  ASSERT_EQ(got[5], 6);
  ASSERT_STREQ_NZ(&got[6], "/stuff");
  ASSERT_EQ(got[12], qos);

  ASSERT_GT(((struct mg_mqtt_proto_data *) nc->proto_data)->last_control_time,
            before);

  destroy_test_connection(nc);
  return NULL;
}

static const char *test_mqtt_unsubscribe(void) {
  struct mg_connection *nc = create_test_connection();
  char *topics[] = {(char *) "/stuff"};
  const double before = mg_time();
  const char *got;
  mg_set_protocol_mqtt(nc);
  msleep(2);

  mg_mqtt_unsubscribe(nc, topics, 1, 42);
  got = nc->send_mbuf.buf;
  ASSERT_EQ(nc->send_mbuf.len, 12);
  ASSERT_EQ((got[0] & 0xf0), (MG_MQTT_CMD_UNSUBSCRIBE << 4));
  ASSERT_EQ((size_t) got[1], (nc->send_mbuf.len - 2));
  ASSERT_EQ(got[2], 0);
  ASSERT_EQ(got[3], 42);

  ASSERT_EQ(got[4], 0);
  ASSERT_EQ(got[5], 6);
  ASSERT_STREQ_NZ(&got[6], "/stuff");

  ASSERT_GT(((struct mg_mqtt_proto_data *) nc->proto_data)->last_control_time,
            before);

  destroy_test_connection(nc);
  return NULL;
}

static const char *test_mqtt_connack(void) {
  struct mg_connection *nc = create_test_connection();
  const char *got;
  mg_set_protocol_mqtt(nc);

  mg_mqtt_connack(nc, 42);
  got = nc->send_mbuf.buf;
  ASSERT(nc->send_mbuf.len > 0);
  ASSERT_EQ((got[0] & 0xf0), (MG_MQTT_CMD_CONNACK << 4));
  ASSERT_EQ((size_t) got[1], (nc->send_mbuf.len - 2));
  ASSERT_EQ(got[3], 42);

  destroy_test_connection(nc);
  return NULL;
}

static const char *test_mqtt_suback(void) {
  struct mg_connection *nc = create_test_connection();
  uint8_t qoss[] = {1};
  const char *got;
  mg_set_protocol_mqtt(nc);

  mg_mqtt_suback(nc, qoss, 1, 42);

  got = nc->send_mbuf.buf;
  ASSERT_EQ(nc->send_mbuf.len, 5);
  ASSERT_EQ((got[0] & 0xf0), (MG_MQTT_CMD_SUBACK << 4));
  ASSERT_EQ(MG_MQTT_GET_QOS(got[0]), 1);
  ASSERT_EQ((size_t) got[1], (nc->send_mbuf.len - 2));
  ASSERT_EQ(got[2], 0);
  ASSERT_EQ(got[3], 42);
  ASSERT_EQ(got[4], 1);

  destroy_test_connection(nc);
  return NULL;
}

static const char *test_mqtt_simple_acks(void) {
  unsigned long i;
  struct {
    uint8_t cmd;
    void (*f)(struct mg_connection *, uint16_t);
  } cases[] = {
      {MG_MQTT_CMD_PUBACK, mg_mqtt_puback},
      {MG_MQTT_CMD_PUBREC, mg_mqtt_pubrec},
      {MG_MQTT_CMD_PUBREL, mg_mqtt_pubrel},
      {MG_MQTT_CMD_PUBCOMP, mg_mqtt_pubcomp},
      {MG_MQTT_CMD_UNSUBACK, mg_mqtt_unsuback},
  };

  for (i = 0; i < ARRAY_SIZE(cases); i++) {
    struct mg_connection *nc = create_test_connection();
    const double before = mg_time();
    const char *got;
    mg_set_protocol_mqtt(nc);
    msleep(2);

    cases[i].f(nc, 42);

    got = nc->send_mbuf.buf;
    ASSERT_EQ(nc->send_mbuf.len, 4);
    ASSERT_EQ((got[0] & 0xf0), (cases[i].cmd << 4));
    if (cases[i].cmd == MG_MQTT_CMD_PUBREL) {
      ASSERT_EQ(got[0] & 0xf, 2);
    } else {
      ASSERT_EQ(got[0] & 0xf, 0);
    }
    ASSERT_EQ((size_t) got[1], (nc->send_mbuf.len - 2));
    ASSERT_EQ(got[2], 0);
    ASSERT_EQ(got[3], 42);
    ASSERT_GT(((struct mg_mqtt_proto_data *) nc->proto_data)->last_control_time,
              before);

    destroy_test_connection(nc);
  }
  return NULL;
}

static const char *test_mqtt_nullary(void) {
  unsigned long i;
  struct {
    uint8_t cmd;
    void (*f)(struct mg_connection *);
  } cases[] = {
      {MG_MQTT_CMD_PINGREQ, mg_mqtt_ping},
      {MG_MQTT_CMD_PINGRESP, mg_mqtt_pong},
      {MG_MQTT_CMD_DISCONNECT, mg_mqtt_disconnect},
  };

  for (i = 0; i < ARRAY_SIZE(cases); i++) {
    struct mg_connection *nc = create_test_connection();
    const double before = mg_time();
    struct mg_mqtt_message msg;
    const char *got;
    mg_set_protocol_mqtt(nc);
    msleep(2);

    cases[i].f(nc);

    got = nc->send_mbuf.buf;
    ASSERT_EQ(nc->send_mbuf.len, 2);
    ASSERT_EQ((got[0] & 0xf0), (cases[i].cmd << 4));
    ASSERT_EQ((size_t) got[1], (nc->send_mbuf.len - 2));
    ASSERT_EQ(parse_mqtt(&nc->send_mbuf, &msg), (int) nc->send_mbuf.len);
    ASSERT_GT(((struct mg_mqtt_proto_data *) nc->proto_data)->last_control_time,
              before);

    destroy_test_connection(nc);
  }
  return NULL;
}

static const size_t mqtt_long_payload_len = 200;
static const size_t mqtt_very_long_payload_len = 20000;

static void mqtt_eh(struct mg_connection *nc, int ev, void *ev_data) {
  int *check = (int *) nc->user_data;
  struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
  size_t i;
  (void) nc;
  (void) ev_data;

  switch (ev) {
    case MG_EV_MQTT_SUBACK:
      *check = 1;
      break;
    case MG_EV_MQTT_PUBLISH:
      *check = 0;
      if (mg_vcmp(&mm->topic, "/topic")) {
        *check = -1;
        break;
      }

      for (i = 0; i < mm->payload.len; i++) {
        if (nc->recv_mbuf.buf[10 + i] != 'A') {
          *check = -1;
          break;
        }
      }

      if (mm->payload.len == mqtt_long_payload_len) {
        *check = 2;
      } else if (mm->payload.len == mqtt_very_long_payload_len) {
        *check = 3;
      }
      break;
    case MG_EV_MQTT_CONNACK:
      *check = 4;
      break;
  }
}

static const char *test_mqtt_parse_mqtt(void) {
  struct mg_connection *nc = create_test_connection();
  char msg[] = {(char) (MG_MQTT_CMD_SUBACK << 4), 2, 1, 1};
  char *long_msg;
  int check = 0;
  int num_bytes = sizeof(msg), rest_len, i;

  nc->user_data = &check;
  nc->handler = mqtt_eh;
  mg_set_protocol_mqtt(nc);

  mbuf_append(&nc->recv_mbuf, msg, num_bytes);
  nc->proto_handler(nc, MG_EV_RECV, &num_bytes);

  ASSERT_EQ(check, 1);
  mbuf_free(&nc->recv_mbuf);

  /* test a payload whose length encodes as two bytes */
  rest_len = 2 + 6 + mqtt_long_payload_len;
  long_msg = (char *) malloc(512);
  long_msg[0] = (char) (MG_MQTT_CMD_PUBLISH << 4);
  long_msg[1] = (rest_len & 0x7f) | 0x80;
  long_msg[2] = rest_len >> 7;
  memcpy(&long_msg[3], "\0\006/topic", 8);
  memset(&long_msg[11], 'A', mqtt_long_payload_len);

  num_bytes = 3 + rest_len;
  mbuf_append(&nc->recv_mbuf, long_msg, num_bytes);

  /* Short read: handler is not run and data is not consumed */
  for (i = 0; i < num_bytes; i++) {
    check = 123;
    nc->recv_mbuf.len = i;
    nc->proto_handler(nc, MG_EV_RECV, &i);
    ASSERT_EQ((int) nc->recv_mbuf.len, i);
    ASSERT_EQ(check, 123);
  }

  nc->recv_mbuf.len = num_bytes;
  nc->proto_handler(nc, MG_EV_RECV, &num_bytes);
  ASSERT_EQ(check, 2);
  mbuf_free(&nc->recv_mbuf);
  free(long_msg);

  /* test a payload whose length encodes as two bytes */
  rest_len = 8 + mqtt_very_long_payload_len;
  long_msg = (char *) malloc(20100);
  long_msg[0] = (char) (MG_MQTT_CMD_PUBLISH << 4);
  long_msg[1] = (rest_len & 127) | 0x80;
  long_msg[2] = ((rest_len >> 7) & 127) | 0x80;
  long_msg[3] = (rest_len >> 14);
  memcpy(&long_msg[4], "\0\006/topic", 8);
  memset(&long_msg[12], 'A', mqtt_very_long_payload_len);

  num_bytes = 4 + rest_len;
  mbuf_append(&nc->recv_mbuf, long_msg, num_bytes);
  nc->proto_handler(nc, MG_EV_RECV, &num_bytes);

  ASSERT_EQ(check, 3);
  mbuf_remove(&nc->recv_mbuf, nc->recv_mbuf.len);

  /* Message length encodings > 4 bytes are not allowed by the standard,
   * connections should be closed immediately. */
  long_msg[0] = (char) (MG_MQTT_CMD_PUBLISH << 4);
  long_msg[1] = 0xff;
  long_msg[2] = 0xff;
  long_msg[3] = 0xff;
  long_msg[4] = 0xff;
  long_msg[5] = 0xff;
  mbuf_append(&nc->recv_mbuf, long_msg, 10);
  nc->proto_handler(nc, MG_EV_RECV, &num_bytes);
  ASSERT(nc->flags & MG_F_CLOSE_IMMEDIATELY);

  mbuf_free(&nc->recv_mbuf);
  free(long_msg);

  /* test encoding a large payload */
  long_msg = (char *) malloc(mqtt_very_long_payload_len);
  memset(long_msg, 'A', mqtt_very_long_payload_len);
  mg_mqtt_publish(nc, "/topic", 0, 0, long_msg, mqtt_very_long_payload_len);
  nc->recv_mbuf = nc->send_mbuf;
  mbuf_init(&nc->send_mbuf, 0);
  num_bytes = nc->recv_mbuf.len;
  nc->proto_handler(nc, MG_EV_RECV, &num_bytes);

  ASSERT_EQ(check, 3);
  mbuf_free(&nc->recv_mbuf);
  free(long_msg);

  /* test connack parsing */
  mg_mqtt_connack(nc, 0);
  nc->recv_mbuf = nc->send_mbuf;
  mbuf_init(&nc->send_mbuf, 0);
  num_bytes = 4;
  nc->proto_handler(nc, MG_EV_RECV, &num_bytes);

  ASSERT_EQ(check, 4);
  mbuf_free(&nc->recv_mbuf);

  destroy_test_connection(nc);
  return NULL;
}

static const char *test_mqtt_parse_mqtt_qos1(void) {
  /* clang-format off */
  struct mg_mqtt_message mm;
  char msg_qos1[] = {
      ((MG_MQTT_CMD_PUBLISH << 4) | (1 << 1)),
      2 + 6 + 2 + 7,
      0, 6, '/', 't', 'o', 'p', 'i', 'c',
      0x12, 0x34,
      'p', 'a', 'y', 'l', 'o', 'a', 'd',
  };
  /* clang-format on */
  struct mbuf mb;
  memset(&mm, 0, sizeof(mm));
  mbuf_init(&mb, 0);
  mbuf_append(&mb, msg_qos1, sizeof(msg_qos1));
  ASSERT_EQ(parse_mqtt(&mb, &mm), (int) sizeof(msg_qos1));
  ASSERT_EQ(mm.cmd, MG_MQTT_CMD_PUBLISH);
  ASSERT_EQ(mm.qos, 1);
  ASSERT_EQ(mm.message_id, 0x1234);
  ASSERT_EQ(mm.topic.len, 6);
  ASSERT_STREQ_NZ(mm.topic.p, "/topic");
  ASSERT_EQ(mm.payload.len, 7);
  ASSERT_STREQ_NZ(mm.payload.p, "payload");
  mbuf_free(&mb);
  return NULL;
}

static const char *test_mqtt_match_topic_expression(void) {
  ASSERT(mg_mqtt_vmatch_topic_expression("foo", mg_mk_str("foo")));
  ASSERT(mg_mqtt_vmatch_topic_expression("/foo", mg_mk_str("/foo")));
  ASSERT(mg_mqtt_vmatch_topic_expression("+/foo", mg_mk_str("/foo")));
  ASSERT(!mg_mqtt_vmatch_topic_expression("foo", mg_mk_str("foobar")));
  ASSERT(mg_mqtt_vmatch_topic_expression("foo", mg_mk_str("foo/")));
  ASSERT(mg_mqtt_vmatch_topic_expression("foo", mg_mk_str("foo//")));
  ASSERT(!mg_mqtt_vmatch_topic_expression("foo", mg_mk_str("foo/bar")));
  ASSERT(!mg_mqtt_vmatch_topic_expression("foo", mg_mk_str("foo/+")));
  ASSERT(mg_mqtt_vmatch_topic_expression("foo/bar", mg_mk_str("foo/bar")));
  ASSERT(mg_mqtt_vmatch_topic_expression("foo/+", mg_mk_str("foo/bar")));
  ASSERT(mg_mqtt_vmatch_topic_expression("+/bar", mg_mk_str("foo/bar")));
  ASSERT(mg_mqtt_vmatch_topic_expression("+/+", mg_mk_str("foo/bar")));
  ASSERT(mg_mqtt_vmatch_topic_expression("foo/+/bar", mg_mk_str("foo//bar")));
  ASSERT(!mg_mqtt_vmatch_topic_expression("foo/+/+", mg_mk_str("foo/bar")));
  ASSERT(mg_mqtt_vmatch_topic_expression("foo/+/#", mg_mk_str("foo/bar")));
  ASSERT(mg_mqtt_vmatch_topic_expression("+/foo/bar", mg_mk_str("/foo/bar")));

  ASSERT(!mg_mqtt_vmatch_topic_expression("", mg_mk_str("")));
  ASSERT(mg_mqtt_vmatch_topic_expression("/", mg_mk_str("")));
  ASSERT(mg_mqtt_vmatch_topic_expression("/", mg_mk_str("/")));

  ASSERT(mg_mqtt_vmatch_topic_expression("#", mg_mk_str("")));
  ASSERT(mg_mqtt_vmatch_topic_expression("#", mg_mk_str("foo")));
  ASSERT(mg_mqtt_vmatch_topic_expression("#", mg_mk_str("foo/bar")));
  ASSERT(mg_mqtt_vmatch_topic_expression("foo/#", mg_mk_str("foo")));
  ASSERT(mg_mqtt_vmatch_topic_expression("foo/#", mg_mk_str("foo/")));
  ASSERT(mg_mqtt_vmatch_topic_expression("foo/#", mg_mk_str("foo/bar")));
  ASSERT(mg_mqtt_vmatch_topic_expression("foo/#", mg_mk_str("foo/bar/baz")));
  ASSERT(!mg_mqtt_vmatch_topic_expression("#/foo", mg_mk_str("foo")));
  ASSERT(!mg_mqtt_vmatch_topic_expression("#/foo", mg_mk_str("bar/foo")));

  return NULL;
}

#if MG_ENABLE_MQTT_BROKER
struct mg_mqtt_topic_expression brk_test_te[] = {{"/dummy", 0}, {"/unit/#", 0}};

static void brk_cln_cb1(struct mg_connection *nc, int ev, void *p) {
  struct mg_mqtt_message *msg = (struct mg_mqtt_message *) p;

  switch (ev) {
    case MG_EV_CONNECT:
      mg_set_protocol_mqtt(nc);
      mg_send_mqtt_handshake(nc, "dummy");
      break;
    case MG_EV_MQTT_CONNACK:
      mg_mqtt_subscribe(nc, brk_test_te, ARRAY_SIZE(brk_test_te), 42);
      break;
    case MG_EV_MQTT_SUBACK:
      mg_mqtt_publish(nc, "/unit/test", 0, MG_MQTT_QOS(0), "payload", 7);
      break;
    case MG_EV_MQTT_PUBLISH:
      if (mg_vcmp(&msg->topic, "/unit/test") == 0 && msg->payload.len == 7 &&
          mg_vcmp(&msg->payload, "payload") == 0) {
        *(int *) nc->user_data = 1;
      }
      break;
  }
}

static void mqtt_dummy_eh(struct mg_connection *nc, int ev, void *ev_data) {
  (void) nc;
  (void) ev;
  (void) ev_data;
}

static const char *test_mqtt_client_keep_alive(void) {
  struct mg_send_mqtt_handshake_opts opts;
  struct mg_connection *nc = create_test_connection();
  struct mg_mqtt_proto_data *pd;
  double before = mg_time();
  const char *got;
  mg_set_protocol_mqtt(nc);
  pd = (struct mg_mqtt_proto_data *) nc->proto_data;
  nc->handler = mqtt_dummy_eh;

  memset(&opts, 0, sizeof(opts));
  before = mg_time();
  msleep(2);
  opts.keep_alive = 2;
  mg_send_mqtt_handshake_opt(nc, "testclient", opts);
  ASSERT_EQ(pd->keep_alive, 2);
  ASSERT_GT(pd->last_control_time, before);
  mbuf_remove(&nc->send_mbuf, nc->send_mbuf.len); /* Remove the CONNECT. */

  before = pd->last_control_time;
  msleep(2);
  nc->proto_handler(nc, MG_EV_POLL, NULL);
  ASSERT_EQ(nc->send_mbuf.len, 0);
  ASSERT_EQ(pd->last_control_time, before);
  sleep(1);
  nc->proto_handler(nc, MG_EV_POLL, NULL);
  ASSERT_EQ(nc->send_mbuf.len, 0); /* Not yet */
  ASSERT_EQ(pd->last_control_time, before);
  sleep(1);
  msleep(2);
  nc->proto_handler(nc, MG_EV_POLL, NULL);
  ASSERT_GT(nc->send_mbuf.len, 0); /* PINGREQ sent */
  got = nc->send_mbuf.buf;
  ASSERT_EQ(nc->send_mbuf.len, 2);
  ASSERT_EQ((got[0] & 0xf0), (MG_MQTT_CMD_PINGREQ << 4));
  ASSERT_GT(pd->last_control_time, before); /* Resets itself */

  destroy_test_connection(nc);
  return NULL;
}

static const char *test_mqtt_broker(void) {
  struct mg_mgr mgr;
  struct mg_mqtt_broker brk;
  struct mg_connection *brk_nc;
  struct mg_connection *cln_nc;
  const char *brk_local_addr = "127.0.0.1:7777";
  int brk_data = 0, cln_data = 0;

  mg_mgr_init(&mgr, NULL);
  mg_mqtt_broker_init(&brk, &brk_data);

  ASSERT((brk_nc = mg_bind(&mgr, brk_local_addr, mg_mqtt_broker)) != NULL);
  brk_nc->priv_2 = &brk;

  ASSERT((cln_nc = mg_connect(&mgr, brk_local_addr, brk_cln_cb1)) != NULL);
  cln_nc->user_data = &cln_data;

  /* Run event loop. Use more cycles to let client and broker communicate. */
  poll_until(&mgr, 1, c_int_eq, &cln_data, (void *) 1);

  ASSERT_EQ(cln_data, 1);

  mg_mgr_free(&mgr);

  return NULL;
}
#endif /* MG_ENABLE_MQTT_BROKER */

static void cb5(struct mg_connection *nc, int ev, void *ev_data) {
  switch (ev) {
    case MG_EV_CONNECT:
      sprintf((char *) nc->user_data, "%d", *(int *) ev_data);
      break;
    default:
      break;
  }
}

static const char *test_connect_fail(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  char buf[100] = "0";

  mg_mgr_init(&mgr, NULL);
  ASSERT((nc = mg_connect(&mgr, "127.0.0.1:33211", cb5)) != NULL);
  nc->user_data = buf;
  poll_until(&mgr, 1, c_str_ne, buf, (void *) "0");
  mg_mgr_free(&mgr);

/* printf("failed connect status: [%s]\n", buf); */
/* TODO(lsm): fix this for Win32 */
#ifndef _WIN32
  ASSERT(strcmp(buf, "0") != 0);
#endif

  return NULL;
}

static void cb6(struct mg_connection *nc, int ev, void *ev_data) {
  (void) ev;
  (void) ev_data;
  nc->flags |= MG_F_USER_4;
  nc->flags |= MG_F_WANT_READ; /* Should not be allowed. */
}

static const char *test_connect_opts(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  struct mg_connect_opts opts;

  memset(&opts, 0, sizeof(opts));
  opts.user_data = (void *) 0xdeadbeef;
  opts.flags = MG_F_USER_6;
  opts.flags |= MG_F_WANT_READ; /* Should not be allowed. */

  mg_mgr_init(&mgr, NULL);
  ASSERT((nc = mg_connect_opt(&mgr, "127.0.0.1:33211", cb6, opts)) != NULL);
  ASSERT(nc->user_data == (void *) 0xdeadbeef);
  ASSERT(nc->flags & MG_F_USER_6);
  ASSERT(!(nc->flags & MG_F_WANT_READ));
  /* TODO(rojer): find a way to test this w/o touching nc (already freed).
    poll_mgr(&mgr, 25);
    ASSERT(nc->flags & MG_F_USER_4);
    ASSERT(nc->flags & MG_F_USER_6);
    ASSERT(!(nc->flags & MG_F_WANT_READ));
  */
  mg_mgr_free(&mgr);
  return NULL;
}

static const char *test_connect_opts_error_string(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  struct mg_connect_opts opts;
  const char *error_string = NULL;

  memset(&opts, 0, sizeof(opts));
  opts.error_string = &error_string;

  mg_mgr_init(&mgr, NULL);
  ASSERT((nc = mg_connect_opt(&mgr, "127.0.0.1:65537", cb6, opts)) == NULL);
  ASSERT(error_string != NULL);
  ASSERT_STREQ(error_string, "cannot parse address");
  mg_mgr_free(&mgr);
  return NULL;
}

#ifndef NO_DNS_TEST
static const char *test_resolve(void) {
  char buf[20];

  ASSERT(mg_resolve("localhost", buf, sizeof(buf)) > 0);
  ASSERT_STREQ(buf, "127.0.0.1");

  ASSERT_EQ(mg_resolve("please_dont_name_a_host_like_ths", buf, sizeof(buf)),
            0);
  return NULL;
}
#endif

static const char *test_base64(void) {
  const char *cases[] = {"test", "longer string"};
  unsigned long i;
  char enc[8192];
  char dec[8192];

  for (i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
    mg_base64_encode((unsigned char *) cases[i], strlen(cases[i]), enc);
    mg_base64_decode((unsigned char *) enc, strlen(enc), dec);

    ASSERT_EQ(strcmp(cases[i], dec), 0);
  }

  ASSERT_EQ(mg_base64_decode((unsigned char *) "кю", 4, dec), 0);
  ASSERT_EQ(mg_base64_decode((unsigned char *) "AAAA----", 8, dec), 4);
  ASSERT_EQ(mg_base64_decode((unsigned char *) "Q2VzYW50YQ==", 12, dec), 12);
  ASSERT_STREQ(dec, "Cesanta");

  return NULL;
}

static const char *test_sock_addr_to_str(void) {
  char buf[60];
  buf[0] = '\0';
  {
    union socket_address a4;
    memset(&a4, 0, sizeof(a4));
    a4.sa.sa_family = AF_INET;
    a4.sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    a4.sin.sin_port = htons(12345);
    mg_sock_addr_to_str(&a4, buf, sizeof(buf), 0);
    ASSERT_STREQ(buf, "");
    mg_sock_addr_to_str(&a4, buf, sizeof(buf), MG_SOCK_STRINGIFY_IP);
    ASSERT_STREQ(buf, "127.0.0.1");
    mg_sock_addr_to_str(&a4, buf, sizeof(buf), MG_SOCK_STRINGIFY_PORT);
    ASSERT_STREQ(buf, "12345");
    mg_sock_addr_to_str(&a4, buf, sizeof(buf),
                        MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
    ASSERT_STREQ(buf, "127.0.0.1:12345");
  }
#if MG_ENABLE_IPV6 && !defined(_WIN32)
  {
    union socket_address a6;
    memset(&a6, 0, sizeof(a6));
    a6.sa.sa_family = AF_INET6;
    ASSERT_EQ(inet_pton(AF_INET6, "2001::123", &a6.sin6.sin6_addr), 1);
    a6.sin6.sin6_port = htons(12345);
    mg_sock_addr_to_str(&a6, buf, sizeof(buf), 0);
    ASSERT_STREQ(buf, "");
    mg_sock_addr_to_str(&a6, buf, sizeof(buf), MG_SOCK_STRINGIFY_IP);
    ASSERT_STREQ(buf, "2001::123");
    mg_sock_addr_to_str(&a6, buf, sizeof(buf), MG_SOCK_STRINGIFY_PORT);
    ASSERT_STREQ(buf, "12345");
    mg_sock_addr_to_str(&a6, buf, sizeof(buf),
                        MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
    ASSERT_STREQ(buf, "[2001::123]:12345");
  }
#endif
  return NULL;
}

static const char *test_hexdump(void) {
  const char *src = "\1\2\3\4abcd";
  char got[256];

  const char *want =
      "0000  01 02 03 04 61 62 63 64"
      "                          ....abcd\n";
  ASSERT_EQ(mg_hexdump(src, strlen(src), got, sizeof(got)), (int) strlen(want));
  ASSERT_STREQ(got, want);
  return NULL;
}

static const char *test_hexdump_file(void) {
  const char *path = "test_hexdump";
  char *data, *got;
  size_t size;
  FILE *fp;
  struct mg_connection *nc = create_test_connection();

  /* "In the GNU system, non-null pointers are printed as unsigned integers,
   * as if a `%#x' conversion were used. Null pointers print as `(nil)'.
   * (Pointers might print differently in other systems.)"
   * indeed it prints 0x0 on apple.
   */
  nc->user_data = (void *) 0xbeef;

  /* truncate file. open+O_TRUNC works on wine but not on real windows */
  fp = fopen(path, "w");
  ASSERT(fp != NULL);
  fclose(fp);

  mbuf_append(&nc->send_mbuf, "foo", 3);
  mbuf_append(&nc->recv_mbuf, "bar", 3);
  mg_hexdump_connection(nc, path, nc->send_mbuf.buf, 3, MG_EV_SEND);

  mbuf_free(&nc->send_mbuf);
  mbuf_free(&nc->recv_mbuf);
  free(nc);

  ASSERT((data = read_file(path, &size)) != NULL);
  unlink(path);

  got = data;
  while (got - data < (int) size && *got++ != ' ')
    ;
  size -= got - data;

  ASSERT(strstr(got,
                "0000  66 6f 6f                "
                "                         foo") != NULL);

  free(data);
  return NULL;
}

static const char *test_basic_auth_helpers() {
  struct mbuf buf;
  mbuf_init(&buf, 0);
  mg_basic_auth_header(mg_mk_str("foo"), mg_mk_str("bar"), &buf);
  ASSERT_EQ(buf.len, 35);
  ASSERT_EQ(memcmp(buf.buf, "Authorization: Basic Zm9vOmJhcg==\r\n", 35), 0);

  buf.len = 0;
  mg_basic_auth_header(mg_mk_str("foo:bar"), mg_mk_str_n(NULL, 0), &buf);
  ASSERT_EQ(buf.len, 35);
  ASSERT_EQ(memcmp(buf.buf, "Authorization: Basic Zm9vOmJhcg==\r\n", 35), 0);

  mbuf_free(&buf);

  {
    char user[256];
    char pass[256];
    size_t i;

    struct {
      const char *hdr;
      const char *user;
      const char *pass;
      int res;
    } cases[] = {
        {"Basic Zm9vOmJhcg==", "foo", "bar", 0},         /* foo:bar */
        {"Basic Zm9v", "foo", "", 0},                    /* foo */
        {"Basic Zm9vOmJhcjpiYXo=", "foo", "bar:baz", 0}, /* foo:bar:baz */
        {"Basic Zm9vOg==", "foo", "", 0},                /* foo: */
        {"Basic OmJhcg==", "", "", -1},                  /* :bar */
    };

    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
      struct mg_str hdr = mg_mk_str(cases[i].hdr);
      int res;

      memset(user, 0, sizeof(user));
      memset(pass, 0, sizeof(pass));
      res = mg_parse_http_basic_auth(&hdr, user, sizeof(user), pass,
                                     sizeof(pass));
      ASSERT_EQ(res, cases[i].res);
      ASSERT_EQ(strcmp(user, cases[i].user), 0);
      ASSERT_EQ(strcmp(pass, cases[i].pass), 0);
    }
  }
  return NULL;
}

static void test_auth_server_handler(struct mg_connection *nc, int ev,
                                     void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;
  struct mg_str *hdr;
  char user[256] = "";
  char pass[256] = "";
  int res;

  switch (ev) {
    case MG_EV_WEBSOCKET_HANDSHAKE_REQUEST:
    case MG_EV_HTTP_REQUEST: {
      hdr = mg_get_http_header(hm, "Sec-WebSocket-Protocol");
      if (hdr != NULL) {
        if (mg_vcmp(hdr, "myproto") == 0) {
          *(int *) nc->user_data = 2;
          break;
        }
      }

      res = mg_get_http_basic_auth(hm, user, sizeof(user), pass, sizeof(pass));
      if (res == -1) {
        *(int *) nc->user_data = -1;
        break;
      }

      if (strcmp(user, "foo") == 0 && strcmp(pass, "bar") == 0) {
        *(int *) nc->user_data = 1;
      } else {
        *(int *) nc->user_data = -2;
      }

      break;
    }
  }
}

static void test_auth_client_handler(struct mg_connection *nc, int ev,
                                     void *ev_data) {
  (void) nc;
  (void) ev;
  (void) ev_data;
}

static const char *test_http_auth() {
  int connected;
  struct mg_connection *nc;
  struct mg_mgr mgr;
  mg_mgr_init(&mgr, NULL);

  nc = mg_bind(&mgr, "127.0.0.1:1234", test_auth_server_handler);
  mg_set_protocol_http_websocket(nc);
  nc->user_data = &connected;

  connected = 0;
  mg_connect_http(&mgr, test_auth_client_handler,
                  "http://foo:bar@127.0.0.1:1234", NULL, NULL);
  poll_until(&mgr, 4, c_int_eq, &connected, (void *) 1);
  ASSERT_EQ(connected, 1);

  connected = 0;
  mg_connect_ws(&mgr, test_auth_client_handler, "ws://foo:bar@127.0.0.1:1234",
                NULL, NULL);
  poll_until(&mgr, 4, c_int_eq, &connected, (void *) 1);
  ASSERT_EQ(connected, 1);

  /* check that we didn't break proto or other headers */
  connected = 0;
  mg_connect_ws(&mgr, test_auth_client_handler, "ws://127.0.0.1:1234",
                "myproto", NULL);
  poll_until(&mgr, 4, c_int_eq, &connected, (void *) 2);
  ASSERT_EQ(connected, 2);

  mg_mgr_free(&mgr);
  return NULL;
}

void tunnel_server_test_handler(struct mg_connection *nc, int ev,
                                void *ev_data) {
  (void) ev_data;
  switch (ev) {
    case MG_EV_HTTP_REQUEST:
      if (nc->user_data) break;

      nc->user_data = (void *) malloc(sizeof(int));
      (*(int *) nc->user_data) = 0;
      mg_set_timer(nc, mg_time() + 0.01);

      mg_printf(nc, "%s",
                "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
      break;
    case MG_EV_TIMER:
      mg_printf_http_chunk(nc, "%d", (*(int *) nc->user_data)++);
      if (*(int *) nc->user_data == 10) {
        mg_send_http_chunk(nc, "", 0);
        nc->flags |= MG_F_SEND_AND_CLOSE;
      } else {
        mg_set_timer(nc, mg_time() + 0.01);
      }
      break;
    case MG_EV_CLOSE:
      free(nc->user_data);
    default:
      break;
  }
}

void tunnel_client_test_handler(struct mg_connection *nc, int ev,
                                void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;
  (void) nc;
  (void) ev_data;
  switch (ev) {
    case MG_EV_HTTP_REPLY:
      if (hm->body.len == 10 && strcmp(hm->body.p, "0123456789") == 0) {
        *(int *) nc->user_data = 1;
      } else {
        fprintf(stderr, "want: 10, got:%d \"%.*s\"\n", (int) hm->body.len,
                (int) hm->body.len, hm->body.p);
      }
      break;
    default:
      break;
  }
}

static const char *test_http_chunk(void) {
  struct mg_connection nc;
  init_test_connection(&nc);

  mg_printf_http_chunk(&nc, "%d %s", 123, ":-)");
  ASSERT_EQ(nc.send_mbuf.len, 12);
  ASSERT_EQ(memcmp(nc.send_mbuf.buf, "7\r\n123 :-)\r\n", 12), 0);
  mbuf_free(&nc.send_mbuf);

  mg_send_http_chunk(&nc, "", 0);
  ASSERT_EQ(nc.send_mbuf.len, 5);
  ASSERT_EQ(memcmp(nc.send_mbuf.buf, "0\r\n\r\n", 3), 0);
  mbuf_free(&nc.send_mbuf);

  return NULL;
}

static int s_handle_chunk_event = 0;
static char s_events[100];

static void eh_chunk2(struct mg_connection *nc, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_CHUNK) {
    if (s_handle_chunk_event) {
      nc->flags |= MG_F_DELETE_CHUNK;
    }
  }
  snprintf(s_events + strlen(s_events), sizeof(s_events) - strlen(s_events),
           "_%d", ev);
  (void) ev_data;
}

static const char *test_http_extra_headers(void) {
  char buf[FETCH_BUF_SIZE];
  const char *expect = "MyHdr: my_val\r\n";
  s_http_server_opts.extra_headers = "MyHdr: my_val";
  fetch_http(buf, "%s", "GET / HTTP/1.0\r\n\r\n");
  ASSERT(strstr(buf, expect) != NULL);
  s_http_server_opts.extra_headers = NULL;
  return NULL;
}

static const char *test_http_not_modified(void) {
  struct http_message hm;
  cs_stat_t st;
  const char *req1 =
      "GET / HTTP/1.0\nIf-None-Match: \"0.123\"\n"
      "If-Modified-Since: Tue 17 Nov 2015 21:01:56 GMT\n\n";
  const char *req4 =
      "GET / HTTP/1.0\nIf-None-Match: \"0.7\"\n"
      "If-Modified-Since: Tue 17 Nov 2015 21:01:56 GMT\n\n";
  const char *req2 =
      "GET / HTTP/1.0\n"
      "If-Modified-Since: Tue 17 Nov 2015 21:01:56 GMT\n\n";
  const char *req3 = "GET / HTTP/1.0\n\n";

  st.st_size = 7;
  st.st_mtime = (time_t) 0;

  /* Not modified according to If-Modified-Since, but modified by Etag. */
  ASSERT(mg_parse_http(req1, strlen(req1), &hm, 1) > 0);
  ASSERT(mg_is_not_modified(&hm, &st) == 0);

  /* Not modified according to If-Modified-Since, and no Etag. */
  ASSERT(mg_parse_http(req2, strlen(req2), &hm, 1) > 0);
  ASSERT(mg_is_not_modified(&hm, &st) != 0);

  ASSERT(mg_parse_http(req3, strlen(req3), &hm, 1) > 0);
  ASSERT(mg_is_not_modified(&hm, &st) == 0);

  ASSERT(mg_parse_http(req4, strlen(req4), &hm, 1) > 0);
  ASSERT(mg_is_not_modified(&hm, &st) != 0);

  return NULL;
}

static const char *test_http_chunk2(void) {
  struct mg_connection nc;
  struct http_message hm;
  char buf[100] = "5\r\nhe";
  struct mg_mgr mgr;
  mg_mgr_init(&mgr, NULL);

  memset(&nc, 0, sizeof(nc));
  memset(&hm, 0, sizeof(hm));
  nc.mgr = &mgr;
  nc.sock = INVALID_SOCKET;
  nc.handler = eh_chunk2;
  mg_http_create_proto_data(&nc);
  hm.message.len = hm.body.len = ~0;

  s_handle_chunk_event = 0;
  ASSERT_EQ(mg_handle_chunked(&nc, &hm, buf, strlen(buf)), 0);

  /* Simulate arrival of chunks. MG_EV_HTTP_CHUNK events are not handled. */
  strcat(buf, "llo\r");
  ASSERT_EQ(mg_handle_chunked(&nc, &hm, buf, strlen(buf)), 0);
  ASSERT_STREQ(buf, "5\r\nhello\r");

  strcat(buf, "\n");
  ASSERT_EQ(mg_handle_chunked(&nc, &hm, buf, strlen(buf)), 5);
  ASSERT_STREQ(buf, "hello");

  s_handle_chunk_event = 1;
  strcat(buf, "3\r\n:-)\r\n");
  ASSERT_EQ(mg_handle_chunked(&nc, &hm, buf, strlen(buf)), 8);
  ASSERT_STREQ(buf, "");

  s_handle_chunk_event = 0;
  strcat(buf, "3\r\n...\r\na\r\n0123456789\r\n0\r");
  ASSERT_EQ(mg_handle_chunked(&nc, &hm, buf, strlen(buf)), 13);
  ASSERT_STREQ(buf, "...01234567890\r");
  ASSERT_EQ64(hm.message.len, (size_t) ~0);

  strcat(buf, "\n\r\n");
  ASSERT_EQ(mg_handle_chunked(&nc, &hm, buf, strlen(buf)), 13);
  ASSERT_STREQ(buf, "...0123456789");
  ASSERT_EQ(hm.message.len, 13);

  ASSERT_STREQ(s_events, "_102_102_102_102");

  ASSERT(nc.proto_data != NULL);
  nc.proto_data_destructor(nc.proto_data);

  mg_mgr_free(&mgr);

  return NULL;
}

static const char *test_parse_date_string(void) {
  ASSERT_EQ(mg_parse_date_string("Sat, 31 Oct 2019 10:51:12 GMT"), 1572519072);
  ASSERT_EQ(mg_parse_date_string("31/Oct/2019 10:51:12 GMT"), 1572519072);
  ASSERT_EQ(mg_parse_date_string("31 Oct 2019 10:51:12 GMT"), 1572519072);
  ASSERT_EQ(mg_parse_date_string("31-Oct-2019 10:51:12 GMT"), 1572519072);
  return NULL;
}

#if MG_ENABLE_HTTP_STREAMING_MULTIPART
struct cb_mp_srv_data {
  struct mbuf res;
  int request_end;
};

static void cb_mp_srv(struct mg_connection *nc, int ev, void *p) {
  struct cb_mp_srv_data *mpd = (struct cb_mp_srv_data *) nc->user_data;
  struct mbuf *data = &mpd->res;
  if (nc->listener == NULL) return;

  switch (ev) {
    case MG_EV_HTTP_MULTIPART_REQUEST: {
      struct http_message *hm = (struct http_message *) p;
      mbuf_append(data, "<MPRQ", 5);
      mbuf_append(data, hm->uri.p, hm->uri.len);
      break;
    }
    case MG_EV_HTTP_PART_BEGIN: {
      struct mg_http_multipart_part *mp = (struct mg_http_multipart_part *) p;
      if (mp->status < 0) {
        mbuf_append(data, "ERROR", 5);
      } else {
        DBG(("BEGIN %s %s", mp->var_name, mp->file_name));
        mbuf_append(data, mp->var_name, strlen(mp->var_name));
        mbuf_append(data, mp->file_name, strlen(mp->file_name));
      }
      break;
    }
    case MG_EV_HTTP_PART_DATA: {
      struct mg_http_multipart_part *mp = (struct mg_http_multipart_part *) p;
      if (mp->status < 0) {
        mbuf_append(data, "ERROR", 5);
      } else {
        DBG(("DATA %d", (int) mp->data.len));
        mbuf_append(data, mp->data.p, mp->data.len);
      }
      break;
    }
    case MG_EV_HTTP_PART_END: {
      struct mg_http_multipart_part *mp = (struct mg_http_multipart_part *) p;
      struct mbuf *data = (struct mbuf *) nc->listener->user_data;
      mbuf_append(data, mp->var_name, strlen(mp->var_name));
      mbuf_append(data, mp->file_name, strlen(mp->file_name));
      if (mp->status < 0) {
        DBG(("PARTERR %s %s", mp->var_name, mp->file_name));
        mbuf_append(data, "ERROR", 5);
      } else {
        DBG(("END %s %s", mp->var_name, mp->file_name));
        mbuf_append(data, "FIN", 3);
      }
      break;
    }
    case MG_EV_HTTP_MULTIPART_REQUEST_END: {
      struct mg_http_multipart_part *mp = (struct mg_http_multipart_part *) p;
      DBG(("RQEND %p %p", (void *) mp->var_name, (void *) mp->file_name));
      mbuf_append(data, (mp->status < 0 ? "-" : "+"), 1);
      mbuf_append(data, (mp->var_name == NULL ? "1" : "0"), 1);
      mbuf_append(data, (mp->file_name == NULL ? "1" : "0"), 1);
      mbuf_append(data, "MPRQ>", 5);
      mpd->request_end = 1;
      break;
    }
  }
}

static void cb_mp_send_one_byte(struct mg_connection *nc, int ev, void *p) {
  static int i = -1;
  (void) p;
  if (ev == MG_EV_CONNECT) {
    i = 0;
  } else if (i >= 0 && ev == MG_EV_POLL) {
    char ch = ((char *) nc->user_data)[i++];
    int l = strlen((char *) nc->user_data);
    if (ch != '\0') {
      mg_send(nc, &ch, 1);
      DBG(("%p sent %d of %d", (void *) nc, i, l));
    } else {
      nc->flags |= MG_F_SEND_AND_CLOSE;
      i = -1;
    }
  }
}

static const char b1[] =
    "111111111111111111111111111111111111111111111111111111111111111\r\n"
    "111111111111111111111111111111111111111111111111111111111111111\r\n"
    "111111111111111111111111111111111111111111111111111111111111111\r\n"
    "111111111111111111111111111111111111111111111111111111111111111\r\n"
    "111111111111111111111111111111111111111111111111111111111111111\r\n"
    "111111111111111111111111111111111111111111111111111111111111111\r\n"
    "111111111111111111111111111111111111111111111111111111111111111\r\n"
    "111111111111111111111111111111111111111111111111111111111111111\r\n"
    "111111111111111111111111111111111111111111111111111111111111111\r\n"
    "111111111111111111111111111111111111111111111111111111111111111";
static const char b2[] =
    "222222222222222222222222222222222222222222222222222222222222222\r\n"
    "222222222222222222222222222222222222222222222222222222222222222\r\n"
    "222222222222222222222222222222222222222222222222222222222222222\r\n"
    "222222222222222222222222222222222222222222222222222222222222222\r\n"
    "222222222222222222222222222222222222222222222222222222222222222\r\n"
    "222222222222222222222222222222222222222222222222222222222222222\r\n"
    "222222222222222222222222222222222222222222222222222222222222222\r\n"
    "222222222222222222222222222222222222222222222222222222222222222\r\n"
    "222222222222222222222222222222222222222222222222222222222222222\r\n"
    "222222222222222222222222222222222222222222222222222222222222222\r\n"
    "222222222222222222222222222222222222222222222222222222222222222\r\n";
static const char b4[] =
    "444444444444444444444444444444444444444444444444444444444444444\r\n"
    "444444444444444444444444444444444444444444444444444444444444444\r\n"
    "444444444444444444444444444444444444444444444444444444444444444\r\n"
    "444444444444444444444444444444444444444444444444444444444444444\r\n"
    "444444444444444444444444444444444444444444444444444444444444444\r\n"
    "444444444444444444444444444444444444444444444444444444444444444\r\n"
    "444444444444444444444444444444444444444444444444444444444444444\r\n"
    "444444444444444444444444444444444444444444444444444444444444444\r\n"
    "444444444444444444444444444444444444444444444444444444444444444\r\n"
    "444444444444444444444444444444444444444444444444444444444444444\r\n"
    "444444444444444444444444444444444444444444444444444444444444444";

static const char *test_http_multipart_check_res(struct mbuf *res) {
  const char *ptr = res->buf;
  ASSERT_STREQ_NZ(ptr, "<MPRQ/test");
  ptr += 10;
  ASSERT_STREQ_NZ(ptr, "afoo");
  ptr += 4;
  ASSERT_STREQ_NZ(ptr, b1);
  ptr += sizeof(b1) - 1;
  ASSERT_STREQ_NZ(ptr, "afooFIN");
  ptr += 7;
  /* No file_name for second part */
  ASSERT_STREQ_NZ(ptr, "b");
  ptr++;
  ASSERT_STREQ_NZ(ptr, b2);
  ptr += sizeof(b2) - 1;
  ASSERT_STREQ_NZ(ptr, "bFIN");
  ptr += 4;
  ASSERT_STREQ_NZ(ptr, "cbar");
  ptr += 4;
  ASSERT_STREQ_NZ(ptr, b4);
  ptr += sizeof(b4) - 1;
  ASSERT_STREQ_NZ(ptr, "cbarFIN");
  ptr += 7;
  ASSERT_STREQ_NZ(ptr, "+11MPRQ>");
  ptr += 8;
  ASSERT_EQ((size_t)(ptr - res->buf), res->len);
  return NULL;
}

static const char *test_http_multipart2(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc_listen;

  const char multi_part_req_fmt[] =
      "%s"
      "Content-Disposition: form-data; name=\"a\"; filename=\"foo\"\r\n"
      "\r\n"
      "%s"
      "\r\n--Asrf456BGe4h\r\n"
      "Content-Disposition: form-data; name=\"b\"\r\n"
      "\r\n"
      "%s"
      "\r\n--Asrf456BGe4h\r\n"
      "Content-Disposition: form-data; name=\"c\"; filename=\"bar\"\r\n"
      "\r\n"
      "%s"
      "\r\n--Asrf456BGe4h--\r\n"
      "\r\n";

  char multi_part_req[1024 * 5];
  int i;
  struct mg_connection *c;
  const char *r;

  struct cb_mp_srv_data mpd;
  memset(&mpd, 0, sizeof(mpd));
  mbuf_init(&mpd.res, 0);

  mg_mgr_init(&mgr, NULL);
  nc_listen = mg_bind(&mgr, "8765", cb_mp_srv);
  nc_listen->user_data = &mpd;

  mg_set_protocol_http_websocket(nc_listen);

  snprintf(multi_part_req, sizeof(multi_part_req), multi_part_req_fmt, "", b1,
           b2, b4);

  ASSERT((c = mg_connect_http(&mgr, cb_mp_send_one_byte,
                              "http://127.0.0.1:8765/test",
                              "Content-Type: "
                              "multipart/form-data;boundary=Asrf456BGe4h",
                              "\r\n--Asrf456BGe4h\r\n")) != NULL);

  c->user_data = multi_part_req;

  poll_until(&mgr, 10, c_int_eq, &mpd.request_end, (void *) 1);

  if ((r = test_http_multipart_check_res(&mpd.res)) != NULL) return r;

  mbuf_free(&mpd.res);
  memset(&mpd, 0, sizeof(mpd));
  mbuf_init(&mpd.res, 0);

  snprintf(multi_part_req, sizeof(multi_part_req), multi_part_req_fmt,
           "\r\n--Asrf456BGe4h\r\n", b1, b2, b4);

  /* Testing delivering to endpoint handler*/
  nc_listen = mg_bind(&mgr, "8766", NULL);
  nc_listen->user_data = &mpd;

  mg_set_protocol_http_websocket(nc_listen);
  mg_register_http_endpoint(nc_listen, "/test", cb_mp_srv MG_UD_ARG(NULL));

  ASSERT((c = mg_connect_http(&mgr, NULL, "http://127.0.0.1:8766/test",
                              "Connection: keep-alive\r\nContent-Type: "
                              "multipart/form-data; boundary=Asrf456BGe4h",
                              multi_part_req)) != NULL);

  poll_until(&mgr, 3, c_int_eq, &mpd.request_end, (void *) 1);
  mg_mgr_poll(&mgr, 1); /* Consume epilogue. */

  if ((r = test_http_multipart_check_res(&mpd.res)) != NULL) return r;

  /* Keep the connection alive */

  mbuf_free(&mpd.res);
  memset(&mpd, 0, sizeof(mpd));
  mbuf_init(&mpd.res, 0);

  mg_printf(c,
            "POST /test HTTP/1.1\r\nHost: 127.0.0.1:8766\r\nContent-Length: "
            "%" SIZE_T_FMT
            "\r\nConnection: keep-alive\r\nContent-Type: multipart/form-data; "
            "boundary=Asrf456BGe4h\r\n%s",
            strlen(multi_part_req), multi_part_req);

  poll_until(&mgr, 3, c_int_eq, &mpd.request_end, (void *) 1);

  if ((r = test_http_multipart_check_res(&mpd.res)) != NULL) return r;

  c->flags |= MG_F_CLOSE_IMMEDIATELY;

  mbuf_free(&mpd.res);
  memset(&mpd, 0, sizeof(mpd));
  mbuf_init(&mpd.res, 0);
  mbuf_remove(&c->recv_mbuf, c->recv_mbuf.len);

  /* Test interrupted request */
  multi_part_req[1800] = '\0';
  c = mg_connect_http(
      &mgr, NULL, "http://127.0.0.1:8765",
      "Content-Type: multipart/form-data; boundary=Asrf456BGe4h",
      multi_part_req);

  ASSERT(c != NULL);
  for (i = 0; i < 20; i++) {
    mg_mgr_poll(&mgr, 1);
  }

  c->flags |= MG_F_CLOSE_IMMEDIATELY;

  poll_until(&mgr, 3, c_int_eq, &mpd.request_end, (void *) 1);

  ASSERT_STREQ_NZ(mpd.res.buf + mpd.res.len - 17, "cbarERROR-11MPRQ>");

  mbuf_free(&mpd.res);
  memset(&mpd, 0, sizeof(mpd));
  mbuf_init(&mpd.res, 0);

  ASSERT(
      mg_connect_http(
          &mgr, NULL, "http://127.0.0.1:8766/test",
          "Content-Type: multipart/form-data; boundary=Asrf456BGe4h",
          "\r\n--Asrf456BGe4h\r\n"
          "Content-Disposition: form-data; name=\"d\"; filename=\"small\"\r\n"
          "\r\n"
          "boooo"
          "\r\n--Asrf456BGe4h--\r\n"
          "\r\n") != NULL);

  poll_until(&mgr, 3, c_int_eq, &mpd.request_end, (void *) 1);

  ASSERT(mpd.res.len != 0);
  ASSERT_STREQ_NZ(mpd.res.buf, "<MPRQ/testdsmallboooodsmallFIN+11MPRQ>");
  mbuf_free(&mpd.res);

  /*
   * Test malformed packet
   * See https://github.com/cesanta/dev/issues/6974
   * This request should not lead to crash
   */
  c = mg_connect(&mgr, "127.0.0.1:8766", NULL);
  mg_printf(c,
            "POST /test HTTP/1.1\r\n"
            "Connection: keep-alive\r\n"
            "Content-Type: multipart/form-data; Content-Length: 1\r\n"
            "1\r\n\r\n");
  for (i = 0; i < 20; i++) {
    mg_mgr_poll(&mgr, 1);
  }

  mg_mgr_free(&mgr);

  return NULL;
}

static const char *test_http_multipart_pipeline(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc_listen;

  const char multi_part_req_fmt[] =
      "%s"
      "Content-Disposition: form-data; name=\"a\"; filename=\"foo\"\r\n"
      "\r\n"
      "%s"
      "\r\n--Asrf456BGe4h\r\n"
      "Content-Disposition: form-data; name=\"b\"\r\n"
      "\r\n"
      "%s"
      "\r\n--Asrf456BGe4h\r\n"
      "Content-Disposition: form-data; name=\"c\"; filename=\"bar\"\r\n"
      "\r\n"
      "%s"
      "\r\n--Asrf456BGe4h--\r\n"
      "\r\n";

  char multi_part_req[1024 * 5];
  struct mg_connection *c;
  const char *ptr;
  int i;

  struct cb_mp_srv_data mpd;
  memset(&mpd, 0, sizeof(mpd));
  mbuf_init(&mpd.res, 0);

  mg_mgr_init(&mgr, NULL);
  nc_listen = mg_bind(&mgr, "8765", cb_mp_srv);
  nc_listen->user_data = &mpd;

  mg_set_protocol_http_websocket(nc_listen);

  snprintf(multi_part_req, sizeof(multi_part_req), multi_part_req_fmt,
           "\r\n--Asrf456BGe4h\r\n", b1, b2, b4);

  ASSERT((c = mg_connect_http(&mgr, NULL, "http://127.0.0.1:8765/test",
                              "Content-Type: "
                              "multipart/form-data;boundary=Asrf456BGe4h\r\n"
                              "Connection: keep-alive",
                              multi_part_req)) != NULL);

  c->user_data = multi_part_req;

  mg_printf(c,
            "POST /test HTTP/1.1\r\n"
            "Connection: keep-alive\r\n"
            "Content-Type: multipart/form-data;boundary=Asrf456BGe4h\r\n"
            "Content-Length: %d\r\n%s",
            (int) strlen(multi_part_req), multi_part_req);

  poll_until(&mgr, 10, c_int_eq, &mpd.request_end, (void *) 1);

  ptr = mpd.res.buf;
  for (i = 0; i < 2; i++) {
    ASSERT_STREQ_NZ(ptr, "<MPRQ/test");
    ptr += 10;
    ASSERT_STREQ_NZ(ptr, "afoo");
    ptr += 4;
    ASSERT_STREQ_NZ(ptr, b1);
    ptr += sizeof(b1) - 1;
    ASSERT_STREQ_NZ(ptr, "afooFIN");
    ptr += 7;
    /* No file_name for second part */
    ASSERT_STREQ_NZ(ptr, "b");
    ptr++;
    ASSERT_STREQ_NZ(ptr, b2);
    ptr += sizeof(b2) - 1;
    ASSERT_STREQ_NZ(ptr, "bFIN");
    ptr += 4;
    ASSERT_STREQ_NZ(ptr, "cbar");
    ptr += 4;
    ASSERT_STREQ_NZ(ptr, b4);
    ptr += sizeof(b4) - 1;
    ASSERT_STREQ_NZ(ptr, "cbarFIN");
    ptr += 7;
    ASSERT_STREQ_NZ(ptr, "+11MPRQ>");
    ptr += 8;
  }
  ASSERT_EQ((size_t)(ptr - mpd.res.buf), mpd.res.len);

  mbuf_free(&mpd.res);
  mg_mgr_free(&mgr);

  return NULL;
}

static struct mg_str upload_lfn_same(struct mg_connection *nc,
                                     struct mg_str fn) {
  if (fn.len == 0) {
    fn = mg_strdup(mg_mk_str("bar"));
  }
  (void) nc;
  return fn;
}

static void cb_mp_srv_upload(struct mg_connection *nc, int ev, void *p) {
  mg_file_upload_handler(nc, ev, p, upload_lfn_same);
  if (ev == MG_EV_CLOSE && nc->listener != NULL) {
    *((int *) nc->listener->user_data) = 1;
  }
  (void) p;
}

static const char *test_http_multipart_upload(void) {
  struct mg_mgr mgr;
  const char req_fmt[] =
      "%s"
      "Content-Disposition: form-data; name=\"a\"; filename=\"foo\"\r\n"
      "\r\n"
      "%s"
      "\r\n--Asrf456BGe4h\r\n"
      "Content-Disposition: form-data; name=\"b\"\r\n"
      "\r\n"
      "%s"
      "\r\n--Asrf456BGe4h\r\n"
      "Content-Disposition: form-data; name=\"c\"; filename=\"baz\"\r\n"
      "\r\n"
      "%s"
      "\r\n--Asrf456BGe4h--\r\n"
      "\r\n";

  char req[1024 * 5], *data;
  struct mg_connection *c, *lc;
  size_t size;
  int done = 0;

  mg_mgr_init(&mgr, NULL);
  lc = mg_bind(&mgr, "8766", cb_mp_srv_upload);
  mg_set_protocol_http_websocket(lc);
  lc->user_data = &done;

  (void) remove("foo");
  (void) remove("bar");
  (void) remove("baz");

  snprintf(req, sizeof(req), req_fmt, "", b1, b2, b4);

  ASSERT((c = mg_connect_http(&mgr, cb_mp_send_one_byte,
                              "http://127.0.0.1:8766/test",
                              "Content-Type: "
                              "multipart/form-data;boundary=Asrf456BGe4h",
                              "\r\n--Asrf456BGe4h\r\n")) != NULL);
  c->user_data = req;

  poll_until(&mgr, 5, c_int_eq, &done, (void *) 1);

  data = read_file("foo", &size);
  ASSERT_PTRNE(data, NULL);
  ASSERT_STREQ_NZ(data, b1);
  (void) remove("foo");
  free(data);

  data = read_file("bar", &size);
  ASSERT_PTRNE(data, NULL);
  ASSERT_STREQ_NZ(data, b2);
  (void) remove("bar");
  free(data);

  data = read_file("baz", &size);
  ASSERT_PTRNE(data, NULL);
  ASSERT_STREQ_NZ(data, b4);
  (void) remove("baz");
  free(data);

  mg_mgr_free(&mgr);
  return NULL;
}

#endif /* MG_ENABLE_HTTP_STREAMING_MULTIPART */

static const char *test_http_multipart(void) {
  struct http_message hm;
  char buf[FETCH_BUF_SIZE] = "", var_name[100], file_name[100];
  const char *chunk;
  size_t chunk_len, ofs;

  fetch_http(buf, "%s", "GET /data/multipart.txt HTTP/1.0\r\n\r\n");
  mg_parse_http(buf, strlen(buf), &hm, 1);

  ofs = mg_parse_multipart(hm.body.p, hm.body.len, var_name, sizeof(var_name),
                           file_name, sizeof(file_name), &chunk, &chunk_len);
  ASSERT(ofs < hm.body.len);
  ASSERT(ofs > 0);
  ASSERT_EQ(chunk_len, 10);
  ASSERT_EQ(memcmp(chunk, "file1 data", chunk_len), 0);

  ofs = mg_parse_multipart(hm.body.p + ofs, hm.body.len - ofs, var_name,
                           sizeof(var_name), file_name, sizeof(file_name),
                           &chunk, &chunk_len);
  ASSERT(ofs < hm.body.len);
  ASSERT(ofs > 0);
  ASSERT_EQ(chunk_len, 10);
  ASSERT_EQ(memcmp(chunk, "file2 data", chunk_len), 0);

  ofs = mg_parse_multipart(hm.body.p + ofs, hm.body.len - ofs, var_name,
                           sizeof(var_name), file_name, sizeof(file_name),
                           &chunk, &chunk_len);
  ASSERT(ofs == 0);

  return NULL;
}

static const char *test_dns_encode(void) {
  struct mg_connection nc;
  const char *got;
  int query_types[] = {MG_DNS_A_RECORD, MG_DNS_AAAA_RECORD};
  size_t i;
  init_test_connection(&nc);

  /*
   * Testing TCP encoding since when the connection
   * is in UDP mode the data is not stored in the send buffer.
   */

  for (i = 0; i < ARRAY_SIZE(query_types); i++) {
    mg_send_dns_query(&nc, "www.cesanta.com", query_types[i]);
    got = nc.send_mbuf.buf;
    ASSERT_EQ(nc.send_mbuf.len, 12 + 4 + 13 + 4 + 2);
    ASSERT_EQ(got[14], 3);
    ASSERT_STREQ_NZ(&got[15], "www");
    ASSERT_EQ(got[18], 7);
    ASSERT_STREQ_NZ(&got[19], "cesanta");
    ASSERT_EQ(got[26], 3);
    ASSERT_STREQ_NZ(&got[27], "com");
    ASSERT_EQ(got[30], 0);
    ASSERT_EQ(got[31], 0);
    ASSERT_EQ(got[32], query_types[i]);
    ASSERT_EQ(got[33], 0);
    ASSERT_EQ(got[34], 1);

    mbuf_free(&nc.send_mbuf);
  }
  return NULL;
}

static const char *test_dns_uncompress(void) {
  /*
   * Order or string constants is important. Names being uncompressed
   * must be before the end of the DNS packet end.
   */
  const char *s =
      "\03www\07cesanta\03com\0\03www\xc0\06\05dummy\07cesanta\3com\0";
  struct mg_dns_message msg;
  struct mg_str name = mg_mk_str(s);
  struct mg_str comp_name = mg_mk_str_n(s + 17, 6);
  struct mg_str n;
  char dst[256];
  int len;
  size_t i;

  const char *cases[] = {"www.cesanta.com", "www", "ww", "www.", "www.c"};

  memset(&msg, 0, sizeof(msg));
  msg.pkt.p = s + 23;
  msg.pkt.len = strlen(msg.pkt.p) + 1;

  memset(dst, 'X', sizeof(dst));
  n = mg_mk_str_n(msg.pkt.p, msg.pkt.len);
  len = mg_dns_uncompress_name(&msg, &n, dst, sizeof(dst));
  ASSERT_EQ(len, 17);
  ASSERT_EQ(len, (int) strlen(dst));
  ASSERT_STREQ_NZ(dst, "dummy.cesanta.com");

  for (i = 0; i < ARRAY_SIZE(cases); i++) {
    size_t l = strlen(cases[i]);
    memset(dst, 'X', sizeof(dst));
    len = mg_dns_uncompress_name(&msg, &name, dst, l);
    ASSERT_EQ(len, (int) l);
    ASSERT_EQ(strncmp(dst, cases[i], l), 0);
    ASSERT_EQ(dst[l], 'X');
  }

  /* if dst has enough space, check the trailing '\0' */
  memset(dst, 'X', sizeof(dst));
  len = mg_dns_uncompress_name(&msg, &name, dst, sizeof(dst));
  ASSERT_EQ(len, 15);
  ASSERT_EQ(len, (int) strlen(dst));
  ASSERT_STREQ_NZ(dst, "www.cesanta.com");
  ASSERT_EQ(dst[15], 0);

  /* check compressed name */
  memset(dst, 'X', sizeof(dst));
  len = mg_dns_uncompress_name(&msg, &comp_name, dst, sizeof(dst));
  printf("%d [%s]\n", len, dst);
  ASSERT_EQ(len, 15);
  ASSERT_EQ(len, (int) strlen(dst));
  ASSERT_STREQ_NZ(dst, "www.cesanta.com");
  ASSERT_EQ(dst[15], 0);

  /* check parsing loop avoidance */
  msg.pkt.p = "\05dummy\xc0\x06\0";
  msg.pkt.len = strlen(msg.pkt.p) + 1;
  memset(dst, 'X', sizeof(dst));
  len = mg_dns_uncompress_name(&msg, &comp_name, dst, sizeof(dst));
  ASSERT_EQ(len, 0);

  return NULL;
}

static const char *test_dns_decode(void) {
  struct mg_dns_message msg;
  char name[256];
  const char *hostname = "go.cesanta.com";
  const char *cname = "ghs.googlehosted.com";
  struct mg_dns_resource_record *r;
  uint16_t tiny;
  struct in_addr ina;
  int n;

  /*
   * Response for a record A query host for `go.cesanta.com`.
   * The response contains two answers:
   *
   * CNAME go.cesanta.com -> ghs.googlehosted.com
   * A ghs.googlehosted.com -> 74.125.136.121
   *
   * Captured from a reply generated by Google DNS server (8.8.8.8)
   */
  const unsigned char pkt[] = {
      0xa1, 0x00, 0x81, 0x80, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
      0x02, 0x67, 0x6f, 0x07, 0x63, 0x65, 0x73, 0x61, 0x6e, 0x74, 0x61, 0x03,
      0x63, 0x6f, 0x6d, 0x00, 0x00, 0x01, 0x00, 0x01, 0xc0, 0x0c, 0x00, 0x05,
      0x00, 0x01, 0x00, 0x00, 0x09, 0x52, 0x00, 0x13, 0x03, 0x67, 0x68, 0x73,
      0x0c, 0x67, 0x6f, 0x6f, 0x67, 0x6c, 0x65, 0x68, 0x6f, 0x73, 0x74, 0x65,
      0x64, 0xc0, 0x17, 0xc0, 0x2c, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01,
      0x2b, 0x00, 0x04, 0x4a, 0x7d, 0x88, 0x79};

  ASSERT_EQ(mg_parse_dns((const char *) pkt, sizeof(pkt), &msg), 0);
  ASSERT_EQ(msg.num_questions, 1);
  ASSERT_EQ(msg.num_answers, 2);

  r = &msg.questions[0];
  ASSERT_EQ(mg_dns_uncompress_name(&msg, &r->name, name, sizeof(name)),
            strlen(hostname));
  ASSERT_EQ(strncmp(name, hostname, strlen(hostname)), 0);

  r = &msg.answers[0];
  ASSERT_EQ(mg_dns_uncompress_name(&msg, &r->name, name, sizeof(name)),
            strlen(hostname));
  ASSERT_EQ(strncmp(name, hostname, strlen(hostname)), 0);

  ASSERT_EQ(mg_dns_uncompress_name(&msg, &r->rdata, name, sizeof(name)),
            strlen(cname));
  ASSERT_EQ(strncmp(name, cname, strlen(cname)), 0);

  r = &msg.answers[1];
  ASSERT_EQ(mg_dns_uncompress_name(&msg, &r->name, name, sizeof(name)),
            strlen(cname));
  ASSERT_EQ(strncmp(name, cname, strlen(cname)), 0);
  ASSERT_EQ(mg_dns_parse_record_data(&msg, r, &tiny, sizeof(tiny)), -1);
  ASSERT_EQ(mg_dns_parse_record_data(&msg, r, &ina, sizeof(ina)), 0);
  ASSERT_EQ(ina.s_addr, inet_addr("74.125.136.121"));

  /* Test iteration */
  n = 0;
  r = NULL;
  while ((r = mg_dns_next_record(&msg, MG_DNS_A_RECORD, r))) {
    n++;
  }
  ASSERT_EQ(n, 1);

  n = 0;
  r = NULL;
  while ((r = mg_dns_next_record(&msg, MG_DNS_CNAME_RECORD, r))) {
    n++;
  }
  ASSERT_EQ(n, 1);

  /* Test unknown record type */
  r = mg_dns_next_record(&msg, MG_DNS_A_RECORD, r);
  r->rtype = 0xff;
  ASSERT_EQ(mg_dns_parse_record_data(&msg, r, &ina, sizeof(ina)), -1);

  return NULL;
}

static const char *test_dns_decode_truncated(void) {
  struct mg_dns_message msg;
  char name[256];
  const char *hostname = "go.cesanta.com";
  const char *cname = "ghs.googlehosted.com";
  struct mg_dns_resource_record *r;
  uint16_t tiny;
  struct in_addr ina;
  int n;
  int i;

  const unsigned char src[] = {
      0xa1, 0x00, 0x81, 0x80, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
      0x02, 0x67, 0x6f, 0x07, 0x63, 0x65, 0x73, 0x61, 0x6e, 0x74, 0x61, 0x03,
      0x63, 0x6f, 0x6d, 0x00, 0x00, 0x01, 0x00, 0x01, 0xc0, 0x0c, 0x00, 0x05,
      0x00, 0x01, 0x00, 0x00, 0x09, 0x52, 0x00, 0x13, 0x03, 0x67, 0x68, 0x73,
      0x0c, 0x67, 0x6f, 0x6f, 0x67, 0x6c, 0x65, 0x68, 0x6f, 0x73, 0x74, 0x65,
      0x64, 0xc0, 0x17, 0xc0, 0x2c, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01,
      0x2b, 0x00, 0x04, 0x4a, 0x7d, 0x88, 0x79};
  char *pkt = NULL;

#define WONDER(expr) \
  if (!(expr)) continue

  for (i = sizeof(src) - 1; i > 0; i--) {
    free(pkt);
    pkt = (char *) malloc(i);
    memcpy(pkt, src, i);

    WONDER(mg_parse_dns((const char *) pkt, i, &msg) == 0);
    WONDER(msg.num_questions == 1);
    WONDER(msg.num_answers == 2);

    r = &msg.questions[0];
    WONDER(mg_dns_uncompress_name(&msg, &r->name, name, sizeof(name)) ==
           strlen(hostname));
    WONDER(strncmp(name, hostname, strlen(hostname)) == 0);

    r = &msg.answers[0];
    WONDER(mg_dns_uncompress_name(&msg, &r->name, name, sizeof(name)) ==
           strlen(hostname));
    WONDER(strncmp(name, hostname, strlen(hostname)) == 0);

    WONDER(mg_dns_uncompress_name(&msg, &r->rdata, name, sizeof(name)) ==
           strlen(cname));
    WONDER(strncmp(name, cname, strlen(cname)) == 0);

    r = &msg.answers[1];
    WONDER(mg_dns_uncompress_name(&msg, &r->name, name, sizeof(name)) ==
           strlen(cname));
    WONDER(strncmp(name, cname, strlen(cname)) == 0);
    WONDER(mg_dns_parse_record_data(&msg, r, &tiny, sizeof(tiny)) == -1);
    WONDER(mg_dns_parse_record_data(&msg, r, &ina, sizeof(ina)) == 0);
    WONDER(ina.s_addr == inet_addr("74.125.136.121"));

    /* Test iteration */
    n = 0;
    r = NULL;
    while ((r = mg_dns_next_record(&msg, MG_DNS_A_RECORD, r))) {
      n++;
    }
    WONDER(n == 1);

    n = 0;
    r = NULL;
    while ((r = mg_dns_next_record(&msg, MG_DNS_CNAME_RECORD, r))) {
      n++;
    }
    WONDER(n == 1);

    /* Test unknown record type */
    r = mg_dns_next_record(&msg, MG_DNS_A_RECORD, r);
    WONDER(r != NULL);
    printf("GOT %p\n", (void *) r);
    r->rtype = 0xff;
    WONDER(mg_dns_parse_record_data(&msg, r, &ina, sizeof(ina)) == -1);

    ASSERT("Should have failed" == NULL);
  }
  free(pkt);
  return NULL;
}

static const char *check_www_cesanta_com_reply(const char *pkt, size_t len) {
  char name[256];

  in_addr_t addr = inet_addr("54.194.65.250");
  struct in_addr ina;
  struct mg_dns_message msg;

  memset(&msg, 0, sizeof(msg));
  ASSERT(mg_parse_dns(pkt, len, &msg) != -1);

  memset(name, 0, sizeof(name));
  ASSERT(mg_dns_uncompress_name(&msg, &msg.questions[0].name, name,
                                sizeof(name)) > 0);
  ASSERT_STREQ_NZ(name, "www.cesanta.com");
  memset(name, 0, sizeof(name));
  ASSERT(mg_dns_uncompress_name(&msg, &msg.answers[0].name, name,
                                sizeof(name)) > 0);
  ASSERT_STREQ_NZ(name, "www.cesanta.com");
  ASSERT_EQ(msg.answers[0].rtype, MG_DNS_CNAME_RECORD);
  memset(name, 0, sizeof(name));
  ASSERT(mg_dns_parse_record_data(&msg, &msg.answers[0], name, sizeof(name)) !=
         -1);
  ASSERT_STREQ_NZ(name, "cesanta.com");
  memset(name, 0, sizeof(name));
  ASSERT(mg_dns_uncompress_name(&msg, &msg.answers[1].name, name,
                                sizeof(name)) > 0);
  ASSERT_STREQ_NZ(name, "cesanta.com");

  ASSERT_EQ(msg.answers[1].rtype, MG_DNS_A_RECORD);
  ASSERT(mg_dns_parse_record_data(&msg, &msg.answers[1], &ina, sizeof(ina)) !=
         -1);
  ASSERT_EQ(ina.s_addr, addr);

  return NULL;
}

static const char *test_dns_reply_encode(void) {
  const char *err;
  struct mg_dns_message msg;
  struct mg_dns_resource_record *rr;
  char name[256];
  in_addr_t addr = inet_addr("54.194.65.250");
  struct mbuf pkt;
  struct mg_connection nc;

  mbuf_init(&pkt, 0);
  init_test_connection(&nc);

  /* create a fake query */

  mg_send_dns_query(&nc, "www.cesanta.com", MG_DNS_A_RECORD);
  /* remove message length from tcp buffer */
  mbuf_remove(&nc.send_mbuf, 2);

  ASSERT_EQ(mg_parse_dns(nc.send_mbuf.buf, nc.send_mbuf.len, &msg), 0);

  /* build an answer */

  msg.num_answers = 2;
  mg_dns_insert_header(&pkt, 0, &msg);
  mg_dns_copy_questions(&pkt, &msg);

  mg_dns_uncompress_name(&msg, &msg.questions[0].name, name, sizeof(name));

  rr = &msg.answers[0];
  *rr = msg.questions[0];
  rr->rtype = MG_DNS_CNAME_RECORD;
  rr->ttl = 3600;
  rr->kind = MG_DNS_ANSWER;
  ASSERT(mg_dns_encode_record(&pkt, rr, "www.cesanta.com", 15,
                              (void *) "cesanta.com", 11) != -1);

  rr = &msg.answers[1];
  *rr = msg.questions[0];
  rr->ttl = 3600;
  rr->kind = MG_DNS_ANSWER;
  ASSERT(mg_dns_encode_record(&pkt, rr, "cesanta.com", 11, &addr, 4) != -1);

  if ((err = check_www_cesanta_com_reply(pkt.buf, pkt.len)) != NULL) {
    return err;
  }

  mbuf_free(&pkt);
  mbuf_free(&nc.send_mbuf);
  return NULL;
}

#if MG_ENABLE_DNS_SERVER
static void dns_server_eh(struct mg_connection *nc, int ev, void *ev_data) {
  struct mg_dns_message *msg;
  struct mg_dns_resource_record *rr;
  struct mg_dns_reply reply;
  char name[512];
  int i;

  name[511] = 0;
  switch (ev) {
    case MG_DNS_MESSAGE:
      msg = (struct mg_dns_message *) ev_data;
      reply = mg_dns_create_reply(&nc->send_mbuf, msg);

      for (i = 0; i < msg->num_questions; i++) {
        rr = &msg->questions[i];
        if (rr->rtype == MG_DNS_A_RECORD) {
          mg_dns_uncompress_name(msg, &rr->name, name, sizeof(name) - 1);

          if (strcmp(name, "cesanta.com") == 0) {
            mg_dns_reply_record(&reply, rr, NULL, rr->rtype, 3600,
                                nc->user_data, 4);
          } else if (strcmp(name, "www.cesanta.com") == 0) {
            mg_dns_reply_record(&reply, rr, NULL, MG_DNS_CNAME_RECORD, 3600,
                                "cesanta.com", strlen("cesanta.com"));

            mg_dns_reply_record(&reply, rr, "cesanta.com", rr->rtype, 3600,
                                nc->user_data, 4);
          }
        }
      }

      /*
       * We don't set the error flag even if there were no answers
       * maching the MG_DNS_A_RECORD query type.
       * This indicates that we have (syntetic) answers for MG_DNS_A_RECORD.
       * See http://goo.gl/QWvufr for a distinction between NXDOMAIN and NODATA.
       */

      mg_dns_send_reply(nc, &reply);
      nc->flags |= MG_F_SEND_AND_CLOSE;
      break;
  }
}

static int check_record_name(struct mg_dns_message *msg, struct mg_str *n,
                             const char *want) {
  char name[512];
  if (mg_dns_uncompress_name(msg, n, name, sizeof(name)) == 0) {
    return 0;
  }
  return strncmp(name, want, sizeof(name)) == 0;
}

static const char *test_dns_server(void) {
  const char *err;
  struct mg_connection nc;
  struct mg_dns_message msg;
  in_addr_t addr = inet_addr("54.194.65.250");
  int ilen;

  init_test_connection(&nc);

  nc.handler = dns_server_eh;
  nc.user_data = &addr;
  mg_set_protocol_dns(&nc);

  mg_send_dns_query(&nc, "www.cesanta.com", MG_DNS_A_RECORD);

  nc.recv_mbuf = nc.send_mbuf;
  mbuf_init(&nc.send_mbuf, 0);

  ilen = nc.recv_mbuf.len;
  nc.proto_handler(&nc, MG_EV_RECV, &ilen);
  mbuf_free(&nc.recv_mbuf);
  /* remove message length from tcp buffer before manually checking */
  mbuf_remove(&nc.send_mbuf, 2);

  if ((err = check_www_cesanta_com_reply(nc.send_mbuf.buf, nc.send_mbuf.len)) !=
      NULL) {
    mbuf_free(&nc.send_mbuf);
    return err;
  }

  mbuf_free(&nc.send_mbuf);

  /* test mg_dns_reply_record */
  mg_send_dns_query(&nc, "cesanta.com", MG_DNS_A_RECORD);

  nc.recv_mbuf = nc.send_mbuf;
  mbuf_init(&nc.send_mbuf, 0);

  ilen = nc.recv_mbuf.len;
  nc.proto_handler(&nc, MG_EV_RECV, &ilen);
  /* remove message length from tcp buffer before manually checking */
  mbuf_remove(&nc.send_mbuf, 2);

  ASSERT(mg_parse_dns(nc.send_mbuf.buf, nc.send_mbuf.len, &msg) != -1);
  ASSERT_EQ(msg.num_answers, 1);
  ASSERT_EQ(msg.answers[0].rtype, MG_DNS_A_RECORD);
  ASSERT(check_record_name(&msg, &msg.answers[0].name, "cesanta.com"));

  mbuf_free(&nc.send_mbuf);
  mbuf_free(&nc.recv_mbuf);

  /* check malformed request error */
  memset(&msg, 0, sizeof(msg));
  ilen = 0;
  nc.proto_handler(&nc, MG_EV_RECV, &ilen);
  /* remove message length from tcp buffer before manually checking */
  mbuf_remove(&nc.send_mbuf, 2);

  ASSERT(mg_parse_dns(nc.send_mbuf.buf, nc.send_mbuf.len, &msg) != -1);
  ASSERT(msg.flags & 1);
  ASSERT_EQ(msg.num_questions, 0);
  ASSERT_EQ(msg.num_answers, 0);

  mbuf_free(&nc.recv_mbuf);
  mbuf_free(&nc.send_mbuf);
  return NULL;
}
#endif /* MG_ENABLE_DNS_SERVER */

static void dns_resolve_cb(struct mg_dns_message *msg, void *data,
                           enum mg_resolve_err e) {
  struct mg_dns_resource_record *rr;
  char cname[256];
  struct in_addr got_addr;
  in_addr_t want_addr = inet_addr("131.107.255.255");

  (void) e;
  rr = mg_dns_next_record(msg, MG_DNS_A_RECORD, NULL);
  if (rr != NULL) {
    mg_dns_parse_record_data(msg, rr, &got_addr, sizeof(got_addr));
  }

  rr = mg_dns_next_record(msg, MG_DNS_CNAME_RECORD, NULL);
  if (rr != NULL) {
    mg_dns_parse_record_data(msg, rr, cname, sizeof(cname));
  }

  /*
   * We saw cases when A query returns only A record, or A and CNAME records.
   * Expect A answer, and optionally CNAME answer.
   */
  if (want_addr == got_addr.s_addr || strcmp(cname, "dns.msftncsi.com") == 0) {
    *(int *) data = 1; /* Success */
  } else {
    *(int *) data = 2; /* Error */
  }
}

static const char *test_dns_resolve(void) {
  struct mg_mgr mgr;
  struct mg_mgr_init_opts opts;

  int data = 0;

  mg_mgr_init(&mgr, NULL);

  /* Microsoft promises dns.msftncsi.com is always 131.107.255.255 */
  mg_resolve_async(&mgr, "dns.msftncsi.com", MG_DNS_A_RECORD, dns_resolve_cb,
                   &data);

  poll_until(&mgr, 1, c_int_ne, &data, (void *) 0);
  ASSERT_EQ(data, 1);

  mg_mgr_free(&mgr);

  data = 0;
  memset(&opts, 0, sizeof(opts));
  opts.nameserver = "8.8.4.4";
  mg_mgr_init_opt(&mgr, NULL, opts);

  /* Microsoft promises dns.msftncsi.com is always 131.107.255.255 */
  mg_resolve_async(&mgr, "dns.msftncsi.com", MG_DNS_A_RECORD, dns_resolve_cb,
                   &data);

  poll_until(&mgr, 1, c_int_ne, &data, (void *) 0);
  ASSERT_EQ(data, 1);

  mg_mgr_free(&mgr);

  data = 0;
  mg_mgr_init(&mgr, NULL);
  mg_set_nameserver(&mgr, "8.8.4.4");

  /* Microsoft promises dns.msftncsi.com is always 131.107.255.255 */
  mg_resolve_async(&mgr, "dns.msftncsi.com", MG_DNS_A_RECORD, dns_resolve_cb,
                   &data);

  poll_until(&mgr, 1, c_int_ne, &data, (void *) 0);
  ASSERT_EQ(data, 1);

  mg_mgr_free(&mgr);

  return NULL;
}

static void dns_resolve_timeout_cb(struct mg_dns_message *msg, void *data,
                                   enum mg_resolve_err e) {
  (void) e;
  if (msg == NULL) {
    *(int *) data = 1;
  }
}

extern char mg_dns_server[256];

static const char *test_dns_resolve_timeout(void) {
  struct mg_mgr mgr;
  struct mg_resolve_async_opts opts;
  int data = 0;
  mg_mgr_init(&mgr, NULL);
  memset(&opts, 0, sizeof(opts));

  opts.nameserver = "7.7.7.7";
  opts.timeout = -1; /* 0 would be the default timeout */
  opts.max_retries = 1;
  mg_resolve_async_opt(&mgr, "www.cesanta.com", MG_DNS_A_RECORD,
                       dns_resolve_timeout_cb, &data, opts);

  poll_until(&mgr, 5, c_int_eq, &data, (void *) 1);
  ASSERT_EQ(data, 1);

  mg_mgr_free(&mgr);
  return NULL;
}

#ifndef NO_RESOLVE_HOSTS_TEST
static const char *test_dns_resolve_hosts(void) {
  union socket_address sa;
  in_addr_t want_addr = inet_addr("127.0.0.1");

  memset(&sa, 0, sizeof(sa));
  ASSERT_EQ(mg_resolve_from_hosts_file("localhost", &sa), 0);
  ASSERT_EQ(sa.sin.sin_addr.s_addr, want_addr);
  ASSERT_EQ(mg_resolve_from_hosts_file("does_not,exist!in_host*file", &sa), -1);

  return NULL;
}
#endif

static void ehb_srv(struct mg_connection *nc, int ev, void *p) {
  struct mbuf *io = &nc->recv_mbuf;
  (void) io;
  (void) p;

  if (ev == MG_EV_RECV) {
    if (*(int *) p == 1) (*(int *) nc->mgr->user_data)++;
    mbuf_remove(io, *(int *) p);
  }
}

static void ehb_srv2(struct mg_connection *c, int ev, void *p) {
  if (ev == MG_EV_CLOSE) {
    if (c->recv_mbuf.size == 1) (*(int *) c->mgr->user_data) = 1;
    (void) p;
  }
}

static const char *test_buffer_limit(void) {
  struct mg_mgr mgr;
  struct mg_connection *clnt, *srv;
  const char *address = "tcp://127.0.0.1:7878";
  int res = 0;

  mg_mgr_init(&mgr, &res);
  ASSERT((srv = mg_bind(&mgr, address, ehb_srv)) != NULL);
  srv->recv_mbuf_limit = 1;
  ASSERT((clnt = mg_connect(&mgr, address, NULL)) != NULL);
  mg_printf(clnt, "abcd");

  poll_until(&mgr, 1, c_int_eq, &res, (void *) 4);

  /* expect four single byte read events */
  ASSERT_EQ(res, 4);

  /*
   * Make sure we're not closing full non-drained connections without any
   * protocol handler
   */
  res = 0;
  srv->handler = ehb_srv2;
  ASSERT((clnt = mg_connect(&mgr, address, NULL)) != NULL);
  mg_printf(clnt, "abcd");
  poll_until(&mgr, 1, c_int_eq, &res, (void *) 1);
  ASSERT_EQ(res, 0);

  /*
   * Make sure we're closing full non-drained connections with mqtt protocol
   * handler
   */
  res = 0;
  srv->handler = ehb_srv2;
  mg_set_protocol_mqtt(srv);
  ASSERT((clnt = mg_connect(&mgr, address, NULL)) != NULL);
  mg_printf(clnt, "abcd");
  poll_until(&mgr, 1, c_int_eq, &res, (void *) 1);
  ASSERT_EQ(res, 1);

  mg_mgr_free(&mgr);
  return NULL;
}

static const char *test_http_parse_header(void) {
  static struct mg_str h = MG_MK_STR(
      "xx=1 kl yy, ert=234 kl=123, qq=ww;"
      "uri=\"/?naii=x,y\";ii=\"12\\\"34\" zz='aa bb',tt=2,gf=\"xx d=1234");
  char buf[20];
  char *buf2;

  ASSERT_EQ(mg_http_parse_header(&h, "ert", buf, sizeof(buf)), 3);
  ASSERT_STREQ(buf, "234");

  ASSERT_EQ(mg_http_parse_header(&h, "ert", buf, 2), 0);
  ASSERT_EQ(mg_http_parse_header(&h, "ert", buf, 3), 0);
  ASSERT_EQ(mg_http_parse_header(&h, "ert", buf, 4), 3);

  buf2 = buf;
  ASSERT_EQ(mg_http_parse_header2(&h, "ert", &buf2, 2), 3);
  ASSERT(buf2 != buf);
  free(buf2);

  buf2 = buf;
  ASSERT_EQ(mg_http_parse_header2(&h, "ert", &buf2, 3), 3);
  ASSERT(buf2 != buf);
  free(buf2);

  buf2 = buf;
  ASSERT_EQ(mg_http_parse_header2(&h, "ert", &buf2, 4), 3);
  ASSERT(buf2 == buf);

  buf2 = NULL;
  ASSERT_EQ(mg_http_parse_header2(&h, "ert", &buf2, 0), 3);
  ASSERT_STREQ(buf2, "234");
  free(buf2);

  ASSERT_EQ(mg_http_parse_header(&h, "gf", buf, sizeof(buf)), 0);
  ASSERT_EQ(mg_http_parse_header(&h, "zz", buf, sizeof(buf)), 5);
  ASSERT_STREQ(buf, "aa bb");
  ASSERT_EQ(mg_http_parse_header(&h, "d", buf, sizeof(buf)), 4);
  ASSERT_STREQ(buf, "1234");
  buf[0] = 'x';
  ASSERT_EQ(mg_http_parse_header(&h, "MMM", buf, sizeof(buf)), 0);
  ASSERT_EQ(buf[0], '\0');
  ASSERT_EQ(mg_http_parse_header(&h, "kl", buf, sizeof(buf)), 3);
  ASSERT_STREQ(buf, "123");
  ASSERT_EQ(mg_http_parse_header(&h, "xx", buf, sizeof(buf)), 1);
  ASSERT_STREQ(buf, "1");
  ASSERT_EQ(mg_http_parse_header(&h, "ii", buf, sizeof(buf)), 5);
  ASSERT_STREQ(buf, "12\"34");
  ASSERT_EQ(mg_http_parse_header(&h, "tt", buf, sizeof(buf)), 1);
  ASSERT_STREQ(buf, "2");
  ASSERT(mg_http_parse_header(&h, "uri", buf, sizeof(buf)) > 0);
  ASSERT_STREQ(buf, "/?naii=x,y");
  ASSERT(mg_http_parse_header(&h, "qq", buf, sizeof(buf)) > 0);
  ASSERT_STREQ(buf, "ww");

  return NULL;
}

#if MG_ENABLE_COAP
struct results {
  int server;
  int client;
};

static void coap_handler_1(struct mg_connection *nc, int ev, void *p) {
  switch (ev) {
    case MG_EV_CONNECT: {
      struct mg_coap_message cm;
      memset(&cm, 0, sizeof(cm));
      cm.msg_id = 1;
      cm.msg_type = MG_COAP_MSG_CON;
      mg_coap_send_message(nc, &cm);
      break;
    }
    case MG_EV_COAP_ACK: {
      struct mg_coap_message *cm = (struct mg_coap_message *) p;
      ((struct results *) (nc->user_data))->client = cm->msg_id + cm->msg_type;
      break;
    }
    case MG_EV_COAP_CON: {
      struct mg_coap_message *cm = (struct mg_coap_message *) p;
      ((struct results *) (nc->user_data))->server = cm->msg_id + cm->msg_type;
      mg_coap_send_ack(nc, cm->msg_id);
      break;
    }
  }
}

static const char *test_coap(void) {
  struct mbuf packet_in, packet_out;
  struct mg_coap_message cm;
  uint32_t res;

  unsigned char coap_packet_1[] = {0x42, 0x01, 0xe9, 0x1b, 0x07, 0x90, 0xb8,
                                   0x73, 0x65, 0x70, 0x61, 0x72, 0x61, 0x74,
                                   0x65, 0x10, 0xd1, 0x23, 0x11};
  unsigned char coap_packet_2[] = {0x60, 0x00, 0xe9, 0x1b};
  unsigned char coap_packet_3[] = {
      0x42, 0x45, 0x57, 0x0f, 0x07, 0x90, 0xff, 0x54, 0x68, 0x69, 0x73, 0x20,
      0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x20, 0x77, 0x61, 0x73, 0x20,
      0x73, 0x65, 0x6e, 0x74, 0x20, 0x62, 0x79, 0x20, 0x61, 0x20, 0x73, 0x65,
      0x70, 0x61, 0x72, 0x61, 0x74, 0x65, 0x20, 0x72, 0x65, 0x73, 0x70, 0x6f,
      0x6e, 0x73, 0x65, 0x2e, 0x0a, 0x59, 0x6f, 0x75, 0x72, 0x20, 0x63, 0x6c,
      0x69, 0x65, 0x6e, 0x74, 0x20, 0x77, 0x69, 0x6c, 0x6c, 0x20, 0x6e, 0x65,
      0x65, 0x64, 0x20, 0x74, 0x6f, 0x20, 0x61, 0x63, 0x6b, 0x6e, 0x6f, 0x77,
      0x6c, 0x65, 0x64, 0x67, 0x65, 0x20, 0x69, 0x74, 0x2c, 0x20, 0x6f, 0x74,
      0x68, 0x65, 0x72, 0x77, 0x69, 0x73, 0x65, 0x20, 0x69, 0x74, 0x20, 0x77,
      0x69, 0x6c, 0x6c, 0x20, 0x62, 0x65, 0x20, 0x72, 0x65, 0x74, 0x72, 0x61,
      0x6e, 0x73, 0x6d, 0x69, 0x74, 0x74, 0x65, 0x64, 0x2e};
  unsigned char coap_packet_4[] = {0x60, 0x00, 0x57, 0x0f};
  unsigned char coap_packet_5[] = {
      0x40, 0x03, 0x95, 0x22, 0xb7, 0x73, 0x74, 0x6f, 0x72, 0x61,
      0x67, 0x65, 0x0a, 0x6d, 0x79, 0x72, 0x65, 0x73, 0x6f, 0x75,
      0x72, 0x63, 0x65, 0xff, 0x6d, 0x79, 0x64, 0x61, 0x74, 0x61};
  unsigned char coap_packet_6[] = {0xFF, 0x00, 0xFF, 0x00};
  unsigned char coap_packet_7[] = {
      0x40, 0x03, 0x95, 0x22, 0xb7, 0x73, 0x74, 0x6f, 0x72, 0x61,
      0x67, 0x65, 0x0a, 0x6d, 0x79, 0x72, 0x65, 0x73, 0x6f, 0x75,
      0x72, 0x63, 0x65, 0xf1, 0x6d, 0x79, 0x64, 0x61, 0x74, 0x61};

  mbuf_init(&packet_in, 0);
  /* empty buf */
  res = mg_coap_parse(&packet_in, &cm);
  ASSERT((res & MG_COAP_NOT_ENOUGH_DATA) != 0);
  mg_coap_free_options(&cm);
  mbuf_free(&packet_in);

  mbuf_init(&packet_out, 0);
  /* ACK, MID: 59675, Empty Message */
  packet_in.buf = (char *) coap_packet_2;
  packet_in.len = sizeof(coap_packet_2);
  res = mg_coap_parse(&packet_in, &cm);
  ASSERT_EQ((res & MG_COAP_ERROR), 0);
  ASSERT_EQ(cm.code_class, 0);
  ASSERT_EQ(cm.code_detail, 0);
  ASSERT_EQ(cm.msg_id, 59675);
  ASSERT_EQ(cm.msg_type, MG_COAP_MSG_ACK);
  ASSERT(cm.options == NULL);
  ASSERT_EQ(cm.payload.len, 0);
  ASSERT(cm.payload.p == NULL);
  ASSERT_EQ(cm.token.len, 0);
  ASSERT(cm.token.p == NULL);
  res = mg_coap_compose(&cm, &packet_out);
  ASSERT_EQ(res, 0);
  ASSERT_EQ(packet_out.len, sizeof(coap_packet_2));
  ASSERT_EQ(memcmp(packet_out.buf, coap_packet_2, packet_out.len), 0);
  mg_coap_free_options(&cm);
  mbuf_free(&packet_out);

  /* ACK, MID: 22287, Empty Message */
  packet_in.buf = (char *) coap_packet_4;
  packet_in.len = sizeof(coap_packet_4);
  res = mg_coap_parse(&packet_in, &cm);
  ASSERT_EQ((res & MG_COAP_ERROR), 0);
  ASSERT_EQ(cm.code_class, 0);
  ASSERT_EQ(cm.code_detail, 0);
  ASSERT_EQ(cm.msg_id, 22287);
  ASSERT_EQ(cm.msg_type, MG_COAP_MSG_ACK);
  ASSERT(cm.options == NULL);
  ASSERT_EQ(cm.payload.len, 0);
  ASSERT(cm.payload.p == NULL);
  ASSERT_EQ(cm.token.len, 0);
  ASSERT(cm.token.p == NULL);
  res = mg_coap_compose(&cm, &packet_out);
  ASSERT_EQ(res, 0);
  ASSERT_EQ(packet_out.len, sizeof(coap_packet_4));
  ASSERT_EQ(memcmp(packet_out.buf, coap_packet_4, packet_out.len), 0);
  mg_coap_free_options(&cm);
  mbuf_free(&packet_out);

  /* CON, MID: 59675 ... */
  packet_in.buf = (char *) coap_packet_1;
  packet_in.len = sizeof(coap_packet_1);
  res = mg_coap_parse(&packet_in, &cm);
  ASSERT_EQ((res & MG_COAP_ERROR), 0);
  ASSERT_EQ(cm.code_class, 0);
  ASSERT_EQ(cm.code_detail, 1);
  ASSERT_EQ(cm.msg_id, 59675);
  ASSERT_EQ(cm.msg_type, MG_COAP_MSG_CON);
  ASSERT(cm.options != 0);
  ASSERT_EQ(cm.options->number, 11);
  ASSERT_EQ(cm.options->value.len, 8);
  ASSERT_STREQ_NZ(cm.options->value.p, "separate");
  ASSERT(cm.options->next != 0);
  ASSERT_EQ(cm.options->next->number, 12);
  ASSERT_EQ(cm.options->next->value.len, 0);
  ASSERT(cm.options->next->next != 0);
  ASSERT_EQ(cm.options->next->next->number, 60);
  ASSERT_EQ(cm.options->next->next->value.len, 1);
  ASSERT_EQ(*cm.options->next->next->value.p, 0x11);
  ASSERT(cm.options->next->next->next == NULL);
  ASSERT_EQ(cm.payload.len, 0);
  ASSERT(cm.payload.p == NULL);
  ASSERT_EQ(cm.token.len, 2);
  ASSERT_EQ(*cm.token.p, 0x07);
  ASSERT_EQ((unsigned char) *(cm.token.p + 1), 0x90);
  res = mg_coap_compose(&cm, &packet_out);
  ASSERT_EQ(res, 0);
  ASSERT_EQ(packet_out.len, sizeof(coap_packet_1));
  ASSERT_EQ(memcmp(packet_out.buf, coap_packet_1, packet_out.len), 0);
  mg_coap_free_options(&cm);
  mbuf_free(&packet_out);

  /* CON, MID: 22287 ... */
  packet_in.buf = (char *) coap_packet_3;
  packet_in.len = sizeof(coap_packet_3);
  res = mg_coap_parse(&packet_in, &cm);
  ASSERT_EQ((res & MG_COAP_ERROR), 0);
  ASSERT_EQ(cm.code_class, 2);
  ASSERT_EQ(cm.code_detail, 5);
  ASSERT_EQ(cm.msg_id, 22287);
  ASSERT_EQ(cm.msg_type, MG_COAP_MSG_CON);
  ASSERT(cm.options == NULL);
  ASSERT_EQ(cm.token.len, 2);
  ASSERT_EQ(*cm.token.p, 0x07);
  ASSERT_EQ((unsigned char) *(cm.token.p + 1), 0x90);
  ASSERT_EQ(cm.payload.len, 122);
  ASSERT(strncmp(cm.payload.p,
                 "This message was sent by a separate response.\n"
                 "Your client will need to acknowledge it,"
                 " otherwise it will be retransmitted.",
                 122) == 0);
  res = mg_coap_compose(&cm, &packet_out);
  ASSERT_EQ(res, 0);
  ASSERT_EQ(packet_out.len, sizeof(coap_packet_3));
  ASSERT_EQ(memcmp(packet_out.buf, coap_packet_3, packet_out.len), 0);
  mg_coap_free_options(&cm);
  mbuf_free(&packet_out);

  packet_in.buf = (char *) coap_packet_5;
  packet_in.len = sizeof(coap_packet_5);
  res = mg_coap_parse(&packet_in, &cm);
  ASSERT_EQ((res & MG_COAP_ERROR), 0);
  ASSERT_EQ(cm.code_class, 0);
  ASSERT_EQ(cm.code_detail, 3);
  ASSERT_EQ(cm.msg_id, 38178);
  ASSERT_EQ(cm.msg_type, MG_COAP_MSG_CON);
  ASSERT(cm.options != 0);
  ASSERT_EQ(cm.options->number, 11);
  ASSERT_EQ(cm.options->value.len, 7);
  ASSERT_STREQ_NZ(cm.options->value.p, "storage");
  ASSERT(cm.options->next != 0);
  ASSERT_EQ(cm.options->next->number, 11);
  ASSERT_EQ(cm.options->next->value.len, 10);
  ASSERT_STREQ_NZ(cm.options->next->value.p, "myresource");
  ASSERT(cm.options->next->next == NULL);
  ASSERT_EQ(cm.token.len, 0);
  ASSERT_EQ(cm.payload.len, 6);
  ASSERT_STREQ_NZ(cm.payload.p, "mydata");
  res = mg_coap_compose(&cm, &packet_out);
  ASSERT_EQ(res, 0);
  ASSERT_EQ(packet_out.len, sizeof(coap_packet_5));
  ASSERT_EQ(memcmp(packet_out.buf, coap_packet_5, packet_out.len), 0);
  mg_coap_free_options(&cm);
  mbuf_free(&packet_out);

  packet_in.buf = (char *) coap_packet_6;
  packet_in.len = sizeof(coap_packet_6);
  res = mg_coap_parse(&packet_in, &cm);
  ASSERT((res & MG_COAP_ERROR) != 0);
  mg_coap_free_options(&cm);

  packet_in.buf = (char *) coap_packet_7;
  packet_in.len = sizeof(coap_packet_7);
  res = mg_coap_parse(&packet_in, &cm);
  ASSERT((res & MG_COAP_ERROR) != 0);
  mg_coap_free_options(&cm);

  {
    unsigned char coap_packet_2_broken[] = {0x6F, 0x00, 0xe9, 0x1b};
    packet_in.buf = (char *) coap_packet_2_broken;
    packet_in.len = sizeof(coap_packet_2_broken);
    res = mg_coap_parse(&packet_in, &cm);
    ASSERT((res & MG_COAP_FORMAT_ERROR) != 0);
  }

  {
    unsigned char coap_packet_2_broken[] = {0x65, 0x00, 0xe9, 0x1b};
    packet_in.buf = (char *) coap_packet_2_broken;
    packet_in.len = sizeof(coap_packet_2_broken);
    res = mg_coap_parse(&packet_in, &cm);
    ASSERT((res & MG_COAP_NOT_ENOUGH_DATA) != 0);
  }

  memset(&cm, 0, sizeof(cm));
  mg_coap_add_option(&cm, 10, 0, 0);
  ASSERT_EQ(cm.options->number, 10);
  ASSERT(cm.options->next == NULL);
  mg_coap_add_option(&cm, 5, 0, 0);
  ASSERT_EQ(cm.options->number, 5);
  ASSERT_EQ(cm.options->next->number, 10);
  ASSERT(cm.options->next->next == NULL);
  mg_coap_add_option(&cm, 7, 0, 0);
  ASSERT_EQ(cm.options->number, 5);
  ASSERT_EQ(cm.options->next->number, 7);
  ASSERT_EQ(cm.options->next->next->number, 10);
  ASSERT(cm.options->next->next->next == NULL);
  mg_coap_add_option(&cm, 1, 0, 0);
  ASSERT_EQ(cm.options->number, 1);
  ASSERT_EQ(cm.options->next->number, 5);
  ASSERT_EQ(cm.options->next->next->number, 7);
  ASSERT_EQ(cm.options->next->next->next->number, 10);
  ASSERT(cm.options->next->next->next->next == NULL);
  mg_coap_free_options(&cm);

  {
    unsigned char value16[] = {0xCC, 0xDD};
    packet_in.buf = (char *) coap_packet_4;
    packet_in.len = sizeof(coap_packet_4);
    res = mg_coap_parse(&packet_in, &cm);
    ASSERT_EQ((res & MG_COAP_ERROR), 0);
    mg_coap_add_option(&cm, 0xAABB, (char *) value16, sizeof(value16));
    res = mg_coap_compose(&cm, &packet_out);
    mg_coap_free_options(&cm);
    ASSERT_EQ(res, 0);
    res = mg_coap_parse(&packet_out, &cm);
    ASSERT_EQ((res & MG_COAP_ERROR), 0);
    ASSERT_EQ(cm.options->number, 0xAABB);
    ASSERT_EQ(cm.options->value.len, 2);
    ASSERT_EQ(memcmp(cm.options->value.p, value16, cm.options->value.len), 0);
    mg_coap_free_options(&cm);
    mbuf_free(&packet_out);
  }

  memset(&cm, 0, sizeof(cm));
  cm.msg_id = 1;
  cm.msg_type = MG_COAP_MSG_MAX + 1;
  res = mg_coap_compose(&cm, &packet_out);
  ASSERT((res & MG_COAP_ERROR) != 0 && (res & MG_COAP_MSG_TYPE_FIELD) != 0);

  cm.msg_type = MG_COAP_MSG_ACK;
  cm.token.len = 10000;
  res = mg_coap_compose(&cm, &packet_out);
  ASSERT((res & MG_COAP_ERROR) != 0 && (res & MG_COAP_TOKEN_FIELD) != 0);

  cm.token.len = 0;
  cm.code_class = 0xFF;
  res = mg_coap_compose(&cm, &packet_out);
  ASSERT((res & MG_COAP_ERROR) != 0 && (res & MG_COAP_CODE_CLASS_FIELD) != 0);

  cm.code_class = 0;
  cm.code_detail = 0xFF;
  res = mg_coap_compose(&cm, &packet_out);
  ASSERT((res & MG_COAP_ERROR) != 0 && (res & MG_COAP_CODE_DETAIL_FIELD) != 0);

  cm.code_detail = 0;
  mg_coap_add_option(&cm, 0xFFFFFFF, 0, 0);
  res = mg_coap_compose(&cm, &packet_out);
  ASSERT((res & MG_COAP_ERROR) != 0 && (res & MG_COAP_OPTIOMG_FIELD) != 0);
  mg_coap_free_options(&cm);

  {
    struct mg_mgr mgr;
    struct mg_connection *nc;
    const char *address = "tcp://127.0.0.1:8686";

    mg_mgr_init(&mgr, 0);

    nc = mg_bind(&mgr, address, coap_handler_1);
    ASSERT(nc != NULL);
    ASSERT_EQ(mg_set_protocol_coap(nc), -1);

    mg_mgr_free(&mgr);
  }

  {
    struct results res;
    struct mg_mgr mgr;
    struct mg_connection *nc1, *nc2;
    const char *address = "udp://127.0.0.1:5683";

    mg_mgr_init(&mgr, 0);

    nc1 = mg_bind(&mgr, address, coap_handler_1);
    mg_set_protocol_coap(nc1);
    nc1->user_data = &res;

    nc2 = mg_connect(&mgr, address, coap_handler_1);
    mg_set_protocol_coap(nc2);
    nc2->user_data = &res;

    poll_until(&mgr, 5, c_int_eq, &res.client, (void *) 3);

    mg_mgr_free(&mgr);

    ASSERT_EQ(res.server, 1);
    ASSERT_EQ(res.client, 3);
  }

  return NULL;
}
#endif

static const char *test_strcmp(void) {
  struct mg_str s1;

  s1.p = "aa";
  s1.len = strlen(s1.p);
  ASSERT_EQ(mg_vcasecmp(&s1, "aa"), 0);
  ASSERT_EQ(mg_vcmp(&s1, "aa"), 0);
  ASSERT(mg_vcasecmp(&s1, "ab") < 0);
  ASSERT(mg_vcmp(&s1, "ab") < 0);
  ASSERT(mg_vcasecmp(&s1, "abb") < 0);
  ASSERT(mg_vcmp(&s1, "abb") < 0);
  ASSERT(mg_vcasecmp(&s1, "b") < 0);
  ASSERT(mg_vcmp(&s1, "b") < 0);
  return NULL;
}

#if MG_ENABLE_SNTP
static const char *test_sntp(void) {
  const char sntp_good[] =
      "\x24\x02\x00\xeb\x00\x00\x00\x1e\x00\x00\x07\xb6\x3e"
      "\xc9\xd6\xa2\xdb\xde\xea\x30\x91\x86\xb7\x10\xdb\xde"
      "\xed\x98\x00\x00\x00\xde\xdb\xde\xed\x99\x0a\xe2\xc7"
      "\x96\xdb\xde\xed\x99\x0a\xe4\x6b\xda";
  const char bad_good[] =
      "\x55\x02\x00\xeb\x00\x00\x00\x1e\x00\x00\x07\xb6\x3e"
      "\xc9\xd6\xa2\xdb\xde\xea\x30\x91\x86\xb7\x10\xdb\xde"
      "\xed\x98\x00\x00\x00\xde\xdb\xde\xed\x99\x0a\xe2\xc7"
      "\x96\xdb\xde\xed\x99\x0a\xe4\x6b\xda";

  int res;
  struct mg_sntp_message msg;
  struct tm *tm;
  time_t t;
  memset(&msg, 0, sizeof(0));

  res = mg_sntp_parse_reply(sntp_good, sizeof(sntp_good) - 1, &msg);
  ASSERT_EQ(res, 0);
  t = (time_t) msg.time;
  tm = gmtime(&t);
  ASSERT_EQ(tm->tm_year, 116);
  ASSERT_EQ(tm->tm_mon, 10);
  ASSERT_EQ(tm->tm_mday, 22);
  ASSERT_EQ(tm->tm_hour, 16);
  ASSERT_EQ(tm->tm_min, 15);
  ASSERT_EQ(tm->tm_sec, 21);
  res = mg_sntp_parse_reply(bad_good, sizeof(bad_good) - 1, &msg);
  ASSERT_EQ(res, -1);

  return NULL;
}
#endif

#if MG_ENABLE_SOCKS
static const char *test_socks(void) {
  struct mg_mgr mgr;
  struct mg_connection *c;
  struct mg_connect_opts opts;
  const char *this_binary;
  const char *web_addr = "127.0.0.1:7257";
  const char *proxy_addr = "127.0.0.1:7258";
  char status[100] = "";

  mg_mgr_init(&mgr, NULL);
#if 0
  mgr.hexdump_file = "-";
#endif

#if 1
  cs_log_set_level(LL_INFO);
#endif

  /* Start web server */
  ASSERT((c = mg_bind(&mgr, web_addr, cb1)) != NULL);
  mg_set_protocol_http_websocket(c);

  /* Start socks proxy */
  ASSERT((c = mg_bind(&mgr, proxy_addr, NULL)) != NULL);
  mg_set_protocol_socks(c);

  /* Create HTTP client that uses socks proxy */
  memset(&opts, 0, sizeof(opts));
  opts.iface = mg_socks_mk_iface(&mgr, proxy_addr);

  ASSERT((c = mg_connect_opt(&mgr, web_addr, cb7, opts)) != NULL);
  mg_set_protocol_http_websocket(c);
  c->user_data = status;
  /* Wine and GDB set argv0 to full path: strip the dir component */
  if ((this_binary = strrchr(g_argv_0, '\\')) != NULL) {
    this_binary++;
  } else if ((this_binary = strrchr(g_argv_0, '/')) != NULL) {
    this_binary++;
  } else {
    this_binary = g_argv_0;
  }
  mg_printf(c, "GET /%s HTTP/1.0\n\n", this_binary);
  mbuf_resize(&c->recv_mbuf, 10000000);

  /* Run event loop. Use more cycles to let file download complete. */
  poll_until(&mgr, 15, c_str_ne, status, (void *) "");
  ASSERT_STREQ(status, "success");

  mg_mgr_free(&mgr);

  return NULL;
}
#endif

void tests_setup(void) {
  test_iface = mg_if_create_iface(mg_ifaces[MG_MAIN_IFACE], NULL);
}

const char *tests_run(const char *filter) {
  RUN_TEST(test_mbuf);
  RUN_TEST(test_parse_uri);
  RUN_TEST(test_assemble_uri);
  RUN_TEST(test_parse_address);
  RUN_TEST(test_mg_normalize_uri_path);
  RUN_TEST(test_mg_uri_to_local_path);
  RUN_TEST(test_mg_url_encode);
  RUN_TEST(test_check_ip_acl);
  RUN_TEST(test_connect_opts);
  RUN_TEST(test_connect_opts_error_string);
  RUN_TEST(test_to64);
  RUN_TEST(test_alloc_vprintf);
  RUN_TEST(test_socketpair);
  RUN_TEST(test_timer);
#ifdef __linux__
  RUN_TEST(test_simple);
#endif
#if MG_ENABLE_THREADS
  RUN_TEST(test_thread);
#endif
  RUN_TEST(test_mgr);
  RUN_TEST(test_parse_http_message);
  RUN_TEST(test_get_http_var);
  RUN_TEST(test_http_serve_file);
  RUN_TEST(test_http_serve_file_streaming);
  RUN_TEST(test_http);
  RUN_TEST(test_http_pipeline);
  RUN_TEST(test_http_send_redirect);
  RUN_TEST(test_http_digest_auth);
  RUN_TEST(test_http_errors);
  RUN_TEST(test_http_index);
  RUN_TEST(test_http_parse_header);
  RUN_TEST(test_ssi);
#ifndef NO_CGI_TEST
  RUN_TEST(test_cgi);
#endif
  RUN_TEST(test_http_rewrites);
  RUN_TEST(test_http_dav);
  RUN_TEST(test_http_range);
  RUN_TEST(test_http_multipart);
#if MG_ENABLE_HTTP_STREAMING_MULTIPART
  RUN_TEST(test_http_multipart2);
  RUN_TEST(test_http_multipart_pipeline);
  RUN_TEST(test_http_multipart_upload);
#endif
  RUN_TEST(test_parse_date_string);
  RUN_TEST(test_websocket);
  RUN_TEST(test_websocket_endpoint);
  RUN_TEST(test_connect_ws);
  RUN_TEST(test_websocket_big);
  RUN_TEST(test_http_chunk);
  RUN_TEST(test_http_chunk2);
  RUN_TEST(test_http_not_modified);
  RUN_TEST(test_http_extra_headers);
  RUN_TEST(test_http_endpoints);
  RUN_TEST(test_mqtt_handshake);
  RUN_TEST(test_mqtt_publish);
  RUN_TEST(test_mqtt_subscribe);
  RUN_TEST(test_mqtt_unsubscribe);
  RUN_TEST(test_mqtt_connack);
  RUN_TEST(test_mqtt_suback);
  RUN_TEST(test_mqtt_simple_acks);
  RUN_TEST(test_mqtt_nullary);
  RUN_TEST(test_mqtt_parse_mqtt);
  RUN_TEST(test_mqtt_parse_mqtt_qos1);
  RUN_TEST(test_mqtt_match_topic_expression);
  RUN_TEST(test_mqtt_client_keep_alive);
#if MG_ENABLE_MQTT_BROKER
  RUN_TEST(test_mqtt_broker);
#endif
  RUN_TEST(test_dns_encode);
  RUN_TEST(test_dns_uncompress);
  RUN_TEST(test_dns_decode);
  RUN_TEST(test_dns_decode_truncated);
  RUN_TEST(test_dns_reply_encode);
#if MG_ENABLE_DNS_SERVER
  RUN_TEST(test_dns_server);
#endif
  RUN_TEST(test_dns_resolve);
  RUN_TEST(test_dns_resolve_timeout);
#ifndef NO_RESOLVE_HOSTS_TEST
  RUN_TEST(test_dns_resolve_hosts);
#endif
  RUN_TEST(test_buffer_limit);
  RUN_TEST(test_connection_errors);
  RUN_TEST(test_connect_fail);
#ifndef NO_DNS_TEST
  RUN_TEST(test_resolve);
#endif
  RUN_TEST(test_base64);
  RUN_TEST(test_sock_addr_to_str);
  RUN_TEST(test_hexdump);
  RUN_TEST(test_hexdump_file);
  RUN_TEST(test_basic_auth_helpers);
  RUN_TEST(test_http_auth);
#if MG_ENABLE_SSL
  RUN_TEST(test_ssl);
#ifdef OPENSSL_VERSION_NUMBER
  RUN_TEST(test_modern_crypto);
#endif
#endif
  RUN_TEST(test_udp);
#if MG_ENABLE_COAP
  RUN_TEST(test_coap);
#endif
#if MG_ENABLE_SNTP
  RUN_TEST(test_sntp);
#endif
  RUN_TEST(test_strcmp);
#if MG_ENABLE_SOCKS
  RUN_TEST(test_socks);
#endif
  return NULL;
}

void tests_teardown(void) {
  free(test_iface);
}
