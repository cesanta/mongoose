#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mongoose.h"

struct file_data {
  FILE *fp;
  const char *data;
  int data_len;
  int written;
};

static int handle_request(struct mg_connection *conn) {
  const char *data;
  int data_len;
  char var_name[100], file_name[100], path[100];

  if (strcmp(conn->uri, "/upload") == 0) {
    if (mg_parse_multipart(conn->content, conn->content_len,
                           var_name, sizeof(var_name),
                           file_name, sizeof(file_name),
                           &data, &data_len) > 0) {
      struct file_data *p = (struct file_data *) malloc(sizeof(*p));
      snprintf(path, sizeof(path), "UPLOAD_%s", file_name);
      p->fp = fopen(path, "wb");
      p->data = data;
      p->data_len = data_len;
      p->written = 0;
      conn->connection_param = p;
      mg_send_header(conn, "Content-Type", "text/html");
    }
    return MG_MORE; // Tell mongoose to keep this connection open
  } else {
    mg_printf_data(conn, "%s",
                   "<html><body>Upload example."
                   "<form method=\"POST\" action=\"/upload\" "
                   "  enctype=\"multipart/form-data\">"
                   "<input type=\"file\" name=\"file\" /> <br/>"
                   "<input type=\"submit\" value=\"Upload\" />"
                   "</form></body></html>");
    return MG_TRUE;   // Tell mongoose to close this connection
  }
}

static int handle_poll(struct mg_connection *conn) {
  struct file_data *p = (struct file_data *) conn->connection_param;
  if (p != NULL) {
    // Write no more then 100 bytes in one go
    int len = p->data_len - p->written;
    int n = fwrite(p->data + p->written, 1, len > 100 ? 100 : len, p->fp);
    if (n > 0) {
      p->written += n;
      mg_send_data(conn, " ", 1);  // Send something back to wake up select()
    }

    // If everything is written, close the connection
    if (p->written >= p->data_len) {
      mg_printf_data(conn, "Written %d bytes.", p->written);
      fclose(p->fp);
      free(p);
      conn->connection_param = NULL;
      return MG_TRUE; // Tell mongoose to close this connection
    }
  }
  return MG_FALSE;  // Tell mongoose to keep this connection open
}

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  switch (ev) {
    case MG_AUTH:     return MG_TRUE;
    case MG_REQUEST:  return handle_request(conn);
    case MG_POLL:     return handle_poll(conn);
    default:          return MG_FALSE;
  }
}

int main(void) {
  struct mg_server *server = mg_create_server(NULL, ev_handler);
  mg_set_option(server, "listening_port", "8080");
  printf("Starting on port %s\n", mg_get_option(server, "listening_port"));

  for (;;) {
    mg_poll_server(server, 1000);
  }

  mg_destroy_server(&server);
  return 0;
}
