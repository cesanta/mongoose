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

static int event_handler(struct mg_event *event) {
  char post_data[1024], input1[sizeof(post_data)], input2[sizeof(post_data)];
  int post_data_len;

  if (event->type == MG_REQUEST_BEGIN) {
    if (!strcmp(event->request_info->uri, "/handle_post_request")) {
      // User has submitted a form, show submitted data and a variable value
      post_data_len = mg_read(event->conn, post_data, sizeof(post_data));

      // Parse form data. input1 and input2 are guaranteed to be NUL-terminated
      mg_get_var(post_data, post_data_len, "input_1", input1, sizeof(input1));
      mg_get_var(post_data, post_data_len, "input_2", input2, sizeof(input2));

      // Send reply to the client, showing submitted form values.
      mg_printf(event->conn, "HTTP/1.0 200 OK\r\n"
                "Content-Type: text/plain\r\n\r\n"
                "Submitted data: [%.*s]\n"
                "Submitted data length: %d bytes\n"
                "input_1: [%s]\n"
                "input_2: [%s]\n",
                post_data_len, post_data, post_data_len, input1, input2);
    } else {
      // Show HTML form.
      mg_printf(event->conn, "HTTP/1.0 200 OK\r\n"
                "Content-Length: %d\r\n"
                "Content-Type: text/html\r\n\r\n%s",
                (int) strlen(html_form), html_form);
    }

    return 1;  // Mark event as processed
  }

  // All other events are left not processed
  return 0;
}

int main(void) {
  struct mg_context *ctx;
  const char *options[] = {"listening_ports", "8080", NULL};

  ctx = mg_start(options, &event_handler, NULL);
  getchar();  // Wait until user hits "enter"
  mg_stop(ctx);

  return 0;
}
