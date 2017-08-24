/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 * This software is dual-licensed: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. For the terms of this
 * license, see <http://www.gnu.org/licenses/>.
 *
 * You are free to use this software under the terms of the GNU General
 * Public License, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * Alternatively, you can license this software under a commercial
 * license, as set out in <https://www.cesanta.com/license>.
 */

#include "mongoose.h"

static const char *s_mqtt_address = "0.0.0.0:1883";
static const char *s_http_address = "0.0.0.0:8080";

static void unproxy(struct mg_connection *c) {
  struct mg_connection *pc = (struct mg_connection *) c->user_data;
  if (pc != NULL) {
    pc->flags |= MG_F_CLOSE_IMMEDIATELY;
    pc->user_data = NULL;
    c->user_data = NULL;
  }
  printf("Closing connection %p\n", c);
}

static void proxy_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_POLL) return;
  printf("%p %s EVENT %d %p\n", c, __func__, ev, ev_data);
  switch (ev) {
    case MG_EV_CLOSE: {
      unproxy(c);
      break;
    }
    case MG_EV_RECV: {
      struct mg_connection *pc = (struct mg_connection *) c->user_data;
      if (pc != NULL) {
        mg_send_websocket_frame(pc, WEBSOCKET_OP_BINARY, c->recv_mbuf.buf,
                                c->recv_mbuf.len);
        mbuf_remove(&c->recv_mbuf, c->recv_mbuf.len);
      }
      break;
    }
  }
}

static void http_handler(struct mg_connection *c, int ev, void *ev_data) {
  struct mg_connection *pc = (struct mg_connection *) c->user_data;
  if (ev == MG_EV_POLL) return;
  printf("%p %s EVENT %d %p\n", c, __func__, ev, ev_data);
  /* Do your custom event processing here */
  switch (ev) {
    case MG_EV_WEBSOCKET_HANDSHAKE_DONE: {
      pc = mg_connect(c->mgr, s_mqtt_address, proxy_handler);
      pc->user_data = c;
      c->user_data = pc;
      printf("Created proxy connection %p\n", pc);
      break;
    }
    case MG_EV_WEBSOCKET_FRAME: {
      struct websocket_message *wm = (struct websocket_message *) ev_data;
      if (pc != NULL) {
        printf("Forwarding %d bytes\n", (int) wm->size);
        mg_send(pc, wm->data, wm->size);
      }
      break;
    }
    case MG_EV_CLOSE: {
      unproxy(c);
      break;
    }
  }
}

static void mqtt_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_POLL) return;
  printf("%p %s EVENT %d %p\n", c, __func__, ev, ev_data);
  /* Do your custom event processing here */
  switch (ev) {
    case MG_EV_CLOSE:
      printf("Closing MQTT connection %p\n", c);
      break;
  }
  mg_mqtt_broker(c, ev, ev_data);
}

static void start_mqtt_server(struct mg_mgr *mgr, const char *addr) {
  struct mg_connection *c;
  static struct mg_mqtt_broker brk;  // static is important - must not perish
  if ((c = mg_bind(mgr, addr, mqtt_handler)) == NULL) {
    fprintf(stderr, "Cannot start MQTT server on port [%s]\n", addr);
    exit(EXIT_FAILURE);
  }
  mg_mqtt_broker_init(&brk, NULL);
  c->user_data = &brk;
  mg_set_protocol_mqtt(c);
  printf("MQTT server started on %s\n", addr);
}

static void start_http_server(struct mg_mgr *mgr, const char *addr) {
  struct mg_connection *c;
  if ((c = mg_bind(mgr, addr, http_handler)) == NULL) {
    fprintf(stderr, "Cannot start HTTP server on port [%s]\n", addr);
    exit(EXIT_FAILURE);
  }
  mg_set_protocol_http_websocket(c);
  printf("HTTP server started on %s\n", addr);
}

int main(void) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr, NULL);
  start_http_server(&mgr, s_http_address);
  start_mqtt_server(&mgr, s_mqtt_address);
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
}
