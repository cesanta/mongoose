#include <stdio.h>
#include <string.h>
#include "mongoose.h"

static int auth_handler(struct mg_connection *conn) {
  //
  //    md5 hash of password 'constantine':
  //        2f2aa5570eb38a679d2690116090ff65
  //
  //    to verify:
  //        curl -u john:2f2aa5570eb38a679d2690116090ff65 -i localhost:8080
  //
  return mg_authorize_basic(conn, "john", "constantine", MG_MD5_PASSWORD);
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
