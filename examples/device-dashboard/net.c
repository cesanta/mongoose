// Copyright (c) 2020-2022 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

#define MQTT_SERVER "mqtt://broker.hivemq.com:1883"
#define MQTT_PUBLISH_TOPIC "mg/my_device"
#define MQTT_SUBSCRIBE_TOPIC "mg/#"

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
  char *url, *pub, *sub;  // MQTT settings
} s_config;

static struct mg_connection *s_mqtt = NULL;  // MQTT connection
static bool s_connected = false;             // MQTT connection established

// Try to update a single configuration value
static void update_config(struct mg_str *body, const char *name, char **value) {
  char buf[256];
  if (mg_http_get_var(body, name, buf, sizeof(buf)) > 0) {
    free(*value);
    *value = strdup(buf);
  }
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
    if (c->label[0] != 'W') continue;
    mg_ws_printf(c, WEBSOCKET_OP_TEXT, "{%Q:%Q,%Q:%s}", "name", name, "data",
                 data);
  }
}

// Send simulated metrics data to the dashboard, for chart rendering
static void timer_metrics_fn(void *param) {
  char buf[50];
  mg_snprintf(buf, sizeof(buf), "[%lu, %d]", (unsigned long) time(NULL),
              10 + (int) ((double) rand() * 10 / RAND_MAX));
  send_notification(param, "metrics", buf);
}

// MQTT event handler function
static void mqtt_fn(struct mg_connection *c, int ev, void *ev_data, void *fnd) {
  if (ev == MG_EV_MQTT_OPEN) {
    s_connected = true;
    c->is_hexdumping = 1;
    mg_mqtt_sub(s_mqtt, mg_str(s_config.sub), 2);
    send_notification(c->mgr, "config", "null");
  } else if (ev == MG_EV_MQTT_MSG) {
    struct mg_mqtt_message *mm = ev_data;
    char buf[100], *p = buf;
    mg_asprintf(&p, sizeof(buf), "{%Q: %.*Q, %Q: %.*Q, %Q: %d}", "topic",
                (int) mm->topic.len, mm->topic.ptr, "data", (int) mm->data.len,
                mm->data.ptr, "qos", (int) mm->qos);
    send_notification(c->mgr, "message", p);
    if (p != buf) free(p);
  } else if (ev == MG_EV_MQTT_CMD) {
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    MG_DEBUG(("cmd %d qos %d", mm->cmd, mm->qos));
  } else if (ev == MG_EV_CLOSE) {
    s_mqtt = NULL;
    if (s_connected) {
      s_connected = false;
      send_notification(c->mgr, "config", "null");
    }
  }
  (void) fnd;
}

// Keep MQTT connection open - reconnect if closed
static void timer_mqtt_fn(void *param) {
  struct mg_mgr *mgr = (struct mg_mgr *) param;
  if (s_mqtt == NULL) {
    struct mg_mqtt_opts opts = {};
    s_mqtt = mg_mqtt_connect(mgr, s_config.url, &opts, mqtt_fn, NULL);
  }
}

// HTTP request handler function
void device_dashboard_fn(struct mg_connection *c, int ev, void *ev_data,
                         void *fn_data) {
  if (ev == MG_EV_OPEN && c->is_listening) {
    mg_timer_add(c->mgr, 1000, MG_TIMER_REPEAT, timer_metrics_fn, c->mgr);
    mg_timer_add(c->mgr, 1000, MG_TIMER_REPEAT, timer_mqtt_fn, c->mgr);
    s_config.url = strdup(MQTT_SERVER);
    s_config.pub = strdup(MQTT_PUBLISH_TOPIC);
    s_config.sub = strdup(MQTT_SUBSCRIBE_TOPIC);
  } else if (ev == MG_EV_HTTP_MSG) {
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
      mg_http_reply(c, 200, NULL, "{%Q:%Q,%Q:%Q,%Q:%Q,%Q:%s}\n", "url",
                    s_config.url, "pub", s_config.pub, "sub", s_config.sub,
                    "connected", s_connected ? "true" : "false");
    } else if (mg_http_match_uri(hm, "/api/config/set")) {
      // Admins only
      if (strcmp(u->name, "admin") == 0) {
        update_config(&hm->body, "url", &s_config.url);
        update_config(&hm->body, "pub", &s_config.pub);
        update_config(&hm->body, "sub", &s_config.sub);
        if (s_mqtt) s_mqtt->is_closing = 1;  // Ask to disconnect from MQTT
        send_notification(fn_data, "config", "null");
        mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
      } else {
        mg_printf(c, "%s", "HTTP/1.1 403 Denied\r\nContent-Length: 0\r\n\r\n");
      }
    } else if (mg_http_match_uri(hm, "/api/message/send")) {
      char buf[256];
      if (s_connected &&
          mg_http_get_var(&hm->body, "message", buf, sizeof(buf)) > 0) {
        mg_mqtt_pub(s_mqtt, mg_str(s_config.pub), mg_str(buf), 1, false);
      }
      mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
    } else if (mg_http_match_uri(hm, "/api/watch")) {
      c->label[0] = 'W';  // Mark ourselves as a event listener
      mg_ws_upgrade(c, hm, NULL);
      // mg_printf(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    } else if (mg_http_match_uri(hm, "/api/login")) {
      mg_http_reply(c, 200, NULL, "{%Q:%Q,%Q:%Q}\n", "user", u->name, "token",
                    u->token);
    } else {
      struct mg_http_serve_opts opts = {0};
#if 0
      opts.root_dir = "/web_root";
      opts.fs = &mg_fs_packed;
#else
      opts.root_dir = "web_root";
#endif
      mg_http_serve_dir(c, ev_data, &opts);
    }
  }
}
