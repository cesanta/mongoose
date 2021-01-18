#include <stdlib.h>
#include "url.h"

struct url {
  int key, user, pass, host, port, uri, end;
};

int mg_url_is_ssl(const char *url) {
  return strncmp(url, "wss:", 4) == 0 || strncmp(url, "https:", 6) == 0 ||
         strncmp(url, "mqtts:", 6) == 0 || strncmp(url, "ssl:", 4) == 0 ||
         strncmp(url, "tls:", 4) == 0;
}

static struct url urlparse(const char *url) {
  int i;
  struct url u;
  memset(&u, 0, sizeof(u));
  for (i = 0; url[i] != '\0'; i++) {
    if (i > 0 && url[i - 1] == '/' && url[i] == '/') {
      u.host = i + 1;
      u.port = 0;
    } else if (url[i] == ']') {
      u.port = 0;  // IPv6 URLs, like http://[::1]/bar
    } else if (url[i] == ':') {
      u.port = i + 1;
    } else if (url[i] == '@') {
      u.user = u.host;
      u.pass = u.port;
      u.host = i + 1;
      u.port = 0;
    } else if (u.host && !u.uri && url[i] == '/') {
      u.uri = i;
    }
  }
  u.end = i;
#if 0
  printf("[%s] %d %d %d %d %d\n", url, u.user, u.pass, u.host, u.port, u.uri);
#endif
  return u;
}

struct mg_str mg_url_host(const char *url) {
  struct url u = urlparse(url);
  int n =
      u.port ? u.port - u.host - 1 : u.uri ? u.uri - u.host : u.end - u.host;
  struct mg_str s = mg_str_n(url + u.host, n);
  if (s.len > 2 && s.ptr[0] == '[' && s.ptr[s.len - 1] == ']') {
    s.len -= 2;
    s.ptr++;
  }
  return s;
}

const char *mg_url_uri(const char *url) {
  struct url u = urlparse(url);
  return u.uri ? url + u.uri : "/";
}

unsigned short mg_url_port(const char *url) {
  struct url u = urlparse(url);
  unsigned short port = 0;
  if (memcmp(url, "http:", 5) == 0 || memcmp(url, "ws:", 3) == 0) port = 80;
  if (memcmp(url, "wss:", 4) == 0 || memcmp(url, "https:", 6) == 0) port = 443;
  if (memcmp(url, "mqtt:", 5) == 0) port = 1883;
  if (memcmp(url, "mqtts:", 6) == 0) port = 8883;
  if (u.port) port = atoi(url + u.port);
  return port;
}

struct mg_str mg_url_user(const char *url) {
  struct url u = urlparse(url);
  struct mg_str s = mg_str("");
  if (u.user && (u.pass || u.host)) {
    int n = u.pass ? u.pass - u.user - 1 : u.host - u.user - 1;
    s = mg_str_n(url + u.user, n);
  }
  return s;
}

struct mg_str mg_url_pass(const char *url) {
  struct url u = urlparse(url);
  struct mg_str s = mg_str("");
  if (u.pass && u.host) {
    int n = u.host - u.pass - 1;
    s = mg_str_n(url + u.pass, n);
  }
  return s;
}
