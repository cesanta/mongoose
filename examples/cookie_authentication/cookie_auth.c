// Copyright (c) 2014 Cesanta Software
// All rights reserved

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "mongoose.h"

static const char *s_login_uri = "/login.html";
static const char *s_secret = ":-)";  // Must be known only to server

static void generate_ssid(const char *user_name, const char *expiration_date,
                          char *ssid, size_t ssid_size) {
  char hash[33];
  mg_md5(hash, user_name, ":", expiration_date, ":", s_secret, NULL);
  snprintf(ssid, ssid_size, "%s|%s|%s", user_name, expiration_date, hash);
}

static int check_auth(struct mg_connection *conn) {
  char ssid[100], calculated_ssid[100], name[100], expire[100];

  // Always authenticate requests to login page
  if (strcmp(conn->uri, s_login_uri) == 0) {
    return MG_TRUE;
  }

  // Look for session ID in the Cookie.
  // That session ID can be validated against the database that stores
  // current active sessions.
  mg_parse_header(mg_get_header(conn, "Cookie"), "ssid", ssid, sizeof(ssid));
  if (sscanf(ssid, "%[^|]|%[^|]|", name, expire) == 2) {
    generate_ssid(name, expire, calculated_ssid, sizeof(calculated_ssid));
    if (strcmp(ssid, calculated_ssid) == 0) {
      return MG_TRUE;  // Authenticate
    }
  }

  // Auth failed, do NOT authenticate, redirect to login page
  mg_printf(conn, "HTTP/1.1 302 Moved\r\nLocation: %s\r\n\r\n", s_login_uri);
  return MG_FALSE;
}

static int check_login_form_submission(struct mg_connection *conn) {
  char name[100], password[100], ssid[100], expire[100], expire_epoch[100];

  mg_get_var(conn, "name", name, sizeof(name));
  mg_get_var(conn, "password", password, sizeof(password));

  // A real authentication mechanism should be employed here.
  // Also, the whole site should be served through HTTPS.
  if (strcmp(name, "Joe") == 0 && strcmp(password, "Doe") == 0) {
    // Generate expiry date
    time_t t = time(NULL) + 3600;  // Valid for 1 hour
    snprintf(expire_epoch, sizeof(expire_epoch), "%lu", (unsigned long) t);
    strftime(expire, sizeof(expire), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&t));
    generate_ssid(name, expire_epoch, ssid, sizeof(ssid));
    // Set "session id" cookie, there could be some data encoded in it.
    mg_printf(conn,
              "HTTP/1.1 302 Moved\r\n"
              "Set-Cookie: ssid=%s; expire=\"%s\"; http-only; HttpOnly;\r\n"
              "Content-Length: 0\r\n"
              "Location: /\r\n\r\n",
              ssid, expire);
    return MG_TRUE;
  }
  return MG_FALSE;
}

static int serve_request(struct mg_connection *conn) {
  if (strcmp(conn->uri, s_login_uri) == 0 &&
      strcmp(conn->request_method, "POST") == 0) {
    return check_login_form_submission(conn);
  }
  return MG_FALSE;  // Serve files in the document_root
}

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  switch (ev) {
    case MG_AUTH: return check_auth(conn);
    case MG_REQUEST: return serve_request(conn);
    default: return MG_FALSE;
  }
}

int main(void) {
  struct mg_server *server = mg_create_server(NULL, ev_handler);
  mg_set_option(server, "listening_port", "8080");
  mg_set_option(server, "document_root", ".");

  printf("Starting on port %s\n", mg_get_option(server, "listening_port"));
  for (;;) {
    mg_poll_server(server, 1000);
  }
  mg_destroy_server(&server);

  return 0;
}
