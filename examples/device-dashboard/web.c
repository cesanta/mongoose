// Copyright (c) 2020-2022 Cesanta Software Limited
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

// This is a configuration structure we're going to show on a dashboard
static struct config {
  int value1;
  char *value2;
} s_config = {123, NULL};

// Update config structure. Return true if changed, false otherwise
static bool update_config(struct mg_http_message *hm, struct config *cfg) {
  bool changed = false;
  char buf[256];
  if (mg_http_get_var(&hm->body, "value1", buf, sizeof(buf)) > 0) {
    cfg->value1 = atoi(buf);
    changed = true;
  }
  if (mg_http_get_var(&hm->body, "value2", buf, sizeof(buf)) > 0) {
    free(cfg->value2);
    cfg->value2 = malloc(strlen(buf) + 1);
    strcpy(cfg->value2, buf);
    changed = true;
  }
  return changed;
}

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

// Notify all config watchers about the config change
static void send_notification(struct mg_mgr *mgr, const char *name,
                              const char *data) {
  struct mg_connection *c;
  for (c = mgr->conns; c != NULL; c = c->next) {
    if (c->label[0] == 'W')
      mg_http_printf_chunk(c, "{\"name\": \"%s\", \"data\": \"%s\"}", name,
                           data == NULL ? "" : data);
  }
}

// HTTP request handler function
void device_dashboard_fn(struct mg_connection *c, int ev, void *ev_data,
                         void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct user *u = getuser(hm);
    // MG_INFO(("%p [%.*s] auth %s", c->fd, (int) hm->uri.len, hm->uri.ptr,
    // u ? u->name : "NULL"));
    if (mg_http_match_uri(hm, "/api/hi")) {
      mg_http_reply(c, 200, "", "hi\n");  // Testing endpoint
    } else if (u == NULL && mg_http_match_uri(hm, "/api/#")) {
      // All URIs starting with /api/ must be authenticated
      mg_printf(c, "%s", "HTTP/1.1 403 Denied\r\nContent-Length: 0\r\n\r\n");
    } else if (mg_http_match_uri(hm, "/api/config/get")) {
      mg_printf(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
      mg_http_printf_chunk(c, "{\"%s\":%d,\"%s\":\"%s\"}", "value1",
                           s_config.value1, "value2", s_config.value2);
      mg_http_printf_chunk(c, "");
    } else if (mg_http_match_uri(hm, "/api/config/set")) {
      // Admins only
      if (strcmp(u->name, "admin") == 0) {
        if (update_config(hm, &s_config))
          send_notification(fn_data, "config", NULL);
        mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
      } else {
        mg_printf(c, "%s", "HTTP/1.1 403 Denied\r\nContent-Length: 0\r\n\r\n");
      }
    } else if (mg_http_match_uri(hm, "/api/message/send")) {
      char buf[256];
      if (mg_http_get_var(&hm->body, "message", buf, sizeof(buf)) > 0) {
        send_notification(fn_data, "message", buf);
      }
      mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
    } else if (mg_http_match_uri(hm, "/api/watch")) {
      c->label[0] = 'W';  // Mark ourselves as a event listener
      mg_printf(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    } else if (mg_http_match_uri(hm, "/api/login")) {
      mg_printf(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
      mg_http_printf_chunk(c, "{\"user\":\"%s\",\"token\":\"%s\"}\n", u->name,
                           u->token);
      mg_http_printf_chunk(c, "");
    } else {
      struct mg_http_serve_opts opts = {0};
#if 1
      opts.root_dir = "/web_root";
      opts.fs = &mg_fs_packed;
#else
      opts.root_dir = "web_root";
#endif
      mg_http_serve_dir(c, ev_data, &opts);
    }
  }
}
