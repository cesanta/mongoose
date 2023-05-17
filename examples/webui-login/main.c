// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

const char *s_listening_url = "http://0.0.0.0:8000";

// Authenticated user.
// A user can be authenticated by:
//   - a name:pass pair
//   - a token
// When a user is shown a login screen, they enter a user:pass. If successful,
// the server returns the user info, which includes the token. From that point
// on, the client can use the token for authentication. Tokens could be
// refreshed/changed at server side, forcing clients to re-login.
struct user {
  const char *name, *pass, *token;
};

// Parse HTTP requests, return authenticated user or NULL
static struct user *getuser(struct mg_http_message *hm) {
  // In production, make passwords strong and tokens randomly generated
  // In this example, user list is kept in RAM. In production, it can
  // be backed by file, database, or some other method.
  static struct user users[] = {
      {"admin", "pass0", "admin_token"},
      {"user1", "pass1", "user1_token"},
      {"user2", "pass2", "user2_token"},
      {NULL, NULL, NULL},
  };
  char user[256], pass[256];
  struct user *u;
  mg_http_creds(hm, user, sizeof(user), pass, sizeof(pass));
  if (user[0] != '\0' && pass[0] != '\0') {
    // Both user and password are set, search by user/password
    for (u = users; u->name != NULL; u++)
      if (strcmp(user, u->name) == 0 && strcmp(pass, u->pass) == 0) return u;
  } else if (user[0] == '\0') {
    // Only password is set, search by token
    for (u = users; u->name != NULL; u++)
      if (strcmp(pass, u->token) == 0) return u;
  }
  return NULL;
}

void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct user *u = getuser(hm);
    if (u == NULL && mg_http_match_uri(hm, "/api/#")) {
      // All URIs starting with /api/ must be authenticated
      mg_http_reply(c, 403, "", "Denied\n");
    } else if (mg_http_match_uri(hm, "/api/data")) {
      mg_http_reply(c, 200, "Content-Type: application/json\r\n",
                    "{%m:%m,%m:%m}\n", MG_ESC("text"), MG_ESC("Hello!"),
                    MG_ESC("data"), MG_ESC("somedata"));
    } else if (mg_http_match_uri(hm, "/api/login")) {
      mg_http_reply(c, 200, "Content-Type: application/json\r\n",
                    "{%m:%m,%m:%m}\n", MG_ESC("user"), MG_ESC(u->name),
                    MG_ESC("token"), MG_ESC(u->token));
    } else {
      struct mg_http_serve_opts opts = {.root_dir = "web_root"};
      mg_http_serve_dir(c, ev_data, &opts);
    }
  }
  (void) fn_data;
}

int main(void) {
  struct mg_mgr mgr;
  mg_log_set(MG_LL_DEBUG);
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, s_listening_url, fn, &mgr);
  while (mgr.conns != NULL) mg_mgr_poll(&mgr, 500);
  mg_mgr_free(&mgr);
  return 0;
}
