// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// Multithreading example.
// For each incoming request, we spawn a separate thread, that sleeps for
// some time to simulate long processing time, produces an output and
// hands over that output to the request handler function.
//

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
  int sock = (int) (size_t) param;  // Paired socket. We own it
  sleep(2);                         // Simulate long execution
  send(sock, "hi", 2, 0);           // Wakeup event manager
  close(sock);                      // Close the connection
}

static void link_conns(struct mg_connection *c1, struct mg_connection *c2) {
  c1->fn_data = c2;
  c2->fn_data = c1;
}

static void unlink_conns(struct mg_connection *c1, struct mg_connection *c2) {
  c1->fn_data = c2->fn_data = NULL;
}

// Pipe event handler
static void pcb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  struct mg_connection *parent = (struct mg_connection *) fn_data;
  MG_INFO(("%lu %p %d %p", c->id, c->fd, ev, parent));
  if (parent == NULL) {  // If parent connection closed, close too
    c->is_closing = 1;
  } else if (ev == MG_EV_READ) {  // Got data from the worker thread
    mg_http_reply(parent, 200, "Host: foo.com\r\n", "%.*s\n", c->recv.len,
                  c->recv.buf);  // Respond!
    c->recv.len = 0;             // Tell Mongoose we've consumed data
  } else if (ev == MG_EV_OPEN) {
    link_conns(c, parent);
  } else if (ev == MG_EV_CLOSE) {
    unlink_conns(c, parent);
  }
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
      int sock = mg_mkpipe(c->mgr, pcb, c, true);             // Create pipe
      start_thread(thread_function, (void *) (size_t) sock);  // Start thread
    }
  } else if (ev == MG_EV_CLOSE) {
    if (c->fn_data != NULL) unlink_conns(c, c->fn_data);
  }
}

int main(void) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_log_set("3");
  mg_http_listen(&mgr, "http://localhost:8000", fn, NULL);  // Create listener
  for (;;) mg_mgr_poll(&mgr, 1000);                         // Event loop
  mg_mgr_free(&mgr);                                        // Cleanup
  return 0;
}
