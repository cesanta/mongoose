#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mongoose.h"

static int handle_request(struct mg_connection *conn) {
  if (strcmp(conn->uri, "/upload") == 0) {
    FILE *fp = (FILE *) conn->connection_param;
    if (fp != NULL) {
      fwrite(conn->content, 1, conn->content_len, fp); // Write last bits
      mg_printf(conn, "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Connection: close\r\n\r\n"
                "Written %ld of POST data to a temp file:\n\n",
                (long) ftell(fp));

      // Temp file will be destroyed after fclose(), do something with the
      // data here -- for example, parse it and extract uploaded files.
      // As an example, we just echo the whole POST buffer back to the client.
      rewind(fp);
      mg_send_file_data(conn, fileno(fp));
      return MG_MORE;  // Tell Mongoose reply is not completed yet
    } else {
      mg_printf_data(conn, "%s", "Had no data to write...");
      return MG_TRUE; // Tell Mongoose we're done with this request
    }
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

// Mongoose sends MG_RECV for every received POST chunk.
// When last POST chunk is received, Mongoose sends MG_REQUEST, then MG_CLOSE.
static int handle_recv(struct mg_connection *conn) {
  FILE *fp = (FILE *) conn->connection_param;

  // Open temporary file where we going to write data
  if (fp == NULL && ((conn->connection_param = fp = tmpfile())) == NULL) {
    return -1;  // Close connection on error
  }

  // Return number of bytes written to a temporary file: that is how many
  // bytes we want to discard from the receive buffer
  return fwrite(conn->content, 1, conn->content_len, fp);
}

// Make sure we free all allocated resources
static int handle_close(struct mg_connection *conn) {
  if (conn->connection_param != NULL) {
    fclose((FILE *) conn->connection_param);
    conn->connection_param = NULL;
  }
  return MG_TRUE;
}

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  switch (ev) {
    case MG_AUTH:     return MG_TRUE;
    case MG_REQUEST:  return handle_request(conn);
    case MG_RECV:     return handle_recv(conn);
    case MG_CLOSE:    return handle_close(conn);
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
