// Copyright (c) 2014 Cesanta Software
// All rights reserved
//
// This example demostrates proxying of WebSocket traffic, regardless of the
// protocol (ws:// or wss://).
// To use this example:
//    1. configure your browser to use a proxy on port 2014
//    2. import certs/ws1_ca.pem and certs/ws2_ca.pem into the trusted
//       certificates list on your browser
//    3. make && ./ws_ssl
//    4. Point your browser to http://ws_ssl.com
//  A page with 4 sections should appear, showing websocket echoes

#include "net_skeleton.h"
#include "mongoose.h"
#include "ssl_wrapper.h"

#define S1_PEM  "certs/ws1_server.pem"
#define C1_PEM  "certs/ws1_client.pem"
#define CA1_PEM "certs/ws1_ca.pem"
#define S2_PEM  "certs/ws2_server.pem"
#define C2_PEM  "certs/ws2_client.pem"
#define CA2_PEM "certs/ws2_ca.pem"

struct config {
  const char *uri;
  const char *wrapper_server_addr;
  const char *wrapper_client_addr;
  const char *target_addr;
};

static struct config s_wrappers[] = {
  {
    "ws1:80",
    "tcp://127.0.0.1:7001",
    "tcp://127.0.0.1:7001",
    "tcp://127.0.0.1:9001"
  },
  {
    "ws1:443",
    "ssl://127.0.0.1:7002:" S1_PEM,
    "tcp://127.0.0.1:7002",
    "tcp://127.0.0.1:9001"
  },
  {
    "ws2:80",
    "tcp://127.0.0.1:7003",
    "tcp://127.0.0.1:7003",
    "ssl://127.0.0.1:9002:" C2_PEM ":" CA2_PEM
  },
  {
    "ws2:443",
    "ssl://127.0.0.1:7004:" S2_PEM,
    "tcp://127.0.0.1:7004",
    "ssl://127.0.0.1:9002:" C2_PEM ":" CA2_PEM
  },
};

static int s_received_signal = 0;

static void signal_handler(int sig_num) {
  signal(sig_num, signal_handler);
  s_received_signal = sig_num;
}

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  int i;

  switch (ev) {
    case MG_AUTH:
      return MG_TRUE;

    case MG_REQUEST:
      printf("==> [%s] [%s]\n", conn->request_method, conn->uri);

      if (strcmp(conn->request_method, "CONNECT") == 0) {
        // Iterate over configured wrappers, see if we can use one of them
        for (i = 0; i < (int) ARRAY_SIZE(s_wrappers); i++) {
          if (strcmp(conn->uri, s_wrappers[i].uri) == 0) {
            mg_forward(conn, s_wrappers[i].wrapper_client_addr);
            return MG_MORE;
          }
        }

        // No suitable wrappers found. Disallow that CONNECT request.
        mg_send_status(conn, 405);
        return MG_TRUE;
      }

      // Not a CONNECT request, serve HTML file.
      mg_send_file(conn, "ws_ssl.html", NULL);
      return MG_MORE;

    default:
      return MG_FALSE;
  }
}

static int ws_handler(struct mg_connection *conn, enum mg_event ev) {
  switch (ev) {
    case MG_AUTH:
      return MG_TRUE;
    case MG_REQUEST:
      if (conn->is_websocket) {
        // Simple websocket echo server
        mg_websocket_write(conn, WEBSOCKET_OPCODE_TEXT,
                           conn->content, conn->content_len);
      } else {
        mg_printf_data(conn, "%s", "websocket connection expected");
      }
      return MG_TRUE;
    default:
      return MG_FALSE;
  }
}

static void *serve_thread_func(void *param) {
  struct mg_server *server = (struct mg_server *) param;
  printf("Listening on port %s\n", mg_get_option(server, "listening_port"));
  while (s_received_signal == 0) {
    mg_poll_server(server, 1000);
  }
  mg_destroy_server(&server);
  return NULL;
}

static void *wrapper_thread_func(void *param) {
  struct config *c = (struct config *) param;
  const char *err_msg;
  void *wrapper;

  wrapper = ssl_wrapper_init(c->wrapper_server_addr, c->target_addr, &err_msg);
  if (wrapper == NULL) {
    fprintf(stderr, "Error: %s\n", err_msg);
    exit(EXIT_FAILURE);
  }
  //((struct ns_mgr *) wrapper)->hexdump_file = "/dev/stderr";
  ssl_wrapper_serve(wrapper, &s_received_signal);

  return NULL;
}

int main(void) {
  struct mg_server *proxy_server = mg_create_server(NULL, ev_handler);
  struct mg_server *ws1_server = mg_create_server(NULL, ws_handler);
  struct mg_server *ws2_server = mg_create_server(NULL, ws_handler);
  size_t i;

  ((struct ns_mgr *) proxy_server)->hexdump_file = "/dev/stderr";

  // Configure proxy server to listen on port 2014
  mg_set_option(proxy_server, "listening_port", "2014");
  //mg_set_option(proxy_server, "enable_proxy", "yes");

  // Configure two websocket echo servers:
  //    ws1 is WS, listening on 9001
  //    ws2 is WSS, listening on 9002
  // Note that HTML page thinks that ws1 is WSS, and ws2 is WS,
  // where in reality it is vice versa and proxy server makes the decision.
  mg_set_option(ws1_server, "listening_port", "tcp://127.0.0.1:9001");
  mg_set_option(ws2_server, "listening_port",
                "ssl://127.0.0.1:9002:" S2_PEM ":" CA2_PEM);

  // Setup signal handlers
  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);

  // Start SSL wrappers, each in it's own thread
  for (i = 0; i < ARRAY_SIZE(s_wrappers); i++) {
    ns_start_thread(wrapper_thread_func, &s_wrappers[i]);
  }

  // Start websocket servers in separate threads
  mg_start_thread(serve_thread_func, ws1_server);
  mg_start_thread(serve_thread_func, ws2_server);

  // Finally, start proxy server in this thread: this call blocks
  serve_thread_func(proxy_server);

  printf("Existing on signal %d\n", s_received_signal);
  return EXIT_SUCCESS;
}
