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

#ifndef SLEEP_TIME
#define SLEEP_TIME 2  // Seconds to sleep to simulate calculation
#endif

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
  LOG(LL_INFO, ("Thread started, pipe %lu/%ld", c->id, (long) (size_t) c->fd));
  LOG(LL_INFO, ("Sleeping for %d sec...", SLEEP_TIME));
  mg_usleep(SLEEP_TIME * 1000000);  // Simulate long execution
  LOG(LL_INFO, ("Sending data..."));
  mg_http_reply(c, 200, "Host: foo.com\r\n", "hi\n");
  mg_rmpipe(c);
}

// HTTP request callback
static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    // Incoming request. Create socket pair.
    // Pass blocking socket to the thread, and keep the non-blocking socket.
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/fast")) {
      // The /fast URI responds immediately without hitting a multi-threaded
      // codepath. It is for measuing performance impact
      mg_http_reply(c, 200, "Host: foo.com\r\n", "hi\n");
    } else {
      // Multithreading code path. Create "pipe" connection.
      // Pipe connection is safe to pass to a different task/thread.
      // Spawn a thread and pass created pipe connection to it.
      // Save a receiving end of the pipe into c->fn_data, in order to
      // close it when this (client) connection closes.
      struct mg_connection *pc[2];           // pc[0]: send, pc[1]: recv
      mg_mkpipe(c, pc);                      // Create pipe
      start_thread(thread_function, pc[0]);  // Spawn a task, pass pc[0] there
      c->fn_data = pc[1];                    // And save recv end for later
    }
  } else if (ev == MG_EV_CLOSE) {
    // Tell the receiving end of the pipe to close
    struct mg_connection *pc = (struct mg_connection *) fn_data;
    if (pc) pc->is_closing = 1, pc->fn_data = NULL;
  }
}

int main(void) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_log_set("3");
  mg_http_listen(&mgr, "http://localhost:8000", cb, NULL);
  for (;;) mg_mgr_poll(&mgr, 1000);
  mg_mgr_free(&mgr);
  return 0;
}
