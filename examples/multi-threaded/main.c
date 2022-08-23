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

struct userdata {
  int sock;  // Paired socket, worker thread owns it
  struct mg_str
      body;  // data to be processed
};

static void thread_function(void *param) {
  struct userdata *p = (struct userdata *) param;
  sleep(2);  // Simulate long execution
  // Respond, wakeup event manager
  if (p->body.len)
    send(p->sock, p->body.ptr, p->body.len, 0);  // if there is a body, echo it
  else
    send(p->sock, "hi", 2, 0);  // otherwise just respond "hi"
  close(p->sock);               // Close the connection
  free((void *) p->body.ptr);   // free body
  free(p);                      // free userdata
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
      struct userdata *data = calloc(1, sizeof(*data));  // worker will free it
      // parse headers or use request body, duplicate data and pass it to worker
      // thread
      data->body = mg_strdup(hm->body);              // worker will free it
      data->sock = mg_mkpipe(c->mgr, pcb, c, true);  // Create pipe
      start_thread(thread_function, data);  // Start thread and pass data
    }
  } else if (ev == MG_EV_CLOSE) {
    if (c->fn_data != NULL) unlink_conns(c, c->fn_data);
  }
}

int main(void) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_log_set(MG_LL_DEBUG);  // Set debug log level
  mg_http_listen(&mgr, "http://localhost:8000", fn, NULL);  // Create listener
  for (;;) mg_mgr_poll(&mgr, 1000);                         // Event loop
  mg_mgr_free(&mgr);                                        // Cleanup
  return 0;
}
