// Copyright (c) 2020-2023 Cesanta Software Limited
// All rights reserved
//
// Multithreading example.
// For each incoming request, we spawn a separate thread, that sleeps for
// some time to simulate long processing time, produces an output and
// sends that output to the parent connection.

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
  struct mg_str message;  // Original HTTP request
};

static void *thread_function(void *param) {
  struct thread_data *p = (struct thread_data *) param;
  sleep(2);                                 // Simulate long execution
  mg_wakeup(p->mgr, p->conn_id, "hi!", 3);  // Respond to parent
  free((void *) p->message.buf);            // Free all resources that were
  free(p);                                  // passed to us
  return NULL;
}

// HTTP request callback
static void fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/fast"), NULL)) {
      // Single-threaded code path, for performance comparison
      // The /fast URI responds immediately
      mg_http_reply(c, 200, "Host: foo.com\r\n", "hi\n");
    } else {
      // Multithreading code path
      struct thread_data *data = calloc(1, sizeof(*data));  // Worker owns it
      data->message = mg_strdup(hm->message);               // Pass message
      data->conn_id = c->id;
      data->mgr = c->mgr;
      start_thread(thread_function, data);  // Start thread and pass data
    }
  } else if (ev == MG_EV_WAKEUP) {
    struct mg_str *data = (struct mg_str *) ev_data;
    mg_http_reply(c, 200, "", "Result: %.*s\n", data->len, data->buf);
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
