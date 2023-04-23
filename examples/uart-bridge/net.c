// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

#define DEFAULT_TCP "tcp://0.0.0.0:4001"
#define DEFAULT_WEBSOCKET "ws://0.0.0.0:4002"
#define DEFAULT_MQTT "mqtt://broker.hivemq.com:1883?tx=b/tx&rx=b/rx"

struct endpoint {
  char *url;
  bool enable;
  struct mg_connection *c;
};

struct state {
  struct endpoint tcp, websocket, mqtt;
  int tx, rx, baud;
} s_state = {.tcp = {.enable = true},
             .websocket = {.enable = true},
             .mqtt = {.enable = false},
             .tx = 5,
             .rx = 4,
             .baud = 115200};

void uart_init(int tx, int rx, int baud);
int uart_read(void *buf, size_t len);
void uart_write(const void *buf, size_t len);
char *config_read(void);
void config_write(struct mg_str config);

// Let users define their own UART API. If they don't, use a dummy one
#if defined(UART_API_IMPLEMENTED)
#else
void uart_init(int tx, int rx, int baud) {
  // We use stdin/stdout as UART. Make stdin non-blocking
#if MG_ARCH != MG_ARCH_WIN32
  fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
#endif
  (void) tx, (void) rx, (void) baud;
}

void uart_write(const void *buf, size_t len) {
  fwrite(buf, 1, len, stdout);  // Write to stdout
  fflush(stdout);
}

int uart_read(void *buf, size_t len) {
#if MG_ARCH == MG_ARCH_WIN32
  (void) buf, (void) len;
  return 0;
#else
  return read(0, buf, len);  // Read from stdin
#endif
}

char *config_read(void) {
  return mg_file_read(&mg_fs_posix, "config.json", NULL);
}

void config_write(struct mg_str config) {
  mg_file_write(&mg_fs_posix, "config.json", config.ptr, config.len);
}
#endif

// Event handler for a connected Websocket client
static void ws_fn(struct mg_connection *c, int ev, void *evd, void *fnd) {
  if (ev == MG_EV_HTTP_MSG) {
    mg_ws_upgrade(c, evd, NULL);
  } else if (ev == MG_EV_WS_OPEN) {
    // c->is_hexdumping = 1;
    c->data[0] = 'W';  // When WS handhake is done, mark us as WS client
  } else if (ev == MG_EV_WS_MSG) {
    struct mg_ws_message *wm = (struct mg_ws_message *) evd;
    uart_write(wm->data.ptr, wm->data.len);  // Send to UART
    c->recv.len = 0;                         // Discard received data
  } else if (ev == MG_EV_CLOSE) {
    if (c->is_listening) s_state.websocket.c = NULL;
  }
  (void) fnd;
}

// Event handler for a connected TCP client
static void tcp_fn(struct mg_connection *c, int ev, void *evd, void *fnd) {
  if (ev == MG_EV_ACCEPT) {
    // c->is_hexdumping = 1;
    c->data[0] = 'T';  // When client is connected, mark us as TCP client
  } else if (ev == MG_EV_READ) {
    uart_write(c->recv.buf, c->recv.len);  // Send to UART
    c->recv.len = 0;                       // Discard received data
  } else if (ev == MG_EV_CLOSE) {
    if (c->is_listening) s_state.tcp.c = NULL;
  }
  (void) fnd, (void) evd;
}

// Extract topic name from the MQTT address
static struct mg_str mqtt_topic(const char *name, const char *dflt) {
  struct mg_str qs = mg_str(strchr(s_state.mqtt.url, '?'));
  struct mg_str v = mg_http_var(qs, mg_str(name));
  return v.ptr == NULL ? mg_str(dflt) : v;
}

// Event handler for MQTT connection
static void mq_fn(struct mg_connection *c, int ev, void *evd, void *fnd) {
  if (ev == MG_EV_OPEN) {
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_MQTT_OPEN) {
    c->data[0] = 'M';
    struct mg_mqtt_opts sub_opts;
    memset(&sub_opts, 0, sizeof(sub_opts));
    sub_opts.topic = mqtt_topic("rx", "b/rx");
    sub_opts.qos = 1;
    mg_mqtt_sub(c, &sub_opts);  // Subscribe to RX topic
  } else if (ev == MG_EV_MQTT_MSG) {
    struct mg_mqtt_message *mm = evd;        // MQTT message
    uart_write(mm->data.ptr, mm->data.len);  // Send to UART
  } else if (ev == MG_EV_CLOSE) {
    s_state.mqtt.c = NULL;
  }
  (void) fnd, (void) evd;
}

