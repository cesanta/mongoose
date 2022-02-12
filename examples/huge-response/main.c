// Copyright (c) 2021 Cesanta Software Limited
// All rights reserved
//
// Example that demonstrates how to send a large responses with limited memory.
// We're going to send a JSON array of many integer values, s_data.
// The idea is to send a response in small chunks, and let the client request
// the next chunk.
// Periodically, s_data changes, which is tracked by s_version.
// Client requests a range and a version, to ensure data integrity.
//
//  1. Start this server, type `make`
//  2. Open http://localhost:8000 in your browser

#include "mjson.h"
#include "mongoose.h"

static const char *s_listen_on = "http://localhost:8000";
static const char *s_root_dir = "web_root";

#define DATA_SIZE 10000         // Total number of elements
#define CHUNK_SIZE 1000         // Max number returned in one API call
static int s_data[DATA_SIZE];   // Simulate some complex big data
static unsigned s_version = 0;  // Data "version"

static double getparam(struct mg_http_message *hm, const char *json_path) {
  double dv = 0;
  mjson_get_number(hm->body.ptr, hm->body.len, json_path, &dv);
  return dv;
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = ev_data;
    if (mg_http_match_uri(hm, "/api/data")) {
      unsigned start = getparam(hm, "$.start");
      unsigned version = getparam(hm, "$.version");
      char *response = NULL;
      if (version > 0 && version != s_version) {
        // Version mismatch: s_data has changed while client fetches it
        // Tell client to restart
        response = mjson_aprintf("{%Q:%Q, %Q:%u}", "error", "wrong version",
                                 "version", version);
      } else {
        // Return data, up to CHUNK_SIZE elements
        unsigned max = start + CHUNK_SIZE;
        const char *comma = "";
        if (max > DATA_SIZE) max = DATA_SIZE;
        mjson_printf(mjson_print_dynamic_buf, &response, "{%Q:%u,%Q:%u,%Q:[",
                     "version", s_version, "start", start, "data");
        while (start < max) {
          mjson_printf(mjson_print_dynamic_buf, &response, "%s%d", comma,
                       s_data[start]);
          comma = ",";
          start++;
        }
        mjson_printf(mjson_print_dynamic_buf, &response, "]}");
      }
      mg_http_reply(c, 200, "Content-Type: text/json\r\n", "%s", response);
      free(response);
    } else {
      struct mg_http_serve_opts opts = {0};
      opts.root_dir = s_root_dir;
      mg_http_serve_dir(c, hm, &opts);
    }
  }
  (void) fn_data;
}

static void timer_fn(void *arg) {
  for (int i = 0; i < DATA_SIZE; i++) {
    s_data[i] = rand();
  }
  s_version++;
  (void) arg;
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_timer t1;
  mg_mgr_init(&mgr);
  srand(time(NULL));
  mg_timer_init(&t1, 1000, MG_TIMER_REPEAT | MG_TIMER_RUN_NOW, timer_fn, NULL);
  mg_http_listen(&mgr, s_listen_on, fn, NULL);
  MG_INFO(("Listening on %s", s_listen_on));
  for (;;) mg_mgr_poll(&mgr, 1000);
  mg_timer_free(&t1);
  mg_mgr_free(&mgr);
  return 0;
}
