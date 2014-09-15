#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "mongoose.h"

static void send_file(struct mg_connection *conn, const char *path) {
  char buf[1024];
  struct stat st;
  int n;
  FILE *fp;

  if (stat(path, &st) == 0 && (fp = fopen(path, "rb")) != NULL) {
    mg_printf(conn, "--w00t\r\nContent-Type: image/jpeg\r\n"
              "Content-Length: %lu\r\n\r\n", (unsigned long) st.st_size);
    while ((n = fread(buf, 1, sizeof(buf), fp)) > 0) {
      mg_write(conn, buf, n);
    }
    fclose(fp);
    mg_write(conn, "\r\n", 2);
  }
}

struct conn_state {
  int file_index;
  time_t last_poll;
};

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  const char **file_names = (const char **) conn->server_param;
  struct conn_state *state;
  time_t now = time(NULL);

  switch (ev) {

    case MG_AUTH:
      return MG_TRUE;

    case MG_REQUEST:
      if (strcmp(conn->uri, "/stream") != 0) {
        mg_send_header(conn, "Content-Type", "text/html");
        mg_printf_data(conn, "%s",
                       "Go to <a href=/stream>/stream</a> for MJPG stream");
        return MG_TRUE;
      }

      mg_printf(conn, "%s",
                "HTTP/1.0 200 OK\r\n" "Cache-Control: no-cache\r\n"
                "Pragma: no-cache\r\nExpires: Thu, 01 Dec 1994 16:00:00 GMT\r\n"
                "Connection: close\r\nContent-Type: multipart/x-mixed-replace; "
                "boundary=--w00t\r\n\r\n");

      send_file(conn, file_names[0]);

      state = (struct conn_state *) malloc(sizeof(*state));
      conn->connection_param = state;
      state->file_index = 1;  // First file is already sent
      state->last_poll = time(NULL);
      return MG_MORE;

    case MG_POLL:
      state = (struct conn_state *) conn->connection_param;

      if (state != NULL && now > state->last_poll) {
        if (file_names[state->file_index] != NULL) {
          send_file(conn, file_names[state->file_index]);
          state->file_index++;
          if (file_names[state->file_index] == NULL) {
            return MG_TRUE;  // No more images, close connection
          }
        }
        state->last_poll = now;
      }
      return MG_FALSE;

    case MG_CLOSE:
      free(conn->connection_param);
      conn->connection_param = NULL;
      return MG_FALSE;

    default:
      return MG_FALSE;
  }
}

int main(int argc, char *argv[]) {
  struct mg_server *server;

  if (argc < 3) {
    printf("Usage: %s image1.jpg image2.jpg ...\n", argv[0]);
    return 1;
  }

  server = mg_create_server(&argv[1], ev_handler);
  mg_set_option(server, "listening_port", "8080");

  printf("Starting on port %s\n", mg_get_option(server, "listening_port"));
  for (;;) {
    mg_poll_server(server, 1000);
  }
  mg_destroy_server(&server);

  return 0;
}
