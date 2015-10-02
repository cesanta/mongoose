// Copyright (c) 2015 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

struct device_settings {
  char setting1[100];
  char setting2[100];
};

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;
static struct device_settings s_settings = { "value1", "value2" };

static void handle_save(struct mg_connection *nc, struct http_message *hm) {
  // Get form variables and store settings values
  mg_get_http_var(&hm->body, "setting1", s_settings.setting1,
                  sizeof(s_settings.setting1));
  mg_get_http_var(&hm->body, "setting2", s_settings.setting2,
                  sizeof(s_settings.setting2));

  // Send response
  mg_printf(nc, "%s", "HTTP/1.1 302 OK\r\nLocation: /\r\n\r\n");
}

static void handle_get_cpu_usage(struct mg_connection *nc) {
  // Generate random value, as an example of changing CPU usage
  // Getting real CPU usage depends on the OS.
  int cpu_usage = (double) rand() / RAND_MAX * 100.0;

  // Use chunked encoding in order to avoid calculating Content-Length
  mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

  // Output JSON object which holds CPU usage data
  mg_printf_http_chunk(nc, "{ \"result\": %d }", cpu_usage);

  // Send empty chunk, the end of response
  mg_send_http_chunk(nc, "", 0);
}

static void handle_ssi_call(struct mg_connection *nc, const char *param) {
  if (strcmp(param, "setting1") == 0) {
    mg_printf_html_escape(nc, "%s", s_settings.setting1);
  } else if (strcmp(param, "setting2") == 0) {
    mg_printf_html_escape(nc, "%s", s_settings.setting2);
  }
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;

  switch (ev) {
    case MG_EV_HTTP_REQUEST:
      if (mg_vcmp(&hm->uri, "/save") == 0) {
        handle_save(nc, hm);
      } else if (mg_vcmp(&hm->uri, "/get_cpu_usage") == 0) {
        handle_get_cpu_usage(nc);
      } else {
        mg_serve_http(nc, hm, s_http_server_opts);  // Serve static content
      }
      break;
    case MG_EV_SSI_CALL:
      handle_ssi_call(nc, ev_data);
      break;
    default:
      break;
  }
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  cs_stat_t st;

  mg_mgr_init(&mgr, NULL);
  nc = mg_bind(&mgr, s_http_port, ev_handler);
  if (nc == NULL) {
    fprintf(stderr, "Cannot bind to %s\n", s_http_port);
    exit(1);
  }

  // Set up HTTP server parameters
  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = "web_root";  // Set up web root directory

  if (mg_stat(s_http_server_opts.document_root, &st) != 0) {
    fprintf(stderr, "%s", "Cannot find web_root directory, exiting\n");
    exit(1);
  }

  printf("Starting web server on port %s\n", s_http_port);
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
