// Copyright (c) 2014 Cesanta Software Limited
// All rights reserved
//
// To build and run this example:
//    git clone https://github.com/cesanta/net_skeleton.git
//    git clone https://github.com/cesanta/mongoose.git
//    cd mongoose/examples
//    make proxy
//    ./proxy   # (then point your browser at http://localhost:2014)

#include "net_skeleton.h"
#include "mongoose.h"

static int s_received_signal = 0;
static const char *s_ca_cert = NULL;
static const char *s_cert = NULL;
static const char *s_sse_port = "2014";
static const char *s_proxy_port = "2015";
static struct mg_server *s_sse_server = NULL;
static struct mg_server *s_proxy_server = NULL;

static void elog(int do_exit, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  fputc('\n', stderr);
  if (do_exit) exit(EXIT_FAILURE);
}

static void signal_handler(int sig_num) {
  signal(sig_num, signal_handler);
  s_received_signal = sig_num;
}

static int sse_push(struct mg_connection *conn, enum mg_event ev) {
  const char *str = (const char *) conn->connection_param;
  if (ev == MG_POLL && str != NULL && strcmp(str, "sse") == 0) {
    mg_printf(conn, "data: %s\r\n\r\n", (const char *) conn->callback_param);
  }
  return MG_TRUE;
}

static void *sse_pusher_thread_func(void *param) {
  while (s_received_signal == 0) {
    mg_wakeup_server_ex(s_sse_server, sse_push, "%lu %s",
                        (unsigned long) time(NULL), (const char *) param);
    sleep(1);
  }
  return NULL;
}

static int sse_handler(struct mg_connection *conn, enum mg_event ev) {
  switch (ev) {
    case MG_REQUEST:
      if (strcmp(conn->uri, "/") == 0) {
        mg_printf(conn, "%s", "HTTP/1.1 302 Moved\r\n"
                  "Location: /sse.html\r\n\r\n");
        return MG_TRUE;
      }
      if (strcmp(conn->uri, "/api/sse") == 0) {
        conn->connection_param = (void *) "sse";
        mg_printf(conn, "%s",
                  "HTTP/1.0 200 OK\r\n"
                  "Content-Type: text/event-stream\r\n"
                  "Cache-Control: no-cache\r\n"
                  "\r\n");
        return MG_MORE;
      }
      return MG_FALSE;
    case MG_AUTH:
      return MG_TRUE;
    case MG_POLL:
      return MG_FALSE;
    default:
      return MG_FALSE;
  }
}

static void *serve_thread_func(void *param) {
  struct mg_server *server = (struct mg_server *) param;
  printf("Starting on port %s\n", mg_get_option(server, "listening_port"));

  while (s_received_signal == 0) {
    mg_poll_server(server, 1000);
  }

  return NULL;
}

static void show_usage_and_exit(const char *prog) {
  elog(1, "Usage: %s [-ca_cert FILE] [-sse_port PORT] [-proxy_port PORT]",
       prog);
  exit(EXIT_FAILURE);
}

static void setopt(struct mg_server *s, const char *opt, const char *val) {
  const char *err_msg = mg_set_option(s, opt, val);
  if (err_msg != NULL) {
    elog(1, "Error setting [%s]: [%s]", opt, err_msg);
  }
}

int main(int argc, char *argv[]) {
  int i;

  // Parse command line options
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-ca_cert") == 0 && i + 1 < argc) {
      s_ca_cert = argv[++i];
    } else if (strcmp(argv[i], "-cert") == 0 && i + 1 < argc) {
      s_cert = argv[++i];
    } else if (strcmp(argv[i], "-sse_port") == 0 && i + 1 < argc) {
      s_sse_port = argv[++i];
    } else if (strcmp(argv[i], "-proxy_port") == 0 && i + 1 < argc) {
      s_proxy_port = argv[++i];
    } else {
      show_usage_and_exit(argv[0]);
    }
  }

  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);
  
  // Create, configure and start proxy server in a separate thread
  s_proxy_server = mg_create_server(NULL, &sse_handler);
  setopt(s_proxy_server, "listening_port", s_proxy_port);
  setopt(s_proxy_server, "ssl_certificate", s_cert);
  setopt(s_proxy_server, "ssl_ca_certificate", s_ca_cert);
  setopt(s_proxy_server, "hexdump_file", "/dev/stdout");
  mg_start_thread(serve_thread_func, s_proxy_server);

  // Create, configure and start SSE server and SSE pusher threads
  // Start two SSE pushing threads
  // Serve SSE server in the main thread
  s_sse_server = mg_create_server(NULL, &sse_handler);
  setopt(s_sse_server, "listening_port", s_sse_port);
  setopt(s_sse_server, "document_root", ".");
  mg_start_thread(sse_pusher_thread_func, (void *) "sse_pusher_thread_1");
  mg_start_thread(sse_pusher_thread_func, (void *) "sse_pusher_thread_2");
  serve_thread_func(s_sse_server);

  printf("Existing on signal %d\n", s_received_signal);
  mg_destroy_server(&s_sse_server);
  mg_destroy_server(&s_proxy_server);

  return EXIT_SUCCESS;
}
