// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

// HTTP request handler function. It implements the following endpoints:
//   /api/log/static - returns contents of log.txt file
//   /api/log/live - hangs forever, and returns live log messages
//   all other URI - serves web_root/ directory
static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/api/log/static")) {
      mg_http_serve_file(c, hm, "log.txt", "text/plain", "");
    } else if (mg_http_match_uri(hm, "/api/log/live")) {
      c->label[0] = 'L';  // Mark that connection as live log listener
      mg_printf(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    } else {
      struct mg_http_serve_opts opts = {.root_dir = "web_root"};
      mg_http_serve_dir(c, ev_data, &opts);
    }
  }
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
static void timer_fn(void *arg) {
  char buf[64];
  snprintf(buf, sizeof(buf), "Time is: %lu\n", (unsigned long) time(NULL));
  log_message("log.txt", buf);
  broadcast_message(arg, buf);
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_timer t1;

  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://localhost:8000", cb, NULL);
  mg_timer_init(&t1, 1000, MG_TIMER_REPEAT, timer_fn, &mgr);

  for (;;) mg_mgr_poll(&mgr, 50);
  mg_timer_free(&t1);
  mg_mgr_free(&mgr);

  return 0;
}
