/*
 * Copyright (c) 2015 Cesanta Software Limited
 * All rights reserved
 */

#include "mongoose.h"

struct device_settings {
  char setting1[100];
  char setting2[100];
  char setting3[4];
};

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;
static struct device_settings s_settings = {"value1", "value2", ""};

static void handle_save(struct mg_connection *nc, struct http_message *hm) {
  /* Get form variables and store settings values */
  mg_get_http_var(&hm->body, "setting1", s_settings.setting1,
                  sizeof(s_settings.setting1));
  mg_get_http_var(&hm->body, "setting2", s_settings.setting2,
                  sizeof(s_settings.setting2));
  mg_get_http_var(&hm->body, "setting3", s_settings.setting3,
                  sizeof(s_settings.setting3));

  printf("Settings updated to '%s' '%s' '%s'\n", s_settings.setting1,
         s_settings.setting2, s_settings.setting3);
  /* Send response */
  mg_printf(nc, "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\n\r\n%.*s",
            (unsigned long) hm->body.len, (int) hm->body.len, hm->body.p);
}

static void handle_ssi_call(struct mg_connection *nc, const char *param) {
  if (strcmp(param, "setting1") == 0) {
    mg_printf_html_escape(nc, "%s", s_settings.setting1);
  } else if (strcmp(param, "setting2") == 0) {
    mg_printf_html_escape(nc, "%s", s_settings.setting2);
  } else if (strcmp(param, "setting3_is_one") == 0) {
    if (strcmp(s_settings.setting3, "one") == 0) mg_printf(nc, "checked");
  } else if (strcmp(param, "setting3_is_two") == 0) {
    if (strcmp(s_settings.setting3, "two") == 0) mg_printf(nc, "checked");
  }
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;

  switch (ev) {
    case MG_EV_HTTP_REQUEST:
      if (mg_vcmp(&hm->uri, "/save") == 0) {
        handle_save(nc, hm); /* Handle RESTful call */
      } else {
        mg_serve_http(nc, hm, s_http_server_opts); /* Serve static content */
      }
      break;
    case MG_EV_SSI_CALL:
      handle_ssi_call(nc, ev_data);
      break;
    default:
      break;
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  char *p, path[512];

  mg_mgr_init(&mgr, NULL);
  nc = mg_bind(&mgr, s_http_port, ev_handler);
  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = "./web_root";
  s_http_server_opts.auth_domain = "example.com";
  // mgr.hexdump_file = "/dev/stdout";

  /* If our current directory */
  if (argc > 0 && (p = strrchr(argv[0], '/'))) {
    snprintf(path, sizeof(path), "%.*s/web_root", (int) (p - argv[0]), argv[0]);
    s_http_server_opts.document_root = path;
  }

  printf("Starting device configurator on port %s\n", s_http_port);
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
