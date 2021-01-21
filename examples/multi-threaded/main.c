// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// Multithreading example.
// For each incoming request, we spawn a separate thread, that sleeps for
// some time to simulate long processing time, produces an output and
// hands over that output to the request handler function.
//
// IMPORTANT: this program must be compiled with -DMG_ENABLE_SOCKETPAIR=1
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

// thread_function() sends this structure back to the request handler
struct response {
  char *data;
  int len;
};

#ifndef SLEEP_TIME
#define SLEEP_TIME 3  // Seconds to sleep to simulate calculation
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
  int sock = (long) param;                     // Grab our blocking socket
  struct response r = {strdup("hello\n"), 6};  // Create response
  mg_usleep(SLEEP_TIME * 1000000);             // Simulate long execution
  LOG(LL_INFO, ("got sock %d", sock));         // For debugging
  send(sock, (void *) &r, sizeof(r), 0);       // Send to request handler
  closesocket(sock);                           // Done, close socket, end thread
}

// HTTP request callback
static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    // Incoming request. Create socket pair.
    // Pass blocking socket to the thread, and keep the non-blocking socket.
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;

    if (mg_http_match_uri(hm, "/fast")) {
      // The /fast URI is for performance impact of the multithreading codepath
      mg_printf(c,
                "HTTP/1.1 200 OK\r\n"        // Reply success
                "Host: foo\r\n"              // Mandatory header
                "Content-Length: 3\r\n\r\n"  // Set to allow keep-alive
                "hi\n");
    } else {
      int blocking = -1, non_blocking = -1;
      mg_socketpair(&blocking, &non_blocking);  // Create connected pair

      // Pass blocking socket to the thread_function.
      start_thread(thread_function, (void *) (long) blocking);

      // Non-blocking is ours.   Store it in the fn_data, in
      // order to use it in the subsequent invocations
      c->fn_data = (void *) (long) non_blocking;
    }
  } else if (ev == MG_EV_POLL && c->fn_data != NULL) {
    // On each poll iteration, try to receive response data
    int sock = (int) (long) c->fn_data;
    struct response response = {NULL, 0};
    if (recv(sock, (void *) &response, sizeof(response), 0) ==
        sizeof(response)) {
      // Yeah! Got the response.
      mg_printf(c, "HTTP/1.0 200 OK\r\nContent-Length: %d\r\n\r\n%.*s",
                response.len, response.len, response.data);
      free(response.data);  // We can free produced data now
      closesocket(sock);    // And close our end of the socket pair
      c->fn_data = NULL;
    }
  }
}

int main(void) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://localhost:8000", cb, NULL);
  for (;;) mg_mgr_poll(&mgr, 50);
  mg_mgr_free(&mgr);
  return 0;
}
