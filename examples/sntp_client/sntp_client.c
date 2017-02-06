/*
 * Copyright (c) 2016 Cesanta Software Limited
 * All rights reserved
 */

#include "mongoose.h"

static int s_exit_flag = 0;
static const char *s_default_server = "pool.ntp.org";

static void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  struct mg_sntp_message *sm = (struct mg_sntp_message *) ev_data;
  time_t t;
  (void) c;

  switch (ev) {
    case MG_SNTP_REPLY:
      t = time(NULL);
      fprintf(stdout, "Local time: %s", ctime(&t));
      t = (time_t) sm->time;
      fprintf(stdout, "Time from %s: %s", s_default_server, ctime(&t));
      s_exit_flag = 1;
      break;
    case MG_SNTP_FAILED:
      fprintf(stderr, "Failed to get time\n");
      s_exit_flag = 1;
      break;
  }
}

int main() {
  struct mg_mgr mgr;
  struct mg_connection *c;

  mg_mgr_init(&mgr, NULL);

  c = mg_sntp_get_time(&mgr, ev_handler, s_default_server);

  if (c == NULL) {
    fprintf(stderr, "Failed to connect to %s\n", s_default_server);
    return -1;
  }

  while (s_exit_flag == 0) {
    mg_mgr_poll(&mgr, 1000);
  }

  mg_mgr_free(&mgr);

  return 0;
}
