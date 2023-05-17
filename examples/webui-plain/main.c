// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved
//
// UI example
// It implements the following endpoints:
//    /api/config/get - respond with current config
//    /api/config/set - POST a config change
//    any other URI serves static files from s_root_dir
// Data and results are JSON strings

#include "mongoose.h"

static const char *s_http_addr = "http://localhost:8000";  // HTTP port
static const char *s_root_dir = "web_root";
#define MQTT_SERVER "mqtt://broker.hivemq.com:1883"
#define MQTT_PUBLISH_TOPIC "mg/my_device"
#define MQTT_SUBSCRIBE_TOPIC "mg/#"

static struct config {
  char *url, *pub, *sub;
} s_config;

// Try to update a single configuration value
static void update_config(struct mg_str json, const char *path, char **value) {
  char *jval;
  if ((jval = mg_json_get_str(json, path)) != NULL) {
    free(*value);
    *value = strdup(jval);
  }
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_OPEN && c->is_listening) {
    s_config.url = strdup(MQTT_SERVER);
    s_config.pub = strdup(MQTT_PUBLISH_TOPIC);
    s_config.sub = strdup(MQTT_SUBSCRIBE_TOPIC);
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/api/config/get")) {
      mg_http_reply(c, 200, "Content-Type: application/json\r\n",
                    "{%m:%m,%m:%m,%m:%m}\n", MG_ESC("url"),
                    MG_ESC(s_config.url), MG_ESC("pub"), MG_ESC(s_config.pub),
                    MG_ESC("sub"), MG_ESC(s_config.sub));
    } else if (mg_http_match_uri(hm, "/api/config/set")) {
      struct mg_str json = hm->body;
      update_config(json, "$.url", &s_config.url);
      update_config(json, "$.pub", &s_config.pub);
      update_config(json, "$.sub", &s_config.sub);
      mg_http_reply(c, 200, "", "ok\n");
    } else {
      struct mg_http_serve_opts opts = {.root_dir = s_root_dir};
      mg_http_serve_dir(c, ev_data, &opts);
    }
  }
  (void) fn_data;
}

int main(void) {
  struct mg_mgr mgr;                            // Event manager
  mg_log_set(MG_LL_INFO);                       // Set to 3 to enable debug
  mg_mgr_init(&mgr);                            // Initialise event manager
  mg_http_listen(&mgr, s_http_addr, fn, NULL);  // Create HTTP listener
  for (;;) mg_mgr_poll(&mgr, 1000);             // Infinite event loop
  mg_mgr_free(&mgr);
  return 0;
}
