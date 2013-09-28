// Copyright (c) 2004-2012 Sergey Lyubka
// This file is a part of mongoose project, http://github.com/valenok/mongoose

#include <stdio.h>
#include <string.h>
#include "mongoose.h"

static int event_handler(struct mg_event *event) {

  if (event->type == MG_REQUEST_BEGIN) {
    if (!strcmp(event->request_info->uri, "/handle_post_request")) {
      char path[200];
      FILE *fp = mg_upload(event->conn, "/tmp", path, sizeof(path));
      if (fp != NULL) {
        fclose(fp);
        mg_printf(event->conn, "HTTP/1.0 200 OK\r\n\r\nSaved: [%s]", path);
      } else {
        mg_printf(event->conn, "%s", "HTTP/1.0 200 OK\r\n\r\nNo files sent");
      }
    } else {
      // Show HTML form. Make sure it has enctype="multipart/form-data" attr.
      static const char *html_form =
        "<html><body>Upload example."
        "<form method=\"POST\" action=\"/handle_post_request\" "
        "  enctype=\"multipart/form-data\">"
        "<input type=\"file\" name=\"file\" /> <br/>"
        "<input type=\"submit\" value=\"Upload\" />"
        "</form></body></html>";

      mg_printf(event->conn, "HTTP/1.0 200 OK\r\n"
          "Content-Length: %d\r\n"
          "Content-Type: text/html\r\n\r\n%s",
          (int) strlen(html_form), html_form);
    }

    // Mark request as processed
    return 1;
  }

  // All other events left unprocessed
  return 1;
}

int main(void) {
  struct mg_context *ctx;
  const char *options[] = {"listening_ports", "8080", NULL};
  ctx = mg_start(options, event_handler, NULL);
  getchar();  // Wait until user hits "enter"
  mg_stop(ctx);

  return 0;
}
