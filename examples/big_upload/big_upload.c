// Copyright (c) 2015 Cesanta Software Limited
// All rights reserved
//
// This example demonstrates how to handle very large requests without keeping
// them in memory.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mongoose.h"

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

struct file_writer_data {
  FILE *fp;
  size_t bytes_written;
};

static void handle_request(struct mg_connection *nc) {
  // This handler gets for all endpoints but /upload
  mg_printf(nc, "%s",
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<html><body>Upload example."
            "<form method=\"POST\" action=\"/upload\" "
            "  enctype=\"multipart/form-data\">"
            "<input type=\"file\" name=\"file\" /> <br/>"
            "<input type=\"submit\" value=\"Upload\" />"
            "</form></body></html>");
  nc->flags |= MG_F_SEND_AND_CLOSE;
}

static void handle_upload(struct mg_connection *nc, int ev, void *p) {
  struct file_writer_data *data = (struct file_writer_data *) nc->user_data;
  struct mg_http_multipart_part *mp = (struct mg_http_multipart_part *) p;

  switch (ev) {
    case MG_EV_HTTP_PART_BEGIN: {
      if (data == NULL) {
        data = calloc(1, sizeof(struct file_writer_data));
        data->fp = tmpfile();
        data->bytes_written = 0;

        if (data->fp == NULL) {
          mg_printf(nc, "%s",
                    "HTTP/1.1 500 Failed to open a file\r\n"
                    "Content-Length: 0\r\n\r\n");
          nc->flags |= MG_F_SEND_AND_CLOSE;
          return;
        }
        nc->user_data = (void *) data;
      }
      break;
    }
    case MG_EV_HTTP_PART_DATA: {
      if (fwrite(mp->data.p, 1, mp->data.len, data->fp) != mp->data.len) {
        mg_printf(nc, "%s",
                  "HTTP/1.1 500 Failed to write to a file\r\n"
                  "Content-Length: 0\r\n\r\n");
        nc->flags |= MG_F_SEND_AND_CLOSE;
        return;
      }
      data->bytes_written += mp->data.len;
      break;
    }
    case MG_EV_HTTP_PART_END: {
      mg_printf(nc,
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Connection: close\r\n\r\n"
                "Written %ld of POST data to a temp file\n\n",
                (long) ftell(data->fp));
      nc->flags |= MG_F_SEND_AND_CLOSE;
      fclose(data->fp);
      free(data);
      nc->user_data = NULL;
      break;
    }
  }
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  (void) ev_data;
  if (ev == MG_EV_HTTP_REQUEST) {
    mg_printf(nc, "%s",
              "HTTP/1.1 200 OK\r\n"
              "Content-Type: text/html\r\n"
              "Connection: close\r\n"
              "\r\n"
              "<html><body>Upload example."
              "Navigate to <a "
              "href=\"simple_upload_demo\">/simple_upload_demo</a> for "
              "uploading using submit "
              "or to <a href=\"/ajax_upload_demo\">/ajax_upload_demo</a> for "
              "uploading using ajax"
              "</form></body></html>");
    nc->flags |= MG_F_SEND_AND_CLOSE;
  }
}

static void upload_demo_handler(struct mg_connection *nc, int ev, void *p) {
  if (ev == MG_EV_HTTP_REQUEST) {
    (void) p;
    handle_request(nc);
  }
}

static void ajax_demo_handler(struct mg_connection *nc, int ev, void *p) {
  if (ev == MG_EV_HTTP_REQUEST) {
    mg_serve_http(nc, (struct http_message *) p, s_http_server_opts);
  }
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_connection *nc;

  mg_mgr_init(&mgr, NULL);
  nc = mg_bind(&mgr, s_http_port, ev_handler);

  s_http_server_opts.document_root = ".";  // Serve current directory

  mg_register_http_endpoint(nc, "/upload", handle_upload MG_UD_ARG(NULL));
  mg_register_http_endpoint(nc, "/ajax_upload_demo",
                            ajax_demo_handler MG_UD_ARG(NULL));
  mg_register_http_endpoint(nc, "/simple_upload_demo",
                            upload_demo_handler MG_UD_ARG(NULL));

  // Set up HTTP server parameters
  mg_set_protocol_http_websocket(nc);

  printf("Starting web server on port %s\n", s_http_port);
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
