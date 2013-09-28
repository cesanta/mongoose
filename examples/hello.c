#include <stdio.h>
#include <string.h>
#include "mongoose.h"

// This function will be called by mongoose on every new request.
static int event_handler(struct mg_event *event) {

  if (event->type == MG_REQUEST_BEGIN) {
    char content[100];

    // Prepare the message we're going to send
    int content_length = snprintf(content, sizeof(content),
        "Hello from mongoose! Requested: [%s] [%s]",
        event->request_info->request_method, event->request_info->uri);

    // Send HTTP reply to the client
    mg_printf(event->conn,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %d\r\n"        // Always set Content-Length
        "\r\n"
        "%s",
        content_length, content);

    // Returning non-zero tells mongoose that our function has replied to
    // the client, and mongoose should not send client any more data.
    return 1;
  }

  // We do not handle any other event
  return 0;
}

int main(void) {
  struct mg_context *ctx;

  // List of options. Last element must be NULL.
  const char *options[] = {"listening_ports", "8080", NULL};

  // Start the web server.
  ctx = mg_start(options, &event_handler, NULL);

  // Wait until user hits "enter". Server is running in separate thread.
  // Navigating to http://localhost:8080 will invoke begin_request_handler().
  getchar();

  // Stop the server.
  mg_stop(ctx);

  return 0;
}
