// Copyright (c) 2014 Cesanta Software
// All rights reserved

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "mongoose.h"

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;
static const char *s_login_uri = "/login.html";
static const char *s_secret = ":-)";  // Must be known only to server

static void generate_ssid(const char *user_name, const char *expiration_date,
                          char *ssid, size_t ssid_size) {
  char hash[33];
  cs_md5(hash, user_name, strlen(user_name), ":", (size_t) 1, expiration_date,
         strlen(expiration_date), ":", (size_t) 1, s_secret, strlen(s_secret),
         NULL);
  snprintf(ssid, ssid_size, "%s|%s|%s", user_name, expiration_date, hash);
}

static int check_auth(struct http_message *hm) {
  int authenticated = 0;
  char ssid[100], calculated_ssid[100], name[100], expire[100];

  if (mg_vcmp(&hm->uri, s_login_uri) == 0) {
    // Always authenticate requests to login page
    authenticated = 1;
  } else {
    // Look for session ID in the Cookie.
    // That session ID can be validated against the database that stores
    // current active sessions.
    mg_http_parse_header(mg_get_http_header(hm, "Cookie"), "ssid", ssid,
                         sizeof(ssid));
    if (sscanf(ssid, "%[^|]|%[^|]|", name, expire) == 2) {
      generate_ssid(name, expire, calculated_ssid, sizeof(calculated_ssid));
      if (strcmp(ssid, calculated_ssid) == 0) {
        authenticated = 1;
      }
    }
  }

  return authenticated;
}

static void check_login_form_submission(struct mg_connection *c,
                                        struct http_message *hm) {
  char name[100], password[100], ssid[100], expire[100], expire_epoch[100];

  mg_get_http_var(&hm->body, "name", name, sizeof(name));
  mg_get_http_var(&hm->body, "password", password, sizeof(password));

  // A real authentication mechanism should be employed here.
  // Also, the whole site should be served through HTTPS.
  if (strcmp(name, "Joe") == 0 && strcmp(password, "Doe") == 0) {
    // Generate expiry date
    time_t t = time(NULL) + 3600;  // Valid for 1 hour
    snprintf(expire_epoch, sizeof(expire_epoch), "%lu", (unsigned long) t);
    strftime(expire, sizeof(expire), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&t));
    generate_ssid(name, expire_epoch, ssid, sizeof(ssid));

    // Set "session id" cookie, there could be some data encoded in it.
    mg_printf(c,
              "HTTP/1.1 302 Moved\r\n"
              "Set-Cookie: ssid=%s; expire=\"%s\"; http-only; HttpOnly;\r\n"
              "Location: /\r\n\r\n",
              ssid, expire);
  } else {
    mg_printf(c, "%s", "HTTP/1.1 302 Moved\r\nLocation: /\r\n\r\n");
  }
}

static void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_REQUEST) {
    struct http_message *hm = (struct http_message *) ev_data;

    if (mg_vcmp(&hm->uri, s_login_uri) == 0 &&
        mg_vcmp(&hm->method, "POST") == 0) {
      check_login_form_submission(c, hm);
    } else if (check_auth(hm)) {
      // Authenticated. Serve files.
      mg_serve_http(c, (struct http_message *) ev_data, s_http_server_opts);
    } else {
      // Not authenticated. Redirect to the login page.
      mg_printf(c, "HTTP/1.1 302 Moved\r\nLocation: %s\r\n\r\n", s_login_uri);
    }
  }
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *c;

  mg_mgr_init(&mgr, NULL);
  c = mg_bind(&mgr, s_http_port, ev_handler);

  if (c == NULL) {
    printf("Cannot start server on port %s\n", s_http_port);
    exit(1);
  }

  // Set up HTTP server parameters
  mg_set_protocol_http_websocket(c);
  s_http_server_opts.document_root = ".";  // Serve current directory

  printf("Starting web server on port %s\n", s_http_port);
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
