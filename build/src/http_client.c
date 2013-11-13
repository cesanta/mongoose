#include "internal.h"

static SOCKET conn2(const char *host, int port, int use_ssl,
                    char *ebuf, size_t ebuf_len) {
  struct sockaddr_in sin;
  struct hostent *he = NULL;
  SOCKET sock = INVALID_SOCKET;

  (void) use_ssl; // Prevent warning for -DNO_SSL case

  if (host == NULL) {
    snprintf(ebuf, ebuf_len, "%s", "NULL host");
#ifndef NO_SSL
  } else if (use_ssl && SSLv23_client_method == NULL) {
    snprintf(ebuf, ebuf_len, "%s", "SSL is not initialized");
    // TODO(lsm): use something threadsafe instead of gethostbyname()
#endif
  } else if ((he = gethostbyname(host)) == NULL) {
    snprintf(ebuf, ebuf_len, "gethostbyname(%s): %s", host, strerror(ERRNO));
  } else if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
    snprintf(ebuf, ebuf_len, "socket(): %s", strerror(ERRNO));
  } else {
    set_close_on_exec(sock);
    sin.sin_family = AF_INET;
    sin.sin_port = htons((uint16_t) port);
    sin.sin_addr = * (struct in_addr *) he->h_addr_list[0];
    if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) != 0) {
      snprintf(ebuf, ebuf_len, "connect(%s:%d): %s",
               host, port, strerror(ERRNO));
      closesocket(sock);
      sock = INVALID_SOCKET;
    }
  }
  return sock;
}

struct mg_connection *mg_connect(const char *host, int port, int use_ssl,
                                 char *ebuf, size_t ebuf_len) {
  static struct mg_context fake_ctx;
  struct mg_connection *conn = NULL;
  SOCKET sock;

  if ((sock = conn2(host, port, use_ssl, ebuf, ebuf_len)) == INVALID_SOCKET) {
  } else if ((conn = (struct mg_connection *)
              calloc(1, sizeof(*conn) + MAX_REQUEST_SIZE)) == NULL) {
    snprintf(ebuf, ebuf_len, "calloc(): %s", strerror(ERRNO));
    closesocket(sock);
#ifndef NO_SSL
  } else if (use_ssl && (conn->client_ssl_ctx =
                         SSL_CTX_new(SSLv23_client_method())) == NULL) {
    snprintf(ebuf, ebuf_len, "SSL_CTX_new error");
    closesocket(sock);
    free(conn);
    conn = NULL;
#endif // NO_SSL
  } else {
    socklen_t len = sizeof(struct sockaddr);
    conn->buf_size = MAX_REQUEST_SIZE;
    conn->buf = (char *) (conn + 1);
    conn->ctx = &fake_ctx;
    conn->client.sock = sock;
    getsockname(sock, &conn->client.rsa.sa, &len);
    conn->client.is_ssl = use_ssl;
#ifndef NO_SSL
    if (use_ssl) {
      // SSL_CTX_set_verify call is needed to switch off server certificate
      // checking, which is off by default in OpenSSL and on in yaSSL.
      SSL_CTX_set_verify(conn->client_ssl_ctx, 0, 0);
      sslize(conn, conn->client_ssl_ctx, SSL_connect);
    }
#endif
  }

  return conn;
}

struct mg_connection *mg_download(const char *host, int port, int use_ssl,
                                  char *ebuf, size_t ebuf_len,
                                  const char *fmt, ...) {
  struct mg_connection *conn;
  va_list ap;

  va_start(ap, fmt);
  ebuf[0] = '\0';
  if ((conn = mg_connect(host, port, use_ssl, ebuf, ebuf_len)) == NULL) {
  } else if (mg_vprintf(conn, fmt, ap) <= 0) {
    snprintf(ebuf, ebuf_len, "%s", "Error sending request");
  } else {
    getreq(conn, ebuf, ebuf_len);
  }
  if (ebuf[0] != '\0' && conn != NULL) {
    mg_close_connection(conn);
    conn = NULL;
  }

  return conn;
}
