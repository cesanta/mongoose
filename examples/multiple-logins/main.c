// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

// Authenticated user.
// A user can be authenticated by:
//   - a name:pass pair
//   - a token
// When a user is shown a login screen, she enters a user:pass. If successful,
// a server returns user info which includes token. From that point on,
// client can use token for authentication. Tokens could be refreshed/changed
// on a server side, forcing clients to re-login.
struct user {
  const char *name, *pass, *token;
};

// Parse HTTP requests, return authenticated user or NULL
static struct user *getuser(struct mg_http_message *hm) {
  // In production, make passwords strong and tokens randomly generated
  // In this example, user list is kept in RAM. In production, it can
  // be backed by file, database, or some other method.
  static struct user users[] = {
      {"admin", "admin", "admin_token"},
      {"user1", "pass1", "user1_token"},
      {"user2", "pass2", "user2_token"},
      {NULL, NULL, NULL},
  };
  char user[256], pass[256];
  struct user *u;
  mg_http_creds(hm, user, sizeof(user), pass, sizeof(pass));
  if (user[0] != '\0' && pass[0] != '\0') {
    // Both user and password is set, search by user/password
    for (u = users; u->name != NULL; u++)
      if (strcmp(user, u->name) == 0 && strcmp(pass, u->pass) == 0) return u;
  } else if (user[0] == '\0') {
    // Only password is set, search by token
    for (u = users; u->name != NULL; u++)
      if (strcmp(pass, u->token) == 0) return u;
  }
  return NULL;
}

// HTTP request handler function
static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct user *u = getuser(hm);
    // LOG(LL_INFO, ("[%.*s] auth %s", (int) hm->uri.len, hm->uri.ptr,
    // u ? u->name : "NULL"));
    if (u == NULL && mg_http_match_uri(hm, "/api/#")) {
      // All URIs starting with /api/ must be authenticated
      mg_printf(c, "%s", "HTTP/1.1 403 Denied\r\nContent-Length: 0\r\n\r\n");
    } else if (mg_http_match_uri(hm, "/api/user/data")) {
      // Data visible to both users and administrators
      mg_printf(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
      mg_http_printf_chunk(c, "{\"data\":\"%s\"}\n", "this is user data");
      mg_http_printf_chunk(c, "");
    } else if (mg_http_match_uri(hm, "/api/admin/data")) {
      // Data visible only to administrators
      if (strcmp(u->name, "admin") == 0) {
        mg_printf(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
        mg_http_printf_chunk(c, "{\"data\":\"%s\"}\n", "this is admin data");
        mg_http_printf_chunk(c, "");
      } else {
        mg_printf(c, "%s", "HTTP/1.1 403 Denied\r\nContent-Length: 0\r\n\r\n");
      }
    } else if (mg_http_match_uri(hm, "/api/login")) {
      mg_printf(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
      mg_http_printf_chunk(c, "{\"user\":\"%s\",\"token\":\"%s\"}\n", u->name,
                           u->token);
      mg_http_printf_chunk(c, "");
    } else {
      struct mg_http_serve_opts opts = {.root_dir = "web_root"};
      mg_http_serve_dir(c, ev_data, &opts);
    }
  }
}

int main(void) {
  struct mg_mgr mgr;

  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://localhost:8000", cb, NULL);

  for (;;) mg_mgr_poll(&mgr, 1000);
  mg_mgr_free(&mgr);

  return 0;
}
