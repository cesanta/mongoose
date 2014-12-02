#include "mongoose.h"

// Start a browser and hit refresh couple of times. The replies will
// come from both server instances.
static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  if (ev == MG_REQUEST) {
    mg_send_header(conn, "Content-Type", "text/plain");
    mg_printf_data(conn, "This is a reply from server instance # %s",
                   (char *) conn->server_param);
    return MG_TRUE;
  } else if (ev == MG_AUTH) {
    return MG_TRUE;
  } else {
    return MG_FALSE;
  }
}

static void *serve(void *server) {
  for (;;) mg_poll_server((struct mg_server *) server, 1000);
  return NULL;
}

int main(void) {
  struct mg_server *server1, *server2;

  server1 = mg_create_server((void *) "1", ev_handler);
  server2 = mg_create_server((void *) "2", ev_handler);

  // Make both server1 and server2 listen on the same sockets
  mg_set_option(server1, "listening_port", "8080");
  mg_copy_listeners(server1, server2);

  // server1 goes to separate thread, server 2 runs in main thread.
  // IMPORTANT: NEVER LET DIFFERENT THREADS HANDLE THE SAME SERVER.
  mg_start_thread(serve, server1);
  mg_start_thread(serve, server2);
  getchar();

  return 0;
}
