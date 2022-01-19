// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// Multithreading example.
// For each incoming request, we spawn a separate thread, that sleeps for
// some time to simulate long processing time, produces an output and
// hands over that output to the request handler function.
//
// The following procedure is used to benchmark the multi-threaded codepath
// against the single-threaded codepath on MacOS:
//   $ make clean all CFLAGS="-DSLEEP_TIME=0 -DMG_ENABLE_SOCKETPAIR=1"
//   $ siege -c50 -t5s http://localhost:8000/multi
//   $ siege -c50 -t5s http://localhost:8000/fast
//
// If, during the test, there are socket errors, increase ephemeral port limit:
//   $ sysctl -a | grep portrange
//   $ sudo sysctl -w net.inet.ip.portrange.first=32768
//   $ sudo sysctl -w net.inet.ip.portrange.hifirst=32768

#include "mongoose.h"

static void start_thread(void (*f)(void *), void *p) {
#ifdef _WIN32
  _beginthread((void(__cdecl *)(void *)) f, 0, p);
#else
#define closesocket(x) close(x)
#include <pthread.h>
  pthread_t thread_id = (pthread_t) 0;
  pthread_attr_t attr;
  (void) pthread_attr_init(&attr);
  (void) pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  pthread_create(&thread_id, &attr, (void *(*) (void *) ) f, p);
  pthread_attr_destroy(&attr);
#endif
}

static void thread_function(void *param) {
  struct mg_connection *c = param;  // Pipe connection
  sleep(2);                         // Simulate long execution
  mg_mgr_wakeup(c, "hi", 2);        // Wakeup event manager
}

// HTTP request callback
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/fast")) {
      // Single-threaded code path, for performance comparison
      // The /fast URI responds immediately
      mg_http_reply(c, 200, "Host: foo.com\r\n", "hi\n");
    } else {
      // Multithreading code path
      c->label[0] = 'W';                       // Mark us as waiting for data
      start_thread(thread_function, fn_data);  // Start handling thread
    }
  }
}

// Pipe event handler
static void pcb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_READ) {
    struct mg_connection *t;
    for (t = c->mgr->conns; t != NULL; t = t->next) {
      if (t->label[0] != 'W') continue;  // Ignore un-marked connections
      mg_http_reply(t, 200, "Host: foo.com\r\n", "%.*s\n", c->recv.len,
                    c->recv.buf);  // Respond!
      t->label[0] = 0;             // Clear mark
    }
  }
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *pipe;  // Used to wake up event manager
  mg_mgr_init(&mgr);
  mg_log_set("3");
  pipe = mg_mkpipe(&mgr, pcb, NULL);                        // Create pipe
  mg_http_listen(&mgr, "http://localhost:8000", fn, pipe);  // Create listener
  for (;;) mg_mgr_poll(&mgr, 1000);                         // Event loop
  mg_mgr_free(&mgr);                                        // Cleanup
  return 0;
}
