// Copyright (c) 2020-2022 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

#if !defined(MQTT_SERVER)
#define MQTT_SERVER "mqtt://broker.hivemq.com:1883"
#endif
#define MQTT_PUBLISH_TOPIC "mg/my_device"
#define MQTT_SUBSCRIBE_TOPIC "mg/#"

static time_t s_boot_timestamp = 0;               // Updated by SNTP
#ifndef DISABLE_ROUTING
static struct mg_connection *s_sntp_conn = NULL;  // SNTP connection
#endif

// Define a system time alternative
time_t ourtime(time_t *tp) {
  time_t t = s_boot_timestamp + (time_t) (mg_millis() / 1000);
  if (tp != NULL) *tp = t;
  return t;
}

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
static void send_notification(struct mg_mgr *mgr, const char *fmt, ...) {
  struct mg_connection *c;
  for (c = mgr->conns; c != NULL; c = c->next) {
    if (c->data[0] == 'W') {
      va_list ap;
      va_start(ap, fmt);
      mg_ws_vprintf(c, WEBSOCKET_OP_TEXT, fmt, &ap);
      va_end(ap);
    }
  }
}

// Send simulated metrics data to the dashboard, for chart rendering
static void timer_metrics_fn(void *param) {
  send_notification(param, "{%Q:%Q,%Q:[%lu, %d]}", "name", "metrics", "data",
                    (unsigned long) ourtime(NULL),
                    10 + (int) ((double) rand() * 10 / RAND_MAX));
}

#ifndef DISABLE_ROUTING

// MQTT event handler function
static void mqtt_fn(struct mg_connection *c, int ev, void *ev_data, void *fnd) {
  if (ev == MG_EV_CONNECT && mg_url_is_ssl(s_config.url)) {
    struct mg_tls_opts opts = {.ca = "ca.pem",
                               .srvname = mg_url_host(s_config.url)};
    mg_tls_init(c, &opts);
  } else if (ev == MG_EV_MQTT_OPEN) {
    s_connected = true;
    c->is_hexdumping = 1;
    mg_mqtt_sub(s_mqtt, mg_str(s_config.sub), 2);
    send_notification(c->mgr, "{%Q:%Q,%Q:null}", "name", "config", "data");
    MG_INFO(("MQTT connected, server %s", MQTT_SERVER));
  } else if (ev == MG_EV_MQTT_MSG) {
    struct mg_mqtt_message *mm = ev_data;
    send_notification(c->mgr, "{%Q:%Q,%Q:{%Q: %.*Q, %Q: %.*Q, %Q: %d}}", "name",
                      "message", "data", "topic", (int) mm->topic.len,
                      mm->topic.ptr, "data", (int) mm->data.len, mm->data.ptr,
                      "qos", (int) mm->qos);
  } else if (ev == MG_EV_MQTT_CMD) {
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    MG_DEBUG(("%lu cmd %d qos %d", c->id, mm->cmd, mm->qos));
  } else if (ev == MG_EV_CLOSE) {
    s_mqtt = NULL;
    if (s_connected) {
      s_connected = false;
      send_notification(c->mgr, "{%Q:%Q,%Q:null}", "name", "config", "data");
    }
  }
  (void) fnd;
}

// Keep MQTT connection open - reconnect if closed
static void timer_mqtt_fn(void *param) {
  struct mg_mgr *mgr = (struct mg_mgr *) param;
  if (s_mqtt == NULL) {
    struct mg_mqtt_opts opts;
    memset(&opts, 0, sizeof(opts));
    s_mqtt = mg_mqtt_connect(mgr, s_config.url, &opts, mqtt_fn, NULL);
  }
}

// SNTP connection event handler. When we get a response from an SNTP server,
// adjust s_boot_timestamp. We'll get a valid time from that point on
static void sfn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_SNTP_TIME) {
    uint64_t t = *(uint64_t *) ev_data;
    s_boot_timestamp = (time_t) ((t - mg_millis()) / 1000);
    c->is_closing = 1;
  } else if (ev == MG_EV_CLOSE) {
    s_sntp_conn = NULL;
  }
  (void) fn_data;
}

