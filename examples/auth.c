#include <stdio.h>
#include <string.h>
#include "mongoose.h"

static int auth_handler(struct mg_connection *conn) {
  int result = MG_AUTH_FAIL; // Not authorized
  FILE *fp;

  // To populate passwords file, do
  // mongoose -A my_passwords.txt mydomain.com admin admin
  if ((fp = fopen("my_passwords.txt", "r")) != NULL) {
    result = mg_authorize_digest(conn, fp);
    fclose(fp);
  }

  return result;
}

int main(void) {
  struct mg_server *server = mg_create_server(NULL);
  mg_set_option(server, "listening_port", "8080");
  mg_set_option(server, "document_root", ".");
  mg_set_auth_handler(server, auth_handler);

  printf("Starting on port %s\n", mg_get_option(server, "listening_port"));
  for (;;) {
    mg_poll_server(server, 1000);
  }
  mg_destroy_server(&server);

  return 0;
}
