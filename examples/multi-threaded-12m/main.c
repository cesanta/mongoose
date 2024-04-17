// Copyright (c) 2020-2023 Cesanta Software Limited
// All rights reserved
//
// Multithreading example.
// On creation, we spawn a separate thread that sleeps for
// some time to simulate some processing time, then produces an output and
// sends that output to the parent connection.
// That connection then broadcasts that data to all connected WebSocket
// connections

#include "mongoose.h"

static void start_thread(void *(*f)(void *), void *p) {
#ifdef _WIN32
  _beginthread((void(__cdecl *)(void *)) f, 0, p);
#else
#define closesocket(x) close(x)
#include <pthread.h>
  pthread_t thread_id = (pthread_t) 0;
  pthread_attr_t attr;
  (void) pthread_attr_init(&attr);
  (void) pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  pthread_create(&thread_id, &attr, f, p);
  pthread_attr_destroy(&attr);
#endif
}

struct thread_data {
  struct mg_mgr *mgr;
  unsigned long conn_id;  // Parent connection ID
};

static void *thread_function(void *param) {
  struct thread_data *p = (struct thread_data *) param;
  printf("THREAD STARTED\n");
  for (;;) {
    sleep(2);
    mg_wakeup(p->mgr, p->conn_id, "hi!", 3);  // Send to parent
  }
  // Free all resources that were passed to us
  free(p);
  return NULL;
}

// HTTP request callback
static void fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_OPEN && c->is_listening) {
    // Start worker thread
    struct thread_data *data = calloc(1, sizeof(*data));  // Worker owns it
    data->conn_id = c->id;
    data->mgr = c->mgr;
    start_thread(thread_function, data);  // Start thread and pass data
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/websocket"), NULL)) {
      mg_ws_upgrade(c, hm, NULL);  // Upgrade HTTP to Websocket
      c->data[0] = 'W';            // Set some unique mark on a connection
    } else {
      // Serve static files
      // struct mg_http_serve_opts opts = {.root_dir = s_web_root};
      // mg_http_serve_dir(c, ev_data, &opts);
    }
  } else if (ev == MG_EV_WS_MSG) {
    // Got websocket frame. Received data is wm->data. Echo it back!
    struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
    mg_ws_send(c, wm->data.buf, wm->data.len, WEBSOCKET_OP_TEXT);
    mg_iobuf_del(&c->recv, 0, c->recv.len);
  } else if (ev == MG_EV_WAKEUP) {
    struct mg_str *data = (struct mg_str *) ev_data;
    // Broadcast message to all connected websocket clients.
    // Traverse over all connections
    for (struct mg_connection *wc = c->mgr->conns; wc != NULL; wc = wc->next) {
      // Send only to marked connections
      if (wc->data[0] == 'W')
        mg_ws_send(wc, data->buf, data->len, WEBSOCKET_OP_TEXT);
    }
  }
}

int main(void) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);        // Initialise event manager
  mg_log_set(MG_LL_DEBUG);  // Set debug log level
  mg_http_listen(&mgr, "http://localhost:8000", fn, NULL);  // Create listener
  mg_wakeup_init(&mgr);  // Initialise wakeup socket pair
  for (;;) {             // Event loop
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);
  return 0;
}
