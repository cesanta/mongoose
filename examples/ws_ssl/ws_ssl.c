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

struct config {
  const char *requested_host;   // Host name that client uses
  struct ssl_wrapper_config c;
};

static struct config s_wrappers[] = {
  {"ws1", {"127.0.0.1", 9001, 0, NULL, "7001", NULL, NULL, NULL, ""}},
  {"ws1", {"127.0.0.1", 9001, 0, NULL, "7002", "certs/ws1_server.pem",
    NULL, NULL, ""}},
  {"ws2", {"127.0.0.1", 9002, 1, NULL, "7003", NULL, NULL, NULL, ""}},
  {"ws2", {"127.0.0.1", 9002, 1, NULL, "7004", "certs/ws2_server.pem",
    NULL, NULL, ""}}
};
static int s_received_signal = 0;

static void signal_handler(int sig_num) {
  signal(sig_num, signal_handler);
  s_received_signal = sig_num;
}

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  switch (ev) {
    case MG_AUTH:
      return MG_TRUE;

    case MG_REQUEST:
      printf("==> [%s] [%s]\n", conn->request_method, conn->uri);
      if (strcmp(conn->request_method, "CONNECT") == 0) {
        char host[1025] = "";
        int i, is_ssl, port = 0;
        
        sscanf(conn->uri, "%1024[^:]:%d", host, &port);
        is_ssl = (port == 443 ? 1 : 0);

        // Iterate over existing wrapper, see if we can use one of them
        for (i = 0; i < (int) ARRAY_SIZE(s_wrappers); i++) {
          if (strcmp(host, s_wrappers[i].requested_host) == 0 &&
              is_ssl == (s_wrappers[i].c.ssl_cert == NULL ? 0 : 1)) {
            // Found usable wrapper, tunnel to it.
            mg_forward(conn, "127.0.0.1",
              atoi(s_wrappers[i].c.listening_port), 0);
            return MG_MORE;
          }
        }

        // No suitable wrappers found. Disallow that CONNECT request.
        mg_send_status(conn, 405);
        return MG_TRUE;
      }

      // Not a CONNECT request, serve HTML file.
      mg_send_file(conn, "ws_ssl.html");
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
  struct ssl_wrapper_config *cfg = &((struct config *) param)->c;
  const char *err_msg;
  void *wrapper;

  if ((wrapper = ssl_wrapper_init(cfg, &err_msg)) == NULL) {
    fprintf(stderr, "Error: %s\n", err_msg);
    exit(EXIT_FAILURE);
  }
  ssl_wrapper_serve(wrapper, &s_received_signal);

  return NULL;
}

int main(void) {
  struct mg_server *proxy_server = mg_create_server(NULL, ev_handler);
  struct mg_server *ws1_server = mg_create_server(NULL, ws_handler);
  struct mg_server *ws2_server = mg_create_server(NULL, ws_handler);
  size_t i;

  // Configure proxy server to listen on port 2014
  mg_set_option(proxy_server, "listening_port", "2014");
  //mg_set_option(proxy_server, "enable_proxy", "yes");

  // Configure two websocket echo servers:
  //    ws1 is WS, listening on 9001
  //    ws2 is WSS, listening on 9002
  // Note that HTML page thinks that ws1 is WSS, and ws2 is WS,
  // where in reality it is vice versa and proxy server makes the decision.
  mg_set_option(ws1_server, "listening_port", "9001");
  mg_set_option(ws2_server, "listening_port", "9002");
  mg_set_option(ws2_server, "ssl_certificate", "certs/ws2_server.pem");

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
