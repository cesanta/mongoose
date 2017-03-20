// Copyright (c) 2017 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

static void ev_handler(struct mg_connection *nc, int ev, void *p) {
  if (ev == MG_EV_HTTP_REQUEST) {
    mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
  }
}

static void upload_handler(struct mg_connection *nc, int ev, void *p) {
  (void) nc;
  struct http_message *hm = (struct http_message *) p;
  if (ev == MG_EV_HTTP_REQUEST) {
    char file_name[256];
    FILE *f = NULL;
    if (mg_get_http_var(&hm->query_string, "file_name", file_name,
                        sizeof(file_name)) <= 0) {
      mg_printf(nc, "%s",
                "HTTP/1.1 500 File name is not specified\r\n"
                "Content-Length: 0\r\n\r\n");
      nc->flags |= MG_F_SEND_AND_CLOSE;
      goto cleanup;
    }
    printf("Saving file %s, size %d bytes\n", file_name, (int) hm->body.len);
    f = fopen(file_name, "w");
    if (f == NULL) {
      mg_printf(nc, "%s",
                "HTTP/1.1 500 Failed to open a file\r\n"
                "Content-Length: 0\r\n\r\n");
      nc->flags |= MG_F_SEND_AND_CLOSE;
      goto cleanup;
    }
    if (fwrite(hm->body.p, 1, hm->body.len, f) != hm->body.len) {
      mg_printf(nc, "%s",
                "HTTP/1.1 500 Failed to write to a file\r\n"
                "Content-Length: 0\r\n\r\n");
      nc->flags |= MG_F_SEND_AND_CLOSE;
      goto cleanup;
    }
    mg_printf(nc, "%s",
              "HTTP/1.1 200 OK\r\n"
              "Content-Type: text/html\r\n"
              "Connection: close\r\n"
              "\r\n"
              "File upload successfully");
    nc->flags |= MG_F_SEND_AND_CLOSE;
    printf("Done.\n");
  cleanup:
    if (f != NULL) {
      fclose(f);
    }
  }
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;

  mg_mgr_init(&mgr, NULL);
  printf("Starting web server on port %s\n", s_http_port);
  nc = mg_bind(&mgr, s_http_port, ev_handler);
  if (nc == NULL) {
    printf("Failed to create listener\n");
    return 1;
  }

  mg_register_http_endpoint(nc, "/upload", upload_handler MG_UD_ARG(NULL));

  // Set up HTTP server parameters
  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = ".";  // Serve current directory
  s_http_server_opts.enable_directory_listing = "yes";

  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