static void timer_sntp_fn(void *param) {  // SNTP timer function. Sync up time
  struct mg_mgr *mgr = (struct mg_mgr *) param;
  if (s_sntp_conn == NULL && s_boot_timestamp == 0) {
    s_sntp_conn = mg_sntp_connect(mgr, NULL, sfn, NULL);
  }
}

#endif

// HTTP request handler function
void device_dashboard_fn(struct mg_connection *c, int ev, void *ev_data,
                         void *fn_data) {
  if (ev == MG_EV_OPEN && c->is_listening) {
    mg_timer_add(c->mgr, 1000, MG_TIMER_REPEAT, timer_metrics_fn, c->mgr);
#ifndef DISABLE_ROUTING
    mg_timer_add(c->mgr, 1000, MG_TIMER_REPEAT, timer_mqtt_fn, c->mgr);
    mg_timer_add(c->mgr, 1000, MG_TIMER_REPEAT, timer_sntp_fn, c->mgr);
#endif
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
    } else if (mg_http_match_uri(hm, "/api/debug")) {
      int level = mg_json_get_long(hm->body, "$.level", MG_LL_DEBUG);
      mg_log_set(level);
      mg_http_reply(c, 200, "", "Debug level set to %d\n", level);
    } else if (u == NULL && mg_http_match_uri(hm, "/api/#")) {
      // All URIs starting with /api/ must be authenticated
      mg_http_reply(c, 403, "", "Denied\n");
    } else if (mg_http_match_uri(hm, "/api/config/get")) {
#ifdef DISABLE_ROUTING
      mg_http_reply(c, 200, NULL, "{%Q:%Q,%Q:%Q,%Q:%Q}\n", "url", s_config.url,
                    "pub", s_config.pub, "sub", s_config.sub);
#else
      mg_http_reply(c, 200, NULL, "{%Q:%Q,%Q:%Q,%Q:%Q,%Q:%s}\n", "url",
                    s_config.url, "pub", s_config.pub, "sub", s_config.sub,
                    "connected", s_connected ? "true" : "false");
#endif
    } else if (mg_http_match_uri(hm, "/api/config/set")) {
      // Admins only
      if (strcmp(u->name, "admin") == 0) {
        update_config(&hm->body, "url", &s_config.url);
        update_config(&hm->body, "pub", &s_config.pub);
        update_config(&hm->body, "sub", &s_config.sub);
        if (s_mqtt) s_mqtt->is_closing = 1;  // Ask to disconnect from MQTT
        send_notification(fn_data, "{%Q:%Q,%Q:null}", "name", "config", "data");
        mg_http_reply(c, 200, "", "ok\n");
      } else {
        mg_http_reply(c, 403, "", "Denied\n");
      }
    } else if (mg_http_match_uri(hm, "/api/message/send")) {
      char buf[256];
      if (s_connected &&
          mg_http_get_var(&hm->body, "message", buf, sizeof(buf)) > 0) {
        mg_mqtt_pub(s_mqtt, mg_str(s_config.pub), mg_str(buf), 1, false);
      }
      mg_http_reply(c, 200, "", "ok\n");
    } else if (mg_http_match_uri(hm, "/api/watch")) {
      c->data[0] = 'W';  // Mark ourselves as a event listener
      mg_ws_upgrade(c, hm, NULL);
    } else if (mg_http_match_uri(hm, "/api/login")) {
      mg_http_reply(c, 200, NULL, "{%Q:%Q,%Q:%Q}\n", "user", u->name, "token",
                    u->token);
    } else {
      struct mg_http_serve_opts opts;
      memset(&opts, 0, sizeof(opts));
#if 1
      opts.root_dir = "/web_root";
      opts.fs = &mg_fs_packed;
#else
      opts.root_dir = "web_root";
#endif
      mg_http_serve_dir(c, ev_data, &opts);
    }
    MG_DEBUG(("%lu %.*s %.*s -> %.*s", c->id, (int) hm->method.len,
              hm->method.ptr, (int) hm->uri.len, hm->uri.ptr, (int) 3,
              &c->send.buf[9]));
  }
}
