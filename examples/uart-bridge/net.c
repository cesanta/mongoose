// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

static const char *s_tcp_url = "tcp://0.0.0.0:4001";
static const char *s_ws_url = "ws://0.0.0.0:4002";
static struct mg_connection *s_tcp_conn;
static struct mg_connection *s_ws_conn;
static int s_rx = 4;
static int s_tx = 5;
static int s_baud = 115200;

void uart_init(int tx, int rx, int baud);
int uart_read(char *buf, size_t len);
void uart_write(const void *buf, size_t len);

// Let users define their own UART API. If they don't, use a dummy one
#if defined(UART_API_IMPLEMENTED)
#else
void uart_init(int tx, int rx, int baud) {
  // We use stdin/stdout as UART. Make stdin non-blocking
  fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
  (void) tx, (void) rx, (void) baud;
}

void uart_write(const void *buf, size_t len) {
  fwrite(buf, 1, len, stdout);  // Write to stdout
  fflush(stdout);
}

int uart_read(char *buf, size_t len) {
  return read(0, buf, len);  // Read from stdin
}
#endif

// Event handler for a connected Websocket client
static void ws_fn(struct mg_connection *c, int ev, void *evd, void *fnd) {
  if (ev == MG_EV_HTTP_MSG) {
    mg_ws_upgrade(c, evd, NULL);
  } else if (ev == MG_EV_WS_OPEN) {
    // c->is_hexdumping = 1;
    c->label[0] = 'W';  // When WS handhake is done, mark us as WS client
  } else if (ev == MG_EV_WS_MSG) {
    struct mg_ws_message *wm = (struct mg_ws_message *) evd;
    uart_write(wm->data.ptr, wm->data.len);  // Send to UART
    c->recv.len = 0;                         // Discard received data
  } else if (ev == MG_EV_CLOSE) {
    if (c->is_listening) s_ws_conn = NULL;
  }
  (void) fnd;
}

// Event handler for a connected TCP client
static void tcp_fn(struct mg_connection *c, int ev, void *evd, void *fnd) {
  if (ev == MG_EV_OPEN) {
    // c->is_hexdumping = 1;
    c->label[0] = 'T';  // When client is connected, mark us as TCP client
  } else if (ev == MG_EV_READ) {
    uart_write(c->recv.buf, c->recv.len);  // Send to UART
    c->recv.len = 0;                       // Discard received data
  } else if (ev == MG_EV_CLOSE) {
    if (c->is_listening) s_tcp_conn = NULL;
  }
  (void) fnd, (void) evd;
}

// Software timer with a frequency close to the scheduling time slot
static void timer_fn(void *param) {
  // Start listeners if they're stopped for any reason
  struct mg_mgr *mgr = (struct mg_mgr *) param;
  if (s_tcp_conn == NULL) s_tcp_conn = mg_listen(mgr, s_tcp_url, tcp_fn, 0);
  if (s_ws_conn == NULL) s_ws_conn = mg_http_listen(mgr, s_ws_url, ws_fn, 0);

  // Read UART
  char buf[512];
  int len = uart_read(buf, sizeof(buf));
  if (len > 0) {
    // Iterate over all connections. Send data to WS and TCP clients
    for (struct mg_connection *c = mgr->conns; c != NULL; c = c->next) {
      if (c->label[0] == 'W') mg_ws_send(c, buf, len, WEBSOCKET_OP_TEXT);
      if (c->label[0] == 'T') mg_send(c, buf, len);
    }
  }
}

// HTTP request handler function
void uart_bridge_fn(struct mg_connection *c, int ev, void *ev_data,
                    void *fn_data) {
  if (ev == MG_EV_OPEN && c->is_listening) {
    mg_timer_add(c->mgr, 20, MG_TIMER_REPEAT, timer_fn, c->mgr);
    uart_init(s_tx, s_rx, s_baud);
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/api/hi")) {
      mg_http_reply(c, 200, "", "hi\n");  // Testing endpoint
    } else if (mg_http_match_uri(hm, "/api/config/get")) {
      mg_http_reply(
          c, 200, "Content-Type: application/json\r\n",
          "{\"tcp\":\"%s\",\"ws\":\"%s\",\"rx\":%d,\"tx\":%d,\"baud\":%d}\n",
          s_tcp_url, s_ws_url, s_rx, s_tx, s_baud);
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
  (void) fn_data;
}
