// Copyright (c) 2014-2016 Cesanta Software Limited
// All rights reserved
//
// This software is dual-licensed: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation. For the terms of this
// license, see <http://www.gnu.org/licenses/>.
//
// You are free to use this software under the terms of the GNU General
// Public License, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// Alternatively, you can license this software under a commercial
// license, as set out in <https://www.cesanta.com/license>.

// This file implements "netcat" utility with SSL and traffic hexdump.

#include "mongoose.h"

static sig_atomic_t s_received_signal = 0;
static int s_is_websocket;

static void signal_handler(int sig_num) {
  signal(sig_num, signal_handler);
  s_received_signal = sig_num;
}

static void show_usage_and_exit(const char *prog_name) {
  fprintf(stderr, "%s\n", "Copyright (c) Cesanta. Built on: " __DATE__);
  fprintf(stderr, "Usage: %s [OPTIONS] [IP:]PORT\n", prog_name);
  fprintf(stderr, "%s\n", "Options:");
  fprintf(stderr, "%s\n", "  -l\t\tOpen a listening socket");
  fprintf(stderr, "%s\n", "  -d file\tHexdump traffic into a file");
  fprintf(stderr, "%s\n", "  -ws\t\tUse WebSocket protocol");
  exit(EXIT_FAILURE);
}

static void on_stdin_read(struct mg_connection *nc, int ev, void *p) {
  int ch = *(int *) p;

  (void) ev;

  if (ch < 0) {
    // EOF is received from stdin. Schedule the connection to close
    nc->flags |= MG_F_SEND_AND_CLOSE;
    if (nc->send_mbuf.len <= 0) {
      nc->flags |= MG_F_CLOSE_IMMEDIATELY;
    }
  } else {
    // A character is received from stdin. Send it to the connection.
    unsigned char c = (unsigned char) ch;
    if (s_is_websocket) {
      mg_send_websocket_frame(nc, WEBSOCKET_OP_TEXT, &c, 1);
    } else {
      mg_send(nc, &c, 1);
    }
  }
}

static void *stdio_thread_func(void *param) {
  struct mg_mgr *mgr = (struct mg_mgr *) param;
  int ch;

  // Read stdin until EOF character by character, sending them to the mgr
  while ((ch = getchar()) != EOF) {
    mg_broadcast(mgr, on_stdin_read, &ch, sizeof(ch));
  }
  s_received_signal = 1;

  return NULL;
}

static void ev_handler(struct mg_connection *nc, int ev, void *p) {
  switch (ev) {
    case MG_EV_ACCEPT:
    case MG_EV_CONNECT:
      mg_start_thread(stdio_thread_func, nc->mgr);
      break;

    case MG_EV_WEBSOCKET_FRAME: {
      struct websocket_message *wm = (struct websocket_message *) p;
      fwrite(wm->data, 1, wm->size, stdout);
      break;
    }

    case MG_EV_CLOSE:
      s_received_signal = 1;
      break;

    case MG_EV_RECV:
      if (!s_is_websocket) {
        fwrite(nc->recv_mbuf.buf, 1, nc->recv_mbuf.len, stdout);
        mbuf_remove(&nc->recv_mbuf, nc->recv_mbuf.len);
      }
      break;

    default:
      break;
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  int i, is_listening = 0;
  const char *address = NULL;
  struct mg_connection *c;
  const char *err = NULL;

  mg_mgr_init(&mgr, NULL);

  // Parse command line options
  for (i = 1; i < argc && argv[i][0] == '-'; i++) {
    if (strcmp(argv[i], "-l") == 0) {
      is_listening = 1;
    } else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
      mgr.hexdump_file = argv[++i];
    } else if (strcmp(argv[i], "-ws") == 0 && i + 1 < argc) {
      s_is_websocket = 1;
    } else {
      show_usage_and_exit(argv[0]);
    }
  }

  if (i + 1 == argc) {
    address = argv[i];
  } else {
    show_usage_and_exit(argv[0]);
  }

  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);
  signal(SIGPIPE, SIG_IGN);

  if (is_listening) {
    struct mg_bind_opts opts;
    memset(&opts, 0, sizeof(opts));
    opts.error_string = &err;
    if ((c = mg_bind_opt(&mgr, address, ev_handler, opts)) == NULL) {
      fprintf(stderr, "mg_bind(%s) failed: %s\n", address, err);
      exit(EXIT_FAILURE);
    }
  } else {
    struct mg_connect_opts opts;
    memset(&opts, 0, sizeof(opts));
    opts.error_string = &err;
    if ((c = mg_connect_opt(&mgr, address, ev_handler, opts)) == NULL) {
      fprintf(stderr, "mg_connect(%s) failed: %s\n", address, err);
      exit(EXIT_FAILURE);
    }
  }
  if (s_is_websocket) {
    mg_set_protocol_http_websocket(c);
    if (!is_listening) {
      mg_send_websocket_handshake2(c, "/", address, NULL, NULL);
    }
  }

  while (s_received_signal == 0) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return EXIT_SUCCESS;
}
