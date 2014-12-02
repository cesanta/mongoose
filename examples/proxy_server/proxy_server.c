// Copyright (c) 2014 Cesanta Software Limited
// All rights reserved
//
// To build and run this example:
//    git clone https://github.com/cesanta/net_skeleton.git
//    git clone https://github.com/cesanta/mongoose.git
//    cd mongoose/examples
//    make proxy
//    ./proxy
//
//  Configure your browser to use localhost:2014 as a proxy for all protocols
//  Then, navigate to https://cesanta.com

#include <sys/stat.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#define sleep(x) Sleep((x) * 1000)
#else
#include <unistd.h>
#endif

#include "mongoose.h"

static int s_received_signal = 0;
static struct mg_server *s_server = NULL;

#define SSE_CONNECTION              ((void *) 1)

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
  if (ev == MG_POLL && conn->connection_param == SSE_CONNECTION) {
    mg_printf(conn, "data: %s\r\n\r\n", (const char *) conn->callback_param);
  }
  return MG_TRUE;
}

static void *sse_pusher_thread_func(void *param) {
  while (s_received_signal == 0) {
    mg_wakeup_server_ex(s_server, sse_push, "%lu %s",
                        (unsigned long) time(NULL), (const char *) param);
    sleep(1);
  }
  return NULL;
}

// Return: 1 if regular file, 2 if directory, 0 if not found
static int exists(const char *path) {
  struct stat st;
  return stat(path, &st) != 0 ? 0 : S_ISDIR(st.st_mode)  == 0 ? 1 : 2;
}

// Return: 1 if regular file, 2 if directory, 0 if not found
static int is_local_file(const char *uri, char *path, size_t path_len) {
  snprintf(path, path_len, "%s/%s",
           mg_get_option(s_server, "document_root"), uri);
  return exists(path);
}

static int try_to_serve_locally(struct mg_connection *conn) {
  char path[500], buf[2000];
  int n, res;
  FILE *fp = NULL;

  if ((res = is_local_file(conn->uri, path, sizeof(path))) == 2) {
    strncat(path, "/index.html", sizeof(path) - strlen(path) - 1);
    res = exists(path);
    printf("PATH: [%s]\n", path);
  }
  if (res == 0) return MG_FALSE;

  if ((fp = fopen(path, "rb")) != NULL) {
    printf("Serving [%s] locally \n", path);
    mg_send_header(conn, "Connection", "close");
    mg_send_header(conn, "Content-Type", mg_get_mime_type(path, "text/plain"));
    while ((n = fread(buf, 1, sizeof(buf), fp)) > 0) {
      mg_send_data(conn, buf, n);
    }
    mg_send_data(conn, "", 0);
    fclose(fp);
  }
  return fp == NULL ? MG_FALSE : MG_TRUE;
}

static int is_resource_present_locally(const char *uri) {
  char path[500];
  return is_local_file(uri, path, sizeof(path)) || strcmp(uri, "/api/sse") == 0;
}

static int proxy_event_handler(struct mg_connection *conn, enum mg_event ev) {
  static const char target_url[] = "http://cesanta.com";
  static int target_url_size = sizeof(target_url) - 1;
  const char *host;

  switch (ev) {
    case MG_REQUEST:
      host = mg_get_header(conn, "Host");
      printf("[%s] [%s] [%s]\n", conn->request_method, conn->uri,
             host == NULL ? "" : host);
      if (strstr(conn->uri, "/qqq") != NULL) s_received_signal = SIGTERM;

      // Proxied HTTPS requests use "CONNECT foo.com:443"
      // Proxied HTTP requests use "GET http://..... "
      // Serve requests for target_url from the local FS.
      if (memcmp(conn->uri, target_url, target_url_size) == 0 &&
          is_resource_present_locally(conn->uri + target_url_size)) {
        conn->uri += target_url_size;   // Leave only path in the URI
      }

      if (strcmp(conn->uri, "/api/sse") == 0) {
        conn->connection_param = SSE_CONNECTION;
        mg_printf(conn, "%s", "HTTP/1.0 200 OK\r\n"
                  "Content-Type: text/event-stream\r\n"
                  "Cache-Control: no-cache\r\n\r\n");
        return MG_MORE;
      }

      if (host != NULL && strstr(host, "cesanta") != NULL) {
        return try_to_serve_locally(conn);
      }

      // Enable man-in-the-middle SSL mode for oracle.com
      if (!strcmp(conn->request_method, "CONNECT") &&
          !strcmp(host, "oracle.com")) {
        mg_terminate_ssl(conn, "ssl_cert.pem");  // MUST return MG_MORE after
        return MG_MORE;
      }

      return MG_FALSE;
    case MG_AUTH:
      return MG_TRUE;
    default:
      return MG_FALSE;
  }
}

static void setopt(struct mg_server *s, const char *opt, const char *val) {
  const char *err_msg = mg_set_option(s, opt, val);
  if (err_msg != NULL) {
    elog(1, "Error setting [%s]: [%s]", opt, err_msg);
  }
}

int main(int argc, char *argv[]) {
  const char *port = "2014", *dump = NULL, *root = "proxy_web_root";
  int i;

  // Parse command line options
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-port") == 0 && i + 1 < argc) {
      port = argv[++i];
    } else if (strcmp(argv[i], "-root") == 0 && i + 1 < argc) {
      root = argv[++i];
    } else if (strcmp(argv[i], "-dump") == 0 && i + 1 < argc) {
      dump = argv[++i];
    } else {
      elog(1, "Usage: %s [-cert FILE] [-ca_cert FILE] [-port PORT]", argv[0]);
    }
  }

  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);

  // Create and configure proxy server
  s_server = mg_create_server(NULL, &proxy_event_handler);
  setopt(s_server, "enable_proxy",        "yes");
  setopt(s_server, "document_root",       root);
  setopt(s_server, "listening_port",      port);
  setopt(s_server, "hexdump_file",        dump);

  // Start two SSE pushing threads
  mg_start_thread(sse_pusher_thread_func, (void *) "sse_pusher_thread_1");
  mg_start_thread(sse_pusher_thread_func, (void *) "sse_pusher_thread_2");

  // Start serving in the main thread
  printf("Starting on port %s\n", mg_get_option(s_server, "listening_port"));
  while (s_received_signal == 0) {
    mg_poll_server(s_server, 1000);
  }
  printf("Existing on signal %d\n", s_received_signal);
  mg_destroy_server(&s_server);

  return EXIT_SUCCESS;
}
