#pragma once
#include "str.h"

// Returns the port from url. Uses scheme defaults when no port is explicit:
// http/ws=80, https/wss=443, mqtt=1883, mqtts=8883. Returns 0 if unknown.
unsigned short mg_url_port(const char *url);

// Returns true if url uses a TLS scheme (https, wss, mqtts, ssl, tls, tcps).
bool mg_url_is_ssl(const char *url);

// Returns the hostname from url as mg_str, without port or brackets.
// E.g. "http://foo.com:8080/x" -> "foo.com". Not NUL-terminated.
struct mg_str mg_url_host(const char *url);

// Returns the username from url (user:pass@host form) as mg_str.
// Returns an empty mg_str if no credentials are present. Not NUL-terminated.
struct mg_str mg_url_user(const char *url);

// Returns the password from url (user:pass@host form) as mg_str.
// Returns an empty mg_str if no credentials are present. Not NUL-terminated.
struct mg_str mg_url_pass(const char *url);

// Returns a pointer to the URI path in url (e.g. "/api/v1").
// Returns "/" if the url has no path. Points into the original url string.
const char *mg_url_uri(const char *url);