// Software timer with a frequency close to the scheduling time slot
static void timer_fn(void *param) {
  // Start listeners if they're stopped for any reason
  struct mg_mgr *mgr = (struct mg_mgr *) param;
  if (s_state.tcp.c == NULL && s_state.tcp.enable) {
    s_state.tcp.c = mg_listen(mgr, s_state.tcp.url, tcp_fn, 0);
  }
  if (s_state.websocket.c == NULL && s_state.websocket.enable) {
    s_state.websocket.c = mg_http_listen(mgr, s_state.websocket.url, ws_fn, 0);
  }
  if (s_state.mqtt.c == NULL && s_state.mqtt.enable) {
    struct mg_mqtt_opts opts = {.clean = true};
    s_state.mqtt.c = mg_mqtt_connect(mgr, s_state.mqtt.url, &opts, mq_fn, 0);
  }

  // Read UART
  char buf[512];
  int len = uart_read(buf, sizeof(buf));
  if (len > 0) {
    // Iterate over all connections. Send data to WS and TCP clients
    for (struct mg_connection *c = mgr->conns; c != NULL; c = c->next) {
      if (c->data[0] == 'W') mg_ws_send(c, buf, len, WEBSOCKET_OP_TEXT);
      if (c->data[0] == 'T') mg_send(c, buf, len);
      if (c->data[0] == 'M') {
        struct mg_mqtt_opts pub_opts;
        memset(&pub_opts, 0, sizeof(pub_opts));
        pub_opts.topic = mqtt_topic("tx", "b/tx");
        pub_opts.message = mg_str_n(buf, len);
        pub_opts.qos = 1, pub_opts.retain = false;
        mg_mqtt_pub(c, &pub_opts);
      }
    }
  }
}

static void update_string(struct mg_str json, const char *path, char **value) {
  char *jval;
  if ((jval = mg_json_get_str(json, path)) != NULL) {
    free(*value);
    *value = strdup(jval);
  }
}

static void config_apply(struct mg_str s) {
  MG_INFO(("Applying config: %.*s", (int) s.len, s.ptr));

  bool b;
  if (mg_json_get_bool(s, "$.tcp.enable", &b)) s_state.tcp.enable = b;
  if (mg_json_get_bool(s, "$.ws.enable", &b)) s_state.websocket.enable = b;
  if (mg_json_get_bool(s, "$.mqtt.enable", &b)) s_state.mqtt.enable = b;

  update_string(s, "$.tcp.url", &s_state.tcp.url);
  update_string(s, "$.mqtt.url", &s_state.mqtt.url);
  update_string(s, "$.ws.url", &s_state.websocket.url);

  double v;
  if (mg_json_get_num(s, "$.rx", &v)) s_state.rx = (int) v;
  if (mg_json_get_num(s, "$.tx", &v)) s_state.tx = (int) v;
  if (mg_json_get_num(s, "$.baud", &v)) s_state.baud = (int) v;

  if (s_state.mqtt.c) s_state.mqtt.c->is_closing = 1;
  if (s_state.tcp.c) s_state.tcp.c->is_closing = 1;
  if (s_state.websocket.c) s_state.websocket.c->is_closing = 1;
}

// HTTP request handler function
void uart_bridge_fn(struct mg_connection *c, int ev, void *ev_data,
                    void *fn_data) {
  if (ev == MG_EV_OPEN && c->is_listening) {
    char *config = config_read();
    if (config != NULL) config_apply(mg_str(config));
    free(config);
    s_state.tcp.url = strdup(DEFAULT_TCP);
    s_state.websocket.url = strdup(DEFAULT_WEBSOCKET);
    s_state.mqtt.url = strdup(DEFAULT_MQTT);
    mg_timer_add(c->mgr, 20, MG_TIMER_REPEAT, timer_fn, c->mgr);
    uart_init(s_state.tx, s_state.rx, s_state.baud);
    // mg_log_set(MG_LL_DEBUG);                  // Set log level
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/api/hi")) {
      mg_http_reply(c, 200, "", "hi\n");  // Testing endpoint
    } else if (mg_http_match_uri(hm, "/api/config/set")) {
      config_apply(hm->body);
      config_write(hm->body);
      mg_http_reply(c, 200, "", "true\n");
    } else if (mg_http_match_uri(hm, "/api/config/get")) {
      mg_http_reply(c, 200, "Content-Type: application/json\r\n",
                    "{%m:{%m:%m,%m:%s},%m:{%m:%m,%m:%s},%m:{%m:%m,%m:%s},"
                    "%m:%d,%m:%d,%m:%d}\n",
                    mg_print_esc, 0, "tcp", mg_print_esc, 0, "url",
                    mg_print_esc, 0, s_state.tcp.url, mg_print_esc, 0, "enable",
                    s_state.tcp.enable ? "true" : "false", mg_print_esc, 0,
                    "ws", mg_print_esc, 0, "url", mg_print_esc, 0,
                    s_state.websocket.url, mg_print_esc, 0, "enable",
                    s_state.websocket.enable ? "true" : "false", mg_print_esc,
                    0, "mqtt", mg_print_esc, 0, "url", mg_print_esc, 0,
                    s_state.mqtt.url, mg_print_esc, 0, "enable",
                    s_state.mqtt.enable ? "true" : "false", mg_print_esc, 0,
                    "rx", s_state.rx, mg_print_esc, 0, "tx", s_state.tx,
                    mg_print_esc, 0, "baud", s_state.baud);
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
  }
  (void) fn_data;
}
