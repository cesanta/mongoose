// Copyright (c) 2014 Cesanta Software Limited
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
//
// $Date: 2014-09-28 05:04:41 UTC $

// This file implements "netcat" utility with SSL and traffic hexdump.

#include "mongoose.h"

static sig_atomic_t s_received_signal = 0;

static void signal_handler(int sig_num) {
  signal(sig_num, signal_handler);
  s_received_signal = sig_num;
}

static void show_usage_and_exit(const char *prog_name) {
  fprintf(stderr, "%s\n", "Copyright (c) 2014 CESANTA SOFTWARE");
  fprintf(stderr, "%s\n", "Usage:");
  fprintf(stderr, "  %s\n [-d debug_file] [-l] [tcp|ssl]://[ip:]port[:cert][:ca_cert]",
          prog_name);
  fprintf(stderr, "%s\n", "Examples:");
  fprintf(stderr, "  %s\n -d hexdump.txt ssl://google.com:443", prog_name);
  fprintf(stderr, "  %s\n -l ssl://443:ssl_cert.pem", prog_name);
  fprintf(stderr, "  %s\n -l tcp://8080", prog_name);
  exit(EXIT_FAILURE);
}

static void on_stdin_read(struct mg_connection *nc, int ev, void *p) {
  int ch = * (int *) p;

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
    mg_send(nc, &c, 1);
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
  (void) p;

  switch (ev) {
    case MG_EV_ACCEPT:
    case MG_EV_CONNECT:
      mg_start_thread(stdio_thread_func, nc->mgr);
      break;

    case MG_EV_CLOSE:
      s_received_signal = 1;
      break;

    case MG_EV_RECV:
      fwrite(nc->recv_mbuf.buf, 1, nc->recv_mbuf.len, stdout);
      mbuf_remove(&nc->recv_mbuf, nc->recv_mbuf.len);
      break;

    default:
      break;
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  int i, is_listening = 0;
  const char *address = NULL;

  mg_mgr_init(&mgr, NULL);

  // Parse command line options
  for (i = 1; i < argc && argv[i][0] == '-'; i++) {
    if (strcmp(argv[i], "-l") == 0) {
      is_listening = 1;
    } else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
      mgr.hexdump_file = argv[++i];
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
    if (mg_bind(&mgr, address, ev_handler) == NULL) {
      fprintf(stderr, "mg_bind(%s) failed\n", address);
      exit(EXIT_FAILURE);
    }
  } else if (mg_connect(&mgr, address, ev_handler) == NULL) {
    fprintf(stderr, "mg_connect(%s) failed\n", address);
    exit(EXIT_FAILURE);
  }

  while (s_received_signal == 0) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return EXIT_SUCCESS;
}
