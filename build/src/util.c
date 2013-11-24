#include "internal.h"

// Return fake connection structure. Used for logging, if connection
// is not applicable at the moment of logging.
static struct mg_connection *fc(struct mg_context *ctx) {
  static struct mg_connection fake_connection;
  fake_connection.ctx = ctx;
  // See https://github.com/cesanta/mongoose/issues/236
  fake_connection.event.user_data = ctx->user_data;
  return &fake_connection;
}

static void sockaddr_to_string(char *buf, size_t len,
                                     const union usa *usa) {
  buf[0] = '\0';
#if defined(USE_IPV6)
  inet_ntop(usa->sa.sa_family, usa->sa.sa_family == AF_INET ?
            (void *) &usa->sin.sin_addr :
            (void *) &usa->sin6.sin6_addr, buf, len);
#elif defined(_WIN32)
  // Only Windoze Vista (and newer) have inet_ntop()
  strncpy(buf, inet_ntoa(usa->sin.sin_addr), len);
#else
  inet_ntop(usa->sa.sa_family, (void *) &usa->sin.sin_addr, buf, len);
#endif
}

static void gmt_time_string(char *buf, size_t buf_len, time_t *t) {
  strftime(buf, buf_len, "%a, %d %b %Y %H:%M:%S GMT", gmtime(t));
}
