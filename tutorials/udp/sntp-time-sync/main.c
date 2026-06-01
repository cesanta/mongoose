// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved
//
// Synchronize time with a remote SNTP server.
// After a successful sync, mg_now() returns wall-clock time in milliseconds
// since Unix epoch and mg_boot_timestamp_ms is set to a non-zero value.
// Useful on embedded devices that lack a real-time clock but need accurate
// time for TLS certificate validation or timestamping.

#include "mongoose.h"

int main(void) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_log_set(MG_LL_DEBUG);

  uint64_t timer = 0;
  for (;;) {
    // Retry every second until we have a valid time, then sync once per hour
    uint64_t period = mg_boot_timestamp_ms == 0 ? 1000 : 3600 * 1000;
    if (mg_timer_expired(&timer, period, mg_millis())) {
      mg_sntp_connect(&mgr, NULL, NULL, NULL);
    }
    if (mg_boot_timestamp_ms > 0) {
      MG_INFO(("Current time: %llu ms from epoch", mg_now()));
    }
    mg_mgr_poll(&mgr, 1000);
  }

  mg_mgr_free(&mgr);
  return 0;
}
