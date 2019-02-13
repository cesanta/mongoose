/*
 * Copyright (c) 2016 Cesanta Software Limited
 * All rights reserved
 */

/*
 * This is a WebSocket client example.
 * Prints WS frames to stdout and forwards stdin to server.
 */

#include "mongoose.h"

static int s_done = 0;
static int s_is_connected = 0;

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  (void) nc;

  switch (ev) {
    case MG_EV_CONNECT: {
      int status = *((int *) ev_data);
      if (status != 0) {
        printf("-- Connection error: %d\n", status);
      }
      break;
    }
    case MG_EV_WEBSOCKET_HANDSHAKE_DONE: {
      struct http_message *hm = (struct http_message *) ev_data;
      if (hm->resp_code == 101) {
        printf("-- Connected\n");
        s_is_connected = 1;
      } else {
        printf("-- Connection failed! HTTP code %d\n", hm->resp_code);
        /* Connection will be closed after this. */
      }
      break;
    }
    case MG_EV_POLL: {
      char msg[500];
      int n = 0;
#ifdef _WIN32 /* Windows console input is special. */
      INPUT_RECORD inp[100];
      HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
      DWORD i, num;
      if (!PeekConsoleInput(h, inp, sizeof(inp) / sizeof(*inp), &num)) break;
      for (i = 0; i < num; i++) {
        if (inp[i].EventType == KEY_EVENT &&
            inp[i].Event.KeyEvent.wVirtualKeyCode == VK_RETURN) {
          break;
        }
      }
      if (i == num) break;
      if (!ReadConsole(h, msg, sizeof(msg), &num, NULL)) break;
      /* Un-unicode. This is totally not the right way to do it. */
      for (i = 0; i < num * 2; i += 2) msg[i / 2] = msg[i];
      n = (int) num;
#else /* For everybody else, we just read() stdin. */
      fd_set read_set, write_set, err_set;
      struct timeval timeout = {0, 0};
      FD_ZERO(&read_set);
      FD_ZERO(&write_set);
      FD_ZERO(&err_set);
      FD_SET(0 /* stdin */, &read_set);
      if (select(1, &read_set, &write_set, &err_set, &timeout) == 1) {
        n = read(0, msg, sizeof(msg));
      }
#endif
      if (n <= 0) break;
      while (msg[n - 1] == '\r' || msg[n - 1] == '\n') n--;
      mg_send_websocket_frame(nc, WEBSOCKET_OP_TEXT, msg, n);
      break;
    }
    case MG_EV_WEBSOCKET_FRAME: {
      struct websocket_message *wm = (struct websocket_message *) ev_data;
      printf("%.*s\n", (int) wm->size, wm->data);
      break;
    }
    case MG_EV_CLOSE: {
      if (s_is_connected) printf("-- Disconnected\n");
      s_done = 1;
      break;
    }
  }
}

int main(int argc, char **argv) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  const char *chat_server_url = argc > 1 ? argv[1] : "ws://127.0.0.1:8000";

  mg_mgr_init(&mgr, NULL);

  nc = mg_connect_ws(&mgr, ev_handler, chat_server_url, "ws_chat", NULL);
  if (nc == NULL) {
    fprintf(stderr, "Invalid address\n");
    return 1;
  }

  while (!s_done) {
    mg_mgr_poll(&mgr, 100);
  }
  mg_mgr_free(&mgr);

  return 0;
}
