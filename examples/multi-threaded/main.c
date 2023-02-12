// Copyright (c) 2020-2023 Cesanta Software Limited
// All rights reserved
//
// Multithreading example.
// For each incoming request, we spawn a separate thread, that sleeps for
// some time to simulate long processing time, produces an output and
// hands over that output to the request handler function.
//
// We pass POST body to the worker thread, and respond with a calculated CRC

#include "mongoose.h"

struct thread_data {
  struct mg_queue queue;  // Worker -> Connection queue
  struct mg_str body;     // Copy of message body
};

static void start_thread(void *(*f)(void *), void *p) {
#ifdef _WIN32
#define usleep(x) Sleep((x) / 1000)
  _beginthread((void(__cdecl *)(void *)) f, 0, p);
#else
#include <pthread.h>
  pthread_t thread_id = (pthread_t) 0;
  pthread_attr_t attr;
  (void) pthread_attr_init(&attr);
  (void) pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  pthread_create(&thread_id, &attr, f, p);
  pthread_attr_destroy(&attr);
#endif
}

static void *worker_thread(void *param) {
  struct thread_data *d = (struct thread_data *) param;
  char buf[100];  // On-stack buffer for the message queue

  mg_queue_init(&d->queue, buf, sizeof(buf));  // Init queue
  usleep(1 * 1000 * 1000);                     // Simulate long execution time

  // Send a response to the connection
  if (d->body.len == 0) {
    mg_queue_printf(&d->queue, "Send me POST data");
  } else {
    uint32_t crc = mg_crc32(0, d->body.ptr, d->body.len);
    mg_queue_printf(&d->queue, "crc32: %#x", crc);
    free((char *) d->body.ptr);
  }

  // Wait until connection reads our message, then it is safe to quit
  while (d->queue.tail != d->queue.head) usleep(1000);
  MG_INFO(("done, cleaning up..."));
  free(d);
  return NULL;
}

// HTTP request callback
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    // Received HTTP request. Allocate thread data and spawn a worker thread
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct thread_data *d = (struct thread_data *) calloc(1, sizeof(*d));
    d->body = mg_strdup(hm->body);   // Pass received body to the worker
    start_thread(worker_thread, d);  // Start a thread
    *(void **) c->data = d;          // Memorise data pointer in c->data
  } else if (ev == MG_EV_POLL) {
    // Poll event. Delivered to us every mg_mgr_poll interval or faster
    struct thread_data *d = *(struct thread_data **) c->data;
    size_t len;
    char *buf;
    // Check if we have a message from the worker
    if (d != NULL && (len = mg_queue_next(&d->queue, &buf)) > 0) {
      // Got message from worker. Send a response and cleanup
      mg_http_reply(c, 200, "", "%.*s\n", (int) len, buf);
      mg_queue_del(&d->queue, len);  // Delete message
      *(void **) c->data = NULL;     // Forget about thread data
    }
  }
  (void) fn_data;
}

int main(void) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_log_set(MG_LL_DEBUG);  // Set debug log level
  mg_http_listen(&mgr, "http://localhost:8000", fn, NULL);  // Create listener
  for (;;) mg_mgr_poll(&mgr, 10);  // Event loop. Use 10ms poll interval
  mg_mgr_free(&mgr);               // Cleanup
  return 0;
}
