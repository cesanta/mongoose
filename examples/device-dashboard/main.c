// Copyright (c) 2020-2023 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"
#include "net.h"

void ui_event_next() {
  if (events_no < 0 || events_no >= MAX_EVENTS_NO)
    return;

  events[events_no].type = rand() % 3;
  events[events_no].prio = rand() % 3;
  events[events_no].timestamp = events_no;
  mg_snprintf(events[events_no].text, MAX_EVENT_TEXT_SIZE,
              "event#%d", events_no);
  events_no++;
}

static int s_sig_num;
static void signal_handler(int sig_num) {
  signal(sig_num, signal_handler);
  s_sig_num = sig_num;
}

int main(void) {
  struct mg_mgr mgr;
  uint64_t last_ts = mg_millis();
  uint64_t crt_ts;

  signal(SIGPIPE, SIG_IGN);
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  srand(time(NULL));
  mg_log_set(MG_LL_DEBUG);  // Set debug log level
  mg_mgr_init(&mgr);

  web_init(&mgr);
  while (s_sig_num == 0) {
    mg_mgr_poll(&mgr, 50);
    crt_ts = mg_millis();
    if (crt_ts - last_ts > 1000) {
      last_ts = crt_ts;
      ui_event_next(); // generate a new event
    }
  }

  mg_mgr_free(&mgr);
  MG_INFO(("Exiting on signal %d", s_sig_num));

  return 0;
}
