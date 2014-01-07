#include <stdio.h>
#include <string.h>
#include "mongoose.h"

static const char *html_form =
  "<html><body>POST example."
  "<form method=\"POST\" action=\"/handle_post_request\">"
  "Input 1: <input type=\"text\" name=\"input_1\" /> <br/>"
  "Input 2: <input type=\"text\" name=\"input_2\" /> <br/>"
  "<input type=\"submit\" />"
  "</form></body></html>";

static int handler(struct mg_connection *conn) {
  char var1[500], var2[500];

  if (strcmp(conn->uri, "/handle_post_request") == 0) {
    // User has submitted a form, show submitted data and a variable value
    // Parse form data. var1 and var2 are guaranteed to be NUL-terminated
    mg_get_var(conn, "input_1", var1, sizeof(var1));
    mg_get_var(conn, "input_2", var2, sizeof(var2));

    // Send reply to the client, showing submitted form values.
    // POST data is in conn->content, data length is in conn->content_len
    mg_send_header(conn, "Content-Type", "text/plain");
    mg_printf_data(conn,
                   "Submitted data: [%.*s]\n"
                   "Submitted data length: %d bytes\n"
                   "input_1: [%s]\n"
                   "input_2: [%s]\n",
                   conn->content_len, conn->content,
                   conn->content_len, var1, var2);
  } else {
    // Show HTML form.
    mg_send_data(conn, html_form, strlen(html_form));
  }

  return 1;
}

int main(void) {
  struct mg_server *server = mg_create_server(NULL);
  mg_set_option(server, "listening_port", "8080");
  mg_add_uri_handler(server, "/", handler);
  printf("Starting on port %s\n", mg_get_option(server, "listening_port"));
  for (;;) {
    mg_poll_server(server, 1000);
  }
  mg_destroy_server(&server);
  return 0;
}
