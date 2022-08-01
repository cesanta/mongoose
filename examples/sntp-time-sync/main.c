// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved
//
// Synchronize time with remote SNTP server
// A working time() call is required by TLS, so an embedded device without
// a clock that wants to use TLS, must sync time via SNTP.

#include "mongoose.h"

// The UNIX epoch of the boot time. Initially, we set it to 0.  But then after
// SNTP response, we update it to the correct value, which will allow us to
// use time(). Uptime in milliseconds is returned by mg_millis().
static time_t s_boot_timestamp = 0;

// SNTP client connection
static struct mg_connection *s_sntp_conn = NULL;

// On embedded systems, rename to time()
time_t my_time(time_t *tp) {
  time_t t = s_boot_timestamp + mg_millis() / 1000;
  if (tp != NULL) *tp = t;
  return t;
}

// SNTP client callback
static void sfn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_SNTP_TIME) {
    int64_t t = *(int64_t *) ev_data;
    MG_INFO(("Got SNTP time: %lld ms from epoch", t));
    s_boot_timestamp = (time_t) ((t - mg_millis()) / 1000);
  } else if (ev == MG_EV_CLOSE) {
    s_sntp_conn = NULL;
  }
  (void) fn_data, (void) c;
}

// Called every 5 seconds. Increase that for production case.
static void timer_fn(void *arg) {
  struct mg_mgr *mgr = (struct mg_mgr *) arg;
  if (s_sntp_conn == NULL) s_sntp_conn = mg_sntp_connect(mgr, NULL, sfn, NULL);
  if (s_sntp_conn != NULL) mg_sntp_request(s_sntp_conn);
}

int main(void) {
  struct mg_mgr mgr;        // Event manager
  mg_mgr_init(&mgr);        // Initialise event manager
  mg_log_set(MG_LL_DEBUG);  // Set log level
  mg_timer_add(&mgr, 5000, MG_TIMER_REPEAT | MG_TIMER_RUN_NOW, timer_fn, &mgr);
  for (;;) mg_mgr_poll(&mgr, 300);  // Infinite event loop
  mg_mgr_free(&mgr);                // Free manager resources
  return 0;
}
