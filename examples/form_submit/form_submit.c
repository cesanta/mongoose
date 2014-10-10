#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mongoose.h"

static int static_value = 123;  // Exposed and changeable via the form

static void print_html_form(struct mg_connection *conn) {
  mg_send_header(conn, "Content-Type", "text/html");
  mg_send_header(conn, "Cache-Control", "max-age=0, post-check=0, "
                 "pre-check=0, no-store, no-cache, must-revalidate");

  // Note that all the following normally should reside in static HTML page
  mg_printf_data(conn, "%s", "<html><head>");
  // It is better to use local copy though
  mg_printf_data(conn, "<script src=\"%s\"></script>",
                 "http://code.jquery.com/jquery-1.11.0.min.js");
  mg_printf_data(conn, "%s", "<script> jQuery(function() {\n");
  // Here is the ajax call that fetches data from the device and
  // updates the form
  mg_printf_data(conn, "%s", "$.ajax({ url: '/get_value', dataType: 'json', "
                 "success: function(d) { $('#val').val(d.value); }});\n");
  // This ajax call is triggered when submit button is pressed. It sends new
  // value to the device.
  mg_printf_data(conn, "%s", "$(document).on('click', '#button', function() {"
                 "  $.ajax({ url: '/set_value', dataType: 'json', "
                 "  data: { new_value: $('#val').val() } });\n"
                 "  return false; });\n");
  mg_printf_data(conn, "%s", "});</script>");
  mg_printf_data(conn, "%s", "</head><body>");
  mg_printf_data(conn, "%s", "<h1>Ajax form submission example</h1>");
  mg_printf_data(conn, "%s", "<form>");
  mg_printf_data(conn, "%s", "Device value: <input type=text id=val />");
  mg_printf_data(conn, "%s", "<input type=submit id=button />");
  mg_printf_data(conn, "%s", "</form>");
  mg_printf_data(conn, "%s", "</body></html>");
}

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  if (ev == MG_REQUEST) {
    if (strcmp(conn->uri, "/get_value") == 0) {
      mg_printf_data(conn, "{\"value\": %d}", static_value);
    } else if (strcmp(conn->uri, "/set_value") == 0) {
      // This Ajax endpoint sets the new value for the device variable
      char buf[100] = "";
      mg_get_var(conn, "new_value", buf, sizeof(buf));  // Get form variable
      static_value = atoi(buf);                         // Set new value
      mg_printf_data(conn, "%s", "{\"success\": true}");
      printf("Setting value to [%d]\n", static_value);  // Debug trace
    } else {
      // Better way is to set "document_root" option, put "index.html" file
      // into document_root and return MG_FALSE here. We're printing HTML
      // page by hands just to keep everything in one C file.
      print_html_form(conn);
    }
    return MG_TRUE;
  } else if (ev == MG_AUTH) {
    return MG_TRUE;
  }

  return MG_FALSE;
}

int main(void) {
  struct mg_server *server;

  // Create and configure the server
  server = mg_create_server(NULL, ev_handler);
  mg_set_option(server, "listening_port", "8000");

  // Serve request. Hit Ctrl-C to terminate the program
  printf("Starting on port %s\n", mg_get_option(server, "listening_port"));
  for (;;) {
    mg_poll_server(server, 1000);
  }

  // Cleanup, and free server instance
  mg_destroy_server(&server);

  return 0;
}
