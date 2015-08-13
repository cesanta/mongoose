/*
 * Copyright (c) 2015 Cesanta Software Limited
 *
 * In this example we have a background data producer that periodically
 * produces a piece of data, which is then distributed to all connected
 * WebSocket clients.
 *
 * Data producing thread is outside of the event loop so synchronization
 * using a mutex is required.
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <pthread.h>

#include "mongoose.h"

static pthread_mutex_t s_data_lock = PTHREAD_MUTEX_INITIALIZER;
static int s_data_version = 0;
static char s_data[100];

void *data_producer(void *arg) {
  (void) arg;
  fprintf(stderr, "Data producer running\n");
  srand(time(NULL));
  while (1) {
    pthread_mutex_lock(&s_data_lock);
    snprintf(s_data, sizeof(s_data), "The lucky number is %d.", rand() % 100);
    s_data_version++;
    pthread_mutex_unlock(&s_data_lock);
    sleep(1 + rand() % 10);
  }
}

struct conn_state {
  int data_version;
  /* More state goes here. */
};

void maybe_send_data(struct mg_connection *conn) {
  struct conn_state *cs = (struct conn_state *) conn->connection_param;
  if (cs == NULL) return;  /* Not connected yet. */
  pthread_mutex_lock(&s_data_lock);
  if (cs->data_version != s_data_version) {
    mg_websocket_printf(conn, WEBSOCKET_OPCODE_TEXT, "%s\n", s_data);
    cs->data_version = s_data_version;
  }
  pthread_mutex_unlock(&s_data_lock);
}

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  switch (ev) {
    case MG_AUTH:
      return MG_TRUE;  /* Authenticated. */
    case MG_WS_HANDSHAKE:
      return MG_FALSE; /* Let Mongoose complete the handshake. */
    case MG_WS_CONNECT:
      fprintf(stderr, "%s:%u joined\n", conn->remote_ip, conn->remote_port);
      conn->connection_param = calloc(1, sizeof(struct conn_state));
      mg_websocket_printf(conn, WEBSOCKET_OPCODE_TEXT, "Hi %p!\n", conn);
      maybe_send_data(conn);
      return MG_FALSE;  /* Keep the connection open. */
    case MG_POLL:
      maybe_send_data(conn);
      return MG_FALSE;  /* Keep the connection open. */
    case MG_CLOSE:
      fprintf(stderr, "%s:%u went away\n", conn->remote_ip, conn->remote_port);
      free(conn->connection_param);
      conn->connection_param = NULL;
      return MG_TRUE;
    default:
      return MG_FALSE;
  }
}

int main(void) {
  const char *listen_port = "8080";
  struct mg_server *server;
  const char *err;

  server = mg_create_server(NULL, ev_handler);
  err = mg_set_option(server, "listening_port", listen_port);
  if (err != NULL) {
    fprintf(stderr, "Error setting up listener on %s: %s\n", listen_port, err);
    return 1;
  }

  mg_start_thread(data_producer, NULL);

  printf("Listening on %s\n", listen_port);
  while (1) {
    mg_poll_server(server, 100);
  }

  return 0;
}
