/*
 * Copyright (c) 2015 Cesanta Software Limited
 * All rights reserved
 *
 * This program sends CoAP CON-message to server (coap.me by default)
 * and waits for answer.
 */

#include "mongoose.h"

static int s_time_to_exit = 0;
static char *s_default_address = "udp://coap.me:5683";

static void coap_handler(struct mg_connection *nc, int ev, void *p) {
  switch (ev) {
    case MG_EV_CONNECT: {
      struct mg_coap_message cm;
      uint32_t res;

      memset(&cm, 0, sizeof(cm));
      cm.msg_id = 1;
      cm.msg_type = MG_COAP_MSG_CON;
      printf("Sending CON...\n");
      res = mg_coap_send_message(nc, &cm);
      if (res == 0) {
        printf("Sent CON with msg_id = %d\n", cm.msg_id);
      } else {
        printf("Error: %d\n", res);
        s_time_to_exit = 1;
      }
      break;
    }
    case MG_EV_COAP_ACK:
    case MG_EV_COAP_RST: {
      struct mg_coap_message *cm = (struct mg_coap_message *) p;
      printf("ACK/RST for message with msg_id = %d received\n", cm->msg_id);
      s_time_to_exit = 1;
      break;
    }
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  char *address = s_default_address;

  if (argc > 1) {
    address = argv[1];
  }

  printf("Using %s as CoAP server\n", address);

  mg_mgr_init(&mgr, 0);

  nc = mg_connect(&mgr, address, coap_handler);
  if (nc == NULL) {
    printf("Unable to connect to %s\n", address);
    return -1;
  }

  mg_set_protocol_coap(nc);

  while (!s_time_to_exit) {
    mg_mgr_poll(&mgr, 1000000);
  }

  mg_mgr_free(&mgr);

  return 0;
}
