/*
 * Copyright (c) 2014-2017 Cesanta Software Limited
 * All rights reserved
 */

#include "mongoose.h"

static sig_atomic_t s_received_signal = 0;
static const char *s_http_port = "8000";
static const int s_num_worker_threads = 5;
static unsigned long s_next_id = 0;

static void signal_handler(int sig_num) {
  signal(sig_num, signal_handler);
  s_received_signal = sig_num;
}
static struct mg_serve_http_opts s_http_server_opts;
static sock_t sock[2];

// This info is passed to the worker thread
struct work_request {
  unsigned long conn_id;  // needed to identify the connection where to send the reply
  // optionally, more data that could be required by worker 
};

// This info is passed by the worker thread to mg_broadcast
struct work_result {
  unsigned long conn_id;
  int sleep_time;
};

static void on_work_complete(struct mg_connection *nc, int ev, void *ev_data) {
  (void) ev;
  char s[32];
  struct mg_connection *c;
  for (c = mg_next(nc->mgr, NULL); c != NULL; c = mg_next(nc->mgr, c)) {
    if (c->user_data != NULL) {
      struct work_result *res = (struct work_result *)ev_data;
      if ((unsigned long)c->user_data == res->conn_id) {
        sprintf(s, "conn_id:%lu sleep:%d", res->conn_id, res->sleep_time);
        mg_send_head(c, 200, strlen(s), "Content-Type: text/plain");
        mg_printf(c, "%s", s);
      }
    }
  }
}

void *worker_thread_proc(void *param) {
  struct mg_mgr *mgr = (struct mg_mgr *) param;
  struct work_request req = {0};
  
  while (s_received_signal == 0) {
    if (read(sock[1], &req, sizeof(req)) < 0)
      perror("Reading worker sock");
    int r = rand() % 10;
    sleep(r);
    struct work_result res = {req.conn_id, r};
    mg_broadcast(mgr, on_work_complete, (void *)&res, sizeof(res));
  }
  return NULL;
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  (void) nc;
  (void) ev_data;
  
  switch (ev) {
    case MG_EV_ACCEPT:
      nc->user_data = (void *)++s_next_id;
      break;
    case MG_EV_HTTP_REQUEST: {
      struct work_request req = {(unsigned long)nc->user_data};

      if (write(sock[0], &req, sizeof(req)) < 0)
        perror("Writing worker sock");
      break;
    }
    case MG_EV_CLOSE: {
      if (nc->user_data) nc->user_data = NULL;
    }
  }
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  int i;

  if (mg_socketpair(sock, SOCK_STREAM) == 0) {
    perror("Opening socket pair");
    exit(1);
  }

  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);

  mg_mgr_init(&mgr, NULL);

  nc = mg_bind(&mgr, s_http_port, ev_handler);
  if (nc == NULL) {
    printf("Failed to create listener\n");
    return 1;
  }

  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = ".";  // Serve current directory
  s_http_server_opts.enable_directory_listing = "no";

  for (i = 0; i < s_num_worker_threads; i++) {
    mg_start_thread(worker_thread_proc, &mgr);
  }

  printf("Started on port %s\n", s_http_port);
  while (s_received_signal == 0) {
    mg_mgr_poll(&mgr, 200);
  }

  mg_mgr_free(&mgr);

  closesocket(sock[0]);
  closesocket(sock[1]);

  return 0;
}
