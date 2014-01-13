#include <stdio.h>
#include <string.h>
#include "mongoose.h"

static int index_html(struct mg_connection *conn) {
  mg_send_header(conn, "Content-Type", "text/html");
  mg_printf_data(conn, "%s",
                 "This link is password-protected: <a href=/secret>link</a>");
  return 1;
}

static int secret_html(struct mg_connection *conn) {
  static const char *passwords_file = "my_passwords.txt";
  FILE *fp = fopen(passwords_file, "r");

  // To populate passwords file, do
  // mongoose -A my_passwords.txt mydomain.com admin admin

  if (mg_authorize_digest(conn, fp)) {
    mg_printf_data(conn, "%s", "Hi, here is a secret message!");
  } else {
    mg_send_digest_auth_request(conn);
  }

  if (fp != NULL) {
    fclose(fp);
  }

  return 1;
}

int main(void) {
  struct mg_server *server = mg_create_server(NULL);
  mg_set_option(server, "listening_port", "8080");
  mg_add_uri_handler(server, "/", index_html);
  mg_add_uri_handler(server, "/secret", secret_html);

  printf("Starting on port %s\n", mg_get_option(server, "listening_port"));
  for (;;) {
    mg_poll_server(server, 1000);
  }
  mg_destroy_server(&server);

  return 0;
}
