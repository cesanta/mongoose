/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#include "bleconfig.h"
#include "myboard.h"

#include "mongoose.h"

/*
 * This is a callback invoked by Mongoose to signal that a poll is needed soon.
 * Since we're in a tight polling loop anyway (see below), we don't need to do
 * anything.
 */
void mg_lwip_mgr_schedule_poll(struct mg_mgr *mgr) {
}

// Define an event handler function
void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  if (ev == MG_EV_POLL) return;
  /* printf("ev %d\r\n", ev); */
  switch (ev) {
    case MG_EV_ACCEPT: {
      char addr[32];
      mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr),
                          MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
      printf("%p: Connection from %s\r\n", nc, addr);
      break;
    }
    case MG_EV_HTTP_REQUEST: {
      struct http_message *hm = (struct http_message *) ev_data;
      char addr[32];
      mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr),
                          MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
      printf("%p: %.*s %.*s\r\n", nc, (int) hm->method.len, hm->method.p,
             (int) hm->uri.len, hm->uri.p);
      mg_send_response_line(nc, 200,
                            "Content-Type: text/html\r\n"
                            "Connection: close");
      mg_printf(nc,
                "\r\n<h1>Hello, %s!</h1>\r\n"
                "You asked for %.*s\r\n",
                addr, (int) hm->uri.len, hm->uri.p);
      nc->flags |= MG_F_SEND_AND_CLOSE;
      LEDS_INVERT(LED_THREE);
      break;
    }
    case MG_EV_CLOSE: {
      printf("%p: Connection closed\r\n", nc);
      break;
    }
  }
}

/**
 * @brief Function for application main entry.
 */
int main(void) {
  cs_log_set_file(stdout);

  bleconfig_init();

  {
    struct mg_mgr mgr;

    mg_mgr_init(&mgr, NULL);  // Initialize event manager object

    // Note that many connections can be added to a single event manager
    // Connections can be created at any point, e.g. in event handler function
    const char *err;
    struct mg_bind_opts opts;
    struct mg_connection *nc = NULL;
    memset(&opts, 0x00, sizeof(opts));
    opts.error_string = &err;
    nc = mg_bind_opt(
        &mgr, "80", ev_handler,
        opts);  // Create listening connection and add it to the event manager
    if (nc == NULL) {
      printf("Failed to create listener: %s\n", err);
      return 1;
    }
    mg_set_protocol_http_websocket(nc);

    for (;;) {  // Start infinite event loop
      bleconfig_poll();
      mg_mgr_poll(&mgr, 0);
    }
  }
}
