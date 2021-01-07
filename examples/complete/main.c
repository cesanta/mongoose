// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

#include "mjson.h"
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

// Stringifies the config. A caller must free() it.
static char *stringify_config(struct config *cfg) {
  char *s = NULL;
  mjson_printf(mjson_print_dynamic_buf, &s, "{%Q:%d,%Q:%Q}", "value1",
               cfg->value1, "value2", cfg->value2);
  return s;
}

// Update config structure. Return true if changed, false otherwise
static bool update_config(struct mg_http_message *hm, struct config *cfg) {
  bool changed = false;
  char buf[256];
  double dv;
  if (mjson_get_number(hm->body.ptr, hm->body.len, "$.value1", &dv)) {
    s_config.value1 = dv;
    changed = true;
  }
  if (mjson_get_string(hm->body.ptr, hm->body.len, "$.value2", buf,
                       sizeof(buf)) > 0) {
    free(s_config.value2);
    s_config.value2 = strdup(buf);
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

// Notify all config watchers about the config change
static void notify_config_change(struct mg_mgr *mgr) {
  struct mg_connection *c;
  char *s = stringify_config(&s_config);
  for (c = mgr->conns; c != NULL; c = c->next) {
    if (c->label[0] == 'W') mg_http_printf_chunk(c, "%s\n", s);
  }
  free(s);
}

// HTTP request handler function
static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct user *u = getuser(hm);
    // LOG(LL_INFO, ("%p [%.*s] auth %s", c->fd, (int) hm->uri.len, hm->uri.ptr,
    // u ? u->name : "NULL"));
    if (u == NULL && mg_http_match_uri(hm, "/api/#")) {
      // All URIs starting with /api/ must be authenticated
      mg_printf(c, "%s", "HTTP/1.1 403 Denied\r\nContent-Length: 0\r\n\r\n");
    } else if (mg_http_match_uri(hm, "/api/config/get")) {
      char *s = stringify_config(&s_config);
      mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s\n",
                (int) strlen(s) + 1, s);
      free(s);
    } else if (mg_http_match_uri(hm, "/api/config/set")) {
      // Admins only
      if (strcmp(u->name, "admin") == 0) {
        if (update_config(hm, &s_config)) notify_config_change(fn_data);
        mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
      } else {
        mg_printf(c, "%s", "HTTP/1.1 403 Denied\r\nContent-Length: 0\r\n\r\n");
      }
    } else if (mg_http_match_uri(hm, "/api/config/watch")) {
      c->label[0] = 'W';  // Mark ourselves as a config watcher
      mg_printf(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    } else if (mg_http_match_uri(hm, "/api/video1")) {
      c->label[0] = 'S';  // Mark that connection as live streamer
      mg_printf(
          c, "%s",
          "HTTP/1.0 200 OK\r\n"
          "Cache-Control: no-cache\r\n"
          "Pragma: no-cache\r\nExpires: Thu, 01 Dec 1994 16:00:00 GMT\r\n"
          "Content-Type: multipart/x-mixed-replace; boundary=--foo\r\n\r\n");
    } else if (mg_http_match_uri(hm, "/api/log/static")) {
      mg_http_serve_file(c, hm, "log.txt", "text/plain", "");
    } else if (mg_http_match_uri(hm, "/api/log/live")) {
      c->label[0] = 'L';  // Mark that connection as live log listener
      mg_printf(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
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

// The image stream is simulated by sending MJPEG frames specified by the
// "files" array of file names.
static void broadcast_mjpeg_frame(struct mg_mgr *mgr) {
  const char *files[] = {"images/1.jpg", "images/2.jpg", "images/3.jpg",
                         "images/4.jpg", "images/5.jpg", "images/6.jpg"};
  size_t nfiles = sizeof(files) / sizeof(files[0]);
  static size_t i;
  const char *path = files[i++ % nfiles];
  size_t size = mg_file_size(path);
  char *data = mg_file_read(path);  // Read next file
  struct mg_connection *c;
  for (c = mgr->conns; c != NULL; c = c->next) {
    if (c->label[0] != 'S') continue;         // Skip non-stream connections
    if (data == NULL || size == 0) continue;  // Skip on file read error
    mg_printf(c,
              "--foo\r\nContent-Type: image/jpeg\r\n"
              "Content-Length: %lu\r\n\r\n",
              (unsigned long) size);
    mg_send(c, data, size);
    mg_send(c, "\r\n", 2);
  }
  free(data);
}

static void mjpeg_cb(void *arg) {
  broadcast_mjpeg_frame(arg);
}

static void log_message(const char *filename, const char *message) {
  FILE *fp = fopen(filename, "a");
  if (fp != NULL) {
    fprintf(fp, "%s", message);
    fclose(fp);
  }
}

static void broadcast_message(struct mg_mgr *mgr, const char *message) {
  struct mg_connection *c;
  for (c = mgr->conns; c != NULL; c = c->next) {
    if (c->label[0] == 'L') mg_http_printf_chunk(c, "%s", message);
  }
}

// Timer function - called periodically.
// Prepare log message. Save it to a file, and broadcast.
static void log_cb(void *arg) {
  char buf[64];
  snprintf(buf, sizeof(buf), "Time is: %lu\n", (unsigned long) time(NULL));
  log_message("log.txt", buf);
  broadcast_message(arg, buf);
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_timer t1, t2;

  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://localhost:8000", cb, &mgr);
  mg_timer_init(&t1, 500, MG_TIMER_REPEAT, mjpeg_cb, &mgr);
  mg_timer_init(&t2, 1000, MG_TIMER_REPEAT, log_cb, &mgr);
  for (;;) mg_mgr_poll(&mgr, 50);
  mg_timer_free(&t1);
  mg_timer_free(&t2);
  mg_mgr_free(&mgr);

  return 0;
}
